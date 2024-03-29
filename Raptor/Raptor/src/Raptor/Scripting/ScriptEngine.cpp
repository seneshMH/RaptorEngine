#include "rtpch.h"
#include "ScriptEngine.h"

#include "Raptor/Scene/Scene.h"
#include "Raptor/Scene/Entity.h"
#include "Raptor/Core/Timestep.h"
#include "Raptor/Core/Application.h"
#include "Raptor/Core/Timer.h"
#include "Raptor/Core/Buffer.h"
#include "Raptor/Core/FileSystem.h"

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/object.h"
#include "mono/metadata/tabledefs.h"
#include "mono/metadata/mono-debug.h"
#include "mono/metadata/threads.h"

#include "ScriptGlue.h"

#include "glm/vec3.hpp"
#include "FileWatch.h"

namespace Raptor {

	static std::unordered_map<std::string, ScriptFieldType> s_ScriptFieldTypeMap = {
		{"System.Single",	ScriptFieldType::Float},
		{"System.Double",	ScriptFieldType::Double},
		{"System.Boolean",	ScriptFieldType::Bool},
		{"System.Char",		ScriptFieldType::Char},
		{"System.Int16",	ScriptFieldType::Short},
		{"System.Int32",	ScriptFieldType::Int},
		{"System.Int64",	ScriptFieldType::Long},
		{"System.Byte",		ScriptFieldType::Byte},
		{"System.UInt16",	ScriptFieldType::UShort},
		{"System.UInt32",	ScriptFieldType::UInt},
		{"System.UInt64",	ScriptFieldType::ULong},
		{"Raptor.Entity",	ScriptFieldType::Entity},
		{"Raptor.Vector2",	ScriptFieldType::Vector2},
		{"Raptor.Vector3",	ScriptFieldType::Vector3},
		{"Raptor.Vector4",	ScriptFieldType::Vector4},
	};

	namespace Utils {

		static MonoAssembly* LoadMonoAssembly(const std::filesystem::path& assemblyPath,bool loadPdb = false)
		{
			
			ScopedBuffer fileData = FileSystem::ReadFileBinary(assemblyPath);
			
			// NOTE: We can't use this image for anything other than loading the assembly because this image doesn't have a reference to the assembly
			MonoImageOpenStatus status;
			MonoImage* image = mono_image_open_from_data_full(fileData.As<char>(), fileData.Size(), 1, &status, 0);

			if (status != MONO_IMAGE_OK)
			{
				const char* errorMessage = mono_image_strerror(status);
				// Log some error message using the errorMessage data
				return nullptr;
			}

			if (loadPdb)
			{
				std::filesystem::path pdbPath = assemblyPath;
				pdbPath.replace_extension(".pdb");
				RT_CORE_WARN("Trying to load PDB : {}", pdbPath);

				if (std::filesystem::exists(pdbPath))
				{
					ScopedBuffer pdbFileData = FileSystem::ReadFileBinary(pdbPath);

					mono_debug_open_image_from_memory(image, pdbFileData.As<const mono_byte>(), pdbFileData.Size());
					RT_CORE_INFO("Loaded PDB : {}", pdbPath);
				}
			}


			std::string string = assemblyPath.string();
			MonoAssembly* assembly = mono_assembly_load_from_full(image, string.c_str(), &status, 0);
			mono_image_close(image);

			return assembly;
		}

		void PrintAssemblyTypes(MonoAssembly* assembly)
		{
			MonoImage* image = mono_assembly_get_image(assembly);
			const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
			int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

			for (int32_t i = 0; i < numTypes; i++)
			{
				uint32_t cols[MONO_TYPEDEF_SIZE];
				mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

				const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
				const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

				printf("%s.%s\n", nameSpace, name);
			}
		}

		ScriptFieldType MonoTypeToScriptFieldType(MonoType* monoType)
		{
			std::string typeName = mono_type_get_name(monoType);

			auto it = s_ScriptFieldTypeMap.find(typeName);
			if (it == s_ScriptFieldTypeMap.end())
			{
				RT_CORE_ERROR("Unknown Type {}",typeName);
				return ScriptFieldType::None;
			}
			return it->second;
		}
	
	}

	struct ScriptEngineData
	{
		MonoDomain* RootDomain = nullptr;
		MonoDomain* AppDomain = nullptr;

		MonoAssembly* CoreAssembly = nullptr;
		MonoImage* CoreAssemblyImage = nullptr;

		MonoAssembly* AppAssembly = nullptr;
		MonoImage* AppAssemblyImage = nullptr;

		std::filesystem::path CoreAssemblyFilePath;
		std::filesystem::path AppAssemblyFilePath;

		ScriptClass EntityClass;

		std::unordered_map<std::string, Ref<ScriptClass>> EntityClasses;
		std::unordered_map<UUID, Ref<ScriptInstance>> EntityInstances;
		std::unordered_map<UUID, ScriptFieldMap> EntityScriptFields;

		Scope<filewatch::FileWatch<std::string>> AppAsembleyFileWatcher;

		bool AssemblyReloadPending = false;

#ifdef RT_DEBUG
		bool EnableDebuging = true;
#else
		bool EnableDebuging = false;
#endif // RT_DEBUG

		Scene* SceneContext = nullptr;
		Timer ReloadTimer;
	};


	static ScriptEngineData* s_Data = nullptr;

	void ScriptEngine::Init()
	{
		s_Data = new ScriptEngineData();

		InitMono();
		ScriptGlue::RegisterFunctions();

		bool status = LoadAssembly("resources/scripts/Raptor-ScriptCore.dll");
		if (!status)
		{
			RT_CORE_ERROR("[ScriptEngine] could not load core assembly");
			return;
		}

		status = LoadAppAssembly("SandboxProject/Assets/Scripts/Binaries/Sandbox.dll");
		if (!status)
		{
			RT_CORE_ERROR("[ScriptEngine] could not load app assembly");
			return;
		}

		LoadAssemblyClasses();
		ScriptGlue::RegisterComponents();

		s_Data->EntityClass = ScriptClass("Raptor", "Entity",true);

	}

	void ScriptEngine::Shutdown()
	{
		ShutdownMono();
		delete s_Data;
	}

	void ScriptEngine::InitMono()
	{
		mono_set_assemblies_path("../mono/lib");

		if (s_Data->EnableDebuging)
		{
			const char* argv[2] = {
				"--debugger-agent=transport=dt_socket,address=127.0.0.1:2550,server=y,suspend=n,loglevel=3,logfile=monoDebugger.log",
				"--soft-breakpoints"
			};

			mono_jit_parse_options(2, (char**)argv);
			mono_debug_init(MONO_DEBUG_FORMAT_MONO);
		}

		MonoDomain* rootDomain = mono_jit_init("RaptorJitRuntime");
		RT_CORE_ASSERT(rootDomain);

		s_Data->RootDomain = rootDomain;

		if (s_Data->EnableDebuging)
			mono_debug_domain_create(s_Data->RootDomain);
	
		mono_thread_set_main(mono_thread_current());
	}

	void ScriptEngine::ShutdownMono()
	{
		mono_domain_set(mono_get_root_domain(), false);

		mono_domain_unload(s_Data->AppDomain);
		s_Data->AppDomain = nullptr;
		mono_jit_cleanup(s_Data->RootDomain);
		s_Data->RootDomain = nullptr;
	}

	bool ScriptEngine::EntityClassExists(const std::string& fullClassName)
	{
		return s_Data->EntityClasses.find(fullClassName) != s_Data->EntityClasses.end();
	}

	bool ScriptEngine::LoadAssembly(const std::filesystem::path& filepath)
	{
		s_Data->AppDomain = mono_domain_create_appdomain("RaptorScriptRuntime", nullptr);
		mono_domain_set(s_Data->AppDomain, true);

		s_Data->CoreAssembly = Utils::LoadMonoAssembly(filepath, s_Data->EnableDebuging);
		if (s_Data->CoreAssembly == nullptr)
			return false;

		s_Data->CoreAssemblyFilePath = filepath;
		s_Data->CoreAssemblyImage = mono_assembly_get_image(s_Data->CoreAssembly);
		return true;
	}

	static void OnAppAssemblyFileSystemEvent(const std::string& path, const filewatch::Event change_type)
	{
		if (!s_Data->AssemblyReloadPending && change_type == filewatch::Event::modified)
		{
			s_Data->AssemblyReloadPending = true;

			s_Data->ReloadTimer = Timer();

			Application::Get().SubmitToMainThread([]() 
				{ 
					s_Data->AppAsembleyFileWatcher.reset();
					ScriptEngine::RelaodAssembly();
				});
		}
	}

	bool ScriptEngine::LoadAppAssembly(const std::filesystem::path& filepath)
	{
		s_Data->AppAssemblyFilePath = filepath;
		s_Data->AppAssembly = Utils::LoadMonoAssembly(filepath,s_Data->EnableDebuging);
		if (s_Data->AppAssembly == nullptr)
			return false;

		s_Data->AppAssemblyImage = mono_assembly_get_image(s_Data->AppAssembly);
		s_Data->AppAsembleyFileWatcher = CreateScope<filewatch::FileWatch<std::string>>(filepath.string(), OnAppAssemblyFileSystemEvent);
		s_Data->AssemblyReloadPending = false;
		return true;
	}

	void ScriptEngine::RelaodAssembly()
	{
		RT_CORE_WARN("Reloading took {}ms",s_Data->ReloadTimer.Elapsed());

		mono_domain_set(mono_get_root_domain(), false);

		mono_domain_unload(s_Data->AppDomain);

		LoadAssembly(s_Data->CoreAssemblyFilePath);
		LoadAppAssembly(s_Data->AppAssemblyFilePath);

		LoadAssemblyClasses();

		s_Data->EntityClass = ScriptClass("Raptor", "Entity", true);
		ScriptGlue::RegisterComponents();
	}

	void ScriptEngine::OnRuntimeStart(Scene* scene)
	{
		s_Data->SceneContext = scene;
	}

	void ScriptEngine::OnRuntimeStop()
	{
		s_Data->SceneContext = nullptr;
		s_Data->EntityInstances.clear();
	}

	Ref<ScriptInstance> ScriptEngine::GetEntityScriptInstance(UUID entityId)
	{
		auto it = s_Data->EntityInstances.find(entityId);
		if (it == s_Data->EntityInstances.end())
			return nullptr;

		return it->second;
	}

	Ref<ScriptClass> ScriptEngine::GetEntityClass(const std::string& name)
	{
		if (s_Data->EntityClasses.find(name) == s_Data->EntityClasses.end())
			return nullptr;

		return s_Data->EntityClasses.at(name);
	}

	std::unordered_map<std::string, Ref<ScriptClass>> ScriptEngine::GetEntityClasses()
	{
		return s_Data->EntityClasses;
	}

	ScriptFieldMap& ScriptEngine::GetScriptFieldMap(Entity entity)
	{
		RT_CORE_ASSERT(entity);

		UUID entityID = entity.GetUUID();
		
		return s_Data->EntityScriptFields[entityID];
	}

	MonoObject* ScriptEngine::GetManagedInstance(UUID uuid)
	{
		RT_CORE_ASSERT(s_Data->EntityInstances.find(uuid) != s_Data->EntityInstances.end());
		return s_Data->EntityInstances.at(uuid)->GetMangedObject();
	}

	void ScriptEngine::LoadAssemblyClasses()
	{
		s_Data->EntityClasses.clear();

		const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(s_Data->AppAssemblyImage, MONO_TABLE_TYPEDEF);
		int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

		MonoClass* entityClass = mono_class_from_name(s_Data->CoreAssemblyImage, "Raptor", "Entity");

		for (int32_t i = 0; i < numTypes; i++)
		{
			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

			const char* nameSpace = mono_metadata_string_heap(s_Data->AppAssemblyImage, cols[MONO_TYPEDEF_NAMESPACE]);
			const char* className = mono_metadata_string_heap(s_Data->AppAssemblyImage, cols[MONO_TYPEDEF_NAME]);

			std::string fullName;
			if (strlen(nameSpace) != 0)
			{
				fullName = fmt::format("{}.{}",nameSpace,className);
			}
			else
			{
				fullName = className;
			}

			MonoClass* monoClass = mono_class_from_name(s_Data->AppAssemblyImage, nameSpace, className);
			
			if (monoClass == entityClass)
				continue;
			
			bool isEntity = mono_class_is_subclass_of(monoClass, entityClass, false);

			if (!isEntity)
				continue;

			Ref<ScriptClass> scriptClass = CreateRef<ScriptClass>(nameSpace, className);

			s_Data->EntityClasses[fullName] = scriptClass;
			
			int fieldCount = mono_class_num_fields(monoClass);
			RT_CORE_WARN("{} has {}", className, fieldCount);
			void* iterator = nullptr;
			while (MonoClassField* field = mono_class_get_fields(monoClass,&iterator))
			{
				const char* fieldName = mono_field_get_name(field);
				uint32_t flags = mono_field_get_flags(field);
				if (flags & FIELD_ATTRIBUTE_PUBLIC)
				{
					MonoType* type = mono_field_get_type(field);
					ScriptFieldType fieldType = Utils::MonoTypeToScriptFieldType(type);
					RT_CORE_WARN("{} ({})",fieldName, Utils::ScriptFieldTypeToString(fieldType));
				
					scriptClass->m_Fields[fieldName] = { fieldType,fieldName,field };
				}
			}
		}
	}

	MonoObject* ScriptEngine::InstantiateClass(MonoClass* monoClass)
	{
		MonoObject* instance = mono_object_new(s_Data->AppDomain, monoClass);
		mono_runtime_object_init(instance);

		return instance;
	}

	void ScriptEngine::OnCreateEntity(Entity entity)
	{
		const auto& sc = entity.GetComponent<ScriptComponent>();

		if (ScriptEngine::EntityClassExists(sc.ClassName))
		{
			UUID entityID = entity.GetUUID();

			Ref<ScriptInstance> instance = CreateRef<ScriptInstance>(s_Data->EntityClasses[sc.ClassName],entity);
			s_Data->EntityInstances[entityID] = instance;

			if (s_Data->EntityScriptFields.find(entityID) != s_Data->EntityScriptFields.end())
			{
				const ScriptFieldMap& fieldMap = s_Data->EntityScriptFields.at(entityID);

				for (const auto& [name, fieldInstance] : fieldMap)
				{
					instance->SetFieldValueInternal(name, fieldInstance.m_Buffer);
				}
			}
			instance->InvokeOnCreate();
		}
	}

	MonoImage* ScriptEngine::GetCoreAssemblyImage()
	{
		return s_Data->CoreAssemblyImage;
	}

	Scene* ScriptEngine::GetSceneContext()
	{
		return s_Data->SceneContext;
	}

	void ScriptEngine::OnUpdateEntity(Entity entity, Timestep ts)
	{
		UUID entityUUID = entity.GetUUID();
		if (s_Data->EntityInstances.find(entityUUID) != s_Data->EntityInstances.end())
		{
			Ref<ScriptInstance> instance = s_Data->EntityInstances[entityUUID];
			instance->InvokeOnUpdate((float)ts);
		}
		else
		{
			RT_CORE_ERROR("Could not find entity script instance {}",entity.GetName());
		}
		
	}

	ScriptClass::ScriptClass(const std::string& classNamespace, const std::string& className, bool isCore)
		:m_ClassNamespace(classNamespace), m_ClassName(className)
	{
		m_MonoClass = mono_class_from_name(isCore ? s_Data->CoreAssemblyImage : s_Data->AppAssemblyImage, classNamespace.c_str(), className.c_str());
	}

	MonoObject* ScriptClass::Instantiate()
	{
		return ScriptEngine::InstantiateClass(m_MonoClass);
	}

	MonoMethod* ScriptClass::GetMethod(const std::string& name, int parameterCount)
	{
		return mono_class_get_method_from_name(m_MonoClass, name.c_str(), parameterCount);
	}

	MonoObject* ScriptClass::InvokeMethod(MonoObject* instance, MonoMethod* method, void** params)
	{
		MonoObject* exception = nullptr;
		return mono_runtime_invoke(method, instance, params, &exception);
	}

	ScriptInstance::ScriptInstance(Ref<ScriptClass> scriptClass, Entity entity)
		:m_ScriptClass(scriptClass)
	{
		m_Instance = scriptClass->Instantiate();

		m_Contructor = s_Data->EntityClass.GetMethod(".ctor",1);
		m_OnCreateMethod = scriptClass->GetMethod("OnCreate",0);
		m_OnUpdateMethod = scriptClass->GetMethod("OnUpdate",1);

		{
			UUID entityID = entity.GetUUID();
			void* param = &entityID;
			m_ScriptClass->InvokeMethod(m_Instance, m_Contructor, &param);
		}
	}

	void ScriptInstance::InvokeOnCreate()
	{
		if(m_OnCreateMethod)
			m_ScriptClass->InvokeMethod(m_Instance, m_OnCreateMethod);
	}

	void ScriptInstance::InvokeOnUpdate(float ts)
	{
		if (m_OnUpdateMethod)
		{
			void* param = &ts;
			m_ScriptClass->InvokeMethod(m_Instance, m_OnUpdateMethod, &param);
		}
	}

	bool ScriptInstance::GetFieldValueInternal(const std::string& name, void* buffer)
	{
		const auto& fields = m_ScriptClass->GetFields();
		auto it = fields.find(name);

		if (it == fields.end())
			return false;

		const ScriptField& field = it->second;
		mono_field_get_value(m_Instance, field.ClassField, buffer);
		return true;
	}

	bool ScriptInstance::SetFieldValueInternal(const std::string& name, const void* value)
	{
		const auto& fields = m_ScriptClass->GetFields();
		auto it = fields.find(name);

		if (it == fields.end())
			return false;

		const ScriptField& field = it->second;
		mono_field_set_value(m_Instance, field.ClassField, (void*)value);

		return true;
	}

}
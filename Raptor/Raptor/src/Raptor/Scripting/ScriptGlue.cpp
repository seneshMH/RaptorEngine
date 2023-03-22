#include "rtpch.h"
#include "ScriptGlue.h"
#include "ScriptEngine.h"

#include "Raptor/Core/UUID.h"
#include "Raptor/Scene/Scene.h"
#include "Raptor/Scene/Entity.h"
#include "Raptor/Core/KeyCodes.h"
#include "Raptor/Core/Input.h"

#include "mono/metadata/object.h"
#include "mono/metadata/reflection.h"
#include "box2d/b2_body.h"

namespace Raptor {

	static std::unordered_map<MonoType*, std::function<bool(Entity)>> s_EntityHasComponentFuncs;

#define RT_ADD_INTERNAL_CALL(Name) mono_add_internal_call("Raptor.InternalCalls::" #Name, Name)

	static void NativeLog(MonoString* string, int value)
	{
		char* cStr = mono_string_to_utf8(string);
		std::string str(cStr);
		mono_free(cStr);

		std::cout << str << value << "\n";
	}

	static void NativeLog_Vector(glm::vec3* parameter, glm::vec3* outResult)
	{
		RT_CORE_WARN(*parameter);

		*outResult = glm::normalize(*parameter);
	}

	static bool Entity_HasComponent(UUID entityID, MonoReflectionType* componentType)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		RT_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		RT_CORE_ASSERT(entity);

		MonoType* managedType = mono_reflection_type_get_type(componentType);
		RT_CORE_ASSERT(s_EntityHasComponentFuncs.find(managedType) != s_EntityHasComponentFuncs.end());
		return s_EntityHasComponentFuncs.at(managedType)(entity);
	}

	static void TransformComponent_GetTranslation(UUID entityID, glm::vec3* outTrnslation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		RT_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		RT_CORE_ASSERT(entity);

		*outTrnslation = entity.GetComponent<TransformComponent>().Translation;
	}

	static void TransformComponent_SetTranslation(UUID entityID, glm::vec3* trnslation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		RT_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		RT_CORE_ASSERT(entity);

		entity.GetComponent<TransformComponent>().Translation = *trnslation;
	}

	static void Rigidbody2DComponent_ApplyLinearImpulse(UUID entityID, glm::vec2* impulse, glm::vec2* point, bool wake)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		RT_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		RT_CORE_ASSERT(entity);

		auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
		b2Body* body = (b2Body*)rb2d.RuntimeBody;
		body->ApplyLinearImpulse(b2Vec2{ impulse->x,impulse->y }, b2Vec2{ point->x,point->y }, wake);
	}

	static void Rigidbody2DComponent_ApplyLinearImpulseToCenter(UUID entityID, glm::vec2* impulse, bool wake)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		RT_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		RT_CORE_ASSERT(entity);

		auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
		b2Body* body = (b2Body*)rb2d.RuntimeBody;
		body->ApplyLinearImpulseToCenter(b2Vec2{ impulse->x,impulse->y }, wake);
	}

	static bool Input_IsKeyDown(KeyCode keycode)
	{
		return Input::IsKeyPressed(keycode);
	}

	void ScriptGlue::RegisterFunctions()
	{
		RT_ADD_INTERNAL_CALL(NativeLog);
		RT_ADD_INTERNAL_CALL(NativeLog_Vector);

		RT_ADD_INTERNAL_CALL(Entity_HasComponent);
		RT_ADD_INTERNAL_CALL(TransformComponent_GetTranslation);
		RT_ADD_INTERNAL_CALL(TransformComponent_SetTranslation);
		RT_ADD_INTERNAL_CALL(Rigidbody2DComponent_ApplyLinearImpulse);
		RT_ADD_INTERNAL_CALL(Rigidbody2DComponent_ApplyLinearImpulseToCenter);

		RT_ADD_INTERNAL_CALL(Input_IsKeyDown);

	}

	template<typename... Component>
	static void RegisterComponent()
	{
		([]()
			{
				std::string_view typeName = typeid(Component).name();
				size_t pos = typeName.find_last_of(":");
				std::string_view structName = typeName.substr(pos + 1);
				std::string managedTypename = fmt::format("Raptor.{}", structName);

				MonoType* managedType = mono_reflection_type_from_name(managedTypename.data(), ScriptEngine::GetCoreAssemblyImage());
				if (!managedType)
				{
					RT_CORE_ERROR("Could not find component type {}", managedTypename);
					return;
				}
				s_EntityHasComponentFuncs[managedType] = [](Entity entity) {return entity.HasComponent<Component>(); };
			}
		(), ...);
	}

	template<typename... Component>
	static void RegisterComponent(ComponentGroup<Component...>)
	{
		RegisterComponent<Component...>();
	}

	void ScriptGlue::RegisterComponents()
	{
		RegisterComponent(AllComponents{});
	}
}

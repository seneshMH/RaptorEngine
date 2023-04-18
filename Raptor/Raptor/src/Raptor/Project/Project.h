#pragma once
#include "Raptor/Core/Base.h"

#include <string>
#include <filesystem>

namespace Raptor {
	struct ProjectConfig
	{
		std::string Name = "Untitled";
		std::filesystem::path StartScene;

		std::filesystem::path AssestDirectory;
		std::filesystem::path ScriptModulePath;
	};

	class Project {
	public:

		static const std::filesystem::path& GetProjectDirectory()
		{
			RT_CORE_ASSERT(s_ActiveProject);
			return s_ActiveProject->m_ProjectDirectory;
		}

		static std::filesystem::path GetAssetsDirectory()
		{
			RT_CORE_ASSERT(s_ActiveProject);
			return GetProjectDirectory() / s_ActiveProject->m_Config.AssestDirectory;
		}

		static std::filesystem::path GetAssetFileSystemPath(const std::filesystem::path& path)
		{
			RT_CORE_ASSERT(s_ActiveProject);
			return GetAssetsDirectory() / path;
		}

		ProjectConfig& GetConfig() { return m_Config; }

		static Ref<Project> GetActive() { return s_ActiveProject; }

		static Ref<Project> New();
		static Ref<Project> Load(const std::filesystem::path& path);
		static bool SaveActive(const std::filesystem::path& path);
	
	private:
		ProjectConfig m_Config;
		std::filesystem::path m_ProjectDirectory;

		inline static Ref<Project> s_ActiveProject;
	};
}
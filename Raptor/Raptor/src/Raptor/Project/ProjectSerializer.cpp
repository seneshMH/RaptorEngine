#include "rtpch.h"
#include "ProjectSerializer.h"

#define YAML_CPP_STATIC_DEFINE
#include "yaml-cpp/yaml.h"
#include <fstream>

namespace Raptor {

	ProjectSerializer::ProjectSerializer(Ref<Project> project)
		:m_Project(project)
	{
	}

	bool ProjectSerializer::Serialize(const std::filesystem::path& filePath)
	{
		const auto& config = m_Project->GetConfig();

		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Project" << YAML::Value;
		
		out << YAML::BeginMap;
		out << YAML::Key << "Name" << YAML::Value << config.Name;
		out << YAML::Key << "StartScene" << YAML::Value << config.StartScene.string();
		out << YAML::Key << "AssestDirectory" << YAML::Value << config.AssestDirectory.string();
		out << YAML::Key << "ScriptModulePath" << YAML::Value << config.ScriptModulePath.string();
		out << YAML::EndMap;
		
		out << YAML::EndMap;

		std::ofstream fout(filePath);
		fout << out.c_str();

		return true;
	}

	bool ProjectSerializer::DeSerialize(const std::filesystem::path& filePath)
	{
		auto& config = m_Project->GetConfig();

		std::ifstream stream(filePath);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data;
		try
		{
			data = YAML::LoadFile(filePath.string());
		}
		catch (YAML::ParserException e)
		{
			RT_CORE_ERROR("Failed to load .rproj file '{0}'\n     {1}", filePath, e.what());
			return false;
		}

		auto projectNode = data["Project"];

		if (!projectNode)
			return false;

		config.Name = projectNode["Name"].as<std::string>();
		config.StartScene = projectNode["StartScene"].as<std::string>();
		config.AssestDirectory = projectNode["AssestDirectory"].as<std::string>();
		config.ScriptModulePath = projectNode["ScriptModulePath"].as<std::string>();
	
		return true;
	}

}
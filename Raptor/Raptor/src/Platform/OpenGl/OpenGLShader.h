#pragma once
#include "Raptor/Renderer/Shader.h"

#include <glm/glm.hpp>


typedef unsigned int GLenum;

namespace Raptor {
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string&  name,const std::string& vertexSrc, const std::string& fragmentSrc);
		OpenGLShader(const std::string& filepath);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void UnBind() const override;

		virtual const std::string& GetName() const override { return m_Name; };

		void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);

		void UploadUniformInt(const std::string& name, int value);

		void UploadUniformFloat(const std::string& name, float value);
		void UploadUniformFloat2(const std::string& name, const glm::vec2& values);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& values);
		void UploadUniformFloat4(const std::string& name, const glm::vec4& values);
	private:
		std::string ReadFile(const std::string& filepath);
		std::unordered_map < GLenum, std::string > Preprocess(const std::string& source);
		void Compile(const std::unordered_map < GLenum, std::string >& shaderSources);
	private:
		uint32_t m_RendererID;
		std::string m_Name;
	};
}
#include "rtpch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Raptor {

	struct Renderer2DStroge
	{
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> FlatColorShader;
	};

	static Renderer2DStroge* s_Data;

	void Renderer2D::Init()
	{
		s_Data = new Renderer2DStroge();
		s_Data->QuadVertexArray = VertexArray::Create();

		float squreVertices[3 * 4] = {
			-0.5f,-0.5f, 0.0f,
			 0.5f,-0.5f, 0.0f,
			 0.5f, 0.5f, 0.0f,
			-0.5f, 0.5f, 0.0f
		};

		Ref<VertexBuffer> squreVB;
		squreVB.reset(VertexBuffer::Create(squreVertices, sizeof(squreVertices)));

		BufferLayout squreVBLayout = {
			{ShaderDataType::Float3,"a_Position"},
		};

		squreVB->SetLayout(squreVBLayout);

		s_Data->QuadVertexArray->AddvertexBuffer(squreVB);


		unsigned int squreIndices[6] = { 0,1,2,2,3,0 };

		Ref<IndexBuffer> squreIB;
		squreIB.reset(IndexBuffer::Create(squreIndices, sizeof(squreIndices) / sizeof(uint32_t)));

		s_Data->QuadVertexArray->SetIndexBuffer(squreIB);
		s_Data->FlatColorShader = Shader::Create("assets/shaders/FlatColor.glsl");
	}

	void Renderer2D::Shutdown()
	{
		delete s_Data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		s_Data->FlatColorShader->Bind();
		s_Data->FlatColorShader->SetMat4("u_ViewProjection",camera.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x,position.y,0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		s_Data->FlatColorShader->Bind();
		s_Data->FlatColorShader->SetFloat4("uColor", color);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f),{size.x,size.y,1.0f});

		s_Data->FlatColorShader->SetMat4("u_Transform", transform);
		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

}
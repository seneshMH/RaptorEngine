#include "rtpch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"
#include "UniformBuffer.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Raptor {

	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoords;
		float TexIndex;
		float TilingFactor;

		int EntityID;
	};

	struct CircleVertex
	{
		glm::vec3 WorldPosition;
		glm::vec3 LocalPosition;

		glm::vec4 Color;
		float Thickness;
		float Fade;

		int EntityID;
	};


	struct LineVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;

		int EntityID;
	};

	struct Renderer2DData
	{
		static const uint32_t MaxQuads = 20000;
		static const uint32_t MaxVertices = MaxQuads * 4;
		static const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32; //TODO: Fix

		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> QuadShader;
		Ref<Texture2D> WhiteTexture;

		Ref<VertexArray> CircleVertexArray;
		Ref<VertexBuffer> CircleVertexBuffer;
		Ref<Shader> CircleShader;

		Ref<VertexArray> LineVertexArray;
		Ref<VertexBuffer> LineVertexBuffer;
		Ref<Shader> LineShader;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferptr = nullptr;

		uint32_t CircleIndexCount = 0;
		CircleVertex* CircleVertexBufferBase = nullptr;
		CircleVertex* CircleVertexBufferptr = nullptr;

		uint32_t LineVertexCount = 0;
		LineVertex* LineVertexBufferBase = nullptr;
		LineVertex* LineVertexBufferptr = nullptr;

		float LineWidth = 2.0f;


		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1; //0 is white texture

		glm::vec4 QuadVertexPosition[4];

		Renderer2D::Statistics stats;

		struct CameraData
		{
			glm::mat4 ViewProjection;
		};
		CameraData CameraBuffer;
		Ref<UniformBuffer> CameraUniformBuffer;
	};



	static Renderer2DData s_Data;

	void Renderer2D::Init()
	{
		RT_PROFILE_FUNCTION();

		s_Data.QuadVertexArray = VertexArray::Create();
		s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
		s_Data.QuadVertexBuffer->SetLayout({
			{ShaderDataType::Float3,"a_Position"},
			{ShaderDataType::Float4,"a_Color"},
			{ShaderDataType::Float2,"a_TexCoord"},
			{ShaderDataType::Float,"a_TexIndex"},
			{ShaderDataType::Float,"a_TilingFactor"},
			{ShaderDataType::Int,"a_EntityID"}
			});

		s_Data.QuadVertexArray->AddvertexBuffer(s_Data.QuadVertexBuffer);
		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

		uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
		s_Data.QuadVertexArray->SetIndexBuffer(quadIB);

		delete[] quadIndices;

		//Circles
		s_Data.CircleVertexArray = VertexArray::Create();
		s_Data.CircleVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(CircleVertex));
		s_Data.CircleVertexBuffer->SetLayout({
			{ShaderDataType::Float3,"a_WorldPosition"},
			{ShaderDataType::Float3,"a_LocalPosition"},
			{ShaderDataType::Float4,"a_Color"},
			{ShaderDataType::Float,"a_Thickness"},
			{ShaderDataType::Float,"a_Fade"},
			{ShaderDataType::Int,"a_EntityID"}
			});

		s_Data.CircleVertexArray->AddvertexBuffer(s_Data.CircleVertexBuffer);
		s_Data.CircleVertexArray->SetIndexBuffer(quadIB);
		s_Data.CircleVertexBufferBase = new CircleVertex[s_Data.MaxVertices];


		//Lines
		s_Data.LineVertexArray = VertexArray::Create();
		s_Data.LineVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(LineVertex));
		s_Data.LineVertexBuffer->SetLayout({
			{ShaderDataType::Float3,"a_Position"},
			{ShaderDataType::Float4,"a_Color"},
			{ShaderDataType::Int,"a_EntityID"}
			});

		s_Data.LineVertexArray->AddvertexBuffer(s_Data.LineVertexBuffer);
		s_Data.LineVertexBufferBase = new LineVertex[s_Data.MaxVertices];

		s_Data.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTexture = 0xffffffff;
		s_Data.WhiteTexture->SetData(&whiteTexture, sizeof(uint32_t));

		int32_t samplers[s_Data.MaxTextureSlots];
		for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
		{
			samplers[i] = i;
		}

		s_Data.QuadShader = Shader::Create("assets/shaders/Renderer2DQuad.glsl");
		s_Data.CircleShader = Shader::Create("assets/shaders/Renderer2DCircle.glsl");
		s_Data.LineShader = Shader::Create("assets/shaders/Renderer2DLine.glsl");


		s_Data.TextureSlots[0] = s_Data.WhiteTexture;

		s_Data.QuadVertexPosition[0] = { -0.5f, -0.5f,  0.0f,  1.0f };
		s_Data.QuadVertexPosition[1] = { 0.5f, -0.5f,  0.0f,  1.0f };
		s_Data.QuadVertexPosition[2] = { 0.5f,  0.5f,  0.0f,  1.0f };
		s_Data.QuadVertexPosition[3] = { -0.5f,  0.5f,  0.0f,  1.0f };
	
		s_Data.CameraUniformBuffer = UniformBuffer::Create(sizeof(Renderer2DData::CameraData), 0);
	}

	void Renderer2D::Shutdown()
	{
		RT_PROFILE_FUNCTION();

		delete[] s_Data.QuadVertexBufferBase;
	}

	void Renderer2D::StartBatch()
	{
		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferptr = s_Data.QuadVertexBufferBase;

		s_Data.CircleIndexCount = 0;
		s_Data.CircleVertexBufferptr = s_Data.CircleVertexBufferBase;

		s_Data.LineVertexCount = 0;
		s_Data.LineVertexBufferptr = s_Data.LineVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
	}

	
	void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform)
	{
		RT_PROFILE_FUNCTION();

		s_Data.CameraBuffer.ViewProjection = camera.GetProjection() * glm::inverse(transform);
		s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer2DData::CameraData));

		StartBatch();
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		RT_PROFILE_FUNCTION();

		s_Data.CameraBuffer.ViewProjection = camera.GetViewProjectionMatrix();
		s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer2DData::CameraData));

		StartBatch();
	}

	void Renderer2D::BeginScene(const EditorCamera& camera)
	{
		RT_PROFILE_FUNCTION();

		s_Data.CameraBuffer.ViewProjection = camera.GetViewProjection();
		s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer2DData::CameraData));

		StartBatch();
	}

	void Renderer2D::EndScene()
	{
		RT_PROFILE_FUNCTION();

		Flush();
	}

	void Renderer2D::Flush()
	{
		if (s_Data.QuadIndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferptr - (uint8_t*)s_Data.QuadVertexBufferBase);
			s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

			for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
			{
				s_Data.TextureSlots[i]->Bind(i);
			}

			s_Data.QuadShader->Bind();
			RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
			s_Data.stats.DrawCalls++;
		}

		if (s_Data.CircleIndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.CircleVertexBufferptr - (uint8_t*)s_Data.CircleVertexBufferBase);
			s_Data.CircleVertexBuffer->SetData(s_Data.CircleVertexBufferBase, dataSize);


			s_Data.CircleShader->Bind();
			RenderCommand::DrawIndexed(s_Data.CircleVertexArray, s_Data.CircleIndexCount);
			s_Data.stats.DrawCalls++;
		}

		if (s_Data.LineVertexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.LineVertexBufferptr - (uint8_t*)s_Data.LineVertexBufferBase);
			s_Data.LineVertexBuffer->SetData(s_Data.LineVertexBufferBase, dataSize);
		
			s_Data.LineShader->Bind();
			RenderCommand::SetLineWidth(s_Data.LineWidth);
			RenderCommand::DrawLines(s_Data.LineVertexArray, s_Data.LineVertexCount);
			s_Data.stats.DrawCalls++;
		}
	}

	void Renderer2D::FlushAndRest()
	{
		EndScene();

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferptr = s_Data.QuadVertexBufferBase;

		s_Data.CircleIndexCount = 0;
		s_Data.CircleVertexBufferptr = s_Data.CircleVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x,position.y,0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		RT_PROFILE_FUNCTION();

		constexpr size_t quadVertexCount = 4;

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
		{
			FlushAndRest();
		}

		const float textureIndex = 0.0f; //White texture

		constexpr glm::vec2 textureCoords[] = {
			{ 0.0f, 0.0f },
			{ 1.0f, 0.0f },
			{ 1.0f, 1.0f },
			{ 0.0f, 1.0f }
		};

		const float tilingFactor = 1.0f;

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadVertexBufferptr->Position = transform * s_Data.QuadVertexPosition[i];
			s_Data.QuadVertexBufferptr->Color = color;
			s_Data.QuadVertexBufferptr->TexCoords = textureCoords[i];
			s_Data.QuadVertexBufferptr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferptr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferptr++;
		}

		s_Data.QuadIndexCount += 6;

		s_Data.stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tililingFactor, const glm::vec4& tintColor)
	{
		DrawQuad({ position.x,position.y,0.0f }, size, texture, tililingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		RT_PROFILE_FUNCTION();

		constexpr size_t quadVertexCount = 4;

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
		{
			FlushAndRest();
		}

		float textureIndex = 0.0f;

		constexpr glm::vec2 textureCoords[] = {
			{ 0.0f, 0.0f },
			{ 1.0f, 0.0f },
			{ 1.0f, 1.0f },
			{ 0.0f, 1.0f }
		};

		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (*s_Data.TextureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			if (s_Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
				FlushAndRest();

			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });


		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadVertexBufferptr->Position = transform * s_Data.QuadVertexPosition[i];
			s_Data.QuadVertexBufferptr->Color = tintColor;
			s_Data.QuadVertexBufferptr->TexCoords = textureCoords[i];
			s_Data.QuadVertexBufferptr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferptr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferptr++;
		}


		s_Data.QuadIndexCount += 6;

		s_Data.stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawQuad({ position.x,position.y,0.0f }, size, subTexture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, float tilingFactor, const glm::vec4& tintColor)
	{
		RT_PROFILE_FUNCTION();

		constexpr size_t quadVertexCount = 4;

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
		{
			FlushAndRest();
		}

		float textureIndex = 0.0f;

		const glm::vec2* textureCoords = subTexture->GetTexCoords();
		const Ref<Texture2D> texture = subTexture->GetTexture();

		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (*s_Data.TextureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			if (s_Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
				FlushAndRest();

			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });


		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadVertexBufferptr->Position = transform * s_Data.QuadVertexPosition[i];
			s_Data.QuadVertexBufferptr->Color = tintColor;
			s_Data.QuadVertexBufferptr->TexCoords = textureCoords[i];
			s_Data.QuadVertexBufferptr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferptr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferptr++;
		}


		s_Data.QuadIndexCount += 6;

		s_Data.stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::mat4 transform, const glm::vec4& color, int entityID)
	{
		RT_PROFILE_FUNCTION();

		constexpr size_t quadVertexCount = 4;

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
		{
			FlushAndRest();
		}

		const float textureIndex = 0.0f; //White texture
		const float tilingFactor = 1.0f;

		constexpr glm::vec2 textureCoords[] = {
			{ 0.0f, 0.0f },
			{ 1.0f, 0.0f },
			{ 1.0f, 1.0f },
			{ 0.0f, 1.0f }
		};


		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadVertexBufferptr->Position = transform * s_Data.QuadVertexPosition[i];
			s_Data.QuadVertexBufferptr->Color = color;
			s_Data.QuadVertexBufferptr->TexCoords = textureCoords[i];
			s_Data.QuadVertexBufferptr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferptr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferptr->EntityID = entityID;
			s_Data.QuadVertexBufferptr++;
		}

		s_Data.QuadIndexCount += 6;

		s_Data.stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::mat4 transform, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor, int entityID)
	{
		RT_PROFILE_FUNCTION();

		constexpr size_t quadVertexCount = 4;

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
		{
			FlushAndRest();
		}

		float textureIndex = 0.0f;

		constexpr glm::vec2 textureCoords[] = {
			{ 0.0f, 0.0f },
			{ 1.0f, 0.0f },
			{ 1.0f, 1.0f },
			{ 0.0f, 1.0f }
		};

		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (*s_Data.TextureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			if (s_Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
				FlushAndRest();

			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadVertexBufferptr->Position = transform * s_Data.QuadVertexPosition[i];
			s_Data.QuadVertexBufferptr->Color = tintColor;
			s_Data.QuadVertexBufferptr->TexCoords = textureCoords[i];
			s_Data.QuadVertexBufferptr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferptr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferptr->EntityID = entityID;
			s_Data.QuadVertexBufferptr++;
		}

		s_Data.QuadIndexCount += 6;

		s_Data.stats.QuadCount++;
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DrawRotatedQuad({ position.x,position.y,0.0f }, size, rotation, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		RT_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f,0.0f,1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });

		DrawQuad(transform, color);

	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawRotatedQuad({ position.x,position.y,0.0f }, size, rotation, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		RT_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f,0.0f,1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });

		DrawQuad(transform, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subTexture, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawRotatedQuad({ position.x,position.y,0.0f }, size, rotation, subTexture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subTexture, float tilingFactor, const glm::vec4& tintColor)
	{
		RT_PROFILE_FUNCTION();

		constexpr size_t quadVertexCount = 4;

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
		{
			FlushAndRest();
		}

		float textureIndex = 0.0f;

		const glm::vec2* textureCoords = subTexture->GetTexCoords();
		const Ref<Texture2D> texture = subTexture->GetTexture();

		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (*s_Data.TextureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			if (s_Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
				FlushAndRest();

			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f,0.0f,1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadVertexBufferptr->Position = transform * s_Data.QuadVertexPosition[i];
			s_Data.QuadVertexBufferptr->Color = tintColor;
			s_Data.QuadVertexBufferptr->TexCoords = textureCoords[i];
			s_Data.QuadVertexBufferptr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferptr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferptr++;
		}

		s_Data.QuadIndexCount += 6;

		s_Data.stats.QuadCount++;
	}

	void Renderer2D::DrawCircle(const glm::mat4& transform, const glm::vec4& color, float thickness, float fade, int entityID)
	{
		RT_PROFILE_FUNCTION();

		//if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
		//{
			//FlushAndRest();
		//}


		for (size_t i = 0; i < 4; i++)
		{
			s_Data.CircleVertexBufferptr->WorldPosition = transform * s_Data.QuadVertexPosition[i];
			s_Data.CircleVertexBufferptr->LocalPosition = s_Data.QuadVertexPosition[i] * 2.0f;
			s_Data.CircleVertexBufferptr->Color = color;
			s_Data.CircleVertexBufferptr->Thickness = thickness;
			s_Data.CircleVertexBufferptr->Fade = fade;
			s_Data.CircleVertexBufferptr->EntityID = entityID;
			s_Data.CircleVertexBufferptr++;
		}

		s_Data.CircleIndexCount += 6;

		s_Data.stats.QuadCount++;
	}

	void Renderer2D::DrawLine(const glm::vec3& p1, const glm::vec3& p2, const glm::vec4& color, int entityID)
	{
		s_Data.LineVertexBufferptr->Position = p1;
		s_Data.LineVertexBufferptr->Color = color;
		s_Data.LineVertexBufferptr->EntityID = entityID;
		s_Data.LineVertexBufferptr++;

		s_Data.LineVertexBufferptr->Position = p2;
		s_Data.LineVertexBufferptr->Color = color;
		s_Data.LineVertexBufferptr->EntityID = entityID;
		s_Data.LineVertexBufferptr++;

		s_Data.LineVertexCount += 2;
	}

	void Renderer2D::DrawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, int entityID)
	{
		glm::vec3 p0 = glm::vec3(position.x - size.x * 0.5f, position.y - size.y * 0.5f, position.z);
		glm::vec3 p1 = glm::vec3(position.x + size.x * 0.5f, position.y - size.y * 0.5f, position.z);
		glm::vec3 p2 = glm::vec3(position.x + size.x * 0.5f, position.y + size.y * 0.5f, position.z);
		glm::vec3 p3 = glm::vec3(position.x - size.x * 0.5f, position.y + size.y * 0.5f, position.z);

		DrawLine(p0, p1, color,entityID);
		DrawLine(p1, p2, color, entityID);
		DrawLine(p2, p3, color, entityID);
		DrawLine(p3, p0, color, entityID);
	}

	void Renderer2D::DrawRect(const glm::mat4& transform, const glm::vec4& color, int entityID)
	{
		glm::vec3 lineVertices[4];
		for (size_t i = 0; i < 4; i++)
			lineVertices[i] = transform *  s_Data.QuadVertexPosition[i];

		DrawLine(lineVertices[0], lineVertices[1], color, entityID);
		DrawLine(lineVertices[1], lineVertices[2], color, entityID);
		DrawLine(lineVertices[2], lineVertices[3], color, entityID);
		DrawLine(lineVertices[3], lineVertices[0], color, entityID);
	}

	void Renderer2D::DrawSprite(const glm::mat4& transform, SpriteRendererComponent& src, int entityID)
	{
		if (src.Texture)
			DrawQuad(transform, src.Texture, src.TilingFactor, src.Color, entityID);
		else
			DrawQuad(transform, src.Color, entityID);
	}

	float Renderer2D::GetLineWidth()
	{
		return s_Data.LineWidth;
	}

	void Renderer2D::SetLineWidth(float width)
	{
		s_Data.LineWidth = width;
	}

	void Renderer2D::ResetStats()
	{
		memset(&s_Data.stats, 0, sizeof(Statistics));
	}

	Renderer2D::Statistics Renderer2D::GetStats()
	{
		return s_Data.stats;
	}

}
#pragma once
#include "Raptor/Core/Base.h"

namespace Raptor {

	enum class FramebufferTextureFormat
	{
		None = 0,

		//color
		RGBA8,

		//DEPTHSTENCIL
		DEPTH24STENCIL8,

		//Default
		Depth = DEPTH24STENCIL8
	};

	struct FramebufferTextureSpecification
	{
		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(FramebufferTextureFormat format)
			:TextureFormat(format) {}

		FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
	};

	struct FramebufferAttatchmentSpecification
	{
		FramebufferAttatchmentSpecification() = default;
		FramebufferAttatchmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments)
			:Attachments(attachments){}

		std::vector<FramebufferTextureSpecification> Attachments;
	};

	struct FrameBufferSpecification
	{
		uint32_t Width, Height;
		FramebufferAttatchmentSpecification Attachments;
		uint32_t Samples = 1;
		bool SwapChainTarget = false;
	};

	class FrameBuffer
	{
	public:

		virtual ~FrameBuffer() = default;

		virtual void Bind() = 0 ;
		virtual void UnBind() = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const = 0;

		virtual const FrameBufferSpecification& GetSpecification() const = 0;

		static Ref<FrameBuffer> Create(const FrameBufferSpecification& spec);
	};
}
#pragma once
#include "Raptor/Renderer/FrameBuffer.h"

namespace Raptor {
	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(const FrameBufferSpecification& spec);
		virtual ~OpenGLFrameBuffer();

		virtual void Bind() override;
		virtual void UnBind() override;

		virtual void Resize(uint32_t width, uint32_t height) override;
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) override;

		virtual void ClearAttachment(uint32_t attachmentIndex, int value) override;

		void Invalidate();

		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const override { RT_CORE_ASSERT(index < m_ColorAttachemnts.size()); return m_ColorAttachemnts[index]; }
		virtual const FrameBufferSpecification& GetSpecification() const override { return m_Specification; };

	private:
		uint32_t m_RendererID = 0;
		FrameBufferSpecification m_Specification;

		std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecs;
		FramebufferTextureSpecification m_DepthAttachmentSpec = FramebufferTextureFormat::None;

		std::vector<uint32_t> m_ColorAttachemnts;
		uint32_t m_DepthAttachment = 0;
	};
}

#pragma once
namespace Raptor {

	enum class RendererAPI
	{
		None   =   0,
		OpenGL =   1
	};

	class Renderer
	{
	public:
		inline static RendererAPI GetAPI() { return s_RendererApi; }
	private:
		static RendererAPI s_RendererApi;
	};
}
#pragma once
#include <filesystem>
#include "Raptor/Renderer/Texture.h"

namespace Raptor {

	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();

		void OnImGuiRender();
	private:
		std::filesystem::path m_CurrentDirectory;
		std::filesystem::path m_BaseDirectory;
		
		Ref<Texture2D> m_DirectoryIcon;
		Ref<Texture2D> m_FileIcon;
	};
}
#include "rtpch.h"
#include <imgui/imgui.h>
#include "ContentBrowserPanel.h"

#include "Raptor/Project/Project.h"

namespace Raptor {

	ContentBrowserPanel::ContentBrowserPanel()
		/* :m_BaseDirectory(Project::GetAssetsDirectory()), m_CurrentDirectory(m_BaseDirectory)*/
	{
		m_BaseDirectory = Project::GetAssetsDirectory();
		m_CurrentDirectory = m_BaseDirectory;


		m_DirectoryIcon = Texture2D::Create("resources/icons/contentBrowser/DirectoryIcon.png");
		m_FileIcon = Texture2D::Create("resources/icons/contentBrowser/FileIcon.png");
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("Content Browser");

		if (m_CurrentDirectory != std::filesystem::path(m_BaseDirectory))
		{
			if (ImGui::Button("<-"))
			{
				m_CurrentDirectory = m_CurrentDirectory.parent_path();
			}
		}

		static float padding = 16.0f;
		static float thumbnailSize = 128.0f;
		float cellSize = thumbnailSize + padding;

		float pannelWidth = ImGui::GetContentRegionAvail().x;
		int coloumCount = (int)(pannelWidth / cellSize);
		if (coloumCount < 1)
			coloumCount = 1;
		ImGui::Columns(coloumCount,0,false);

		for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
		{
			const auto& path = directoryEntry.path();

			std::string filenameString = path.filename().string();
			
			ImGui::PushID(filenameString.c_str());

			Ref<Texture2D> icon = directoryEntry.is_directory() ? m_DirectoryIcon : m_FileIcon;
			
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0,0,0,0));
			ImGui::ImageButton((ImTextureID)icon->GetRendererID(), {thumbnailSize,thumbnailSize},{0,1},{1,0});

			if (ImGui::BeginDragDropSource())
			{
				std::filesystem::path relativePath(path);

				const wchar_t* itemPath = relativePath.c_str();
				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath,(wcslen(itemPath) + 1) * sizeof(wchar_t),ImGuiCond_Once);
				ImGui::EndDragDropSource();
			}

			ImGui::PopStyleColor();

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (directoryEntry.is_directory())
					m_CurrentDirectory /= path.filename();
			}

			ImGui::TextWrapped(filenameString.c_str());

			ImGui::NextColumn();
			ImGui::PopID();
		}

		ImGui::Columns(1);

		ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512);
		ImGui::SliderFloat("Padding", &padding, 0, 32);

		ImGui::End();
	}

}
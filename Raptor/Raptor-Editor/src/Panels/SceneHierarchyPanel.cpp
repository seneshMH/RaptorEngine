#include "SceneHierarchyPanel.h"
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <glm/gtc/type_ptr.hpp>

#include "Raptor/Scene/Component.h"
#include "Raptor/Renderer/Texture.h"


namespace Raptor {

	extern const std::filesystem::path s_AssetPath;

	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& scene)
	{
		SetContext(scene);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
	{
		m_Context = context;
		m_SelectionContext = {};
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		if (m_Context) {
			m_Context->m_Registry.each([&](auto entityID)
				{
					Entity entity{ entityID,m_Context.get() };
					DrawEntityNode(entity);
				});

			if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
				m_SelectionContext = {};

			if (ImGui::BeginPopupContextWindow(0, 1, false))
			{
				if (ImGui::MenuItem("Create Empty Entity"))
					m_Context->CreateEntity("Empty Entity");

				ImGui::EndPopup();
			}
		}

		ImGui::End();

		ImGui::Begin("Properties Panel");

		if (m_SelectionContext)
		{
			DrawComponents(m_SelectionContext);
		}
		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;

		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());

		if (ImGui::IsItemClicked())
		{
			m_SelectionContext = entity;
		}

		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
				entityDeleted = true;

			ImGui::EndPopup();
		}

		if (opened)
		{
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
			opened = ImGui::TreeNodeEx((void*)98456732, flags, tag.c_str());
			if (opened)
				ImGui::TreePop();
			ImGui::TreePop();
		}

		if (entityDeleted)
		{
			m_Context->DestroyEntity(entity);
			if (m_SelectionContext == entity)
				m_SelectionContext = {};
		}
	}

	static void DrawVec3Control(const std::string& label,glm::vec3& value,float resetValue = 0.0f,float columnWidth = 100.0f)
	{
		ImGuiIO io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f,lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button,ImVec4(0.8f,0.1f,0.15f,1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered,ImVec4(0.9f,0.2f,0.2f,1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive,ImVec4(0.8f,0.1f,0.15f,1.0f));
		ImGui::PushFont(boldFont);
		if (ImGui::Button("x", buttonSize))
			value.x = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##x", &value.x, 0.1f,0.0f,0.0f,"%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.8f, 0.3f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
		ImGui::PushFont(boldFont);
		if (ImGui::Button("y", buttonSize))
			value.y = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##y", &value.y, 0.1f,0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.35f, 0.9f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
		ImGui::PushFont(boldFont);
		if (ImGui::Button("z", buttonSize))
			value.z = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##z", &value.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();
		ImGui::Columns(1);

		ImGui::PopID();
	}

	template<typename T,typename UIFunction>
	static void DrawComponent(const std::string& name,Entity entity,UIFunction uiFunftion)
	{
		if (entity.HasComponent<T>())
		{
			const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

			auto& component = entity.GetComponent<T>();
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImGui::Separator();
			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
			ImGui::PopStyleVar();

			ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
			if (ImGui::Button("+", ImVec2(lineHeight, lineHeight)))
			{
				ImGui::OpenPopup("ComponentSettings");
			}


			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove Component"))
					removeComponent = true;

				ImGui::EndPopup();
			}

			if (open)
			{
				uiFunftion(component);
				ImGui::TreePop();
			}


			if (removeComponent)
				entity.RemoveComponent<T>();
		}
	}

	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().Tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());

			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);

		if (ImGui::Button("Add Component"))
			ImGui::OpenPopup("Add Component");

		if (ImGui::BeginPopup("Add Component"))
		{
			DisplayAddComponentEntry<CameraComponent>("Camera");
			DisplayAddComponentEntry<SpriteRendererComponent>("Sprite Renderer");
			DisplayAddComponentEntry<CircleRendererComponent>("Circle Renderer");
			DisplayAddComponentEntry<Rigidbody2DComponent>("Rigidbody 2D");
			DisplayAddComponentEntry<BoxCollider2DComponent>("Box Collider 2D");
			DisplayAddComponentEntry<CircleCollider2DComponent>("Circle Collider 2D");

			ImGui::EndPopup();
		}

		ImGui::PopItemWidth();


		DrawComponent<TransformComponent>("Transform", entity, [](auto& component)
			{

				DrawVec3Control("Translation", component.Translation);
				glm::vec3 rotation = glm::degrees(component.Rotation);
				DrawVec3Control("Rotation", rotation);
				component.Rotation = glm::radians(rotation);
				DrawVec3Control("Scale", component.Scale, 1.0f);

			});

		DrawComponent<CameraComponent>("Camera", entity, [](auto& component)
			{
				
				auto& camera = component.Camera;

				ImGui::Checkbox("Primary", &component.Primary);

				const char* projectionTypeString[] = { "Perspective", "Orthographic" };
				const char* currentProjectionTypeString = projectionTypeString[(int)camera.GetProjectionType()];
				if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
				{
					for (int i = 0; i < 2; i++)
					{
						bool isSelected = currentProjectionTypeString == projectionTypeString[i];
						if (ImGui::Selectable(projectionTypeString[i], isSelected))
						{
							currentProjectionTypeString == projectionTypeString[i];
							camera.SetProjectionType((SceneCamera::ProjectionType)i);
						}

						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::perspective)
				{
					float verticalFov = glm::degrees(camera.GetPerspectiveVerticalFOV());
					if (ImGui::DragFloat("Veryical FOV", &verticalFov))
					{
						camera.SetPerspectiveVerticalFOV(glm::radians(verticalFov));
					}

					float nearClip = camera.GetPerspectiveNearClip();
					if (ImGui::DragFloat("Near", &nearClip))
					{
						camera.SetPerspectiveNearClip(nearClip);
					}

					float farClip = camera.GetPerspectiveFarClip();
					if (ImGui::DragFloat("Far", &farClip))
					{
						camera.SetPerspectiveFarClip(farClip);
					}
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::orthographic)
				{
					float orthoSize = camera.GetOrthographicsize();
					if (ImGui::DragFloat("Size", &orthoSize))
					{
						camera.SetOrthographicSize(orthoSize);
					}

					float nearClip = camera.GetOrthographicNearClip();
					if (ImGui::DragFloat("Near", &nearClip))
					{
						camera.SetOrthographicNearClip(nearClip);
					}

					float farClip = camera.GetOrthographicFarClip();
					if (ImGui::DragFloat("Far", &farClip))
					{
						camera.SetOrthographicFarClip(farClip);
					}

					ImGui::Checkbox("Fixed Aspect Ratio", &component.FixedAspectRatio);

				}
			});

		DrawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [](auto& component)
			{
				ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));

				ImGui::Button("Texture",ImVec2(100.0f,0.0f));
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
					{
						const wchar_t* path = (const wchar_t*)payload->Data;
						std::filesystem::path texturePath = std::filesystem::path(s_AssetPath) / path;
						component.Texture = Texture2D::Create(texturePath.string());
					}
					ImGui::EndDragDropTarget();
				}

				ImGui::DragFloat("Tiling Factor",&component.TilingFactor,0.1f,0.0f,100.0f);
			});

		DrawComponent<CircleRendererComponent>("Circle Renderer", entity, [](auto& component)
			{
				ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
				ImGui::DragFloat("Thickness", &component.Thickness, 0.025f, 0.0f, 1.0f);
				ImGui::DragFloat("Fade", &component.Fade, 0.00025f, 0.0f, 1.0f);
			});

			DrawComponent<Rigidbody2DComponent>("Rigidbody 2D", entity, [](auto& component)
			{
				const char* bodyTypeString[] = { "Static", "Dynamic" , "Kinematic"};
				const char* currentBodyTypeString = bodyTypeString[(int)component.Type];
				if (ImGui::BeginCombo("Body Type", currentBodyTypeString))
				{
					for (int i = 0; i < 2; i++)
					{
						bool isSelected = currentBodyTypeString == bodyTypeString[i];
						if (ImGui::Selectable(bodyTypeString[i], isSelected))
						{
							currentBodyTypeString == bodyTypeString[i];
							component.Type = ((Rigidbody2DComponent::BodyType)i);
						}

						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				ImGui::Checkbox("Fixed Rotation",&component.FixedRotation);
			});

			DrawComponent<BoxCollider2DComponent>("Box Collider 2D", entity, [](auto& component)
				{
					ImGui::DragFloat2("Offset",glm::value_ptr(component.Offset));
					ImGui::DragFloat2("Size",glm::value_ptr(component.Size));

					ImGui::DragFloat("Density",&component.Density,0.01f,0.0f,1.0f);
					ImGui::DragFloat("Friction", &component.Friction, 0.01f, 0.0f, 1.0f);
					ImGui::DragFloat("Restitution", &component.Restitution, 0.01f, 0.0f, 1.0f);
					ImGui::DragFloat("RestitutionThreshold", &component.RestitutionThreshold, 0.01f, 0.0f);

				});

			DrawComponent<CircleCollider2DComponent>("Circle Collider 2D", entity, [](auto& component)
				{
					ImGui::DragFloat2("Offset", glm::value_ptr(component.Offset));
					ImGui::DragFloat("Radius", &component.Radius);

					ImGui::DragFloat("Density", &component.Density, 0.01f, 0.0f, 1.0f);
					ImGui::DragFloat("Friction", &component.Friction, 0.01f, 0.0f, 1.0f);
					ImGui::DragFloat("Restitution", &component.Restitution, 0.01f, 0.0f, 1.0f);
					ImGui::DragFloat("RestitutionThreshold", &component.RestitutionThreshold, 0.01f, 0.0f);

				});

		
	}

	template<typename T>
	void SceneHierarchyPanel::DisplayAddComponentEntry(const std::string& entryName)
	{
		if (!m_SelectionContext.HasComponent<T>())
		{
			if (ImGui::MenuItem(entryName.c_str()))
			{
				m_SelectionContext.AddComponent<T>();
				ImGui::CloseCurrentPopup();
			}
		}
	}
	
	void SceneHierarchyPanel::SetSelectedEntity(Entity entity)
	{
		m_SelectionContext = entity;
	}
}
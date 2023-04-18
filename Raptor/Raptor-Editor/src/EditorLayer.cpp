#include "EditorLayer.h"

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <filesystem>

#include "Raptor/Scene/SceneSrializer.h"
#include "Raptor/utils/PlatformUtils.h"
#include "Raptor/Math/Math.h"
#include "Raptor/Scripting/ScriptEngine.h"

#include "ImGuizmo.h"

namespace Raptor {

	
	EditorLayer::EditorLayer()
		:Layer("EditorLayer"), m_CameraController(1280.0f / 720.0f, true)
	{
	}

	void EditorLayer::OnAttach()
	{
		RT_PROFILE_FUNCTION();

		//m_CheckerBordTexture = Texture2D::Create("assets/images/checker.png");
		m_IconPlay = Texture2D::Create("resources/icons/PlayButton.png");
		m_IconPause = Texture2D::Create("resources/icons/PauseButton.png");
		m_IconStop = Texture2D::Create("resources/icons/StopButton.png");
		m_IconSimiulate = Texture2D::Create("resources/icons/SimulateButton.png");
		m_IconStep = Texture2D::Create("resources/icons/StepButton.png");

		FrameBufferSpecification fbSpec;
		fbSpec.Attachments = { FramebufferTextureFormat::RGBA8 , FramebufferTextureFormat::RED_INTEGER ,FramebufferTextureFormat::Depth };
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_FrameBuffer = FrameBuffer::Create(fbSpec);

		m_EditorScene = CreateRef<Scene>();
		m_ActiveScene = m_EditorScene;

		m_EditorCamera = EditorCamera(30.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
#if 0
		auto Square = m_ActiveScene->CreateEntity("Green Square");
		Square.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.0f,1.0f,0.0f,1.0f });

		auto redSquare = m_ActiveScene->CreateEntity("Red Square");
		redSquare.AddComponent<SpriteRendererComponent>(glm::vec4{ 1.0f,0.0f,0.0f,1.0f });


		m_SqureEntity = Square;

		m_CameraEntity = m_ActiveScene->CreateEntity("Camera A");
		m_CameraEntity.AddComponent<CameraComponent>();

		m_SecondCamera = m_ActiveScene->CreateEntity("Camera B");
		auto& cc = m_SecondCamera.AddComponent<CameraComponent>();
		cc.Primary = false;

		class CameraController : public ScriptableEntity
		{
		public:
			void OnCreate()
			{
			}

			void OnDestroy()
			{

			}

			void OnUpdate(Timestep ts)
			{
				auto& translation = GetComponent<TransformComponent>().Translation;
				float speed = 5.0f;

				if (Input::IsKeyPressed(RT_KEY_A))
					translation.x -= speed * ts;
				if (Input::IsKeyPressed(RT_KEY_D))
					translation.x += speed * ts;
				if (Input::IsKeyPressed(RT_KEY_W))
					translation.y += speed * ts;
				if (Input::IsKeyPressed(RT_KEY_S))
					translation.y -= speed * ts;
			}
		};

		m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
#endif
		auto commandLineArgs = Application::Get().GetSpecification().CommandLineArgs;
		if (commandLineArgs.Count > 1)
		{
			auto projectFilePath = commandLineArgs[1];
			OpenProject(projectFilePath);
		}
		else
		{
			//NewProject();
			if (!OpenProject())
				Application::Get().Close();
		}
		
		Renderer2D::SetLineWidth(4.0f);

		m_SceneHierarchyPanel.SetContext(m_ActiveScene);


	}

	void EditorLayer::OnDetach()
	{
		RT_PROFILE_FUNCTION();

	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		RT_PROFILE_FUNCTION();
		m_ActiveScene->OnViewportResize((uint32_t)m_viewportSize.x, (uint32_t)m_viewportSize.y);


		if (FrameBufferSpecification spec = m_FrameBuffer->GetSpecification();
			m_viewportSize.x > 0.0f && m_viewportSize.y > 0.0f && // zero sized framebuffer is invalid
			(spec.Width != m_viewportSize.x || spec.Height != m_viewportSize.y))
		{
			m_FrameBuffer->Resize((uint32_t)m_viewportSize.x, (uint32_t)m_viewportSize.y);
			m_CameraController.OnResize(m_viewportSize.x, m_viewportSize.y);
			m_EditorCamera.SetviewportSize(m_viewportSize.x, m_viewportSize.y);
			m_EditorCamera.SetviewportSize(m_viewportSize.x, m_viewportSize.y);
		}

		Renderer2D::ResetStats();
		m_FrameBuffer->Bind();
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::Clear();

		m_FrameBuffer->ClearAttachment(1, -1);


		switch (m_SceneState)
		{
		case SceneState::Edit:
		{
			if (m_ViewportFocused)
				m_CameraController.OnUpdate(ts);

			m_EditorCamera.OnUpdate(ts);
			m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);
			break;
		}
		case SceneState::Simulate:
		{
			m_EditorCamera.OnUpdate(ts);
			m_ActiveScene->OnUpdateSimiulation(ts, m_EditorCamera);
			break;
		}
		case SceneState::Play:
		{
			m_ActiveScene->OnUpdateRunTime(ts);
			break;
		}
		}


		auto [mx, my] = ImGui::GetMousePos();
		mx -= m_ViewportBounds[0].x;
		my -= m_ViewportBounds[0].y;

		glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
		my = viewportSize.y - my;
		int mouseX = (int)mx;
		int mouseY = (int)my;

		if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
		{
			int pixelData = m_FrameBuffer->ReadPixel(1, mouseX, mouseY);
			m_HoveredEntity = pixelData == -1 ? Entity() : Entity((entt::entity)pixelData, m_ActiveScene.get());

		}

		OnOverlayRender();


		m_FrameBuffer->UnBind();
	}

	void EditorLayer::OnImGuiRender()
	{
		RT_PROFILE_FUNCTION();

		static bool dockingEnabled = true;

		static bool dockSpaceOpen = true;
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockSpaceOpen, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		float minSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 370.0f;

		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
		style.WindowMinSize.x = minSizeX;

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open Project...", "Ctrl+O"))
				{
					OpenProject();
				}

				ImGui::Separator();

				if (ImGui::MenuItem("New Scene", "Ctrl+N"))
				{
					NewScene();
				}

				if (ImGui::MenuItem("Save Scene", "Ctrl+S"))
				{
					SaveScene();
				}

				if (ImGui::MenuItem("Save Scene AS...", "Ctrl+Shift+S"))
				{
					SaveSceneAs();
				}

				if (ImGui::MenuItem("Exit")) Application::Get().Close();
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Script"))
			{
				if (ImGui::MenuItem("Relaod Assembly", "Ctrl+R"))
				{
					ScriptEngine::RelaodAssembly();
				}
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		m_SceneHierarchyPanel.OnImGuiRender();
		m_ContentBrowserPanel->OnImGuiRender();

		ImGui::Begin("Render Stats");

		std::string name = "None";
		if (m_HoveredEntity)
			name = m_HoveredEntity.GetComponent<TagComponent>().Tag;

		ImGui::Text("Entity : %s", name.c_str());

		auto stats = Renderer2D::GetStats();
		ImGui::Text("Renderer2D Stats :");
		ImGui::Text("Draw Calls	: %d", stats.DrawCalls);
		ImGui::Text("Quads		: %d", stats.QuadCount);
		ImGui::Text("Vertices	: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices		: %d", stats.GetTotalIndexCount());

		ImGui::End();

		ImGui::Begin("Settings");
		ImGui::Checkbox("Show Physics Colliders", &m_ShowPhysicsColliders);
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

		//Viewport
		ImGui::Begin("viewport");
		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();

		Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

		m_viewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		uint64_t textureID = m_FrameBuffer->GetColorAttachmentRendererID();
		ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2(m_viewportSize.x, m_viewportSize.y), ImVec2(0, 1), ImVec2(1, 0));

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				const wchar_t* path = (const wchar_t*)payload->Data;
				OpenScene(path);
			}
			ImGui::EndDragDropTarget();
		}

		//Gizmos
		Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
		if (selectedEntity && m_GizmoType != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);

			//auto cameraEntity = m_ActiveScene->GetPrimaryCameraEntity();
			//const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
			//const glm::mat4& cameraProjection = camera.GetProjection();
			//glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());

			//Editor Camera
			const glm::mat4& cameraProjection = m_EditorCamera.GetProjection();
			glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();


			auto& tc = selectedEntity.GetComponent<TransformComponent>();
			glm::mat4 transform = tc.GetTransform();

			//Snaping
			bool snap = Input::IsKeyPressed(Key::LeftControl);
			float snapValue = 0.5f;
			if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
				snapValue = 45.0f;

			float snapValues[3] = { snapValue,snapValue ,snapValue };

			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
				(ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform), nullptr,
				snap ? snapValues : nullptr);

			if (ImGuizmo::IsUsing())
			{
				glm::vec3 translation, rotation, scale;
				Math::DecomposeTransform(transform, translation, rotation, scale);

				glm::vec3 deltaRotation = rotation - tc.Rotation;

				tc.Translation = translation;
				tc.Rotation += deltaRotation;
				tc.Scale = scale;
			}
		}

		ImGui::End();
		ImGui::PopStyleVar();

		UI_ToolBar();

		ImGui::End();

	}

	void EditorLayer::OnEvent(Event& e)
	{
		m_CameraController.OnEvevnt(e);
		if (m_SceneState == SceneState::Edit)
		{
			m_EditorCamera.OnEvent(e);
		}
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(RT_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
		dispatcher.Dispatch<MouseButtonPressedEvent>(RT_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		if (e.IsRepeat())
			return false;

		bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
		bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);

		switch (e.GetKeyCode())
		{
		case Key::N:
		{
			if (control)
			{
				NewScene();
			}
			break;
		}
		case Key::O:
		{
			if (control)
			{
				OpenProject();
			}
			break;
		}

		case Key::S:
		{
			if (control)
			{
				if (shift)
					SaveSceneAs();
				else
					SaveScene();
			}
			break;
		}

		//Commands

		case Key::D:
		{
			if (control)
			{
				OnDuplicateEntity();
			}
			break;
		}

		//Gizmos
		case Key::Q:
		{
			m_GizmoType = -1;
			break;
		}
		case Key::W:
		{
			m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
			break;
		}
		case Key::E:
		{
			m_GizmoType = ImGuizmo::OPERATION::ROTATE;
			break;
		}
		case Key::R:
		{
			if(control)
			{ 
				ScriptEngine::RelaodAssembly();
			}
			else
			{
				m_GizmoType = ImGuizmo::OPERATION::SCALE;
			}
			break;
		}

		default:
			break;
		}

		return false;
	}

	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		if (e.GetMouseButton() == Mouse::ButtonLeft)
		{
			if (m_ViewportHovered && !ImGuizmo::IsOver() && !Input::IsKeyPressed(Key::LeftAlt))
				m_SceneHierarchyPanel.SetSelectedEntity(m_HoveredEntity);
		}
		return false;
	}

	void EditorLayer::OnOverlayRender()
	{
		if (m_SceneState == SceneState::Play)
		{
			Entity camera = m_ActiveScene->GetPrimaryCameraEntity();
			if (!camera)
				return;

			Renderer2D::BeginScene(camera.GetComponent<CameraComponent>().Camera, camera.GetComponent<TransformComponent>().GetTransform());
		}
		else
		{
			Renderer2D::BeginScene(m_EditorCamera);
		}

		if (m_ShowPhysicsColliders)
		{
			//BOX COLLIDERS
			{
				auto view = m_ActiveScene->GetAllEntitesWidth<TransformComponent, BoxCollider2DComponent>();

				for (auto entity : view)
				{
					auto [tc, bc2d] = view.get<TransformComponent, BoxCollider2DComponent>(entity);

					glm::vec3 translation = tc.Translation + glm::vec3(bc2d.Offset, 0.001f);
					glm::vec3 scale = tc.Scale * glm::vec3(bc2d.Size * 2.0f, 1.0f);

					glm::mat4 transform = glm::translate(glm::mat4(1.0f), translation)
						* glm::rotate(glm::mat4(1.0f), tc.Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f))
						* glm::scale(glm::mat4(1.0f), scale);

					Renderer2D::DrawRect(transform, glm::vec4(0, 1, 0, 1));
				}
			}

			//CIRCLE COLLIDERS
			{
				auto view = m_ActiveScene->GetAllEntitesWidth<TransformComponent, CircleCollider2DComponent>();

				for (auto entity : view)
				{
					auto [tc, cc2d] = view.get<TransformComponent, CircleCollider2DComponent>(entity);

					glm::vec3 translation = tc.Translation + glm::vec3(cc2d.Offset, 0.01f);
					glm::vec3 scale = tc.Scale * glm::vec3(cc2d.Radius * 2.0f);

					glm::mat4 transform = glm::translate(glm::mat4(1.0f), translation)
						* glm::scale(glm::mat4(1.0f), scale);

					Renderer2D::DrawCircle(transform, glm::vec4(0, 1, 0, 1), 0.05f);
				}
			}

		}

		if (Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity())
		{
			const TransformComponent& transform = selectedEntity.GetComponent<TransformComponent>();
			Renderer2D::DrawRect(transform.GetTransform(), glm::vec4(1.0f, 0.5f, 0.0f, 1.0f));
		}

		Renderer2D::EndScene();
	}

	void EditorLayer::NewProject()
	{
		Project::New();
	}

	bool EditorLayer::OpenProject()
	{
		std::string filePath = FileDalogs::OpenFile("Raptor Project (*.rproj)\0*.rproj\0");

		if (filePath.empty())
			return false;
		
		OpenProject(filePath);
		return true;
	}

	void EditorLayer::OpenProject(const std::filesystem::path& path)
	{
		if (Project::Load(path))
		{
			std::filesystem::path startScenePath = Project::GetAssetFileSystemPath( Project::GetActive()->GetConfig().StartScene);
			OpenScene(startScenePath);
			m_ContentBrowserPanel = CreateScope<ContentBrowserPanel>();
		}
	}

	void EditorLayer::SaveProject()
	{
		//Project::SaveActive();
	}

	void EditorLayer::NewScene()
	{
		m_ActiveScene = CreateRef<Scene>();
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);

		m_EditorScenePath = std::filesystem::path();
	}

	void EditorLayer::OpenScene()
	{
		std::string filePath = FileDalogs::OpenFile("Raptor Scene (*.raptor)\0*.raptor\0");

		if (!filePath.empty())
		{
			OpenScene(filePath);
		}
	}

	void EditorLayer::OpenScene(const std::filesystem::path& path)
	{
		if (m_SceneState != SceneState::Edit)
			OnSceneStop();

		if (path.extension().string() != ".raptor")
		{
			RT_WARN("Could not load {0} as scene file", path.filename().string());
			return;
		}

		Ref<Scene> newScene = CreateRef<Scene>();
		SceneSrializer serializer(newScene);

		if (serializer.DeSerialize(path.string()))
		{
			m_EditorScene = newScene;

			m_SceneHierarchyPanel.SetContext(m_EditorScene);
			m_ActiveScene = m_EditorScene;
			m_EditorScenePath = path;
		}
	}

	void EditorLayer::SaveSceneAs()
	{
		std::string filePath = FileDalogs::SaveFile("Raptor Scene (*.raptor)\0*.raptor\0");

		if (!filePath.empty())
		{
			SerializeScene(m_ActiveScene, filePath);
			m_EditorScenePath = filePath;
		}
	}

	void EditorLayer::SaveScene()
	{
		if (!m_EditorScenePath.empty())
		{
			SerializeScene(m_ActiveScene, m_EditorScenePath);
		}
		else
		{
			SaveSceneAs();
		}
	}

	void EditorLayer::SerializeScene(Ref<Scene> scene, const std::filesystem::path& path)
	{
		SceneSrializer serializer(scene);
		serializer.Serialize(path.string());
	}

	void EditorLayer::UI_ToolBar()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

		auto& colors = ImGui::GetStyle().Colors;
		const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
		const auto& buttonActive = colors[ImGuiCol_ButtonActive];

		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

		ImGui::Begin("##toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		float size = ImGui::GetWindowHeight() - 4.0f;

		bool toolbarEnabled = (bool)m_ActiveScene;
		ImVec4 tintColor = ImVec4(1, 1, 1, 1);
		if (!toolbarEnabled)
			tintColor.x = 0.5f;

		ImGui::SetCursorPosX((ImGui::GetContentRegionMax().x * 0.5f) - (size * 0.5f));
		
		bool hasPlayButton = m_SceneState == SceneState::Edit || m_SceneState == SceneState::Play;
		bool hasSimulateButton = m_SceneState == SceneState::Edit || m_SceneState == SceneState::Simulate;
		bool hasPauseButton = m_SceneState != SceneState::Edit;

		if (hasPlayButton)
		{
			{
				Ref<Texture2D> icon = (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Simulate) ? m_IconPlay : m_IconStop;
				if (ImGui::ImageButton((ImTextureID)(uint64_t)icon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0, 0, 0, 0), tintColor) && toolbarEnabled)
				{
					if (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Simulate)
						OnScenePlay();
					else if (m_SceneState == SceneState::Play)
						OnSceneStop();
				}
			}
		}

		if (hasSimulateButton)
		{
			if (hasPlayButton)
				ImGui::SameLine();
			
			Ref<Texture2D> icon = (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Play) ? m_IconSimiulate : m_IconStop;
			if (ImGui::ImageButton((ImTextureID)(uint64_t)icon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0, 0, 0, 0), tintColor) && toolbarEnabled)
			{
				if (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Play)
					OnSceneSimulate();
				else if (m_SceneState == SceneState::Simulate)
					OnSceneStop();
			}
			
		}
		

		if (hasPauseButton)
		{
			bool isPaused = m_ActiveScene->IsPaused();
			ImGui::SameLine();
			{
				Ref<Texture2D> icon = m_IconPause;
				if (ImGui::ImageButton((ImTextureID)(uint64_t)icon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0, 0, 0, 0), tintColor) && toolbarEnabled)
				{
					m_ActiveScene->SetPause(!isPaused);
				}
			}

			if (isPaused)
			{
				ImGui::SameLine();
				{
					Ref<Texture2D> icon = m_IconStep;
					if (ImGui::ImageButton((ImTextureID)(uint64_t)icon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0, 0, 0, 0), tintColor) && toolbarEnabled)
					{
						m_ActiveScene->Step(1);
					}
				}
			}
		}


		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(3);
		ImGui::End();
	}

	void EditorLayer::OnScenePlay()
	{
		if (m_SceneState == SceneState::Simulate)
			OnSceneStop();

		m_SceneState = SceneState::Play;
		m_ActiveScene = Scene::Copy(m_EditorScene);
		m_ActiveScene->OnRuntimeStart();

		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void EditorLayer::OnSceneStop()
	{
		RT_CORE_ASSERT(m_SceneState == SceneState::Simulate || m_SceneState == SceneState::Play)

			if (m_SceneState == SceneState::Play)
				m_ActiveScene->OnRuntimeStop();
			else if (m_SceneState == SceneState::Simulate)
				m_ActiveScene->OnSimiulationStop();

		m_SceneState = SceneState::Edit;
		m_ActiveScene = m_EditorScene;

		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void EditorLayer::OnScenePause()
	{
		if (m_SceneState == SceneState::Edit)
			return;

		m_ActiveScene->SetPause(true);
	}

	void EditorLayer::OnSceneSimulate()
	{
		if (m_SceneState == SceneState::Play)
			OnSceneStop();

		m_SceneState = SceneState::Simulate;
		m_ActiveScene = Scene::Copy(m_EditorScene);
		m_ActiveScene->OnSimiulationStart();

		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void EditorLayer::OnDuplicateEntity()
	{
		if (m_SceneState != SceneState::Edit)
			return;

		Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
		if (selectedEntity)
			m_EditorScene->DuplicateEntity(selectedEntity);
	}

}
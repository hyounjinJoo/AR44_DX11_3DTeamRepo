#include "ClientPCH.h"
#include "guiEditor.h"

#include <Engine/Mesh.h>
#include <Engine/ResMgr.h>
#include <Engine/Material.h>
#include <Engine/Com_Transform.h>
#include <Engine/Com_Renderer_Mesh.h>
#include <Engine/GridScript.h>
#include <Engine/Object.h>
#include <Engine/Application.h>
#include <Engine/GraphicDevice_DX11.h>

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "guiInspector.h"
#include "guiGame.h"
#include "guiHierarchy.h"
#include "guiProject.h"
#include "guiYamYamEditor.h"
#include "guiConsole.h"
#include "guiListWidget.h"

extern mh::Application application;

namespace gui
{
	using namespace mh::define;
	using namespace mh::math;
	void Editor::Initialize()
	{
		mbEnable = false;

		if (mbEnable == false)
			return;

		// 충돌체의 종류 갯수만큼만 있으면 된다.
		mDebugObjects.resize((UINT)eColliderType::End);

		std::shared_ptr<mh::Mesh> rectMesh = mh::ResMgr::GetInst()->Find<mh::Mesh>(mh::strKey::Default::mesh::DebugRectMesh);
		std::shared_ptr<mh::Material> material = mh::ResMgr::GetInst()->Find<mh::GPU::Material>(mh::strKey::Default::material::DebugMaterial);

		mDebugObjects[(UINT)eColliderType::Rect] = new DebugObject();
		mh::Com_Renderer_Mesh* renderer
			= mDebugObjects[(UINT)eColliderType::Rect]->AddComponent<mh::Com_Renderer_Mesh>();

		renderer->SetMaterial(material);
		renderer->SetMesh(rectMesh);

		std::shared_ptr<mh::Mesh> circleMesh = mh::ResMgr::GetInst()->Find<mh::Mesh>("CircleMesh");

		mDebugObjects[(UINT)eColliderType::Circle] = new DebugObject();
		renderer
			= mDebugObjects[(UINT)eColliderType::Circle]->AddComponent<mh::Com_Renderer_Mesh>();

		renderer->SetMaterial(material);
		renderer->SetMesh(circleMesh);

		//그리드 이쪽으로 옮겨줘야 한다.
		// Grid Object
		//EditorObject* gridObject = new EditorObject();
		//mh::Com_Renderer_Mesh* gridMr = gridObject->AddComponent<mh::Com_Renderer_Mesh>();
		//gridMr->SetMesh(mh::ResMgr::GetInst()->Find<mh::Mesh>(L"RectMesh"));
		//gridMr->SetMaterial(mh::ResMgr::GetInst()->Find<Material>(L"GridMaterial"));
		//mh::GridScript* gridScript = gridObject->AddComponent<mh::GridScript>();
		//gridScript->SetCamera(gMainCamera);

		//mEditorObjects.push_back(gridObject);

		ImGuiInitialize();

		mYamYamEditor = new YamYamEditor();
		//mWidgets.push_back(mYamYamEditor);

		// Init Widget 
		Inspector* inspector = new Inspector();
		mWidgets.insert(std::make_pair("Inspector", inspector));

		Game* game = new Game();
		mWidgets.insert(std::make_pair("Game", game));

		Hierarchy* hierarchy = new Hierarchy();
		mWidgets.insert(std::make_pair("Hierarchy", hierarchy));

		Project* project = new Project();
		mWidgets.insert(std::make_pair("Project", project));
		
		Console* console = new Console();
		mWidgets.insert(std::make_pair("Console", console));

		ListWidget* listWidget = new ListWidget();
		mWidgets.insert(std::make_pair("ListWidget", listWidget));

	}

	void Editor::Run()
	{
		if (mbEnable == false)
			return;


		Update();
		FixedUpdate();
		Render();

		ImGuiRun();
	}

	void Editor::Update()
	{
		for (EditorObject* obj : mEditorObjects)
		{
			obj->Update();
		}
	}

	void Editor::FixedUpdate()
	{
		for (EditorObject* obj : mEditorObjects)
		{
			obj->FixedUpdate();
		}
	}

	void Editor::Render()
	{
		for (EditorObject* obj : mEditorObjects)
		{
			obj->Render();
		}

		for ( mh::GPU::tDebugMesh& mesh : mh::renderer::gDebugMeshes)
		{
			DebugRender(mesh);
		}
		mh::renderer::gDebugMeshes.clear();
	}

	void Editor::Release()
	{
		if (mbEnable == false)
			return;


		ImGuiRelease();

		for (auto iter : mWidgets)
		{
			delete iter.second;
			iter.second = nullptr;
		}

		delete mYamYamEditor;
		mYamYamEditor = nullptr;

		for (auto obj : mEditorObjects)
		{
			delete obj;
			obj = nullptr;
		}

		delete mDebugObjects[(UINT)eColliderType::Rect];
		delete mDebugObjects[(UINT)eColliderType::Circle];
	}

	void Editor::DebugRender(mh::GPU::tDebugMesh& mesh)
	{
		DebugObject* debugObj = mDebugObjects[(UINT)mesh.type];
		
		mh::Com_Transform* tr = debugObj->GetComponent<mh::Com_Transform>();
		tr->SetPosition(mesh.position);
		tr->SetRotation(mesh.rotatation);
		

		if (mesh.type == eColliderType::Rect)
			tr->SetScale(mesh.scale);
		else
			tr->SetScale(Vector3(mesh.radius));


		mh::IRenderer* renderer = debugObj->GetComponent<mh::IRenderer>();
		mh::Com_Camera* camera = mh::renderer::gMainCamera;

		tr->FixedUpdate();

		mh::Com_Camera::SetGpuViewMatrix(mh::renderer::gMainCamera->GetViewMatrix());
		mh::Com_Camera::SetGpuProjectionMatrix(mh::renderer::gMainCamera->GetProjectionMatrix());

		debugObj->Render();
	}

	void Editor::ImGuiInitialize()
	{
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
		//io.ConfigViewportsNoAutoMerge = true;
		//io.ConfigViewportsNoTaskBarIcon = true;
		//io.ConfigViewportsNoDefaultParent = true;
		//io.ConfigDockingAlwaysTabBar = true;
		//io.ConfigDockingTransparentPayload = true;
		//io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;     // FIXME-DPI: Experimental. THIS CURRENTLY DOESN'T WORK AS EXPECTED. DON'T USE IN USER APP!
		//io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports; // FIXME-DPI: Experimental.

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsLight();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		// Setup Platform/Renderer backends
		ImGui_ImplWin32_Init(application.GetHwnd());
		ImGui_ImplDX11_Init(mh::GPU::GetDevice()->GetID3D11Device()
			, mh::GPU::GetDevice()->GetID3D11DeviceContext());

		// Load Fonts
		// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
		// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
		// - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
		// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
		// - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
		// - Read 'docs/FONTS.md' for more instructions and details.
		// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
		//io.Fonts->AddFontDefault();
		//io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
		//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
		//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
		//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
		//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
		//IM_ASSERT(font != nullptr);

		// Our state

	}

	void Editor::ImGuiRun()
	{
		bool show_demo_window = true;
		bool show_another_window = false;
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

		ImGuiIO io = ImGui::GetIO();

		// Start the Dear ImGui frame
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		//// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		//for (Widget* widget : mWidgets) 
		//{
		//	widget->Update();
		//}

		mYamYamEditor->Render();
		for (auto iter : mWidgets)
		{
			iter.second->Render();
		}

#pragma region  SAMPLE
		 //2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
		//{
		//	static float f = 0.0f;
		//	static int counter = 0;

		//	ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		//	ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		//	ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		//	ImGui::Checkbox("Another Window", &show_another_window);

		//	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		//	ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		//	if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		//		counter++;
		//	ImGui::SameLine();
		//	ImGui::Text("counter = %d", counter);

		//	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		//	ImGui::End();
		//}

		//// 3. Show another simple window.
		//if (show_another_window)
		//{
		//	ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		//	ImGui::Text("Hello from another window!");
		//	if (ImGui::Button("Close Me"))
		//		show_another_window = false;
		//	ImGui::End();
		//}
#pragma endregion
		// Rendering
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		
		// Update and Render additional Platform Windows
		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	}

	void Editor::ImGuiRelease()
	{
		// Cleanup
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}
}

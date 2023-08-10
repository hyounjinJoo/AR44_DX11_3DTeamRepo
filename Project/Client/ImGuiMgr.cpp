#include "PCH_Client.h"
#include "ImGuiMgr.h"

#include <Engine/Mesh.h>
#include <Engine/ResMgr.h>
#include <Engine/Material.h>
#include <Engine/Com_Transform.h>
#include <Engine/Com_Renderer_Mesh.h>
#include <Engine/GridScript.h>
#include <Engine/Object.h>
#include <Engine/Application.h>
#include <Engine/GPUMgr.h>

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "guiInspector.h"
#include "guiGame.h"
#include "guiBase.h"
#include "guiProject.h"
#include "guiMainMenu.h"
#include "guiConsole.h"
#include "guiList.h"


#include <Engine/AtExit.h>
#include "GameClient.h"

#include "guiGraphicsShaderEditor.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
namespace gui
{
	//std::unordered_map<std::string, guiBase*, mh::define::tUmap_StringViewHasher, std::equal_to<>> ImGuiMgr::mGuiWindows{};
	std::vector<guiBase*> ImGuiMgr::mGuiWindows{};
	std::vector<EditorObject*> ImGuiMgr::mEditorObjects{};
	std::vector<DebugObject*> ImGuiMgr::mDebugObjects{};

	bool ImGuiMgr::mbEnable{};
	bool ImGuiMgr::mbInitialized;

	using namespace mh::define;
	using namespace mh::math;

	
	void ImGuiMgr::Init()
	{
		AtExit::AddFunc(ImGuiMgr::Release);

		::GameClient::RegisterImGuiWndProc(ImGui_ImplWin32_WndProcHandler);

		// 충돌체의 종류 갯수만큼만 있으면 된다.
		mDebugObjects.resize((UINT)eColliderType::End);

		std::shared_ptr<mh::Mesh> rectMesh = mh::ResMgr::Find<mh::Mesh>(mh::strKey::Default::mesh::DebugRectMesh);
		std::shared_ptr<mh::Material> material = mh::ResMgr::Find<mh::Material>(mh::strKey::Default::material::DebugMaterial);

		mDebugObjects[(UINT)eColliderType::Rect] = new DebugObject();
		mh::Com_Renderer_Mesh* renderer
			= mDebugObjects[(UINT)eColliderType::Rect]->AddComponent<mh::Com_Renderer_Mesh>();

		renderer->SetMaterial(material, 0);
		renderer->SetMesh(rectMesh);

		std::shared_ptr<mh::Mesh> circleMesh = mh::ResMgr::Find<mh::Mesh>("CircleMesh");

		mDebugObjects[(UINT)eColliderType::Circle] = new DebugObject();
		renderer
			= mDebugObjects[(UINT)eColliderType::Circle]->AddComponent<mh::Com_Renderer_Mesh>();

		renderer->SetMaterial(material, 0);
		renderer->SetMesh(circleMesh);

		//그리드 이쪽으로 옮겨줘야 한다.
		// Grid Object
		//EditorObject* gridObject = new EditorObject();
		//mh::Com_Renderer_Mesh* gridMr = gridObject->AddComponent<mh::Com_Renderer_Mesh>();
		//gridMr->SetMesh(mh::ResMgr::Find<mh::Mesh>(L"RectMesh"));
		//gridMr->SetMaterial(mh::ResMgr::Find<Material>(L"GridMaterial"));
		//mh::GridScript* gridScript = gridObject->AddComponent<mh::GridScript>();
		//gridScript->SetCamera(gMainCamera);

		//mEditorObjects.push_back(gridObject);

		ImGuiInitialize();

		guiMainMenu* MainMenu = new guiMainMenu;
		mGuiWindows.push_back(MainMenu);


		// Init guiWidget 
		//guiInspector* inspector = new guiInspector();
		//mWidgets.insert(std::make_pair("guiInspector", inspector));

		////Game* game = new Game();
		////mWidgets.insert(std::make_pair("Game", game));

		//guiTree_GameObject* hierarchy = new guiTree_GameObject();
		//mWidgets.insert(std::make_pair("guiTree_GameObject", hierarchy));

		//Project* project = new Project();
		//mWidgets.insert(std::make_pair("Project", project));
		//
		//Console* console = new Console();
		//mWidgets.insert(std::make_pair("Console", console));

		//GraphicsShaderEditor* Editor = new GraphicsShaderEditor;
		//mWidgets.insert(std::make_pair("Graphics Shader Editor", Editor));

		//ListWidget* listWidget = new ListWidget();
		//mWidgets.insert(std::make_pair("ListWidget", listWidget));

	}

	void ImGuiMgr::Run()
	{
		if (false == mbEnable)
			return;
		Update();
		FixedUpdate();
		Render();
	}


	void ImGuiMgr::Update()
	{
		ImGuiNewFrame();

		for (EditorObject* obj : mEditorObjects)
		{
			obj->Update();
		}
	}

	void ImGuiMgr::FixedUpdate()
	{
		for (EditorObject* obj : mEditorObjects)
		{
			obj->FixedUpdate();
		}

		for (guiBase* gui : mGuiWindows)
		{
			gui->FixedUpdate();
		}

		//for (const auto& pair : mGuiWindows)
		//{
		//	if (nullptr == pair.second->GetParent())
		//	{
		//		pair.second->FixedUpdate();
		//	}
		//}
	}

	void ImGuiMgr::Render()
	{
		for (EditorObject* obj : mEditorObjects)
		{
			obj->Render();
		}

		auto& DebugMeshes = mh::RenderMgr::GetDebugMeshes();
		for ( mh::tDebugMesh& mesh : DebugMeshes)
		{
			DebugRender(mesh);
		}
		DebugMeshes.clear();

		ImGuiRender();
	}

	void ImGuiMgr::Release()
	{
		if (mbEnable == false)
			return;

		//for (auto& iter : mGuiWindows)
		//{
		//	if (iter.second && nullptr == iter.second->GetParent())
		//	{
		//		SAFE_DELETE(iter.second);
		//	}
		//}
		for (guiBase* gui : mGuiWindows)
		{
			if (gui)
				delete gui;
		}
		
		mGuiWindows.clear();
		
		for (auto obj : mEditorObjects)
		{
			SAFE_DELETE(obj);
		}

		SAFE_DELETE(mDebugObjects[(UINT)eColliderType::Rect]);
		SAFE_DELETE(mDebugObjects[(UINT)eColliderType::Circle]);

		mEditorObjects.clear();
		mDebugObjects.clear();

		ImGuiRelease();
	}

	void ImGuiMgr::DebugRender(mh::tDebugMesh& mesh)
	{
		DebugObject* debugObj = mDebugObjects[(UINT)mesh.type];
		
		mh::Com_Transform* tr = debugObj->GetComponent<mh::Com_Transform>();
		tr->SetRelativePos(mesh.position);
		tr->SetRelativeRotXYZ(mesh.rotatation);
		

		if (mesh.type == eColliderType::Rect)
			tr->SetRelativeScale(mesh.scale);
		else
			tr->SetRelativeScale(Vector3(mesh.radius));


		mh::IRenderer* renderer = debugObj->GetComponent<mh::IRenderer>();
		mh::Com_Camera* mainCam = mh::RenderMgr::GetMainCam();

		tr->FixedUpdate();

		mh::Com_Camera::SetGpuViewMatrix(
			mainCam->GetViewMatrix());
		mh::Com_Camera::SetGpuProjectionMatrix(mainCam->GetProjectionMatrix());

		debugObj->Render();
	}

	void ImGuiMgr::ImGuiInitialize()
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
		ImGui_ImplWin32_Init(mh::Application::GetHwnd());
		ImGui_ImplDX11_Init(mh::GPUMgr::Device().Get()
			, mh::GPUMgr::Context().Get());

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

	void ImGuiMgr::ImGuiNewFrame()
	{
		// Start the Dear ImGui frame
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		ImGuiIO io = ImGui::GetIO();
	}

	void ImGuiMgr::ImGuiRender()
	{
		bool show_demo_window = false;
		bool show_another_window = false;
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

		//// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		
		// Update and Render additional Platform Windows
		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	}

	void ImGuiMgr::ImGuiRelease()
	{
		// Cleanup
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}
}

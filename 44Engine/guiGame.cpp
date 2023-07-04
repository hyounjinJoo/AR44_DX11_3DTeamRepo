#include "guiGame.h"
#include "yaResources.h"
#include "yaTexture.h"
#include "yaGraphicDevice_DX11.h"

namespace gui
{
	Game::Game()
	{
		SetName("Game");
	}

	Game::~Game()
	{
	}

	void Game::FixedUpdate()
	{
	}

	void Game::Update()
	{
		std::shared_ptr<ya::graphics::Texture> renderTarget
			= ya::Resources::Find<ya::graphics::Texture>(L"RenderTargetTexture");

		std::shared_ptr<ya::graphics::Texture> gameTex
			= std::make_shared<ya::graphics::Texture>();
		gameTex->Create(1600, 900, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE);
		
		//61 번 셰이더 리소스 뷰 null초기화
		ID3D11ShaderResourceView* gameSRV = nullptr;
		ya::graphics::GetDevice()->BindShaderResource(ya::graphics::eShaderStage::PS, 61, &gameSRV);
		ya::graphics::GetDevice()->CopyResource(gameTex->GetTexture().Get()
			, renderTarget->GetTexture().Get());

		gameTex->BindShaderResource(ya::graphics::eShaderStage::PS, 61);

		ImGuiIO io = ImGui::GetIO();
		ImVec2 panelSize = ImGui::GetWindowSize();
		ImVec4 tintColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		ImVec2 scene_size = ImVec2(io.DisplaySize.x, io.DisplaySize.y);

		ImGui::Image((void*)gameTex->GetSRV().Get(), panelSize
				, ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), tintColor);
	}

	void Game::LateUpdate()
	{
	}

}

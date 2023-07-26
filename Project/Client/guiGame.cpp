
#include "ClientPCH.h"
#include "guiGame.h"

#include <Engine/GameResources.h>
#include <Engine/Texture.h>
#include <Engine/GraphicDevice_DX11.h>

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
		std::shared_ptr<mh::GPU::Texture> renderTarget
			= mh::GameResources::Find<mh::GPU::Texture>("RenderTargetTexture");

		std::shared_ptr<mh::GPU::Texture> gameTex
			= std::make_shared<mh::GPU::Texture>();
		gameTex->Create(1600, 900, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE);
		
		//61 번 셰이더 리소스 뷰 null초기화
		ID3D11ShaderResourceView* gameSRV = nullptr;
		mh::GPU::GetDevice()->BindShaderResource(mh::GPU::eShaderStage::PS, 61, &gameSRV);
		mh::GPU::GetDevice()->CopyResource(gameTex->GetTexture().Get()
			, renderTarget->GetTexture().Get());

		gameTex->BindShaderResource(mh::GPU::eShaderStage::PS, 61);

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

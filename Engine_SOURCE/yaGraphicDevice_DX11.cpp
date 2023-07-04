#include "yaGraphicDevice_DX11.h"
#include "yaApplication.h"
#include "yaRenderer.h"
#include "yaConstantBuffer.h"
#include "yaMesh.h"
#include "yaTexture.h"
#include "yaResources.h"

extern ya::Application application;

namespace ya::graphics
{
	GraphicDevice_DX11::GraphicDevice_DX11(ValidationMode validationMode)
	{
		graphics::GetDevice() = this;
		/// <summary>
		/// 1. Device 와 SwapChain 생성한다.
		/// 2. 백버퍼에 실제로 렌더링할 렌더타겟 뷰를 생성해야한다.
		/// 3. 화면을 클리어 해줘야한다. 뷰포트를 생성해줘야 한다.
		/// 4. 매프레임마다 위에서 생성한 렌더타겟뷰에 렌더링해주어야한다.
		/// 5. Swapchain을 이용하여 최종 디바이스(디스플레이)에 화면을 그려줘야한다.
		/// </summary>
		/// <param name="validationMode"></param>

		HWND hwnd = application.GetHwnd();

		// Device, Device Context
		UINT DeviceFlag = D3D11_CREATE_DEVICE_DEBUG;
		D3D_FEATURE_LEVEL FeatureLevel = (D3D_FEATURE_LEVEL)0;

		HRESULT hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr
			, DeviceFlag, nullptr, 0
			, D3D11_SDK_VERSION
			, mDevice.GetAddressOf()
			, &FeatureLevel
			, mContext.GetAddressOf());

		// SwapChain
		DXGI_SWAP_CHAIN_DESC swapChainDesc = {};

		swapChainDesc.OutputWindow = hwnd;
		swapChainDesc.Windowed = true;
		swapChainDesc.BufferCount = 2;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferDesc.Width = application.GetWidth();
		swapChainDesc.BufferDesc.Height = application.GetHeight();
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 144;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;

		if (!CreateSwapChain(&swapChainDesc))
			return;

		mRenderTargetTexture = std::make_shared<Texture>();
		Microsoft::WRL::ComPtr<ID3D11Texture2D> renderTarget;
		// Get rendertarget for swapchain
		hr = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)renderTarget.GetAddressOf());
		mRenderTargetTexture->Create(renderTarget);
		Resources::Insert<Texture>(L"RenderTargetTexture", mRenderTargetTexture);

		D3D11_TEXTURE2D_DESC depthBuffer = {};
		depthBuffer.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL;
		depthBuffer.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
		depthBuffer.CPUAccessFlags = 0;

		depthBuffer.Format = DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthBuffer.Width = application.GetWidth();
		depthBuffer.Height = application.GetHeight();
		depthBuffer.ArraySize = 1;

		depthBuffer.SampleDesc.Count = 1;
		depthBuffer.SampleDesc.Quality = 0;

		depthBuffer.MipLevels = 0;
		depthBuffer.MiscFlags = 0;

		mDepthStencilBufferTexture = std::make_shared<Texture>();
		mDepthStencilBufferTexture->Create(1600, 900, DXGI_FORMAT_D24_UNORM_S8_UINT, D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL);

		RECT winRect;
		GetClientRect(application.GetHwnd(), &winRect);
		mViewPort = { 0.0f, 0.0f, FLOAT(winRect.right - winRect.left), FLOAT(winRect.bottom - winRect.top), 0.0f, 1.0f };
		BindViewports(&mViewPort);
		mContext->OMSetRenderTargets(1, mRenderTargetTexture->GetRTV().GetAddressOf(), mDepthStencilBufferTexture->GetDSV().Get());
	}

	GraphicDevice_DX11::~GraphicDevice_DX11()
	{
		renderer::Release();
	}

	bool GraphicDevice_DX11::CreateSwapChain(DXGI_SWAP_CHAIN_DESC* desc)
	{
		Microsoft::WRL::ComPtr<IDXGIDevice> pDXGIDevice = nullptr;
		Microsoft::WRL::ComPtr<IDXGIAdapter> pDXGIAdapter = nullptr;
		Microsoft::WRL::ComPtr<IDXGIFactory> pDXGIFactory = nullptr;

		if (FAILED(mDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)pDXGIDevice.GetAddressOf())))
			return false;

		if (FAILED(pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)pDXGIAdapter.GetAddressOf())))
			return false;

		if (FAILED(pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void**)pDXGIFactory.GetAddressOf())))
			return false;

		if (FAILED(pDXGIFactory->CreateSwapChain(mDevice.Get(), desc, mSwapChain.GetAddressOf())))
			return false;

		return true;
	}

	bool GraphicDevice_DX11::CreateTexture(D3D11_TEXTURE2D_DESC* desc, ID3D11Texture2D** ppTexture2D)
	{
		if (FAILED(mDevice->CreateTexture2D(desc, nullptr, ppTexture2D)))
			return false;



		return true;
	}

	bool GraphicDevice_DX11::CreateInputLayout(D3D11_INPUT_ELEMENT_DESC* desc, UINT NumElements, const void* byteCode, SIZE_T bytecodeLength, ID3D11InputLayout** ppInputLayout)
	{
		if (FAILED(mDevice->CreateInputLayout(desc, NumElements, byteCode, bytecodeLength, ppInputLayout)))
			return false;

		return true;
	}

	bool GraphicDevice_DX11::CreateBuffer(D3D11_BUFFER_DESC* desc, D3D11_SUBRESOURCE_DATA* data, ID3D11Buffer** buffer)
	{
		if (FAILED(mDevice->CreateBuffer(desc, data, buffer)))
			return false;

		return true;
	}

	bool GraphicDevice_DX11::CreateRenderTargetView(ID3D11Resource* pResource, const D3D11_RENDER_TARGET_VIEW_DESC* pDesc, ID3D11RenderTargetView** ppRTView)
	{
		if (FAILED(mDevice->CreateRenderTargetView(pResource, pDesc, ppRTView)))
			return false;

		return true;
	}

	bool GraphicDevice_DX11::CreateUnorderedAccessView(ID3D11Resource* pResource, const D3D11_UNORDERED_ACCESS_VIEW_DESC* pDesc, ID3D11UnorderedAccessView** ppUAView)
	{
		if (FAILED(mDevice->CreateUnorderedAccessView(pResource, pDesc, ppUAView)))
			return false;

		return true;
	}

	bool GraphicDevice_DX11::CreateDepthStencilView(ID3D11Resource* pResource, const D3D11_DEPTH_STENCIL_VIEW_DESC* pDesc, ID3D11DepthStencilView** ppDepthStencilView)
	{
		if (FAILED(mDevice->CreateDepthStencilView(pResource, pDesc, ppDepthStencilView)))
			return false;

		return true;
	}

	bool GraphicDevice_DX11::CreateShaderResourceView(ID3D11Resource* pResource, const D3D11_SHADER_RESOURCE_VIEW_DESC* pDesc, ID3D11ShaderResourceView** ppSRView)
	{
		if (FAILED(mDevice->CreateShaderResourceView(pResource, pDesc, ppSRView)))
			return false;

		return true;
	}

	bool GraphicDevice_DX11::CreateVertexShader(const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage, ID3D11VertexShader** ppVertexShader)
	{
		if (FAILED(mDevice->CreateVertexShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppVertexShader)))
			return false;

		return true;
	}

	bool GraphicDevice_DX11::CreateGeometryShader(const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage, ID3D11GeometryShader** ppGeometryShader)
	{
		if (FAILED(mDevice->CreateGeometryShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppGeometryShader)))
			return false;

		return true;
	}

	bool GraphicDevice_DX11::CreatePixelShader(const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage, ID3D11PixelShader** ppPixelShader)
	{
		if (FAILED(mDevice->CreatePixelShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppPixelShader)))
			return false;

		return true;
	}

	bool GraphicDevice_DX11::CreateComputeShader(const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage, ID3D11ComputeShader** ppComputeShader)
	{
		if (FAILED(mDevice->CreateComputeShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppComputeShader)))
			return false;
		
		return true;
	}

	bool GraphicDevice_DX11::CreateSamplerState(const D3D11_SAMPLER_DESC* pSamplerDesc, ID3D11SamplerState** ppSamplerState)
	{
		if (FAILED(mDevice->CreateSamplerState(pSamplerDesc, ppSamplerState)))
			return false;

		return true;
	}

	bool GraphicDevice_DX11::CreateRasterizerState(const D3D11_RASTERIZER_DESC* pRasterizerDesc, ID3D11RasterizerState** ppRasterizerState)
	{
		if (FAILED(mDevice->CreateRasterizerState(pRasterizerDesc, ppRasterizerState)))
			return false;

		return true;
	}

	bool GraphicDevice_DX11::CreateDepthStencilState(const D3D11_DEPTH_STENCIL_DESC* pDepthStencilDesc, ID3D11DepthStencilState** ppDepthStencilState)
	{
		if (FAILED(mDevice->CreateDepthStencilState(pDepthStencilDesc, ppDepthStencilState)))
			return false;

		return true;
	}

	bool GraphicDevice_DX11::CreateBlendState(const D3D11_BLEND_DESC* pBlendStateDesc, ID3D11BlendState** ppBlendState)
	{
		if (FAILED(mDevice->CreateBlendState(pBlendStateDesc, ppBlendState)))
			return false;

		return true;
	}

	void GraphicDevice_DX11::BindPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY topology)
	{
		mContext->IASetPrimitiveTopology(topology);
	}

	void GraphicDevice_DX11::BindInputLayout(ID3D11InputLayout* pInputLayout)
	{
		mContext->IASetInputLayout(pInputLayout);
	}

	void GraphicDevice_DX11::BindVertexBuffer(UINT StartSlot
		, UINT NumBuffers
		, ID3D11Buffer* const* ppVertexBuffers
		, const UINT* pStrides
		, const UINT* pOffsets)
	{
		mContext->IASetVertexBuffers(StartSlot, NumBuffers, ppVertexBuffers, pStrides, pOffsets);
	}

	void GraphicDevice_DX11::BindIndexBuffer(ID3D11Buffer* pIndexBuffer, DXGI_FORMAT Format, UINT Offset)
	{
		mContext->IASetIndexBuffer(pIndexBuffer, Format, Offset);
	}

	void GraphicDevice_DX11::BindVertexShader(ID3D11VertexShader* pVertexShader, ID3D11ClassInstance* const* ppClassInstances, UINT NumClassInstances)
	{
		mContext->VSSetShader(pVertexShader, ppClassInstances, NumClassInstances);
	}

	void GraphicDevice_DX11::BindHullShader(ID3D11HullShader* pVertexShader, ID3D11ClassInstance* const* ppClassInstances, UINT NumClassInstances)
	{
		mContext->HSSetShader(pVertexShader, ppClassInstances, NumClassInstances);
	}

	void GraphicDevice_DX11::BindDomainShader(ID3D11DomainShader* pVertexShader, ID3D11ClassInstance* const* ppClassInstances, UINT NumClassInstances)
	{
		mContext->DSSetShader(pVertexShader, ppClassInstances, NumClassInstances);
	}

	void GraphicDevice_DX11::BindGeometryShader(ID3D11GeometryShader* pVertexShader, ID3D11ClassInstance* const* ppClassInstances, UINT NumClassInstances)
	{
		mContext->GSSetShader(pVertexShader, ppClassInstances, NumClassInstances);
	}

	void GraphicDevice_DX11::BindPixelShader(ID3D11PixelShader* pPixelShader, ID3D11ClassInstance* const* ppClassInstances, UINT NumClassInstances)
	{
		mContext->PSSetShader(pPixelShader, ppClassInstances, NumClassInstances);
	}

	void GraphicDevice_DX11::BindComputeShader(ID3D11ComputeShader* pComputeShader, ID3D11ClassInstance* const* ppClassInstances, UINT NumClassInstances)
	{
		mContext->CSSetShader(pComputeShader, ppClassInstances, NumClassInstances);
	}

	void GraphicDevice_DX11::Dispatch(UINT ThreadGroupCountX, UINT ThreadGroupCountY, UINT ThreadGroupCountZ)
	{
		mContext->Dispatch(ThreadGroupCountX, ThreadGroupCountY, ThreadGroupCountZ);
	}

	void GraphicDevice_DX11::BindViewports(D3D11_VIEWPORT* viewPort)
	{
		mContext->RSSetViewports(1, viewPort);
	}

	void GraphicDevice_DX11::SetData(ID3D11Buffer* buffer, void* data, UINT size)
	{
		D3D11_MAPPED_SUBRESOURCE sub = {};
		mContext->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &sub);
		memcpy(sub.pData, data, size);
		mContext->Unmap(buffer, 0);
	}

	void GraphicDevice_DX11::BindConstantBuffer(eShaderStage stage, eCBType type, ID3D11Buffer* buffer)
	{
		switch (stage)
		{
		case ya::graphics::eShaderStage::VS:
			mContext->VSSetConstantBuffers((UINT)type, 1, &buffer);
			break;
		case ya::graphics::eShaderStage::HS:
			mContext->HSSetConstantBuffers((UINT)type, 1, &buffer);
			break;
		case ya::graphics::eShaderStage::DS:
			mContext->DSSetConstantBuffers((UINT)type, 1, &buffer);
			break;
		case ya::graphics::eShaderStage::GS:
			mContext->GSSetConstantBuffers((UINT)type, 1, &buffer);
			break;
		case ya::graphics::eShaderStage::PS:
			mContext->PSSetConstantBuffers((UINT)type, 1, &buffer);
			break;
		case ya::graphics::eShaderStage::CS:
			mContext->CSSetConstantBuffers((UINT)type, 1, &buffer);
			break;
		default:
			break;
		}
	}

	void GraphicDevice_DX11::BindShaderResource(eShaderStage stage, UINT slot
		, ID3D11ShaderResourceView* const* ppShaderResourceViews)
	{
		switch (stage)
		{
		case ya::graphics::eShaderStage::VS:
			mContext->VSSetShaderResources(slot, 1, ppShaderResourceViews);
			break;
		case ya::graphics::eShaderStage::HS:
			mContext->HSSetShaderResources(slot, 1, ppShaderResourceViews);
			break;
		case ya::graphics::eShaderStage::DS:
			mContext->DSSetShaderResources(slot, 1, ppShaderResourceViews);
			break;
		case ya::graphics::eShaderStage::GS:
			mContext->GSSetShaderResources(slot, 1, ppShaderResourceViews);
			break;
		case ya::graphics::eShaderStage::PS:
			mContext->PSSetShaderResources(slot, 1, ppShaderResourceViews);
			break;
		case ya::graphics::eShaderStage::CS:
			mContext->CSSetShaderResources(slot, 1, ppShaderResourceViews);
			break;
		default:
			break;
		}
	}

	void GraphicDevice_DX11::BindUnorderdAccessView(UINT startSlot, UINT NumUAVs
		, ID3D11UnorderedAccessView* const* ppUnorderedAccessViews , const UINT* pUAVInitialCounts)
	{
		mContext->CSSetUnorderedAccessViews(startSlot, NumUAVs, ppUnorderedAccessViews, pUAVInitialCounts);
	}

	void GraphicDevice_DX11::BindSamplers(eShaderStage stage, UINT slot, UINT NumSamplers, ID3D11SamplerState* const* ppSamplers)
	{
		switch (stage)
		{
		case ya::graphics::eShaderStage::VS:
			mContext->VSSetSamplers(slot, NumSamplers, ppSamplers);
			break;
		case ya::graphics::eShaderStage::HS:
			mContext->HSSetSamplers(slot, NumSamplers, ppSamplers);
			break;
		case ya::graphics::eShaderStage::DS:
			mContext->DSSetSamplers(slot, NumSamplers, ppSamplers);
			break;
		case ya::graphics::eShaderStage::GS:
			mContext->GSSetSamplers(slot, NumSamplers, ppSamplers);
			break;
		case ya::graphics::eShaderStage::PS:
			mContext->PSSetSamplers(slot, NumSamplers, ppSamplers);
			break;
		case ya::graphics::eShaderStage::CS:
			mContext->CSSetSamplers(slot, NumSamplers, ppSamplers);
			break;
		default:
			break;
		}
	}

	void GraphicDevice_DX11::BindsSamplers(UINT slot, UINT NumSamplers, ID3D11SamplerState* const* ppSamplers)
	{
		mContext->VSSetSamplers(slot, NumSamplers, ppSamplers);
		mContext->HSSetSamplers(slot, NumSamplers, ppSamplers);
		mContext->DSSetSamplers(slot, NumSamplers, ppSamplers);
		mContext->GSSetSamplers(slot, NumSamplers, ppSamplers);
		mContext->PSSetSamplers(slot, NumSamplers, ppSamplers);
	}

	void GraphicDevice_DX11::BindRasterizerState(ID3D11RasterizerState* pRasterizerState)
	{
		mContext->RSSetState(pRasterizerState);
	}

	void GraphicDevice_DX11::BindDepthStencilState(ID3D11DepthStencilState* pDepthStencilState)
	{
		mContext->OMSetDepthStencilState(pDepthStencilState, 0);
	}

	void GraphicDevice_DX11::BindBlendState(ID3D11BlendState* pBlendState)
	{
		mContext->OMSetBlendState(pBlendState, nullptr, 0xffffffff);
	}

	void GraphicDevice_DX11::CopyResource(ID3D11Resource* pDstResource, ID3D11Resource* pSrcResource)
	{
		mContext->CopyResource(pDstResource, pSrcResource);
	}

	void GraphicDevice_DX11::Clear()
	{
		// 화면 지워주기
		FLOAT backgroundColor[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
		mContext->ClearRenderTargetView(mRenderTargetTexture->GetRTV().Get(), backgroundColor);
		mContext->ClearDepthStencilView(mDepthStencilBufferTexture->GetDSV().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
	}

	void GraphicDevice_DX11::AdjustViewPorts()
	{
		// ViewPort, RenderTaget
		RECT winRect;
		GetClientRect(application.GetHwnd(), &winRect);
		mViewPort = { 0.0f, 0.0f, FLOAT(winRect.right - winRect.left), FLOAT(winRect.bottom - winRect.top), 0.0f, 1.0f };
		BindViewports(&mViewPort);
		mContext->OMSetRenderTargets(1, mRenderTargetTexture->GetRTV().GetAddressOf(), mDepthStencilBufferTexture->GetDSV().Get());
	}

	void GraphicDevice_DX11::Draw()
	{
		mContext->Draw(0, 0);
	}

	void GraphicDevice_DX11::DrawIndexed(UINT indexCount, UINT StartIndexLocation, UINT BaseVertexLocation)
	{
		mContext->DrawIndexed(indexCount, StartIndexLocation, BaseVertexLocation);
		
	}

	void GraphicDevice_DX11::DrawIndexedInstanced(UINT IndexCountPerInstance, UINT InstanceCount, UINT StartIndexLocation, INT BaseVertexLocation, UINT StartInstanceLocation)
	{
		mContext->DrawIndexedInstanced(IndexCountPerInstance, InstanceCount, StartIndexLocation, BaseVertexLocation, StartInstanceLocation);
	}



	void GraphicDevice_DX11::Present()
	{
		mSwapChain->Present(1, 0);
	}

}



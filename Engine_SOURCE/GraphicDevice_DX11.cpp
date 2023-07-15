#include "GraphicDevice_DX11.h"

#include "Application.h"
#include "ConstantBuffer.h"
#include "Mesh.h"
#include "Renderer.h"
#include "Resources.h"
#include "Texture.h"

extern mh::Application application;

namespace mh::graphics
{
	GraphicDevice_DX11::GraphicDevice_DX11(eValidationMode _validationMode)
	{
		graphics::GetDevice() = this;

		/// 1. Device 와 SwapChain 생성한다.
		/// 2. 백버퍼에 실제로 렌더링할 렌더타겟 뷰를 생성해야한다.
		/// 3. 화면을 클리어 해줘야한다. 뷰포트를 생성해줘야 한다.
		/// 4. 매프레임마다 위에서 생성한 렌더타겟뷰에 렌더링해주어야한다.
		/// 5. Swapchain을 이용하여 최종 디바이스(디스플레이)에 화면을 그려줘야한다.
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
		{
			return;
		}

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

	bool GraphicDevice_DX11::CreateSwapChain(DXGI_SWAP_CHAIN_DESC* _desc)
	{
		Microsoft::WRL::ComPtr<IDXGIDevice> pDXGIDevice = nullptr;
		Microsoft::WRL::ComPtr<IDXGIAdapter> pDXGIAdapter = nullptr;
		Microsoft::WRL::ComPtr<IDXGIFactory> pDXGIFactory = nullptr;

		if (FAILED(mDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)pDXGIDevice.GetAddressOf())))
		{
			return;
		}

		if (FAILED(pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)pDXGIAdapter.GetAddressOf())))
		{
			return false;
		}

		if (FAILED(pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void**)pDXGIFactory.GetAddressOf())))
		{
			return false;
		}

		if (FAILED(pDXGIFactory->CreateSwapChain(mDevice.Get(), _desc, mSwapChain.GetAddressOf())))
		{
			return false;
		}

		return true;
	}

	bool GraphicDevice_DX11::CreateTexture(D3D11_TEXTURE2D_DESC* _desc, ID3D11Texture2D** _ppTexture2D)
	{
		if (FAILED(mDevice->CreateTexture2D(_desc, nullptr, _ppTexture2D)))
		{
			return false;
		}

		return true;
	}

	bool GraphicDevice_DX11::CreateInputLayout(D3D11_INPUT_ELEMENT_DESC* _desc, UINT _NumElements, const void* _byteCode, SIZE_T _byteCodeLength, ID3D11InputLayout** _ppInputLayout)
	{
		if (FAILED(mDevice->CreateInputLayout(_desc, _NumElements, _byteCode, _byteCodeLength, _ppInputLayout)))
		{
			return false;
		}

		return true;
	}

	bool GraphicDevice_DX11::CreateBuffer(D3D11_BUFFER_DESC* _desc, D3D11_SUBRESOURCE_DATA* _data, ID3D11Buffer** _buffer)
	{
		if (FAILED(mDevice->CreateBuffer(_desc, _data, _buffer)))
		{
			return false;
		}

		return true;
	}

	bool GraphicDevice_DX11::CreateRenderTargetView(ID3D11Resource* _pResource, const D3D11_RENDER_TARGET_VIEW_DESC* _pDesc, ID3D11RenderTargetView** _ppRTView)
	{
		if (FAILED(mDevice->CreateRenderTargetView(_pResource, _pDesc, _ppRTView)))
		{
			return false;
		}

		return true;
	}

	bool GraphicDevice_DX11::CreateUnorderedAccessView(ID3D11Resource* _pResource, const D3D11_UNORDERED_ACCESS_VIEW_DESC* _pDesc, ID3D11UnorderedAccessView** _ppUAView)
	{
		if (FAILED(mDevice->CreateUnorderedAccessView(_pResource, _pDesc, _ppUAView)))
		{
			return false;
		}

		return true;
	}

	bool GraphicDevice_DX11::CreateDepthStencilView(ID3D11Resource* _pResource, const D3D11_DEPTH_STENCIL_VIEW_DESC* _pDesc, ID3D11DepthStencilView** _ppDepthStencilView)
	{
		if (FAILED(mDevice->CreateDepthStencilView(_pResource, _pDesc, _ppDepthStencilView)))
		{
			return false;
		}

		return true;
	}

	bool GraphicDevice_DX11::CreateShaderResourceView(ID3D11Resource* _pResource, const D3D11_SHADER_RESOURCE_VIEW_DESC* _pDesc, ID3D11ShaderResourceView** _ppSRView)
	{
		if (FAILED(mDevice->CreateShaderResourceView(_pResource, _pDesc, _ppSRView)))
		{
			return false;
		}

		return true;
	}

	bool GraphicDevice_DX11::CreateVertexShader(const void* _pShaderByteCode, SIZE_T _byteCodeLength, ID3D11ClassLinkage* _pClassLinkage, ID3D11VertexShader** _ppVertexShader)
	{
		if (FAILED(mDevice->CreateVertexShader(_pShaderByteCode, _byteCodeLength, _pClassLinkage, _ppVertexShader)))
		{
			return false;
		}

		return true;
	}

	bool GraphicDevice_DX11::CreateGeometryShader(const void* _pShaderByteCode, SIZE_T _bytecodeLength, ID3D11ClassLinkage* _pClassLinkage, ID3D11GeometryShader** _ppGeometryShader)
	{
		if (FAILED(mDevice->CreateGeometryShader(_pShaderByteCode, _bytecodeLength, _pClassLinkage, _ppGeometryShader)))
		{
			return false;
		}

		return true;
	}

	bool GraphicDevice_DX11::CreatePixelShader(const void* _pShaderByteCode, SIZE_T _byteCodeLength, ID3D11ClassLinkage* _pClassLinkage, ID3D11PixelShader** _ppPixelShader)
	{
		if (FAILED(mDevice->CreatePixelShader(_pShaderByteCode, _byteCodeLength, _pClassLinkage, _ppPixelShader)))
		{
			return false;
		}

		return true;
	}

	bool GraphicDevice_DX11::CreateComputeShader(const void* _pShaderByteCode, SIZE_T _byteCodeLength, ID3D11ClassLinkage* _pClassLinkage, ID3D11ComputeShader** _ppComputeShader)
	{
		if (FAILED(mDevice->CreateComputeShader(_pShaderByteCode, _byteCodeLength, _pClassLinkage, _ppComputeShader)))
		{
			return false;
		}

		return true;
	}

	bool GraphicDevice_DX11::CreateSamplerState(const D3D11_SAMPLER_DESC* _pSamplerDesc, ID3D11SamplerState** _ppSamplerState)
	{
		if (FAILED(mDevice->CreateSamplerState(_pSamplerDesc, _ppSamplerState)))
		{
			return false;
		}

		return true;
	}

	bool GraphicDevice_DX11::CreateRasterizerState(const D3D11_RASTERIZER_DESC* _pRasterizerDesc, ID3D11RasterizerState** _ppRasterizerState)
	{
		if (FAILED(mDevice->CreateRasterizerState(_pRasterizerDesc, _ppRasterizerState)))
		{
			return false;
		}

		return true;
	}

	bool GraphicDevice_DX11::CreateDepthStencilState(const D3D11_DEPTH_STENCIL_DESC* _pDepthStencilDesc, ID3D11DepthStencilState** _ppDepthStencilState)
	{
		if (FAILED(mDevice->CreateDepthStencilState(_pDepthStencilDesc, _ppDepthStencilState)))
		{
			return false;
		}

		return true;
	}

	bool GraphicDevice_DX11::CreateBlendState(const D3D11_BLEND_DESC* _pBlendStateDesc, ID3D11BlendState** _ppBlendState)
	{
		if (FAILED(mDevice->CreateBlendState(_pBlendStateDesc, _ppBlendState)))
		{
			return false;
		}

		return true;
	}

	void GraphicDevice_DX11::BindPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY _topology) const
	{
		mContext->IASetPrimitiveTopology(_topology);
	}

	void GraphicDevice_DX11::BindInputLayout(ID3D11InputLayout* _pInputLayout) const
	{
		mContext->IASetInputLayout(_pInputLayout);
	}

	void GraphicDevice_DX11::BindVertexBuffer(UINT _startSlot
		, UINT _NumBuffers
		, ID3D11Buffer* const* _ppVertexBuffers
		, const UINT* _pStrides
		, const UINT* _pOffsets) const
	{
		mContext->IASetVertexBuffers(_startSlot, _NumBuffers, _ppVertexBuffers, _pStrides, _pOffsets);
	}

	void GraphicDevice_DX11::BindIndexBuffer(ID3D11Buffer* _pIndexBuffer, DXGI_FORMAT _format, UINT _offset) const
	{
		mContext->IASetIndexBuffer(_pIndexBuffer, _format, _offset);
	}

	void GraphicDevice_DX11::BindVertexShader(ID3D11VertexShader* _pVertexShader, ID3D11ClassInstance* const* _ppClassInstances, UINT _NumClassInstances) const
	{
		mContext->VSSetShader(_pVertexShader, _ppClassInstances, _NumClassInstances);
	}

	void GraphicDevice_DX11::BindHullShader(ID3D11HullShader* _pVertexShader, ID3D11ClassInstance* const* _ppClassInstances, UINT _NumClassInstances) const
	{
		mContext->HSSetShader(_pVertexShader, _ppClassInstances, _NumClassInstances);
	}

	void GraphicDevice_DX11::BindDomainShader(ID3D11DomainShader* _pVertexShader, ID3D11ClassInstance* const* _ppClassInstances, UINT _NumClassInstances) const
	{
		mContext->DSSetShader(_pVertexShader, _ppClassInstances, _NumClassInstances);
	}

	void GraphicDevice_DX11::BindGeometryShader(ID3D11GeometryShader* _pVertexShader, ID3D11ClassInstance* const* _ppClassInstances, UINT _NumClassInstances) const
	{
		mContext->GSSetShader(_pVertexShader, _ppClassInstances, _NumClassInstances);
	}

	void GraphicDevice_DX11::BindPixelShader(ID3D11PixelShader* _pPixelShader, ID3D11ClassInstance* const* _ppClassInstances, UINT _NumClassInstances) const
	{
		mContext->PSSetShader(_pPixelShader, _ppClassInstances, _NumClassInstances);
	}

	void GraphicDevice_DX11::BindComputeShader(ID3D11ComputeShader* _pComputeShader, ID3D11ClassInstance* const* _ppClassInstances, UINT _NumClassInstances) const
	{
		mContext->CSSetShader(_pComputeShader, _ppClassInstances, _NumClassInstances);
	}

	void GraphicDevice_DX11::Dispatch(UINT _threadGroupCountX, UINT _threadGroupCountY, UINT _threadGroupCountZ) const
	{
		mContext->Dispatch(_threadGroupCountX, _threadGroupCountY, _threadGroupCountZ);
	}

	void GraphicDevice_DX11::BindViewports(D3D11_VIEWPORT* _viewPort) const
	{
		mContext->RSSetViewports(1, _viewPort);
	}

	void GraphicDevice_DX11::BindConstantBuffer(eShaderStage _stage, eCBType _type, ID3D11Buffer* _buffer) const
	{
		switch (_stage)
		{
		case mh::graphics::eShaderStage::VS:
			mContext->VSSetConstantBuffers((UINT)_type, 1, &_buffer);
			break;
		case mh::graphics::eShaderStage::HS:
			mContext->HSSetConstantBuffers((UINT)_type, 1, &_buffer);
			break;
		case mh::graphics::eShaderStage::DS:
			mContext->DSSetConstantBuffers((UINT)_type, 1, &_buffer);
			break;
		case mh::graphics::eShaderStage::GS:
			mContext->GSSetConstantBuffers((UINT)_type, 1, &_buffer);
			break;
		case mh::graphics::eShaderStage::PS:
			mContext->PSSetConstantBuffers((UINT)_type, 1, &_buffer);
			break;
		case mh::graphics::eShaderStage::CS:
			mContext->CSSetConstantBuffers((UINT)_type, 1, &_buffer);
			break;
		default:
			assert(true);
			break;
		}
	}

	void GraphicDevice_DX11::BindShaderResource(eShaderStage _stage, UINT _slot
		, ID3D11ShaderResourceView* const* _ppShaderResourceViews) const
	{
		switch (_stage)
		{
		case mh::graphics::eShaderStage::VS:
			mContext->VSSetShaderResources(_slot, 1, _ppShaderResourceViews);
			break;
		case mh::graphics::eShaderStage::HS:
			mContext->HSSetShaderResources(_slot, 1, _ppShaderResourceViews);
			break;
		case mh::graphics::eShaderStage::DS:
			mContext->DSSetShaderResources(_slot, 1, _ppShaderResourceViews);
			break;
		case mh::graphics::eShaderStage::GS:
			mContext->GSSetShaderResources(_slot, 1, _ppShaderResourceViews);
			break;
		case mh::graphics::eShaderStage::PS:
			mContext->PSSetShaderResources(_slot, 1, _ppShaderResourceViews);
			break;
		case mh::graphics::eShaderStage::CS:
			mContext->CSSetShaderResources(_slot, 1, _ppShaderResourceViews);
			break;
		default:
			assert(true);
			break;
		}
	}

	void GraphicDevice_DX11::BindUnorderdAccessView(UINT _startSlot, UINT _NumUAVs
		, ID3D11UnorderedAccessView* const* _ppUnorderedAccessViews, const UINT* _pUAVInitialCounts) const
	{
		mContext->CSSetUnorderedAccessViews(_startSlot, _NumUAVs, _ppUnorderedAccessViews, _pUAVInitialCounts);
	}

	void GraphicDevice_DX11::BindSamplers(eShaderStage _stage, UINT _slot, UINT _NumSamplers, ID3D11SamplerState* const* _ppSamplers) const
	{
		switch (_stage)
		{
		case mh::graphics::eShaderStage::VS:
			mContext->VSSetSamplers(_slot, _NumSamplers, _ppSamplers);
			break;
		case mh::graphics::eShaderStage::HS:
			mContext->HSSetSamplers(_slot, _NumSamplers, _ppSamplers);
			break;
		case mh::graphics::eShaderStage::DS:
			mContext->DSSetSamplers(_slot, _NumSamplers, _ppSamplers);
			break;
		case mh::graphics::eShaderStage::GS:
			mContext->GSSetSamplers(_slot, _NumSamplers, _ppSamplers);
			break;
		case mh::graphics::eShaderStage::PS:
			mContext->PSSetSamplers(_slot, _NumSamplers, _ppSamplers);
			break;
		case mh::graphics::eShaderStage::CS:
			mContext->CSSetSamplers(_slot, _NumSamplers, _ppSamplers);
			break;
		default:
			assert(true);
			break;
		}
	}

	void GraphicDevice_DX11::BindsSamplers(UINT _slot, UINT _NumSamplers, ID3D11SamplerState* const* _ppSamplers) const
	{
		mContext->VSSetSamplers(_slot, _NumSamplers, _ppSamplers);
		mContext->HSSetSamplers(_slot, _NumSamplers, _ppSamplers);
		mContext->DSSetSamplers(_slot, _NumSamplers, _ppSamplers);
		mContext->GSSetSamplers(_slot, _NumSamplers, _ppSamplers);
		mContext->PSSetSamplers(_slot, _NumSamplers, _ppSamplers);
	}

	void GraphicDevice_DX11::BindRasterizerState(ID3D11RasterizerState* _pRasterizerState) const
	{
		mContext->RSSetState(_pRasterizerState);
	}

	void GraphicDevice_DX11::BindDepthStencilState(ID3D11DepthStencilState* _pDepthStencilState) const
	{
		mContext->OMSetDepthStencilState(_pDepthStencilState, 0);
	}

	void GraphicDevice_DX11::BindBlendState(ID3D11BlendState* _pBlendState) const
	{
		mContext->OMSetBlendState(_pBlendState, nullptr, 0xffffffff);
	}

	void GraphicDevice_DX11::CopyResource(ID3D11Resource* _pDstResource, ID3D11Resource* _pSrcResource)
	{
		mContext->CopyResource(_pDstResource, _pSrcResource);
	}

	void GraphicDevice_DX11::SetData(ID3D11Buffer* _buffer, void* _data, UINT _size)
	{
		D3D11_MAPPED_SUBRESOURCE sub = {};
		mContext->Map(_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &sub);
		memcpy(sub.pData, _data, _size);
		mContext->Unmap(_buffer, 0);
	}

	void GraphicDevice_DX11::Clear() const
	{
		FLOAT backgroundColor[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
		mContext->ClearRenderTargetView(mRenderTargetTexture->GetRTV().Get(), backgroundColor);
		mContext->ClearDepthStencilView(mDepthStencilBufferTexture->GetDSV().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
	}

	void GraphicDevice_DX11::AdjustViewPorts()
	{
		RECT winRect;
		GetClientRect(application.GetHwnd(), &winRect);
		mViewPort = { 0.0f, 0.0f, FLOAT(winRect.right - winRect.left), FLOAT(winRect.bottom - winRect.top), 0.0f, 1.0f };
		BindViewports(&mViewPort);

		mContext->OMSetRenderTargets(1, mRenderTargetTexture->GetRTV().GetAddressOf(), mDepthStencilBufferTexture->GetDSV().Get());
	}

	void GraphicDevice_DX11::OMSetRenderTarget()
	{
		mContext->OMSetRenderTargets(1
			, mRenderTargetTexture->GetRTV().GetAddressOf()
			, mDepthStencilBufferTexture->GetDSV().Get());
	}

	void GraphicDevice_DX11::Draw()
	{
		mContext->Draw(0, 0);
	}

	void GraphicDevice_DX11::DrawIndexed(UINT _indexCount, UINT _startIndexLocation, UINT _baseVertexLocation)
	{
		mContext->DrawIndexed(_indexCount, _startIndexLocation, _baseVertexLocation);

	}

	void GraphicDevice_DX11::DrawIndexedInstanced(UINT _indexCountPerInstance, UINT _instanceCount, UINT _startIndexLocation, INT _baseVertexLocation, UINT _startInstanceLocation)
	{
		mContext->DrawIndexedInstanced(_indexCountPerInstance, _instanceCount, _startIndexLocation, _baseVertexLocation, _startInstanceLocation);
	}

	void GraphicDevice_DX11::Present()
	{
		mSwapChain->Present(0, 0);
	}
}



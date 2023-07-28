#pragma once
#include "GPUMgr.h"

namespace mh
{

	inline bool GPUMgr::CreateTexture(D3D11_TEXTURE2D_DESC* _desc, ID3D11Texture2D** _ppTexture2D)
	{
		if (FAILED(mDevice->CreateTexture2D(_desc, nullptr, _ppTexture2D)))
		{
			return false;
		}

		return true;
	}

	inline bool GPUMgr::CreateInputLayout(D3D11_INPUT_ELEMENT_DESC* _desc, UINT _NumElements, const void* _byteCode, SIZE_T _byteCodeLength, ID3D11InputLayout** _ppInputLayout)
	{
		if (FAILED(mDevice->CreateInputLayout(_desc, _NumElements, _byteCode, _byteCodeLength, _ppInputLayout)))
		{
			return false;
		}

		return true;
	}

	inline bool GPUMgr::CreateBuffer(D3D11_BUFFER_DESC* _desc, D3D11_SUBRESOURCE_DATA* _data, ID3D11Buffer** _buffer)
	{
		if (FAILED(mDevice->CreateBuffer(_desc, _data, _buffer)))
		{
			return false;
		}

		return true;
	}

	bool GPUMgr::CreateRenderTargetView(ID3D11Resource* _pResource, const D3D11_RENDER_TARGET_VIEW_DESC* _pDesc, ID3D11RenderTargetView** _ppRTView)
	{
		if (FAILED(mDevice->CreateRenderTargetView(_pResource, _pDesc, _ppRTView)))
		{
			return false;
		}

		return true;
	}

	inline bool GPUMgr::CreateUnorderedAccessView(ID3D11Resource* _pResource, const D3D11_UNORDERED_ACCESS_VIEW_DESC* _pDesc, ID3D11UnorderedAccessView** _ppUAView)
	{
		if (FAILED(mDevice->CreateUnorderedAccessView(_pResource, _pDesc, _ppUAView)))
		{
			return false;
		}

		return true;
	}

	inline bool GPUMgr::CreateDepthStencilView(ID3D11Resource* _pResource, const D3D11_DEPTH_STENCIL_VIEW_DESC* _pDesc, ID3D11DepthStencilView** _ppDepthStencilView)
	{
		if (FAILED(mDevice->CreateDepthStencilView(_pResource, _pDesc, _ppDepthStencilView)))
		{
			return false;
		}

		return true;
	}

	inline bool GPUMgr::CreateShaderResourceView(ID3D11Resource* _pResource, const D3D11_SHADER_RESOURCE_VIEW_DESC* _pDesc, ID3D11ShaderResourceView** _ppSRView)
	{
		if (FAILED(mDevice->CreateShaderResourceView(_pResource, _pDesc, _ppSRView)))
		{
			return false;
		}

		return true;
	}

	inline bool GPUMgr::CreateVertexShader(const void* _pShaderByteCode, SIZE_T _byteCodeLength, ID3D11ClassLinkage* _pClassLinkage, ID3D11VertexShader** _ppVertexShader)
	{
		if (FAILED(mDevice->CreateVertexShader(_pShaderByteCode, _byteCodeLength, _pClassLinkage, _ppVertexShader)))
		{
			return false;
		}

		return true;
	}

	inline bool GPUMgr::CreateHullShader(const void* _pShaderByteCode, SIZE_T _byteCodeLength, ID3D11ClassLinkage* _pClassLinkage, ID3D11HullShader** _ppHullShader)
	{
		if (FAILED(mDevice->CreateHullShader(_pShaderByteCode, _byteCodeLength, _pClassLinkage, _ppHullShader)))
		{
			return false;
		}

		return true;
	}

	inline bool GPUMgr::CreateDomainShader(const void* _pShaderByteCode, SIZE_T _byteCodeLength, ID3D11ClassLinkage* _pClassLinkage, ID3D11DomainShader** _ppDomainShader)
	{
		if (FAILED(mDevice->CreateDomainShader(_pShaderByteCode, _byteCodeLength, _pClassLinkage, _ppDomainShader)))
		{
			return false;
		}

		return true;
	}

	inline bool GPUMgr::CreateGeometryShader(const void* _pShaderByteCode, SIZE_T _bytecodeLength, ID3D11ClassLinkage* _pClassLinkage, ID3D11GeometryShader** _ppGeometryShader)
	{
		if (FAILED(mDevice->CreateGeometryShader(_pShaderByteCode, _bytecodeLength, _pClassLinkage, _ppGeometryShader)))
		{
			return false;
		}

		return true;
	}

	inline bool GPUMgr::CreatePixelShader(const void* _pShaderByteCode, SIZE_T _byteCodeLength, ID3D11ClassLinkage* _pClassLinkage, ID3D11PixelShader** _ppPixelShader)
	{
		if (FAILED(mDevice->CreatePixelShader(_pShaderByteCode, _byteCodeLength, _pClassLinkage, _ppPixelShader)))
		{
			return false;
		}

		return true;
	}

	inline bool GPUMgr::CreateComputeShader(const void* _pShaderByteCode, SIZE_T _byteCodeLength, ID3D11ClassLinkage* _pClassLinkage, ID3D11ComputeShader** _ppComputeShader)
	{
		if (FAILED(mDevice->CreateComputeShader(_pShaderByteCode, _byteCodeLength, _pClassLinkage, _ppComputeShader)))
		{
			return false;
		}

		return true;
	}

	inline bool GPUMgr::CreateSamplerState(const D3D11_SAMPLER_DESC* _pSamplerDesc, ID3D11SamplerState** _ppSamplerState)
	{
		if (FAILED(mDevice->CreateSamplerState(_pSamplerDesc, _ppSamplerState)))
		{
			return false;
		}

		return true;
	}

	inline bool GPUMgr::CreateRasterizerState(const D3D11_RASTERIZER_DESC* _pRasterizerDesc, ID3D11RasterizerState** _ppRasterizerState)
	{
		if (FAILED(mDevice->CreateRasterizerState(_pRasterizerDesc, _ppRasterizerState)))
		{
			return false;
		}

		return true;
	}

	inline bool GPUMgr::CreateDepthStencilState(const D3D11_DEPTH_STENCIL_DESC* _pDepthStencilDesc, ID3D11DepthStencilState** _ppDepthStencilState)
	{
		if (FAILED(mDevice->CreateDepthStencilState(_pDepthStencilDesc, _ppDepthStencilState)))
		{
			return false;
		}

		return true;
	}

	inline bool GPUMgr::CreateBlendState(const D3D11_BLEND_DESC* _pBlendStateDesc, ID3D11BlendState** _ppBlendState)
	{
		if (FAILED(mDevice->CreateBlendState(_pBlendStateDesc, _ppBlendState)))
		{
			return false;
		}

		return true;
	}

	inline void GPUMgr::BindPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY _topology) const
	{
		mContext->IASetPrimitiveTopology(_topology);
	}

	inline void GPUMgr::BindInputLayout(ID3D11InputLayout* _pInputLayout) const
	{
		mContext->IASetInputLayout(_pInputLayout);
	}

	inline void GPUMgr::BindVertexBuffer(UINT _startSlot
		, UINT _NumBuffers
		, ID3D11Buffer* const* _ppVertexBuffers
		, const UINT* _pStrides
		, const UINT* _pOffsets) const
	{
		mContext->IASetVertexBuffers(_startSlot, _NumBuffers, _ppVertexBuffers, _pStrides, _pOffsets);
	}

	inline void GPUMgr::BindIndexBuffer(ID3D11Buffer* _pIndexBuffer, DXGI_FORMAT _format, UINT _offset) const
	{
		mContext->IASetIndexBuffer(_pIndexBuffer, _format, _offset);
	}

	inline void GPUMgr::BindVertexShader(ID3D11VertexShader* _pVertexShader, ID3D11ClassInstance* const* _ppClassInstances, UINT _NumClassInstances) const
	{
		mContext->VSSetShader(_pVertexShader, _ppClassInstances, _NumClassInstances);
	}

	inline void GPUMgr::BindHullShader(ID3D11HullShader* _pVertexShader, ID3D11ClassInstance* const* _ppClassInstances, UINT _NumClassInstances) const
	{
		mContext->HSSetShader(_pVertexShader, _ppClassInstances, _NumClassInstances);
	}

	inline void GPUMgr::BindDomainShader(ID3D11DomainShader* _pVertexShader, ID3D11ClassInstance* const* _ppClassInstances, UINT _NumClassInstances) const
	{
		mContext->DSSetShader(_pVertexShader, _ppClassInstances, _NumClassInstances);
	}

	inline void GPUMgr::BindGeometryShader(ID3D11GeometryShader* _pVertexShader, ID3D11ClassInstance* const* _ppClassInstances, UINT _NumClassInstances) const
	{
		mContext->GSSetShader(_pVertexShader, _ppClassInstances, _NumClassInstances);
	}

	inline void GPUMgr::BindPixelShader(ID3D11PixelShader* _pPixelShader, ID3D11ClassInstance* const* _ppClassInstances, UINT _NumClassInstances) const
	{
		mContext->PSSetShader(_pPixelShader, _ppClassInstances, _NumClassInstances);
	}

	inline void GPUMgr::BindComputeShader(ID3D11ComputeShader* _pComputeShader, ID3D11ClassInstance* const* _ppClassInstances, UINT _NumClassInstances) const
	{
		mContext->CSSetShader(_pComputeShader, _ppClassInstances, _NumClassInstances);
	}

	inline void GPUMgr::Dispatch(UINT _threadGroupCountX, UINT _threadGroupCountY, UINT _threadGroupCountZ) const
	{
		mContext->Dispatch(_threadGroupCountX, _threadGroupCountY, _threadGroupCountZ);
	}

	inline void GPUMgr::BindViewports(D3D11_VIEWPORT* _viewPort) const
	{
		mContext->RSSetViewports(1, _viewPort);
	}

	inline void GPUMgr::BindUnorderdAccessView(UINT _startSlot, UINT _NumUAVs
		, ID3D11UnorderedAccessView* const* _ppUnorderedAccessViews, const UINT* _pUAVInitialCounts) const
	{
		mContext->CSSetUnorderedAccessViews(_startSlot, _NumUAVs, _ppUnorderedAccessViews, _pUAVInitialCounts);
	}

	inline void GPUMgr::BindRasterizerState(ID3D11RasterizerState* _pRasterizerState) const
	{
		mContext->RSSetState(_pRasterizerState);
	}

	inline void GPUMgr::BindDepthStencilState(ID3D11DepthStencilState* _pDepthStencilState) const
	{
		mContext->OMSetDepthStencilState(_pDepthStencilState, 0);
	}

	inline void GPUMgr::BindBlendState(ID3D11BlendState* _pBlendState) const
	{
		mContext->OMSetBlendState(_pBlendState, nullptr, 0xffffffff);
	}

	inline void GPUMgr::CopyResource(ID3D11Resource* _pDstResource, ID3D11Resource* _pSrcResource)
	{
		mContext->CopyResource(_pDstResource, _pSrcResource);
	}

	inline void GPUMgr::Draw()
	{
		mContext->Draw(0, 0);
	}

	inline void GPUMgr::DrawIndexed(UINT _indexCount, UINT _startIndexLocation, UINT _baseVertexLocation)
	{
		mContext->DrawIndexed(_indexCount, _startIndexLocation, _baseVertexLocation);

	}

	inline void GPUMgr::DrawIndexedInstanced(UINT _indexCountPerInstance, UINT _instanceCount, UINT _startIndexLocation, INT _baseVertexLocation, UINT _startInstanceLocation)
	{
		mContext->DrawIndexedInstanced(_indexCountPerInstance, _instanceCount, _startIndexLocation, _baseVertexLocation, _startInstanceLocation);
	}

	inline void GPUMgr::Present(bool _bVSync)
	{
		int VSync = 0;
		if (_bVSync)
		{
			VSync = 1;
		}
		mSwapChain->Present(VSync, 0);
	}



}

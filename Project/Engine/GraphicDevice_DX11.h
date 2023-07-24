#pragma once
#include "define_GPU.h"

#include "Enums.h"

namespace mh::GPU
{
	using namespace mh::enums;
	class Texture;

	class GraphicDevice_DX11
	{
	public:
		GraphicDevice_DX11(eValidationMode _validationMode = eValidationMode::Disabled);
		~GraphicDevice_DX11();

		bool CreateSwapChain(DXGI_SWAP_CHAIN_DESC* _desc);
		bool CreateTexture(D3D11_TEXTURE2D_DESC* _desc, ID3D11Texture2D** _ppTexture2D);

		bool CreateInputLayout(D3D11_INPUT_ELEMENT_DESC* _desc, UINT _NumElements, const void* _byteCode, SIZE_T _byteCodeLength, ID3D11InputLayout** _ppInputLayout);
		bool CreateBuffer(D3D11_BUFFER_DESC* _desc, D3D11_SUBRESOURCE_DATA* _data, ID3D11Buffer** _buffer);
		bool CreateRenderTargetView(ID3D11Resource* _pResource, const D3D11_RENDER_TARGET_VIEW_DESC* _pDesc, ID3D11RenderTargetView** _ppRTView);
		bool CreateUnorderedAccessView(ID3D11Resource* _pResource, const D3D11_UNORDERED_ACCESS_VIEW_DESC* _pDesc, ID3D11UnorderedAccessView** _ppUAView);
		bool CreateDepthStencilView(ID3D11Resource* _pResource, const D3D11_DEPTH_STENCIL_VIEW_DESC* _pDesc, ID3D11DepthStencilView** _ppDepthStencilView);
		bool CreateShaderResourceView(ID3D11Resource* _pResource, const D3D11_SHADER_RESOURCE_VIEW_DESC* _pDesc, ID3D11ShaderResourceView** _ppSRView);
		bool CreateVertexShader(const void* _pShaderByteCode, SIZE_T _byteCodeLength, ID3D11ClassLinkage* _pClassLinkage, ID3D11VertexShader** _ppVertexShader);

		bool CreateHullShader(const void* _pShaderByteCode, SIZE_T _byteCodeLength, ID3D11ClassLinkage* _pClassLinkage, ID3D11HullShader** _ppHullShader);
		bool CreateDomainShader(const void* _pShaderByteCode, SIZE_T _byteCodeLength, ID3D11ClassLinkage* _pClassLinkage, ID3D11DomainShader** _ppDomainShader);

		bool CreateGeometryShader(const void* pShaderByteCode, SIZE_T _byteCodeLength, ID3D11ClassLinkage* _pClassLinkage, ID3D11GeometryShader** _ppVertexShader);
		bool CreatePixelShader(const void* _pShaderByteCode, SIZE_T _byteCodeLength, ID3D11ClassLinkage* _pClassLinkage, ID3D11PixelShader** _ppPixelShader);
		bool CreateComputeShader(const void* _pShaderByteCode, SIZE_T _byteCodeLength, ID3D11ClassLinkage* _pClassLinkage, ID3D11ComputeShader** _ppComputeShader);
		bool CreateSamplerState(const D3D11_SAMPLER_DESC* _pSamplerDesc, ID3D11SamplerState** _ppSamplerState);
		bool CreateRasterizerState(const D3D11_RASTERIZER_DESC* _pRasterizerDesc, ID3D11RasterizerState** _ppRasterizerState);
		bool CreateDepthStencilState(const D3D11_DEPTH_STENCIL_DESC* _pDepthStencilDesc, ID3D11DepthStencilState** _ppDepthStencilState);
		bool CreateBlendState(const D3D11_BLEND_DESC* _pBlendStateDesc, ID3D11BlendState** _ppBlendState);

		void BindPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY _topology) const;
		void BindInputLayout(ID3D11InputLayout* _pInputLayout) const;
		void BindVertexBuffer(UINT _startSlot, UINT _NumBuffers, ID3D11Buffer* const* _ppVertexBuffers, const UINT* _pStrides, const UINT* _pOffsets)const;
		void BindIndexBuffer(ID3D11Buffer* _pIndexBuffer, DXGI_FORMAT _Format, UINT _Offset)const;
		void BindVertexShader(ID3D11VertexShader* _pVertexShader, ID3D11ClassInstance* const* _ppClassInstances, UINT _NumClassInstances)const;
		void BindHullShader(ID3D11HullShader* _pHullShader, ID3D11ClassInstance* const* _ppClassInstances, UINT _NumClassInstances)const;
		void BindDomainShader(ID3D11DomainShader* _pDomainShader, ID3D11ClassInstance* const* _ppClassInstances, UINT _NumClassInstances)const;
		void BindGeometryShader(ID3D11GeometryShader* _pGeometryShader, ID3D11ClassInstance* const* _ppClassInstances, UINT _NumClassInstances)const;
		void BindPixelShader(ID3D11PixelShader* _pPixelShader, ID3D11ClassInstance* const* _ppClassInstances, UINT _NumClassInstances)const;
		void BindComputeShader(ID3D11ComputeShader* _pComputeShader, ID3D11ClassInstance* const* _ppClassInstances, UINT _NumClassInstances)const;
		void Dispatch(UINT _threadGroupCountX, UINT _threadGroupCountY, UINT _threadGroupCountZ)const;
		void BindViewports(D3D11_VIEWPORT* _viewPort)const;
		void BindConstantBuffer(eShaderStage _stage, eCBType _type, ID3D11Buffer* _buffer)const;
		void BindShaderResource(eShaderStage _stage, UINT _slot, ID3D11ShaderResourceView* const* _ppShaderResourceViews)const;
		void BindUnorderdAccessView(UINT _startSlot, UINT _NumUAVs
			, ID3D11UnorderedAccessView* const* _ppUnorderedAccessViews, const UINT* _pUAVInitialCounts)const;
		void BindSamplers(eShaderStage _stage, UINT _slot, UINT _NumSamplers, ID3D11SamplerState* const* _ppSamplers)const;
		void BindsSamplers(UINT _slot, UINT _NumSamplers, ID3D11SamplerState* const* _ppSamplers)const;
		void BindRasterizerState(ID3D11RasterizerState* _pRasterizerState)const;
		void BindDepthStencilState(ID3D11DepthStencilState* _pDepthStencilState)const;
		void BindBlendState(ID3D11BlendState* _pBlendState)const;

		void CopyResource(ID3D11Resource* _pDstResource, ID3D11Resource* _pSrcResource);
		void SetData(ID3D11Buffer* _buffer, void* _data, UINT _size);

		void Clear() const;// 화면 지워주기
		void AdjustViewPorts();
		void OMSetRenderTarget();

		void Draw();
		void DrawIndexed(UINT _indexCount, UINT _startIndexLocation, UINT _baseVertexLocation);
		void DrawIndexedInstanced(UINT _indexCountPerInstance, UINT _instanceCount, UINT _startIndexLocation, INT _baseVertexLocation, UINT _startInstanceLocation);

		void Present();

		ID3D11Device* GetID3D11Device() { return mDevice.Get(); }
		ID3D11DeviceContext* GetID3D11DeviceContext() { return mContext.Get(); }

	private:
		Microsoft::WRL::ComPtr <ID3D11Device> mDevice;
		Microsoft::WRL::ComPtr <ID3D11DeviceContext> mContext;

		std::shared_ptr<Texture> mRenderTargetTexture;
		std::shared_ptr<Texture> mDepthStencilBufferTexture;

		Microsoft::WRL::ComPtr <IDXGISwapChain> mSwapChain;

		D3D11_VIEWPORT mViewPort;
	};

	inline GraphicDevice_DX11*& GetDevice()
	{
		static GraphicDevice_DX11* device = nullptr;
		return device;
	}
}

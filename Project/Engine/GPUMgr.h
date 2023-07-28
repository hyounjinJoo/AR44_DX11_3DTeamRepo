#pragma once
#include "define_GPU.h"


#include "Singleton.h"
#include "define_Enum.h"


namespace mh
{
	using namespace mh::define;
	class Texture;

	class GPUMgr 
		: public Singleton<GPUMgr>
	{
		SINGLETON(GPUMgr);

	public:
		bool Initialize(HWND _hwnd, UINT _Width, UINT _Height);
		ID3D11Device* GetDevice() { return mDevice.Get(); }
		ID3D11DeviceContext* GetContext() { return mContext.Get(); }

		//================================ INLINE WRAPPER ====================================
	public:
		inline bool CreateTexture(D3D11_TEXTURE2D_DESC* _desc, ID3D11Texture2D** _ppTexture2D);
		inline bool CreateInputLayout(D3D11_INPUT_ELEMENT_DESC* _desc, UINT _NumElements, const void* _byteCode, SIZE_T _byteCodeLength, ID3D11InputLayout** _ppInputLayout);
		inline bool CreateBuffer(D3D11_BUFFER_DESC* _desc, D3D11_SUBRESOURCE_DATA* _data, ID3D11Buffer** _buffer);
		inline bool CreateRenderTargetView(ID3D11Resource* _pResource, const D3D11_RENDER_TARGET_VIEW_DESC* _pDesc, ID3D11RenderTargetView** _ppRTView);
		inline bool CreateUnorderedAccessView(ID3D11Resource* _pResource, const D3D11_UNORDERED_ACCESS_VIEW_DESC* _pDesc, ID3D11UnorderedAccessView** _ppUAView);
		inline bool CreateDepthStencilView(ID3D11Resource* _pResource, const D3D11_DEPTH_STENCIL_VIEW_DESC* _pDesc, ID3D11DepthStencilView** _ppDepthStencilView);
		inline bool CreateShaderResourceView(ID3D11Resource* _pResource, const D3D11_SHADER_RESOURCE_VIEW_DESC* _pDesc, ID3D11ShaderResourceView** _ppSRView);
		inline bool CreateVertexShader(const void* _pShaderByteCode, SIZE_T _byteCodeLength, ID3D11ClassLinkage* _pClassLinkage, ID3D11VertexShader** _ppVertexShader);

		inline bool CreateHullShader(const void* _pShaderByteCode, SIZE_T _byteCodeLength, ID3D11ClassLinkage* _pClassLinkage, ID3D11HullShader** _ppHullShader);
		inline bool CreateDomainShader(const void* _pShaderByteCode, SIZE_T _byteCodeLength, ID3D11ClassLinkage* _pClassLinkage, ID3D11DomainShader** _ppDomainShader);

		inline bool CreateGeometryShader(const void* pShaderByteCode, SIZE_T _byteCodeLength, ID3D11ClassLinkage* _pClassLinkage, ID3D11GeometryShader** _ppVertexShader);
		inline bool CreatePixelShader(const void* _pShaderByteCode, SIZE_T _byteCodeLength, ID3D11ClassLinkage* _pClassLinkage, ID3D11PixelShader** _ppPixelShader);
		inline bool CreateComputeShader(const void* _pShaderByteCode, SIZE_T _byteCodeLength, ID3D11ClassLinkage* _pClassLinkage, ID3D11ComputeShader** _ppComputeShader);
		inline bool CreateSamplerState(const D3D11_SAMPLER_DESC* _pSamplerDesc, ID3D11SamplerState** _ppSamplerState);
		inline bool CreateRasterizerState(const D3D11_RASTERIZER_DESC* _pRasterizerDesc, ID3D11RasterizerState** _ppRasterizerState);
		inline bool CreateDepthStencilState(const D3D11_DEPTH_STENCIL_DESC* _pDepthStencilDesc, ID3D11DepthStencilState** _ppDepthStencilState);
		inline bool CreateBlendState(const D3D11_BLEND_DESC* _pBlendStateDesc, ID3D11BlendState** _ppBlendState);

		inline void BindPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY _topology) const;
		inline void BindInputLayout(ID3D11InputLayout* _pInputLayout) const;
		inline void BindVertexBuffer(UINT _startSlot, UINT _NumBuffers, ID3D11Buffer* const* _ppVertexBuffers, const UINT* _pStrides, const UINT* _pOffsets)const;
		inline void BindIndexBuffer(ID3D11Buffer* _pIndexBuffer, DXGI_FORMAT _Format, UINT _Offset)const;
		inline void BindVertexShader(ID3D11VertexShader* _pVertexShader, ID3D11ClassInstance* const* _ppClassInstances, UINT _NumClassInstances)const;
		inline void BindHullShader(ID3D11HullShader* _pHullShader, ID3D11ClassInstance* const* _ppClassInstances, UINT _NumClassInstances)const;
		inline void BindDomainShader(ID3D11DomainShader* _pDomainShader, ID3D11ClassInstance* const* _ppClassInstances, UINT _NumClassInstances)const;
		inline void BindGeometryShader(ID3D11GeometryShader* _pGeometryShader, ID3D11ClassInstance* const* _ppClassInstances, UINT _NumClassInstances)const;
		inline void BindPixelShader(ID3D11PixelShader* _pPixelShader, ID3D11ClassInstance* const* _ppClassInstances, UINT _NumClassInstances)const;
		inline void BindComputeShader(ID3D11ComputeShader* _pComputeShader, ID3D11ClassInstance* const* _ppClassInstances, UINT _NumClassInstances)const;
		inline void Dispatch(UINT _threadGroupCountX, UINT _threadGroupCountY, UINT _threadGroupCountZ)const;
		inline void BindViewports(D3D11_VIEWPORT* _viewPort)const;
		
		
		inline void BindUnorderdAccessView(UINT _startSlot, UINT _NumUAVs
			, ID3D11UnorderedAccessView* const* _ppUnorderedAccessViews, const UINT* _pUAVInitialCounts)const;

		
		inline void BindRasterizerState(ID3D11RasterizerState* _pRasterizerState)const;
		inline void BindDepthStencilState(ID3D11DepthStencilState* _pDepthStencilState)const;
		inline void BindBlendState(ID3D11BlendState* _pBlendState)const;

		inline void CopyResource(ID3D11Resource* _pDstResource, ID3D11Resource* _pSrcResource);
		

		inline void Draw();
		inline void DrawIndexed(UINT _indexCount, UINT _startIndexLocation, UINT _baseVertexLocation);
		inline void DrawIndexedInstanced(UINT _indexCountPerInstance, UINT _instanceCount, UINT _startIndexLocation, INT _baseVertexLocation, UINT _startInstanceLocation);

		inline void Present(bool _bVSync = false);


		void BindsSamplers(UINT _slot, UINT _NumSamplers, ID3D11SamplerState* const* _ppSamplers)const;
		void BindSamplers(eShaderStage _stage, UINT _slot, UINT _NumSamplers, ID3D11SamplerState* const* _ppSamplers)const;
		void BindShaderResource(eShaderStage _stage, UINT _slot, ID3D11ShaderResourceView* const* _ppShaderResourceViews)const;
		void BindConstBuffer(eShaderStage _stage, eCBType _type, ID3D11Buffer* _buffer)const;
		void SetData(ID3D11Buffer* _buffer, void* _data, UINT _size);
		void Clear() const;// 화면 지워주기
		void AdjustViewPorts(HWND _hWnd);
		void OMSetRenderTarget();

	private:
		void Reset();
		bool CreateDevice(ID3D11Device** _ppDevice, ID3D11DeviceContext** _ppContext);
		bool CreateSwapChain(DXGI_SWAP_CHAIN_DESC* _desc);
		std::shared_ptr<Texture> CreateRenderTarget();
		std::shared_ptr<Texture> CreateDepthStencil(UINT _Width, UINT _Height);
		void CreateViewPort(HWND _hWnd);

	private:
		Microsoft::WRL::ComPtr <ID3D11Device> mDevice;
		Microsoft::WRL::ComPtr <ID3D11DeviceContext> mContext;
		Microsoft::WRL::ComPtr <IDXGISwapChain> mSwapChain;
		
		std::shared_ptr<mh::Texture> mRenderTargetTexture;
		std::shared_ptr<mh::Texture> mDepthStencilBufferTexture;

		D3D11_VIEWPORT mViewPort;
	};
}

#include "GPUMgr.inl"

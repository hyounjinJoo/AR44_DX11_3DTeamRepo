#include "EnginePCH.h"
#include "Texture.h"

#ifdef _DEBUG
#pragma comment(lib, "DirectXTex/Debug/DirectXTex.lib") 
#else 
#pragma comment(lib, "DirectXTex/Release/DirectXTex.lib") 
#endif


#include "Func.h"
#include "PathMgr.h"




namespace mh::GPU
{
	namespace stdfs = std::filesystem;

	Texture::Texture()
		: GameResource(eResourceType::Texture)
		, mDesc{}
		, mTexture(nullptr)
	{

	}

	Texture::~Texture()
	{

	}

	void Texture::Clear(UINT _startSlot)
	{
		ID3D11ShaderResourceView* srv = nullptr;

		GetDevice()->BindShaderResource(eShaderStage::VS, _startSlot, &srv);
		GetDevice()->BindShaderResource(eShaderStage::DS, _startSlot, &srv);
		GetDevice()->BindShaderResource(eShaderStage::GS, _startSlot, &srv);
		GetDevice()->BindShaderResource(eShaderStage::HS, _startSlot, &srv);
		GetDevice()->BindShaderResource(eShaderStage::CS, _startSlot, &srv);
		GetDevice()->BindShaderResource(eShaderStage::PS, _startSlot, &srv);
	}

	bool Texture::Create(UINT _width, UINT _height, DXGI_FORMAT _format, UINT _bindFlag)
	{
		//Depth stencil _texture
		mDesc.BindFlags = _bindFlag;
		mDesc.Usage = D3D11_USAGE_DEFAULT;
		mDesc.CPUAccessFlags = 0;
		mDesc.Format = _format;
		mDesc.Width = _width;
		mDesc.Height = _height;
		mDesc.ArraySize = 1;

		mDesc.SampleDesc.Count = 1;
		mDesc.SampleDesc.Quality = 0;

		mDesc.MipLevels = 1;
		mDesc.MiscFlags = 0;

		if (!GetDevice()->CreateTexture(&mDesc, mTexture.GetAddressOf()))
			return false;

		if (_bindFlag & D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL)
		{
			if (!GetDevice()->CreateDepthStencilView(mTexture.Get(), nullptr, mDSV.GetAddressOf()))
				return false;
		}

		if (_bindFlag & D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE)
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC tSRVDesc = {};
			tSRVDesc.Format = _format;
			tSRVDesc.Texture2D.MipLevels = 1;
			tSRVDesc.Texture2D.MostDetailedMip = 0;
			tSRVDesc.ViewDimension = D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2D;
			
			if (!GetDevice()->CreateShaderResourceView(mTexture.Get(), nullptr, mSRV.GetAddressOf()))
				return false;
		}
		
		if (_bindFlag & D3D11_BIND_FLAG::D3D11_BIND_UNORDERED_ACCESS)
		{
			D3D11_UNORDERED_ACCESS_VIEW_DESC tUAVDesc = {};
			tUAVDesc.Format = _format;
			tUAVDesc.Texture2D.MipSlice = 0;
			tUAVDesc.ViewDimension = D3D11_UAV_DIMENSION::D3D11_UAV_DIMENSION_TEXTURE2D;

			if (!GetDevice()->CreateUnorderedAccessView(mTexture.Get(), nullptr, mUAV.GetAddressOf()))
				return false;
		}

		return true;
	}

	bool Texture::Create(Microsoft::WRL::ComPtr<ID3D11Texture2D> _texture)
	{
		mTexture = _texture;
		mTexture->GetDesc(&mDesc);

		if (mDesc.BindFlags & D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL)
		{
			if (!GetDevice()->CreateDepthStencilView(mTexture.Get(), nullptr, mDSV.GetAddressOf()))
				return false;
		}

		if (mDesc.BindFlags & D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET)
		{
			if (!GetDevice()->CreateRenderTargetView(mTexture.Get(), nullptr, mRTV.GetAddressOf()))
				return false;
		}

		if (mDesc.BindFlags & D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE)
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC tSRVDesc = {};
			tSRVDesc.Format = mDesc.Format;
			tSRVDesc.Texture2D.MipLevels = 1;
			tSRVDesc.Texture2D.MostDetailedMip = 0;
			tSRVDesc.ViewDimension = D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2D;

			if (!GetDevice()->CreateShaderResourceView(mTexture.Get(), nullptr, mSRV.GetAddressOf()))
				return false;
		}

		if (mDesc.BindFlags & D3D11_BIND_FLAG::D3D11_BIND_UNORDERED_ACCESS)
		{
			D3D11_UNORDERED_ACCESS_VIEW_DESC tUAVDesc = {};
			tUAVDesc.Format = mDesc.Format;
			tUAVDesc.Texture2D.MipSlice = 0;
			tUAVDesc.ViewDimension = D3D11_UAV_DIMENSION::D3D11_UAV_DIMENSION_TEXTURE2D;

			if (!GetDevice()->CreateUnorderedAccessView(mTexture.Get(), nullptr, mUAV.GetAddressOf()))
				return false;
		}

		return true;
	}

	
	HRESULT Texture::Load(const std::filesystem::path& _FileName)
	{
		stdfs::path FullPath = PathMgr::GetInst()->GetRelativeResourcePath(GetResType());

		HRESULT hr = LoadFile(FullPath / _FileName);

		if (FAILED(hr))
			return E_FAIL;

		InitializeResource();
		return S_OK;
	}

	HRESULT Texture::LoadFile(const std::filesystem::path& _fullPath)
	{
		std::wstring Extension = _fullPath.extension().wstring();
		StringConv::UpperCase(Extension);

		if (Extension == L".DDS")
		{
			if (FAILED(LoadFromDDSFile(_fullPath.c_str(), DDS_FLAGS::DDS_FLAGS_NONE, nullptr, mImage)))
				return E_FAIL;
		}
		else if (Extension == L".TGA")
		{
			if (FAILED(LoadFromTGAFile(_fullPath.c_str(), nullptr, mImage)))
				return E_FAIL;
		}
		else // WIC (png, jpg, jpeg, bmp )
		{
			if (FAILED(LoadFromWICFile(_fullPath.c_str(), WIC_FLAGS::WIC_FLAGS_NONE, nullptr, mImage)))
				return E_FAIL;
		}

		return S_OK;
	}

	void Texture::InitializeResource()
	{
		CreateShaderResourceView
		(
			GetDevice()->GetID3D11Device(),
			mImage.GetImages(),
			mImage.GetImageCount(),
			mImage.GetMetadata(),
			mSRV.GetAddressOf()
		);

		mSRV->GetResource((ID3D11Resource**)mTexture.GetAddressOf());
		mTexture->GetDesc(&mDesc);
	}

	void Texture::BindShaderResource(eShaderStage _stage, UINT _slot)
	{
		GetDevice()->BindShaderResource(_stage, _slot, mSRV.GetAddressOf());
	}

	void Texture::BindUnorderedAccessView(UINT _startSlot)
	{
		UINT i = -1;
		GetDevice()->BindUnorderdAccessView(_startSlot, 1, mUAV.GetAddressOf(), &i);
	}

	void Texture::ClearUnorderedAccessView(UINT _startSlot)
	{
		ID3D11UnorderedAccessView* p = nullptr;
		UINT i = -1;
		GetDevice()->BindUnorderdAccessView(_startSlot, 1, &p, &i);
	}

	void Texture::Clear()
	{
		ID3D11ShaderResourceView* srv = nullptr;

		GetDevice()->BindShaderResource(eShaderStage::VS, 0, &srv);
		GetDevice()->BindShaderResource(eShaderStage::DS, 0, &srv);
		GetDevice()->BindShaderResource(eShaderStage::GS, 0, &srv);
		GetDevice()->BindShaderResource(eShaderStage::HS, 0, &srv);
		GetDevice()->BindShaderResource(eShaderStage::CS, 0, &srv);
		GetDevice()->BindShaderResource(eShaderStage::PS, 0, &srv);
	}


}

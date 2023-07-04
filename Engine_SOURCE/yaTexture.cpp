#include "yaTexture.h"



namespace ya::graphics
{
	

	Texture::Texture()
		: Resource(eResourceType::Texture)
		, mDesc{}
		, mTexture(nullptr)
	{

	}

	Texture::~Texture()
	{

	}

	void Texture::Clear(UINT startSlot)
	{
		ID3D11ShaderResourceView* srv = nullptr;

		GetDevice()->BindShaderResource(eShaderStage::VS, startSlot, &srv);
		GetDevice()->BindShaderResource(eShaderStage::DS, startSlot, &srv);
		GetDevice()->BindShaderResource(eShaderStage::GS, startSlot, &srv);
		GetDevice()->BindShaderResource(eShaderStage::HS, startSlot, &srv);
		GetDevice()->BindShaderResource(eShaderStage::CS, startSlot, &srv);
		GetDevice()->BindShaderResource(eShaderStage::PS, startSlot, &srv);
	}

	bool Texture::Create(UINT width, UINT height, DXGI_FORMAT format, UINT bindFlag)
	{
		//Depth stencil texture
		mDesc.BindFlags = bindFlag;
		mDesc.Usage = D3D11_USAGE_DEFAULT;
		mDesc.CPUAccessFlags = 0;
		mDesc.Format = format;
		mDesc.Width = width;
		mDesc.Height = height;
		mDesc.ArraySize = 1;

		mDesc.SampleDesc.Count = 1;
		mDesc.SampleDesc.Quality = 0;

		mDesc.MipLevels = 1;
		mDesc.MiscFlags = 0;

		if (!GetDevice()->CreateTexture(&mDesc, mTexture.GetAddressOf()))
			return false;

		if (bindFlag & D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL)
		{
			if (!GetDevice()->CreateDepthStencilView(mTexture.Get(), nullptr, mDSV.GetAddressOf()))
				return false;
		}

		if (bindFlag & D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE)
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC tSRVDesc = {};
			tSRVDesc.Format = format;
			tSRVDesc.Texture2D.MipLevels = 1;
			tSRVDesc.Texture2D.MostDetailedMip = 0;
			tSRVDesc.ViewDimension = D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2D;
			
			if (!GetDevice()->CreateShaderResourceView(mTexture.Get(), nullptr, mSRV.GetAddressOf()))
				return false;
		}
		
		if (bindFlag & D3D11_BIND_FLAG::D3D11_BIND_UNORDERED_ACCESS)
		{
			D3D11_UNORDERED_ACCESS_VIEW_DESC tUAVDesc = {};
			tUAVDesc.Format = format;
			tUAVDesc.Texture2D.MipSlice = 0;
			tUAVDesc.ViewDimension = D3D11_UAV_DIMENSION::D3D11_UAV_DIMENSION_TEXTURE2D;

			if (!GetDevice()->CreateUnorderedAccessView(mTexture.Get(), nullptr, mUAV.GetAddressOf()))
				return false;
		}

		return true;
	}

	bool Texture::Create(Microsoft::WRL::ComPtr<ID3D11Texture2D> texture)
	{
		mTexture = texture;
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

	//test.cpp
	HRESULT Texture::Load(const std::wstring& name)
	{
		std::filesystem::path parentPath = std::filesystem::current_path();
		std::wstring fullPath = parentPath.wstring() + L"\\Resources\\" + name;

		LoadFile(fullPath);
		InitializeResource();

		return S_OK;
	}

	void Texture::LoadFile(const std::wstring& fullPath)
	{
		wchar_t szExtension[256] = {};
		_wsplitpath_s(fullPath.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szExtension, 256);

		std::wstring extension(szExtension);

		if (extension == L".dds" || extension == L".DDS")
		{
			if (FAILED(LoadFromDDSFile(fullPath.c_str(), DDS_FLAGS::DDS_FLAGS_NONE, nullptr, mImage)))
				return;
		}
		else if (extension == L".tga" || extension == L".TGA")
		{
			if (FAILED(LoadFromTGAFile(fullPath.c_str(), nullptr, mImage)))
				return;
		}
		else // WIC (png, jpg, jpeg, bmp )
		{
			if (FAILED(LoadFromWICFile(fullPath.c_str(), WIC_FLAGS::WIC_FLAGS_NONE, nullptr, mImage)))
				return;
		}
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

	void Texture::BindShaderResource(eShaderStage stage, UINT slot)
	{
		GetDevice()->BindShaderResource(stage, slot, mSRV.GetAddressOf());
	}

	void Texture::BindUnorderedAccessView(UINT startSlot)
	{
		UINT i = -1;
		GetDevice()->BindUnorderdAccessView(startSlot, 1, mUAV.GetAddressOf(), &i);
	}

	void Texture::ClearUnorderedAccessView(UINT startSlot)
	{
		ID3D11UnorderedAccessView* p = nullptr;
		UINT i = -1;
		GetDevice()->BindUnorderdAccessView(startSlot, 1, &p, &i);
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

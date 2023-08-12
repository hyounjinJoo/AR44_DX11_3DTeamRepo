#pragma once
#include "IShader.h"
#include "define_GPU.h"
#include <unordered_set>

namespace gui
{
	class guiGraphicsShaderEditor;
}

namespace mh
{
	using Microsoft::WRL::ComPtr;
	namespace stdfs = std::filesystem;

	struct tShaderCode
	{
		ComPtr<ID3DBlob> blob;
		std::string strKey;
	};

	class GraphicsShader : public IShader
	{
		friend class gui::guiGraphicsShaderEditor;
	public:
		GraphicsShader();
		virtual ~GraphicsShader();

		//여기 수정할 경우 CodeGenerator의 함수도 수정해 줄것
		virtual eResult SaveJson(Json::Value* _pJVal) override;
		virtual eResult LoadJson(const Json::Value* _pJVal) override;

		virtual eResult Save(const std::filesystem::path& _path) override;
		virtual eResult Load(const std::filesystem::path& _path) override;

		eResult CreateByCompile(eGSStage _stage, const stdfs::path& _FullPath, const std::string_view _funcName);
		eResult CreateByHeader(eGSStage _stage, const unsigned char* _pByteCode, size_t _ByteCodeSize);
		eResult CreateByCSO(eGSStage _stage, const stdfs::path& _FileName);

		inline void AddInputLayoutDesc(const D3D11_INPUT_ELEMENT_DESC& _desc);
		inline void SetInputLayoutDesc(const std::vector<D3D11_INPUT_ELEMENT_DESC>& _descs);
		eResult CreateInputLayout();
		const std::vector<D3D11_INPUT_ELEMENT_DESC>& GetInputLayoutDescs() { return mInputLayoutDescs; }

		ID3D11InputLayout* GetInputLayout() { return mInputLayout.Get(); }
		ID3D11InputLayout** GetInputLayoutAddressOf() { return mInputLayout.GetAddressOf(); }

		void SetTopology(D3D11_PRIMITIVE_TOPOLOGY _topology) { mTopology = _topology; }
		D3D11_PRIMITIVE_TOPOLOGY GetTopology() { return mTopology; }

		void SetRSState(eRSType _state) { mRSType = _state; }
		eRSType GetRSState() const { return mRSType; }

		void SetDSState(eDSType _state) { mDSType = _state; }
		eDSType GetDSState() const { return mDSType; }

		void SetBSState(eBSType _state) { mBSType = _state; }
		eBSType GetBSState() const { return mBSType; }



		void Binds();

		//에디터용
		inline void SetEditMode(bool _bEditMode) { mbEditMode = _bEditMode; }
		inline void SetShaderKey(eGSStage _stage, const std::string_view _strKey);
		inline const std::string& GetShaderKey(eGSStage _stage) { return mArrShaderCode[(int)_stage].strKey; }
		

	private:
		eResult CreateShader(eGSStage _stage, const void* _pByteCode, size_t _ByteCodeSize);

	private:
		static std::unordered_set<std::string> mSemanticNames;
		std::vector<D3D11_INPUT_ELEMENT_DESC> mInputLayoutDescs;
		ComPtr<ID3D11InputLayout> mInputLayout;
		D3D11_PRIMITIVE_TOPOLOGY mTopology;

		tShaderCode mArrShaderCode[(int)eGSStage::END];

		ComPtr<ID3D11VertexShader>		mVS;
		ComPtr<ID3D11HullShader>		mHS;
		ComPtr<ID3D11DomainShader>		mDS;
		ComPtr<ID3D11GeometryShader>	mGS;
		ComPtr<ID3D11PixelShader>		mPS;

		eRSType mRSType;
		eDSType mDSType;
		eBSType mBSType;

		ComPtr<ID3DBlob> mErrorBlob;

		bool mbEditMode;
	};

	inline void GraphicsShader::AddInputLayoutDesc(const D3D11_INPUT_ELEMENT_DESC& _desc)
	{
		mInputLayoutDescs.push_back(_desc);

		//이름을 별도 저장된 공간에 저장된 뒤 해당 주소로 교체
		const auto& pair = mSemanticNames.insert(mInputLayoutDescs.back().SemanticName);
		mInputLayoutDescs.back().SemanticName = pair.first->c_str();
	}

	inline void GraphicsShader::SetInputLayoutDesc(const std::vector<D3D11_INPUT_ELEMENT_DESC>& _descs)
	{
		mInputLayoutDescs = _descs;

		for (size_t i = 0; i < mInputLayoutDescs.size(); ++i)
		{
			if (mInputLayoutDescs[i].SemanticName)
			{
				const auto& pair = mSemanticNames.insert(mInputLayoutDescs[i].SemanticName);
				mInputLayoutDescs[i].SemanticName = pair.first->c_str();
			}
		}
	}


	inline void GraphicsShader::SetShaderKey(eGSStage _stage, const std::string_view _strKey)
	{
		mArrShaderCode[(int)_stage].strKey = _strKey;
	}
}

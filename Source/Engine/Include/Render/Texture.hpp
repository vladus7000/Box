#pragma once
#include "System/ResourceSystem/ResourceExtraData.hpp"
#include <memory>

#include <d3d11.h>
#include <wrl.h>

namespace box
{
	class Texture : public ResourceExtraData
	{
	public:
		using TextureStrongPtr = std::shared_ptr<Texture>;
		using TextureWeakPtr = std::weak_ptr<Texture>;

	public:
		Texture(
			U32 width = 0,
			U32 height = 0,
			ID3D11ShaderResourceView* srv = nullptr,
			ID3D11RenderTargetView* rtv = nullptr,
			bool rtvAttach = false,
			ID3D11DepthStencilView* dsv = nullptr,
			bool dsvAttach = false
		);
		virtual ~Texture();

		void initDefaultSampler(ID3D11Device* pDevice);
		void setSampler();
		void setName(const std::string& name) { m_name = name; }
		virtual const std::string& getName() const { return m_name; };

		bool isCube() const { return m_cube; }
		U32 getWidth() const { return m_width; }
		U32 getHeight() const { return m_height; }
		U32 getArraySize() const { return m_array; }
		U32 getMipsCount() const { return m_mips; }
		U32 getSampleCount() const { return m_sample; }

		ID3D11ShaderResourceView* getSRV_Raw() const { return m_shaderResourceView.Get(); }
		ID3D11ShaderResourceView* getSRV_cube_Raw() const { return m_shaderResourceView_cube.Get(); }
		ID3D11SamplerState* getSamplerState_Raw() const { return m_samplerState.Get(); }
		ID3D11RenderTargetView* getRTV_Raw() const { return m_renderTargetView.Get(); }
		ID3D11RenderTargetView* getRTV_cube_Raw(U32 i) const { return m_renderTargetView_cube[i].Get(); }
		ID3D11DepthStencilView* getDSV_Raw() const { return m_depthStencilView.Get(); }
		ID3D11DepthStencilView* getDSV_cube_Raw(U32 i) const { return m_depthStencilView_cube[i].Get(); }
		ID3D11DepthStencilView* getDSV_ro_Raw() const { return m_depthStencilView_ro.Get(); }
		ID3D11UnorderedAccessView* getUAV_Raw() const { return m_unorderedAccessView.Get(); }
		ID3D11Texture2D* getTexture2D_Raw() const { return m_texture2D.Get(); }

		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> getSRV() const { return m_shaderResourceView; }
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> getSRV_cube() const { return m_shaderResourceView_cube; }
		Microsoft::WRL::ComPtr<ID3D11SamplerState> getSamplerState() const { return m_samplerState; }
		Microsoft::WRL::ComPtr<ID3D11Texture2D> getTexture2D() const { return m_texture2D; }
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> getRTV() const { return m_renderTargetView; }
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> getRTV_cube(U32 i) const { return m_renderTargetView_cube[i]; }
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> getDSV() const { return m_depthStencilView; }
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> getDSV_cube(U32 i) const { return m_depthStencilView_cube[i]; }
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> getDSV_ro() const { return m_depthStencilView_ro; }
		Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> getUAV() const { return m_unorderedAccessView; }

		bool createSurface(
			ID3D11Device* pDevice,
			U32 uWidth,
			U32 uHeight,
			U32 uSampleCount = 1,
			U32 uArraySize = 1,
			U32 uMipLevels = 1,
			DXGI_FORMAT T2D_Format = DXGI_FORMAT_UNKNOWN,
			DXGI_FORMAT SRV_Format = DXGI_FORMAT_UNKNOWN,
			DXGI_FORMAT RTV_Format = DXGI_FORMAT_UNKNOWN,
			DXGI_FORMAT DSV_Format = DXGI_FORMAT_UNKNOWN,
			DXGI_FORMAT UAV_Format = DXGI_FORMAT_UNKNOWN,
			DXGI_FORMAT DSV_RO_Format = DXGI_FORMAT_UNKNOWN,
			D3D11_USAGE usage = D3D11_USAGE_DEFAULT,
			bool bCube = false,
			U32 pitch = 0,
			void* data = nullptr);

	private:
		friend class DDSTextureResourceLoader;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_shaderResourceView;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_shaderResourceView_cube;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView_cube[6];
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStencilView;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStencilView_cube[6];
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStencilView_ro;
		Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> m_unorderedAccessView;
		Microsoft::WRL::ComPtr<ID3D11SamplerState> m_samplerState;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_texture2D;

		U32 m_width;
		U32 m_height;
		U32 m_array;
		U32 m_mips;
		U32 m_sample;
		bool m_cube;
		bool m_rtvAttached;
		bool m_dsvAttached;
		std::string m_name;
	};
}
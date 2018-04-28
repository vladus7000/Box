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
		virtual ~Texture() {}
		void setName(const std::string& name) { m_name = name; }
		virtual const std::string& getName() const { return m_name; };

		ID3D11ShaderResourceView* getSRV_Raw() const { return m_shaderResourceView.Get(); }
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> getSRV() const { return m_shaderResourceView; }

		ID3D11SamplerState* getSamplerState_Raw() const { return m_samplerState.Get(); }
		Microsoft::WRL::ComPtr<ID3D11SamplerState> getSamplerState() const { return m_samplerState; }

	private:
		friend class DDSTextureResourceLoader;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_shaderResourceView;
		Microsoft::WRL::ComPtr<ID3D11SamplerState> m_samplerState;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_texture2D;
		std::string m_name;
	};
}
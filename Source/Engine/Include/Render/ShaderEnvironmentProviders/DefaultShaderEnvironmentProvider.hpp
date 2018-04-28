#pragma once

#include "ShaderEnvironmentProvider.hpp"
#include "Math/Matrix4D.hpp"
#include <wrl.h>

struct ID3D11Buffer;

namespace box
{
	class DefaultShaderEnvironmentProvider : public ShaderEnvironmentProvider
	{
	public:
		DefaultShaderEnvironmentProvider();
		virtual ~DefaultShaderEnvironmentProvider();

		std::string getName() { return "DefaultShaderEnvironmentProvider"; }

		virtual void onDeviceLost() override;
		virtual bool restore(ID3D11Device* device) override;

		virtual void prepareShader(ID3D11DeviceContext* context, Shader& shader, const Material& material, const GraphicsNode& graphicsNode) override;

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_constants;
		Matrix4D m_defaultTransform;
	};
}
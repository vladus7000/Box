#pragma once

#include <string>
#include <memory>

struct ID3D11Device;
struct ID3D11DeviceContext;

namespace box
{
	class Shader;
	class Material;
	class GraphicsNode;

	class ShaderEnvironmentProvider
	{
	public:
		using ShaderEnvironmentProviderWeak = std::weak_ptr<ShaderEnvironmentProvider>;
		using ShaderEnvironmentProviderStrong = std::shared_ptr<ShaderEnvironmentProvider>;

		ShaderEnvironmentProvider() = default;
		virtual ~ShaderEnvironmentProvider() {};

		virtual void onDeviceLost() = 0;
		virtual bool restore(ID3D11Device* device) = 0;

		virtual std::string getName() = 0;

		virtual void prepareShader(ID3D11DeviceContext* context, Shader& shader, const Material& material, const GraphicsNode& graphicsNode) = 0;
	};
}
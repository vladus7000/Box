#pragma once

#include <string>
#include <memory>

namespace box
{
	class Shader;

	class ShaderEnvironmentProvider
	{
	public:
		using ShaderEnvironmentProviderWeak = std::weak_ptr<ShaderEnvironmentProvider>;
		using ShaderEnvironmentProviderStrong = std::shared_ptr<ShaderEnvironmentProvider>;

		ShaderEnvironmentProvider() = default;
		virtual ~ShaderEnvironmentProvider() {};
	
		virtual std::string getName() = 0;

		virtual void prepareShader(Shader& shader) = 0;
	};
}
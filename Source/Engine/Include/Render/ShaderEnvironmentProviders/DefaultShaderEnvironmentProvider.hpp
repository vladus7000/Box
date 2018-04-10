#pragma once

#include "ShaderEnvironmentProvider.hpp"

namespace box
{
	class DefaultShaderEnvironmentProvider : public ShaderEnvironmentProvider
	{
	public:
		DefaultShaderEnvironmentProvider() = default;
		virtual ~DefaultShaderEnvironmentProvider() {}

		std::string getName() { return "DefaultShaderEnvironmentProvider"; }

		virtual void prepareShader(Shader& shader) override;
	};
}
#pragma once

#include <memory>
#include <string>
#include "Shader.hpp"

namespace box
{
	class Material
	{
	public:
		using MaterialStrongPtr = std::shared_ptr<Material>;
		using MaterialWeakPtr = std::weak_ptr<Material>;

	public:
		Material(const std::string& name)
			: m_name(name)
		{}

		~Material() = default;

		void setShader(Shader::ShaderStrongPtr shader) { m_shader = shader; }
		Shader::ShaderWeakPtr getShader() const { return m_shader; }

		bool apply(ID3D11DeviceContext* context);

	private:
		Shader::ShaderStrongPtr m_shader;
		std::string m_name;
	};
}

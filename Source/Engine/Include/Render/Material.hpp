#pragma once

#include <memory>
#include <string>
#include "System/ResourceSystem/ResourceExtraData.hpp"
#include "Shader.hpp"
#include <tinyxml2/tinyxml2.h>

namespace box
{
	class Material : public ResourceExtraData
	{
	public:
		using MaterialStrongPtr = std::shared_ptr<Material>;
		using MaterialWeakPtr = std::weak_ptr<Material>;

	public:
		Material(const std::string& name = "")
			: m_name(name)
		{}

		virtual ~Material() = default;

		const std::string& getName() const override { return m_name; }
		void setName(const std::string& name) { m_name = name; }
		void setShader(Shader::ShaderStrongPtr shader) { m_shader = shader; }
		Shader::ShaderWeakPtr getShader() const { return m_shader; }

		bool apply(ID3D11DeviceContext* context);

		int getSizeForXML() const { return 0; };
		tinyxml2::XMLNode* serializeToXML(tinyxml2::XMLNode* node, tinyxml2::XMLDocument& doc) const;
		bool loadFromXML(tinyxml2::XMLNode* node);

	private:
		friend class MaterialResourceLoader;
		Shader::ShaderStrongPtr m_shader;
		std::string m_name;
	};
}

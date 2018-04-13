#pragma once

#include <memory>
#include <string>
#include "System/ResourceSystem/ResourceExtraData.hpp"
#include "Shader.hpp"
#include <tinyxml2/tinyxml2.h>
#include <D3D11.h>

namespace box
{
	class Material : public ResourceExtraData
	{
	public:
		using MaterialStrongPtr = std::shared_ptr<Material>;
		using MaterialWeakPtr = std::weak_ptr<Material>;
		static const U32 MaxTextures = 3;

	public:
		Material(const std::string& name = "")
			: m_name(name)
		{
			for (U32 i = 0; i < MaxTextures; i++)
			{
				m_textures[i] = nullptr;
			}
		}

		virtual ~Material();

		const std::string& getName() const override { return m_name; }
		void setName(const std::string& name) { m_name = name; }
		void setSrcFile(const std::string& file) { m_srcFile = file; }
		const std::string& getSrcFile() const { return m_srcFile; }
		void setShader(Shader::ShaderStrongPtr shader) { m_shader = shader; }
		Shader::ShaderWeakPtr getShader() const { return m_shader; }

		bool apply(ID3D11DeviceContext* context);

		ID3D11ShaderResourceView* getTexture(U32 slot) const { ASSERT(slot < MaxTextures); return m_textures[slot]; }

		int getSizeForXML() const { return 0; };
		tinyxml2::XMLNode* serializeToXML(tinyxml2::XMLNode* node, tinyxml2::XMLDocument& doc) const;
		bool loadFromXML(tinyxml2::XMLNode* node);

	private:
		void parseTexureSlot(tinyxml2::XMLElement* root, U32 slot, const char* name);
	private:
		friend class MaterialResourceLoader;
		Shader::ShaderStrongPtr m_shader;
		std::string m_name;
		std::string m_srcFile;

		ID3D11ShaderResourceView* m_textures[MaxTextures];
	};
}

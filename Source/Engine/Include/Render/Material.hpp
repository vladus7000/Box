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

		enum class TextureName_Def
		{
			ambient,
			diffuse,
			alpha,
			normal,
			specular,
			Max
		};

		enum class TextureName_Pbr
		{
			metallic,
			albedo,
			roughness,
			normal,
			mask,
			Max
		};
		static const U32 MaxTextures = static_cast<U32>(TextureName_Def::Max);

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
		void setShaderName(const std::string& shaderName) { m_shaderName = shaderName; }
		Shader::ShaderStrongPtr getShader() const { return m_shader; }

		ID3D11ShaderResourceView* getTexture(U32 slot) const { ASSERT(slot < MaxTextures); return m_textures[slot]; }
		void setTextureName(U32 slot, const std::string& path) { ASSERT(slot < MaxTextures); m_texturesNames[slot] = path; }

		bool apply(ID3D11DeviceContext* context);

		int getSizeForXML() const { return 0; };
		tinyxml2::XMLNode* serializeToXML(tinyxml2::XMLNode* node, tinyxml2::XMLDocument& doc) const;
		bool loadFromXML(tinyxml2::XMLNode* node);

	private:
		void parseTexureSlot(tinyxml2::XMLElement* root, U32 slot, const char* name);
	private:
		friend class MaterialResourceLoader;
		ID3D11ShaderResourceView* m_textures[MaxTextures];
		Shader::ShaderStrongPtr m_shader;
		std::string m_name;
		std::string m_srcFile;
		std::string m_shaderName;
		std::string m_texturesNames[MaxTextures];
	};
}

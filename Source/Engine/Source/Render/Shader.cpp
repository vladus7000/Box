#include "StdAfx.hpp"
#include "Render/Shader.hpp"

#include "System/ResourceSystem/ResourceManager.hpp"

namespace box
{
	Shader::Shader(const std::string& resourceName)
		: m_name(resourceName)
		, m_activeTechnique(-1)
	{}

	Shader::~Shader()
	{
		deinit();
	}

	void Shader::restore()
	{
		deinit();

		Resource r(m_name);
		m_resourceHandle = ResourceManager::Instance().getHandle(r);
		std::shared_ptr<ShaderResourceExtraData> shaderResources(std::static_pointer_cast<ShaderResourceExtraData>(m_resourceHandle->getExtra()));

		if (shaderResources)
		{
			m_techniques = shaderResources->m_techniques;
		}
	}

	void Shader::lostDevice()
	{
	}

	void Shader::deinit()
	{
		m_techniques.clear();
		m_resourceHandle = std::shared_ptr<ResourceHandle>();
	}
}

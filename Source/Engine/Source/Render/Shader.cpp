#include "StdAfx.hpp"
#include "Render/Shader.hpp"

#include "System/ResourceSystem/ResourceManager.hpp"
#include <Render/ResourceLoaders/ShaderResourceExtraData.hpp>

namespace box
{
	Shader::Shader(const std::string& resourceName)
		: m_name(resourceName)
		, m_blendState(nullptr)
		, m_hullShader(nullptr)
		, m_domainShader(nullptr)
		, m_geometryShader(nullptr)
		, m_computeShader(nullptr)
		, m_pixelShader(nullptr)
		, m_vertexShader(nullptr)
		, m_inputLayout(nullptr)
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
			m_blendState = shaderResources->m_blendState;
			m_hullShader = shaderResources->m_hullShader;
			m_domainShader = shaderResources->m_domainShader;
			m_geometryShader = shaderResources->m_geometryShader;
			m_computeShader = shaderResources->m_computeShader;
			m_pixelShader = shaderResources->m_pixelShader;
			m_vertexShader = shaderResources->m_vertexShader;
			m_inputLayout = shaderResources->m_inputLayout;
		}
	}

	void Shader::lostDevice()
	{
	}

	void Shader::deinit()
	{
		m_resourceHandle = std::shared_ptr<ResourceHandle>();
		RELEASE(m_blendState);
		RELEASE(m_hullShader);
		RELEASE(m_domainShader);
		RELEASE(m_geometryShader);
		RELEASE(m_computeShader);
		RELEASE(m_pixelShader);
		RELEASE(m_vertexShader);
		RELEASE(m_inputLayout);
	}
}

#include "StdAfx.hpp"
#include <Render/ResourceLoaders/ShaderResourceExtraData.hpp>

namespace box
{
	ShaderResourceExtraData::ShaderResourceExtraData()
		: m_blendState(nullptr)
		, m_hullShader(nullptr)
		, m_domainShader(nullptr)
		, m_geometryShader(nullptr)
		, m_computeShader(nullptr)
		, m_pixelShader(nullptr)
		, m_vertexShader(nullptr)
		, m_inputLayout(nullptr)
	{
	}

	ShaderResourceExtraData::~ShaderResourceExtraData()
	{
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
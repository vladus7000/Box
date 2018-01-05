#include "StdAfx.hpp"
#include "System/Process/Processes/PreloadProcess.hpp"
#include "System/ResourceSystem/ResourceCache.hpp"

namespace box
{

	PreloadProcess::PreloadProcess(const std::string& pattern, ResourceCache* cache)
		: m_pattern(pattern)
		, m_cache(cache)
	{

	}

	void PreloadProcess::onUpdate(F32 delta)
	{
		m_cache->preload(m_pattern);
		succeeded();
	}
}
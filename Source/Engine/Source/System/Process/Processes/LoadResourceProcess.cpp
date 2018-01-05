#include "StdAfx.hpp"
#include "System/Process/Processes/LoadResourceProcess.hpp"
#include "System/ResourceSystem/ResourceCache.hpp"

namespace box
{

	LoadResourceProcess::LoadResourceProcess(ResourceHandle::StrongResourceHandlePtr handle)
		: m_handle(handle)
	{

	}

	void LoadResourceProcess::onUpdate(F32 delta)
	{
		auto cache = m_handle->m_cache;
		cache->load(m_handle);
		succeeded();
	}
}
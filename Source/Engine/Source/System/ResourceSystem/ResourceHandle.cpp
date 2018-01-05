#include "StdAfx.hpp"
#include "System\ResourceSystem\ResourceHandle.hpp"
#include "System\ResourceSystem\ResourceCache.hpp"

namespace box
{
	ResourceHandle::~ResourceHandle()
	{
		if (m_buffer)
		{
			delete[] m_buffer;
			m_buffer = nullptr;
		}
		m_size = 0;
		m_dataReady = false;
		m_cache->memoryHasBeenFreed(m_size);
	}
}
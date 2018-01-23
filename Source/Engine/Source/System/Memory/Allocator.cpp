#include "StdAfx.hpp"
#include "System\Memory\Allocator.hpp"

#include <stdlib.h>

namespace
{
	box::Allocator* g_allocator;
}

void * operator new(size_t size, size_t allignement)
{
	if (g_allocator)
	{
		g_allocator->m_allocatedMemory += size;
	}
	return _aligned_malloc(size, allignement);
}

void * operator new(size_t size)
{
	size_t originalSize = size;
	if (g_allocator)
	{
		g_allocator->m_allocatedMemory += originalSize;
		g_allocator->m_allocatedMemoryWithOverhead += (originalSize + sizeof(size_t));
	}

	size += sizeof(size_t);

	void* prt = _aligned_malloc(size, 1);
	size_t* sizeVal = reinterpret_cast<size_t*>(prt);
	*sizeVal = originalSize;
	box::U8* ret = reinterpret_cast<box::U8*>(prt) + sizeof(size_t);

	return reinterpret_cast<void*>(ret);
}

void* operator new[](size_t size)
{
	size_t originalSize = size;
	if (g_allocator)
	{
		g_allocator->m_allocatedMemory += originalSize;
		g_allocator->m_allocatedMemoryWithOverhead += (originalSize + sizeof(size_t));
	}

	size += sizeof(size_t);

	void* ptr = _aligned_malloc(size, 1);
	size_t* sizeVal = reinterpret_cast<size_t*>(ptr);
	*sizeVal = originalSize;
	box::U8* ret = reinterpret_cast<box::U8*>(ptr) + sizeof(size_t);

	return reinterpret_cast<void*>(ret);
}

void operator delete(void* ptr)
{
	if (!ptr)
	{
		return;
	}

	box::U8* originalPtr = reinterpret_cast<box::U8*>(ptr) - sizeof(size_t);
	size_t* sizeVal = reinterpret_cast<size_t*>(originalPtr);

	if (g_allocator)
	{
		g_allocator->m_allocatedMemory -= *sizeVal;
		g_allocator->m_allocatedMemoryWithOverhead -= (*sizeVal + sizeof(size_t));
	}

	_aligned_free(originalPtr);
}

void operator delete[](void* ptr)
{
	if (!ptr)
	{
		return;
	}

	box::U8* originalPtr = reinterpret_cast<box::U8*>(ptr) - sizeof(size_t);
	size_t* sizeVal = reinterpret_cast<size_t*>(originalPtr);

	if (g_allocator)
	{
		g_allocator->m_allocatedMemory -= *sizeVal;
		g_allocator->m_allocatedMemoryWithOverhead -= (*sizeVal + sizeof(size_t));
	}

	_aligned_free(originalPtr);
}

namespace box
{
	SINGLETON_ACCESSOR(Allocator);

	bool Allocator::init()
	{
		m_allocatedMemory = 0;
		m_allocatedMemoryWithOverhead = 0;
		g_allocator = this;
		return true;
	}

	void Allocator::deinit()
	{
		g_allocator = nullptr;
		ASSERT(m_allocatedMemory == 0);
		ASSERT(m_allocatedMemoryWithOverhead == 0);
	}

	Allocator::MemoryStats Allocator::getStats()
	{
		MemoryStats ret;
		ret.allocatedMemory = m_allocatedMemory;
		ret.allocatedMemoryWithOverhead = m_allocatedMemoryWithOverhead;
		return ret;
	}
}
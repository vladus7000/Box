#include "StdAfx.hpp"
#include "System\Memory\Allocator.hpp"

#include <stdlib.h>

void * operator new(size_t size)
{
	return _aligned_malloc(size, 1);
}

void * operator new[](size_t size)
{
	return _aligned_malloc(size, 1);
}

void operator delete(void * ptr)
{
	_aligned_free(ptr);
}

void operator delete[](void * ptr)
{
	_aligned_free(ptr);
}

namespace box
{
	bool Allocator::init()
	{
		m_allocatedMemoryMb = 0;

		return true;
	}

	void Allocator::deinit()
	{
		//TODO assert(m_allocatedMemoryMb == 0);
	}
}
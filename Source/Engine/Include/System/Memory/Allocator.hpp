#pragma once
#include <memory>
#include "System\Patterns\Singleton.hpp"
void* operator new (size_t size, size_t allignement);
void* operator new (size_t size);
void* operator new[](size_t size);

void operator delete (void* ptr);
void operator delete[](void* ptr);

namespace box
{
	class Allocator
	{
		SINGLETON(Allocator);
	public:
		bool init();
		void deinit();
	private:
		U32 m_allocatedMemoryMb;
	};
}
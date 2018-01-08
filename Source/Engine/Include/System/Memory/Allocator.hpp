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
		struct MemoryStats 
		{
			size_t allocatedMemory;
			size_t allocatedMemoryWithOverhead;
		};

		bool init();
		void deinit();

		MemoryStats getStats();

	private:
		friend void* ::operator new (size_t size, size_t allignement);
		friend void* ::operator new (size_t size);
		friend void* ::operator new[](size_t size);

		friend void ::operator delete (void* ptr);
		friend void ::operator delete[](void* ptr);

		size_t m_allocatedMemory;
		size_t m_allocatedMemoryWithOverhead;
	};
}
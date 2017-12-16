#pragma once
#include "System\Patterns\Singleton.hpp"

namespace box
{
	class Display
	{
		SINGLETON(Display);
	public:
		bool init(void* displayHandle);
		void deinit();

		void* getDisplayHandle() const { return m_displayHandle; }

	private:
		void* m_displayHandle;
	};
}
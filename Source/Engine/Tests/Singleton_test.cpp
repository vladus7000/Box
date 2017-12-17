#include "gtest/gtest.h"
#include "System/Types.hpp"
#include "System/Patterns/Singleton.hpp"

namespace
{
	class A
	{
		SINGLETON(A);
	public:
		void foo() {}
	};
	SINGLETON_ACCESSOR(A);
	
	void foo()
	{
			A::Instance().foo();
	}

}
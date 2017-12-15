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
	
	void foo()
	{
			A::Instance().foo();
	}

}
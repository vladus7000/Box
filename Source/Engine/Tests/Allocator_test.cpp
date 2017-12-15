#include "gtest/gtest.h"
#include "System/Types.hpp"
#include "System/Memory/Allocator.hpp"

using namespace box;

TEST(Allocator_Startup_test, Positive)
{
	const bool result = Allocator::Instance().init();
	Allocator::Instance().deinit();
	EXPECT_TRUE(result);
}

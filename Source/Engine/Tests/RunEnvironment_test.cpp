#include "gtest/gtest.h"
#include "System/Types.hpp"
#include "System/RunEnvironment.hpp"

using namespace box;

TEST(RunEnvironment_Startup_test, Positive)
{
	const int argc = 1;
	char* argv[] = { "Path", 0 };
	const bool result = RunEnvironment::Instance().init(argc, (char**)argv);
	RunEnvironment::Instance().deinit();
	EXPECT_TRUE(result);
}
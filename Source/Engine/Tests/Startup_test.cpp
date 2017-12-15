#include "gtest/gtest.h"
#include "System/Types.hpp"
#include "System/Engine.hpp"

using namespace box;

TEST(Engine_Startup_test, Positive)
{
	
	box::Engine engine;
	const bool result = engine.startup();
	engine.shutdown();
	EXPECT_TRUE(result);
}

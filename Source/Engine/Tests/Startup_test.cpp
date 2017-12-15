#include "gtest/gtest.h"
#include "System/Types.hpp"
#include "System/Engine.hpp"

TEST(StartupTest, Positive) {
	
	box::Engine engine;
	const bool result = engine.startup();
	EXPECT_TRUE(result);
}

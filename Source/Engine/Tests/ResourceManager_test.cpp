#include "gtest/gtest.h"
#include "System/Types.hpp"
#include "System/ResourceSystem/ResourceManager.hpp"

using namespace box;

TEST(ResourceManager_Startup_test, Positive)
{
	
	const bool result = ResourceManager::Instance().init();
	ResourceManager::Instance().deinit();
	EXPECT_TRUE(result);
}

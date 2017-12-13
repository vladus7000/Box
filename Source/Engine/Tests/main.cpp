#include <stdio.h>
#include <System/Engine.hpp>
#include "gtest/gtest.h"

int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
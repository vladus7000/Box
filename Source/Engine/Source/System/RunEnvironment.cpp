#pragma once
#include "StdAfx.hpp"
#include "System/RunEnvironment.hpp"

namespace box
{
	bool RunEnvironment::init(U32 argc, char** argv)
	{
		(void)argc; (void)argv;

		return true;
	}

	void RunEnvironment::deinit()
	{
	}
}
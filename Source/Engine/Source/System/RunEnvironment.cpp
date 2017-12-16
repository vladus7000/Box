#pragma once
#include "StdAfx.hpp"
#include "System/RunEnvironment.hpp"

namespace box
{
	SINGLETON_ACCESSOR(RunEnvironment);

	bool RunEnvironment::init(U32 argc, char** argv)
	{
		(void)argc; (void)argv;

		return true;
	}

	void RunEnvironment::deinit()
	{
	}
}
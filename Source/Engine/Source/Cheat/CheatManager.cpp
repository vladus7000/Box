#include "StdAfx.hpp"
#include <Cheat\CheatManager.hpp>

namespace box
{
	SINGLETON_ACCESSOR(CheatManager);

	bool CheatManager::init()
	{
		return false;
	}

	void CheatManager::deinit()
	{
	}
}
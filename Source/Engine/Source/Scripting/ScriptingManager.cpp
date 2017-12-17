#include "StdAfx.hpp"
#include "Scripting\ScriptingManager.hpp"

namespace box
{
	SINGLETON_ACCESSOR(ScriptingManager);

	bool ScriptingManager::init()
	{
		return false;
	}

	void ScriptingManager::deinit()
	{
	}
}
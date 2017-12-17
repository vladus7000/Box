#include "StdAfx.hpp"
#include "Sound\SoundSystem.hpp"

namespace box
{
	SINGLETON_ACCESSOR(SoundSystem);

	bool SoundSystem::init()
	{
		return false;
	}

	void SoundSystem::deinit()
	{
	}
}
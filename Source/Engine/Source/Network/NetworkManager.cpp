#include "StdAfx.hpp"
#include <Network\NetworkManager.hpp>

namespace box
{
	SINGLETON_ACCESSOR(NetworkManager);

	bool NetworkManager::init()
	{
		return false;
	}

	void NetworkManager::deinit()
	{
	}
}
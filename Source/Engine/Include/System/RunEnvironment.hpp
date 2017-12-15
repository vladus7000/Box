#pragma once
#include <string>
#include <map>
#include "Patterns/Singleton.hpp"

namespace box
{
	class RunEnvironment
	{
		SINGLETON(RunEnvironment);
	public:
		bool init(U32 argc, char** argv);
		void deinit();

	private:
		std::map<std::string, std::string> m_commandLine;
	};
}
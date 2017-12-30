#pragma once

#include <string>

namespace box
{
	class File
	{
	public:
		File();
		~File();



	private:
		std::string m_name;
		std::string m_ext;
		std::string m_fullPath;
	};
}
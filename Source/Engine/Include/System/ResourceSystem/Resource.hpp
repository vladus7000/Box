#pragma once

#include <string>
#include <algorithm>
#include <functional>

namespace box
{
	struct Resource
	{
		explicit Resource(const std::string& name)
			: m_name(name)
			, m_guid(0)
		{
			std::transform(m_name.begin(), m_name.end(), m_name.begin(), ::tolower);
			std::hash<std::string> hash_fn;
			m_guid = hash_fn(m_name);
		}

		bool operator == (const Resource& r) const
		{
			return r.m_guid == m_guid;
		}

		std::string m_name;
		U64 m_guid;
	};
}
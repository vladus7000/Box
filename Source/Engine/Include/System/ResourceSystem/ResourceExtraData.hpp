#pragma once

#include <string>

namespace box
{
	class ResourceExtraData
	{
	public:
		ResourceExtraData() = default;
		virtual ~ResourceExtraData() = default;

		virtual const std::string& getName() const = 0;
	};
}
#pragma once

#include <string>
#include <memory>

namespace box
{
	class ResourceExtraData
	{
	public:
		using ResourceExtraDataStrongPtr = std::shared_ptr<ResourceExtraData>;
		using ResourceExtraDataWeakPtr = std::weak_ptr<ResourceExtraData>;

		ResourceExtraData() = default;
		virtual ~ResourceExtraData() = default;

		virtual const std::string& getName() const = 0;
	};
}
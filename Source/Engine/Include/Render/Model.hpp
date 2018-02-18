#pragma once

#include "System/ResourceSystem/ResourceExtraData.hpp"

namespace box
{
	class Model : public ResourceExtraData
	{
	public:
		Model() = default;
		~Model() = default;

		virtual const std::string& getName() const { return "Model"; }
	};
}

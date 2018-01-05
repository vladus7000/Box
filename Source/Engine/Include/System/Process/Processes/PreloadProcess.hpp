#pragma once

#include <string>
#include "System/Process/Process.hpp"

namespace box
{
	class ResourceCache;
}

namespace box
{
	class PreloadProcess : public Process
	{
	public:
		PreloadProcess(const std::string& pattern, ResourceCache* cache);

	protected:
		virtual void onUpdate(F32 delta) override;

	private:
		std::string m_pattern;
		ResourceCache* m_cache;
	};
}
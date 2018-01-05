#pragma once

#include <string>
#include "System/Process/Process.hpp"
#include "System/ResourceSystem/ResourceHandle.hpp"

namespace box
{
	class ResourceCache;
}

namespace box
{
	class LoadResourceProcess : public Process
	{
	public:
		LoadResourceProcess(ResourceHandle::StrongResourceHandlePtr handle);

	protected:
		virtual void onUpdate(F32 delta) override;

	private:
		ResourceHandle::StrongResourceHandlePtr m_handle;
	};
}
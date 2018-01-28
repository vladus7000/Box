#pragma once

#include "System/ResourceSystem/ResourceExtraData.hpp"

#include <DXUT11\Core\DXUT.h>
#include "SDKmisc.h"
#include "SDKMesh.h"

namespace box
{
	class SdkmeshResourceExtraData : public ResourceExtraData
	{
	public:
		class TextureData;
		~SdkmeshResourceExtraData() = default;

		virtual const std::string& getName() const { return "SdkmeshResourceExtraData"; }
		const CDXUTSDKMesh& getMesh() const { return m_mesh; }
		CDXUTSDKMesh& getMesh() { return m_mesh; }
	private:
		friend class SdkmeshResourceLoader;
		CDXUTSDKMesh m_mesh;
	};
}
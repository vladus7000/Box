#include "StdAfx.hpp"
#include <Render/ResourceLoaders/SdkmeshResourceLoader.hpp>
#include <Render/ResourceLoaders/SdkmeshResourceExtraData.hpp>
#include <System/ResourceSystem/ResourceHandle.hpp>
#include <System/ResourceSystem/ResourceManager.hpp>

namespace box
{
	bool SdkmeshResourceLoader::loadResource(U8* buffer, size_t size, std::shared_ptr<ResourceHandle> handle)
	{
		std::shared_ptr<SdkmeshResourceExtraData> extra(new SdkmeshResourceExtraData());

		ID3D11Device* device = DXUTGetD3D11Device();
		auto hr = extra->m_mesh.Create(device, buffer, size);
		if (FAILED(hr))
		{
			return false;
		}
		else
		{
			handle->setExtra(extra);
			return true;
		}
	}
}
#pragma once 

#include "ResourceCache.hpp"
#include "Resource.hpp"

namespace box
{
	class ResourceExtraData;

	class ResourceHandle
	{
		friend class ResourceCache;
	public:
		using StrongResourceHandlePtr = std::shared_ptr<ResourceHandle>;
		using WeakResourceHandlePtr = std::weak_ptr<ResourceHandle>;

	public:
		ResourceHandle(const Resource& res, U8* buffer, size_t size, ResourceCache* cache)
			: m_resource(res)
			, m_buffer(buffer)
			, m_size(size)
			, m_cache(cache)
		{}
		virtual ~ResourceHandle()
		{
			if (m_buffer)
			{
				delete[] m_buffer;
				m_buffer = nullptr;
			}
			m_cache->memoryHasBeenFreed(m_size);
		}

		size_t getSize() const { return m_size; }
		const U8* buffer() const { return m_buffer; }
		U8* buffer() { return m_buffer; }
		std::shared_ptr<ResourceExtraData> getExtra() { return m_extra; }
		void setExtra(std::shared_ptr<ResourceExtraData> extra) { m_extra = extra; }

	protected:
		Resource m_resource;
		U8* m_buffer;
		size_t m_size;
		std::shared_ptr<ResourceExtraData> m_extra;
		ResourceCache* m_cache;
	};
}
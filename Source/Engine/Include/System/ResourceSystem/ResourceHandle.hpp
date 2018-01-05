#pragma once 

#include <memory>

#include "Resource.hpp"

namespace box
{
	class ResourceExtraData;

	class ResourceHandle
	{
		friend class ResourceCache;
		friend class LoadResourceProcess;
	public:
		using StrongResourceHandlePtr = std::shared_ptr<ResourceHandle>;
		using WeakResourceHandlePtr = std::weak_ptr<ResourceHandle>;

	public:
		ResourceHandle(const Resource& res, U8* buffer, size_t size, ResourceCache* cache)
			: m_resource(res)
			, m_buffer(buffer)
			, m_size(size)
			, m_cache(cache)
			, m_dataReady(false)
		{}
		virtual ~ResourceHandle();

		size_t getSize() const { return m_size; }
		const U8* buffer() const { return m_buffer; }
		U8* buffer() { return m_buffer; }
		std::shared_ptr<ResourceExtraData> getExtra() { return m_extra; }
		void setExtra(std::shared_ptr<ResourceExtraData> extra) { m_extra = extra; }
		bool isDataReady() const { return m_dataReady; }
		void setDataReady() { m_dataReady = true; }

	protected:
		Resource m_resource;
		U8* m_buffer;
		size_t m_size;
		std::shared_ptr<ResourceExtraData> m_extra;
		ResourceCache* m_cache;
		bool m_dataReady;
	};
}
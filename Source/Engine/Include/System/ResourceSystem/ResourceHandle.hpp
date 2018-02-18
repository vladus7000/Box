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

		enum class Status
		{
			NotInited,
			WaitingForLoading,
			Loading,
			Ready,
			ResourceNotFound,
			LoaderNotDound
		};

	public:
		ResourceHandle(const Resource& res, U8* buffer, size_t size, ResourceCache* cache)
			: m_resource(res)
			, m_buffer(buffer)
			, m_size(size)
			, m_cache(cache)
			, m_status(Status::NotInited)
		{}
		virtual ~ResourceHandle();

		size_t getSize() const { return m_size; }
		const U8* buffer() const { return m_buffer; }
		U8* buffer() { return m_buffer; }
		std::shared_ptr<ResourceExtraData> getExtra() { return m_extra; }

		template <class T>
		std::shared_ptr<T> getExtraTyped() { return std::static_pointer_cast<T>(m_extra); }

		void setExtra(std::shared_ptr<ResourceExtraData> extra) { m_extra = extra; }
		bool isDataReady() const { return m_status == Status::Ready; }
		void setDataReady() { m_status = Status::Ready; }
		Status getStatus() const { return m_status; }
		const Resource& getResource() const { return m_resource; }

	protected:
		Resource m_resource;
		U8* m_buffer;
		size_t m_size;
		std::shared_ptr<ResourceExtraData> m_extra;
		ResourceCache* m_cache;
		Status m_status;
	};
}
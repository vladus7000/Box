#pragma once
#include <memory>

namespace box
{
	class Actor;
	class Component
	{
		friend class ActorComponentFactory;
	public:
		using ComponentId = U64;
		using StrongComponentPtr = std::shared_ptr<Component>;
		using WeakComponentPtr = std::weak_ptr<Component>;

	public:
		Component() {}
		virtual ~Component() {}

		virtual bool init();
		virtual void deinit();

		virtual void update(F32 delta) = 0;

		ComponentId getId() const { return m_componentId; }
		void setOwner(std::shared_ptr<Actor> owner) { m_owner = owner; }

	protected:
		ComponentId m_componentId;
	private:
		std::shared_ptr<Actor> m_owner;
	};
}
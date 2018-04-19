#pragma once
#include <memory>
#include <tinyxml2/tinyxml2.h>

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

		virtual bool init() { return true; };
		virtual void deinit() {};

		virtual void update(F32 delta) {};

		ComponentId getId() const { return m_componentId; }
		void setOwner(std::weak_ptr<Actor> owner) { m_owner = owner; }

		virtual tinyxml2::XMLNode* serializeToXML(tinyxml2::XMLNode* node, tinyxml2::XMLDocument& doc) const { return nullptr; }
		virtual bool loadFromXML(tinyxml2::XMLNode* node) { return true; }

	protected:
		ComponentId m_componentId;
	private:
		std::weak_ptr<Actor> m_owner;
	};
}
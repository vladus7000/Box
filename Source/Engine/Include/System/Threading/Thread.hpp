#pragma once

#include <string>
#include <memory>

namespace box
{
	class Thread
	{
	public:
		enum class State
		{
			Uninited,
			Inited,
			Running,
			Paused,
			Stopped
		};
		enum class Priority
		{
			TimeCritical,
			Higest,
			AboveNormal,
			Normal,
			BelowNormal,
			Lowest,
			Idle
		};
		using StrongThreadPtr = std::shared_ptr<Thread>;
		using WeakThreadPtr = std::weak_ptr<Thread>;

		Thread(const std::string& name = "Unnamed", U32 stackSize = 0, U64 mask = 0xFFFFFFFFFFFFFFFF, Priority priority = Priority::Normal);
		virtual ~Thread();

		virtual S32 run() = 0;
		virtual void init();
		virtual void start();
		virtual void stop();
		virtual void pause();
		virtual void unPause();

		const std::string& getName() const { return m_name; }
		U64 getAffinityMask() const { return m_affinityMask; }
		State getState() const { return m_state; }
		Priority getPriority() const { return m_priority; }
		S32 getResult();

		void* getShadow() { return m_shadow; }

	protected:
		std::string m_name;
		U32 m_stackSize;
		U64 m_affinityMask;
		U8* m_shadow[32];
		bool m_stopped;
		State m_state;
		Priority m_priority;
	};
}

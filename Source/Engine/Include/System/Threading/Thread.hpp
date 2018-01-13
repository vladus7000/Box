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
		using StrongThreadPtr = std::shared_ptr<Thread>;
		using WeakThreadPtr = std::weak_ptr<Thread>;

		Thread(const std::string& name = "", U32 stackSize = 0, U32 mask = 0xFFFFFFFF);
		virtual ~Thread();

		virtual S32 run() = 0;
		virtual void init();
		virtual void start();
		virtual void stop();
		virtual void pause();
		virtual void unPause();

		State getState() const { return m_state; }
		S32 getResult();

		void* getShadow() { return m_shadow; }

	protected:
		std::string m_name;
		U32 m_stackSize;
		U32 m_affinityMask;
		U8* m_shadow[10];
		bool m_stopped;
		State m_state;
	};
}
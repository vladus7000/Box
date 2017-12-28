#pragma once

#include <memory>

namespace box
{
	class Process
	{
		friend class ProcessManager;
	public:
		using StrongProcessPtr = std::shared_ptr<Process>;
		using WeakProcessPtr = std::weak_ptr<Process>;

		enum class State
		{
			Uninitialized = 0,
			Removed,
			Running,
			Paused,
			Succeeded,
			Failed,
			Aborted,

			Count
		};

	public:
		Process() : m_state(State::Uninitialized) {}
		virtual ~Process() {}

		void succeeded() { setState(State::Succeeded); }
		void fail() { setState(State::Failed); }

		void pause() { setState(State::Paused); }
		void unpause() { setState(State::Running); }

		State getState() const { return m_state; }
		bool isAlive() const { return (m_state == State::Running || m_state == State::Paused); }
		bool isPaused() const { return m_state == State::Paused; }
		bool isRemoved() const { return m_state == State::Removed; }
		bool isDead() const { return (m_state == State::Aborted || m_state == State::Failed || m_state == State::Succeeded); }

		void attachChild(StrongProcessPtr child) { m_child = child; }
		StrongProcessPtr removeChild() { StrongProcessPtr tmp; std::swap(tmp, m_child); return tmp; }
		StrongProcessPtr peekChild() const { return m_child; }

	protected:
		virtual void onInit() { setState(State::Running); }
		virtual void onUpdate(F32 delta) = 0;
		virtual void onSucceed() {}
		virtual void onFail() {}
		virtual void onAbort() {}

	private:
		void setState(State state) { m_state = state; }

	private:
		State m_state;
		StrongProcessPtr m_child;
	};
}
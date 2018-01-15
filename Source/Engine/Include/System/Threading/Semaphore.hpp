#pragma once

namespace box
{
	class Semaphore
	{
	public:
		Semaphore(U32 initCount = 0, U32 maxCount = 0x7FFFFFFF);
		~Semaphore();

		void signal();
		void wait();
		bool tryWait();

	private:
		Semaphore(const Semaphore&);
		Semaphore(Semaphore&&);
		Semaphore& operator=(const Semaphore&);
		Semaphore&operator=(Semaphore&&);

		U8 m_shadow[64];
	};
}
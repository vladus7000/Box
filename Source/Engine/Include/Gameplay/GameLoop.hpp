#pragma once

namespace box
{
	class GameLoop
	{
	public:
		virtual ~GameLoop() = default;
		virtual void logicTick(F64 fTime, F32 fElapsedTime) = 0;
		virtual void leaveGameLoop() = 0;
	};
}
#pragma once

#include "Types.hpp"

namespace box
{
	struct MachineInfo
	{
		enum class MachineType : U8
		{
			WindowsPC,
			LinuxPC,
			XBOX,
			SonyPlaystation,

			Unrecognizable
		};

		enum class GpuInfo : U8
		{
			Nvidia,
			Amd,
			Intel,

			Unrecognizable
		};

		enum class CpuInfo : U8
		{
			Intel32,
			Intel64,
			Amd32,
			Amd64,

			Unrecognizable
		};

		enum class InputDevices : U32
		{
			Nothing = 0,
			Mouse = 1,
			Keyboard = Mouse << 1,
			Gamepad = Keyboard << 1
		};

		MachineInfo()
			: m_ramSizeMb(0)
			, m_virtualRamSizeMb(0)
			, m_diskSizeMb(0)
			, m_videoRamSizeMb(0)
			, m_cpuSpeedMhz(0)
			, m_gpuSpeedMhz(0)
			, m_machneType(MachineType::Unrecognizable)
			, m_gpu(GpuInfo::Unrecognizable)
			, m_cpu(CpuInfo::Unrecognizable)
			, m_inputDevices(InputDevices::Nothing)
		{}

		bool detect();

		U32 m_ramSizeMb;
		U32 m_virtualRamSizeMb;
		U32 m_diskSizeMb;
		U32 m_videoRamSizeMb;
		U32 m_cpuSpeedMhz;
		U32 m_gpuSpeedMhz;
		MachineType m_machneType;
		GpuInfo m_gpu;
		CpuInfo m_cpu;
		InputDevices m_inputDevices;
	};
}
#pragma once

#include <d3d11.h>
#include "Scene/EnvironmentSettings.hpp"
#include <wrl.h>

namespace box
{
	class FrameGlobals final
	{
	public:
		FrameGlobals();
		~FrameGlobals();

		FrameGlobals(const FrameGlobals&) = delete;
		FrameGlobals(FrameGlobals&&) = delete;
		FrameGlobals& operator=(const FrameGlobals&) = delete;
		FrameGlobals& operator=(FrameGlobals&&) = delete;

		void onDeviceLost();
		bool restore(ID3D11Device* device);

		void updateEnvironmentSettings(const EnvironmentSettings& settings);
		void bind(ID3D11DeviceContext* context);
		void update(ID3D11DeviceContext* context, F32 delta);

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_frameConstants;
		Vector3D m_sunPosition;
		Vector3D m_sunColor;
	};
}
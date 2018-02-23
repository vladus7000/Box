#pragma once

#include <d3d11.h>

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

		void bind(ID3D11DeviceContext* context);
		void update(ID3D11DeviceContext* context, F32 delta);

	private:
		ID3D11Buffer* m_frameConstants;
	};
}
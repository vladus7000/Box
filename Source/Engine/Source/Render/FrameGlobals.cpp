#include "StdAfx.hpp"
#include "Render/FrameGlobals.hpp"
#include "Render/Renderer.hpp"
#include "Window/Window.hpp"

namespace box
{
	struct ShaderData
	{
		float cameraViewMatrix[16];
		float cameraProjectionMatrix[16];
		float cameraPosition[4];
		float cameraTarget[4];
		float screenW_screenH_InvGamma_0[4];
		float sunPosition[4];
		float sunColor[4];
	};

	FrameGlobals::FrameGlobals()
		: m_frameConstants(nullptr)
		, m_gamma(2.2f)
	{
	}

	FrameGlobals::~FrameGlobals()
	{
	}

	void FrameGlobals::onDeviceLost()
	{
		m_frameConstants.Reset();
	}

	bool FrameGlobals::restore(ID3D11Device* device)
	{
		onDeviceLost();

		D3D11_BUFFER_DESC desc;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.ByteWidth = sizeof(ShaderData);
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;

		device->CreateBuffer(&desc, nullptr, m_frameConstants.GetAddressOf());

		return m_frameConstants != nullptr;
	}

	void FrameGlobals::bind(ID3D11DeviceContext* context)
	{
		context->VSSetConstantBuffers(0, 1, m_frameConstants.GetAddressOf());
		context->PSSetConstantBuffers(0, 1, m_frameConstants.GetAddressOf());
	}

	void FrameGlobals::update(ID3D11DeviceContext* context, F32 delta)
	{
		D3D11_MAPPED_SUBRESOURCE subResource;
		context->Map(m_frameConstants.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);

		ShaderData* shaderData =  reinterpret_cast<ShaderData*>(subResource.pData);

		if (shaderData)
		{
			auto camera = Renderer::Instance().getCamera().lock();

			memcpy(shaderData->cameraViewMatrix, camera->getViewMatrix().m, sizeof(float) * 16);
			memcpy(shaderData->cameraProjectionMatrix, camera->getProjectionMatrix().m, sizeof(float) * 16);

			shaderData->cameraPosition[0] = camera->getPosition().x;
			shaderData->cameraPosition[1] = camera->getPosition().y;
			shaderData->cameraPosition[2] = camera->getPosition().z;
			shaderData->cameraPosition[3] = 1.0f;

			shaderData->cameraTarget[0] = camera->getTarget().x;
			shaderData->cameraTarget[1] = camera->getTarget().y;
			shaderData->cameraTarget[2] = camera->getTarget().z;
			shaderData->cameraTarget[3] = 1.0f;

			shaderData->screenW_screenH_InvGamma_0[0] = static_cast<F32>(Window::Instance().getWidth());
			shaderData->screenW_screenH_InvGamma_0[1] = static_cast<F32>(Window::Instance().getHeight());
			shaderData->screenW_screenH_InvGamma_0[2] = 1.0f / m_gamma;
			shaderData->screenW_screenH_InvGamma_0[3] = 0.0f;

			shaderData->sunPosition[0] = m_sunPosition.x;
			shaderData->sunPosition[1] = m_sunPosition.y;
			shaderData->sunPosition[2] = m_sunPosition.z;
			shaderData->sunPosition[3] = 1.0f;

			shaderData->sunColor[0] = m_sunColor.x;
			shaderData->sunColor[1] = m_sunColor.y;
			shaderData->sunColor[2] = m_sunColor.z;
			shaderData->sunColor[3] = 1.0f;
		}
		context->Unmap(m_frameConstants.Get(), 0);

	}

	void FrameGlobals::updateEnvironmentSettings(const EnvironmentSettings& settings)
	{
		m_sunPosition = settings.sunPosition;
		m_sunColor = settings.sunColor;
		m_gamma = settings.gamma;
	}

}
#include "StdAfx.hpp"

#include "Render/ShaderEnvironmentProviders/PbrShaderEnvironmentProvider.hpp"
#include "Render/Material.hpp"
#include "Render/Mesh.hpp"
#include "Render/GraphicsNode.hpp"

namespace box
{
	struct Constants
	{
		float transformMatrix[16];
	};

	PbrShaderEnvironmentProvider::PbrShaderEnvironmentProvider()
		: m_constants(nullptr)
	{
		m_defaultTransform.setIdentity();
	}

	PbrShaderEnvironmentProvider::~PbrShaderEnvironmentProvider()
	{
	}

	void PbrShaderEnvironmentProvider::onDeviceLost()
	{
	}

	bool PbrShaderEnvironmentProvider::restore(ID3D11Device* device)
	{
		D3D11_BUFFER_DESC desc;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.ByteWidth = sizeof(Constants);
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;

		device->CreateBuffer(&desc, nullptr, m_constants.GetAddressOf());

		return true;
	}

	void PbrShaderEnvironmentProvider::prepareShader(ID3D11DeviceContext* context, Shader& shader, const Material& material, const GraphicsNode& graphicsNode)
	{
		D3D11_MAPPED_SUBRESOURCE subResource;
		context->Map(m_constants.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);

		Constants* shaderData = reinterpret_cast<Constants*>(subResource.pData);

		if (shaderData)
		{
			if (TransformComponent* component = graphicsNode.getTransform())
			{
				memcpy(shaderData->transformMatrix, component->m_transformMatrix.m, sizeof(float) * 16);
			}
			else
			{
				memcpy(shaderData->transformMatrix, m_defaultTransform.m, sizeof(float) * 16);
			}
		}
		context->Unmap(m_constants.Get(), 0);

		if (auto texture = material.getTexture(1))
		{
			ID3D11SamplerState* samplerStae[] = { texture->getSamplerState_Raw() };
			ID3D11ShaderResourceView* srv[] = { texture->getSRV_Raw() };

			context->PSSetSamplers(0, 1, samplerStae);
			context->PSSetShaderResources(0, 1, srv);
		}

		context->VSSetConstantBuffers(1, 1, m_constants.GetAddressOf());
	}

}
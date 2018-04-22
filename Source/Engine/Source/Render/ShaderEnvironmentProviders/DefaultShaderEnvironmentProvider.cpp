#include "StdAfx.hpp"

#include "Render/ShaderEnvironmentProviders/DefaultShaderEnvironmentProvider.hpp"
#include "Render/Material.hpp"
#include "Render/Mesh.hpp"
#include "Render/GraphicsNode.hpp"

namespace box
{
	struct Constants
	{
		float transformMatrix[16];
	};

	DefaultShaderEnvironmentProvider::DefaultShaderEnvironmentProvider()
		: m_constants(nullptr)
	{
		m_defaultTransform.setIdentity();
	}

	DefaultShaderEnvironmentProvider::~DefaultShaderEnvironmentProvider()
	{
		SAVE_RELEASE(m_constants);
	}

	void DefaultShaderEnvironmentProvider::onDeviceLost()
	{
	}

	bool DefaultShaderEnvironmentProvider::restore(ID3D11Device* device)
	{
		D3D11_BUFFER_DESC desc;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.ByteWidth = sizeof(Constants);
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;

		device->CreateBuffer(&desc, nullptr, &m_constants);

		return true;
	}

	void DefaultShaderEnvironmentProvider::prepareShader(ID3D11DeviceContext* context, Shader& shader, const Material& material, const GraphicsNode& graphicsNode)
	{
		D3D11_MAPPED_SUBRESOURCE subResource;
		context->Map(m_constants, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);

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
		context->Unmap(m_constants, 0);

		context->VSSetConstantBuffers(1, 1, &m_constants);
	}

}
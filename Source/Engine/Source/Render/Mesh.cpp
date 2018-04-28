#include "StdAfx.hpp"
#include "Render/Mesh.hpp"
#include <DXUT11\Core\DXUT.h>
#include "System/ResourceSystem/ResourceManager.hpp"

namespace box
{

	Mesh::Mesh(Microsoft::WRL::ComPtr<ID3D11Buffer> vertex, Microsoft::WRL::ComPtr<ID3D11Buffer> index, U32 count, U32 vertSize, D3D_PRIMITIVE_TOPOLOGY topology)
		: m_vertexBuffer(vertex)
		, m_indexBuffer(index)
		, m_indexCount(count)
		, m_vertexSize(vertSize)
		, m_topology(topology)
	{
	}

	Mesh::~Mesh()
	{
	}

	void Mesh::render(F32 delta)
	{
		ID3D11DeviceContext* context = DXUTGetD3D11DeviceContext();
		ID3D11Device* device = DXUTGetD3D11Device();

		if (!m_indexBuffer || !m_vertexBuffer || !m_indexCount)
		{
			return;
		}
		//if (!m_material->apply(context))
		//{
		//	return;
		//}
		unsigned int stride = m_vertexSize;
		unsigned int offset = 0;

		context->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		context->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);

		context->IASetPrimitiveTopology(m_topology);
		context->DrawIndexed(m_indexCount, 0, 0);
	}

	void Mesh::preRender()
	{
	}

	void Mesh::postRender()
	{
	}

	void Mesh::restore()
	{
	}

	void Mesh::deviceLost()
	{
	}

	int Mesh::getSizeForXML() const
	{
		return 0;
	}

	tinyxml2::XMLNode* Mesh::serializeToXML(tinyxml2::XMLNode* node, tinyxml2::XMLDocument& doc) const
	{
		tinyxml2::XMLElement* element = doc.NewElement("Mesh");
		element->SetAttribute("name", m_name.c_str());
		element->SetAttribute("materialName", m_material->getSrcFile().c_str());

		if (node)
		{
			node->InsertEndChild(element);
		}

		return element;
	}

	bool Mesh::loadFromXML(tinyxml2::XMLNode* node)
	{
		if (auto element = node->ToElement())
		{
			if (strcmp(element->Name(), "Mesh") == 0)
			{
				{ // hacky way to not render mesh
					bool del = false;
					element->QueryBoolAttribute("del", &del);
					if (del)
					{
						m_indexBuffer.Reset();
						m_vertexBuffer.Reset();
						m_indexCount = 0;
						return true;
					}
				}
				if (const char* name = element->Attribute("name"))
				{
					m_name = name;
				}
				const char* materialName = element->Attribute("materialName");

				if (materialName)
				{
					Resource r(materialName);
					auto handle = ResourceManager::Instance().getHandle(r);
					auto material = handle->getExtraTyped<Material>();
					if (material)
					{
						setMaterial(material);
						return true;
					}
				}
			}
		}
		return false;
	}

}
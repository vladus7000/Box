#include "StdAfx.hpp"
#include "Render/Mesh.hpp"
#include <DXUT11\Core\DXUT.h>

namespace box
{

	Mesh::Mesh(ID3D11Buffer* vertex, ID3D11Buffer* index, U32 count)
		: m_vertexBuffer(vertex)
		, m_indexBuffer(index)
		, m_indexCount(count)
	{

	}

	Mesh::~Mesh()
	{
		SAVE_RELEASE(m_vertexBuffer);
		SAVE_RELEASE(m_indexBuffer);
	}

	void Mesh::render(F32 delta)
	{
		ID3D11DeviceContext* context = DXUTGetD3D11DeviceContext();
		ID3D11Device* device = DXUTGetD3D11Device();

		if (!m_material || !m_indexBuffer || !m_vertexBuffer || !m_indexCount)
		{
			return;
		}
		if (!m_material->apply(context))
		{
			return;
		}

		struct SimpleVertexFormat
		{
			float pos[3];
			float tcoord[2];
			float norm[3];
		};
		unsigned int stride = sizeof(SimpleVertexFormat);
		unsigned int offset = 0;

		context->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R16_UINT, 0);
		context->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
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

}
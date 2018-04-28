#include "StdAfx.hpp"
#include "Render/GridNode.hpp"
#include "System/ResourceSystem/ResourceManager.hpp"
#include <DXUT11\Core\DXUT.h>

namespace box
{
	GridNode::GridNode()
	{
		const S32 linesCount = 50;

		Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
		D3D11_BUFFER_DESC vbDesc;
		vbDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vbDesc.ByteWidth = linesCount * 20 * sizeof(VFs::Pos);
		vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbDesc.CPUAccessFlags = 0;
		vbDesc.MiscFlags = 0;
		vbDesc.StructureByteStride = 0;

		std::vector<VFs::Pos> verts;
		std::vector<U32> indices;
		verts.reserve(linesCount * 2 * 2);
		indices.reserve(linesCount * 2 * 2);
		U32 index = 0;
		for (S32 x = 0; x <= linesCount ; x++)
		{
			VFs::Pos p;
			p.pos[0] = -linesCount / 2 + x;
			p.pos[1] = 0.0f;
			p.pos[2] = linesCount / 2;
			verts.push_back(p);
			indices.push_back(index++);
			p.pos[2] = -linesCount /2;
			verts.push_back(p);
			indices.push_back(index++);
		}
		for (S32 z = 0; z <= linesCount; z++)
		{
			VFs::Pos p;
			p.pos[0] = linesCount / 2;
			p.pos[1] = 0.0f;
			p.pos[2] = -linesCount / 2 + z;
			verts.push_back(p);
			indices.push_back(index++);
			p.pos[0] = -linesCount / 2;
			verts.push_back(p);
			indices.push_back(index++);
		}
		
		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = verts.data();

		ID3D11DeviceContext* context = DXUTGetD3D11DeviceContext();
		ID3D11Device* device = DXUTGetD3D11Device();

		device->CreateBuffer(&vbDesc, &initData, vertexBuffer.GetAddressOf());

		D3D11_BUFFER_DESC ibDesc;
		ibDesc.Usage = D3D11_USAGE_IMMUTABLE;
		ibDesc.ByteWidth = indices.size() * sizeof(U32);
		ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibDesc.CPUAccessFlags = 0;
		ibDesc.MiscFlags = 0;
		ibDesc.StructureByteStride = 0;

		initData.pSysMem = indices.data();

		device->CreateBuffer(&ibDesc, &initData, indexBuffer.GetAddressOf());

		m_gridMesh = std::make_shared<Mesh>(vertexBuffer, indexBuffer, indices.size(), sizeof(VFs::Pos), D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

		Resource r("desc/line.material");
		auto handle = ResourceManager::Instance().getHandle(r);
		auto material = handle->getExtraTyped<Material>();
		if (material)
		{
			m_gridMesh->setMaterial(material);
		}
	}

	void GridNode::render(F32 delta)
	{

	}

	void GridNode::gatherCurrentNodeGraphicsObjects(RenderObjects& out)
	{
		out.m_editorObjects.push_back(this);
	}
}
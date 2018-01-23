#pragma once

#include "System/ResourceSystem/ResourceExtraData.hpp"
#include <D3D11.h>

namespace box
{
	class ShaderResourceExtraData : public ResourceExtraData
	{
	public:
		ShaderResourceExtraData();
		~ShaderResourceExtraData();

		virtual const std::string& getName() const { return "ShaderResourceExtraData"; }

	public:
		friend class ShaderTextureResourceLoader;
		ID3D11BlendState* m_blendState;
		ID3D11HullShader *m_hullShader;
		ID3D11DomainShader* m_domainShader;
		ID3D11GeometryShader* m_geometryShader;
		ID3D11ComputeShader* m_computeShader;
		ID3D11PixelShader* m_pixelShader;
		ID3D11VertexShader* m_vertexShader;
		ID3D11InputLayout* m_inputLayout;
	};
}
#pragma once

#include "System/ResourceSystem/ResourceExtraData.hpp"
#include <D3D11.h>

#include <vector>
#include <string>

namespace box
{
	class ShaderResourceExtraData : public ResourceExtraData
	{
	public:
		ShaderResourceExtraData() = default;
		~ShaderResourceExtraData() = default;

		virtual const std::string& getName() const { return "ShaderResourceExtraData"; }

	public:
		friend class ShaderTextureResourceLoader;
		struct Technique
		{
			Technique()
				: blendState(nullptr)
				, depthStencilView(nullptr)
				, rasterizerState(nullptr)
				, inputLayout(nullptr)
				, hullShader(nullptr)
				, domainShader(nullptr)
				, geometryShader(nullptr)
				, computeShader(nullptr)
				, pixelShader(nullptr)
				, vertexShader(nullptr)
			{}
			~Technique()
			{
				RELEASE(blendState);
				RELEASE(depthStencilView);
				RELEASE(rasterizerState);
				RELEASE(inputLayout);
				RELEASE(hullShader);
				RELEASE(domainShader);
				RELEASE(geometryShader);
				RELEASE(computeShader);
				RELEASE(pixelShader);
				RELEASE(vertexShader);
			}
			ID3D11BlendState* blendState;
			ID3D11DepthStencilState* depthStencilView;
			ID3D11RasterizerState* rasterizerState;
			ID3D11InputLayout* inputLayout;

			ID3D11HullShader* hullShader;
			ID3D11DomainShader* domainShader;
			ID3D11GeometryShader* geometryShader;
			ID3D11ComputeShader* computeShader;
			ID3D11PixelShader* pixelShader;
			ID3D11VertexShader* vertexShader;
			std::string name;
		};
		std::vector<Technique> m_techniques;
	};
}
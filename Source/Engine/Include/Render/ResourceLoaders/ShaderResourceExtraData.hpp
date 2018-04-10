#pragma once

#include "System/ResourceSystem/ResourceExtraData.hpp"
#include <D3D11.h>

#include <vector>
#include <string>

namespace box
{
//	class ShaderResourceExtraData : public ResourceExtraData
//	{
//	public:
//		ShaderResourceExtraData() = default;
//		~ShaderResourceExtraData() = default;
//
//		virtual const std::string& getName() const { return "ShaderResourceExtraData"; }
//
//	public:
//		friend class ShaderTextureResourceLoader;
		/*class Technique
		{
			void releaseAll()
			{
				SAVE_RELEASE(blendState);
				SAVE_RELEASE(depthStencilView);
				SAVE_RELEASE(rasterizerState);
				SAVE_RELEASE(inputLayout);
				SAVE_RELEASE(hullShader);
				SAVE_RELEASE(domainShader);
				SAVE_RELEASE(geometryShader);
				SAVE_RELEASE(computeShader);
				SAVE_RELEASE(pixelShader);
				SAVE_RELEASE(vertexShader);
			}
		public:
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

			Technique(const Technique& rhs)
				: Technique()
			{
				*this = rhs;
			}
			Technique(Technique&& rhs)
				: Technique()
			{
				*this = rhs;
			}

			Technique& operator=(const Technique& rhs)
			{
				releaseAll();
				if (rhs.blendState)
				{
					blendState = rhs.blendState;
					blendState->AddRef();
				}

				if (rhs.depthStencilView)
				{
					depthStencilView = rhs.depthStencilView;
					depthStencilView->AddRef();
				}

				if (rhs.rasterizerState)
				{
					rasterizerState = rhs.rasterizerState;
					rasterizerState->AddRef();
				}

				if (rhs.inputLayout)
				{
					inputLayout = rhs.inputLayout;
					inputLayout->AddRef();
				}

				if (rhs.hullShader)
				{
					hullShader = rhs.hullShader;
					hullShader->AddRef();
				}

				if (rhs.domainShader)
				{
					domainShader = rhs.domainShader;
					domainShader->AddRef();
				}

				if (rhs.geometryShader)
				{
					geometryShader = rhs.geometryShader;
					geometryShader->AddRef();
				}

				if (rhs.computeShader)
				{
					computeShader = rhs.computeShader;
					computeShader->AddRef();
				}

				if (rhs.pixelShader)
				{
					pixelShader = rhs.pixelShader;
					pixelShader->AddRef();
				}

				if (rhs.vertexShader)
				{
					vertexShader = rhs.vertexShader;
					vertexShader->AddRef();
				}
				return *this;
			}
			Technique& operator=(Technique&& rhs)
			{
				if (rhs.blendState)
				{
					blendState = rhs.blendState;
					rhs.blendState = nullptr;
				}

				if (rhs.depthStencilView)
				{
					depthStencilView = rhs.depthStencilView;
					rhs.depthStencilView = nullptr;
				}

				if (rhs.rasterizerState)
				{
					rasterizerState = rhs.rasterizerState;
					rhs.rasterizerState = nullptr;
				}

				if (rhs.inputLayout)
				{
					inputLayout = rhs.inputLayout;
					rhs.inputLayout = nullptr;
				}

				if (rhs.hullShader)
				{
					hullShader = rhs.hullShader;
					rhs.hullShader = nullptr;
				}

				if (rhs.domainShader)
				{
					domainShader = rhs.domainShader;
					rhs.domainShader = nullptr;
				}

				if (rhs.geometryShader)
				{
					geometryShader = rhs.geometryShader;
					rhs.geometryShader = nullptr;
				}

				if (rhs.computeShader)
				{
					computeShader = rhs.computeShader;
					rhs.computeShader = nullptr;
				}

				if (rhs.pixelShader)
				{
					pixelShader = rhs.pixelShader;
					rhs.pixelShader = nullptr;
				}

				if (rhs.vertexShader)
				{
					vertexShader = rhs.vertexShader;
					rhs.vertexShader = nullptr;
				}
				return *this;
			}
			~Technique()
			{
				releaseAll();
			}

			bool apply(ID3D11DeviceContext* context)
			{
				if (blendState)
				{
					FLOAT blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
					context->OMSetBlendState(blendState, blendFactor, 0xFFFFFFFF);
				}
				if (depthStencilView)
				{
					UINT ref = 1;
					context->OMSetDepthStencilState(depthStencilView, ref);
				}
				if (rasterizerState)
				{
					context->RSSetState(rasterizerState);
				}
				if (inputLayout)
				{
					context->IASetInputLayout(inputLayout);
				}
				if (hullShader)
				{
					context->HSSetShader(hullShader, nullptr, 0);
				}
				if (domainShader)
				{
					context->DSSetShader(domainShader, nullptr, 0);
				}
				if (geometryShader)
				{
					context->GSSetShader(geometryShader, nullptr, 0);
				}
				if (computeShader)
				{
					context->CSSetShader(computeShader, nullptr, 0);
				}
				if (pixelShader)
				{
					context->PSSetShader(pixelShader, nullptr, 0);
				}
				if (vertexShader)
				{
					context->VSSetShader(vertexShader, nullptr, 0);
				}
				return true;
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
		};*/
		//std::vector<Technique> m_techniques;
	//};
}
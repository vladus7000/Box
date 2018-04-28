#pragma once

#include <D3D11.h>
#include <wrl.h>

#include <vector>
#include <string>

namespace box
{
	class Technique
	{
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

		bool apply(ID3D11DeviceContext* context)
		{
			if (blendState)
			{
				FLOAT blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
				context->OMSetBlendState(blendState.Get(), blendFactor, 0xFFFFFFFF);
			}
			if (depthStencilView)
			{
				UINT ref = 1;
				context->OMSetDepthStencilState(depthStencilView.Get(), ref);
			}
			if (rasterizerState)
			{
				context->RSSetState(rasterizerState.Get());
			}
			if (inputLayout)
			{
				context->IASetInputLayout(inputLayout.Get());
			}
			if (hullShader)
			{
				context->HSSetShader(hullShader.Get(), nullptr, 0);
			}
			if (domainShader)
			{
				context->DSSetShader(domainShader.Get(), nullptr, 0);
			}
			if (geometryShader)
			{
				context->GSSetShader(geometryShader.Get(), nullptr, 0);
			}
			if (computeShader)
			{
				context->CSSetShader(computeShader.Get(), nullptr, 0);
			}
			if (pixelShader)
			{
				context->PSSetShader(pixelShader.Get(), nullptr, 0);
			}
			if (vertexShader)
			{
				context->VSSetShader(vertexShader.Get(), nullptr, 0);
			}
			return true;
		}

		Microsoft::WRL::ComPtr<ID3D11BlendState> blendState;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilView;
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;

		Microsoft::WRL::ComPtr<ID3D11HullShader> hullShader;
		Microsoft::WRL::ComPtr<ID3D11DomainShader> domainShader;
		Microsoft::WRL::ComPtr<ID3D11GeometryShader> geometryShader;
		Microsoft::WRL::ComPtr<ID3D11ComputeShader> computeShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
		std::string name;
	};
}
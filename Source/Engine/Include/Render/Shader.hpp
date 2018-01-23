#pragma once

#include "System/ResourceSystem/ResourceHandle.hpp"

#include <D3D11.h>
#include <string>

namespace box
{
	class Shader
	{
	public:
		Shader(const std::string& resourceName);
		~Shader();

		void restore();
		void lostDevice();

		const std::string& getName() const { return m_name; }

		ID3D11BlendState* getBlendState() const { return m_blendState; }
		ID3D11HullShader *getHullShader() const { return m_hullShader; }
		ID3D11DomainShader* getDomainShader() const { return m_domainShader; }
		ID3D11GeometryShader* getGeometryShader() const { return m_geometryShader; }
		ID3D11ComputeShader* getComputeShader() const { return m_computeShader; }
		ID3D11PixelShader* getPixelShader() const { return m_pixelShader; }
		ID3D11VertexShader* getVertexShader() const { return m_vertexShader; }
		ID3D11InputLayout* getInputLayout() const { return m_inputLayout; }

	private:
		void deinit();

	private:
		std::string m_name;
		std::shared_ptr<ResourceHandle> m_resourceHandle;

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

#pragma once

#include "Technique.hpp"
#include "System/ResourceSystem/ResourceHandle.hpp"
#include <Render/ShaderEnvironmentProviders/ShaderEnvironmentProvider.hpp>
#include "System/ResourceSystem/ResourceExtraData.hpp"

#include <D3D11.h>
#include <string>
#include <vector>

namespace box
{
	class Shader : public ResourceExtraData
	{
	public:
		using ShaderStrongPtr = std::shared_ptr<Shader>;
		using ShaderWeakPtr = std::weak_ptr<Shader>;

	public:
		Shader(const std::string& resourceName);
		virtual ~Shader();

		void restore();
		void lostDevice();

		const std::string& getName() const { return m_name; }

		S32 getActiveTechniqueIndex() const { return m_activeTechnique; }
		U32 getNumTechniques() const { return static_cast<U32>(m_techniques.size()); }
		void setActiveTechnique(U32 tech) { ASSERT(tech < m_techniques.size() && m_activeTechnique != -1); m_activeTechnique = tech; }

		const Technique& getActiveTechnique() const { return m_techniques[m_activeTechnique]; }
		Technique& getActiveTechnique() { return m_techniques[m_activeTechnique]; }

		ShaderEnvironmentProvider::ShaderEnvironmentProviderStrong getEnvProvider() const { return m_envProvider; }
		void setProvider(ShaderEnvironmentProvider::ShaderEnvironmentProviderStrong provider) { m_envProvider = provider; }
		void updateEnvironment() { m_envProvider->prepareShader(*this); };

		//ID3D11BlendState* getBlendState() const { return m_techniques[m_activeTechnique].blendState; }
		//ID3D11DepthStencilView* getDepthStencilView() const { m_techniques[m_activeTechnique].depthStencilView; }
		//ID3D11RasterizerState* getRasterizerState() const { m_techniques[m_activeTechnique].rasterizerState; }
		//ID3D11InputLayout* getInputLayout() const { return m_techniques[m_activeTechnique].inputLayout; }
		//ID3D11HullShader *getHullShader() const { return m_techniques[m_activeTechnique].hullShader; }
		//ID3D11DomainShader* getDomainShader() const { return m_techniques[m_activeTechnique].domainShader; }
		//ID3D11GeometryShader* getGeometryShader() const { return m_techniques[m_activeTechnique].geometryShader; }
		//ID3D11ComputeShader* getComputeShader() const { return m_techniques[m_activeTechnique].computeShader; }
		//ID3D11PixelShader* getPixelShader() const { return m_techniques[m_activeTechnique].pixelShader; }
		//ID3D11VertexShader* getVertexShader() const { return m_techniques[m_activeTechnique].vertexShader; }

	private:
		friend class ShaderResourceLoader;
		void deinit();

	private:
		S32 m_activeTechnique;
		std::string m_name;
		std::shared_ptr<ResourceHandle> m_resourceHandle;
		std::vector<Technique> m_techniques;
		ShaderEnvironmentProvider::ShaderEnvironmentProviderStrong m_envProvider;
	};
}

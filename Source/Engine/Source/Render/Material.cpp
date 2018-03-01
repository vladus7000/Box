#include "StdAfx.hpp"
#include "Render/Material.hpp"

namespace box
{

	bool Material::apply(ID3D11DeviceContext* context)
	{
		m_shader->setActiveTechnique(0);
		ShaderResourceExtraData::Technique& technique =  m_shader->getActiveTechnique();
		return technique.apply(context);
	}

}

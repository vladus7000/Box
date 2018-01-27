#pragma once
#include "StdAfx.hpp"
#include "Render\ResourceLoaders\XmlHelper.hpp"

namespace box
{
	std::map<std::string, D3D11_COMPARISON_FUNC> getCmpFuncs()
	{
		std::map<std::string, D3D11_COMPARISON_FUNC> ret;
		ret["Never"] = D3D11_COMPARISON_NEVER;
		ret["Less"] = D3D11_COMPARISON_LESS;
		ret["Equal"] = D3D11_COMPARISON_EQUAL;
		ret["LessEqual"] = D3D11_COMPARISON_LESS_EQUAL;
		ret["Greater"] = D3D11_COMPARISON_GREATER;
		ret["NotEqual"] = D3D11_COMPARISON_NOT_EQUAL;
		ret["GrEqual"] = D3D11_COMPARISON_GREATER_EQUAL;
		ret["Always"] = D3D11_COMPARISON_ALWAYS;
		return ret;
	}

	std::map<std::string, D3D11_FILL_MODE> getFillMode()
	{
		std::map<std::string, D3D11_FILL_MODE> ret;
		ret["Solid"] = D3D11_FILL_SOLID;
		ret["Wireframe"] = D3D11_FILL_WIREFRAME;
		return ret;
	}
	std::map<std::string, D3D11_CULL_MODE> getCullModes()
	{
		std::map<std::string, D3D11_CULL_MODE> ret;
		ret["None"] = D3D11_CULL_NONE;
		ret["Front"] = D3D11_CULL_FRONT;
		ret["Back"] = D3D11_CULL_BACK;
		return ret;
	}
	std::map<std::string, D3D11_DEPTH_WRITE_MASK> getWriteMasks()
	{
		std::map<std::string, D3D11_DEPTH_WRITE_MASK> ret;
		ret["MaskAll"] = D3D11_DEPTH_WRITE_MASK_ALL;
		ret["MaskZero"] = D3D11_DEPTH_WRITE_MASK_ZERO;

		return ret;
	}

	std::map<std::string, D3D11_STENCIL_OP> getStencilOp()
	{
		std::map<std::string, D3D11_STENCIL_OP> ret;
		ret["Keep"] = D3D11_STENCIL_OP_KEEP;
		ret["Zero"] = D3D11_STENCIL_OP_ZERO;
		ret["Replace"] = D3D11_STENCIL_OP_REPLACE;
		ret["IncrSat"] = D3D11_STENCIL_OP_INCR_SAT;
		ret["DecrSat"] = D3D11_STENCIL_OP_DECR_SAT;
		ret["Invert"] = D3D11_STENCIL_OP_INVERT;
		ret["Incr"] = D3D11_STENCIL_OP_INCR;
		ret["Decr"] = D3D11_STENCIL_OP_DECR;

		return ret;
	}

	std::map<std::string, D3D11_BLEND> getBlend()
	{
		std::map<std::string, D3D11_BLEND> ret;

		ret["Zero"] = D3D11_BLEND_ZERO;
		ret["One"] = D3D11_BLEND_ONE;
		ret["SrcColor"] = D3D11_BLEND_SRC_COLOR;
		ret["InvSrcColor"] = D3D11_BLEND_INV_SRC_COLOR;
		ret["SrcAlpha"] = D3D11_BLEND_SRC_ALPHA;
		ret["InvSrcAlpha"] = D3D11_BLEND_INV_SRC_ALPHA;
		ret["DstAlpha"] = D3D11_BLEND_DEST_ALPHA;
		ret["InvDstAlpha"] = D3D11_BLEND_INV_DEST_ALPHA;
		ret["DstColor"] = D3D11_BLEND_DEST_COLOR;
		ret["InvDstColor"] = D3D11_BLEND_INV_DEST_COLOR;
		ret["SrcAlphaSat"] = D3D11_BLEND_SRC_ALPHA_SAT;
		ret["BlendFactor"] = D3D11_BLEND_BLEND_FACTOR;
		ret["InvBlendFactor"] = D3D11_BLEND_INV_BLEND_FACTOR;
		ret["Src1Color"] = D3D11_BLEND_SRC1_COLOR;
		ret["InvSrc1Color"] = D3D11_BLEND_INV_SRC1_COLOR;
		ret["Src1Alpha"] = D3D11_BLEND_SRC1_ALPHA;
		ret["InvSrc1Alpha"] = D3D11_BLEND_INV_SRC1_ALPHA;
		return ret;
	}

	std::map<std::string, D3D11_BLEND_OP> getBlendOp()
	{
		std::map<std::string, D3D11_BLEND_OP> ret;
		ret["Add"] = D3D11_BLEND_OP_ADD;
		ret["Substract"] = D3D11_BLEND_OP_SUBTRACT;
		ret["RevSubstract"] = D3D11_BLEND_OP_REV_SUBTRACT;
		ret["Min"] = D3D11_BLEND_OP_MIN;
		ret["Max"] = D3D11_BLEND_OP_MAX;
		return ret;
	}

	std::map<std::string, UINT8> getColorWriteMask()
	{
		std::map<std::string, UINT8> ret;
		ret["Red"] = D3D11_COLOR_WRITE_ENABLE_RED;
		ret["Green"] = D3D11_COLOR_WRITE_ENABLE_GREEN;
		ret["Blue"] = D3D11_COLOR_WRITE_ENABLE_BLUE;
		ret["Alpha"] = D3D11_COLOR_WRITE_ENABLE_ALPHA;
		ret["All"] = D3D11_COLOR_WRITE_ENABLE_ALL;
		return ret;
	}

	std::map<std::string, D3D11_FILTER> getFilters()
	{
		std::map<std::string, D3D11_FILTER> ret;
		ret["MinMagMipPoint"] = D3D11_FILTER_MIN_MAG_MIP_POINT;
		ret["MinMagPoingMipLinear"] = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
		ret["MinPointMapLinearMipPoint"] = D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;
		ret["MinPointMagMipLinear"] = D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;
		ret["MinLinearMagMipPoint"] = D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;
		ret["MinLinearMagPointMipLinear"] = D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
		ret["MinMagLinearMipPoint"] = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
		ret["MinMagMipLinear"] = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		ret["Anisotropic"] = D3D11_FILTER_ANISOTROPIC;

		/*
		TODO: write others
		D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT	= 0x80,
		D3D11_FILTER_COMPARISON_MIN_MAG_POINT_MIP_LINEAR	= 0x81,
		D3D11_FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT	= 0x84,
		D3D11_FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR	= 0x85,
		D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT	= 0x90,
		D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR	= 0x91,
		D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT	= 0x94,
		D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR	= 0x95,
		D3D11_FILTER_COMPARISON_ANISOTROPIC	= 0xd5
		*/

		return ret;
	}

	std::map<std::string, D3D11_TEXTURE_ADDRESS_MODE> getAddressModes()
	{
		std::map<std::string, D3D11_TEXTURE_ADDRESS_MODE> ret;
		ret["Wrap"] = D3D11_TEXTURE_ADDRESS_WRAP;
		ret["Mirror"] = D3D11_TEXTURE_ADDRESS_MIRROR;
		ret["Clamp"] = D3D11_TEXTURE_ADDRESS_CLAMP;
		ret["Border"] = D3D11_TEXTURE_ADDRESS_BORDER;
		ret["MirrorOnce"] = D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;

		return ret;
	}

	std::map<std::string, D3D11_INPUT_CLASSIFICATION> getInputClassifications()
	{
		std::map<std::string, D3D11_INPUT_CLASSIFICATION> ret;
		ret["PerVertexData"] = D3D11_INPUT_PER_VERTEX_DATA;
		ret["PerInstanceData"] = D3D11_INPUT_PER_INSTANCE_DATA;

		return ret;
	}

	std::map<std::string, DXGI_FORMAT> getDxgiFormats()
	{
		std::map<std::string, DXGI_FORMAT> ret;
		ret["Unknown"] = DXGI_FORMAT_UNKNOWN;

		/*
		TODO: write others
		DXGI_FORMAT_R32G32B32A32_TYPELESS       = 1,
		DXGI_FORMAT_R32G32B32A32_FLOAT          = 2,
		DXGI_FORMAT_R32G32B32A32_UINT           = 3,
		DXGI_FORMAT_R32G32B32A32_SINT           = 4,
		DXGI_FORMAT_R32G32B32_TYPELESS          = 5,
		DXGI_FORMAT_R32G32B32_FLOAT             = 6,
		DXGI_FORMAT_R32G32B32_UINT              = 7,
		DXGI_FORMAT_R32G32B32_SINT              = 8,
		DXGI_FORMAT_R16G16B16A16_TYPELESS       = 9,
		DXGI_FORMAT_R16G16B16A16_FLOAT          = 10,
		DXGI_FORMAT_R16G16B16A16_UNORM          = 11,
		DXGI_FORMAT_R16G16B16A16_UINT           = 12,
		DXGI_FORMAT_R16G16B16A16_SNORM          = 13,
		DXGI_FORMAT_R16G16B16A16_SINT           = 14,
		DXGI_FORMAT_R32G32_TYPELESS             = 15,
		DXGI_FORMAT_R32G32_FLOAT                = 16,
		DXGI_FORMAT_R32G32_UINT                 = 17,
		DXGI_FORMAT_R32G32_SINT                 = 18,
		DXGI_FORMAT_R32G8X24_TYPELESS           = 19,
		DXGI_FORMAT_D32_FLOAT_S8X24_UINT        = 20,
		DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS    = 21,
		DXGI_FORMAT_X32_TYPELESS_G8X24_UINT     = 22,
		DXGI_FORMAT_R10G10B10A2_TYPELESS        = 23,
		DXGI_FORMAT_R10G10B10A2_UNORM           = 24,
		DXGI_FORMAT_R10G10B10A2_UINT            = 25,
		DXGI_FORMAT_R11G11B10_FLOAT             = 26,
		DXGI_FORMAT_R8G8B8A8_TYPELESS           = 27,
		DXGI_FORMAT_R8G8B8A8_UNORM              = 28,
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB         = 29,
		DXGI_FORMAT_R8G8B8A8_UINT               = 30,
		DXGI_FORMAT_R8G8B8A8_SNORM              = 31,
		DXGI_FORMAT_R8G8B8A8_SINT               = 32,
		DXGI_FORMAT_R16G16_TYPELESS             = 33,
		DXGI_FORMAT_R16G16_FLOAT                = 34,
		DXGI_FORMAT_R16G16_UNORM                = 35,
		DXGI_FORMAT_R16G16_UINT                 = 36,
		DXGI_FORMAT_R16G16_SNORM                = 37,
		DXGI_FORMAT_R16G16_SINT                 = 38,
		DXGI_FORMAT_R32_TYPELESS                = 39,
		DXGI_FORMAT_D32_FLOAT                   = 40,
		DXGI_FORMAT_R32_FLOAT                   = 41,
		DXGI_FORMAT_R32_UINT                    = 42,
		DXGI_FORMAT_R32_SINT                    = 43,
		DXGI_FORMAT_R24G8_TYPELESS              = 44,
		DXGI_FORMAT_D24_UNORM_S8_UINT           = 45,
		DXGI_FORMAT_R24_UNORM_X8_TYPELESS       = 46,
		DXGI_FORMAT_X24_TYPELESS_G8_UINT        = 47,
		DXGI_FORMAT_R8G8_TYPELESS               = 48,
		DXGI_FORMAT_R8G8_UNORM                  = 49,
		DXGI_FORMAT_R8G8_UINT                   = 50,
		DXGI_FORMAT_R8G8_SNORM                  = 51,
		DXGI_FORMAT_R8G8_SINT                   = 52,
		DXGI_FORMAT_R16_TYPELESS                = 53,
		DXGI_FORMAT_R16_FLOAT                   = 54,
		DXGI_FORMAT_D16_UNORM                   = 55,
		DXGI_FORMAT_R16_UNORM                   = 56,
		DXGI_FORMAT_R16_UINT                    = 57,
		DXGI_FORMAT_R16_SNORM                   = 58,
		DXGI_FORMAT_R16_SINT                    = 59,
		DXGI_FORMAT_R8_TYPELESS                 = 60,
		DXGI_FORMAT_R8_UNORM                    = 61,
		DXGI_FORMAT_R8_UINT                     = 62,
		DXGI_FORMAT_R8_SNORM                    = 63,
		DXGI_FORMAT_R8_SINT                     = 64,
		DXGI_FORMAT_A8_UNORM                    = 65,
		DXGI_FORMAT_R1_UNORM                    = 66,
		DXGI_FORMAT_R9G9B9E5_SHAREDEXP          = 67,
		DXGI_FORMAT_R8G8_B8G8_UNORM             = 68,
		DXGI_FORMAT_G8R8_G8B8_UNORM             = 69,
		DXGI_FORMAT_BC1_TYPELESS                = 70,
		DXGI_FORMAT_BC1_UNORM                   = 71,
		DXGI_FORMAT_BC1_UNORM_SRGB              = 72,
		DXGI_FORMAT_BC2_TYPELESS                = 73,
		DXGI_FORMAT_BC2_UNORM                   = 74,
		DXGI_FORMAT_BC2_UNORM_SRGB              = 75,
		DXGI_FORMAT_BC3_TYPELESS                = 76,
		DXGI_FORMAT_BC3_UNORM                   = 77,
		DXGI_FORMAT_BC3_UNORM_SRGB              = 78,
		DXGI_FORMAT_BC4_TYPELESS                = 79,
		DXGI_FORMAT_BC4_UNORM                   = 80,
		DXGI_FORMAT_BC4_SNORM                   = 81,
		DXGI_FORMAT_BC5_TYPELESS                = 82,
		DXGI_FORMAT_BC5_UNORM                   = 83,
		DXGI_FORMAT_BC5_SNORM                   = 84,
		DXGI_FORMAT_B5G6R5_UNORM                = 85,
		DXGI_FORMAT_B5G5R5A1_UNORM              = 86,
		DXGI_FORMAT_B8G8R8A8_UNORM              = 87,
		DXGI_FORMAT_B8G8R8X8_UNORM              = 88,
		DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM  = 89,
		DXGI_FORMAT_B8G8R8A8_TYPELESS           = 90,
		DXGI_FORMAT_B8G8R8A8_UNORM_SRGB         = 91,
		DXGI_FORMAT_B8G8R8X8_TYPELESS           = 92,
		DXGI_FORMAT_B8G8R8X8_UNORM_SRGB         = 93,
		DXGI_FORMAT_BC6H_TYPELESS               = 94,
		DXGI_FORMAT_BC6H_UF16                   = 95,
		DXGI_FORMAT_BC6H_SF16                   = 96,
		DXGI_FORMAT_BC7_TYPELESS                = 97,
		DXGI_FORMAT_BC7_UNORM                   = 98,
		DXGI_FORMAT_BC7_UNORM_SRGB              = 99,
		DXGI_FORMAT_FORCE_UINT                  = 0xffffffff
		*/

		return ret;
	}

	std::map<std::string, D3D11_COMPARISON_FUNC> g_cmpFuncs = getCmpFuncs();
	std::map<std::string, D3D11_FILL_MODE> g_fillModes = getFillMode();
	std::map<std::string, D3D11_CULL_MODE> g_cullModes = getCullModes();
	std::map<std::string, D3D11_DEPTH_WRITE_MASK> g_WriteMasks = getWriteMasks();
	std::map<std::string, D3D11_STENCIL_OP> g_stencilOps = getStencilOp();

	std::map<std::string, D3D11_BLEND> g_Blends = getBlend();
	std::map<std::string, D3D11_BLEND_OP> g_BlendOps = getBlendOp();
	std::map<std::string, UINT8> g_colorWriteMasks = getColorWriteMask();

	std::map<std::string, D3D11_FILTER> g_Filters = getFilters();
	std::map<std::string, D3D11_TEXTURE_ADDRESS_MODE> g_addressModes = getAddressModes();

	std::map<std::string, D3D11_INPUT_CLASSIFICATION> g_inputClassifications = getInputClassifications();
	extern std::map<std::string, DXGI_FORMAT> g_dxgiFormats = getDxgiFormats();
}
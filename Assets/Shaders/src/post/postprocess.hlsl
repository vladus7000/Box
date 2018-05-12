cbuffer ShaderData : register(b0)
{
	float4x4 cameraViewMatrix;
	float4x4 cameraProjectionMatrix;
	float4 cameraPosition;
	float4 cameraTarget;
	float4 screenW_screenH_InvGamma_0;
	float3 sunDirection;
	float3 sunColor;
};


struct VSInput_ID
{
	uint vertexId : SV_VertexID;
};

struct PixelShaderInput
{
	float4 pos : SV_POSITION;
	float2 texCoord : TEXCOORD0;
};

Texture2D ldrTexture : register( t0 ); 
SamplerState TexSampler : register( s0 );

static float2 quad[4] = { float2(-1.0, 1.0),  float2(-1.0, -1.0), float2(1.0, 1.0),float2(1.0, -1.0) };
static float2 tcoord[4] = { float2(0.0, 0.0),  float2(0.0, 1.0), float2(1.0, 0.0),float2(1.0, 1.0) };

PixelShaderInput t1_VsMain(VSInput_ID input)
{
	PixelShaderInput output;

	output.pos = float4(quad[input.vertexId], 0.0, 1.0);
	output.texCoord = tcoord[input.vertexId];
	return output;
}

float4 t1_psMain(PixelShaderInput input) : SV_TARGET
{
	float3 outColor = pow(ldrTexture.Sample( TexSampler, input.texCoord ).rgb, screenW_screenH_InvGamma_0[2]);
	return float4(outColor, 1.0);
}
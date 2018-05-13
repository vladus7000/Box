cbuffer ShaderData : register(b0)
{
	float4x4 cameraViewMatrix;
	float4x4 cameraProjectionMatrix;
	float4 cameraPosition;
	float4 cameraTarget;
	float4 screenW_screenH_0_0;
	float3 sunDirection;
	float3 sunColor;
};

TextureCube	g_EnvironmentTexture : register( t0 );
SamplerState g_sam : register( s0 );

struct VertexShaderInput
{
	float4 pos : POSITION;
};

struct PixelShaderInput
{
	float4 pos : SV_POSITION;
	float3 texCoord : TEXCOORD0;
};

PixelShaderInput t1_VsMain(VertexShaderInput input)
{
	PixelShaderInput output;
	float4x4 m = mul(cameraProjectionMatrix, cameraViewMatrix);
	
    output.pos = input.pos;
    output.texCoord = normalize( mul(m, input.pos.xyz) );
    
	return output;
}

float4 t1_psMain(PixelShaderInput input) : SV_TARGET
{
    float4 color = g_EnvironmentTexture.Sample( g_sam, input.texCoord );
    return color;
}
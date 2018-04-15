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

struct VertexShaderInput
{
	float3 pos : POSITION;
};


struct PixelShaderInput
{
	float4 pos : SV_POSITION;
};

PixelShaderInput t1_VsMain(VertexShaderInput input)
{
	PixelShaderInput output;

	float4x4 m = mul(cameraProjectionMatrix, cameraViewMatrix);
	
	output.pos = mul(m, float4(input.pos, 1.0f));

	return output;
}

float4 t1_psMain(PixelShaderInput input) : SV_TARGET
{
	return float4(0.3, 0.3, 0.3, 1.0);
}
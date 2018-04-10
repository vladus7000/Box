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
	float2 texCoord : TEXCOORD0;
	float3 normal : NORMAL;
};


struct PixelShaderInput
{
	float4 pos : SV_POSITION;
	float2 texCoord : TEXCOORD0;
	float3 normal : NORMAL0;
	float3 posPixel : POSITION;
};

PixelShaderInput t1_VsMain(VertexShaderInput input)
{
	PixelShaderInput output;

	float4x4 m = mul(cameraProjectionMatrix, cameraViewMatrix);
	
	output.pos = mul(m, float4(input.pos, 1.0f));

	output.texCoord = input.texCoord;
	output.normal = input.normal;
	output.posPixel = output.pos;

	return output;
}

float4 t1_psMain(PixelShaderInput input) : SV_TARGET
{
	float3 normal = normalize(input.normal);
	float3 posPixel	= input.posPixel;
	float3 intensity = float3(0.3, 0.3, 0.3) * sunColor * saturate(dot(normal, normalize(sunDirection))) + float3(0.3, 0.3, 0.3);
	return float4(intensity, 1.0);
}
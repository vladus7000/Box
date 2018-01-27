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

	output.pos = float4(input.pos, 1.0f);

	output.texCoord = input.texCoord;
	output.normal = input.normal;
	output.posPixel = input.pos;

	return output;
}

float4 t1_psMain(PixelShaderInput input) : SV_TARGET
{
	return float4(1.0, 0.0, 0.0, 1.0);
}
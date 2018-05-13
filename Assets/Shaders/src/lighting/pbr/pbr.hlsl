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

cbuffer ModelData : register(b1)
{
	float4x4 worldMatrix;
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

TextureCube	g_EnvironmentTexture : register( t0 );
SamplerState g_sam : register( s0 );

Texture2D metalness : register( t1 ); 
SamplerState metalnessSampler : register( s1 );

Texture2D baseColor : register( t2 ); 
SamplerState baseColorSampler : register( s2 );

Texture2D normal : register( t3 ); 
SamplerState normalSampler : register( s3 );

Texture2D roughness : register( t4 ); 
SamplerState roughnessSampler : register( s4 );

PixelShaderInput t1_VsMain(VertexShaderInput input)
{
	PixelShaderInput output;

	float4x4 m = mul(cameraProjectionMatrix, cameraViewMatrix);
	m = mul(m, worldMatrix);
	
	output.pos = mul(m, float4(input.pos, 1.0f));

	output.texCoord = input.texCoord;
	output.normal = input.normal;
	output.posPixel = output.pos.xyz;

	return output;
}

float4 t1_psMain(PixelShaderInput input) : SV_TARGET
{
	float3 ao = 1.0;
	float3 outColor = 0.0;
	
	float3 normal = normalize(input.normal);
	float3 posPixel	= input.posPixel;
	float3 albedo = baseColor.Sample( baseColorSampler, input.texCoord ).rgb;
	float3 ambientColor = g_EnvironmentTexture.Sample(g_sam, normal).rgb;
	
	outColor += albedo * ao * ambientColor;
	
	return float4(outColor, 1.0);
}
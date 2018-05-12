struct VSInput_ID
{
	uint vertexId : SV_VertexID;
};

struct PixelShaderInput
{
	float4 pos : SV_POSITION;
	float2 texCoord : TEXCOORD0;
};

Texture2D hdrTexture : register( t0 ); 
SamplerState TexSampler : register( s0 );
Texture2D lumTex : register( t1 ); 
SamplerState TexSampler2 : register( s1 );

static float2 quad[4] = { float2(-1.0, 1.0),  float2(-1.0, -1.0), float2(1.0, 1.0),float2(1.0, -1.0) };
static float2 tcoord[4] = { float2(0.0, 0.0),  float2(0.0, 1.0), float2(1.0, 0.0),float2(1.0, 1.0) };

static const float4 LUM_VECTOR = float4(.299, .587, .114, 0);
static const float  MIDDLE_GRAY = 0.72f;
static const float  LUM_WHITE = 1.5f;
static const float  BRIGHT_THRESHOLD = 0.5f;

PixelShaderInput t1_VsMain(VSInput_ID input)
{
	PixelShaderInput output;

	output.pos = float4(quad[input.vertexId], 0.0, 1.0);
	output.texCoord = tcoord[input.vertexId];
	return output;
}

float4 PS_avgLum(PixelShaderInput input) : SV_TARGET
{
	float4 vColor = 0.0f;
    float  fAvg = 0.0f;

    for( int y = -1; y < 1; y++ )
    {
        for( int x = -1; x < 1; x++ )
        {
            // Compute the sum of color values
            vColor = hdrTexture.Sample( TexSampler, input.texCoord, int2(x,y) );                       
                
            fAvg += dot( vColor, LUM_VECTOR );
        }
    }
    
    fAvg /= 4;
    
    return float4(fAvg, fAvg, fAvg, 1.0f);
}

float4 PS_downsample(PixelShaderInput input) : SV_TARGET
{
	float fAvg = 0.0f; 
    float4 vColor;
    
    for( int y = -1; y < 1; y++ )
    {
        for( int x = -1; x < 1; x++ )
        {
            // Compute the sum of color values
            vColor = hdrTexture.Sample( TexSampler, input.texCoord, int2(x,y) );
                        
            fAvg += vColor.r; 
        }
    }
    
    // Divide the sum to complete the average
    fAvg /= 4;
    
    return float4(fAvg, fAvg, fAvg, 1.0f);
}

float4 PS_tonemap(PixelShaderInput input) : SV_TARGET
{
    float4 vColor = hdrTexture.Sample( TexSampler, input.texCoord );
    float4 vLum = lumTex.Sample( TexSampler, float2(0,0) );
    
    // Tone mapping
    vColor.rgb *= MIDDLE_GRAY / (vLum.r + 0.001f);
    vColor.rgb *= (1.0f + vColor/LUM_WHITE);
    vColor.rgb /= (1.0f + vColor);
    
    vColor.a = 1.0f;    
    
    return vColor;
	
}
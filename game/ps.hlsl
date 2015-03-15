Texture2D tex;
SamplerState samplerState;
cbuffer MaterialCBuffer : register(b0)
{
	int hasTextures;
	int type;
	float3 ambient, diffuse, specular, emissive;
	float alpha, shininess, reflectivity;
	int pad[3];
};
cbuffer LightCBuffer : register(b1)
{
	int lightType;
	float4 lightDiffuseCol, lightAmbientCol;
	float3 lightDiffuseDir;
};
struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float3 Normal : NORMAL;
	float2 UV : TEXCOORD0;
};
float4 PS(VS_OUTPUT input) : SV_TARGET
{
	if (hasTextures){
		return tex.Sample(samplerState, input.UV);
		// lighting
		float4 outColor = float4(1, 1, 1, 1);
			if (type == 0 || type == 1){
				float s = max(dot(lightDiffuseDir, input.Normal), 0);
				outColor.rgb = ambient * lightAmbientCol + s*(tex.Sample(samplerState, input.UV).rgb*lightDiffuseCol).rgb;
				outColor.a = alpha;
			}
		return outColor;
	}
	return float4(1, 1, 1, 1);
}



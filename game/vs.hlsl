 cbuffer CameraCBuffer : register(b0)
{
	float4x4 view, proj;
};
cbuffer ModelCBuffer : register(b1)
{
	float4x4 world;
};

cbuffer MaterialCBuffer : register(b2)
{
	int type;
	float3 ambient, diffuse, specular, emissive;
	float alpha, shininess, reflectivity;
};
cbuffer LightCBuffer : register(b3)
{
	int lightType;
	float4 lightDiffuseCol, lightAmbientCol;
	float3 lightDiffuseDir;
};

struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR;
};
VS_OUTPUT VS(float3 inPos : POSITION, float3 inNormal : NORMAL, float2 inUV : TEXCOORD) //do
{
	VS_OUTPUT output;
	output.Pos = mul(float4(inPos, 1.0f), world);
	output.Pos = mul(output.Pos, view);
	output.Pos = mul(output.Pos, proj);
	// lighting
	if (type == 0 || type == 1){
			float3 normalW = normalize(mul(float4(inNormal, 0), world).xyz);
			float s = max(dot(lightDiffuseDir, normalW), 0); // perhaps lightDiffuseDir needs to be WORLD - TRANSFORMED
		float4 ambientColor = float4(0.2, 0.2, 0.2, 1.0);
			output.Color.rgb = ambient * lightAmbientCol + s*(diffuse*lightDiffuseCol).rgb;
		output.Color.a = alpha;
	}
	else {
		float4 diffuseMaterial = float4(1, 1, 1, 1), diffuseLight = float4(1, 1, 1, 1);
			float3 lightVecW = -float3(0, 0, 1);
			float3 normalW = normalize(mul(float4(inNormal, 0), world).xyz);
			float s = max(dot(lightVecW, normalW), 0);
		float4 ambientMaterial = float4(1, 0.2, 0.2, 1.0), ambientColor = float4(1, 0.2, 0.2, 1.0);
			output.Color.rgb = ambientMaterial * ambientColor + s*(diffuseMaterial*diffuseLight).rgb;
		output.Color.a = 1;
	}
	return output;
}



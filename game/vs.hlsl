 cbuffer CameraCBuffer : register(b0)
{
	float4x4 view, proj;
};
cbuffer ModelCBuffer : register(b1)
{
	float4x4 world;
};
/*
cbuffer MaterialCBuffer : register(b2)
{
	float3 lightVecW;
};
*/
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
	float4 diffuseMaterial = float4(1, 1, 1, 1), diffuseLight = float4(1, 1, 1, 1);
	float3 lightVecW = -float3(0,0,1);
	float3 normalW = normalize(mul(float4(inNormal, 0), world).xyz);
	float s = max(dot(lightVecW, normalW), 0);
	output.Color.rgb = s*(diffuseMaterial*diffuseLight).rgb;
	output.Color.a = 1;
	return output;
}



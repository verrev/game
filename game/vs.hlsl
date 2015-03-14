cbuffer CameraCBuffer : register(b0)
{
	float4x4 view, proj;
};
cbuffer ModelCBuffer : register(b1)
{
	float4x4 world;
};
struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float3 Normal : NORMAL;
	float2 UV : TEXCOORD0;
};
VS_OUTPUT VS(float3 inPos : POSITION, float3 inNormal : NORMAL, float2 inUV : TEXCOORD) 
{
	VS_OUTPUT output;
	output.Pos = mul(float4(inPos, 1.0f), world);
	output.Pos = mul(output.Pos, view);
	output.Pos = mul(output.Pos, proj);
	output.UV = inUV;
	//output.Normal = inNormal;
	output.Normal = mul(inNormal, world); // WORLD TRANS!
	return output;
}



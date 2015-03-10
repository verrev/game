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
	float4 Color : COLOR;
};
VS_OUTPUT VS(float3 inPos : POSITION, float3 inNormal : NORMAL, float2 inUV : TEXCOORD) //do
{
	VS_OUTPUT output;
	output.Pos = mul(float4(inPos, 1.0f), world);
	output.Pos = mul(output.Pos, view);
	output.Pos = mul(output.Pos, proj);
	output.Color = float4(1, 0.5, 0.3, 1);
	return output;
}



cbuffer CameraCBuffer : register(b0)
{
	float4x4 view, proj;
};
cbuffer ModelCBuffer : register(b1)
{
	float4x4 world;
};
struct SKYMAP_VS_OUTPUT	//output structure for skymap vertex shader
{
	float4 Pos : SV_POSITION;
	float3 texCoord : TEXCOORD;
};
SKYMAP_VS_OUTPUT VS(float3 inPos : POSITION, float2 inTexCoord : TEXCOORD, float3 normal : NORMAL)
{
	SKYMAP_VS_OUTPUT output = (SKYMAP_VS_OUTPUT)0;
	output.Pos = mul(float4(inPos, 1.0f), world).xyww;
	output.Pos = mul(float4(inPos, 1.0f), view).xyww;
	output.Pos = mul(float4(inPos, 1.0f), proj).xyww;
	output.texCoord = inPos;
	return output;
}

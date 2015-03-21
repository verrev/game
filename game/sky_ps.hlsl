TextureCube SkyMap;
SamplerState samplerState;
struct SKYMAP_VS_OUTPUT	//output structure for skymap vertex shader
{
	float4 Pos : SV_POSITION;
	float3 texCoord : TEXCOORD;
};
float4 PS(SKYMAP_VS_OUTPUT input) : SV_Target
{
	return SkyMap.Sample(samplerState, input.texCoord);
}

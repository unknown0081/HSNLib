#include "FullScreenQuad.hlsli"
#include "FilterFunctions.hlsli"

#define POINT 0
#define LINEAR 1
#define ANISOTROPIC 2

SamplerState samplerStates[3] : register(s0);
Texture2D textureMaps[4] : register(t0);

float4 main(VS_OUT pin) : SV_TARGET
{
    float4 color = textureMaps[0].Sample(samplerStates[LINEAR], pin.texcoord);
    
    // RGB > HSV に変換
    color.rgb = RGB2HSV(color.rgb);
    
    // 色相調整
    color.r += hueShift;
    
    // 彩度調整
    color.g *= saturation;
    
    // 明度調整
    color.b *= brightness;
    
    // HSV > RGB に変換
    color.rgb = HSV2RGB(color.rgb);
    
    return color;
}
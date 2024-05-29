#include "object3d.hlsli"

struct Material {
    float32_t4 color;
};

ConstantBuffer<Material> gMaterial : register(b0);
struct PixelShaderOutput {
    float32_t4 color : SV_TARGET0;
};

float32_t4 textureCplor = gTexture.Sample(gSampler, input.texcoord);

Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);

PixelShaderOutput main(VertexShanderOutput input)
{
    PixelShaderOutput output;
    output.color = gMaterial.color * textureColor;
    
    return output;
}

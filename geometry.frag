#version 460

layout(set = 0, binding = 5) buffer IndexSurface
{
    uint indexSurface[];
};
layout(set = 0, binding = 6) uniform sampler2D textureSampler;
layout(push_constant, std430) uniform Window
{
    vec2 window;
};

layout(location = 0) in vec4 color;
layout(location = 1) in flat uint entityIndex;
layout(location = 2) in vec2 texturePosition;
layout(location = 0) out vec4 outputColor;

vec4 screenPosition = gl_FragCoord;

void main()
{
    uint x = uint(screenPosition.x);
    uint y = uint(screenPosition.y);

    uint width = uint(window.x);
    uint pixelIndex =  (y * width) + x;

    indexSurface[pixelIndex] = entityIndex;

    outputColor = texture(textureSampler, texturePosition);
}

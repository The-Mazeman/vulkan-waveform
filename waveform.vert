#version 460

layout(set = 0, binding = 1) buffer OutputSamples
{
    float outputSamples[];
};
layout(push_constant, std430) uniform Window
{
    uint samples;
    uint width;
};

layout(location = 0) out vec4 color;
uint vertexID = gl_VertexIndex;

void main()
{
    float x = float(vertexID) / width;

    float sampleHeight = outputSamples[vertexID];;
    gl_Position = vec4(x, sampleHeight, 1.0, 1.0);
    color = vec4(1.0, 0.0, 0.0, 1.0);
}

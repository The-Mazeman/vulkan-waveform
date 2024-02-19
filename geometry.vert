#version 460

struct Entity
{
    uint transformIndex;
    uint positionIndex;
    uint colorIndex;
    float depth;
};
layout(set = 0, binding = 0) buffer ActiveEntities
{
    uint activeEntities[];
};
layout(set = 0, binding = 1) buffer Entities
{
    Entity entities[];
};
layout(set = 0, binding = 2) buffer Transforms
{
    vec2 transforms[];
};
layout(set = 0, binding = 3) buffer Positions
{
    vec2 positions[];
};
layout(set = 0, binding = 4) buffer Colors
{
    vec4 colors[];
};

layout(push_constant, std430) uniform Window
{
    vec2 window;
};
layout(location = 0) in vec2 vertex;

layout(location = 0) out vec4 colorOutput;
layout(location = 1) out uint entityIndexOutput;
layout(location = 2) out vec2 texturePosition;

void main()
{
    vec2 outVertex = vertex;
    vec2 offset = vec2(1.0, 1.0);
    
    texturePosition = vertex + offset;
    texturePosition /= 2;


    uint instanceIndex = gl_InstanceIndex;
    uint entityIndex = activeEntities[instanceIndex];
    entityIndexOutput = entityIndex;

    Entity entity = entities[entityIndex]; 

    uint transformIndex = entity.transformIndex;
    vec2 transform = transforms[transformIndex];
    transform /= window;
    outVertex *= transform;

    uint positionIndex = entity.positionIndex;
    vec2 position = positions[positionIndex];

    float offsetX = position.x / (window.x / 2.0) - 1.0;
    float offsetY = position.y / (window.y / 2.0) - 1.0;

    outVertex.x += offsetX + transform.x;
    outVertex.y += offsetY + transform.y;
    
    float depth = entity.depth;
    gl_Position = vec4(outVertex, depth, 1.0);

    uint colorIndex = entity.colorIndex;
    vec4 color = colors[colorIndex];
    colorOutput = color;
}

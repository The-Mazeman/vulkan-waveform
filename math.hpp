#pragma once
#include "dataTypes.hpp"
#include "defines.hpp"
#define PI (float)3.14159265358979323846

struct vec2
{
    float cells[2];
};
struct vec4
{
    float cells[4];
};
struct mat2
{
    float cells[2][2];
};
struct mat4
{
    float cells[4][4];
};

void rotate(mat2* matrix, float radians);
void identity(mat2* matrix);
void scale(mat2* matrix, float width, float height);

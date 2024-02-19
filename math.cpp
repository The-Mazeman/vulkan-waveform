#include "include.hpp"
#include "math.hpp"

void multiply(mat2* a, mat2* b, mat2* c)
{
    c->cells[0][0] = (a->cells[0][0] * b->cells[0][0]) + (a->cells[0][1] * b->cells[1][0]);
    c->cells[0][1] = (a->cells[0][0] * b->cells[0][1]) + (a->cells[0][1] * b->cells[1][1]);
    c->cells[1][0] = (a->cells[1][0] * b->cells[0][0]) + (a->cells[1][1] * b->cells[1][0]);
    c->cells[1][1] = (a->cells[1][0] * b->cells[0][1]) + (a->cells[1][1] * b->cells[1][1]);
}
void rotate(mat2* matrix, float radians)
{
    mat2 rotateMatrix = {};
    rotateMatrix.cells[0][0] = cosf(radians);
    rotateMatrix.cells[0][1] = sinf(radians);
    rotateMatrix.cells[1][0] = -sinf(radians);
    rotateMatrix.cells[1][1] = cosf(radians);

    mat2 result = {};
    multiply(matrix, &rotateMatrix, &result);
    *matrix = result;
}
void identity(mat2* matrix)
{
    matrix->cells[0][0] = 1.0f;
    matrix->cells[1][1] = 1.0f;
}
void scale(mat2* matrix, float width, float height)
{
    mat2 scaleMatrix = {};
    scaleMatrix.cells[0][0] = width;
    scaleMatrix.cells[1][1] = height;
    
    mat2 result = {};
    multiply(matrix, &scaleMatrix, &result);
    *matrix = result;

}


#pragma once

#include "Ghurund.h"
#include <DirectXMath.h>

using namespace DirectX;

struct Float2 {
    float x, y;
};

struct Float3 {
    float x, y, z;
};

struct Matrix {
    float a00, a01, a02, a03;
    float a10, a11, a12, a13;
    float a20, a21, a22, a23;
    float a30, a31, a32, a33;
};

XMFLOAT2 convertFloat2(const Float2 &float2);
Float2 convertFloat2(const XMFLOAT2 &xmFloat2);

XMFLOAT3 convertFloat3(const Float3 &float3);
Float3 convertFloat3(const XMFLOAT3 &xmFloat3);

XMFLOAT4X4 convertMatrix(const Matrix &matrix);
Matrix convertMatrix(const XMFLOAT4X4 &xmFloat4x4);
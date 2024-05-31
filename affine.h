#pragma once
#include "Matrix4x4.h"
#include "Vector3.h"
#include "math.h"
#include <cassert>
#include <Windows.h>
#include <cstdint>
#include <string>
#include <format>

Matrix4x4 Multiply(const Matrix4x4& mt1, const Matrix4x4& mt2);

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

Matrix4x4 MakeScaleMatrix(const Vector3& scale);

Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

Matrix4x4 Inverse(const Matrix4x4& m);

Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);
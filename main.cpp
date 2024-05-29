#include <Windows.h>
#include <cstdint>
#include <string>
#include <format>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib") 

#include <dxgidebug.h>
#pragma comment(lib, "dxguid.lib")

#include <dxcapi.h>
#pragma comment(lib, "dxcompiler.lib")

#include "externals/imgui/imgui.h"
#include "externals/imgui/imgui_impl_dx12.h"
#include "externals/imgui/imgui_impl_win32.h"
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wPARAm, LPARAM lParam);

#include"externals/DirectXTex/DirectXTex.h"
#include"externals/DirectXTex/d3dx12.h"


struct Vector4 {
	float x, y, z, w;
};

struct Vector3 {
	float x, y, z;
};

struct Vector2 {
	float x, y;
};

struct Matrix4x4 {
	float m[4][4];
};

#pragma region

Matrix4x4 Add(const Matrix4x4& mt1, const Matrix4x4& mt2) {


	Matrix4x4 ans;

	ans.m[0][0] = mt1.m[0][0] + mt2.m[0][0];
	ans.m[0][1] = mt1.m[0][1] + mt2.m[0][1];
	ans.m[0][2] = mt1.m[0][2] + mt2.m[0][2];
	ans.m[0][3] = mt1.m[0][3] + mt2.m[0][3];

	ans.m[1][0] = mt1.m[1][0] + mt2.m[1][0];
	ans.m[1][1] = mt1.m[1][1] + mt2.m[1][1];
	ans.m[1][2] = mt1.m[1][2] + mt2.m[1][2];
	ans.m[1][3] = mt1.m[1][3] + mt2.m[1][3];

	ans.m[2][0] = mt1.m[2][0] + mt2.m[2][0];
	ans.m[2][1] = mt1.m[2][1] + mt2.m[2][1];
	ans.m[2][2] = mt1.m[2][2] + mt2.m[2][2];
	ans.m[2][3] = mt1.m[2][3] + mt2.m[2][3];

	ans.m[3][0] = mt1.m[3][0] + mt2.m[3][0];
	ans.m[3][1] = mt1.m[3][1] + mt2.m[3][1];
	ans.m[3][2] = mt1.m[3][2] + mt2.m[3][2];
	ans.m[3][3] = mt1.m[3][3] + mt2.m[3][3];

	return ans;

}

Matrix4x4 Subtract(const Matrix4x4& mt1, const Matrix4x4& mt2) {

	Matrix4x4 ans;

	ans.m[0][0] = mt1.m[0][0] - mt2.m[0][0];
	ans.m[0][1] = mt1.m[0][1] - mt2.m[0][1];
	ans.m[0][2] = mt1.m[0][2] - mt2.m[0][2];
	ans.m[0][3] = mt1.m[0][3] - mt2.m[0][3];

	ans.m[1][0] = mt1.m[1][0] - mt2.m[1][0];
	ans.m[1][1] = mt1.m[1][1] - mt2.m[1][1];
	ans.m[1][2] = mt1.m[1][2] - mt2.m[1][2];
	ans.m[1][3] = mt1.m[1][3] - mt2.m[1][3];

	ans.m[2][0] = mt1.m[2][0] - mt2.m[2][0];
	ans.m[2][1] = mt1.m[2][1] - mt2.m[2][1];
	ans.m[2][2] = mt1.m[2][2] - mt2.m[2][2];
	ans.m[2][3] = mt1.m[2][3] - mt2.m[2][3];

	ans.m[3][0] = mt1.m[3][0] - mt2.m[3][0];
	ans.m[3][1] = mt1.m[3][1] - mt2.m[3][1];
	ans.m[3][2] = mt1.m[3][2] - mt2.m[3][2];
	ans.m[3][3] = mt1.m[3][3] - mt2.m[3][3];

	return ans;
}

Matrix4x4 Multiply(const Matrix4x4& mt1, const Matrix4x4& mt2) {

	Matrix4x4 ans = {};
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {

				ans.m[i][j] += mt1.m[i][k] * mt2.m[k][j];
			}
		}
	}
	return ans;
}

Matrix4x4 Inverse(const Matrix4x4& mt1) {
	Matrix4x4 ans;
	float A = 0;
	A = mt1.m[0][0] * mt1.m[1][1] * mt1.m[2][2] * mt1.m[3][3] + mt1.m[0][0] * mt1.m[1][2] * mt1.m[2][3] * mt1.m[3][1] + mt1.m[0][0] * mt1.m[1][3] * mt1.m[2][1] * mt1.m[3][2] -
		mt1.m[0][0] * mt1.m[1][3] * mt1.m[2][2] * mt1.m[3][1] - mt1.m[0][0] * mt1.m[1][2] * mt1.m[2][1] * mt1.m[3][3] - mt1.m[0][0] * mt1.m[1][1] * mt1.m[2][3] * mt1.m[3][2] -
		mt1.m[0][1] * mt1.m[1][0] * mt1.m[2][2] * mt1.m[3][3] - mt1.m[0][2] * mt1.m[1][0] * mt1.m[2][3] * mt1.m[3][1] - mt1.m[0][3] * mt1.m[1][0] * mt1.m[2][1] * mt1.m[3][2] +
		mt1.m[0][3] * mt1.m[1][0] * mt1.m[2][2] * mt1.m[3][1] + mt1.m[0][2] * mt1.m[1][0] * mt1.m[2][1] * mt1.m[3][3] + mt1.m[0][1] * mt1.m[1][0] * mt1.m[2][3] * mt1.m[3][2] +
		mt1.m[0][1] * mt1.m[1][2] * mt1.m[2][0] * mt1.m[2][2] + mt1.m[0][2] * mt1.m[1][3] * mt1.m[2][0] * mt1.m[3][1] + mt1.m[0][3] * mt1.m[1][1] * mt1.m[2][0] * mt1.m[3][2] -
		mt1.m[0][3] * mt1.m[1][2] * mt1.m[2][0] * mt1.m[3][1] - mt1.m[0][2] * mt1.m[1][1] * mt1.m[2][0] * mt1.m[3][3] - mt1.m[0][1] * mt1.m[1][3] * mt1.m[2][0] * mt1.m[3][2] -
		mt1.m[0][1] * mt1.m[1][2] * mt1.m[2][3] * mt1.m[3][0] - mt1.m[0][2] * mt1.m[1][3] * mt1.m[2][1] * mt1.m[3][0] - mt1.m[0][3] * mt1.m[1][1] * mt1.m[2][2] * mt1.m[3][0] +
		mt1.m[0][3] * mt1.m[1][2] * mt1.m[2][0] * mt1.m[3][0] + mt1.m[0][2] * mt1.m[1][1] * mt1.m[2][3] * mt1.m[3][0] + mt1.m[0][1] * mt1.m[1][3] * mt1.m[2][2] * mt1.m[3][0];

	ans.m[0][0] = (mt1.m[1][1] * mt1.m[2][2] * mt1.m[3][3] + mt1.m[1][2] * mt1.m[2][3] * mt1.m[3][1] + mt1.m[1][3] * mt1.m[2][1] * mt1.m[3][2] - mt1.m[1][3] * mt1.m[2][2] * mt1.m[3][1] -
		mt1.m[1][2] * mt1.m[2][1] * mt1.m[3][3] - mt1.m[1][1] * mt1.m[2][3] * mt1.m[3][2]) /
		A;

	ans.m[0][1] = (-mt1.m[0][1] * mt1.m[2][2] * mt1.m[3][3] - mt1.m[0][2] * mt1.m[2][3] * mt1.m[3][1] - mt1.m[0][3] * mt1.m[2][1] * mt1.m[3][2] + mt1.m[0][3] * mt1.m[2][2] * mt1.m[3][1] +
		mt1.m[0][2] * mt1.m[2][1] * mt1.m[3][3] + mt1.m[0][1] * mt1.m[2][3] * mt1.m[3][2]) /
		A;

	ans.m[0][2] = (mt1.m[0][1] * mt1.m[1][2] * mt1.m[3][3] + mt1.m[0][2] * mt1.m[1][3] * mt1.m[3][1] + mt1.m[0][3] * mt1.m[1][1] * mt1.m[3][2] - mt1.m[0][3] * mt1.m[1][2] * mt1.m[3][1] -
		mt1.m[0][2] * mt1.m[1][1] * mt1.m[3][3] - mt1.m[0][1] * mt1.m[1][3] * mt1.m[3][2]) /
		A;

	ans.m[0][3] = (-mt1.m[0][1] * mt1.m[1][2] * mt1.m[2][3] - mt1.m[0][2] * mt1.m[1][3] * mt1.m[2][1] - mt1.m[0][3] * mt1.m[1][1] * mt1.m[2][2] + mt1.m[0][3] * mt1.m[1][2] * mt1.m[2][1] +
		mt1.m[0][2] * mt1.m[1][1] * mt1.m[2][3] + mt1.m[0][1] * mt1.m[1][3] * mt1.m[2][2]) /
		A;

	ans.m[1][0] = (-mt1.m[1][0] * mt1.m[2][2] * mt1.m[3][3] - mt1.m[1][2] * mt1.m[2][3] * mt1.m[3][0] - mt1.m[1][3] * mt1.m[2][0] * mt1.m[3][2] + mt1.m[1][3] * mt1.m[2][2] * mt1.m[3][0] +
		mt1.m[1][2] * mt1.m[2][0] * mt1.m[3][3] + mt1.m[1][0] * mt1.m[2][3] * mt1.m[3][2]) /
		A;

	ans.m[1][1] = (mt1.m[0][0] * mt1.m[2][2] * mt1.m[3][3] + mt1.m[0][2] * mt1.m[2][3] * mt1.m[3][0] + mt1.m[0][3] * mt1.m[2][0] * mt1.m[3][2] - mt1.m[0][3] * mt1.m[2][2] * mt1.m[3][0] -
		mt1.m[0][2] * mt1.m[2][0] * mt1.m[3][3] - mt1.m[0][0] * mt1.m[2][3] * mt1.m[3][2]) /
		A;

	ans.m[1][2] = (-mt1.m[0][0] * mt1.m[1][2] * mt1.m[3][3] - mt1.m[0][2] * mt1.m[1][3] * mt1.m[3][0] - mt1.m[0][3] * mt1.m[1][0] * mt1.m[3][2] + mt1.m[0][3] * mt1.m[1][2] * mt1.m[3][0] +
		mt1.m[0][2] * mt1.m[1][0] * mt1.m[3][3] + mt1.m[0][0] * mt1.m[1][3] * mt1.m[3][2]) /
		A;

	ans.m[1][3] = (mt1.m[0][0] * mt1.m[1][2] * mt1.m[2][3] + mt1.m[0][2] * mt1.m[1][3] * mt1.m[2][0] + mt1.m[0][3] * mt1.m[1][0] * mt1.m[2][2] - mt1.m[0][3] * mt1.m[1][2] * mt1.m[2][0] -
		mt1.m[0][2] * mt1.m[1][0] * mt1.m[2][3] - mt1.m[0][0] * mt1.m[1][3] * mt1.m[2][2]) /
		A;

	ans.m[2][0] = (mt1.m[1][0] * mt1.m[2][1] * mt1.m[3][3] + mt1.m[1][1] * mt1.m[2][3] * mt1.m[3][0] + mt1.m[1][3] * mt1.m[2][0] * mt1.m[3][1] - mt1.m[1][3] * mt1.m[2][1] * mt1.m[3][0] -
		mt1.m[1][1] * mt1.m[2][0] * mt1.m[3][3] - mt1.m[1][0] * mt1.m[2][3] * mt1.m[3][1]) /
		A;

	ans.m[2][1] = (-mt1.m[0][0] * mt1.m[2][1] * mt1.m[3][3] - mt1.m[0][1] * mt1.m[2][3] * mt1.m[3][0] - mt1.m[0][3] * mt1.m[2][0] * mt1.m[3][1] + mt1.m[0][3] * mt1.m[2][1] * mt1.m[3][0] +
		mt1.m[0][1] * mt1.m[2][0] * mt1.m[3][3] + mt1.m[0][0] * mt1.m[2][3] * mt1.m[3][1]) /
		A;

	ans.m[2][2] = (mt1.m[0][0] * mt1.m[1][1] * mt1.m[3][3] + mt1.m[0][1] * mt1.m[1][3] * mt1.m[3][0] + mt1.m[0][3] * mt1.m[1][0] * mt1.m[3][1] - mt1.m[0][3] * mt1.m[1][1] * mt1.m[3][0] -
		mt1.m[0][1] * mt1.m[1][0] * mt1.m[3][3] - mt1.m[0][0] * mt1.m[1][3] * mt1.m[3][1]) /
		A;

	ans.m[2][3] = (-mt1.m[0][0] * mt1.m[1][1] * mt1.m[2][3] - mt1.m[0][1] * mt1.m[1][3] * mt1.m[2][0] - mt1.m[0][3] * mt1.m[1][0] * mt1.m[2][1] + mt1.m[0][3] * mt1.m[1][1] * mt1.m[2][0] +
		mt1.m[0][1] * mt1.m[1][0] * mt1.m[2][3] + mt1.m[0][0] * mt1.m[1][3] * mt1.m[2][1]) /
		A;

	ans.m[3][0] = (-mt1.m[1][0] * mt1.m[2][1] * mt1.m[3][2] - mt1.m[1][1] * mt1.m[2][2] * mt1.m[3][0] - mt1.m[1][2] * mt1.m[2][0] * mt1.m[3][1] + mt1.m[1][2] * mt1.m[2][1] * mt1.m[3][0] +
		mt1.m[1][1] * mt1.m[2][0] * mt1.m[3][2] + mt1.m[1][0] * mt1.m[2][2] * mt1.m[3][1]) /
		A;

	ans.m[3][1] = (mt1.m[0][0] * mt1.m[2][1] * mt1.m[3][2] + mt1.m[0][1] * mt1.m[2][2] * mt1.m[3][0] + mt1.m[0][2] * mt1.m[2][0] * mt1.m[3][1] - mt1.m[0][2] * mt1.m[2][1] * mt1.m[3][0] -
		mt1.m[0][1] * mt1.m[2][0] * mt1.m[3][2] - mt1.m[0][0] * mt1.m[2][2] * mt1.m[3][1]) /
		A;

	ans.m[3][2] = (-mt1.m[0][0] * mt1.m[1][1] * mt1.m[3][2] - mt1.m[0][1] * mt1.m[1][2] * mt1.m[3][0] - mt1.m[0][2] * mt1.m[1][0] * mt1.m[3][1] + mt1.m[0][2] * mt1.m[1][1] * mt1.m[3][0] +
		mt1.m[0][1] * mt1.m[1][0] * mt1.m[3][2] + mt1.m[0][0] * mt1.m[1][2] * mt1.m[3][1]) /
		A;

	ans.m[3][3] = (mt1.m[0][0] * mt1.m[1][1] * mt1.m[2][2] + mt1.m[0][1] * mt1.m[1][2] * mt1.m[2][0] + mt1.m[0][2] * mt1.m[1][0] * mt1.m[2][1] - mt1.m[0][2] * mt1.m[1][1] * mt1.m[2][0] -
		mt1.m[0][1] * mt1.m[1][0] * mt1.m[2][2] - mt1.m[0][0] * mt1.m[1][2] * mt1.m[2][1]) /
		A;

	return ans;
}

Matrix4x4 Transpose(const Matrix4x4& mt1) {

	Matrix4x4 ans;

	ans.m[0][0] = mt1.m[0][0];
	ans.m[0][1] = mt1.m[1][0];
	ans.m[0][2] = mt1.m[2][0];
	ans.m[0][3] = mt1.m[3][0];

	ans.m[1][0] = mt1.m[0][1];
	ans.m[1][1] = mt1.m[1][1];
	ans.m[1][2] = mt1.m[2][1];
	ans.m[1][3] = mt1.m[3][1];

	ans.m[2][0] = mt1.m[0][2];
	ans.m[2][1] = mt1.m[1][2];
	ans.m[2][2] = mt1.m[2][2];
	ans.m[2][3] = mt1.m[3][2];

	ans.m[3][0] = mt1.m[0][3];
	ans.m[3][1] = mt1.m[1][3];
	ans.m[3][2] = mt1.m[2][3];
	ans.m[3][3] = mt1.m[3][3];

	return ans;
}

Matrix4x4 MekeIdentity4x4() {

	Matrix4x4 ans;

	ans.m[0][0] = 1;
	ans.m[0][1] = 0;
	ans.m[0][2] = 0;
	ans.m[0][3] = 0;

	ans.m[1][0] = 0;
	ans.m[1][1] = 1;
	ans.m[1][2] = 0;
	ans.m[1][3] = 0;

	ans.m[2][0] = 0;
	ans.m[2][1] = 0;
	ans.m[2][2] = 1;
	ans.m[2][3] = 0;

	ans.m[3][0] = 0;
	ans.m[3][1] = 0;
	ans.m[3][2] = 0;
	ans.m[3][3] = 1;

	return ans;
}


Matrix4x4 MakeTranslateMatrix(const Vector3& translate) {
	Matrix4x4 ans;

	ans.m[0][0] = 1;
	ans.m[0][1] = 0;
	ans.m[0][2] = 0;
	ans.m[0][3] = 0;

	ans.m[1][0] = 0;
	ans.m[1][1] = 1;
	ans.m[1][2] = 0;
	ans.m[1][3] = 0;

	ans.m[2][0] = 0;
	ans.m[2][1] = 0;
	ans.m[2][2] = 1;
	ans.m[2][3] = 0;

	ans.m[3][0] = translate.x;
	ans.m[3][1] = translate.y;
	ans.m[3][2] = translate.z;
	ans.m[3][3] = 1;

	return ans;
}

Matrix4x4 MakeScaleMatrix(const Vector3& scale) {
	Matrix4x4 ans;

	ans.m[0][0] = scale.x;
	ans.m[0][1] = 0;
	ans.m[0][2] = 0;
	ans.m[0][3] = 0;

	ans.m[1][0] = 0;
	ans.m[1][1] = scale.y;
	ans.m[1][2] = 0;
	ans.m[1][3] = 0;

	ans.m[2][0] = 0;
	ans.m[2][1] = 0;
	ans.m[2][2] = scale.z;
	ans.m[2][3] = 0;

	ans.m[3][0] = 0;
	ans.m[3][1] = 0;
	ans.m[3][2] = 0;
	ans.m[3][3] = 1;
	return ans;
}

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix) {

	Vector3 ans;

	ans.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
	ans.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
	ans.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];

	assert(w != 0.0f);
	ans.x /= w;
	ans.y /= w;
	ans.z /= w;
	return ans;
}

Matrix4x4 MakeRotateXMatrix(float radian) {

	Matrix4x4 ans;
	ans.m[0][0] = 1;
	ans.m[0][1] = 0;
	ans.m[0][2] = 0;
	ans.m[0][3] = 0;

	ans.m[1][0] = 0;
	ans.m[1][1] = std::cos(radian);
	;
	ans.m[1][2] = std::sin(radian);
	;
	ans.m[1][3] = 0;

	ans.m[2][0] = 0;
	ans.m[2][1] = -std::sin(radian);
	;
	ans.m[2][2] = std::cos(radian);
	;
	ans.m[2][3] = 0;

	ans.m[3][0] = 0;
	ans.m[3][1] = 0;
	ans.m[3][2] = 0;
	ans.m[3][3] = 1;

	return ans;
}

Matrix4x4 MakeRotateYMatrix(float radian) {

	Matrix4x4 ans;
	ans.m[0][0] = std::cos(radian);
	ans.m[0][1] = 0;
	ans.m[0][2] = -std::sin(radian);
	ans.m[0][3] = 0;

	ans.m[1][0] = 0;
	ans.m[1][1] = 1;
	ans.m[1][2] = 0;
	ans.m[1][3] = 0;

	ans.m[2][0] = std::sin(radian);
	;
	ans.m[2][1] = 0;
	ans.m[2][2] = std::cos(radian);
	;
	ans.m[2][3] = 0;

	ans.m[3][0] = 0;
	ans.m[3][1] = 0;
	ans.m[3][2] = 0;
	ans.m[3][3] = 1;

	return ans;
}

Matrix4x4 MakeRotateZMatrix(float radian) {
	Matrix4x4 ans;
	ans.m[0][0] = std::cos(radian);
	ans.m[0][1] = std::sin(radian);
	ans.m[0][2] = 0;
	ans.m[0][3] = 0;

	ans.m[1][0] = -std::sin(radian);
	ans.m[1][1] = std::cos(radian);
	ans.m[1][2] = 0;
	ans.m[1][3] = 0;

	ans.m[2][0] = 0;
	ans.m[2][1] = 0;
	ans.m[2][2] = 1;
	ans.m[2][3] = 0;

	ans.m[3][0] = 0;
	ans.m[3][1] = 0;
	ans.m[3][2] = 0;
	ans.m[3][3] = 1;

	return ans;
}

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {

	Matrix4x4 rotateXYZ = Multiply(Multiply(MakeRotateXMatrix(rotate.x), MakeRotateYMatrix(rotate.y)), MakeRotateZMatrix(rotate.z));
	return Multiply(Multiply(MakeScaleMatrix(scale), rotateXYZ), MakeTranslateMatrix(translate));
}



//透視投影行列
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip) {

	Matrix4x4 result;

	result.m[0][0] = 1 / (aspectRatio * std::tan(fovY / 2));
	result.m[1][0] = 0;
	result.m[2][0] = 0;
	result.m[3][0] = 0;

	result.m[0][1] = 0;
	result.m[1][1] = 1 / std::tan(fovY / 2);
	result.m[2][1] = 0;
	result.m[3][1] = 0;

	result.m[0][2] = 0;
	result.m[1][2] = 0;
	result.m[2][2] = farClip / (farClip - nearClip);
	result.m[3][2] = (-nearClip * farClip * std::cos(0.0f)) / (farClip - nearClip);

	result.m[0][3] = 0;
	result.m[1][3] = 0;
	result.m[2][3] = 1;
	result.m[3][3] = 0;

	return result;
}


#pragma endregion


Matrix4x4 MakeIdentity4x4() {
	Matrix4x4 result = {};
	for (int i = 0; i < 4; i++) {
		result.m[i][i] = 1;
	}
	return result;
}


struct Transform1 {
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
};

Transform1 transform{ { 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f, }};

Transform1 cameraTransform{ {1.0f,1.0f,1.0f,}, {0.0f,0.0f,0.0f}, {0.0f, 0.0f, -5.0f} };


struct VertexData {
	Vector4 position;
	Vector2 texcoord;
};


std::wstring ConvertString(const std::string& str) {
	if (str.empty()) {
		return std::wstring();
	}

	auto sizeNeeded = MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(&str[0]), static_cast<int>(str.size()), NULL, 0);
	if (sizeNeeded == 0) {
		return std::wstring();
	}
	std::wstring result(sizeNeeded, 0);
	MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(&str[0]), static_cast<int>(str.size()), &result[0], sizeNeeded);
	return result;
}

std::string ConvertString(const std::wstring& str) {
	if (str.empty()) {
		return std::string();
	}

	auto sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), NULL, 0, NULL, NULL);
	if (sizeNeeded == 0) {
		return std::string();
	}
	std::string result(sizeNeeded, 0);
	WideCharToMultiByte(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), result.data(), sizeNeeded, NULL, NULL);
	return result;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {

	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)) {
		return true;
	}


	switch (msg) {

	case WM_DESTROY:

		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, msg, wparam, lparam);
}

const int32_t kClientWidth = 1280;

const int32_t kClientHeight = 720;

RECT wrc = { 0, 0, kClientWidth, kClientHeight };



//std::string str0{ "STRING!!!" };
//std::string str1{ std::to_string(10) };

void Log(const std::string& message) {
	OutputDebugStringA(message.c_str());
}



IDxcBlob* CompileShader(
	//CompilerするShaderファイルへのパス
	const std::wstring& filePath,
	//Compilerに思料するProfile
	const wchar_t* profile,
	//初期化で生成したものを3つ
	IDxcUtils* dxcUtils,
	IDxcCompiler3* dxcCompiler,
	IDxcIncludeHandler* includeHandler)

{

	Log(ConvertString(std::format(L"Begin CompileShader, path:{}, profile:{}\n", filePath, profile)));
	//hlsファイルを読む
	IDxcBlobEncoding* shaderSource = nullptr;
	HRESULT hr = dxcUtils->LoadFile(filePath.c_str(), nullptr, &shaderSource);
	//読めなかったら止める
	assert(SUCCEEDED(hr));
	//読み込んだファイルの内容を設定する
	DxcBuffer shaderSourceBuffer;
	shaderSourceBuffer.Ptr = shaderSource->GetBufferPointer();
	shaderSourceBuffer.Size = shaderSource->GetBufferSize();
	shaderSourceBuffer.Encoding = DXC_CP_UTF8; // UTF8の文字コードであることを通知


	LPCWSTR arguments[] = {
		filePath.c_str(),
		L"-E", L"main",
		L"-T", profile,
		L"-Zi", L"-Qembed_debug",
		L"-Od",
		L"-Zpr",
	};

	//実際にShaderをコンパイルする
	IDxcResult* shaderResult = nullptr;
	hr = dxcCompiler->Compile(
		&shaderSourceBuffer,
		arguments,
		_countof(arguments),
		includeHandler,
		IID_PPV_ARGS(&shaderResult)
	);
	assert(SUCCEEDED(hr));


	IDxcBlobUtf8* shaderError = nullptr;
	shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError), nullptr);
	if (shaderError != nullptr && shaderError->GetStringLength() != 0) {
		Log(shaderError->GetStringPointer());
		assert(false);
	}


	IDxcBlob* shaderBlob = nullptr;
	hr = shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob), nullptr);
	assert(SUCCEEDED(hr));
	//成功したらログを出す
	Log(ConvertString(std::format(L"Compile Succeeded, path:{}, profile:{}\n", filePath, profile)));
	//もう使わないリソースを解放
	shaderSource->Release();
	shaderResult->Release();
	//実行用のバイナリを返却
	return shaderBlob;

}



ID3D12Resource* CreateBufferResource(ID3D12Device* device, size_t sizeIntBytes)
{

	//頂点リソース用のヒープの設定
	D3D12_HEAP_PROPERTIES uploadHeapProperties{};
	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;// UploadHeapを使う
	//頂点リソースの設定
	D3D12_RESOURCE_DESC vertexResourceDesc{};
	//バッファリソース、テクスチャの場合はまた別の設定をする
	vertexResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	vertexResourceDesc.Width = sizeIntBytes;
	//バッファの場合はこれらは1にする決まり
	vertexResourceDesc.Height = 1;
	vertexResourceDesc.DepthOrArraySize = 1;
	vertexResourceDesc.MipLevels = 1;
	vertexResourceDesc.SampleDesc.Count = 1;
	//バッファの場合はこれにする決まり
	vertexResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//実際に頂点リソースを作る
	ID3D12Resource* vertexResource = nullptr;
	HRESULT hr = device->CreateCommittedResource(
		&uploadHeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&vertexResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertexResource));
	assert(SUCCEEDED(hr));

	return vertexResource;

};



ID3D12DescriptorHeap* CreateDescriptorHeap(
	ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible){

		ID3D12DescriptorHeap * descriptorHeap = nullptr;
        D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc{};
        descriptorHeapDesc.Type = heapType;
        descriptorHeapDesc.NumDescriptors = numDescriptors;
        descriptorHeapDesc.Flags = shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        HRESULT hr = device->CreateDescriptorHeap(&descriptorHeapDesc, IID_PPV_ARGS(&descriptorHeap));
	    assert(SUCCEEDED(hr));
        return descriptorHeap;

	}


DirectX::ScratchImage LoadTexture(const std::string& filePath)
{
	//テクスチャファイルを読んでプログラムで扱えるようにする
	DirectX::ScratchImage image{};
	std::wstring filePathW = ConvertString(filePath);
	HRESULT hr = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
	assert(SUCCEEDED(hr));

	//ミップマップの作成
	DirectX::ScratchImage mipImages{};
	hr = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 0, mipImages);
	assert(SUCCEEDED(hr));

	//ミップマップ付きのデータを返す
	return mipImages;
}

ID3D12Resource* CreateTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metadata) 
{

	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = UINT(metadata.width);
	resourceDesc.Height = UINT(metadata.height);
	resourceDesc.MipLevels = UINT16(metadata.mipLevels);
	resourceDesc.DepthOrArraySize = UINT16(metadata.arraySize);
	resourceDesc.Format = metadata.format;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION(metadata.dimension);

	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_CUSTOM;
	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	//Resourceの生成
	ID3D12Resource* resource = nullptr;
	HRESULT hr = device->CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&resource));
	assert(SUCCEEDED(hr));
	return resource;

}



void UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages)
{

	//Meta情報を取得
	const DirectX::TexMetadata& metadata = mipImages.GetMetadata();
	//全MipMapについて
	for (size_t mipLevel = 0; mipLevel < metadata.mipLevels; ++mipLevel) {
		//MipMapLevelを指定して名Imageを取得
		const DirectX::Image* img = mipImages.GetImage(mipLevel, 0, 0);
		// Textureに転送
		HRESULT hr = texture->WriteToSubresource(
			UINT(mipLevel),
			nullptr,
			img->pixels,
			UINT(img->rowPitch),
			UINT(img->slicePitch)
		);
		assert(SUCCEEDED(hr));
	}
}





// Windowsアプリでのエントリーポイント（main関数）
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {


	CoInitializeEx(0, COINIT_MULTITHREADED);
	CoUninitialize();


#pragma region Windowの生成

	WNDCLASS wc{};

	wc.lpfnWndProc = WindowProc;

	wc.lpszClassName = L"CG2WindowClass";

	wc.hInstance = GetModuleHandle(nullptr);

	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

	RegisterClass(&wc);

	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	HWND hwnd = CreateWindow(
		wc.lpszClassName,
		L"CG2",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		wrc.right - wrc.left,
		wrc.bottom - wrc.top,
		nullptr,
		nullptr,
		wc.hInstance,
		nullptr);

	ShowWindow(hwnd, SW_SHOW);

#pragma endregion

#ifdef _DEBUG
	ID3D12Debug1* debugController = nullptr;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {

		debugController->EnableDebugLayer();

		debugController->SetEnableGPUBasedValidation(TRUE);

	}
#endif



#pragma region deviceの作成

	IDXGIFactory7* dxgiFactory = nullptr;

	HRESULT hr = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));

	assert(SUCCEEDED(hr));

	IDXGIAdapter4* useAdapter = nullptr;

#pragma endregion

#pragma region

	for (UINT i = 0; dxgiFactory->EnumAdapterByGpuPreference(i,
		DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&useAdapter)) !=
		DXGI_ERROR_NOT_FOUND; ++i) {

		DXGI_ADAPTER_DESC3 adapterDesc{};
		hr = useAdapter->GetDesc3(&adapterDesc);
		assert(SUCCEEDED(hr));

		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)) {
			Log(ConvertString(std::format(L"Use Adapater:{}\n", adapterDesc.Description)));
			break;
		}
		useAdapter = nullptr;
	}

	assert(useAdapter != nullptr);


	ID3D12Device* device = nullptr;

	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_12_2,  D3D_FEATURE_LEVEL_12_1, D3D_FEATURE_LEVEL_12_0
	};
	const char* featureLevelStrings[] = { "12.2", "12.1","12.0" };

	for (size_t i = 0; i < _countof(featureLevels); ++i) {

		hr = D3D12CreateDevice(useAdapter, featureLevels[i], IID_PPV_ARGS(&device));

		if (SUCCEEDED(hr)) {

			Log(std::format("FeatureLevel : {}\n", featureLevelStrings[i]));
			break;
		}
	}

	assert(device != nullptr);
	Log("Complete create D3D12Device!!!\n");

#pragma endregion

#ifdef _DEBUG

	ID3D12InfoQueue* infoQueue = nullptr;
	if (SUCCEEDED(device->QueryInterface(IID_PPV_ARGS(&infoQueue)))) {

		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);

		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);

		D3D12_MESSAGE_ID denyIds[] = {
			D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE
		};

		D3D12_MESSAGE_SEVERITY severities[] = { D3D12_MESSAGE_SEVERITY_INFO };
		D3D12_INFO_QUEUE_FILTER filter{};
		filter.DenyList.NumIDs = _countof(denyIds);
		filter.DenyList.pIDList = denyIds;
		filter.DenyList.NumSeverities = _countof(severities);
		filter.DenyList.pSeverityList = severities;

		infoQueue->PushStorageFilter(&filter);

		infoQueue->Release();
	}
#endif

#pragma region

	ID3D12CommandQueue* commandQueue = nullptr;
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
	hr = device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&commandQueue));
	assert(SUCCEEDED(hr));


	ID3D12CommandAllocator* commandAllocator = nullptr;
	hr = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator));
	assert(SUCCEEDED(hr));

	ID3D12GraphicsCommandList* commandList = nullptr;
	hr = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator, nullptr, IID_PPV_ARGS(&commandList));
	assert(SUCCEEDED(hr));

#pragma endregion


	IDXGISwapChain4* swapChain = nullptr;
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	swapChainDesc.Width = kClientWidth;
	swapChainDesc.Height = kClientHeight;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 2;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	hr = dxgiFactory->CreateSwapChainForHwnd(commandQueue, hwnd, &swapChainDesc, nullptr, nullptr, reinterpret_cast<IDXGISwapChain1**>(&swapChain));
	assert(SUCCEEDED(hr));

	ID3D12DescriptorHeap* rtvDescriptorHeap = CreateDescriptorHeap(device, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 2, false);
	ID3D12DescriptorHeap* srvDescriptorHeap = CreateDescriptorHeap(device, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 128, true);
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescriptorHeapDesc{};
	rtvDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDescriptorHeapDesc.NumDescriptors = 2;
	hr = device->CreateDescriptorHeap(&rtvDescriptorHeapDesc, IID_PPV_ARGS(&rtvDescriptorHeap));
	assert(SUCCEEDED(hr));


	ID3D12Resource* swapChainResources[2] = { nullptr };
	hr = swapChain->GetBuffer(0, IID_PPV_ARGS(&swapChainResources[0]));
	assert(SUCCEEDED(hr));
	hr = swapChain->GetBuffer(1, IID_PPV_ARGS(&swapChainResources[1]));
	assert(SUCCEEDED(hr));

	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	D3D12_CPU_DESCRIPTOR_HANDLE rtvStartHandle = rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles[2];
	rtvHandles[0] = rtvStartHandle;
	device->CreateRenderTargetView(swapChainResources[0], &rtvDesc, rtvHandles[0]);
	rtvHandles[1].ptr = rtvHandles[0].ptr + device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	device->CreateRenderTargetView(swapChainResources[1], &rtvDesc, rtvHandles[1]);


	DirectX::ScratchImage mipImages = LoadTexture("resources/uvChecker.png");
	const DirectX::TexMetadata& metadata = mipImages.GetMetadata();
	ID3D12Resource* textureResource = CreateTextureResource(device, metadata);
	UploadTextureData(textureResource, mipImages);



	//metaDataを基にSRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = UINT(metadata.mipLevels);

	//SRVを作成するDescriptorHeapの場所を決める
	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU = srvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU = srvDescriptorHeap->GetGPUDescriptorHandleForHeapStart();
	//戦闘はImGuiが使っているのでその次を使う
	textureSrvHandleCPU.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	textureSrvHandleGPU.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	//SRVの生成
	device->CreateShaderResourceView(textureResource, &srvDesc, textureSrvHandleCPU);


	//出力ウィンドウへの文字出力
	OutputDebugStringA("Hello,DirectX!\n");

	MSG msg{};


	ID3D12Fence* fence = nullptr;
	uint64_t fenceValue = 0;
	hr = device->CreateFence(fenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
	assert(SUCCEEDED(hr));

	HANDLE fenceEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	assert(fenceEvent != nullptr);


	IDxcUtils* dxcUtils = nullptr;
	IDxcCompiler3* dxcCompiler = nullptr;
	hr = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils));
	assert(SUCCEEDED(hr));
	hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler));
	assert(SUCCEEDED(hr));


	//現時点でincludeはしないが、includeに対応するための設定を行っておく
	IDxcIncludeHandler* includeHandler = nullptr;
	hr = dxcUtils->CreateDefaultIncludeHandler(&includeHandler);
	assert(SUCCEEDED(hr));


	//RootSignature作成
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	//RootParameter作成。複数設定できるので配列。今回は結果は1つだけなので長さ１の配列
	D3D12_ROOT_PARAMETER rootParameters[2] = {};
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters[0].Descriptor.ShaderRegister = 0;
	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rootParameters[1].Descriptor.ShaderRegister = 0;
	descriptionRootSignature.pParameters = rootParameters;
	descriptionRootSignature.NumParameters = _countof(rootParameters);


	//シリアライズしてバイナリにする
	ID3DBlob* signatureBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	hr = D3D12SerializeRootSignature(&descriptionRootSignature,
		D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	if (FAILED(hr)) {
		Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		assert(false);
	}
	//バイナリをもとび生成
	ID3D12RootSignature* rootSignature = nullptr;
	hr = device->CreateRootSignature(0,
		signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(hr));


	D3D12_INPUT_ELEMENT_DESC inputElementDescs[2] = {};
	inputElementDescs[0].SemanticName = "POSITION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	inputElementDescs[1].SemanticName = "TEXCOORD";
	inputElementDescs[1].SemanticIndex = 0;
	inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = _countof(inputElementDescs);

	//BlendStateの設定
	D3D12_BLEND_DESC blendDesc{};
	//すべての色要素を書き込む
	blendDesc.RenderTarget[0].RenderTargetWriteMask =
		D3D12_COLOR_WRITE_ENABLE_ALL;

	// RasiterzerStateの設定
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	//　裏面　（時計回り）を表示しない
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	//　三角形の中を塗りつぶす
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	// Shaderをコンパイルする
	IDxcBlob* vertexShaderBlob = CompileShader(L"Object3D.VS.hlsl",
		L"vs_6_0", dxcUtils, dxcCompiler, includeHandler);
	assert(vertexShaderBlob != nullptr);

	IDxcBlob* pixelShaderBlob = CompileShader(L"Object3D.PS.hlsl",
		L"ps_6_0", dxcUtils, dxcCompiler, includeHandler);
	assert(pixelShaderBlob != nullptr);


	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	graphicsPipelineStateDesc.pRootSignature = rootSignature;
	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc;
	graphicsPipelineStateDesc.VS = { vertexShaderBlob->GetBufferPointer(),
	vertexShaderBlob->GetBufferSize() };
	graphicsPipelineStateDesc.PS = { pixelShaderBlob->GetBufferPointer(),
	pixelShaderBlob->GetBufferSize() };
	graphicsPipelineStateDesc.BlendState = blendDesc;
	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc;
	//　書き込むRTVの情報
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	//利用するトロポジ（形状）のタイプ。三角形
	graphicsPipelineStateDesc.PrimitiveTopologyType =
		D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	//どのように画面に色を打ち込むのかの設定
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	//実際に生成
	ID3D12PipelineState* graphicsPipelineState = nullptr;
	hr = device->CreateGraphicsPipelineState(&graphicsPipelineStateDesc,
		IID_PPV_ARGS(&graphicsPipelineState));
	assert(SUCCEEDED(hr));


	ID3D12Resource* vertexResource = CreateBufferResource(device, sizeof(VertexData) * 3);

	//マテリアル用のリソースを作る。今回はcolor1つ文のサイズを用意する
	ID3D12Resource* materialResource = CreateBufferResource(device, sizeof(VertexData));
	//マテリアルにデータを書き込む
	Vector4* materialDate = nullptr;
	//書き込むためのアドレスを取得
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialDate));
	//今回は赤を書き込んでみる
	*materialDate = Vector4(1.0f, 0.0f, 0.0f, 1.0f);


	//WVp用のリソースを作る。Matrix4x4　一つ分のサイズを用意する
	ID3D12Resource* wvpResource = CreateBufferResource(device, sizeof(Matrix4x4));
	//データを書き込む
	Matrix4x4* wvpDate = nullptr;
	//書き込むためのアドレスえお取得
	wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvpDate));
	//単位行列を書き込んでおく
	*wvpDate = MakeIdentity4x4();


		//頂点バッファビューを作成する
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
		//リソースのsizeは頂点３つ分のsize
		vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
		//使用するリソースのサイズは頂点3つ分のサイズ
		vertexBufferView.SizeInBytes = sizeof(VertexData) * 3;
		//1頂点当たりのサイズ
		vertexBufferView.StrideInBytes = sizeof(VertexData);




	//頂点リソースにデータを書き込む
	VertexData* vertexData = nullptr;
	//書き込むためのアドレスを取得
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	//左下
	vertexData[0].position = { -0.5, -0.5f, 0.0f, 1.0f };
	vertexData[0].texcoord = { 0.0f, 1.0f };
	//上
	vertexData[1].position = { 0.0f, 0.5f, 0.0f, 1.0f };
	vertexData[1].texcoord = { 0.5f,0.0f };
	//右下
	vertexData[2].position = { 0.5f, -0.5f, 0.0f, 1.0f };
	vertexData[2].texcoord = { 1.0f, 1.0f };



	D3D12_VIEWPORT viewport{};
	//クライアント領域のサイズと１一緒にして画面全体に表示
	viewport.Width = kClientWidth;
	viewport.Height = kClientHeight;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	//シザー矩形
	D3D12_RECT scissorRect{};
	//基本的にビューポートと同じ矩形が構成されるようにする
	scissorRect.left = 0;
	scissorRect.right = kClientWidth;
	scissorRect.top = 0;
	scissorRect.bottom = kClientHeight;



	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX12_Init(device,
		swapChainDesc.BufferCount,
		rtvDesc.Format,
		srvDescriptorHeap,
		srvDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
		srvDescriptorHeap->GetGPUDescriptorHandleForHeapStart());

	

	while (msg.message != WM_QUIT) {

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {

			//ゲーム処理

			transform.rotate.y += 0.03f;
			Matrix4x4 worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
			Matrix4x4 cameraMatrix = MakeAffineMatrix(cameraTransform.scale, cameraTransform.rotate, cameraTransform.translate);
			Matrix4x4 viewMatrix = Inverse(cameraMatrix);
			Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kClientWidth) / float(kClientHeight), 0.1f, 100.0f);
			Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
		//	*transformationMatrixData = 
			*wvpDate = worldViewProjectionMatrix;


	


			//これから書き込むバッファのインデックスを取得
			UINT backBufferIndex = swapChain->GetCurrentBackBufferIndex();
	
			ImGui_ImplDX12_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();
			ImGui::Begin("SetColor");
			ImGui::ColorEdit4("*materialData", &materialDate->x);
			ImGui::End();
			ImGui::ShowDemoWindow();
			ImGui::Render();
		

			D3D12_RESOURCE_BARRIER barrier{};

			barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;

			barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;

			barrier.Transition.pResource = swapChainResources[backBufferIndex];

			barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;

			barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;



			commandList->ResourceBarrier(1, &barrier);
			//////////////////////////////////////////////

			//描画先のRTVを設定する
			commandList->OMSetRenderTargets(1, &rtvHandles[backBufferIndex], false, nullptr);
			//指定した色で画面全体をクリアする
			float clearColor[] = { 0.1f, 0.25f, 0.5f, 1.0f };
			commandList->ClearRenderTargetView(rtvHandles[backBufferIndex], clearColor, 0, nullptr);

			//描画用のDescriptorHeapの設定
			ID3D12DescriptorHeap* descriptorHeaps[] = { srvDescriptorHeap };
			commandList->SetDescriptorHeaps(1, descriptorHeaps);

			commandList->RSSetViewports(1, &viewport);
			commandList->RSSetScissorRects(1, &scissorRect);
			//RootSignatureを設定。PSOに設定しているけど別途設定が必要
			commandList->SetGraphicsRootSignature(rootSignature);
			commandList->SetPipelineState(graphicsPipelineState);
			commandList->IASetVertexBuffers(0, 1, &vertexBufferView);//
			commandList->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());
			commandList->SetGraphicsRootConstantBufferView(1, wvpResource->GetGPUVirtualAddress());
			//形状を設定。PSOに設定しているものとはまた別、同じものを設定すると考えておけば良い
			commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			//描画　（DrawCall/drawコール）　。　3頂点で1つのインスタンス。
			commandList->DrawInstanced(3, 1, 0, 0);


			barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
			//実際のcommandListのImGuiの描画コマンドを積む
			ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList);
			barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;

			commandList->ResourceBarrier(1, &barrier);


			//コマンドリストの内容を確定させる。　すべてのコマンドを頼んでからclauseすること
			hr = commandList->Close();
			assert(SUCCEEDED(hr));


			//GPUにコマンドリストの実行を行わせる
			ID3D12CommandList* commandLists[] = { commandList };
			commandQueue->ExecuteCommandLists(1, commandLists);
			//　GPUとOSに画面の交換を行うように通知する
			swapChain->Present(1, 0);


			fenceValue++;
			commandQueue->Signal(fence, fenceValue);

			if (fence->GetCompletedValue() < fenceValue) {

				fence->SetEventOnCompletion(fenceValue, fenceEvent);

				WaitForSingleObject(fenceEvent, INFINITE);

			}


			//次のフレーム用のコマンドリストを準備
			hr = commandAllocator->Reset();
			assert(SUCCEEDED(hr));
			hr = commandList->Reset(commandAllocator, nullptr);
			assert(SUCCEEDED(hr));


		}
	}


	//Log(ConvertString(std::format(L"Begin CompileShader, path:{}, profile:{}\n", filePath, profile)));

	//Log("Hello\n");
	//Log(std::format("enemyHp:{}, texturePath:{}\n", 10, 20));

	//std::wstring Convertstring(const std::string & str);
	//std::string ConvertString(const std::wstring & str);

	//Log(ConvertString(std::format(L"WSTRING{}\n",10)));

	//1_3////////////

	IDXGIDebug1* debug;
	if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debug)))) {
		debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
		debug->ReportLiveObjects(DXGI_DEBUG_APP, DXGI_DEBUG_RLO_ALL);
		debug->ReportLiveObjects(DXGI_DEBUG_D3D12, DXGI_DEBUG_RLO_ALL);
		debug->Release();
	}


	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	wvpResource->Release();
	vertexResource->Release();
	graphicsPipelineState->Release();
	signatureBlob->Release();
	if (errorBlob) {
		errorBlob->Release();
	}
	rootSignature->Release();
	pixelShaderBlob->Release();
	vertexShaderBlob->Release();
    materialResource->Release();

	srvDescriptorHeap->Release();


	CloseHandle(fenceEvent);
	fence->Release();
	rtvDescriptorHeap->Release();
	swapChainResources[0]->Release();
	swapChainResources[1]->Release();
	swapChain->Release();
	commandList->Release();
	commandAllocator->Release();
	commandQueue->Release();
	device->Release();
	useAdapter->Release();
	dxgiFactory->Release();
#ifdef _DEBUG
	debugController->Release();
#endif
	CloseWindow(hwnd);

	return 0;
}



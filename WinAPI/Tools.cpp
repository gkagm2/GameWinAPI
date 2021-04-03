#include "stdafx.h"
#include "Tools.h"



///////////////// Matrix ////////////////

// 단위행렬
void SetIdentityMatrix4x4(Matrix4x4& m) {
	memset(m.m_afElements, 0, sizeof(m.m_afElements));
	m(0, 0) = m(1, 1) = m(2, 2) = m(3, 3) = 1.0f;
}

// 행렬 덧셈
void MatrixPlus(Matrix4x4 q) {
	/*for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			q(i, j) = a(i, j) + b(i, j);
		}
	}*/
}


// 행렬 뺄셈
void MatrixMinus(Matrix4x4& q, const Matrix4x4& a, const Matrix4x4& b) {
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			q(i, j) = a(i, j) - b(i, j);
		}
	}
}

// 행렬 곱셈
void MatrixMultiply(Matrix4x4& q, const Matrix4x4& a, const Matrix4x4& b) {
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			for (int k = 0; k < 4; ++k) {
				q(i, j) += a(i, k) * b(k, j);
			}
		}
	}
}

// 이동 행렬
inline void SetTranslateMatrix(Matrix4x4& m, float tx, float ty, float tz) {
	SetIdentityMatrix4x4(m);
	m(3, 0) = tx;
	m(3, 1) = ty;
	m(3, 2) = tz;
}

// 확대,축소 행렬
inline void SetScaleMatrix(Matrix4x4& m, float sx, float sy, float sz) {
	SetIdentityMatrix4x4(m);
	m(0, 0) = sx;
	m(1, 1) = sy;
	m(2, 2) = sz;
}

// 회전 행렬 X
void SetRotateXMatrix(Matrix4x4& m, float fRads) {
	SetIdentityMatrix4x4(m);
	m(1, 1) = cos(fRads);
	m(1, 2) = sin(fRads);
	m(2, 1) = -sin(fRads);
	m(2, 2) = cos(fRads);
}

// 회전 행렬 Y
void SetRotateYMatrix(Matrix4x4& m, float fRads) {
	SetIdentityMatrix4x4(m);
	m(0, 0) = cosf(fRads);
	m(0, 2) = -sinf(fRads);
	m(2, 0) = sinf(fRads);
	m(2, 2) = cos(fRads);
}

// 회전 행렬 Z
void SetRotateZMatrix(Matrix4x4& m, float fRads) {
	SetIdentityMatrix4x4(m);
	m(0, 0) = cosf(fRads);
	m(0, 1) = sinf(fRads);
	m(1, 0) = -sinf(fRads);
	m(1, 1) = cos(fRads);
}

// 프로젝션
void SetProjection(Matrix4x4& m, float d) {
	m.SetZero();
	m(0, 0) = d;
	m(1, 1) = d;
	m(2, 2) = 1;
	m(3, 3) = d;
}

// 전치 행렬
void TransposedMatrix(Matrix4x4& m) {
	float t;
	t = m(0, 1);
	m(0, 1) = m(1, 0);
	m(1, 0) = t;
	for (int y = 0; y < 4; ++y) {
		for (int x = 0; x < 4; ++x) {
			t = m(y, x);
			m(y, x) = m(x, y);
			m(x, y) = t;
		}
	}
}

// TODO (Sagacity Jang)
// 역행렬

// 행렬 내적

// 행렬 외적



// TODO :  공식으로 추려야한다.! (성능 엄청안좋음)
// 비율을 유지하면서 원하는 사이즈로 맞춘다.
// 맞출 사이즈 : _fitWidth, _fitHeight, 원본 크기 : _width, _height
Vector2 ResizeScaleMaintainRatio(float _fitWidth, float _fitHeight, float _width, float _height)
{
	float maxValue = _width > _height ? _width : _height;
	float x = _width / maxValue;
	float y = _height / maxValue;

	
	float i = _fitWidth > _fitHeight ? _fitWidth : _fitHeight;
	while (i > 0) {
		if (x * i <= _fitWidth && y * i <= _fitHeight) {
			x = x * (i + 0.01f);
			y = y * (i + 0.01f);
			break;
		}
		i -= 0.1f;
	}


	return Vector2(x, y);
}


// 벡터 회전 2D Rotate
Vector3 Rotate(const Vector3& _vVec, float _fDegree)
{
	float fRadian = CMyMath::DegreeToRadian(_fDegree);

	// 회전 
	Vector3 vNewVec(
		_vVec.x * cosf(fRadian) - _vVec.y * sinf(fRadian),
		_vVec.x * sinf(fRadian) + _vVec.y * cosf(fRadian),
		0.0f
	);
	return vNewVec;
}

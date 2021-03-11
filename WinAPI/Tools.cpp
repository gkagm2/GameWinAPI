#include "stdafx.h"
#include "Tools.h"



///////////////// Matrix ////////////////

// �������
void SetIdentityMatrix4x4(Matrix4x4& m) {
	memset(m.m_afElements, 0, sizeof(m.m_afElements));
	m(0, 0) = m(1, 1) = m(2, 2) = m(3, 3) = 1.0f;
}

// ��� ����
void MatrixPlus(Matrix4x4 q) {
	/*for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			q(i, j) = a(i, j) + b(i, j);
		}
	}*/
}


// ��� ����
void MatrixMinus(Matrix4x4& q, const Matrix4x4& a, const Matrix4x4& b) {
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			q(i, j) = a(i, j) - b(i, j);
		}
	}
}

// ��� ����
void MatrixMultiply(Matrix4x4& q, const Matrix4x4& a, const Matrix4x4& b) {
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			for (int k = 0; k < 4; ++k) {
				q(i, j) += a(i, k) * b(k, j);
			}
		}
	}
}

// �̵� ���
inline void SetTranslateMatrix(Matrix4x4& m, float tx, float ty, float tz) {
	SetIdentityMatrix4x4(m);
	m(3, 0) = tx;
	m(3, 1) = ty;
	m(3, 2) = tz;
}

// Ȯ��,��� ���
inline void SetScaleMatrix(Matrix4x4& m, float sx, float sy, float sz) {
	SetIdentityMatrix4x4(m);
	m(0, 0) = sx;
	m(1, 1) = sy;
	m(2, 2) = sz;
}

// ȸ�� ��� X
void SetRotateXMatrix(Matrix4x4& m, float fRads) {
	SetIdentityMatrix4x4(m);
	m(1, 1) = cos(fRads);
	m(1, 2) = sin(fRads);
	m(2, 1) = -sin(fRads);
	m(2, 2) = cos(fRads);
}

// ȸ�� ��� Y
void SetRotateYMatrix(Matrix4x4& m, float fRads) {
	SetIdentityMatrix4x4(m);
	m(0, 0) = cosf(fRads);
	m(0, 2) = -sinf(fRads);
	m(2, 0) = sinf(fRads);
	m(2, 2) = cos(fRads);
}

// ȸ�� ��� Z
void SetRotateZMatrix(Matrix4x4& m, float fRads) {
	SetIdentityMatrix4x4(m);
	m(0, 0) = cosf(fRads);
	m(0, 1) = sinf(fRads);
	m(1, 0) = -sinf(fRads);
	m(1, 1) = cos(fRads);
}

// ��������
void SetProjection(Matrix4x4& m, float d) {
	m.SetZero();
	m(0, 0) = d;
	m(1, 1) = d;
	m(2, 2) = 1;
	m(3, 3) = d;
}

// ��ġ ���
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
// �����

// ��� ����

// ��� ����



// ������ �����ϸ鼭 ���ϴ� ������� �����.
Vector2 ResizeScaleMaintainRatio(float _fitWidth, float _fitHeight, float _width, float _height)
{
	float maxValue = _width > _height ? _width : _height;
	float x = _width / maxValue;
	float y = _height / maxValue;
	float minGap = _fitWidth - x > _fitHeight - y ? _fitHeight : _fitWidth;
	x *= minGap;
	y *= minGap;
	return Vector2(x, y);
}
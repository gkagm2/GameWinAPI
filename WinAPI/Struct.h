#pragma once

struct Vector2;
struct Vector3;

////////////////// Vector ///////////////////
struct Vector2 {
	float x;
	float y;

	Vector2() :x(0), y(0) {}
	Vector2(float _x, float _y) : x(_x), y(_y) {}
	Vector2(const POINT& _point) : x((float)_point.x), y((float)_point.y) {}
	Vector2(const Vector2& _vec) : x(_vec.x), y (_vec.y) {}

	void Set(float _x, float _y) { x = _x, y = _y; }
	void Set(int _x, int _y) { x = (float)_x, y = (float)_y; }

	Vector2 operator=(const Vector2& _rhs) {
		x = _rhs.x;
		y = _rhs.y;
		return *this;
	}
	Vector2 operator=(const POINT& _rhs) {
		x = (float)_rhs.x;
		y = (float)_rhs.y;
		return *this;
	}
	Vector2 operator+(const Vector2& _rhs) const {
		return Vector2{ x + _rhs.x, y + _rhs.y };
	}
	Vector2 operator-(const Vector2& _rhs) const {
		return Vector2{ x - _rhs.x, y - _rhs.y };
	}
	Vector2 operator/=(const float& rhs) {
		x /= rhs;
		y /= rhs;
		return *this;
	}
	Vector2 operator-=(const Vector2& rhs) {
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}
	Vector2 operator-=(const float& rhs) {
		x -= rhs;
		y -= rhs;
		return *this;
	}

	bool operator==(const Vector2& _rhs) const {
		return (x == _rhs.x && y == _rhs.y);
	}
	bool operator!=(const Vector2& _rhs) const {
		return !(*this == _rhs);
	}
};

struct Vector3 : public Vector2{
	float z;

	Vector3() : Vector2(), z(0) {}
	Vector3(float _x, float _y) : Vector2(_x,_y), z(0) {}
	Vector3(float _x, float _y, float _z) : Vector2(_x, _y), z(_z) {}
	Vector3(const POINT& _point) : Vector2((float)_point.x, (float)_point.y), z(0) {}
	Vector3(const Vector3& _vec) : Vector2(_vec.x, _vec.y), z(_vec.z){}

	void Set(float _x, float _y) { Vector2::Set(_x, _y); }
	void Set(int _x, int _y) { Vector2::Set((float)_x, (float)_y); }
	void Set(float _x, float _y, float _z) { Vector2::Set(_x, _y); z = _z; }
	void Set(int _x, int _y, int _z) { Vector2::Set((float)_x, (float)_y); z = (float)_z; }
	void Set(const Vector3& _vec) { *this = _vec; }
	
	void Normalized() {
		float w = GetDistance();
		if (w == 0) Set(0, 0, 0);
		else Set(x / w, y / w, z / w);
	}

	float GetDistance() {
		return sqrtf(x * x + y * y + z * z);
	}
	float GetDistance(const Vector3& _vec1) {
		Vector3 gap(_vec1.x - x, _vec1.y -y, _vec1.z -z);
		return sqrtf(gap.x * gap.x + gap.y * gap.y + gap.z * +gap.z);
	}

	Vector3 operator=(const Vector3& _rhs) {
		x = _rhs.x;
		y = _rhs.y;
		z = _rhs.z;
		return *this;
	}
	Vector3 operator=(const Vector2& _rhs) {
		x = _rhs.x;
		y = _rhs.y;
		return *this;
	}
	Vector3 operator=(const POINT& _rhs) {
		x = (float)_rhs.x;
		y = (float)_rhs.y;
		return *this;
	}
	Vector3 operator*(const float _value) {
		return Vector3(x * _value, y * _value, z * _value);
	}
	Vector3 operator+(const Vector3& rhs) const {
		return Vector3{ x + rhs.x, y + rhs.y, z + rhs.z };
	}
	Vector3 operator+(const float& rhs) const {
		return Vector3(x + rhs, y + rhs, z + rhs);
	}
	Vector3 operator-(const Vector3& rhs) const {
		return Vector3{ x - rhs.x, y - rhs.y, z - rhs.z };
	}
	Vector3 operator-(const float& rhs) const {
		return Vector3(x - rhs, y - rhs, z - rhs);
	}
	Vector3 operator/=(const float& rhs) {
		x /= rhs;
		y /= rhs;
		z /= rhs;
		return *this;
	}
	Vector3 operator/(const float& rhs) const {
		return Vector3(x / rhs, y / rhs, z / rhs);
	}
	Vector3 operator+=(const Vector3& rhs) {
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}
	Vector3 operator-=(const Vector3& rhs) {
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}
	Vector3 operator+=(const float& rhs) {
		x += rhs;
		y += rhs;
		z += rhs;
		return *this;
	}
	Vector3 operator-=(const float& rhs) {
		x -= rhs;
		y -= rhs;
		z -= rhs;
		return *this;
	}

	bool operator==(const Vector3& rhs) const {
		return (x == rhs.x && y == rhs.y && z == rhs.z);
	}
	bool operator!=(const Vector3& rhs) const {
		return (!(*this == rhs));
	}
};

///////////////// Matrix ////////////////

class Matrix4x4 {
public:
	float m_afElements[4][4];

	Matrix4x4() {
		SetZero();
	}
	Matrix4x4 SetZero() {
		memset(m_afElements, 0, sizeof(m_afElements));
		return *this;
	}

	Matrix4x4 SetIdentityMatrix4x4() {
		SetZero();
		m_afElements[0][0] = m_afElements[1][1] = m_afElements[2][2] = m_afElements[3][3] = 1.f;
		return *this;
	}

	float& operator() (int iRow, int iCol) {
		return m_afElements[iRow][iCol];
	}

	Matrix4x4 operator* (Matrix4x4& mRight) {
		Matrix4x4 mRet;
		mRet.SetZero();

		for (WORD i = 0; i < 4; ++i) {
			for (WORD j = 0; j < 4; ++j) {
				for (WORD k = 0; k < 4; ++k) {
					mRet(i, j) += m_afElements[i][k] * mRight(k, j);
				}
			}
		}
		return mRet;
	}

	Vector3 operator*(Vector3& vLeft) {
		Vector3 vRet;
		vRet.x =
			vLeft.x * m_afElements[0][0] +
			vLeft.y * m_afElements[0][1] +
			vLeft.z * m_afElements[0][2] +
			m_afElements[0][3];
		vRet.y =
			vLeft.x * m_afElements[1][0] +
			vLeft.y * m_afElements[1][1] +
			vLeft.z * m_afElements[1][2] +
			m_afElements[1][3];
		vRet.z =
			vLeft.x * m_afElements[2][0] +
			vLeft.y * m_afElements[2][1] +
			vLeft.z * m_afElements[2][2] +
			m_afElements[2][3];
		return vRet;
	}

	Matrix4x4 operator+(Matrix4x4& mRight) {
		Matrix4x4 mRet;
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				mRet(i, j) = m_afElements[i][j] + mRight(i, j);
			}
		}
	}

	Matrix4x4 operator=(Matrix4x4& mRight) {
		memcpy(m_afElements, mRight.m_afElements, sizeof(m_afElements));
		return *this;
	}

	Matrix4x4 SetProjection(float d) {
		SetZero();
		m_afElements[0][0] = d;
		m_afElements[1][1] = d;
		m_afElements[2][2] = 1;
		m_afElements[3][3] = d;

		return *this;
	}
};

// 매트릭스를 출력한다.
inline void PrintMatrix(Matrix4x4& m) {

}

// 단위행렬
inline void SetIdentityMatrix4x4(Matrix4x4& m) {
	memset(m.m_afElements, 0, sizeof(m.m_afElements));
	m(0, 0) = m(1, 1) = m(2, 2) = m(3, 3) = 1.0f;
}

// 행렬 덧셈
void MatrixPlus(Matrix4x4& q, Matrix4x4& a, Matrix4x4& b) {
	for (WORD i = 0; i < 4; ++i) {
		for (WORD j = 0; j < 4; ++j) {
			q(i, j) = a(i, j) + b(i, j);
		}
	}
}

// 행렬 뺄셈
void MatrixMinus(Matrix4x4& q, Matrix4x4& a, Matrix4x4& b) {
	for (WORD i = 0; i < 4; ++i) {
		for (WORD j = 0; j < 4; ++j) {
			q(i, j) = a(i, j) - b(i, j);
		}
	}
}

// 행렬 곱셈
void MatrixMultiply(Matrix4x4& q, Matrix4x4& a, Matrix4x4& b) {
	for (WORD i = 0; i < 4; ++i) {
		for (WORD j = 0; j < 4; ++j) {
			for (WORD k = 0; k < 4; ++k) {
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



class CMyMath {
public:
	static float GetDistance(const Vector3& v1, const Vector3& v2) {
		Vector3 gap;
		gap.x = v1.x - v2.x;
		gap.y = v1.y - v2.y;
		gap.z = v1.z - v2.z;

		return sqrtf(gap.x * gap.x + gap.y * gap.y + gap.z * gap.z);
	}

	static float GetSqrMagnitude(const Vector3& v1, const Vector3& v2) {
		Vector3 gap;
		gap.x = v1.x - v2.x;
		gap.y = v1.y - v2.y;
		gap.z = v1.z - v2.z;
		return gap.x * gap.x + gap.y * gap.y + gap.z * gap.z;
	}

	//벡터를 각도로 변환 
	static float VectorToDegree(Vector2 vector) { 
		float radian = atan2f(vector.y, vector.x);
		return (radian * 180.0f / PI);
	} 

	//벡터를 라디안으로 변환
	static float VectorToRadian(Vector2 vector) {
		return atan2f(vector.y, vector.x);
	}

	//라디안을 각도로 변환
	static float RadianToDegree(float radian) {
		return (radian * 180.0f / PI);
	} 

	//각도를 라디안으로 변환 
	static float DegreeToRadian(float degree) {
		return (PI / 180.0f) * degree;
	}

	// 외적
	Vector3 GetCross(const Vector3& v1, const Vector3& v2) {
		Vector3 p;
		p.x = (v1.y * v2.z - v1.z * v2.y);
		p.y = (v1.z * v2.x - v1.x * v2.z);
		p.z = (v1.x * v2.y - v1.y * v2.x);
		return p;
	}

	// 내적 Vector3
	float GetDot(const Vector3& v1, const Vector3& v2) {
		float scala = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
		return scala;
	}

	// 내적 Vector2
	float GetDot(const Vector2& v1, const Vector2& v2) {
		float scala = v1.x * v2.x + v1.y * v2.y;
		return scala;
	}
};
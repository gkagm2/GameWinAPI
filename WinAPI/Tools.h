#pragma once
class Matrix4x4;
struct Vector2;
struct Vector3;

////////////////// Vector ///////////////////
struct Vector2 {
	float x;
	float y;

	Vector2() :x(0), y(0) {}
	Vector2(float _x, float _y) : x(_x), y(_y) {}
	Vector2(const POINT& _point) : x((float)_point.x), y((float)_point.y) {}
	Vector2(const Vector2& _vec) : x(_vec.x), y(_vec.y) {}

	void Set(float _x, float _y) { x = _x, y = _y; }
	void Set(int _x, int _y) { x = (float)_x, y = (float)_y; }

	void Normalized() {
		float w = GetDistance();
		if (w == 0) Set(0, 0);
		else Set(x / w, y / w);
	}

	float GetDistance() {
		return sqrtf(x * x + y * y);
	}

	float GetDistance(const Vector2& _vec1) {
		Vector2 gap(_vec1.x - x, _vec1.y - y);
		return sqrtf(gap.x * gap.x + gap.y * gap.y);
	}

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

struct Vector3 : public Vector2 {
	float z;

	Vector3() : Vector2(), z(0) {}
	Vector3(float _x, float _y) : Vector2(_x, _y), z(0) {}
	Vector3(float _x, float _y, float _z) : Vector2(_x, _y), z(_z) {}
	Vector3(const POINT& _point) : Vector2((float)_point.x, (float)_point.y), z(0) {}
	Vector3(const Vector3& _vec) : Vector2(_vec.x, _vec.y), z(_vec.z) {}

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
		Vector3 gap(_vec1.x - x, _vec1.y - y, _vec1.z - z);
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
		std::memset(m_afElements, 0, sizeof(m_afElements));
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

	float operator() (int iRow, int iCol) const {
		return m_afElements[iRow][iCol];
	}

	Matrix4x4 operator* (Matrix4x4& mRight) {
		Matrix4x4 mRet;
		mRet.SetZero();

		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				for (int k = 0; k < 4; ++k) {
					mRet(i, j) += m_afElements[i][k] * mRight(k, j);
				}
			}
		}
		return mRet;
	}

	Vector3 operator*(Vector3& vLeft) const {
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

	Matrix4x4 operator+(Matrix4x4& mRight) const {
		Matrix4x4 mRet;
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				mRet(i, j) = m_afElements[i][j] + mRight(i, j);
			}
		}
	}

	Matrix4x4 operator-(Matrix4x4& mRight) const {
		Matrix4x4 mRet;
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {

			}
		}
	}

	Matrix4x4 operator=(Matrix4x4& mRight) {
		std::memcpy(m_afElements, mRight.m_afElements, sizeof(m_afElements));
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

	//���͸� ������ ��ȯ 
	static float VectorToDegree(Vector2 vector) {
		float radian = atan2f(vector.y, vector.x);
		return (radian * 180.0f / PI);
	}

	//���͸� �������� ��ȯ
	static float VectorToRadian(Vector2 vector) {
		return atan2f(vector.y, vector.x);
	}

	//������ ������ ��ȯ
	static float RadianToDegree(float radian) {
		// 1���� = 180�� / PI -> 57.29578��
		return radian * (180.0f / PI);
	}
	static float Rad2Deg() { return 57.29578f; } // ������ ������

	//������ �������� ��ȯ 
	static float DegreeToRadian(float degree) {
		// 1�� = PI / 180�� -> 0.017453924����
		return degree * (PI / 180.0f);
	}
	static float Deg2Rad() { return 0.0174532924f; } // ������ ��������

	// ����
	static Vector3 GetCross(const Vector3& v1, const Vector3& v2) {
		Vector3 p;
		p.x = (v1.y * v2.z - v1.z * v2.y);
		p.y = (v1.z * v2.x - v1.x * v2.z);
		p.z = (v1.x * v2.y - v1.y * v2.x);
		return p;
	}

	// ���� Vector3
	static float GetDot(const Vector3& v1, const Vector3& v2) {
		float scala = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
		return scala;
	}

	// ���� Vector2
	static float GetDot(const Vector2& v1, const Vector2& v2) {
		float scala = v1.x * v2.x + v1.y * v2.y;
		return scala;
	}
};

// ������ ������ä�� ������ ũ��� ����� ����.
Vector2 ResizeScaleMaintainRatio(float _fitWidth, float _fitHeight, float _width, float _height);

// ���� ȸ�� (2D Rotate)
Vector3 Rotate(const Vector3& _vVec, float _degree);

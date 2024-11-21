#pragma once

#include <cmath>
#include <limits>
#include <string>
#include "../DirectX/ImGui/imgui.h"
#include <Windows.h>
#include <stdio.h>

#include <assert.h>

#include <windef.h>
#include <stdint.h>

#include <cmath>
#include <cfloat>

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#define M_PI_F (float)M_PI
#endif

struct ViewMatrix
{
	constexpr float* operator[](int index) noexcept
	{
		return data[index];
	}

	constexpr const float* operator[](int index) const noexcept
	{
		return data[index];
	}

	float data[4][4] = { };
};

class Matrix3x3;
class Matrix4x4;
class Quaternion;

struct ProjectionInfo;
struct IntersectionResult;
class Vector2
{

public:
	float x, y;

	Vector2() = default;
	Vector2(float xx, float yy) : x(xx), y(yy) {}
	operator float* ();

	operator ImVec2() const { return ImVec2(this->x, this->y); }

	Vector2& operator+=(const Vector2& v);
	Vector2& operator+=(float fl);
	Vector2 operator+(const Vector2& v) const;
	Vector2 operator+(float mod) const;

	Vector2& operator-=(const Vector2& v);
	Vector2& operator-=(float fl);
	Vector2 operator-(const Vector2& v) const;
	Vector2 operator-(float mod) const;

	Vector2& operator*=(const Vector2& v);
	Vector2& operator*=(float s);
	Vector2 operator*(const Vector2& v) const;
	Vector2 operator*(float mod) const;

	Vector2& operator/=(const Vector2& v);
	Vector2& operator/=(float s);
	Vector2 operator/(const Vector2& v) const;
	Vector2 operator/(float mod) const;
};
class Vector3
{
public:
	float x, y, z;

	Vector3(float xx, float yy, float zz) : x(xx), y(yy), z(zz) {}
	Vector3();
	operator float* ();

	bool IsValid() const;
	Vector3 toGround() const;
	bool operator==(const Vector3& other) const;
	bool operator!=(const Vector3& other) const;
	/*Vector3& operator/=(const Vector3& v);
	Vector3& operator/=(float fl);*/
	bool IsZero(float tolerance = 0.01f) const;

	float DistanceLine(Vector3 segmentStart, Vector3 segmentEnd, bool onlyIfOnSegment, bool squared);

	float getX() const;
	float getY() const;
	float getZ() const;

	float Distance(const Vector3& v) const;

	float distanceTo(const Vector3& v) const;
	float LengthSquared() const;
	float Distance(Vector3 const& segment_start, Vector3 const& segment_end, bool only_if_on_segment = false, bool squared = false) const;
	float DistanceSquared(Vector3 const& to) const;

	operator Vector2() const { return Vector2(this->x, this->y); }
	operator ImVec2() const { return ImVec2(this->x, this->y); }
	operator std::string() { return ("x: " + std::to_string(this->x) + ", y: " + std::to_string(this->x) + ", z: " + std::to_string(this->z)); }

	Vector3& operator*=(const Vector3& v);
	Vector3& operator*=(float s);

	Vector3& operator/=(const Vector3& v);
	Vector3& operator/=(float s);

	Vector3& operator+=(const Vector3& v);
	Vector3& operator+=(float fl);

	Vector3& operator-=(const Vector3& v);
	Vector3& operator-=(float fl);

	Vector3 operator-(const Vector3& v) const;
	Vector3 operator-(float mod) const;
	Vector3 operator+(const Vector3& v) const;
	Vector3 operator+(float mod) const;

	Vector3 operator/(const Vector3& v) const;
	Vector3 operator/(float mod) const;
	Vector3 operator*(const Vector3& v) const;
	Vector3 operator*(float mod) const;

	Vector3& operator=(const Vector3& v);

	Vector3& SwitchYZ();
	Vector3& Negate();

	float Length() const;
	Vector3 Rotate_x(float angle) const;
	Vector3 Rotate_y(float angle) const;
	Vector3 Normalized() const;
	float NormalizeInPlace() const;

	float DotProduct(Vector3 const& other) const;
	float Magnitude() const;
	float CrossProduct(Vector3 const& other) const;
	float Polar() const;
	float AngleBetween(Vector3 const& other) const;

	Vector3 Cross(Vector3 vector2);

	bool Close(float a, float b, float eps) const;

	Vector3 Transform(Matrix3x3 matrix);
	Vector3 Transform(Matrix4x4 matrix);
	Vector3 Transform(Quaternion quat);
	Vector3 Rotated(float angle) const;
	Vector3 Perpendicular() const;
	Vector3 Perpendicular2() const;
	Vector3 Extend(Vector3 const& to, float distance) const;

	Vector3 Append(Vector3 pos1, Vector3 pos2, float dist) const;
	Vector3 Prepend(Vector3 pos1, Vector3 pos2, float dist) const;

	ProjectionInfo ProjectOn(Vector3 const& segment_start, Vector3 const& segment_end) const;
	IntersectionResult Intersection(Vector3 const& line_segment_end, Vector3 const& line_segment2_start, Vector3 const& line_segment2_end) const;

	Vector3 Scale(float s)
	{
		return Vector3(x * s, y * s, z * s);
	}

	Vector3 Rotate(Vector3 startPos, float theta)
	{
		float dx = this->x - startPos.x;
		float dz = this->z - startPos.z;

		float px = dx * cos(theta) - dz * sin(theta);
		float pz = dx * sin(theta) + dz * cos(theta);
		return { px + startPos.x, this->y, pz + startPos.z };
	}

	std::string ToString() {
		return std::to_string(this->x) + "\n" + std::to_string(this->y) + "\n" + std::to_string(this->z);
	}
};

class DVector3
{
public:
	double x, y, z;

	static Vector3 toVec3(const DVector3& vec) { return Vector3(vec.x, vec.y, vec.z); }

	//operator Vector3() const { return Vector3(x, y, z); }
	DVector3(const Vector3& v) : x(v.x), y(v.y), z(v.z) {}


	DVector3(double xx, double yy, double zz) : x(xx), y(yy), z(zz) {}
	DVector3();

	operator double* ();

	bool IsValid() const;
	DVector3 toGround() const;
	bool operator==(const DVector3& other) const;
	bool operator!=(const DVector3& other) const;
	/*DVector3& operator/=(const DVector3& v);
	DVector3& operator/=(double fl);*/
	bool IsZero(double tolerance = 0.01f) const;

	double DistanceLine(DVector3 segmentStart, DVector3 segmentEnd, bool onlyIfOnSegment, bool squared);

	double getX() const;
	double getY() const;
	double getZ() const;

	double Distance(const DVector3& v) const;

	double distanceTo(const DVector3& v) const;
	double LengthSquared() const;
	double Distance(DVector3 const& segment_start, DVector3 const& segment_end, bool only_if_on_segment = false, bool squared = false) const;
	double DistanceSquared(DVector3 const& to) const;

	operator Vector2() const { return Vector2(this->x, this->y); }
	operator ImVec2() const { return ImVec2(this->x, this->y); }
	operator std::string() { return ("x: " + std::to_string(this->x) + ", y: " + std::to_string(this->x) + ", z: " + std::to_string(this->z)); }

	DVector3& operator*=(const DVector3& v);
	DVector3& operator*=(double s);

	DVector3& operator/=(const DVector3& v);
	DVector3& operator/=(double s);

	DVector3& operator+=(const DVector3& v);
	DVector3& operator+=(double fl);

	DVector3& operator-=(const DVector3& v);
	DVector3& operator-=(double fl);

	DVector3 operator-(const DVector3& v) const;
	DVector3 operator-(double mod) const;
	DVector3 operator+(const DVector3& v) const;
	DVector3 operator+(double mod) const;

	DVector3 operator/(const DVector3& v) const;
	DVector3 operator/(double mod) const;
	DVector3 operator*(const DVector3& v) const;
	DVector3 operator*(double mod) const;

	DVector3& operator=(const DVector3& v);

	DVector3& SwitchYZ();
	DVector3& Negate();

	double Length() const;
	DVector3 Rotate_x(double angle) const;
	DVector3 Rotate_y(double angle) const;
	DVector3 Normalized() const;
	double NormalizeInPlace() const;

	double DotProduct(DVector3 const& other) const;
	double Magnitude() const;
	double CrossProduct(DVector3 const& other) const;
	double Polar() const;
	double AngleBetween(DVector3 const& other) const;

	DVector3 Cross(DVector3 vector2);

	bool Close(double a, double b, double eps) const;

	DVector3 Transform(Matrix3x3 matrix);
	DVector3 Transform(Matrix4x4 matrix);
	DVector3 Transform(Quaternion quat);
	DVector3 Rotated(double angle) const;
	DVector3 Perpendicular() const;
	DVector3 Perpendicular2() const;
	DVector3 Extend(DVector3 const& to, double distance) const;

	DVector3 Append(DVector3 pos1, DVector3 pos2, double dist) const;
	DVector3 Prepend(DVector3 pos1, DVector3 pos2, double dist) const;

	ProjectionInfo ProjectOn(DVector3 const& segment_start, DVector3 const& segment_end) const;
	IntersectionResult Intersection(DVector3 const& line_segment_end, DVector3 const& line_segment2_start, DVector3 const& line_segment2_end) const;

	DVector3 Scale(double s)
	{
		return DVector3(x * s, y * s, z * s);
	}

	DVector3 Rotate(DVector3 startPos, double theta)
	{
		double dx = this->x - startPos.x;
		double dz = this->z - startPos.z;

		double px = dx * cos(theta) - dz * sin(theta);
		double pz = dx * sin(theta) + dz * cos(theta);
		return { px + startPos.x, this->y, pz + startPos.z };
	}

	std::string ToString() {
		return std::to_string(this->x) + "\n" + std::to_string(this->y) + "\n" + std::to_string(this->z);
	}
};

struct ProjectionInfo
{
	bool IsOnSegment;
	Vector3 LinePoint;
	Vector3 SegmentPoint;

	ProjectionInfo(bool is_on_segment, Vector3 const& segment_point, Vector3 const& line_point);
};

struct IntersectionResult
{
	bool Intersects;
	Vector3 Point;

	IntersectionResult(bool intersects = false, Vector3 const& point = Vector3());
};

struct Vector4 {
	Vector4() {};
	Vector4(float _x, float _y, float _z, float _w) {
		x = _x;
		y = _y;
		z = _z;
		w = _w;
	}

	float x;
	float y;
	float z;
	float w;

	float length() {
		return sqrt(x * x + y * y + z * z + w * w);
	}

	float distance(const Vector4& o) {
		return sqrt(pow(x - o.x, 2) + pow(y - o.y, 2) + pow(z - o.z, 2) + pow(w - o.w, 2));
	}

	Vector4 vscale(const Vector4& s) {
		return Vector4(x * s.x, y * s.y, z * s.z, w * s.w);
	}

	Vector4 scale(float s) {
		return Vector4(x * s, y * s, z * s, w * s);
	}

	Vector4 normalize() {
		float l = length();
		return Vector4(x / l, y / l, z / l, w / l);
	}

	Vector4 add(const Vector4& o) {
		return Vector4(x + o.x, y + o.y, z + o.z, w + o.w);
	}

	Vector4 sub(const Vector4& o) {
		return Vector4(x - o.x, y - o.y, z - o.z, w - o.w);
	}

	Vector4 clone() {
		return Vector4(x, y, z, w);
	}
};

#pragma region manth stuff

inline float clamp(float X, float Min, float Max)
{
	X = (X + Max - fabsf(X - Max)) * 0.5f;
	X = (X + Min + fabsf(X - Min)) * 0.5f;

	return X;
}

inline float isqrt_tpl(float op) { return 1.0f / sqrt(op); }
inline float fabs_tpl(float op) { return fabs(op); }
inline void cry_sincos(float angle, float* pSin, float* pCos) { *pSin = (sin(angle));  *pCos = (cos(angle)); }
inline void sincos(float angle, float* pSin, float* pCos) { cry_sincos(angle, pSin, pCos); }
inline float isqrt_safe_tpl(float op) { return 1.0f / sqrt(op + (float)DBL_MIN); }

inline float asin_tpl(float op) { return asin(clamp(op, -1.0f, +1.0f)); }
static float g_PI = 3.14159265358979323846264338327950288419716939937510f;
inline float atan2_tpl(float op1, float op2) { return atan2(op1, op2); }

#define FloatU32ExpMask (0xFF << 23)
inline int FloatU32(const float x)
{
	union { int ui; float f; } cvt;
	cvt.f = x;

	return cvt.ui;
}

inline bool NumberValid(const float& x)
{
	int i = FloatU32(x);
	int expmask = (0xFF << 23);
	int iexp = i & expmask;
	bool invalid = (iexp == expmask);

	if (invalid)
	{
		int i = 0x7F800001;
		float fpe = *(float*)(&i);
	}

	return !invalid;
}

struct Matrix3x3
{
	constexpr float* operator[](int index) noexcept
	{
		return data[index];
	}

	constexpr const float* operator[](int index) const noexcept
	{
		return data[index];
	}

	float data[3][3] = { };
};

struct Matrix4x4
{
	constexpr float* operator[](int index) noexcept
	{
		return data[index];
	}

	constexpr const float* operator[](int index) const noexcept
	{
		return data[index];
	}

	float data[4][4] = { };

	operator Matrix3x3() {
		Matrix3x3 mat;
		auto matr = *this;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				mat[i][j] = matr[i][j];
			}
		}
		return mat;
	}

	static Matrix4x4 From3x3(Matrix3x3 matrix) {
		Matrix4x4 newMat;
		for (int i = 0; i < 4; i++) {
			newMat[i][3] = 1.f;
		}
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				newMat[i][j] = matrix[i][j];
			}
		}
		return newMat;
	}
};

class Quaternion
{
public:

	Quaternion();
	~Quaternion();

	Matrix4x4 CreateMatrix();
	static Quaternion CreateFromAxisAngle(Vector3 vec, float deg) {
		return Quaternion::CreateFromAxisAngle(vec.x, vec.y, vec.z, deg);
	}
	static Quaternion CreateFromAxisAngle(const float& in_x,
		const float& in_y,
		const float& in_z,
		const float& in_degrees);

	static Quaternion Identity()
	{
		Quaternion quat;
		quat.x = 0;
		quat.y = 0;
		quat.z = 0;
		quat.w = 1;
		return quat;
	}

	Quaternion operator *(const Quaternion& q);
	operator Matrix4x4() { return this->CreateMatrix(); }
	operator Matrix3x3() { return (Matrix3x3)this->CreateMatrix(); }
	float x, y, z, w;

};

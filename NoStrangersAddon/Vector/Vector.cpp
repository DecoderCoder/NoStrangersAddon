#include "Vector.h"
#include <cmath>

Vector2::operator float* ()
{
	return &x;
}

Vector2& Vector2::operator+=(const Vector2& v)
{
	this->x += v.x;
	this->y += v.y;
	return *this;
}

Vector2& Vector2::operator+=(float fl)
{
	this->x += fl;
	this->y += fl;
	return *this;
}

Vector2 Vector2::operator+(const Vector2& v) const
{
	Vector2 result(this->x + v.x, this->y + v.y);
	return result;
}

Vector2 Vector2::operator+(float mod) const
{
	Vector2 result(this->x + mod, this->y + mod);
	return result;
}

Vector2& Vector2::operator-=(const Vector2& v)
{
	this->x -= v.x;
	this->y -= v.y;
	return *this;
}

Vector2& Vector2::operator-=(float fl)
{
	this->x -= fl;
	this->y -= fl;
	return *this;
}

Vector2 Vector2::operator-(const Vector2& v) const
{
	Vector2 result(this->x - v.x, this->y - v.y);
	return result;
}

Vector2 Vector2::operator-(float mod) const
{
	Vector2 result(this->x - mod, this->y - mod);
	return result;
}

Vector2& Vector2::operator*=(const Vector2& v)
{
	this->x *= v.x;
	this->y *= v.y;
	return *this;
}

Vector2& Vector2::operator*=(float s)
{
	this->x *= s;
	this->y *= s;
	return *this;
}

Vector2 Vector2::operator*(const Vector2& v) const
{
	Vector2 result(this->x * v.x, this->y * v.y);
	return result;
}

Vector2 Vector2::operator*(float mod) const
{
	Vector2 result(this->x * mod, this->y * mod);
	return result;
}

Vector2& Vector2::operator/=(const Vector2& v)
{
	this->x /= v.x;
	this->y /= v.y;
	return *this;
}

Vector2& Vector2::operator/=(float s)
{
	this->x /= s;
	this->y /= s;
	return *this;
}

Vector2 Vector2::operator/(const Vector2& v) const
{
	Vector2 result(this->x / v.x, this->y / v.y);
	return result;
}

Vector2 Vector2::operator/(float mod) const
{
	Vector2 result(this->x / mod, this->y / mod);
	return result;
}
Vector3::Vector3()
{
	this->x = 0; this->y = 0; this->z = 0;
}

bool Vector3::operator==(Vector3 const& other) const
{
	return other.x == this->x && other.y == this->y && other.z == this->z;
}

bool Vector3::operator!=(Vector3 const& other) const
{
	return other.x != this->x || other.y != this->y || other.z != this->z;
}

/*Vector3& Vector3::operator/=(const Vector3& v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;
	return *this;
}
Vector3& Vector3::operator/=(float fl)
{
	x /= fl;
	y /= fl;
	z /= fl;
	return *this;
}*/

bool Vector3::IsValid() const
{
	return this->x != 0/* && this->y != 0*/ && this->z != 0;
}

Vector3 Vector3::toGround() const
{
	Vector3 result(this->x, 0, this->z);
	return result;
}

bool Vector3::IsZero(float tolerance) const
{
	return this->x > -tolerance && this->x < tolerance &&
		this->z > -tolerance && this->z < tolerance;
}

float Vector3::DistanceLine(Vector3 segmentStart, Vector3 segmentEnd, bool onlyIfOnSegment, bool squared)
{
	auto objects = this->ProjectOn(segmentStart, segmentEnd);
	if (objects.IsOnSegment || onlyIfOnSegment == false)
	{
		return squared ? objects.SegmentPoint.DistanceSquared(Vector3(this->x, this->y, this->z)) : objects.SegmentPoint.distanceTo(Vector3(this->x, this->y, this->z));
	}
	return FLT_MAX;
}

Vector3::operator float* ()
{
	return &x;
}

float Vector3::getX() const
{
	return x;
}

float Vector3::getY() const
{
	return y;
}

float Vector3::getZ() const
{
	return z;
}

float Vector3::Distance(const Vector3& v) const
{
	return (float)sqrt(pow(v.x - x, 2) + pow(v.z - z, 2) + pow(v.y - y, 2));
}

float Vector3::distanceTo(const Vector3& v) const
{
	return (float)sqrt(pow(v.x - x, 2) + pow(v.z - z, 2) + pow(v.y - y, 2));
}

float Vector3::LengthSquared() const
{
	return this->x * this->x + this->y * this->y + this->z * this->z;
}

float Vector3::Distance(Vector3 const& segment_start, Vector3 const& segment_end, bool only_if_on_segment,
	bool squared) const
{
	auto const projection_info = this->ProjectOn(segment_start, segment_end);

	if (projection_info.IsOnSegment || !only_if_on_segment)
	{
		return squared
			? this->DistanceSquared(projection_info.SegmentPoint)
			: this->distanceTo(projection_info.SegmentPoint);
	}
	return FLT_MAX;
}

float Vector3::DistanceSquared(Vector3 const& to) const
{
	Vector3 delta;

	delta.x = x - to.x;
	delta.y = y - to.y;
	delta.y = z - to.z;

	return delta.LengthSquared();
}

Vector3& Vector3::operator*=(const Vector3& v)
{
	this->x *= v.x;
	this->y *= v.y;
	this->z *= v.z;
	return *this;
}

Vector3& Vector3::operator*=(float s)
{
	this->x *= s;
	this->y *= s;
	this->z *= s;
	return *this;
}

Vector3& Vector3::operator/=(const Vector3& v)
{
	this->x /= v.x;
	this->y /= v.y;
	this->z /= v.z;
	return *this;
}

Vector3& Vector3::operator/=(float s)
{
	this->x /= s;
	this->y /= s;
	this->z /= s;
	return *this;
}

Vector3& Vector3::operator+=(const Vector3& v)
{
	this->x += v.x;
	this->y += v.y;
	this->z += v.z;
	return *this;
}

Vector3& Vector3::operator+=(float fl)
{
	this->x += fl;
	this->y += fl;
	this->z += fl;
	return *this;
}

Vector3& Vector3::operator-=(const Vector3& v)
{
	this->x -= v.x;
	this->y -= v.y;
	this->z -= v.z;
	return *this;
}

Vector3& Vector3::operator-=(float fl)
{
	this->x -= fl;
	this->y -= fl;
	this->z -= fl;
	return *this;
}

Vector3 Vector3::operator-(const Vector3& v) const
{
	Vector3 result(this->x - v.x, this->y - v.y, this->z - v.z);
	return result;
}

Vector3 Vector3::operator-(float mod) const
{
	Vector3 result(this->x - mod, this->y - mod, this->z - mod);
	return result;
}

Vector3 Vector3::operator+(const Vector3& v) const
{
	Vector3 result(this->x + v.x, this->y + v.y, this->z + v.z);
	return result;
}

Vector3 Vector3::operator+(float mod) const
{
	Vector3 result(this->x + mod, this->y + mod, this->z + mod);
	return result;
}

Vector3 Vector3::operator/(const Vector3& v) const
{
	Vector3 result(this->x / v.x, this->y / v.y, this->z / v.z);
	return result;
}

Vector3 Vector3::operator/(float mod) const
{
	Vector3 result(this->x / mod, this->y / mod, this->z / mod);
	return result;
}

Vector3 Vector3::operator*(const Vector3& v) const
{
	Vector3 result(this->x * v.x, this->y * v.y, this->z * v.z);
	return result;
}

Vector3 Vector3::operator*(float mod) const
{
	Vector3 result(this->x * mod, this->y * mod, this->z * mod);
	return result;
}

Vector3& Vector3::operator=(const Vector3& v) = default;

Vector3& Vector3::SwitchYZ()
{
	const auto temp = this->y;

	this->y = this->z;
	this->z = temp;
	return *this;
}

Vector3& Vector3::Negate()
{
	this->x = -this->x;
	this->y = -this->y;
	this->z = -this->z;

	return *this;
}

float Vector3::Length() const
{
	return sqrtf(this->x * this->x + this->y * this->y + this->z * this->z);
}

Vector3 Vector3::Rotate_x(float angle) const {
	return Vector3(
		this->x * cos(angle) + this->z * sin(angle),
		this->y,
		-this->x * sin(angle) + this->z * cos(angle)
	);
}

Vector3 Vector3::Rotate_y(float angle) const {
	Vector3 result(this->x * cos(angle) - this->y * sin(angle), this->x * sin(angle) + this->y * cos(angle), this->z);
	return result;
}

Vector3 Vector3::Normalized() const
{
	float ls = this->x * this->x + this->y * this->y + this->z * this->z;
	float length = (float)sqrt(ls);
	return Vector3(this->x / length, this->y / length, this->z / length);
}
float Vector3::NormalizeInPlace() const
{
	auto v = *this;
	auto const l = this->Length();

	if (l != 0.0f)
	{
		v /= l;
	}
	else
	{
		v.x = v.z = 0.0f; v.y = 1.0f;
	}
	return l;
}

float Vector3::DotProduct(Vector3 const& other) const
{
	return this->x * other.x + this->y * other.y + this->z * other.z;
}

float Vector3::Magnitude() const
{
	return sqrt(pow(this->x, 2) + pow(this->y, 2) + pow(this->z, 2));
}

float Vector3::CrossProduct(Vector3 const& other) const
{
	return other.y * this->x - other.x * this->y;
}

float Vector3::Polar() const
{
	if (this->Close(x, 0.f, 0.f))
	{
		if (y > 0.f)
		{
			return 90.f;
		}
		return y < 0.f ? 270.f : 0.f;
	}

	auto theta = atan(y / x) * 180.f / M_PI;
	if (x < 0.f)
	{
		theta = theta + 180.f;
	}
	if (theta < 0.f)
	{
		theta = theta + 360.f;
	}
	return theta;
}

float Vector3::AngleBetween(Vector3 const& other) const
{
	float dot = this->DotProduct(other);
	float a = this->Magnitude();
	float b = other.Magnitude();

	float angle = acos(dot / (a * b));
	return angle;
}

Vector3 Vector3::Cross(Vector3 vector2)
{
	return Vector3(this->y * vector2.z - this->z * vector2.y,	this->z * vector2.x - this->x * vector2.z, this->x * vector2.y - this->y * vector2.x);
}

bool Vector3::Close(float a, float b, float eps) const
{
	if (abs(eps) < FLT_EPSILON)
	{
		eps = static_cast<float>(1e-9);
	}
	return abs(a - b) <= eps;
}

Vector3 Vector3::Transform(Matrix3x3 matrix) {
	return Vector3(
		this->x * matrix[0][0] + this->y * matrix[1][0] + this->z * matrix[2][0],
		this->x * matrix[0][1] + this->y * matrix[1][1] + this->z * matrix[2][1],
		this->x * matrix[0][2] + this->y * matrix[1][2] + this->z * matrix[2][2]);
}

Vector3 Vector3::Transform(Matrix4x4 matrix) {
	return Vector3(
		this->x * matrix[0][0] + this->y * matrix[1][0] + this->z * matrix[2][0],
		this->x * matrix[0][1] + this->y * matrix[1][1] + this->z * matrix[2][1],
		this->x * matrix[0][2] + this->y * matrix[1][2] + this->z * matrix[2][2]);
}

Vector3 Vector3::Transform(Quaternion quat)
{
	float x2 = quat.x + quat.x;
	float y2 = quat.y + quat.y;
	float z2 = quat.z + quat.z;

	float wx2 = quat.w * x2;
	float wy2 = quat.w * y2;
	float wz2 = quat.w * z2;
	float xx2 = quat.x * x2;
	float xy2 = quat.x * y2;
	float xz2 = quat.x * z2;
	float yy2 = quat.y * y2;
	float yz2 = quat.y * z2;
	float zz2 = quat.z * z2;

	return Vector3(
		this->x * (1.0f - yy2 - zz2) + this->y * (xy2 - wz2) + this->z * (xz2 + wy2),
		this->x * (xy2 + wz2) + this->y * (1.0f - xx2 - zz2) + this->z * (yz2 - wx2),
		this->x * (xz2 - wy2) + this->y * (yz2 + wx2) + this->z * (1.0f - xx2 - yy2)
	);

}

// Expects angle in radians!
Vector3 Vector3::Rotated(float angle) const
{
	auto const c = cos(angle);
	auto const s = sin(angle);

	return { static_cast<float>(x * c - z * s), y, static_cast<float>(z * c + x * s) };
}

Vector3 Vector3::Perpendicular() const
{
	return { -z,y,x };
}

Vector3 Vector3::Perpendicular2() const
{
	return { z,y,-x };
}

Vector3 Vector3::Extend(Vector3 const& to, float distance) const
{
	const auto from = *this;
	const auto result = from + (to - from).Normalized() * distance;
	return result;
}

Vector3 Vector3::Append(Vector3 pos1, Vector3 pos2, float dist) const
{
	return pos2 + (pos2 - pos1).Normalized() * dist;
}

Vector3 Vector3::Prepend(Vector3 pos1, Vector3 pos2, float dist) const
{
	return pos1 + (pos2 - pos1).Normalized() * dist;
	//return pos2 + (pos2 - pos1).Normalized() * dist;
}

ProjectionInfo::ProjectionInfo(const bool is_on_segment, Vector3 const& segment_point, Vector3 const& line_point) :
	IsOnSegment(is_on_segment), LinePoint(line_point), SegmentPoint(segment_point)
{
}

IntersectionResult::IntersectionResult(const bool intersects, Vector3 const& point) :
	Intersects(intersects), Point(point)
{
}

ProjectionInfo Vector3::ProjectOn(Vector3 const& segment_start, Vector3 const& segment_end) const
{
	float rs;
	float cx = x;
	float cz = z;
	float ax = segment_start.x;
	float az = segment_start.z;
	float bx = segment_end.x;
	float bz = segment_end.z;

	float cy = y;
	float ay = segment_start.y;
	float by = segment_end.y;

	float rl = float((cx - ax) * (bx - ax) + (cz - az) * (bz - az) + (cy - ay) * (by - ay)) / (pow(bx - ax, 2) + pow(bz - az, 2) + pow(by - ay, 2));
	Vector3 point_line = Vector3(ax + rl * (bx - ax), ay + rl * (by - ay), az + rl * (bz - az));

	//	const auto rl = ((cx - ax) * (bx - ax) + (cz - az) * (bz - az)) / (pow(bx - ax, 2) + pow(bz - az, 2));
	//	const auto point_line = Vector3(ax + rl * (bx - ax), 0, az + rl * (bz - az));

	if (rl < 0)
	{
		rs = 0;
	}
	else if (rl > 1)
	{
		rs = 1;
	}
	else
	{
		rs = rl;
	}

	bool is_on_segment = rs == rl;
	Vector3 point_segment = is_on_segment ? point_line : Vector3(ax + rs * (bx - ax), ay + rs * (by - ay), az + rs * (bz - az));
	//auto const point_segment = is_on_segment ? point_line : Vector3(ax + rs * (bx - ax), 0, az + rs * (bz - az));

	return ProjectionInfo(is_on_segment, point_segment, point_line);
}


IntersectionResult Vector3::Intersection(Vector3 const& line_segment_end, Vector3 const& line_segment2_start,
	Vector3 const& line_segment2_end) const
{
	Vector3 side1 = Vector3(line_segment_end.x - this->x, line_segment_end.y - this->y, line_segment_end.z - this->z);
	Vector3 side2 = Vector3(line_segment2_end.x - line_segment2_start.x, line_segment2_end.y - line_segment2_start.y, line_segment2_end.z - line_segment2_start.z);

	float s = (-side1.z * (this->x - line_segment2_start.x) + side1.x * (this->z - line_segment2_start.z)) / (-side2.x * side1.z + side1.x * side2.z);
	float t = (side2.x * (this->z - line_segment2_start.z) - side2.z * (this->x - line_segment2_start.x)) / (-side2.x * side1.z + side1.x * side2.z);

	if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
		return { true,{ this->x + t * side1.x,  this->y + t * side1.y, this->z + t * side1.z } };

	return { false,{} };
}


Quaternion::Quaternion()
	: x(0.0f), y(0.0f), z(0.0f), w(1.0f)
{

}

Quaternion::~Quaternion()
{

}

Quaternion Quaternion::CreateFromAxisAngle(const float& in_x, const float& in_y, const float& in_z, const float& angle)
{
	Quaternion ans;

	float halfAngle = angle * 0.5f;
	float s = (float)sin(halfAngle);
	float c = (float)cos(halfAngle);

	ans.x = in_x * s;
	ans.y = in_y * s;
	ans.z = in_z * s;
	ans.w = c;

	return ans;
}

Matrix4x4 Quaternion::CreateMatrix()
{
	Matrix4x4 pMatrix;

	// First row
	pMatrix[0][0] = 1.0f - 2.0f * (y * y + z * z);
	pMatrix[0][1] = 2.0f * (x * y - w * z);
	pMatrix[0][2] = 2.0f * (x * z + w * y);
	pMatrix[0][3] = 0.0f;

	// Second row
	pMatrix[1][0] = 2.0f * (x * y + w * z);
	pMatrix[1][1] = 1.0f - 2.0f * (x * x + z * z);
	pMatrix[1][2] = 2.0f * (y * z - w * x);
	pMatrix[1][3] = 0.0f;

	// Third row
	pMatrix[2][0] = 2.0f * (x * z - w * y);
	pMatrix[2][1] = 2.0f * (y * z + w * x);
	pMatrix[2][2] = 1.0f - 2.0f * (x * x + y * y);
	pMatrix[2][3] = 0.0f;

	// Fourth row
	pMatrix[3][0] = 0;
	pMatrix[3][1] = 0;
	pMatrix[3][2] = 0;
	pMatrix[3][3] = 1.0f;

	return pMatrix;
}

Quaternion Quaternion::operator *(const Quaternion& q)
{
	Quaternion ans;

	float q1x = this->x;
	float q1y = this->y;
	float q1z = this->z;
	float q1w = this->w;

	float q2x = q.x;
	float q2y = q.y;
	float q2z = q.z;
	float q2w = q.w;

	// cross(av, bv)
	float cx = q1y * q2z - q1z * q2y;
	float cy = q1z * q2x - q1x * q2z;
	float cz = q1x * q2y - q1y * q2x;

	float dot = q1x * q2x + q1y * q2y + q1z * q2z;

	ans.x = q1x * q2w + q2x * q1w + cx;
	ans.y = q1y * q2w + q2y * q1w + cy;
	ans.z = q1z * q2w + q2z * q1w + cz;
	ans.w = q1w * q2w - dot;

	return ans;
}

double DVector3::distanceTo(const DVector3& v) const
{
	return (double)sqrt(pow(v.x - x, 2) + pow(v.z - z, 2) + pow(v.y - y, 2));
}



DVector3& DVector3::operator*=(const DVector3& v)
{
	this->x *= v.x;
	this->y *= v.y;
	this->z *= v.z;
	return *this;
}

DVector3& DVector3::operator*=(double s)
{
	this->x *= s;
	this->y *= s;
	this->z *= s;
	return *this;
}

DVector3& DVector3::operator/=(const DVector3& v)
{
	this->x /= v.x;
	this->y /= v.y;
	this->z /= v.z;
	return *this;
}

DVector3& DVector3::operator/=(double s)
{
	this->x /= s;
	this->y /= s;
	this->z /= s;
	return *this;
}

DVector3& DVector3::operator+=(const DVector3& v)
{
	this->x += v.x;
	this->y += v.y;
	this->z += v.z;
	return *this;
}

DVector3& DVector3::operator+=(double fl)
{
	this->x += fl;
	this->y += fl;
	this->z += fl;
	return *this;
}

DVector3& DVector3::operator-=(const DVector3& v)
{
	this->x -= v.x;
	this->y -= v.y;
	this->z -= v.z;
	return *this;
}

DVector3& DVector3::operator-=(double fl)
{
	this->x -= fl;
	this->y -= fl;
	this->z -= fl;
	return *this;
}

DVector3 DVector3::operator-(const DVector3& v) const
{
	DVector3 result(this->x - v.x, this->y - v.y, this->z - v.z);
	return result;
}

DVector3 DVector3::operator-(double mod) const
{
	DVector3 result(this->x - mod, this->y - mod, this->z - mod);
	return result;
}

DVector3 DVector3::operator+(const DVector3& v) const
{
	DVector3 result(this->x + v.x, this->y + v.y, this->z + v.z);
	return result;
}

DVector3 DVector3::operator+(double mod) const
{
	DVector3 result(this->x + mod, this->y + mod, this->z + mod);
	return result;
}

DVector3 DVector3::operator/(const DVector3& v) const
{
	DVector3 result(this->x / v.x, this->y / v.y, this->z / v.z);
	return result;
}

DVector3 DVector3::operator/(double mod) const
{
	DVector3 result(this->x / mod, this->y / mod, this->z / mod);
	return result;
}

DVector3 DVector3::operator*(const DVector3& v) const
{
	DVector3 result(this->x * v.x, this->y * v.y, this->z * v.z);
	return result;
}

DVector3 DVector3::operator*(double mod) const
{
	DVector3 result(this->x * mod, this->y * mod, this->z * mod);
	return result;
}

DVector3& DVector3::operator=(const DVector3& v) = default;

double DVector3::Length() const
{
	return sqrtf(this->x * this->x + this->y * this->y + this->z * this->z);
}


DVector3 DVector3::Transform(Quaternion quat)
{
	double x2 = quat.x + quat.x;
	double y2 = quat.y + quat.y;
	double z2 = quat.z + quat.z;

	double wx2 = quat.w * x2;
	double wy2 = quat.w * y2;
	double wz2 = quat.w * z2;
	double xx2 = quat.x * x2;
	double xy2 = quat.x * y2;
	double xz2 = quat.x * z2;
	double yy2 = quat.y * y2;
	double yz2 = quat.y * z2;
	double zz2 = quat.z * z2;

	return DVector3(
		this->x * (1.0f - yy2 - zz2) + this->y * (xy2 - wz2) + this->z * (xz2 + wy2),
		this->x * (xy2 + wz2) + this->y * (1.0f - xx2 - zz2) + this->z * (yz2 - wx2),
		this->x * (xz2 - wy2) + this->y * (yz2 + wx2) + this->z * (1.0f - xx2 - yy2)
	);

}

DVector3::DVector3()
{
	this->x = 0; this->y = 0; this->z = 0;
}

#pragma warning(disable : 4819)
//
// Created by jun on 2016/11/20.
//
#include <string>
#include <map>
#include <vector>
#include <cassert>
#include <cmath>
#include "MatVec.h"

#define PI 3.1415926535

namespace m {
	/************/
	/* Vector2i */
	/************/
	/* =演算子 */
	//Vector2i &Vector2i::operator=(const Vector2i &rhs) {
	//	x = rhs.x;	y = rhs.y;
	//	return *this;
	//}

	/* 添え字演算子 */
	int Vector2i::operator[](int n) {
		assert(n >= 0 && n <= 1);
		if (0 == n)
			return x;
		else
			return y;
	}

	/* 単項演算子 */
	Vector2i& Vector2i::operator+=(const Vector2i &v) {
		this->x += v.x;	this->y += v.y;
		return *this;
	}
	Vector2i& Vector2i::operator-=(const Vector2i &v) {
		this->x -= v.x;	this->y -= v.y;
		return *this;
	}
	Vector2i& Vector2i::operator*=(int k) {
		this->x *= k;	this->y *= k;
		return *this;
	}
	Vector2i& Vector2i::operator/=(int k) {
		this->x /= k;		this->y /= k;
		return *this;
	}
	/* +Vector3f */
	Vector2i Vector2i::operator+() const {
		return *this;
	}
	/* -Vector3f */
	Vector2i Vector2i::operator-() const {
		return Vector2i(-x, -y);
	}
	int Vector2i::DotProduct(const Vector2i &v) {
		return this->x*v.x + this->y*v.y;
	}

	/* 比較演算子 */
	bool Vector2i::operator==(const Vector2i &v) const {
		return (x == v.x) && (y == v.y);
	}
	bool Vector2i::operator!=(const Vector2i& v) const {
		return !(*this == v);
	}

	/* べクトルの大きさ */
	int Vector2i::magnitude() const {
		return (int)sqrt(power());
	}
	int Vector2i::power() const {
		return x*x + y*y;
	}

	/* 正規化 */
	void Vector2i::normalize() {
		*this /= magnitude();
	}

	/* 2項演算子 */
	Vector2i operator+(const Vector2i &u, const Vector2i &v) {	//vector+vector
		Vector2i w;
		w.x = u.x + v.x;
		w.y = u.y + v.y;
		return w;
	}

	Vector2i operator-(const Vector2i &u, const Vector2i &v) {	//vector-vector
		Vector2i w;
		w.x = u.x - v.x;
		w.y = u.y - v.y;
		return w;
	}

	/* vector*scalar */
	Vector2i operator*(const Vector2i &v, int k) {
		Vector2i w;
		w.x = v.x * k;
		w.y = v.y * k;
		return w;
	}

	/* scalar*vector */
	Vector2i operator*(int k, const Vector2i &v) {
		Vector2i w;
		w.x = v.x * k;
		w.y = v.y * k;
		return w;
	}

	/* vector/scalar */
	Vector2i operator/(const Vector2i &v, int k) {
		Vector2i w;
		w.x = v.x / k;
		w.y = v.y / k;
		return w;
	}
	/* 内積 vector*vector */
	int operator*(const Vector2i &u, const Vector2i &v) {
		return u.x * v.x + u.y * v.y;
	}

	/* 内積 DotProduct(Vector3f,Vector3f) */
	int DotProduct(const Vector2i &u, const Vector2i &v) {
		return u.x*v.x + u.y*v.y;
	}
	/* 2つのベクトルのなす角 */
	int angle(const Vector2i &u, const Vector2i &v) {
		int cos = u*v / (u.magnitude()*v.magnitude());
		return int(acos(cos) / PI * 180);
	}

	/****************/
	/* Vector2f */
	/****************/
	/* =演算子 */
	Vector2f &Vector2f::operator=(const Vector2f &rhs) {
		x = rhs.x;	y = rhs.y;
		return *this;
	}

	/* 添え字演算子 */
	float Vector2f::operator[](int n) {
		assert(n >= 0 && n <= 1);
		if (0 == n)
			return x;
		else
			return y;
	}

	/* 単項演算子 */
	Vector2f &Vector2f::operator+=(const Vector2f &v) {
		this->x += v.x;	this->y += v.y;
		return *this;
	}
	Vector2f &Vector2f::operator-=(const Vector2f &v) {
		this->x -= v.x;	this->y -= v.y;
		return *this;
	}
	Vector2f &Vector2f::operator*=(float k) {
		this->x *= k;	this->y *= k;
		return *this;
	}
	Vector2f &Vector2f::operator/=(float k) {
		this->x /= k;		this->y /= k;
		return *this;
	}
	/* +Vector3f */
	Vector2f Vector2f::operator+() const {
		return *this;
	}
	/* -Vector3f */
	Vector2f Vector2f::operator-() const {
		return Vector2f(-x, -y);
	}
	float Vector2f::DotProduct(const Vector2f &v) {
		return this->x*v.x + this->y*v.y;
	}

	/* 比較演算子 */
	bool Vector2f::operator==(const Vector2f &v) const {
		return (x == v.x) && (y == v.y);
	}
	bool Vector2f::operator!=(const Vector2f &v) const {
		return !(*this == v);
	}

	/* べクトルの大きさ */
	float Vector2f::magnitude() const {
		return (float)sqrt(power());
	}
	float Vector2f::power() const {
		return x*x + y*y;
	}

	/* 正規化 */
	void Vector2f::normalize() {
		*this /= magnitude();
	}

	/* 2項演算子 */
	Vector2f operator+(const Vector2f &u, const Vector2f &v) {	//vector+vector
		Vector2f w;
		w.x = u.x + v.x;
		w.y = u.y + v.y;
		return w;
	}

	Vector2f operator-(const Vector2f &u, const Vector2f &v) {	//vector-vector
		Vector2f w;
		w.x = u.x - v.x;
		w.y = u.y - v.y;
		return w;
	}

	/* vector*scalar */
	Vector2f operator*(const Vector2f &v, float k) {
		Vector2f w;
		w.x = v.x * k;
		w.y = v.y * k;
		return w;
	}

	/* scalar*vector */
	Vector2f operator*(float k, const Vector2f &v) {
		Vector2f w;
		w.x = v.x * k;
		w.y = v.y * k;
		return w;
	}

	/* vector/scalar */
	Vector2f operator/(const Vector2f &v, float k) {
		Vector2f w;
		w.x = v.x / k;
		w.y = v.y / k;
		return w;
	}
	/* 内積 vector*vector */
	float operator*(const Vector2f &u, const Vector2f &v) {
		return u.x * v.x + u.y * v.y;
	}

	/* 内積 DotProduct(Vector3f,Vector3f) */
	float DotProduct(const Vector2f &u, const Vector2f &v) {
		return u.x*v.x + u.y*v.y;
	}
	/* 2つのベクトルのなす角 */
	float angle(const Vector2f &u, const Vector2f &v) {
		float cos = u*v / (u.magnitude()*v.magnitude());
		return float(acos(cos) / PI * 180);
	}

	/****************/
	/* Vector3i */
	/****************/
	/* =演算子 */
	Vector3i &Vector3i::operator=(const Vector3i &rhs) {
		x = rhs.x;	y = rhs.y;	z = rhs.z;
		return *this;
	}

	/* 添え字演算子 */
	int &Vector3i::operator[](int n) {
		assert(n >= 0 && n <= 2);
		if (0 == n)
			return x;
		else if (1 == n)
			return y;
		else
			return z;
	}

	/* 単項演算子 */
	Vector3i &Vector3i::operator+=(const Vector3i &v) {
		this->x += v.x;	this->y += v.y;	this->z += v.z;
		return *this;
	}
	Vector3i &Vector3i::operator-=(const Vector3i &v) {
		this->x -= v.x;	this->y -= v.y;	this->z -= v.z;
		return *this;
	}
	Vector3i &Vector3i::operator*=(int k) {
		this->x *= k;	this->y *= k;	this->z *= k;
		return *this;
	}
	Vector3i &Vector3i::operator/=(int k) {
		this->x /= k;	this->y /= k;	this->z /= k;
		return *this;
	}
	/* +Vector3i */
	Vector3i Vector3i::operator+() const {
		return *this;
	}
	/* -Vector3i */
	Vector3i Vector3i::operator-() const {
		return Vector3i(-x, -y, -z);
	}
	/* 内積 DotProduct(Vector3i) */
	int Vector3i::DotProduct(const Vector3i &v) {
		return this->x*v.x + this->y*v.y + this->z*v.z;
	}
	/* 外積 CrossProduct(Vector3i) */
	Vector3i Vector3i::CrossProduct(const Vector3i &v) {
		Vector3i w;
		w.x = this->y*v.z - this->z*v.y;
		w.y = this->z*v.x - this->x*v.z;
		w.z = this->x*v.y - this->y*v.x;
		return w;
	}

	/* 比較演算子 */
	bool Vector3i::operator==(const Vector3i &v) const {
		return (x == v.x) && (y == v.y) && (z == v.z);
	}
	bool Vector3i::operator!=(const Vector3i &v) const {
		return !(*this == v);
	}

	/* べクトルの大きさ */
	int Vector3i::magnitude() const {
		return (int)std::sqrt(power());
	}
	int Vector3i::power() const {
		return x*x + y*y + z*z;
	}

	/* 正規化 */
	void Vector3i::normalize() {
		*this /= magnitude();
	}

	/* 2項演算子 */
	/* Vector3i+Vector3i */
	Vector3i operator+(const Vector3i &u, const Vector3i &v) {
		Vector3i w;
		w.x = u.x + v.x;
		w.y = u.y + v.y;
		w.z = u.z + v.z;
		return w;
	}
	/* Vector3i-Vector3i */
	Vector3i operator-(const Vector3i &u, const Vector3i &v) {
		Vector3i w;
		w.x = u.x - v.x;
		w.y = u.y - v.y;
		w.z = u.z - v.z;
		return w;
	}
	/* int*Vector3i */
	Vector3i operator*(int k, const Vector3i &v) {
		return Vector3i(k * v.x, k * v.y, k * v.z);
	}
	/* Vector3i*int */
	Vector3i operator*(const Vector3i &v, int k) {
		return Vector3i(v.x * k, v.y * k, v.z * k);
	}
	/* 外積 Vector3i*Vector3i */
	Vector3i operator*(const Vector3i &v0, const Vector3i &v1){
		Vector3i ret;
		ret.x = v0.y*v1.z - v0.z*v1.y;
		ret.y = v0.z*v1.x - v0.x*v1.z;
		ret.z = v0.x*v1.y - v0.y*v1.x;
		return ret;
	};
	/* Vector3i/int */
	Vector3i operator/(const Vector3i &v, int k) {
		return Vector3i(v.x / k, v.y / k, v.z / k);
	}

	/* Matrix4f*Vector3i */
	Vector3i operator*(const Matrix4f &m, const Vector3i &v) {
		Vector3i ret;
		ret.x = (int)(m.mM[ 0 * 4 + 0] * v.x + m.mM[0 * 4 + 1] * v.y + m.mM[0 * 4 + 2] * v.z + m.mM[0 * 4 + 3] * 1);
		ret.y = (int)(m.mM[ 1 * 4 + 0] * v.x + m.mM[1 * 4 + 1] * v.y + m.mM[1 * 4 + 2] * v.z + m.mM[1 * 4 + 3] * 1);
		ret.z = (int)(m.mM[ 2 * 4 + 0] * v.x + m.mM[2 * 4 + 1] * v.y + m.mM[2 * 4 + 2] * v.z + m.mM[2 * 4 + 3] * 1);
		return ret;
	}

	/* 内積 DotProduct(Vector3i,Vector3i) */
	int DotProduct(const Vector3i &u, const Vector3i &v) {
		return u.x*v.x + u.y*v.y + u.z*v.z;
	}
	/* 外積 CrossProduct(Vector3i,Vector3i) */
	Vector3i CrossProduct(const Vector3i &u, const Vector3i &v) {
		Vector3i w;
		w.x = u.y*v.z - u.z*v.y;
		w.y = u.z*v.x - u.x*v.z;
		w.z = u.x*v.y - u.y*v.x;
		return w;
	}

	/* 2つのベクトルのなす角 */
	int angle(const Vector3i &u, const Vector3i &v) {
		int cos = DotProduct(u,v) / (u.magnitude()*v.magnitude());
		return int(std::acos(cos) / PI * 180);
	}

	/****************/
	/* Vector3f */
	/****************/
	///* =演算子 */
	//Vector3f &Vector3f::operator=(const Vector3f &rhs) {
	//	x = rhs.x;	y = rhs.y;	z = rhs.z;
	//	return *this;
	//}

	/* 添え字演算子 */
	float &Vector3f::operator[](int n) {
		assert(n >= 0 && n <= 2);
		if (0 == n)
			return x;
		else if (1 == n)
			return y;
		else
			return z;
	}

	/* 単項演算子 */
	Vector3f &Vector3f::operator+=(const Vector3f &v) {
		this->x += v.x;	this->y += v.y;	this->z += v.z;
		return *this;
	}
	Vector3f &Vector3f::operator-=(const Vector3f &v) {
		this->x -= v.x;	this->y -= v.y;	this->z -= v.z;
		return *this;
	}
	Vector3f &Vector3f::operator*=(float k) {
		this->x *= k;	this->y *= k;	this->z *= k;
		return *this;
	}
	Vector3f &Vector3f::operator/=(float k) {
		this->x /= k;	this->y /= k;	this->z /= k;
		return *this;
	}
	/* +Vector3f */
	Vector3f Vector3f::operator+() const {
		return *this;
	}
	/* -Vector3f */
	Vector3f Vector3f::operator-() const {
		return Vector3f(-x, -y, -z);
	}
	/* 内積 DotProduct(Vector3f) */
	float Vector3f::DotProduct(const Vector3f &v) {
		return this->x*v.x + this->y*v.y + this->z*v.z;
	}
	/* 外積 CrossProduct(Vector3f) */
	Vector3f Vector3f::CrossProduct(const Vector3f &v) {
		Vector3f w;
		w.x = this->y*v.z - this->z*v.y;
		w.y = this->z*v.x - this->x*v.z;
		w.z = this->x*v.y - this->y*v.x;
		return w;
	}

	/* 比較演算子 */
	bool Vector3f::operator==(const Vector3f &v) const {
		return (x == v.x) && (y == v.y) && (z == v.z);
	}
	bool Vector3f::operator!=(const Vector3f &v) const {
		return !(*this == v);
	}

	/* べクトルの大きさ */
	float Vector3f::magnitude() const {
		return (float)std::sqrt(power());
	}
	float Vector3f::power() const {
		return x*x + y*y + z*z;
	}

	/* 正規化 */
	void Vector3f::normalize() {
		*this /= magnitude();
	}

	/* 2項演算子 */
	/* Vector3f+Vector3f */
	Vector3f operator+(const Vector3f &u, const Vector3f &v) {
		Vector3f w;
		w.x = u.x + v.x;
		w.y = u.y + v.y;
		w.z = u.z + v.z;
		return w;
	}
	/* Vector3f-Vector3f */
	Vector3f operator-(const Vector3f &u, const Vector3f &v) {
		Vector3f w;
		w.x = u.x - v.x;
		w.y = u.y - v.y;
		w.z = u.z - v.z;
		return w;
	}
	/* float*Vector3f */
	Vector3f operator*(float k, const Vector3f &v) {
		return Vector3f(k * v.x, k * v.y, k * v.z);
	}
	/* Vector3f*float */
	Vector3f operator*(const Vector3f &v, float k) {
		return Vector3f(v.x * k, v.y * k, v.z * k);
	}
	/* 外積 Vector3f*Vector3f */
	Vector3f operator*(const Vector3f &v0, const Vector3f &v1){
		Vector3f ret;
		ret.x = v0.y*v1.z - v0.z*v1.y;
		ret.y = v0.z*v1.x - v0.x*v1.z;
		ret.z = v0.x*v1.y - v0.y*v1.x;
		return ret;
	};
	/* Vector3f/float */
	Vector3f operator/(const Vector3f &v, float k) {
		return Vector3f(v.x / k, v.y / k, v.z / k);
	}

	/* Matrix4f*Vector3f */
	Vector3f operator*(const Matrix4f &m, const Vector3f &v) {
		Vector3f ret;
		ret.x = m.mM[ 0 * 4 + 0] * v.x + m.mM[0 * 4 + 1] * v.y + m.mM[0 * 4 + 2] * v.z + m.mM[0 * 4 + 3] * 1;
		ret.y = m.mM[ 1 * 4 + 0] * v.x + m.mM[1 * 4 + 1] * v.y + m.mM[1 * 4 + 2] * v.z + m.mM[1 * 4 + 3] * 1;
		ret.z = m.mM[ 2 * 4 + 0] * v.x + m.mM[2 * 4 + 1] * v.y + m.mM[2 * 4 + 2] * v.z + m.mM[2 * 4 + 3] * 1;
		return ret;
	}

	/* 内積 DotProduct(Vector3f,Vector3f) */
	float DotProduct(const Vector3f &u, const Vector3f &v) {
		return u.x*v.x + u.y*v.y + u.z*v.z;
	}
	/* 外積 CrossProduct(Vector3f,Vector3f) */
	Vector3f CrossProduct(const Vector3f &u, const Vector3f &v) {
		Vector3f w;
		w.x = u.y*v.z - u.z*v.y;
		w.y = u.z*v.x - u.x*v.z;
		w.z = u.x*v.y - u.y*v.x;
		return w;
	}

	/* 2つのベクトルのなす角 */
	float angle(const Vector3f &u, const Vector3f &v) {
		float cos = DotProduct(u,v) / (u.magnitude()*v.magnitude());
		return float(std::acos(cos) / PI * 180);
	}

	/************/
	/* Vector4i */
	/************/
	/* =演算子 */
	//Vector4i &Vector4i::operator=(const Vector4i &rhs) {
	//	x = rhs.x;	y = rhs.y;	z = rhs.z;	w = rhs.w;
	//	return *this;
	//}

	/* 添え字演算子 */
	float &Vector4i::operator[](int n) {
		assert(n >= 0 && n <= 3);
		if (0 == n)
			return x;
		else if (1 == n)
			return y;
		else if (2 == n)
			return z;
		else
			return w;
	}

	/* 単項演算子 */
	Vector4i &Vector4i::operator+=(const Vector4i &v) {
		this->x += v.x;	this->y += v.y;	this->z += v.z;	this->w += v.w;
		return *this;
	}
	Vector4i &Vector4i::operator-=(const Vector4i &v) {
		this->x -= v.x;	this->y -= v.y;	this->z -= v.z;	this->w -= v.w;
		return *this;
	}
	Vector4i &Vector4i::operator*=(float k) {
		this->x *= k;	this->y *= k;	this->z *= k;	this->w *= k;
		return *this;
	}
	Vector4i &Vector4i::operator/=(float k) {
		this->x /= k;	this->y /= k;	this->z /= k;	this->w /= k;
		return *this;
	}
	/* +Vector4i */
	Vector4i Vector4i::operator+() const {
		return *this;
	}
	/* -Vector4i */
	Vector4i Vector4i::operator-() const {
		return Vector4i(-x, -y, -z, -w);
	}
	/* 内積 DotProduct(Vector4i) */
	float Vector4i::DotProduct(const Vector4i &v) {
		return this->x*v.x + this->y*v.y + this->z*v.z + this->w*v.w;
	}

	/* 比較演算子 */
	bool Vector4i::operator==(const Vector4i &v) const {
		return (x == v.x) && (y == v.y) && (z == v.z) && (w == v.w);
	}
	bool Vector4i::operator!=(const Vector4i &v) const {
		return !(*this == v);
	}

	/* べクトルの大きさ */
	float Vector4i::magnitude() const {
		return (float)sqrt(power());
	}
	float Vector4i::power() const {
		return x*x + y*y + z*z + w*w;
	}

	/* 正規化 */
	void Vector4i::normalize() {
		*this /= magnitude();
	}

	/* 2項演算子 */
	/* Vector4i+Vector4i */
	Vector4i operator+(const Vector4i &u, const Vector4i &v) {
		Vector4i w;
		w.x = u.x + v.x;
		w.y = u.y + v.y;
		w.z = u.z + v.z;
		w.w = u.w + v.w;
		return w;
	}
	/* Vector4i-Vector4i */
	Vector4i operator-(const Vector4i &u, const Vector4i &v) {
		Vector4i w;
		w.x = u.x - v.x;
		w.y = u.y - v.y;
		w.z = u.z - v.z;
		w.w = u.w - v.w;
		return w;
	}
	/* float*Vector4i */
	Vector4i operator*(float k, const  Vector4i &v) {
		return Vector4i(k * v.x, k * v.y, k * v.z, k * v.w);
	}
	/* Vector4i*float */
	Vector4i operator*(const Vector4i &v, float k) {
		return Vector4i(v.x * k, v.y * k, v.z * k, v.w * k);
	}
	/* Vector4i/float */
	Vector4i operator/(const Vector4i &v, float k) {
		return Vector4i(v.x / k, v.y / k, v.z / k, v.w / k);
	}
	/* 内積 Vector4i*Vector4i */
	float operator*(const Vector4i &u, const Vector4i &v) {
		return u.x*v.x + u.y*v.y + u.z*v.z + u.w*v.w;
	}
	/* 内積 DotProduct(Vector4i,Vector4i) */
	float DotProduct(const Vector4i &u, const Vector4i &v) {
		return u.x*v.x + u.y*v.y + u.z*v.z + u.w*v.w;
	}
	/* 2つのベクトルのなす角 */
	float angle(const Vector4i &u, const Vector4i &v) {
		float cos = u*v / (u.magnitude()*v.magnitude());
		return float(acos(cos) / PI * 180);
	}

	/**************/
	/* Vector4f */
	/**************/
	/* =演算子 */
	Vector4f &Vector4f::operator=(const Vector4f &rhs) {
		x = rhs.x;	y = rhs.y;	z = rhs.z;	w = rhs.w;
		return *this;
	}

	/* 添え字演算子 */
	float &Vector4f::operator[](int n) {
		assert(n >= 0 && n <= 3);
		if (0 == n)
			return x;
		else if (1 == n)
			return y;
		else if (2 == n)
			return z;
		else
			return w;
	}

	/* 単項演算子 */
	Vector4f &Vector4f::operator+=(const Vector4f &v) {
		this->x += v.x;	this->y += v.y;	this->z += v.z;	this->w += v.w;
		return *this;
	}
	Vector4f &Vector4f::operator-=(const Vector4f &v) {
		this->x -= v.x;	this->y -= v.y;	this->z -= v.z;	this->w -= v.w;
		return *this;
	}
	Vector4f &Vector4f::operator*=(float k) {
		this->x *= k;	this->y *= k;	this->z *= k;	this->w *= k;
		return *this;
	}
	Vector4f &Vector4f::operator/=(float k) {
		this->x /= k;	this->y /= k;	this->z /= k;	this->w /= k;
		return *this;
	}
	/* +Vector4f */
	Vector4f Vector4f::operator+() const {
		return *this;
	}
	/* -Vector4f */
	Vector4f Vector4f::operator-() const {
		return Vector4f(-x, -y, -z, -w);
	}
	/* 内積 DotProduct(Vector4f) */
	float Vector4f::DotProduct(const Vector4f &v) {
		return this->x*v.x + this->y*v.y + this->z*v.z + this->w*v.w;
	}

	/* 比較演算子 */
	bool Vector4f::operator==(const Vector4f &v) const {
		return (x == v.x) && (y == v.y) && (z == v.z) && (w == v.w);
	}
	bool Vector4f::operator!=(const Vector4f &v) const {
		return !(*this == v);
	}

	/* べクトルの大きさ */
	float Vector4f::magnitude() const {
		return (float)sqrt(power());
	}
	float Vector4f::power() const {
		return x*x + y*y + z*z + w*w;
	}

	/* 正規化 */
	void Vector4f::normalize() {
		*this /= magnitude();
	}

	/* 2項演算子 */
	/* Vector4f+Vector4f */
	Vector4f operator+(const Vector4f &u, const Vector4f &v) {
		Vector4f w;
		w.x = u.x + v.x;
		w.y = u.y + v.y;
		w.z = u.z + v.z;
		w.w = u.w + v.w;
		return w;
	}
	/* Vector4f-Vector4f */
	Vector4f operator-(const Vector4f &u, const Vector4f &v) {
		Vector4f w;
		w.x = u.x - v.x;
		w.y = u.y - v.y;
		w.z = u.z - v.z;
		w.w = u.w - v.w;
		return w;
	}
	/* float*Vector4f */
	Vector4f operator*(float k, const  Vector4f &v) {
		return Vector4f(k * v.x, k * v.y, k * v.z, k * v.w);
	}
	/* Vector4f*float */
	Vector4f operator*(const Vector4f &v, float k) {
		return Vector4f(v.x * k, v.y * k, v.z * k, v.w * k);
	}
	/* Vector4f/float */
	Vector4f operator/(const Vector4f &v, float k) {
		return Vector4f(v.x / k, v.y / k, v.z / k, v.w / k);
	}
	/* 内積 Vector4f*Vector4f */
	float operator*(const Vector4f &u, const Vector4f &v) {
		return u.x*v.x + u.y*v.y + u.z*v.z + u.w*v.w;
	}
	/* 内積 DotProduct(Vector4f,Vector4f) */
	float DotProduct(const Vector4f &u, const Vector4f &v) {
		return u.x*v.x + u.y*v.y + u.z*v.z + u.w*v.w;
	}
	/* 2つのベクトルのなす角 */
	float angle(const Vector4f &u, const Vector4f &v) {
		float cos = u*v / (u.magnitude()*v.magnitude());
		return float(acos(cos) / PI * 180);
	}

	/**************/
	/* Matrix4f */
	/**************/
	const std::array<float, 16> Matrix4f::IDENTITY = {
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0
	};

	Matrix4f::Matrix4f(float a0, float a1, float a2, float a3, float a4, float a5, float a6, float a7, float a8, float a9, float a10, float a11, float a12, float a13, float a14, float a15) {
		std::array<float, 16> &M = this->mM;
		M[0] = a0;  M[1] = a1;  M[2] = a2;  M[3] = a3;
		M[4] = a4;  M[5] = a5;  M[6] = a6;  M[7] = a7;
		M[8] = a8;  M[9] = a9;  M[10]= a10; M[11]= a11;
		M[12]= a12; M[13]= a13; M[14]= a14; M[15]= a15;
	}

	void Matrix4f::setIdentity() {
		this->mM = Matrix4f::IDENTITY;
	}

	/* =演算子 */
	Matrix4f &Matrix4f::operator=(const Matrix4f &rhs) {
		mM = rhs.mM;
		return *this;
	}

	/* 単項演算子 */
	Matrix4f &Matrix4f::operator+=(const Matrix4f &v) {
		std::array<float, 16> &M = this->mM;
		M[ 0] += v.mM[ 0]; M[ 1] += v.mM[ 1]; M[ 2] += v.mM[ 2]; M[ 3] += v.mM[ 3];
		M[ 4] += v.mM[ 4]; M[ 5] += v.mM[ 5]; M[ 6] += v.mM[ 6]; M[ 7] += v.mM[ 7];
		M[ 8] += v.mM[ 8]; M[ 9] += v.mM[ 9]; M[10] += v.mM[10]; M[11] += v.mM[11];
		M[12] += v.mM[12]; M[13] += v.mM[13]; M[14] += v.mM[14]; M[15] += v.mM[15];
		return *this;
	}
	Matrix4f &Matrix4f::operator-=(const Matrix4f &v) {
		std::array<float, 16> &M = this->mM;
		M[ 0] -= v.mM[ 0]; M[ 1] -= v.mM[ 1]; M[ 2] -= v.mM[ 2]; M[ 3] -= v.mM[ 3];
		M[ 4] -= v.mM[ 4]; M[ 5] -= v.mM[ 5]; M[ 6] -= v.mM[ 6]; M[ 7] -= v.mM[ 7];
		M[ 8] -= v.mM[ 8]; M[ 9] -= v.mM[ 9]; M[10] -= v.mM[10]; M[11] -= v.mM[11];
		M[12] -= v.mM[12]; M[13] -= v.mM[13]; M[14] -= v.mM[14]; M[15] -= v.mM[15];
		return *this;
	}
	Matrix4f &Matrix4f::operator*=(float k) {
		std::array<float, 16> &M = this->mM;
		M[ 0] *= k; M[ 1] *= k; M[ 2] *= k; M[ 3] *= k;
		M[ 4] *= k; M[ 5] *= k; M[ 6] *= k; M[ 7] *= k;
		M[ 8] *= k; M[ 9] *= k; M[10] *= k; M[11] *= k;
		M[12] *= k; M[13] *= k; M[14] *= k; M[15] *= k;
		return *this;
	}
	Matrix4f &Matrix4f::operator/=(float k) {
		std::array<float, 16> &M = this->mM;
		M[ 0] /= k; M[ 1] /= k; M[ 2] /= k; M[ 3] /= k;
		M[ 4] /= k; M[ 5] /= k; M[ 6] /= k; M[ 7] /= k;
		M[ 8] /= k; M[ 9] /= k; M[10] /= k; M[11] /= k;
		M[12] /= k; M[13] /= k; M[14] /= k; M[15] /= k;
		return *this;
	}
	Matrix4f &Matrix4f::operator*=(const Matrix4f &v) {
		std::array<float, 16> &M = this->mM;
		M[ 0] = M[ 0] * v.mM[0] + M[ 1] * v.mM[4] + M[ 2] * v.mM[ 8] + M[ 3] * v.mM[12];
		M[ 1] = M[ 0] * v.mM[1] + M[ 1] * v.mM[5] + M[ 2] * v.mM[ 9] + M[ 3] * v.mM[13];
		M[ 2] = M[ 0] * v.mM[2] + M[ 1] * v.mM[6] + M[ 2] * v.mM[10] + M[ 3] * v.mM[14];
		M[ 3] = M[ 0] * v.mM[3] + M[ 1] * v.mM[7] + M[ 2] * v.mM[11] + M[ 3] * v.mM[15];

		M[ 4] = M[ 4] * v.mM[0] + M[ 5] * v.mM[4] + M[ 6] * v.mM[ 8] + M[ 7] * v.mM[12];
		M[ 5] = M[ 4] * v.mM[1] + M[ 5] * v.mM[5] + M[ 6] * v.mM[ 9] + M[ 7] * v.mM[13];
		M[ 6] = M[ 4] * v.mM[2] + M[ 5] * v.mM[6] + M[ 6] * v.mM[10] + M[ 7] * v.mM[14];
		M[ 7] = M[ 4] * v.mM[3] + M[ 5] * v.mM[7] + M[ 6] * v.mM[11] + M[ 7] * v.mM[15];

		M[ 8] = M[ 8] * v.mM[0] + M[ 9] * v.mM[4] + M[10] * v.mM[ 8] + M[11] * v.mM[12];
		M[ 9] = M[ 8] * v.mM[1] + M[ 9] * v.mM[5] + M[10] * v.mM[ 9] + M[11] * v.mM[13];
		M[10] = M[ 8] * v.mM[2] + M[ 9] * v.mM[6] + M[10] * v.mM[10] + M[11] * v.mM[14];
		M[11] = M[ 8] * v.mM[3] + M[ 9] * v.mM[7] + M[10] * v.mM[11] + M[11] * v.mM[15];

		M[12] = M[12] * v.mM[0] + M[13] * v.mM[4] + M[14] * v.mM[ 8] + M[15] * v.mM[12];
		M[13] = M[12] * v.mM[1] + M[13] * v.mM[5] + M[14] * v.mM[ 9] + M[15] * v.mM[13];
		M[14] = M[12] * v.mM[2] + M[13] * v.mM[6] + M[14] * v.mM[10] + M[15] * v.mM[14];
		M[15] = M[12] * v.mM[3] + M[13] * v.mM[7] + M[14] * v.mM[11] + M[15] * v.mM[15];
		return *this;
	}
	/* 行列式取得 */
	float Matrix4f::det() {
		return
				+ mM[3 * 4 + 0] * mM[2 * 4 + 1] * mM[1 * 4 + 2] * mM[0 * 4 + 3] - mM[2 * 4 + 0] * mM[3 * 4 + 1] * mM[1 * 4 + 2] * mM[0 * 4 + 3]
				- mM[3 * 4 + 0] * mM[1 * 4 + 1] * mM[2 * 4 + 2] * mM[0 * 4 + 3] + mM[1 * 4 + 0] * mM[3 * 4 + 1] * mM[2 * 4 + 2] * mM[0 * 4 + 3]

				+ mM[2 * 4 + 0] * mM[1 * 4 + 1] * mM[3 * 4 + 2] * mM[0 * 4 + 3] - mM[1 * 4 + 0] * mM[2 * 4 + 1] * mM[3 * 4 + 2] * mM[0 * 4 + 3]
				- mM[3 * 4 + 0] * mM[2 * 4 + 1] * mM[0 * 4 + 2] * mM[1 * 4 + 3] + mM[2 * 4 + 0] * mM[3 * 4 + 1] * mM[0 * 4 + 2] * mM[1 * 4 + 3]

				+ mM[3 * 4 + 0] * mM[0 * 4 + 1] * mM[2 * 4 + 2] * mM[1 * 4 + 3] - mM[0 * 4 + 0] * mM[3 * 4 + 1] * mM[2 * 4 + 2] * mM[1 * 4 + 3]
				- mM[2 * 4 + 0] * mM[0 * 4 + 1] * mM[3 * 4 + 2] * mM[1 * 4 + 3] + mM[0 * 4 + 0] * mM[2 * 4 + 1] * mM[3 * 4 + 2] * mM[1 * 4 + 3]

				+ mM[3 * 4 + 0] * mM[1 * 4 + 1] * mM[0 * 4 + 2] * mM[2 * 4 + 3] - mM[1 * 4 + 0] * mM[3 * 4 + 1] * mM[0 * 4 + 2] * mM[2 * 4 + 3]
				- mM[3 * 4 + 0] * mM[0 * 4 + 1] * mM[1 * 4 + 2] * mM[2 * 4 + 3] + mM[0 * 4 + 0] * mM[3 * 4 + 1] * mM[1 * 4 + 2] * mM[2 * 4 + 3]

				+ mM[1 * 4 + 0] * mM[0 * 4 + 1] * mM[3 * 4 + 2] * mM[2 * 4 + 3] - mM[0 * 4 + 0] * mM[1 * 4 + 1] * mM[3 * 4 + 2] * mM[2 * 4 + 3]
				- mM[2 * 4 + 0] * mM[1 * 4 + 1] * mM[0 * 4 + 2] * mM[3 * 4 + 3] + mM[1 * 4 + 0] * mM[2 * 4 + 1] * mM[0 * 4 + 2] * mM[3 * 4 + 3]

				+ mM[2 * 4 + 0] * mM[0 * 4 + 1] * mM[1 * 4 + 2] * mM[3 * 4 + 3] - mM[0 * 4 + 0] * mM[2 * 4 + 1] * mM[1 * 4 + 2] * mM[3 * 4 + 3]
				- mM[1 * 4 + 0] * mM[0 * 4 + 1] * mM[2 * 4 + 2] * mM[3 * 4 + 3] + mM[0 * 4 + 0] * mM[1 * 4 + 1] * mM[2 * 4 + 2] * mM[3 * 4 + 3];
	}
	/* 逆行列取得 */
	Matrix4f Matrix4f::getInverse() {
		Matrix4f ret;

		ret.mM[0 * 4 + 0] =	+mM[2 * 4 + 1] * mM[3 * 4 + 2] * mM[1 * 4 + 3] - mM[3 * 4 + 1] * mM[2 * 4 + 2] * mM[1 * 4 + 3] + mM[3 * 4 + 1] * mM[1 * 4 + 2] * mM[2 * 4 + 3]
							-mM[1 * 4 + 1] * mM[3 * 4 + 2] * mM[2 * 4 + 3] - mM[2 * 4 + 1] * mM[1 * 4 + 2] * mM[3 * 4 + 3] + mM[1 * 4 + 1] * mM[2 * 4 + 2] * mM[3 * 4 + 3];

		ret.mM[1 * 4 + 0] = +mM[3 * 4 + 0] * mM[2 * 4 + 2] * mM[1 * 4 + 3] - mM[2 * 4 + 0] * mM[3 * 4 + 2] * mM[1 * 4 + 3] - mM[3 * 4 + 0] * mM[1 * 4 + 2] * mM[2 * 4 + 3]
							+mM[1 * 4 + 0] * mM[3 * 4 + 2] * mM[2 * 4 + 3] + mM[2 * 4 + 0] * mM[1 * 4 + 2] * mM[3 * 4 + 3] - mM[1 * 4 + 0] * mM[2 * 4 + 2] * mM[3 * 4 + 3];

		ret.mM[2 * 4 + 0] = +mM[2 * 4 + 0] * mM[3 * 4 + 1] * mM[1 * 4 + 3] - mM[3 * 4 + 0] * mM[2 * 4 + 1] * mM[1 * 4 + 3] + mM[3 * 4 + 0] * mM[1 * 4 + 1] * mM[2 * 4 + 3]
							-mM[1 * 4 + 0] * mM[3 * 4 + 1] * mM[2 * 4 + 3] - mM[2 * 4 + 0] * mM[1 * 4 + 1] * mM[3 * 4 + 3] + mM[1 * 4 + 0] * mM[2 * 4 + 1] * mM[3 * 4 + 3];

		ret.mM[3 * 4 + 0] = +mM[3 * 4 + 0] * mM[2 * 4 + 1] * mM[1 * 4 + 2] - mM[2 * 4 + 0] * mM[3 * 4 + 1] * mM[1 * 4 + 2] - mM[3 * 4 + 0] * mM[1 * 4 + 1] * mM[2 * 4 + 2]
							+mM[1 * 4 + 0] * mM[3 * 4 + 1] * mM[2 * 4 + 2] + mM[2 * 4 + 0] * mM[1 * 4 + 1] * mM[3 * 4 + 2] - mM[1 * 4 + 0] * mM[2 * 4 + 1] * mM[3 * 4 + 2];

		ret.mM[0 * 4 + 1] = +mM[3 * 4 + 1] * mM[2 * 4 + 2] * mM[0 * 4 + 3] - mM[2 * 4 + 1] * mM[3 * 4 + 2] * mM[0 * 4 + 3] - mM[3 * 4 + 1] * mM[0 * 4 + 2] * mM[2 * 4 + 3]
							+mM[0 * 4 + 1] * mM[3 * 4 + 2] * mM[2 * 4 + 3] + mM[2 * 4 + 1] * mM[0 * 4 + 2] * mM[3 * 4 + 3] - mM[0 * 4 + 1] * mM[2 * 4 + 2] * mM[3 * 4 + 3];

		ret.mM[1 * 4 + 1] = +mM[2 * 4 + 0] * mM[3 * 4 + 2] * mM[0 * 4 + 3] - mM[3 * 4 + 0] * mM[2 * 4 + 2] * mM[0 * 4 + 3] + mM[3 * 4 + 0] * mM[0 * 4 + 2] * mM[2 * 4 + 3]
							-mM[0 * 4 + 0] * mM[3 * 4 + 2] * mM[2 * 4 + 3] - mM[2 * 4 + 0] * mM[0 * 4 + 2] * mM[3 * 4 + 3] + mM[0 * 4 + 0] * mM[2 * 4 + 2] * mM[3 * 4 + 3];

		ret.mM[2 * 4 + 1] = +mM[3 * 4 + 0] * mM[2 * 4 + 1] * mM[0 * 4 + 3] - mM[2 * 4 + 0] * mM[3 * 4 + 1] * mM[0 * 4 + 3] - mM[3 * 4 + 0] * mM[0 * 4 + 1] * mM[2 * 4 + 3]
							+mM[0 * 4 + 0] * mM[3 * 4 + 1] * mM[2 * 4 + 3] + mM[2 * 4 + 0] * mM[0 * 4 + 1] * mM[3 * 4 + 3] - mM[0 * 4 + 0] * mM[2 * 4 + 1] * mM[3 * 4 + 3];

		ret.mM[3 * 4 + 1] = +mM[2 * 4 + 0] * mM[3 * 4 + 1] * mM[0 * 4 + 2] - mM[3 * 4 + 0] * mM[2 * 4 + 1] * mM[0 * 4 + 2] + mM[3 * 4 + 0] * mM[0 * 4 + 1] * mM[2 * 4 + 2]
							-mM[0 * 4 + 0] * mM[3 * 4 + 1] * mM[2 * 4 + 2] - mM[2 * 4 + 0] * mM[0 * 4 + 1] * mM[3 * 4 + 2] + mM[0 * 4 + 0] * mM[2 * 4 + 1] * mM[3 * 4 + 2];

		ret.mM[0 * 4 + 2] = +mM[1 * 4 + 1] * mM[3 * 4 + 2] * mM[0 * 4 + 3] - mM[3 * 4 + 1] * mM[1 * 4 + 2] * mM[0 * 4 + 3] + mM[3 * 4 + 1] * mM[0 * 4 + 2] * mM[1 * 4 + 3]
							-mM[0 * 4 + 1] * mM[3 * 4 + 2] * mM[1 * 4 + 3] - mM[1 * 4 + 1] * mM[0 * 4 + 2] * mM[3 * 4 + 3] + mM[0 * 4 + 1] * mM[1 * 4 + 2] * mM[3 * 4 + 3];

		ret.mM[1 * 4 + 2] = +mM[3 * 4 + 0] * mM[1 * 4 + 2] * mM[0 * 4 + 3] - mM[1 * 4 + 0] * mM[3 * 4 + 2] * mM[0 * 4 + 3] - mM[3 * 4 + 0] * mM[0 * 4 + 2] * mM[1 * 4 + 3]
							+mM[0 * 4 + 0] * mM[3 * 4 + 2] * mM[1 * 4 + 3] + mM[1 * 4 + 0] * mM[0 * 4 + 2] * mM[3 * 4 + 3] - mM[0 * 4 + 0] * mM[1 * 4 + 2] * mM[3 * 4 + 3];

		ret.mM[2 * 4 + 2] = +mM[1 * 4 + 0] * mM[3 * 4 + 1] * mM[0 * 4 + 3] - mM[3 * 4 + 0] * mM[1 * 4 + 1] * mM[0 * 4 + 3] + mM[3 * 4 + 0] * mM[0 * 4 + 1] * mM[1 * 4 + 3]
							-mM[0 * 4 + 0] * mM[3 * 4 + 1] * mM[1 * 4 + 3] - mM[1 * 4 + 0] * mM[0 * 4 + 1] * mM[3 * 4 + 3] + mM[0 * 4 + 0] * mM[1 * 4 + 1] * mM[3 * 4 + 3];

		ret.mM[3 * 4 + 2] = +mM[3 * 4 + 0] * mM[1 * 4 + 1] * mM[0 * 4 + 2] - mM[1 * 4 + 0] * mM[3 * 4 + 1] * mM[0 * 4 + 2] - mM[3 * 4 + 0] * mM[0 * 4 + 1] * mM[1 * 4 + 2]
							+mM[0 * 4 + 0] * mM[3 * 4 + 1] * mM[1 * 4 + 2] + mM[1 * 4 + 0] * mM[0 * 4 + 1] * mM[3 * 4 + 2] - mM[0 * 4 + 0] * mM[1 * 4 + 1] * mM[3 * 4 + 2];

		ret.mM[0 * 4 + 3] = +mM[2 * 4 + 1] * mM[1 * 4 + 2] * mM[0 * 4 + 3] - mM[1 * 4 + 1] * mM[2 * 4 + 2] * mM[0 * 4 + 3] - mM[2 * 4 + 1] * mM[0 * 4 + 2] * mM[1 * 4 + 3]
							+mM[0 * 4 + 1] * mM[2 * 4 + 2] * mM[1 * 4 + 3] + mM[1 * 4 + 1] * mM[0 * 4 + 2] * mM[2 * 4 + 3] - mM[0 * 4 + 1] * mM[1 * 4 + 2] * mM[2 * 4 + 3];

		ret.mM[1 * 4 + 3] = +mM[1 * 4 + 0] * mM[2 * 4 + 2] * mM[0 * 4 + 3] - mM[2 * 4 + 0] * mM[1 * 4 + 2] * mM[0 * 4 + 3] + mM[2 * 4 + 0] * mM[0 * 4 + 2] * mM[1 * 4 + 3]
							-mM[0 * 4 + 0] * mM[2 * 4 + 2] * mM[1 * 4 + 3] - mM[1 * 4 + 0] * mM[0 * 4 + 2] * mM[2 * 4 + 3] + mM[0 * 4 + 0] * mM[1 * 4 + 2] * mM[2 * 4 + 3];

		ret.mM[2 * 4 + 3] = +mM[2 * 4 + 0] * mM[1 * 4 + 1] * mM[0 * 4 + 3] - mM[1 * 4 + 0] * mM[2 * 4 + 1] * mM[0 * 4 + 3] - mM[2 * 4 + 0] * mM[0 * 4 + 1] * mM[1 * 4 + 3]
							+mM[0 * 4 + 0] * mM[2 * 4 + 1] * mM[1 * 4 + 3] + mM[1 * 4 + 0] * mM[0 * 4 + 1] * mM[2 * 4 + 3] - mM[0 * 4 + 0] * mM[1 * 4 + 1] * mM[2 * 4 + 3];

		ret.mM[3 * 4 + 3] = +mM[1 * 4 + 0] * mM[2 * 4 + 1] * mM[0 * 4 + 2] - mM[2 * 4 + 0] * mM[1 * 4 + 1] * mM[0 * 4 + 2] + mM[2 * 4 + 0] * mM[0 * 4 + 1] * mM[1 * 4 + 2]
							-mM[0 * 4 + 0] * mM[2 * 4 + 1] * mM[1 * 4 + 2] - mM[1 * 4 + 0] * mM[0 * 4 + 1] * mM[2 * 4 + 2] + mM[0 * 4 + 0] * mM[1 * 4 + 1] * mM[2 * 4 + 2];

		return ret / det();
	}
	Matrix4f Matrix4f::operator+() const {
		return *this;
	}
	Matrix4f Matrix4f::operator-() const {
		return Matrix4f(
				-1 * this->mM[ 0], -1 * this->mM[ 1], -1 * this->mM[ 2], -1 * this->mM[ 3],
				-1 * this->mM[ 4], -1 * this->mM[ 5], -1 * this->mM[ 6], -1 * this->mM[ 7],
				-1 * this->mM[ 8], -1 * this->mM[ 9], -1 * this->mM[10], -1 * this->mM[11],
				-1 * this->mM[12], -1 * this->mM[13], -1 * this->mM[14], -1 * this->mM[15]
		);
	}

	/* 添え字演算子 */
	float &Matrix4f::operator[](int i) {
		assert(0 <= i && i <= 15);
		return this->mM[i];
	}

	/* 比較演算子 */
	bool Matrix4f::operator==(const Matrix4f &v) const {
		return (this->mM[ 0] == v.mM[ 0]) && (this->mM[ 1] == v.mM[ 1]) && (this->mM[ 2] == v.mM[ 2]) && (this->mM[ 3] == v.mM[ 3])
			&& (this->mM[ 4] == v.mM[ 4]) && (this->mM[ 5] == v.mM[ 5]) && (this->mM[ 6] == v.mM[ 6]) && (this->mM[ 7] == v.mM[ 7])
			&& (this->mM[ 8] == v.mM[ 8]) && (this->mM[ 9] == v.mM[ 9]) && (this->mM[10] == v.mM[10]) && (this->mM[11] == v.mM[11])
			&& (this->mM[12] == v.mM[12]) && (this->mM[13] == v.mM[13]) && (this->mM[14] == v.mM[14]) && (this->mM[15] == v.mM[15])
			;
	}

	bool Matrix4f::operator!=(const Matrix4f &v) const {
		return !(*this == v);
	}

	/* ベクトル設定/取得 */
	void Matrix4f::setTranslation(const Vector3f &v) {
		mM[3 * 4 + 0] = v.x;
		mM[3 * 4 + 1] = v.y;
		mM[3 * 4 + 2] = v.z;
		mM[3 * 4 + 3] = 1;
	}

	Vector3f Matrix4f::getTranslation() const {
		return Vector3f(mM[3 * 4 + 0], mM[3 * 4 + 1], mM[3 * 4 + 2]);
	}

	void Matrix4f::setScale(float s) {
		mM[0 * 4 + 0] = mM[1 * 4 + 1] = mM[2 * 4 + 2] = s;
	}

	void Matrix4f::setScale(float sx, float sy, float sz) {
		mM[0 * 4 + 0] = sx;
		mM[1 * 4 + 1] = sy;
		mM[2 * 4 + 2] = sz;
	}

	void Matrix4f::setScale(const Vector3f &s) {
		mM[0 * 4 + 0] = s.x;
		mM[1 * 4 + 1] = s.y;
		mM[2 * 4 + 2] = s.z;
	}

	Vector3f Matrix4f::getScale() const {
		return Vector3f(mM[0 * 4 + 0], mM[1 * 4 + 1], mM[2 * 4 + 2]);
	}

	Matrix4f Matrix4f::getTrancepose() {
		Matrix4f ret;
		std::array<float, 16> &M = ret.mM;

		M[0*4+0] = mM[0*4+0];	/* (0,0) <=> (0,0) */
		M[0*4+1] = mM[1*4+0];	/* (0,1) <=> (1,0) */
		M[0*4+2] = mM[2*4+0];	/* (0,2) <=> (2,0) */
		M[0*4+3] = mM[3*4+0];	/* (0,3) <=> (3,0) */

		M[1*4+0] = mM[0*4+1];	/* (1,0) <=> (0,1) */
		M[1*4+1] = mM[1*4+1];	/* (1,1) <=> (1,1) */
		M[1*4+2] = mM[2*4+1];	/* (1,2) <=> (2,1) */
		M[1*4+3] = mM[3*4+1];	/* (1,3) <=> (3,1) */

		M[2*4+0] = mM[0*4+2];	/* (2,0) <=> (0,2) */
		M[2*4+1] = mM[1*4+2];	/* (2,1) <=> (1,2) */
		M[2*4+2] = mM[2*4+2];	/* (2,2) <=> (2,2) */
		M[2*4+3] = mM[3*4+2];	/* (2,3) <=> (3,2) */

		M[3*4+0] = mM[0*4+3];	/* (3,0) <=> (0,3) */
		M[3*4+1] = mM[1*4+3];	/* (3,1) <=> (1,3) */
		M[3*4+2] = mM[2*4+3];	/* (3,2) <=> (2,3) */
		M[3*4+3] = mM[3*4+3];	/* (3,3) <=> (3,3) */
		return ret;
	}

	void Matrix4f::normalize() {
		std::array<float, 16> &M = this->mM;
		/* 2乗和の√を求める */
		float norm0 = std::sqrt(M[0] * M[0] + M[4] * M[4] + M[ 8] * M[ 8]);
		float norm1 = std::sqrt(M[1] * M[1] + M[5] * M[5] + M[ 9] * M[ 9]);
		float norm2 = std::sqrt(M[2] * M[2] + M[6] * M[6] + M[10] * M[10]);

		M[ 0] = M[ 0] / norm0;	M[ 1] = M[ 1] / norm1;	M[ 2] = M[ 2] / norm2;/*	M[ 3] = M[ 3]; */
		M[ 4] = M[ 4] / norm0;	M[ 5] = M[ 5] / norm1;	M[ 6] = M[ 6] / norm2;/*	M[ 7] = M[ 7]; */
		M[ 8] = M[ 8] / norm0;	M[ 9] = M[ 9] / norm1;	M[10] = M[10] / norm2;/*	M[11] = M[11]; */
		M[12] = M[12] / norm0;	M[13] = M[13] / norm1;	M[14] = M[14] / norm2;/*	M[15] = M[15]; */
	}

	/* 2項演算子 */
	/* Matrix4f+Matrix4f */
	Matrix4f operator+(const Matrix4f &u, const Matrix4f &v) {
		Matrix4f w;
		std::array<float, 16> &M = w.mM;
		M[ 0] = u.mM[ 0] + v.mM[ 0]; M[ 1] = u.mM[ 1] + v.mM[ 1]; M[ 2] = u.mM[ 2] + v.mM[ 2]; M[ 3] = u.mM[ 3] + v.mM[ 3];
		M[ 4] = u.mM[ 4] + v.mM[ 4]; M[ 5] = u.mM[ 5] + v.mM[ 5]; M[ 6] = u.mM[ 6] + v.mM[ 6]; M[ 7] = u.mM[ 7] + v.mM[ 7];
		M[ 8] = u.mM[ 8] + v.mM[ 8]; M[ 9] = u.mM[ 9] + v.mM[ 9]; M[10] = u.mM[10] + v.mM[10]; M[11] = u.mM[11] + v.mM[11];
		M[12] = u.mM[12] + v.mM[12]; M[13] = u.mM[13] + v.mM[13]; M[14] = u.mM[14] + v.mM[14]; M[15] = u.mM[15] + v.mM[15];
		return w;
	}
	/* Matrix4f-Matrix4f */
	Matrix4f operator-(const Matrix4f &u, const Matrix4f &v) {
		Matrix4f w;
		std::array<float, 16> &M = w.mM;
		M[ 0] = u.mM[ 0] - v.mM[ 0]; M[ 1] = u.mM[ 1] - v.mM[ 1]; M[ 2] = u.mM[ 2] - v.mM[ 2]; M[ 3] = u.mM[ 3] - v.mM[ 3];
		M[ 4] = u.mM[ 4] - v.mM[ 4]; M[ 5] = u.mM[ 5] - v.mM[ 5]; M[ 6] = u.mM[ 6] - v.mM[ 6]; M[ 7] = u.mM[ 7] - v.mM[ 7];
		M[ 8] = u.mM[ 8] - v.mM[ 8]; M[ 9] = u.mM[ 9] - v.mM[ 9]; M[10] = u.mM[10] - v.mM[10]; M[11] = u.mM[11] - v.mM[11];
		M[12] = u.mM[12] - v.mM[12]; M[13] = u.mM[13] - v.mM[13]; M[14] = u.mM[14] - v.mM[14]; M[15] = u.mM[15] - v.mM[15];
		return w;
	}
	/* float*Matrix4f */
	Matrix4f operator*(float k, const  Matrix4f &v) {
		Matrix4f w;
		std::array<float, 16> &M = w.mM;
		M[ 0] = k * v.mM[ 0]; M[ 1] = k * v.mM[ 1]; M[ 2] = k * v.mM[ 2]; M[ 3] = k * v.mM[ 3];
		M[ 4] = k * v.mM[ 4]; M[ 5] = k * v.mM[ 5]; M[ 6] = k * v.mM[ 6]; M[ 7] = k * v.mM[ 7];
		M[ 8] = k * v.mM[ 8]; M[ 9] = k * v.mM[ 9]; M[10] = k * v.mM[10]; M[11] = k * v.mM[11];
		M[12] = k * v.mM[12]; M[13] = k * v.mM[13]; M[14] = k * v.mM[14]; M[15] = k * v.mM[15];
		return w;
	}
	/* Matrix4f*float */
	Matrix4f operator*(const Matrix4f &v, float k) {
		return operator*(k, v);
	}
	/* Matrix4f/float */
	Matrix4f operator/(const Matrix4f &v, float k) {
		Matrix4f w;
		std::array<float, 16> &M = w.mM;
		M[ 0] = v.mM[ 0] / k; M[ 1] = v.mM[ 1] / k; M[ 2] = v.mM[ 2] / k; M[ 3] = v.mM[ 3] / k;
		M[ 4] = v.mM[ 4] / k; M[ 5] = v.mM[ 5] / k; M[ 6] = v.mM[ 6] / k; M[ 7] = v.mM[ 7] / k;
		M[ 8] = v.mM[ 8] / k; M[ 9] = v.mM[ 9] / k; M[10] = v.mM[10] / k; M[11] = v.mM[11] / k;
		M[12] = v.mM[12] / k; M[13] = v.mM[13] / k; M[14] = v.mM[14] / k; M[15] = v.mM[15] / k;
		return w;
	}
	/* Vector3f*Vector3f */
	Matrix4f operator*(const Matrix4f &u, const Matrix4f &v) {
		Matrix4f w;
		std::array<float, 16> &M = w.mM;
		M[ 0] = u.mM[ 0] * v.mM[ 0] + u.mM[ 1] * v.mM[ 4] + u.mM[ 2] * v.mM[ 8] + u.mM[ 3] * v.mM[12];
		M[ 1] = u.mM[ 0] * v.mM[ 1] + u.mM[ 1] * v.mM[ 5] + u.mM[ 2] * v.mM[ 9] + u.mM[ 3] * v.mM[13];
		M[ 2] = u.mM[ 0] * v.mM[ 2] + u.mM[ 1] * v.mM[ 6] + u.mM[ 2] * v.mM[10] + u.mM[ 3] * v.mM[14];
		M[ 3] = u.mM[ 0] * v.mM[ 3] + u.mM[ 1] * v.mM[ 7] + u.mM[ 2] * v.mM[11] + u.mM[ 3] * v.mM[15];

		M[ 4] = u.mM[ 4] * v.mM[ 0] + u.mM[ 5] * v.mM[ 4] + u.mM[ 6] * v.mM[ 8] + u.mM[ 7] * v.mM[12];
		M[ 5] = u.mM[ 4] * v.mM[ 1] + u.mM[ 5] * v.mM[ 5] + u.mM[ 6] * v.mM[ 9] + u.mM[ 7] * v.mM[13];
		M[ 6] = u.mM[ 4] * v.mM[ 2] + u.mM[ 5] * v.mM[ 6] + u.mM[ 6] * v.mM[10] + u.mM[ 7] * v.mM[14];
		M[ 7] = u.mM[ 4] * v.mM[ 3] + u.mM[ 5] * v.mM[ 7] + u.mM[ 6] * v.mM[11] + u.mM[ 7] * v.mM[15];

		M[ 8] = u.mM[ 8] * v.mM[ 0] + u.mM[ 9] * v.mM[ 4] + u.mM[10] * v.mM[ 8] + u.mM[11] * v.mM[12];
		M[ 9] = u.mM[ 8] * v.mM[ 1] + u.mM[ 9] * v.mM[ 5] + u.mM[10] * v.mM[ 9] + u.mM[11] * v.mM[13];
		M[10] = u.mM[ 8] * v.mM[ 2] + u.mM[ 9] * v.mM[ 6] + u.mM[10] * v.mM[10] + u.mM[11] * v.mM[14];
		M[11] = u.mM[ 8] * v.mM[ 3] + u.mM[ 9] * v.mM[ 7] + u.mM[10] * v.mM[11] + u.mM[11] * v.mM[15];

		M[12] = u.mM[12] * v.mM[ 0] + u.mM[13] * v.mM[ 4] + u.mM[14] * v.mM[ 8] + u.mM[15] * v.mM[12];
		M[13] = u.mM[12] * v.mM[ 1] + u.mM[13] * v.mM[ 5] + u.mM[14] * v.mM[ 9] + u.mM[15] * v.mM[13];
		M[14] = u.mM[12] * v.mM[ 2] + u.mM[13] * v.mM[ 6] + u.mM[14] * v.mM[10] + u.mM[15] * v.mM[14];
		M[15] = u.mM[12] * v.mM[ 3] + u.mM[13] * v.mM[ 7] + u.mM[14] * v.mM[11] + u.mM[15] * v.mM[15];
		return w;
	}

	/* 同軸チェック */
	bool isSameAxle(const Axis &a1, const Axis &a2) {
		if (a1 == Axis::X || a1 == Axis::_X) {
			if (a2 == Axis::X || a2 == Axis::_X)	return true;
			else									return false;
		}
		else if (a1 == Axis::Y || a1 == Axis::_Y) {
			if (a2 == Axis::Y || a2 == Axis::_Y)	return true;
			else									return false;
		}
		else if (a1 == Axis::Z || a1 == Axis::_Z) {
			if (a2 == Axis::Z || a2 == Axis::_Z)	return true;
			else									return false;
		}
		return false;
	}

	/**************/
	/* 行列初期化 */
	/**************/
	void MatVec::LoadIdentity(std::array<float, 16> &M) {
		M = Matrix4f::IDENTITY;
	}

	Matrix4f MatVec::LoadIdentity() {
		Matrix4f ret;
		ret.mM = Matrix4f::IDENTITY;
		return ret;
	}

	void MatVec::LoadMatrix(std::array<float, 16> &retmat, const std::array<float, 16> &a) {
		retmat = a;
	}

	void MatVec::MultMatrixf(std::array<float, 16> &retmat, const std::array<float, 16> &m) {
		std::array<float, 16> a = {};
		LoadMatrix(a, retmat);

	#define A(row, col) a[(col << 2) + row]
	#define M(row, col) m[(col << 2) + row]
	#define MAT(row, col) retmat[(col << 2) + row]

		for (int i = 0; i < 4; i++) {
			MAT(i, 0) = A(i, 0) * M(0, 0) + A(i, 1) * M(1, 0) + A(i, 2) * M(2, 0) + A(i, 3) * M(3, 0);
			MAT(i, 1) = A(i, 0) * M(0, 1) + A(i, 1) * M(1, 1) + A(i, 2) * M(2, 1) + A(i, 3) * M(3, 1);
			MAT(i, 2) = A(i, 0) * M(0, 2) + A(i, 1) * M(1, 2) + A(i, 2) * M(2, 2) + A(i, 3) * M(3, 2);
			MAT(i, 3) = A(i, 0) * M(0, 3) + A(i, 1) * M(1, 3) + A(i, 2) * M(2, 3) + A(i, 3) * M(3, 3);
		}

	#undef A
	#undef M
	#undef MAT
	}

	void MatVec::MultMatrixf(std::array<float, 16> &retmat, const std::array<float, 16> &a, const std::array<float, 16> &m) {
	#define A(row, col) a[(col << 2) + row]
	#define M(row, col) m[(col << 2) + row]
	#define MAT(row, col) retmat[(col << 2) + row]

		for (int i = 0; i < 4; i++) {
			MAT(i, 0) = A(i, 0) * M(0, 0) + A(i, 1) * M(1, 0) + A(i, 2) * M(2, 0) + A(i, 3) * M(3, 0);
			MAT(i, 1) = A(i, 0) * M(0, 1) + A(i, 1) * M(1, 1) + A(i, 2) * M(2, 1) + A(i, 3) * M(3, 1);
			MAT(i, 2) = A(i, 0) * M(0, 2) + A(i, 1) * M(1, 2) + A(i, 2) * M(2, 2) + A(i, 3) * M(3, 2);
			MAT(i, 3) = A(i, 0) * M(0, 3) + A(i, 1) * M(1, 3) + A(i, 2) * M(2, 3) + A(i, 3) * M(3, 3);
		}

	#undef A
	#undef M
	#undef MAT
	}

	Matrix4f MatVec::MultMatrix(const Matrix4f &a, const Matrix4f &m) {
		Matrix4f ret;

	#define A(row, col) a.mM[(col << 2) + row]
	#define M(row, col) m.mM[(col << 2) + row]
	#define MAT(row, col) ret.mM[(col << 2) + row]

		for (int i = 0; i < 4; i++) {
			MAT(i, 0) = A(i, 0) * M(0, 0) + A(i, 1) * M(1, 0) + A(i, 2) * M(2, 0) + A(i, 3) * M(3, 0);
			MAT(i, 1) = A(i, 0) * M(0, 1) + A(i, 1) * M(1, 1) + A(i, 2) * M(2, 1) + A(i, 3) * M(3, 1);
			MAT(i, 2) = A(i, 0) * M(0, 2) + A(i, 1) * M(1, 2) + A(i, 2) * M(2, 2) + A(i, 3) * M(3, 2);
			MAT(i, 3) = A(i, 0) * M(0, 3) + A(i, 1) * M(1, 3) + A(i, 2) * M(2, 3) + A(i, 3) * M(3, 3);
		}

	#undef A
	#undef M
	#undef MAT

		return ret;
	}

	/**********/
	/* 正規化 */
	/**********/
	void MatVec::normalize(std::array<float, 3> &v) {
		float l = (float)sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
		if (l == 0.0f)
			return;

		v[0] /= l;
		v[1] /= l;
		v[2] /= l;
	}

	/********/
	/* 外積 */
	/********/
	void MatVec::cross(const std::array<float, 3> &v1, const std::array<float, 3> &v2, std::array<float, 3> &result) {
		result[0] = v1[1] * v2[2] - v1[2] * v2[1];
		result[1] = v1[2] * v2[0] - v1[0] * v2[2];
		result[2] = v1[0] * v2[1] - v1[1] * v2[0];
	}

	/*******************/
	/* setPerspectivef */
	/*******************/
	std::array<float, 16> MatVec::GetPerspectivef(float fovy, float aspect, float zNear, float zFar) {
		std::array<float, 16> retMat = {Matrix4f::IDENTITY};

		std::array<float, 16> m = {};
		float left, right, top, bottom;
		float xx, yy, zz;
		float n2 = zNear + zNear;

		bottom = (float)tan(fovy / 2.0f * PI / 180.0) * zNear;
		top = -bottom;
		right = aspect * bottom;
		left = -right;
		xx = 1.0f / (left - right);
		yy = 1.0f / (bottom - top);
		zz = 1.0f / (zNear - zFar);

	#define M(row, col) m[col * 4 + row]

		M(0, 0) = n2 * xx;
		M(0, 1) = 0.0f;
		M(0, 2) = (right + left) * xx;
		M(0, 3) = 0.0f;

		M(1, 0) = 0.0f;
		M(1, 1) = n2 * yy;
		M(1, 2) = (top + bottom) * yy;
		M(1, 3) = 0.0f;

		M(2, 0) = 0.0f;
		M(2, 1) = 0.0f;
		M(2, 2) = -(zFar + zNear) * zz;
		M(2, 3) = -(n2 * zFar) * zz;

		M(3, 0) = 0.0f;
		M(3, 1) = 0.0f;
		M(3, 2) = -1.0f;
		M(3, 3) = 0.0f;

	#undef M

		MultMatrixf(retMat, m);

		return retMat;
	}

	/*******************/
	/*   setLookAtf	*/
	/*******************/
	std::array<float, 16> MatVec::GetLookAtf(float eyex, float eyey, float eyez, float tarx, float tary, float tarz, float upx, float upy, float upz) {
		std::array<float, 16> retmat = Matrix4f::IDENTITY;
		std::array<float, 3> view = {}, up = {}, side = {};
		std::array<float, 16> m = {};

		view[0] = tarx - eyex;
		view[1] = tary - eyey;
		view[2] = tarz - eyez;

		up[0] = upx;
		up[1] = upy;
		up[2] = upz;

		MatVec::normalize(view);
		MatVec::normalize(up);

		MatVec::cross(view, up, side);
		MatVec::normalize(side);

		MatVec::cross(side, view, up);

	#define M(row, col) m[(col << 2) + row]

		M(0, 0) = side[0];
		M(0, 1) = side[1];
		M(0, 2) = side[2];
		M(0, 3) = 0.0f;

		M(1, 0) = up[0];
		M(1, 1) = up[1];
		M(1, 2) = up[2];
		M(1, 3) = 0.0f;

		M(2, 0) = -view[0];
		M(2, 1) = -view[1];
		M(2, 2) = -view[2];
		M(2, 3) = 0.0f;

		M(3, 0) = 0.0f;
		M(3, 1) = 0.0f;
		M(3, 2) = 0.0f;
		M(3, 3) = 1.0f;

	#undef M

		MatVec::MultMatrixf(retmat, m);
		MatVec::Translatef(retmat, -eyex, -eyey, -eyez);
		return retmat;
	}

	/******************/
	/* 移動行列を設定 */
	/******************/
	void MatVec::Translatef(std::array<float, 16> &retmat, float x, float y, float z) {
		retmat[12] = retmat[0] * x + retmat[4] * y + retmat[ 8] * z + retmat[12];
		retmat[13] = retmat[1] * x + retmat[5] * y + retmat[ 9] * z + retmat[13];
		retmat[14] = retmat[2] * x + retmat[6] * y + retmat[10] * z + retmat[14];
		retmat[15] = retmat[3] * x + retmat[7] * y + retmat[11] * z + retmat[15];
	}

	/******************/
	/* 回転行列を加算 */
	/* @param rm returns the result */
	/* @param a angle to rotate in degrees */
	/* @param x X axis component */
	/* @param y Y axis component */
	/* @param z Z axis component */
	/******************/
	void MatVec::Rotatef(std::array<float, 16> &retmat, float angle, float x, float y, float z) {
		std::array<float, 16> rotm = MatVec::GetRotatef(angle, x, y, z);
		MatVec::MultMatrixf(retmat, rotm);
	}

	/******************/
	/* 回転行列を設定 */
	/* @param rm returns the result */
	/* @param a angle to rotate in degrees */
	/* @param x X axis component */
	/* @param y Y axis component */
	/* @param z Z axis component */
	/******************/
	std::array<float, 16> MatVec::GetRotatef(float angle, float x, float y, float z) {
		std::array<float, 16> rm = {};
		rm[3] = 0;
		rm[7] = 0;
		rm[11]= 0;
		rm[12]= 0;
		rm[13]= 0;
		rm[14]= 0;
		rm[15]= 1;
		angle *= (float) (PI / 180.0f);
		float s = (float) std::sin(angle);
		float c = (float) std::cos(angle);
		if (1.0f == x && 0.0f == y && 0.0f == z) {
			rm[5] = c;   rm[10]= c;
			rm[6] = s;   rm[9] = -s;
			rm[1] = 0;   rm[2] = 0;
			rm[4] = 0;   rm[8] = 0;
			rm[0] = 1;
		} else if (0.0f == x && 1.0f == y && 0.0f == z) {
			rm[0] = c;   rm[10]= c;
			rm[8] = s;   rm[2] = -s;
			rm[1] = 0;   rm[4] = 0;
			rm[6] = 0;   rm[9] = 0;
			rm[5] = 1;
		} else if (0.0f == x && 0.0f == y && 1.0f == z) {
			rm[0] = c;   rm[5] = c;
			rm[1] = s;   rm[4] = -s;
			rm[2] = 0;   rm[6] = 0;
			rm[8] = 0;   rm[9] = 0;
			rm[10]= 1;
		} else {
			float len = std::sqrt(x*x + y*y + z*z);
			if (1.0f != len) {
				float recipLen = 1.0f / len;
				x *= recipLen;
				y *= recipLen;
				z *= recipLen;
			}
			float nc = 1.0f - c;
			float xy = x * y;
			float yz = y * z;
			float zx = z * x;
			float xs = x * s;
			float ys = y * s;
			float zs = z * s;
			rm[ 0] = x*x*nc +  c;
			rm[ 4] =  xy*nc - zs;
			rm[ 8] =  zx*nc + ys;
			rm[ 1] =  xy*nc + zs;
			rm[ 5] = y*y*nc +  c;
			rm[ 9] =  yz*nc - xs;
			rm[ 2] =  zx*nc - ys;
			rm[ 6] =  yz*nc + xs;
			rm[10] = z*z*nc +  c;
		}

		return rm;
	}

	void MatVec::Scalef(std::array<float, 16> &retmat, float x, float y, float z) {
		for (int i=0 ; i<4 ; i++) {
			retmat[	 i] *= x;
			retmat[ 4 + i] *= y;
			retmat[ 8 + i] *= z;
		}
	}

	bool MatVec::invertf(std::array<float, 16> &retmat, const std::array<float, 16> &matrix) {
		/* 転置行列 */
		float src0  = matrix[ 0];
		float src4  = matrix[ 1];
		float src8  = matrix[ 2];
		float src12 = matrix[ 3];

		float src1  = matrix[ 4];
		float src5  = matrix[ 5];
		float src9  = matrix[ 6];
		float src13 = matrix[ 7];

		float src2  = matrix[ 8];
		float src6  = matrix[ 9];
		float src10 = matrix[10];
		float src14 = matrix[11];

		float src3  = matrix[12];
		float src7  = matrix[13];
		float src11 = matrix[14];
		float src15 = matrix[15];

		/* calculate pairs for first 8 elements (cofactors) */
		float atmp0  = src10 * src15;
		float atmp1  = src11 * src14;
		float atmp2  = src9  * src15;
		float atmp3  = src11 * src13;
		float atmp4  = src9  * src14;
		float atmp5  = src10 * src13;
		float atmp6  = src8  * src15;
		float atmp7  = src11 * src12;
		float atmp8  = src8  * src14;
		float atmp9  = src10 * src12;
		float atmp10 = src8  * src13;
		float atmp11 = src9  * src12;

		/* calculate first 8 elements (cofactors) */
		float dst0  = (atmp0 * src5 + atmp3 * src6 + atmp4  * src7)
					  - (atmp1 * src5 + atmp2 * src6 + atmp5  * src7);
		float dst1  = (atmp1 * src4 + atmp6 * src6 + atmp9  * src7)
					  - (atmp0 * src4 + atmp7 * src6 + atmp8  * src7);
		float dst2  = (atmp2 * src4 + atmp7 * src5 + atmp10 * src7)
					  - (atmp3 * src4 + atmp6 * src5 + atmp11 * src7);
		float dst3  = (atmp5 * src4 + atmp8 * src5 + atmp11 * src6)
					  - (atmp4 * src4 + atmp9 * src5 + atmp10 * src6);
		float dst4  = (atmp1 * src1 + atmp2 * src2 + atmp5  * src3)
					  - (atmp0 * src1 + atmp3 * src2 + atmp4  * src3);
		float dst5  = (atmp0 * src0 + atmp7 * src2 + atmp8  * src3)
					  - (atmp1 * src0 + atmp6 * src2 + atmp9  * src3);
		float dst6  = (atmp3 * src0 + atmp6 * src1 + atmp11 * src3)
					  - (atmp2 * src0 + atmp7 * src1 + atmp10 * src3);
		float dst7  = (atmp4 * src0 + atmp9 * src1 + atmp10 * src2)
					  - (atmp5 * src0 + atmp8 * src1 + atmp11 * src2);

		/* calculate pairs for second 8 elements (cofactors) */
		float btmp0  = src2 * src7;
		float btmp1  = src3 * src6;
		float btmp2  = src1 * src7;
		float btmp3  = src3 * src5;
		float btmp4  = src1 * src6;
		float btmp5  = src2 * src5;
		float btmp6  = src0 * src7;
		float btmp7  = src3 * src4;
		float btmp8  = src0 * src6;
		float btmp9  = src2 * src4;
		float btmp10 = src0 * src5;
		float btmp11 = src1 * src4;

		/* calculate second 8 elements (cofactors) */
		float dst8  = (btmp0  * src13 + btmp3  * src14 + btmp4  * src15)
					  - (btmp1  * src13 + btmp2  * src14 + btmp5  * src15);
		float dst9  = (btmp1  * src12 + btmp6  * src14 + btmp9  * src15)
					  - (btmp0  * src12 + btmp7  * src14 + btmp8  * src15);
		float dst10 = (btmp2  * src12 + btmp7  * src13 + btmp10 * src15)
					  - (btmp3  * src12 + btmp6  * src13 + btmp11 * src15);
		float dst11 = (btmp5  * src12 + btmp8  * src13 + btmp11 * src14)
					  - (btmp4  * src12 + btmp9  * src13 + btmp10 * src14);
		float dst12 = (btmp2  * src10 + btmp5  * src11 + btmp1  * src9 )
					  - (btmp4  * src11 + btmp0  * src9  + btmp3  * src10);
		float dst13 = (btmp8  * src11 + btmp0  * src8  + btmp7  * src10)
					  - (btmp6  * src10 + btmp9  * src11 + btmp1  * src8 );
		float dst14 = (btmp6  * src9  + btmp11 * src11 + btmp3  * src8 )
					  - (btmp10 * src11 + btmp2  * src8  + btmp7  * src9 );
		float dst15 = (btmp10 * src10 + btmp4  * src8  + btmp9  * src9 )
					  - (btmp8  * src9  + btmp11 * src10 + btmp5  * src8 );

		/* calculate determinant */
		float det = src0 * dst0 + src1 * dst1 + src2 * dst2 + src3 * dst3;

		if (det == 0.0f) return false;

		/* calculate matrix inverse */
		float invdet = 1.0f / det;
		retmat[ 0] = dst0  * invdet;
		retmat[ 1] = dst1  * invdet;
		retmat[ 2] = dst2  * invdet;
		retmat[ 3] = dst3  * invdet;

		retmat[ 4] = dst4  * invdet;
		retmat[ 5] = dst5  * invdet;
		retmat[ 6] = dst6  * invdet;
		retmat[ 7] = dst7  * invdet;

		retmat[ 8] = dst8  * invdet;
		retmat[ 9] = dst9  * invdet;
		retmat[10] = dst10 * invdet;
		retmat[11] = dst11 * invdet;

		retmat[12] = dst12 * invdet;
		retmat[13] = dst13 * invdet;
		retmat[14] = dst14 * invdet;
		retmat[15] = dst15 * invdet;

		return true;
	}

	void MatVec::transposef(std::array<float, 16> &retmat, const std::array<float, 16> &matrix) {
		for (int i = 0; i < 4; i++) {
			int mBase = i * 4;
			retmat[i	 ] = matrix[mBase	];
			retmat[i + 4 ] = matrix[mBase + 1];
			retmat[i + 8 ] = matrix[mBase + 2];
			retmat[i + 12] = matrix[mBase + 3];
		}
	}

	/*******************************************************************/
	/* 回転行列生成													*/
	/* * @param xDeg Angle (in degrees(°)) of rotation around axis X. */
	/* * @param yDeg Angle (in degrees(°)) of rotation around axis Y. */
	/* * @param zDeg Angle (in degrees(°)) of rotation around axis Z. */
	/*******************************************************************/
	#define DEG2RAD(x) ((x * PI) / 180.0)
	Matrix4f MatVec::createRotation(float xDeg, float yDeg, float zDeg) {
		float xRads = (float)DEG2RAD(xDeg);
		float yRads = (float)DEG2RAD(yDeg);
		float zRads = (float)DEG2RAD(zDeg);

		Matrix4f ma, mb, mc;
		ma.setIdentity(); mb.setIdentity(); mc.setIdentity();
		float ac = cos(xRads);
		float as = sin(xRads);
		float bc = cos(yRads);
		float bs = sin(yRads);
		float cc = cos(zRads);
		float cs = sin(zRads);

		ma[1*4+1] = ac;
		ma[2*4+1] = as;
		ma[1*4+2] = -as;
		ma[2*4+2] = ac;

		mb[0*4+0] = bc;
		mb[2*4+0] = -bs;
		mb[0*4+2] = bs;
		mb[2*4+2] = bc;

		mc[0*4+0] = cc;
		mc[1*4+0] = cs;
		mc[0*4+1] = -cs;
		mc[1*4+1] = cc;

		Matrix4f ret = ma * mb * mc;
		return ret;
	}

	/****************************************************************/
	/* 移動行列生成												 */
	/* @param x X - direction translation						   */
	/* @param y Y - direction translation						   */
	/* @param z Z - direction translation						   */
	/* @param w for W - coordinate translation(implicitly set to 1) */
	/****************************************************************/
	Matrix4f MatVec::createTranslation(float x, float y, float z, float w) {
		Matrix4f ret;
		ret.setIdentity();
		ret[3 * 4 + 0] = x;
		ret[3 * 4 + 1] = y;
		ret[3 * 4 + 2] = z;
		ret[3 * 4 + 3] = w;
		return ret;
	}

	/***********************************************************/
	/* 拡縮行列生成											*/
	/* being values of matrix main diagonal.				   */
	/* @param sx Scale in X-axis							   */
	/* @param sy Scale in Y-axis							   */
	/* @param sz Scale in Z-axis							   */
	/* @return Transform matrix 4x4 with scale transformation. */
	/***********************************************************/
	Matrix4f MatVec::createScale(float sx, float sy, float sz) {
		Matrix4f ret;
		ret.setIdentity();
		ret[0 * 4 + 0] = sx;
		ret[1 * 4 + 1] = sy;
		ret[2 * 4 + 2] = sz;
		return ret;
	}

	/***************************************************************************/
	/* カメラビュー行列生成													*/
	/* @param eyePos A position of camera									  */
	/* @param centerPos A position where camera looks-at					   */
	/* @param upDir Direction of up vector									 */
	/* @return Resulting view matrix that looks from and at specific position. */
	/***************************************************************************/
	Matrix4f MatVec::createLookAt(const Vector3f &eyePos, const Vector3f &centerPos, const Vector3f &upDir) {

		Vector3f forward = centerPos - eyePos;
		Vector3f up = upDir;

		forward.normalize();

		// Side = forward x up
		Vector3f side = forward.CrossProduct(up);
		side.normalize();

		// Recompute up as: up = side x forward
		up = side.CrossProduct(forward);

		Matrix4f m;
		m[0 * 4 + 0] = side.x;
		m[1 * 4 + 0] = side.y;
		m[2 * 4 + 0] = side.z;

		m[0 * 4 + 1] = up.x;
		m[1 * 4 + 1] = up.y;
		m[2 * 4 + 1] = up.z;

		m[0 * 4 + 2] = -forward.x;
		m[1 * 4 + 2] = -forward.y;
		m[2 * 4 + 2] = -forward.z;

		m = m * createTranslation(-eyePos.x, -eyePos.y, -eyePos.z);
		return m;
	}

	/**
	* Creates OpenGL compatible perspective projection according specified frustum parameters.
	*
	* @param left Specify the coordinate for the left vertical clipping plane,
	* @param right Specify the coordinate for the right vertical clipping plane.
	* @param bottom Specify the coordinate for the bottom horizontal clipping plane,
	* @param top Specify the coordinate for the top horizontal clipping plane.
	* @param zNear Specify the distance to the near clipping plane.  Distance must be positive.
	* @param zFar Specify the distance to the far depth clipping plane.  Distance must be positive.
	*
	* @return Projection matrix for specified frustum.
	*/
	Matrix4f MatVec::createFrustum(float left, float right, float bottom, float top, float zNear, float zFar) {
		/*
		*
		  2 zNear
		------------	0			A			0
		right - left

				   2 zNear
		0		 ------------		B			0
				  top - bottom

		0				0			C			D

		0				0			-1			0

		A = (right + left) / (right - left)
		B = (top + bottom) / (top - bottom)
		C = - (zFar + zNear) / (zFar - zNear)
		D = - (2 zFar zNear) / (zFar - zNear)
		*
		*/
		Matrix4f ret;

		const float invWidth = 1.0f / (right- left);
		const float invHeight= 1.0f / (top  - bottom);
		const float invDepth = 1.0f / (zFar - zNear);

		const float twoZNear = 2.0f * zNear;

		ret[0 * 4 + 0] = twoZNear * invWidth;
		ret[1 * 4 + 1] = twoZNear * invHeight;

		ret[2 * 4 + 0] = (right + left) * invWidth;
		ret[2 * 4 + 1] = (top + bottom) * invHeight;
		ret[2 * 4 + 2] = -(zFar + zNear) * invDepth;
		ret[2 * 4 + 3] = -1;

		ret[3 * 4 + 2] = -twoZNear * zFar * invDepth;

		return ret;
	}

	/**
	* Creates OpenGL compatible orthographic projection matrix.
	* @param left Specify the coordinate for the left vertical clipping plane,
	* @param right Specify the coordinate for the right vertical clipping plane.
	* @param bottom Specify the coordinate for the bottom horizontal clipping plane,
	* @param top Specify the coordinate for the top horizontal clipping plane.
	* @param zNear Specify the distance to the nearer depth clipping plane.
	*	   This value is negative if the plane is to be behind the viewer,
	* @param zFar Specify the distance to the farther depth clipping plane.
	*	   This value is negative if the plane is to be behind the viewer.
	* @return Othrographic projection matrix.
	*/
	Matrix4f MatVec::createOrtho(float left, float right, float bottom, float top, float zNear, float zFar) {
		/*
			 2
		------------   0			0			tx
		right - left

					   2
		0		 ------------		0			ty
				   top - bottom

									 -2
		0			  0		 	------------	tz
								  zFar-zNear

		0			  0				0			1

		where
		tx = - (right + left) / (right - left)
		ty = - (top + bottom) / (top - bottom)
		tz = - (zFar + zNear) / (zFar - zNear)
		*/

		const float invWidth = 1.0f / (right - left);
		const float invHeight= 1.0f / (top - bottom);
		const float invDepth = 1.0f / (zFar - zNear);

		Matrix4f ret;

		ret[0 * 4 + 0] =  2.0f * invWidth;
		ret[1 * 4 + 1] =  2.0f * invHeight;
		ret[2 * 4 + 2] = -2.0f * invDepth;

		ret[3 * 4 + 0] = -(right + left) * invWidth;
		ret[3 * 4 + 1] = -(top + bottom) * invHeight;
		ret[3 * 4 + 2] = -(zFar + zNear) * invDepth;

		return ret;
	}

	Matrix4f MatVec::createAxisConversion(Axis fromfront, Axis fromup, Axis tofront, Axis toup) {
		Matrix4f ret;
		ret.setIdentity();

		if (fromfront == tofront && fromup == toup) {
			ret.setIdentity();
			return ret;
		}

		/* 同軸変換のチェック(2軸が同じ軸上になる変換は不可なので) */
		assert(isSameAxle(fromfront, fromup) != true && "aaaaa2軸が同じ軸上になる変換は不可");
		assert(isSameAxle(tofront,   toup  ) != true && "aaaaa2軸が同じ軸上になる変換は不可");

		const std::map<Axis, int> _axis_convert_num = { {Axis::X, 0}, {Axis::Y, 1}, {Axis::Z, 2}, {Axis::_X, 3}, {Axis::_Y, 4}, {Axis::_Z, 5} };
		const std::vector<Axis> AxisList = { fromfront, fromup, tofront, toup };

		int serchvalue = 0;
		for (size_t lpct = 0; lpct < AxisList.size(); lpct++) {
			serchvalue |= _axis_convert_num.at(AxisList[lpct]) << (lpct * 3);
		}

		for (size_t lpct = 0; lpct < _axis_convert_lut.size(); lpct++) {
			const std::vector<int> &vec = _axis_convert_lut[lpct];
			std::vector<int>::const_iterator itr = std::find(vec.begin(), vec.end(), serchvalue);
			if (itr != vec.end()) {
				/* 見つかったのでその行列が該当 */
				auto saxis_convert_matrix = (const float(*)[3])&(_axis_convert_matrix[lpct]);
				/* 戻り値に設定 */
				for (int lpct = 0; lpct < 16; lpct++) {
					int rowi = lpct / 4;
					int colj = lpct % 4;
					if (rowi == 3 && colj==3)   ret.mM[lpct] = 1.0f;
					else if(colj == 3)		  ret.mM[lpct] = 0.0f;
					else if(colj == 3)		  ret.mM[lpct] = 0.0f;
					else						ret.mM[lpct] = saxis_convert_matrix[rowi][colj];
				}
			}
		}
		return ret;
	}

	Matrix4f MatVec::createNormalize(const Matrix4f &mat) {
		Matrix4f ret;

		const std::array<float, 16> &M = mat.mM;
		/* 2乗和の√を求める */
		float norm0 = std::sqrt(M[0] * M[0] + M[4] * M[4] + M[ 8] * M[ 8]);
		float norm1 = std::sqrt(M[1] * M[1] + M[5] * M[5] + M[ 9] * M[ 9]);
		float norm2 = std::sqrt(M[2] * M[2] + M[6] * M[6] + M[10] * M[10]);

		std::array<float, 16> &rM = ret.mM;
		rM[ 0] = M[ 0] / norm0;	rM[ 1] = M[ 1] / norm1;	rM[ 2] = M[ 2] / norm2;	rM[ 3] = M[ 3];
		rM[ 4] = M[ 4] / norm0;	rM[ 5] = M[ 5] / norm1;	rM[ 6] = M[ 6] / norm2;	rM[ 7] = M[ 7];
		rM[ 8] = M[ 8] / norm0;	rM[ 9] = M[ 9] / norm1;	rM[10] = M[10] / norm2;	rM[11] = M[11];
		rM[12] = M[12] / norm0;	rM[13] = M[13] / norm1;	rM[14] = M[14] / norm2;	rM[15] = M[15];
		return ret;
	}
}   /* namespace m */

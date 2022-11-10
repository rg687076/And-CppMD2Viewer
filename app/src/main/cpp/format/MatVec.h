#pragma warning(disable:4819)
//
// Created by jun on 2016/11/20.
//
#ifndef MQOVIEWER_TKSMATRIXVECTOR_H
#define MQOVIEWER_TKSMATRIXVECTOR_H

#include <array>
#include <stdio.h>
#include "../CG3DCom.h"
#ifdef __ANDROID__
#include <android/log.h>
#endif /* __ANDROID__ */

namespace m {
	/************/
	/* Vector2i */
	/************/
	class Vector2i
	{
	public:
		union { int x; int s; int u; };
		union { int y; int t; int v; };

	public:
		Vector2i() : x(0), y(0) {}
		Vector2i(int ax, int ay) : x(ax), y(ay) {}
		Vector2i(const Vector2i &src) : x(src.x), y(src.y) {}
		~Vector2i() {};
		/* =演算子 */
	//	Vector2i& operator=(const Vector2i &rhs);
		/* 添え字演算子 */
		int operator[](int n);
		/* 単項演算子 */
		Vector2i& operator+=(const Vector2i &v);
		Vector2i& operator-=(const Vector2i &v);
		Vector2i& operator*=(int k);
		Vector2i& operator/=(int k);
		Vector2i operator+()const;
		Vector2i operator-()const;
		int DotProduct(const Vector2i &v);
		//	Vector2i& CrossProduct(const Vector2i &v);
			/* 比較演算子 */
		bool operator==(const Vector2i &v) const;
		bool operator!=(const Vector2i &v) const;
		/* べクトルの大きさ */
		int magnitude() const;
		int power() const;
		/* 正規化 */
		void normalize();
	};
	/* 2項演算子 */
	Vector2i operator+(const Vector2i &u, const Vector2i &v);	/* Vector2i+Vector2i */
	Vector2i operator-(const Vector2i &u, const Vector2i &v);	/* Vector2i-Vector2i */
	Vector2i operator*(int k, const  Vector2i &v);					/* int*Vector2i */
	Vector2i operator*(const Vector2i &v, int k);					/* Vector2i*int */
	Vector2i operator/(const Vector2i &v, int k);					/* Vector2i/int */
	int operator*(const Vector2i &u, const Vector2i &v);			/* 内積 Vector2i*Vector2i */
	int DotProduct(const Vector2i &u, const Vector2i &v);			/* 内積 DotProduct(Vector2i,Vector2i) */
	//Vector2i CrossProduct(const Vector2i &u, const Vector2i &v);	/* 外積 CrossProduct(Vector2i,Vector2i) */
	int angle(const Vector2i &u, const Vector2i &v);				/* 2つのベクトルのなす角度 */

	/************/
	/* Vector2f */
	/************/
	class Vector2f
	{
	public:
		union { float x; float s; float u; };
		union { float y; float t; float v; };

	public:
		Vector2f(): x(0), y(0){}
		Vector2f(float ax, float ay): x(ax), y(ay){}
		Vector2f(const Vector2f &src) : x(src.x), y(src.y) {}
		~Vector2f() {};
		/* =演算子 */
		Vector2f &operator=(const Vector2f &rhs);
		/* 添え字演算子 */
		float operator[](int n);
		/* 単項演算子 */
		Vector2f &operator+=(const Vector2f &v);
		Vector2f &operator-=(const Vector2f &v);
		Vector2f &operator*=(float k);
		Vector2f &operator/=(float k);
		Vector2f operator+()const;
		Vector2f operator-()const;
		float DotProduct(const Vector2f &v);
	//	Vector2f &CrossProduct(const Vector2f &v);
		/* 比較演算子 */
		bool operator==(const Vector2f &v) const;
		bool operator!=(const Vector2f &v) const;
		/* べクトルの大きさ */
		float magnitude() const;
		float power() const;
		/* 正規化 */
		void normalize();
	};
	/* 2項演算子 */
	Vector2f operator+(const Vector2f &u, const Vector2f &v);		/* Vector2f+Vector2f */
	Vector2f operator-(const Vector2f &u, const Vector2f &v);		/* Vector2f-Vector2f */
	Vector2f operator*(float k, const  Vector2f &v);					/* float*Vector2f */
	Vector2f operator*(const Vector2f &v, float k);						/* Vector2f*float */
	Vector2f operator/(const Vector2f &v, float k);						/* Vector2f/float */
	float operator*(const Vector2f &u, const Vector2f &v);				/* 内積 Vector2f*Vector2f */
	float DotProduct(const Vector2f &u, const Vector2f &v);				/* 内積 DotProduct(Vector2f,Vector2f) */
	//Vector2f CrossProduct(const Vector2f &u, const Vector2f &v);	/* 外積 CrossProduct(Vector2f,Vector2f) */
	float angle(const Vector2f &u, const Vector2f &v);					/* 2つのベクトルのなす角度 */

	/************/
	/* Vector3i */
	/************/
	class Vector3i
	{
	public:
		union { int x; int s; int r; };
		union { int y; int t; int b; };
		union { int z; int u; int g; };

	public:
		Vector3i(): x(0), y(0), z(0) {}
		Vector3i(int ax, int ay, int az): x(ax), y(ay), z(az) {}
		Vector3i(const Vector3i &src): x(src.x), y(src.y), z(src.z) {}
		~Vector3i(){};
		/* =演算子 */
		Vector3i &operator=(const Vector3i &rhs);
		/* 添え字演算子 */
		int &operator[](int n);
		/* 単項演算子 */
		Vector3i &operator+=(const Vector3i &v);
		Vector3i &operator-=(const Vector3i &v);
		Vector3i &operator*=(int k);
		Vector3i &operator/=(int k);
		Vector3i operator+()const;
		Vector3i operator-()const;
		int DotProduct(const Vector3i &v);
		Vector3i CrossProduct(const Vector3i &v);
		/* 比較演算子 */
		bool operator==(const Vector3i &v) const;
		bool operator!=(const Vector3i &v) const;
		/* べクトルの大きさ */
		int magnitude() const;
		int power() const;
		/* 正規化 */
		void normalize();
	};
	/* 2項演算子 */
	Vector3i operator+(const Vector3i &u, const Vector3i &v);	/* Vector3i+Vector3i */
	Vector3i operator-(const Vector3i &u, const Vector3i &v);	/* Vector3i-Vector3i */
	Vector3i operator*(int k, const  Vector3i &v);					/* int*Vector3i */
	Vector3i operator*(const Vector3i &v, int k);					/* Vector3i*int */
	Vector3i operator*(const Vector3i &v0, const Vector3i &v1);	/* Vector3i*Vector3i */
	Vector3i operator/(const Vector3i &v, int k);					/* Vector3i/int */
	int DotProduct(const Vector3i &u, const Vector3i &v);			/* 内積 DotProduct(Vector3i,Vector3i) */
	Vector3i CrossProduct(const Vector3i &u, const Vector3i &v);/* 外積 CrossProduct(Vector3i,Vector3i) */
	int angle(const Vector3i &u, const Vector3i &v);				/* 2つのベクトルのなす角度 */

	/************/
	/* Vector3f */
	/************/
	class Vector3f
	{
	public:
		union { float x; float s; float r; };
		union { float y; float t; float b; };
		union { float z; float u; float g; };

	public:
		Vector3f(): x(0), y(0), z(0) {}
		Vector3f(float ax, float ay, float az): x(ax), y(ay), z(az) {}
		Vector3f(const Vector3f &src): x(src.x), y(src.y), z(src.z) {}
		~Vector3f(){};
		/* =演算子 */
	//	Vector3f &operator=(const Vector3f &rhs);
		/* 添え字演算子 */
		float &operator[](int n);
		/* 単項演算子 */
		Vector3f &operator+=(const Vector3f &v);
		Vector3f &operator-=(const Vector3f &v);
		Vector3f &operator*=(float k);
		Vector3f &operator/=(float k);
		Vector3f operator+()const;
		Vector3f operator-()const;
		float DotProduct(const Vector3f &v);
		Vector3f CrossProduct(const Vector3f &v);
		/* 比較演算子 */
		bool operator==(const Vector3f &v) const;
		bool operator!=(const Vector3f &v) const;
		/* べクトルの大きさ */
		float magnitude() const;
		float power() const;
		/* 正規化 */
		void normalize();
	};
	/* 2項演算子 */
	Vector3f operator+(const Vector3f &u, const Vector3f &v);		/* Vector3f+Vector3f */
	Vector3f operator-(const Vector3f &u, const Vector3f &v);		/* Vector3f-Vector3f */
	Vector3f operator*(float k, const  Vector3f &v);					/* float*Vector3f */
	Vector3f operator*(const Vector3f &v, float k);					/* Vector3f*float */
	Vector3f operator*(const Vector3f &v0, const Vector3f &v1);	/* Vector3f*Vector3f */
	Vector3f operator/(const Vector3f &v, float k);					/* Vector3f/float */
	float DotProduct(const Vector3f &u, const Vector3f &v);			/* 内積 DotProduct(Vector3f,Vector3f) */
	Vector3f CrossProduct(const Vector3f &u, const Vector3f &v);	/* 外積 CrossProduct(Vector3f,Vector3f) */
	float angle(const Vector3f &u, const Vector3f &v);				/* 2つのベクトルのなす角度 */

	/************/
	/* Vector4i */
	/************/
	class Vector4i
	{
	public:
		union { float x; float r; float diffuse; };
		union { float y; float g; float ambient; };
		union { float z; float b; float emission; };
		union { float w; float a; float specular; };

	public:
		Vector4i(): x(0), y(0), z(0), w(0) {}
		Vector4i(float ax, float ay, float az, float aw): x(ax), y(ay), z(az), w(aw) {}
		Vector4i(const Vector4i &src): x(src.x), y(src.y), z(src.z), w(src.w) {}
		~Vector4i(){};
		/* =演算子 */
	//	Vector4i &operator=(const Vector4i &rhs);
		/* 添え字演算子 */
		float &operator[](int n);
		/* 単項演算子 */
		Vector4i &operator+=(const Vector4i &v);
		Vector4i &operator-=(const Vector4i &v);
		Vector4i &operator*=(float k);
		Vector4i &operator/=(float k);
		Vector4i operator+()const;
		Vector4i operator-()const;
		float DotProduct(const Vector4i &v);
	//	Vector4i &CrossProduct(const Vector4i &v);
		/* 比較演算子 */
		bool operator==(const Vector4i &v) const;
		bool operator!=(const Vector4i &v) const;
		/* べクトルの大きさ */
		float magnitude() const;
		float power() const;
		/* 正規化 */
		void normalize();
	};
	/* 2項演算子 */
	Vector4i operator+(const Vector4i &u, const Vector4i &v);		/* Vector4i+Vector4i */
	Vector4i operator-(const Vector4i &u, const Vector4i &v);		/* Vector4i-Vector4i */
	Vector4i operator*(float k, const  Vector4i &v);				/* float*Vector4i */
	Vector4i operator*(const Vector4i &v, float k);					/* Vector4i*float */
	Vector4i operator/(const Vector4i &v, float k);					/* Vector4i/float */
	float operator*(const Vector4i &u, const Vector4i &v);			/* 内積 Vector4i*Vector4i */
	float DotProduct(const Vector4i &u, const Vector4i &v);			/* 内積 DotProduct(Vector4i,Vector4i) */
	//Vector3f CrossProduct(const Vector3f &u, const Vector3f &v)	;/* 外積 CrossProduct(Vector4i) */
	float angle(const Vector4i &u, const Vector4i &v);				/* 2つのベクトルのなす角度 */

	/************/
	/* Vector4f */
	/************/
	class Vector4f
	{
	public:
		union { float x; float r; float diffuse; };
		union { float y; float g; float ambient; };
		union { float z; float b; float emission; };
		union { float w; float a; float specular; };

	public:
		Vector4f(): x(0), y(0), z(0), w(0) {}
		Vector4f(float ax, float ay, float az, float aw): x(ax), y(ay), z(az), w(aw) {}
		Vector4f(const Vector4f &src): x(src.x), y(src.y), z(src.z), w(src.w) {}
		~Vector4f(){};
		/* =演算子 */
		Vector4f &operator=(const Vector4f &rhs);
		/* 添え字演算子 */
		float &operator[](int n);
		/* 単項演算子 */
		Vector4f &operator+=(const Vector4f &v);
		Vector4f &operator-=(const Vector4f &v);
		Vector4f &operator*=(float k);
		Vector4f &operator/=(float k);
		Vector4f operator+()const;
		Vector4f operator-()const;
		float DotProduct(const Vector4f &v);
	//	Vector4f &CrossProduct(const Vector4f &v);
		/* 比較演算子 */
		bool operator==(const Vector4f &v) const;
		bool operator!=(const Vector4f &v) const;
		/* べクトルの大きさ */
		float magnitude() const;
		float power() const;
		/* 正規化 */
		void normalize();
	};
	/* 2項演算子 */
	Vector4f operator+(const Vector4f &u, const Vector4f &v);		/* Vector4f+Vector4f */
	Vector4f operator-(const Vector4f &u, const Vector4f &v);		/* Vector4f-Vector4f */
	Vector4f operator*(float k, const  Vector4f &v);					/* float*Vector4f */
	Vector4f operator*(const Vector4f &v, float k);					/* Vector4f*float */
	Vector4f operator/(const Vector4f &v, float k);					/* Vector4f/float */
	float operator*(const Vector4f &u, const Vector4f &v);			/* 内積 Vector4f*Vector4f */
	float DotProduct(const Vector4f &u, const Vector4f &v);			/* 内積 DotProduct(Vector4f,Vector4f) */
	//Vector3f CrossProduct(const Vector3f &u, const Vector3f &v);/* 外積 CrossProduct(Vector4f) */
	float angle(const Vector4f &u, const Vector4f &v);				/* 2つのベクトルのなす角度 */

	typedef Vector4f Reflection4;
	typedef Vector4f Color4;
	typedef Vector2f UV;

	/* 3角形ポリゴン情報構造体 */
	typedef struct TRIANGLE {
		int         MaterialID = 0; /* マテリアルNo. */
		int         Index[3] = {};  /* インデックス */
		Vector3f  Normal = {};    /* 法線 */
		UV          uv[3] = {};     /* UV情報 */
	} Triangle;

	/* 4角形ポリゴン情報構造体 */
	typedef struct QUAD {
		int         MaterialID = 0; /* マテリアルNo. */
		int         Index[4] = {};  /* インデックス */
		Vector3f  Normal = {};    /* 法線 */
		UV          uv[4] = {};     /* UV情報 */
	} Quad;

	/************/
	/* Matrix4f */
	/************/
	class Matrix4f
	{
	public:
		static const std::array<float, 16> IDENTITY;
		std::array<float, 16> mM ={0};

	public:
		int cnt;
		Matrix4f() : cnt(rand()) { __android_log_print(ANDROID_LOG_DEBUG, "aaaaa", "aaaaa constructor%d %s() s %d", cnt, __func__, __LINE__); }
		Matrix4f(const Matrix4f &src) : cnt(rand()) { mM = src.mM; __android_log_print(ANDROID_LOG_DEBUG, "aaaaa", "aaaaa copy-constructor%d %s() s %d", cnt, __func__, __LINE__); }
		Matrix4f(const Matrix4f &&src) : mM(src.mM), cnt(rand()) { __android_log_print(ANDROID_LOG_DEBUG, "aaaaa", "aaaaa move-constructor%d %s() s %d", cnt, __func__, __LINE__); }
		Matrix4f(const std::array<float, 16> &src) : mM(IDENTITY), cnt(rand()) { __android_log_print(ANDROID_LOG_DEBUG, "aaaaa", "aaaaa arg-constructor%d %s() s %d", cnt, __func__, __LINE__); }
		Matrix4f(float a0, float a1, float a2, float a3, float a4, float a5, float a6, float a7, float a8, float a9, float a10, float a11, float a12, float a13, float a14, float a15);
		~Matrix4f(){ __android_log_print(ANDROID_LOG_DEBUG, "aaaaa", "aaaaa destructor%d %s() s %d", cnt, __func__, __LINE__); };
		void setIdentity();
		void normalize();		/* 行列正規化 */
		/* =演算子 */
		Matrix4f &operator=(const Matrix4f &rhs);
		/* 単項演算子 */
		Matrix4f &operator+=(const Matrix4f &v);
		Matrix4f &operator-=(const Matrix4f &v);
		Matrix4f &operator*=(float k);
		Matrix4f &operator/=(float k);
		Matrix4f &operator*=(const Matrix4f &v);
	//	Matrix4f &operator/=(const Matrix4f &v);
		float det();				/* 行列式 */
		Matrix4f operator+() const;
		Matrix4f operator-() const;
		/* 添え字演算子 */
		float &operator[](int i);
		/* 比較演算子 */
		bool operator==(const Matrix4f &v) const;
		bool operator!=(const Matrix4f &v) const;
		/* ベクトル設定/取得 */
		void setTranslation(const Vector3f &sv);
		Matrix4f getInverse();		/* 逆行列 */
		Matrix4f getTrancepose();	/* 転置行列 */
		Vector3f getTranslation() const;
	//	void setRotation(const Vector3f &m);
	//	Vector3f getRotation() const;
		void setScale(float s);
		void setScale(float sx, float sy, float sz);
		void setScale(const Vector3f &s);
		Vector3f getScale() const;
	};
	/* 2項演算子 */
	Matrix4f operator+(const Matrix4f &u, const Matrix4f &v);/* Vector4f+Vector4f */
	Matrix4f operator-(const Matrix4f &u, const Matrix4f &v);/* Vector4f-Vector4f */
	Matrix4f operator*(float k, const  Matrix4f &v);				/* float*Vector4f */
	Matrix4f operator*(const Matrix4f &v, float k);				/* Vector4f*float */
	Matrix4f operator/(const Matrix4f &v, float k);				/* Vector4f/float */
	Matrix4f operator*(const Matrix4f &u, const Matrix4f &v);/* Vector4f*Vector4f */
	//Matrix4f operator/(const Matrix4f &u, const Matrix4f &v);		/* Vector4f/Vector4f */

	Vector2f operator*(const Matrix4f &m, const Vector2f &v);		/* Matrix4f*Vector2f */
	Vector3f operator*(const Matrix4f &m, const Vector3f &v);		/* Matrix4f*Vector3f */
	Vector4f operator*(const Matrix4f &m, const Vector4f &v);		/* Matrix4f*Vector4f */

	enum class Axis : char {
		 X = 'X',
		_X = 'x',
		 Y = 'Y',
		_Y = 'y',
		 Z = 'Z',
		_Z = 'z',
	};
	bool isSameAxle(const Axis &a1, const Axis &a2);  /* 同軸チェック */

	//# store args as a single int
	//# (X Y Z -X -Y -Z) --> (0, 1, 2, 3, 4, 5)
	//# each value is((src_forward, src_up), (dst_forward, dst_up))
	//# where all 4 values are or 'd into a single value...
	//#    (i1<<0 | i1<<3 | i1<<6 | i1<<9)

	const std::vector<std::vector<int>> _axis_convert_lut = {
		{ 0x8C8, 0x4D0, 0x2E0, 0xAE8, 0x701, 0x511, 0x119, 0xB29, 0x682, 0x88A, 0x09A, 0x2A2, 0x80B, 0x413, 0x223, 0xA2B, 0x644, 0x454, 0x05C, 0xA6C, 0x745, 0x94D, 0x15D, 0x365 },
		{ 0xAC8, 0x8D0, 0x4E0, 0x2E8, 0x741, 0x951, 0x159, 0x369, 0x702, 0xB0A, 0x11A, 0x522, 0xA0B, 0x813, 0x423, 0x22B, 0x684, 0x894, 0x09C, 0x2AC, 0x645, 0xA4D, 0x05D, 0x465 },
		{ 0x4C8, 0x2D0, 0xAE0, 0x8E8, 0x681, 0x291, 0x099, 0x8A9, 0x642, 0x44A, 0x05A, 0xA62, 0x40B, 0x213, 0xA23, 0x82B, 0x744, 0x354, 0x15C, 0x96C, 0x705, 0x50D, 0x11D, 0xB25 },
		{ 0x2C8, 0xAD0, 0x8E0, 0x4E8, 0x641, 0xA51, 0x059, 0x469, 0x742, 0x34A, 0x15A, 0x962, 0x20B, 0xA13, 0x823, 0x42B, 0x704, 0xB14, 0x11C, 0x52C, 0x685, 0x28D, 0x09D, 0x8A5 },
		{ 0x708, 0xB10, 0x120, 0x528, 0x8C1, 0xAD1, 0x2D9, 0x4E9, 0x942, 0x74A, 0x35A, 0x162, 0x64B, 0xA53, 0x063, 0x46B, 0x804, 0xA14, 0x21C, 0x42C, 0x885, 0x68D, 0x29D, 0x0A5 },
		{ 0xB08, 0x110, 0x520, 0x728, 0x941, 0x151, 0x359, 0x769, 0x802, 0xA0A, 0x21A, 0x422, 0xA4B, 0x053, 0x463, 0x66B, 0x884, 0x094, 0x29C, 0x6AC, 0x8C5, 0xACD, 0x2DD, 0x4E5 },
		{ 0x508, 0x710, 0xB20, 0x128, 0x881, 0x691, 0x299, 0x0A9, 0x8C2, 0x4CA, 0x2DA, 0xAE2, 0x44B, 0x653, 0xA63, 0x06B, 0x944, 0x754, 0x35C, 0x16C, 0x805, 0x40D, 0x21D, 0xA25 },
		{ 0x108, 0x510, 0x720, 0xB28, 0x801, 0x411, 0x219, 0xA29, 0x882, 0x08A, 0x29A, 0x6A2, 0x04B, 0x453, 0x663, 0xA6B, 0x8C4, 0x4D4, 0x2DC, 0xAEC, 0x945, 0x14D, 0x35D, 0x765 },
		{ 0x748, 0x350, 0x160, 0x968, 0xAC1, 0x2D1, 0x4D9, 0x8E9, 0xA42, 0x64A, 0x45A, 0x062, 0x68B, 0x293, 0x0A3, 0x8AB, 0xA04, 0x214, 0x41C, 0x82C, 0xB05, 0x70D, 0x51D, 0x125 },
		{ 0x948, 0x750, 0x360, 0x168, 0xB01, 0x711, 0x519, 0x129, 0xAC2, 0x8CA, 0x4DA, 0x2E2, 0x88B, 0x693, 0x2A3, 0x0AB, 0xA44, 0x654, 0x45C, 0x06C, 0xA05, 0x80D, 0x41D, 0x225 },
		{ 0x348, 0x150, 0x960, 0x768, 0xA41, 0x051, 0x459, 0x669, 0xA02, 0x20A, 0x41A, 0x822, 0x28B, 0x093, 0x8A3, 0x6AB, 0xB04, 0x114, 0x51C, 0x72C, 0xAC5, 0x2CD, 0x4DD, 0x8E5 },
		{ 0x148, 0x950, 0x760, 0x368, 0xA01, 0x811, 0x419, 0x229, 0xB02, 0x10A, 0x51A, 0x722, 0x08B, 0x893, 0x6A3, 0x2AB, 0xAC4, 0x8D4, 0x4DC, 0x2EC, 0xA45, 0x04D, 0x45D, 0x665 },
		{ 0x688, 0x890, 0x0A0, 0x2A8, 0x4C1, 0x8D1, 0xAD9, 0x2E9, 0x502, 0x70A, 0xB1A, 0x122, 0x74B, 0x953, 0x163, 0x36B, 0x404, 0x814, 0xA1C, 0x22C, 0x445, 0x64D, 0xA5D, 0x065 },
		{ 0x888, 0x090, 0x2A0, 0x6A8, 0x501, 0x111, 0xB19, 0x729, 0x402, 0x80A, 0xA1A, 0x222, 0x94B, 0x153, 0x363, 0x76B, 0x444, 0x054, 0xA5C, 0x66C, 0x4C5, 0x8CD, 0xADD, 0x2E5 },
		{ 0x288, 0x690, 0x8A0, 0x0A8, 0x441, 0x651, 0xA59, 0x069, 0x4C2, 0x2CA, 0xADA, 0x8E2, 0x34B, 0x753, 0x963, 0x16B, 0x504, 0x714, 0xB1C, 0x12C, 0x405, 0x20D, 0xA1D, 0x825 },
		{ 0x088, 0x290, 0x6A0, 0x8A8, 0x401, 0x211, 0xA19, 0x829, 0x442, 0x04A, 0xA5A, 0x662, 0x14B, 0x353, 0x763, 0x96B, 0x4C4, 0x2D4, 0xADC, 0x8EC, 0x505, 0x10D, 0xB1D, 0x725 },
		{ 0x648, 0x450, 0x060, 0xA68, 0x2C1, 0x4D1, 0x8D9, 0xAE9, 0x282, 0x68A, 0x89A, 0x0A2, 0x70B, 0x513, 0x123, 0xB2B, 0x204, 0x414, 0x81C, 0xA2C, 0x345, 0x74D, 0x95D, 0x165 },
		{ 0xA48, 0x650, 0x460, 0x068, 0x341, 0x751, 0x959, 0x169, 0x2C2, 0xACA, 0x8DA, 0x4E2, 0xB0B, 0x713, 0x523, 0x12B, 0x284, 0x694, 0x89C, 0x0AC, 0x205, 0xA0D, 0x81D, 0x425 },
		{ 0x448, 0x050, 0xA60, 0x668, 0x281, 0x091, 0x899, 0x6A9, 0x202, 0x40A, 0x81A, 0xA22, 0x50B, 0x113, 0xB23, 0x72B, 0x344, 0x154, 0x95C, 0x76C, 0x2C5, 0x4CD, 0x8DD, 0xAE5 },
		{ 0x048, 0xA50, 0x660, 0x468, 0x201, 0xA11, 0x819, 0x429, 0x342, 0x14A, 0x95A, 0x762, 0x10B, 0xB13, 0x723, 0x52B, 0x2C4, 0xAD4, 0x8DC, 0x4EC, 0x285, 0x08D, 0x89D, 0x6A5 },
		{ 0x808, 0xA10, 0x220, 0x428, 0x101, 0xB11, 0x719, 0x529, 0x142, 0x94A, 0x75A, 0x362, 0x8CB, 0xAD3, 0x2E3, 0x4EB, 0x044, 0xA54, 0x65C, 0x46C, 0x085, 0x88D, 0x69D, 0x2A5 },
		{ 0xA08, 0x210, 0x420, 0x828, 0x141, 0x351, 0x759, 0x969, 0x042, 0xA4A, 0x65A, 0x462, 0xACB, 0x2D3, 0x4E3, 0x8EB, 0x084, 0x294, 0x69C, 0x8AC, 0x105, 0xB0D, 0x71D, 0x525 },
		{ 0x408, 0x810, 0xA20, 0x228, 0x081, 0x891, 0x699, 0x2A9, 0x102, 0x50A, 0x71A, 0xB22, 0x4CB, 0x8D3, 0xAE3, 0x2EB, 0x144, 0x954, 0x75C, 0x36C, 0x045, 0x44D, 0x65D, 0xA65 },
	};

	const float _axis_convert_matrix[23][3][3] = {
		{{-1.0,  0.0,  0.0}, { 0.0, -1.0,  0.0}, { 0.0,  0.0,  1.0}},
		{{-1.0,  0.0,  0.0}, { 0.0,  0.0, -1.0}, { 0.0, -1.0,  0.0}},
		{{-1.0,  0.0,  0.0}, { 0.0,  0.0,  1.0}, { 0.0,  1.0,  0.0}},
		{{-1.0,  0.0,  0.0}, { 0.0,  1.0,  0.0}, { 0.0,  0.0, -1.0}},
		{{ 0.0, -1.0,  0.0}, {-1.0,  0.0,  0.0}, { 0.0,  0.0, -1.0}},
		{{ 0.0,  0.0,  1.0}, {-1.0,  0.0,  0.0}, { 0.0, -1.0,  0.0}},
		{{ 0.0,  0.0, -1.0}, {-1.0,  0.0,  0.0}, { 0.0,  1.0,  0.0}},
		{{ 0.0,  1.0,  0.0}, {-1.0,  0.0,  0.0}, { 0.0,  0.0,  1.0}},
		{{ 0.0, -1.0,  0.0}, { 0.0,  0.0,  1.0}, {-1.0,  0.0,  0.0}},
		{{ 0.0,  0.0, -1.0}, { 0.0, -1.0,  0.0}, {-1.0,  0.0,  0.0}},
		{{ 0.0,  0.0,  1.0}, { 0.0,  1.0,  0.0}, {-1.0,  0.0,  0.0}},
		{{ 0.0,  1.0,  0.0}, { 0.0,  0.0, -1.0}, {-1.0,  0.0,  0.0}},
		{{ 0.0, -1.0,  0.0}, { 0.0,  0.0, -1.0}, { 1.0,  0.0,  0.0}},
		{{ 0.0,  0.0,  1.0}, { 0.0, -1.0,  0.0}, { 1.0,  0.0,  0.0}},
		{{ 0.0,  0.0, -1.0}, { 0.0,  1.0,  0.0}, { 1.0,  0.0,  0.0}},
		{{ 0.0,  1.0,  0.0}, { 0.0,  0.0,  1.0}, { 1.0,  0.0,  0.0}},
		{{ 0.0, -1.0,  0.0}, { 1.0,  0.0,  0.0}, { 0.0,  0.0,  1.0}},
		{{ 0.0,  0.0, -1.0}, { 1.0,  0.0,  0.0}, { 0.0, -1.0,  0.0}},
		{{ 0.0,  0.0,  1.0}, { 1.0,  0.0,  0.0}, { 0.0,  1.0,  0.0}},
		{{ 0.0,  1.0,  0.0}, { 1.0,  0.0,  0.0}, { 0.0,  0.0, -1.0}},
		{{ 1.0,  0.0,  0.0}, { 0.0, -1.0,  0.0}, { 0.0,  0.0, -1.0}},
		{{ 1.0,  0.0,  0.0}, { 0.0,  0.0,  1.0}, { 0.0, -1.0,  0.0}},
		{{ 1.0,  0.0,  0.0}, { 0.0,  0.0, -1.0}, { 0.0,  1.0,  0.0}},
	};

	class MatVec {
	public:
		static void LoadIdentity(std::array<float, 16> &M);
		static Matrix4f LoadIdentity();
		static void LoadMatrix(std::array<float, 16> &retmat, const std::array<float, 16> &a);
		static void MultMatrixf(std::array<float, 16> &retmat, const std::array<float, 16> &m);
		static void MultMatrixf(std::array<float, 16> &retmat, const std::array<float, 16> &a, const std::array<float, 16> &m);
		static Matrix4f MultMatrix(const Matrix4f &a, const Matrix4f &m);
		static void normalize(std::array<float, 3> &v);
		static void cross(const std::array<float, 3> &v1, const std::array<float, 3> &v2, std::array<float, 3> &result);
		static std::array<float, 16> GetPerspectivef(float fovy, float aspect, float zNear, float zFar);
		static std::array<float, 16> GetLookAtf(float eyex, float eyey, float eyez, float tarx, float tary, float tarz, float upx, float upy, float upz);
		static void Translatef(std::array<float, 16> &retmat, float x, float y, float z);
		static void Rotatef(std::array<float, 16> &retmat, float angle, float x, float y, float z);
		static std::array<float, 16> GetRotatef(float angle, float x, float y, float z);
		static void Scalef(std::array<float, 16> &retmat, float x, float y, float z);
		static bool invertf(std::array<float, 16> &retmat, const std::array<float, 16> &matrix);
		static void transposef(std::array<float, 16> &retmat, const std::array<float, 16> &matrix);
		static Matrix4f createRotation(float xDeg, float yDeg, float zDeg);
		static Matrix4f createTranslation(float x, float y, float z, float w=1);
		static Matrix4f createScale(float sx, float sy, float sz);
		static Matrix4f createLookAt(const Vector3f &eyePos, const Vector3f &centerPos, const Vector3f &upDir);
		static Matrix4f createFrustum(float left, float right, float bottom, float top, float zNear, float zFar);
		static Matrix4f createOrtho(float left, float right, float bottom, float top, float zNear, float zFar);
		static Matrix4f createAxisConversion(Axis fromfront= Axis::Y, Axis fromup=Axis::Z, Axis tofront= Axis::Y, Axis toup= Axis::Z);
		static Matrix4f createNormalize(const Matrix4f &mat);
	};
} /* namespace m */

#endif //MQOVIEWER_TKSMATRIXVECTOR_H

#pragma warning(disable:4819)
//
// Created by jun on 2021/06/08.
//

#ifndef CG3DVIEWER_MQO_H
#define CG3DVIEWER_MQO_H
#include <vector>
#include <string>
#include <iterator>
#include <map>
#include "MatVec.h"
#include "../OpenGL/GLES2.h"

/* マテリアル管理の構造体 */
class MqoMaterial {
public:
	int MaterialID;						/* ID */
	std::string MaterialName;			/* マテリアル名 */
	m::Color4 Color;					/* カラー */
	m::Reflection4 ReflectionColor;	/* 反射 */
	float Power;						/* shiness */
	std::string TexName;				/* テクスチャ名(模様マッピング) */
	std::string AlpName;				/* 透明マッピング */
	std::string BmpName;				/* 凹凸マッピング ※読込むだけ、使い方がわからんから今回は非対応 */
	MqoMaterial():MaterialID(0),MaterialName(),Color(),ReflectionColor(),Power(0),TexName(),AlpName(),BmpName(){};
	MqoMaterial &operator=(const MqoMaterial &rhs) {
		this->MaterialID		= rhs.MaterialID;
		this->MaterialName		= rhs.MaterialName;
		this->Color				= rhs.Color;
		this->ReflectionColor	= rhs.ReflectionColor;
		this->Power				= rhs.Power;
		this->TexName			= rhs.TexName;
		this->AlpName			= rhs.AlpName;
		this->BmpName			= rhs.BmpName;
		return *this;
	};
	~MqoMaterial() {}
};

/* オブジェクト(メッシュ)管理の構造体 */
class MqoObject {
public:
	std::string Name;								/* オブジェクト名 */
	int		 visible;								/* 可視/不可視 */
	int		 shading;								/* 法線適用種別 */
	m::Vector4f  color;						/* カラー */
	std::vector<m::Vector3f>	Vertex;			/* 頂点データ */
	std::vector<m::Vector3f>	Normal;			/* 法線データ */
	std::vector<m::Triangle>		TriangleData;	/* 3角面データ */
	std::vector<m::Quad>			QuadData;		/* 4角面データ */
public:
	MqoObject():Name(),visible(0),shading(0),color(),Vertex(),Normal(),TriangleData(),QuadData(){};
	MqoObject &operator=(const MqoObject &rhs);
	~MqoObject() {Name.clear(); Vertex.clear(); Normal.clear();  TriangleData.clear();  QuadData.clear(); }
};

class MqoInfo {
public:
	std::vector<MqoMaterial>   mMqoMaterials;	/* マテリアル */
	std::vector<MqoObject>	 mMqoObjects;		/* オブジェクトデータ */
public:
	MqoInfo():mMqoMaterials(),mMqoObjects(){};
	MqoInfo &operator=(const MqoInfo &rhs) {
		this->mMqoMaterials.reserve(rhs.mMqoMaterials.size());
		std::copy(rhs.mMqoMaterials.begin(), rhs.mMqoMaterials.end(), std::back_inserter(this->mMqoMaterials));
		this->mMqoObjects.reserve(rhs.mMqoObjects.size());
		std::copy(rhs.mMqoObjects.begin(), rhs.mMqoObjects.end(), std::back_inserter(this->mMqoObjects));
		return *this;
	};
	~MqoInfo() {mMqoMaterials.clear(); mMqoObjects.clear(); }
};

class MQO {
public:
	static std::tuple<bool, MqoInfo> init(const std::vector<char> &MqoModelData);
	static bool remakeDrawInfo(MqoInfo &mqoinfo, std::vector<DrawInfo> &aDrawInfos);
	static bool TextureInit(const std::map<std::string, std::vector<char>> &AssetDatas, const std::vector<MqoMaterial> &aMqoMaterial, std::vector<DrawInfo> &DrawInfos);

private:
	static bool Load(const std::vector<char> &MqoModelData, MqoInfo &aMqoInfo);
	static void LoadMaterial(std::vector<std::string> &contents, unsigned int &lpct, std::vector<MqoMaterial> &aMaterial);
	static void LoadObject(std::vector<std::string> &contents, unsigned int &lpct, std::vector<MqoObject> &aObjects);
	static void LoadVertex(std::vector<std::string> &contents, unsigned int &lpct, std::vector<m::Vector3f> &aVertexs);
	static void LoadFace(std::vector<std::string> &contents, unsigned int &lpct, std::vector<m::Triangle> &aTriangles, std::vector<m::Quad> &aQuadRilaterals);
	static void setNormal(MqoObject &mqoobject);
	static m::Vector3f calcNormal(m::Vector3f &aV0, m::Vector3f &aV1, m::Vector3f &aV2);
	static float *vector2array(std::vector<m::Vector3f> &vector);
	static float *vector2array(std::vector<m::UV> &vector);
	static float *vector2array(std::vector<m::Color4> &vector);
};


#endif //CG3DVIEWER_MQO_H

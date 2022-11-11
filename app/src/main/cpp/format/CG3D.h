//
// Created by jun on 2021/06/23.
//
#ifndef ANDCG3DVIEWER_CG3D_H
#define ANDCG3DVIEWER_CG3D_H

#include <vector>
#include <array>
#include "MatVec.h"

namespace cg {
	class Face {
	public:
		m::Vector4i vs;
		short mat_nr;
		/** We keep edcode, for conversion to edges draw flags in old files. */
		char edcode, flag;
	};

	class Edge {
	public:
		m::Vector2i	Vertices;
		bool 		UseEdgeSharp = false;
		char		Crease = 0;
		char		bweight = 0;
		short		flag = 35;
	};

	class ColorLayer {
	public:
		std::string					Name;
		std::vector<m::Vector3i>	ColorData;
	};

	class UvLayer {
	public:
		std::string					Name;
		std::vector<m::Vector2f>	UvData;
	};

	class Vertex {
	public:
		m::Vector3f	Co;
		m::Vector3f	No;
		char		bweight;/* TODO 必要なのか？ */
		char		flag;	/* TODO 必要なのか？ */
	};

	class Loop {
	public:
		int			VertexIndex;
		int			EdgeIndex;
		m::Vector3f	Normal;
	};

	class Polygon {
	public:
		int					LoopStart;
		int					LoopTotal;
		std::vector<int>	LoopIndices;
		bool				UseSmooth;
		int					MaterialIndex;
		char				flag = 2;
		char				_pad = 0;
	};

	class SortPoly {
	public:
		std::uint32_t		index = 0;
		bool				invalid = false;
		std::vector<int>	verts;
		int					loopstart = 0;
	};

	class DeformWeight {
	public:
		/** The index for the vertex group, must *always* be unique when in an array. */
		unsigned int def_nr;
		/** Weight between 0.0 and 1.0. */
		float weight;
	};

	class DeformVertex {
	public:
		std::vector<DeformWeight> dws;
		/** Flag is only in use as a run-time tag at the moment. */
		int flag;
	};

	class Mesh {
	public:
		std::string					name;
		std::vector<Vertex>			Vertexs;
		std::vector<Loop>			Loops;
		std::vector<Polygon>		Polygons;
		UvLayer						UvLayer;
		ColorLayer					ColorLayers;
		std::vector<Edge>			Edges;
		std::vector<Face>			Faces;
		std::vector<DeformVertex>	DeformVerts;
		bool 						UseAutoSmooth;
		bool						UseCustomdataEdgeCrease;
	public:
		std::tuple<bool, bool> validateArrays(bool isCleanCustomdata);
	//	static	bool					validateAllCustomdata(std::vector<Vertex>& Vertexs, std::vector<Edge>& Edges, std::vector<Loop>& Loops, std::vector<Polygon>& Polygons, bool CheckMask, bool doVerbose, bool doFixes);
	//	static	std::tuple<bool, bool>	validateCustomdata(std::vector<Vertex>& vector, uint64_t vmask, bool verbose, bool fixes);
	private:
		static std::tuple<bool, bool> validateArrays(std::vector<Vertex> &Vertexs, std::vector<Edge> &Edges, std::vector<Face> &Faces, std::vector<Loop> &Loops, std::vector<Polygon> &Polygons, std::vector<DeformVertex> &DeformVertexs, bool doVerbose, bool doFixes);
	};

	class Material {
	public:
		std::string Name;
	public:
		bool operator<(const Material &material) const {
			return this->Name < material.Name;
		}
	};

	class CustomData_MeshMasks {
	public:
		uint64_t vmask;
		uint64_t emask;
		uint64_t fmask;
		uint64_t pmask;
		uint64_t lmask;
	};

	class Image {
	public:
		std::string			FbxFileName;
		std::string			Key;
		std::vector<char>	Img;
	};

	/* CustomData.type */
	typedef enum CustomDataType {
		/* Used by GLSL attributes in the cases when we need a delayed CD type
		 * assignment (in the cases when we don't know in advance which layer
		 * we are addressing).
		 */
		CD_AUTO_FROM_NAME = -1,

		CD_MVERT = 0,
	#ifdef DNA_DEPRECATED_ALLOW
		CD_MSTICKY = 1, /* DEPRECATED */
	#endif
		CD_MDEFORMVERT = 2,
		CD_MEDGE = 3,
		CD_MFACE = 4,
		CD_MTFACE = 5,
		CD_MCOL = 6,
		CD_ORIGINDEX = 7,
		CD_NORMAL = 8,
		CD_FACEMAP = 9, /* exclusive face group, each face can only be part of one */
		CD_PROP_FLOAT = 10,
		CD_PROP_INT32 = 11,
		CD_PROP_STRING = 12,
		CD_ORIGSPACE = 13, /* for modifier stack face location mapping */
		CD_ORCO = 14,      /* undeformed vertex coordinates, normalized to 0..1 range */
	#ifdef DNA_DEPRECATED_ALLOW
		CD_MTEXPOLY = 15, /* deprecated */
	#endif
		CD_MLOOPUV = 16,
		CD_MLOOPCOL = 17,
		CD_TANGENT = 18,
		CD_MDISPS = 19,
		CD_PREVIEW_MCOL = 20,           /* for displaying weightpaint colors */
		/*  CD_ID_MCOL          = 21, */
		/* CD_TEXTURE_MLOOPCOL = 22, */ /* UNUSED */
		CD_CLOTH_ORCO = 23,
		/* CD_RECAST = 24, */ /* UNUSED */

		/* BMESH ONLY START */
		CD_MPOLY = 25,
		CD_MLOOP = 26,
		CD_SHAPE_KEYINDEX = 27,
		CD_SHAPEKEY = 28,
		CD_BWEIGHT = 29,
		CD_CREASE = 30,
		CD_ORIGSPACE_MLOOP = 31,
		CD_PREVIEW_MLOOPCOL = 32,
		CD_BM_ELEM_PYPTR = 33,
		/* BMESH ONLY END */

		CD_PAINT_MASK = 34,
		CD_GRID_PAINT_MASK = 35,
		CD_MVERT_SKIN = 36,
		CD_FREESTYLE_EDGE = 37,
		CD_FREESTYLE_FACE = 38,
		CD_MLOOPTANGENT = 39,
		CD_TESSLOOPNORMAL = 40,
		CD_CUSTOMLOOPNORMAL = 41,
		CD_SCULPT_FACE_SETS = 42,

		/* CD_LOCATION = 43, */ /* UNUSED */
		/* CD_RADIUS = 44, */   /* UNUSED */
		CD_HAIRCURVE = 45,
		CD_HAIRMAPPING = 46,

		CD_PROP_COLOR = 47,
		CD_PROP_FLOAT3 = 48,
		CD_PROP_FLOAT2 = 49,

		CD_PROP_BOOL = 50,

		CD_NUMTYPES = 51,
	} CustomDataType;
	/* Bits for CustomDataMask */
	#define CD_MASK_MVERT (1 << CD_MVERT)
	// #define CD_MASK_MSTICKY      (1 << CD_MSTICKY)  /* DEPRECATED */
	#define CD_MASK_MDEFORMVERT (1 << CD_MDEFORMVERT)
	#define CD_MASK_MEDGE (1 << CD_MEDGE)
	#define CD_MASK_MFACE (1 << CD_MFACE)
	#define CD_MASK_MTFACE (1 << CD_MTFACE)
	#define CD_MASK_MCOL (1 << CD_MCOL)
	#define CD_MASK_ORIGINDEX (1 << CD_ORIGINDEX)
	#define CD_MASK_NORMAL (1 << CD_NORMAL)
	#define CD_MASK_FACEMAP (1 << CD_FACEMAP)
	#define CD_MASK_PROP_FLOAT (1 << CD_PROP_FLOAT)
	#define CD_MASK_PROP_INT32 (1 << CD_PROP_INT32)
	#define CD_MASK_PROP_STRING (1 << CD_PROP_STRING)
	#define CD_MASK_ORIGSPACE (1 << CD_ORIGSPACE)
	#define CD_MASK_ORCO (1 << CD_ORCO)
	// #define CD_MASK_MTEXPOLY (1 << CD_MTEXPOLY)  /* DEPRECATED */
	#define CD_MASK_MLOOPUV (1 << CD_MLOOPUV)
	#define CD_MASK_MLOOPCOL (1 << CD_MLOOPCOL)
	#define CD_MASK_TANGENT (1 << CD_TANGENT)
	#define CD_MASK_MDISPS (1 << CD_MDISPS)
	#define CD_MASK_PREVIEW_MCOL (1 << CD_PREVIEW_MCOL)
	#define CD_MASK_CLOTH_ORCO (1 << CD_CLOTH_ORCO)
	/* BMESH ONLY START */
	#define CD_MASK_MPOLY (1 << CD_MPOLY)
	#define CD_MASK_MLOOP (1 << CD_MLOOP)
	#define CD_MASK_SHAPE_KEYINDEX (1 << CD_SHAPE_KEYINDEX)
	#define CD_MASK_SHAPEKEY (1 << CD_SHAPEKEY)
	#define CD_MASK_BWEIGHT (1 << CD_BWEIGHT)
	#define CD_MASK_CREASE (1 << CD_CREASE)
	#define CD_MASK_ORIGSPACE_MLOOP (1LL << CD_ORIGSPACE_MLOOP)
	#define CD_MASK_PREVIEW_MLOOPCOL (1LL << CD_PREVIEW_MLOOPCOL)
	#define CD_MASK_BM_ELEM_PYPTR (1LL << CD_BM_ELEM_PYPTR)
	/* BMESH ONLY END */
	#define CD_MASK_PAINT_MASK (1LL << CD_PAINT_MASK)
	#define CD_MASK_GRID_PAINT_MASK (1LL << CD_GRID_PAINT_MASK)
	#define CD_MASK_MVERT_SKIN (1LL << CD_MVERT_SKIN)
	#define CD_MASK_FREESTYLE_EDGE (1LL << CD_FREESTYLE_EDGE)
	#define CD_MASK_FREESTYLE_FACE (1LL << CD_FREESTYLE_FACE)
	#define CD_MASK_MLOOPTANGENT (1LL << CD_MLOOPTANGENT)
	#define CD_MASK_TESSLOOPNORMAL (1LL << CD_TESSLOOPNORMAL)
	#define CD_MASK_CUSTOMLOOPNORMAL (1LL << CD_CUSTOMLOOPNORMAL)
	#define CD_MASK_SCULPT_FACE_SETS (1LL << CD_SCULPT_FACE_SETS)
	#define CD_MASK_PROP_COLOR (1ULL << CD_PROP_COLOR)
	#define CD_MASK_PROP_FLOAT3 (1ULL << CD_PROP_FLOAT3)
	#define CD_MASK_PROP_FLOAT2 (1ULL << CD_PROP_FLOAT2)
	#define CD_MASK_PROP_BOOL (1ULL << CD_PROP_BOOL)
	#define CD_MASK_PROP_ALL \
	  (CD_MASK_PROP_FLOAT | CD_MASK_PROP_FLOAT2 | CD_MASK_PROP_FLOAT3 | CD_MASK_PROP_INT32 | \
	   CD_MASK_PROP_COLOR | CD_MASK_PROP_STRING | CD_MASK_MLOOPCOL | CD_MASK_PROP_BOOL)

		// #define CD_MASK_RECAST (1 << CD_RECAST)  /* DEPRECATED */
	const CustomData_MeshMasks CD_MASK_MESH = {
			.vmask = (CD_MASK_MVERT | CD_MASK_MDEFORMVERT | CD_MASK_MVERT_SKIN | CD_MASK_PAINT_MASK |
					  CD_MASK_PROP_ALL | CD_MASK_PROP_COLOR),
			.emask = (CD_MASK_MEDGE | CD_MASK_FREESTYLE_EDGE | CD_MASK_PROP_ALL),
			.fmask = 0,
			.pmask = (CD_MASK_MPOLY | CD_MASK_FACEMAP | CD_MASK_FREESTYLE_FACE | CD_MASK_PROP_ALL |
					  CD_MASK_SCULPT_FACE_SETS),
			.lmask = (CD_MASK_MLOOP | CD_MASK_MDISPS | CD_MASK_MLOOPUV | CD_MASK_MLOOPCOL |
					  CD_MASK_CUSTOMLOOPNORMAL | CD_MASK_GRID_PAINT_MASK | CD_MASK_PROP_ALL),
	};
} /* namespace cg */

namespace cg::wk {
	using uint = std::uint32_t;

	class _EdgeHash_Edge {
	public:
		uint v_low, v_high;
	public:
		static std::uint32_t calc_edge_hash(_EdgeHash_Edge &edge) { return (edge.v_low << 8) ^ edge.v_high; }
	};

	class EdgeHashEntry {
	public:
		_EdgeHash_Edge	edge;
		uint			value = 0;
	};

	class EdgeHash {
	public:
		std::vector<EdgeHashEntry>	entries;
		std::vector<int32_t>		maps;
	//	uint32_t					slot_mask;		/* TODO たぶん不要。EdgeHash.size()に関連する値だけど、std::vectorで管理するから不要。 */
	//	uint						capacity_exp;	/* TODO たぶん不要。EdgeHash.size()に関連する値だけど、std::vectorで管理するから不要。 */
	//	uint						length;			/* TODO たぶん不要。EdgeHash.size()で代替可能と思う。 */
	//	uint						dummy_count;	/* TODO たぶん不要。EdgeHash.size()に関連する値だけど、std::vectorで管理するから不要。 */
	public:
		EdgeHash(size_t size): maps((1 << (calc_capacity_exp_for_reserve(size)+1)), -1) { entries.reserve(size); }
		static bool isHaskey(const EdgeHash &hash, uint v0, uint v1);
		static std::tuple<bool, wk::EdgeHashEntry> lookupEntry(const EdgeHash &hash, uint v0, uint v1);
		static uint calc_capacity_exp_for_reserve(uint reserve);
	};
}	/* namespace cg::wk */

#endif //ANDCG3DVIEWER_CG3D_H

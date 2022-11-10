//
// Created by jun on 2021/06/23.
//
#include <tuple>
#include <cmath>
#include <cassert>
#include <algorithm>
#include <sstream>
#include <iterator>
#include <functional> 
#ifdef __ANDROID__
#include <android/log.h>
#else   /* __ANDROID__ */
#endif  /* __ANDROID__ */
#include "CG3D.h"

namespace cg {
	std::tuple<bool, bool> Mesh::validateArrays(bool isCleanCustomdata) {
		return Mesh::validateArrays(Vertexs, Edges, Faces, Loops, Polygons, DeformVerts, isCleanCustomdata, true);
	}

	#pragma region /* TODO validate()実装は必要かどうかちゃんと考える。*/
	/* TODO validate()実装は必要かどうかちゃんと考える。 */
//	void Mesh::validate(bool isCleanCustomdata) {
//		bool isValid1 = Mesh::validateAllCustomdata(Vertexs, Edges, Loops, Polygons, isCleanCustomdata, isCleanCustomdata, true);
//		bool isValid2 = Mesh::validateArrays(Vertexs, Edges, Faces, Loops, Polygons, isCleanCustomdata, isCleanCustomdata, true);
//	}

//	bool Mesh::validateAllCustomdata(std::vector<Vertex> &Vertexs, std::vector<Edge> &Edges, std::vector<Loop> &Loops, std::vector<Polygon> &Polygons, bool CheckMask, bool doVerbose, bool doFixes) {
//		CustomData_MeshMasks mask = {0};
//		if (CheckMask) {
//			mask = CD_MASK_MESH;
//		}
//
//		bool isValidv, isChangeV;
//		std::tie(isValidv, isChangeV) = Mesh::validateCustomdata(Vertexs, mask.vmask, doVerbose, doFixes);
//
//		bool is_change_e, is_change_l, is_change_p;
//
//		return false;
//	}

//	std::tuple<bool, bool> Mesh::validateCustomdata(std::vector<Vertex> &Vertex, uint64_t vmask, bool doVerbose, bool doFixes) {
//		bool isValid = true;
//		bool hasFixes = false;
//		int i = 0;
//
//		__android_log_print(ANDROID_LOG_DEBUG,"aaaaa", "%s: Checking %d CD layers...", __PRETTY_FUNCTION__ , Vertex.size());
//
////		while (i < data->totlayer) {
////			CustomDataLayer *layer = &data->layers[i];
////			bool ok = true;
////
////			if (CustomData_layertype_is_singleton(layer->type)) {
////				const int layer_tot = CustomData_number_of_layers(data, layer->type);
////				if (layer_tot > 1) {
////					PRINT_ERR("\tCustomDataLayer type %d is a singleton, found %d in Mesh structure\n",
////							  layer->type,
////							  layer_tot);
////					ok = false;
////				}
////			}
////
////			if (mask != 0) {
////				CustomDataMask layer_typemask = CD_TYPE_AS_MASK(layer->type);
////				if ((layer_typemask & mask) == 0) {
////					PRINT_ERR("\tCustomDataLayer type %d which isn't in the mask\n", layer->type);
////					ok = false;
////				}
////			}
////
////			if (ok == false) {
////				if (do_fixes) {
////					CustomData_free_layer(data, layer->type, 0, i);
////					has_fixes = true;
////				}
////			}
////
////			if (ok) {
////				if (CustomData_layer_validate(layer, totitems, do_fixes)) {
////					PRINT_ERR("\tCustomDataLayer type %d has some invalid data\n", layer->type);
////					has_fixes = do_fixes;
////				}
////				i++;
////			}
////		}
////
////		PRINT_MSG("%s: Finished (is_valid=%d)\n\n", __func__, (int)!has_fixes);
////
////		*r_change = has_fixes;
////
////		return is_valid;
////		return {is_valid, has_fixes};
//		return {true,true};
//	}
	#pragma endregion

	std::tuple<bool, bool> Mesh::validateArrays(std::vector<Vertex> &Vertexs, std::vector<Edge> &Edges, std::vector<Face> &Faces, std::vector<Loop> &Loops, std::vector<Polygon> &Polygons, std::vector<DeformVertex> &DeformVertexs, bool doVerbose, bool doFixes) {
		union {
			struct {
				int verts : 1;
				int verts_weight : 1;
				int loops_edge : 1;
			};
			int as_flag;
		} fix_flag{};

		union {
			struct {
				int edges : 1;
				int faces : 1;
				/* This regroups loops and polys! */
				int polyloops : 1;
				int mselect : 1;
			};
			int as_flag;
		} free_flag{};

		union {
			struct {
				int edges : 1;
			};
			int as_flag;
		} recalc_flag{};

		fix_flag.as_flag = 0;
		free_flag.as_flag = 0;
		recalc_flag.as_flag = 0;

		/* Edges数と、Polygons数の判定 */
		if (Edges.empty() && !Polygons.empty()) {
			__android_log_print(ANDROID_LOG_ERROR, "aaaaa", "\tLogical error, %lu polygons and 0 edges", Polygons.size());
			recalc_flag.edges = doFixes;
		}

		/* Vertexsの値 検証 */
		for(size_t lpct = 0; lpct < Vertexs.size(); lpct++) {
			Vertex &vert = Vertexs[lpct];
			if(std::isfinite(vert.Co.x) == false || std::isfinite(vert.Co.y) == false || std::isfinite(vert.Co.z) == false) {
				if(doFixes) {
					vert.Co.x = vert.Co.y = vert.Co.z = 0;
					fix_flag.verts = true;
				}
			}

			if(vert.No.x != 0 || vert.No.y != 0 || vert.No.z != 0) {
				/* 法線に値がある場合は、何もしなくていい */
				continue;
			}
			else {
				if(vert.Co.x != 0 || vert.Co.y != 0 || vert.Co.z != 0) {
					/* エラーログ出力して、z軸にSHRT_MAXを設定 */
					__android_log_print(ANDROID_LOG_ERROR, "aaaaa", "\tVertex[%zu]: is zero normal, assuming Z-up normal %s(%d)", lpct, __PRETTY_FUNCTION__, __LINE__);
					if(doFixes) {
						vert.No.z = SHRT_MAX;
						fix_flag.verts = true;
					}
				}
			}
		}

		wk::EdgeHash EdgeHash(Edges.size());

		/* Edgesの値 検証 */
		for(size_t lpct = 0;  lpct < Edges.size(); lpct++) {
			Edge &e = Edges[lpct];
			bool isRemove = false;
			if(e.Vertices.x == e.Vertices.y) {
				__android_log_print(ANDROID_LOG_ERROR, "aaaaa", "\tEdge[%zu]: is matching verts. %d %s(%d)", lpct, e.Vertices.x, __PRETTY_FUNCTION__, __LINE__);
				isRemove = doFixes;
			}
			if (e.Vertices.x >= (int)Vertexs.size()) {
				__android_log_print(ANDROID_LOG_ERROR, "aaaaa", "\tEdge[%zu]: x is index out of range, %d", lpct, e.Vertices.x);
				isRemove = doFixes;
			}
			if (e.Vertices.y >= (int)Vertexs.size()) {
				__android_log_print(ANDROID_LOG_ERROR, "aaaaa", "\tEdge[%zu]: y is index out of range, %d", lpct, e.Vertices.y);
				isRemove = doFixes;
			}

			bool ishash = wk::EdgeHash::isHaskey(EdgeHash, e.Vertices.x, e.Vertices.y);
			if ((e.Vertices.x != e.Vertices.y) && ishash) {
				__android_log_print(ANDROID_LOG_ERROR, "aaaaa", "\tEdge[%zu]: is a duplicated=(%d,%d)", lpct, e.Vertices.x, e.Vertices.y);
				isRemove = doFixes;
			}

			if(isRemove) {
				/* xとyを同じ値にしてしまう。 */
				e.Vertices.y = e.Vertices.x;
				free_flag.edges = doFixes;
			}
			else {
				/* EdgeHash.entriesにentry追加 */
				assert(e.Vertices.x != e.Vertices.y);
				std::uint32_t ilow = (e.Vertices.x<e.Vertices.y)?e.Vertices.x:e.Vertices.y;
				std::uint32_t ihigh= (e.Vertices.x>e.Vertices.y)?e.Vertices.x:e.Vertices.y;
				wk::_EdgeHash_Edge edge{.v_low=ilow, .v_high=ihigh};
				std::uint32_t hash = wk::_EdgeHash_Edge::calc_edge_hash(edge);
				std::uint32_t perturb = hash;
				std::uint32_t mask = EdgeHash.maps.size()-1;
				std::uint32_t slot = mask & hash;
				int index = EdgeHash.maps[slot];
				const int PERTURB_SHIFT = 5;
				const int SLOT_EMPTY = -1;
				const int SLOT_DUMMY = -2;
				for (;; slot = mask & ((5 * slot) + 1 + perturb), perturb >>= PERTURB_SHIFT, index = EdgeHash.maps[slot]) {
					if (index == SLOT_EMPTY || index == SLOT_DUMMY) {
						wk::EdgeHashEntry entry;
						entry.edge = edge;
						entry.value = (int32_t)EdgeHash.entries.size();
						EdgeHash.maps[slot] = (int32_t)EdgeHash.entries.size();
						EdgeHash.entries.push_back(entry);
						break;
					}
				}
			}
		}

		/* Facesの値 検証 */
		if( !Faces.empty() && Polygons.empty()) {
			/* TODO Facesは将来対応 */
			assert(false && "実データにないので、動作未確認!!");
		}

		/* Polygonsの値 検証 */
		std::vector<SortPoly> sortPolygons;
		sortPolygons.reserve(Polygons.size());
		for(size_t lpi = 0; lpi < Polygons.size(); lpi++) {
			SortPoly sortPolygon;
			Polygon &poly = Polygons[lpi];

			/* index設定 */
			sortPolygon.index = lpi;

			/* Material::Indexの値 判定 */
			if(poly.MaterialIndex < 0) {
				__android_log_print(ANDROID_LOG_ERROR, "aaaaa", "\tPolygon[%zu] MaterialIndexが無効値になっている (%d)", lpi, Polygons[lpi].MaterialIndex);
				if(doFixes)
					poly.MaterialIndex = 0;
			}

			/* Material::LoopStart,LoopTotalの値 判定 */
			if (poly.LoopStart < 0 || poly.LoopTotal < 3) {
				/* Invalid loop data. */
				__android_log_print(ANDROID_LOG_ERROR, "aaaaa", "\tPolygon[%zu] に無効値 (loopstart: %d, totloop: %d) %s(%d)", lpi, poly.LoopStart, poly.LoopTotal, __PRETTY_FUNCTION__, __LINE__);
				sortPolygon.invalid = true;
			}
			else if (poly.LoopStart+poly.LoopTotal > (int)Loops.size()) {
				/* Invalid loop data. */
				__android_log_print(ANDROID_LOG_ERROR, "aaaaa",
									"\tPolygon[%zu] uses loops out of range (loopstart: %d, loopend: %d, max nbr of loops: %zu) %s(%d)", lpi, poly.LoopStart, (poly.LoopStart + poly.LoopTotal - 1), (Loops.size() - 1), __PRETTY_FUNCTION__, __LINE__);
				sortPolygon.invalid = true;
			}
			else {
				/* Poly itself is valid, for now. */
				sortPolygon.invalid = false;
				sortPolygon.verts.reserve(poly.LoopTotal);
				sortPolygon.loopstart = poly.LoopStart;

				/* Ideally we would only have to do that once on all vertices
				 * before we start checking each poly, but several polys can use same vert,
				 * so we have to ensure here all verts of current poly are cleared. */
				/* 理想的には、すべての頂点で1回だけ実行する必要があります。
				 * 各ポリゴンのチェックを開始する前に、複数のポリゴンが同じ頂点を使用できるため、
				 * ここで現在のポリゴンのすべての頂点がクリアされていることを確認する。 */
				const char ME_VERT_TMP_TAG = (1 << 2);
				for (int lpj = 0; lpj < poly.LoopTotal; lpj++) {
					Loop &ml = Loops[sortPolygon.loopstart + lpj];
					if (ml.VertexIndex < (int)Vertexs.size()) {
						Vertexs[ml.VertexIndex].flag &= ~ME_VERT_TMP_TAG;
					}
				}

				/* Test all poly's loops' vert idx. */
				for (int lpj = 0; lpj < poly.LoopTotal; lpj++) {
					Loop &ml = Loops[sortPolygon.loopstart+lpj];

					if (ml.VertexIndex >= (int)Vertexs.size()) {
						/* Invalid vert idx. */
						__android_log_print(ANDROID_LOG_ERROR, "aaaaa", "\tLoops[%d] is invalid vert reference (%d)!!", sortPolygon.loopstart+lpj, ml.VertexIndex);
						sortPolygon.invalid = true;
					}
					else if (Vertexs[ml.VertexIndex].flag & ME_VERT_TMP_TAG) {
						__android_log_print(ANDROID_LOG_ERROR, "aaaaa", "\tPolygons[%zu] is duplicated vert reference at corner (%d)", lpi, lpj);
						sortPolygon.invalid = true;
					}
					else {
						Vertexs[ml.VertexIndex].flag |= ME_VERT_TMP_TAG;
					}
					sortPolygon.verts.push_back(ml.VertexIndex);
				}

				if (sortPolygon.invalid)
					continue;

				/* Test all poly's loops. */
				for (int lpj = 0; lpj < poly.LoopTotal; lpj++) {
					Loop &ml = Loops[sortPolygon.loopstart+lpj];
					int v1 = ml.VertexIndex;													/* v1 is prev loop vert idx */
					int v2 = Loops[sortPolygon.loopstart+(lpj+1) % poly.LoopTotal].VertexIndex;	/* v2 is current loop one. */
					bool ishash = wk::EdgeHash::isHaskey(EdgeHash, v1, v2);
					if ( !ishash) {
						/* Edge not existing. */
						__android_log_print(ANDROID_LOG_ERROR, "aaaaa", "\tPolygons[%u] needs (%d, %d) form edge but not finded.", sortPolygon.index, v1, v2);
						if (doFixes) {
							recalc_flag.edges = true;
						}
						else {
							sortPolygon.invalid = true;
						}
					}
					else if (ml.EdgeIndex >= (int)Edges.size()) {
						/* Invalid edge idx.
						 * We already know from previous text that a valid edge exists, use it (if allowed)! */
						if (doFixes) {
							int prev_e = ml.EdgeIndex;
							auto ret = wk::EdgeHash::lookupEntry(EdgeHash, v1, v2);
							ml.EdgeIndex = std::get<1>(ret).value;
							fix_flag.loops_edge = true;
							__android_log_print(ANDROID_LOG_ERROR, "aaaaa", "\tLoops[%d] is invalid edge reference (%d), fixed using edge %d", sortPolygon.loopstart+lpj, prev_e, ml.EdgeIndex);
						}
						else {
							__android_log_print(ANDROID_LOG_ERROR, "aaaaa", "\tLoops[%d] is invalid edge reference (%d)", sortPolygon.loopstart+lpj, ml.EdgeIndex);
							sortPolygon.invalid = true;
						}
					}
					else {
						Edge &me = Edges[ml.EdgeIndex];
						bool isRemovedEdge = (me.Vertices.x == me.Vertices.y);
						if ( isRemovedEdge ||
							!((me.Vertices.x == v1 && me.Vertices.y == v2) || (me.Vertices.x == v2 && me.Vertices.y == v1))) {
							/* The pointed edge is invalid (tagged as removed, or vert idx mismatch),
							 * and we already know from previous test that a valid one exists,
							 * use it (if allowed)! */
							if (doFixes) {
								int prev_e = ml.EdgeIndex;
								auto ret  = wk::EdgeHash::lookupEntry(EdgeHash, v1, v2);
								ml.EdgeIndex = std::get<1>(ret).value;
								fix_flag.loops_edge = true;
								__android_log_print(ANDROID_LOG_ERROR, "aaaaa", "\tPolygons[%u] is invalid edge reference (%d, is_removed: %d), fixed using edge %d", sortPolygon.index, prev_e, isRemovedEdge, ml.EdgeIndex);
							}
							else {
								__android_log_print(ANDROID_LOG_ERROR, "aaaaa", "\tPoly[%u] has invalid edge reference (%d)", sortPolygon.index, ml.EdgeIndex);
								sortPolygon.invalid = true;
							}
						}
					}
				}

				if ( !sortPolygon.invalid) {
					/* Needed for checking polys using same verts below. */
					std::sort(sortPolygon.verts.begin(), sortPolygon.verts.end());
				}
			}

			sortPolygons.push_back(sortPolygon);
		}

		/* Second check pass, testing polys using the same verts. */
		std::sort(sortPolygons.begin(), sortPolygons.end() ,[](const SortPoly &sp1, const SortPoly &sp2){
			/* Reject all invalid polys at end of list! */
			if (sp1.invalid || sp2.invalid)
				return sp1.invalid != true;

			const int maxidx = sp1.verts.size() > sp2.verts.size() ? sp2.verts.size() : sp1.verts.size();
			for(int idx = 0; idx < maxidx; idx++) {
				if (sp1.verts[idx] != sp2.verts[idx])
					return (sp1.verts[idx] < sp2.verts[idx]);
			}

			if(sp1.verts.size() != sp2.verts.size())
				return sp1.verts.size() < sp2.verts.size();

			return sp1.index > sp2.index;
		});

		bool is_valid = true;
		for (size_t lpi = 1; lpi < Polygons.size(); lpi++) {
			SortPoly &prev_sp = sortPolygons[lpi-1];
			SortPoly &sp      = sortPolygons[lpi];
			if (sp.invalid) {
				/* Break, because all known invalid polys have been put at the end by qsort with search_poly_cmp. */
				break;
			}

			/* Test same polys. */
			if (sp.verts == prev_sp.verts) {
				if (doVerbose) {
					std::ostringstream oss;
					std::copy(sp.verts.begin(), sp.verts.end(), std::ostream_iterator<const decltype(sp.verts)::value_type&>(oss, ","));
					__android_log_print(ANDROID_LOG_ERROR, "aaaaa", "\tPolygons[%u] and Polygons[%u] use same vertices (%s), considering poly %u as invalid.", prev_sp.index, sp.index, oss.str().c_str(), sp.index);
				}
				else {
					is_valid = false;
				}
				sp.invalid = true;
			}
		}

		/* Third check pass, testing loops used by none or more than one poly. */
		std::sort(sortPolygons.begin(), sortPolygons.end(), [](const SortPoly &sp1, const SortPoly &sp2){
			/* Reject all invalid polys at end of list! */
			if (sp1.invalid || sp2.invalid)
				return (sp1.invalid == false);
			/* Else, sort on loopstart. */
			return (sp1.loopstart < sp2.loopstart);
		});

		int prevend = 0;
		std::uint32_t prevsp_idx = 0;

		const uint32_t INVALID_LOOP_EDGE_MARKER = 4294967295u;
		for (size_t lpi = 0; lpi < Polygons.size(); lpi++) {
			SortPoly &sp = sortPolygons[lpi];

			if (!sp.verts.empty()) {
//				sp.verts.clear();
			}

			/* Note above prev_sp: in following code, we make sure it is always valid poly (or NULL). */
			if (sp.invalid) {
				if (doFixes) {
					Polygons[sp.index].LoopTotal *= -1;
					free_flag.polyloops = doFixes;
					/* DO NOT REMOVE ITS LOOPS!!!
					 * As already invalid polys are at the end of the SortPoly list, the loops they
					 * were the only users have already been tagged as "to remove" during previous
					 * iterations, and we don't want to remove some loops that may be used by
					 * another valid poly! */
				}
			}
			/* Test loops users. */
			else {
				/* Unused loops. */
				if (prevend < sp.loopstart) {
					for (int lpj = prevend; lpj < sp.loopstart; lpj++) {
						Loop &ml = Loops[lpj];

						__android_log_print(ANDROID_LOG_ERROR, "aaaaa", "\tLoops[%d] is unused.", lpj);
						if (doFixes) {
							ml.EdgeIndex = INVALID_LOOP_EDGE_MARKER;
							free_flag.polyloops = doFixes;
						}
					}
					prevend = sp.loopstart + (int)sp.verts.size();
					prevsp_idx = sp.index;
				}
				/* Multi-used loops. */
				else if (prevend > sp.loopstart) {
					__android_log_print(ANDROID_LOG_ERROR, "aaaaa", "\tPolys %u and %u share loops from %d to %d, considering poly %u as invalid.",
							  prevsp_idx, sp.index, sp.loopstart, prevend, sp.index);
					if (doFixes) {
						Polygons[sp.index].LoopTotal *= -1;
						free_flag.polyloops = doFixes;
						/* DO NOT REMOVE ITS LOOPS!!!
						 * They might be used by some next, valid poly!
						 * Just not updating prev_end/prev_sp vars is enough to ensure the loops
						 * effectively no more needed will be marked as "to be removed"! */
					}
				}
				else {
					prevend = sp.loopstart + (int)sp.verts.size();
					prevsp_idx = sp.index;
				}
			}
		}
		/* We may have some remaining unused loops to get rid of! */
		if (prevend < (int)Loops.size()) {
			for (size_t lpj = prevend; lpj < Loops.size(); lpj++) {
				Loop &ml = Loops[lpj];
				__android_log_print(ANDROID_LOG_ERROR, "aaaaa", "\tLoop %zu is unused.", lpj);
				if (doFixes) {
					ml.EdgeIndex = INVALID_LOOP_EDGE_MARKER;
					free_flag.polyloops = doFixes;;
				}
			}
		}

		sortPolygons.clear();
		EdgeHash.entries.clear();
		EdgeHash.maps.clear();

		/* fix deform verts */
		if ( !DeformVertexs.empty()) {
			assert(false && "実データにないので、動作未確認!!");
			for (size_t lpi = 0; lpi < Vertexs.size(); lpi++) {
				DeformVertex &dv = DeformVertexs[lpi];
				for (size_t lpj = 0; lpj < dv.dws.size(); lpj++) {
					DeformWeight &dw = dv.dws[lpj];

					/* note, greater than max defgroups is accounted for in our code, but not < 0 */
					if (!isfinite(dw.weight)) {
						__android_log_print(ANDROID_LOG_ERROR, "aaaaa", "\tVertex deform %zu, group %u has weight: %f", lpi, dw.def_nr, dw.weight);
						if (doFixes) {
							dw.weight = 0.0f;
							fix_flag.verts_weight = true;
						}
					}
					else if (dw.weight < 0.0f || dw.weight > 1.0f) {
						__android_log_print(ANDROID_LOG_ERROR, "aaaaa", "\tVertex deform %zu, group %u has weight: %f", lpi, dw.def_nr, dw.weight);
						if (doFixes) {
							std::function<void(float, float, float)> CLAMP = [](float a, float b, float c){
								if (a < b)		{ a = b;}
								else if (a > c)	{ a = c;}
							};
							CLAMP(dw.weight, 0.0f, 1.0f);
							fix_flag.verts_weight = true;
						}
					}

					/* Not technically incorrect since this is unsigned, however,
					 * a value over INT_MAX is almost certainly caused by wrapping an uint. */
					if (dw.def_nr >= INT_MAX) {
						__android_log_print(ANDROID_LOG_ERROR, "aaaaa", "\tVertex deform %zu, has invalid group %u", lpi, dw.def_nr);
						if (doFixes) {
							dv.dws.clear();
							fix_flag.verts_weight = true;

							if (!dv.dws.empty()) {
								/* re-allocated, the new values compensate for stepping
								 * within the for loop and may not be valid */
								lpj--;
							}
							else { /* all freed */
								break;
							}
						}
					}
				}
			}
		}

		if (free_flag.faces) {
			/* Facesから不要要素削除(z成分が0のを削除) */
			auto facedelitr = std::remove_if(Faces.begin(), Faces.end(),[](const Face &face) {
									return face.vs.z == 0;
								});
			Faces.erase(facedelitr, Faces.end());
		}

		if (free_flag.polyloops) {
			/* Polygonsから不要要素削除 */
			auto polydelitr = std::remove_if(Polygons.begin(), Polygons.end(),[&Loops](const Polygon &p) {
				int stop = p.LoopStart + p.LoopTotal;
				if(stop > (int)Loops.size() || stop < p.LoopStart || p.LoopStart < 0)
					return true;
				else if(p.LoopTotal < 3)
					return true;
				else {
//定義済		const uint INVALID_LOOP_EDGE_MARKER = 4294967295u;
					auto finditr = std::find_if(Loops.begin()+p.LoopStart, Loops.begin()+p.LoopStart+p.LoopTotal, [](const Loop &loop){
											/* Loops[p.LoopStart]～Loops[p.LoopTotal]までのEdgeIndexにINVALID_LOOP_EDGE_MARKERが含まれていれば、削除対象となる */
											return loop.EdgeIndex == INVALID_LOOP_EDGE_MARKER;
										});
					if(finditr != Loops.begin()+p.LoopStart+p.LoopTotal)
						return true;
				}
				return false;
			});
			Polygons.erase(polydelitr, Polygons.end());

			/* Loopsの不要要素を消すための前準備(Polygon.LoopStartの値も変更する必要がある) */
			std::vector<int > newidxs(Loops.size());

			/* Loopsの不要要素削除 */
			int newidx = 0;
			for(int baseidx = 0; baseidx < (int)Loops.size(); baseidx++) {
				if (Loops[baseidx].EdgeIndex != INVALID_LOOP_EDGE_MARKER) {
					if (baseidx != newidx)
						Loops[newidx] = Loops[baseidx];

					newidxs[baseidx] = newidx;
					newidx++;
				}
				else {
					newidxs[baseidx] = -baseidx;
				}
			}

			/* Polygon.LoopStartの値更新 */
			for (size_t lpi = 0; lpi < Polygons.size(); lpi++)
				Polygons[lpi].LoopStart = newidxs[Polygons[lpi].LoopStart];

			/* Loopsの不要要素削除2 */
			Loops.resize(newidx);
		}

		if (free_flag.edges) {
			/* Edgesの不要要素を消すための前準備(Loops.EdgeIndexの値も変更する必要があるので、先に保持っとく) */
			std::vector<int > newidxs(Edges.size());

			/* Edgesの不要要素削除 */
			int newidx = 0;
			for (size_t baseidx = 0; baseidx < Edges.size(); baseidx++) {
				if (Edges[baseidx].Vertices.x != Edges[baseidx].Vertices.y) {
					if (baseidx != newidx)
						Edges[newidx] = Edges[baseidx];

					newidxs[baseidx] = newidx;
					newidx++;
				}
				else {
					newidxs[baseidx] = INVALID_LOOP_EDGE_MARKER;
				}
			}

			/* Loops.EdgeIndexの値を更新 */
			for (size_t lpi = 0; lpi < Edges.size(); lpi++)
				Loops[lpi].EdgeIndex = newidxs[Loops[lpi].EdgeIndex];
		}

		if (recalc_flag.edges) {
			assert(false && "実データにないので、動作未確認!!要実装!!");
		}

		bool changed = (fix_flag.as_flag || free_flag.as_flag || recalc_flag.as_flag);

		return { is_valid, changed };
	}

	bool wk::EdgeHash::isHaskey(const wk::EdgeHash &hash, uint v0, uint v1) {
		auto [retbool, edgehashentry] = lookupEntry(hash, v0, v1);
		return retbool;
	}

	std::tuple<bool, wk::EdgeHashEntry> wk::EdgeHash::lookupEntry(const wk::EdgeHash &edgehash, uint v0, uint v1) {
		std::uint32_t ilow = (v0 < v1) ? v0 : v1;
		std::uint32_t ihigh= (v0 > v1) ? v0 : v1;
		wk::_EdgeHash_Edge edge{ .v_low = ilow, .v_high = ihigh };
		std::uint32_t hash = wk::_EdgeHash_Edge::calc_edge_hash(edge);
		std::uint32_t perturb = hash;
		std::uint32_t mask = edgehash.maps.size()-1;
		std::uint32_t slot = mask & hash;
		int index = edgehash.maps[slot];
		const int PERTURB_SHIFT = 5;
		const int SLOT_EMPTY = -1;
		for (;; slot = mask & ((5 * slot) + 1 + perturb), perturb>>=PERTURB_SHIFT, index = edgehash.maps[slot]) {
			if (index>=0 &&
				(edge.v_low == edgehash.entries[index].edge.v_low &&
				 edge.v_high== edgehash.entries[index].edge.v_high)) {
				return {true,  edgehash.entries[index]};
			}
			if (index == SLOT_EMPTY) {
				return {false, {}};
			}
		}
		return { false, {} };
	}

	std::uint32_t wk::EdgeHash::calc_capacity_exp_for_reserve(std::uint32_t reserve) {
		std::uint32_t result = 1;
		while (reserve >>= 1) {
			result++;
		}
		return result;
	}
} /* namespace cg */
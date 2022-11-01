#include <string>
#include <vector>
#include "GlObj.h"
#include "ShaderObj.h"

bool ShaderObj::LoadShaders(const std::string &vshstrdata, const std::string &fshstrdata) {
    auto [retbool, programid] = GlObj::LoadShaders(vshstrdata, fshstrdata);
    if( !retbool) {
        mProgramId = -1;
    }

    return retbool;
}

bool ShaderObj::setAttribute(GLuint programId, int totalframes, int numPolys, int numVertexsperframe,
                        const std::vector<vertex> &vertexs, const std::vector<mesh> &polyIndexs,
                        const std::vector<texstcoord> &sts) {
    /* 現在頂点,次頂点,UV座標で詰替え */
    std::vector<float> md2Vertices = {0};

    for(int frameidx = 0; frameidx < totalframes; frameidx++) {
        /* 現在フレームと次フレームを取得 */
        const vertex *currentFrame= &vertexs[numVertexsperframe * frameidx];
        const vertex *nextFrame   = (frameidx+1 >= totalframes) ? &vertexs[0] : &vertexs[numVertexsperframe * (frameidx+1)];

        for(int plyidx = 0; plyidx < numPolys; plyidx++) {
            for(int meshdx = 0; meshdx < 3; meshdx++) {
                /* vertices */
                for (size_t vidx = 0; vidx < 3; vidx++) {
                    md2Vertices.emplace_back(currentFrame[polyIndexs[plyidx].meshIndex[meshdx]].v[vidx]);
                }

                /* next frame */
                for (size_t vidx = 0; vidx < 3; vidx++) {
                    // vertices
                    md2Vertices.emplace_back(nextFrame[polyIndexs[plyidx].meshIndex[meshdx]].v[vidx]);
                }

                /* tex coords */
                md2Vertices.emplace_back(sts[polyIndexs[plyidx].stIndex[meshdx]].s);
                md2Vertices.emplace_back(sts[polyIndexs[plyidx].stIndex[meshdx]].t);
            }
        }


    }

    mCurPosAttrib  = glGetAttribLocation( mProgramId, "pos");
    mNextPosAttrib = glGetAttribLocation( mProgramId, "nextPos");
    mTexCoordAttrib= glGetAttribLocation( mProgramId, "texCoord");

    return false;
}

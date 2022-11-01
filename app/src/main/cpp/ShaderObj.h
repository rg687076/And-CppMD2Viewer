#ifndef CPPMD2VIEWER_SHADEROBJ_H
#define CPPMD2VIEWER_SHADEROBJ_H

#include "Md2Parts.h"

class ShaderObj {
public:
    bool LoadShaders(const std::string &vshstrdata, const std::string &fshstrdata);
    bool setAttribute(GLuint programId, int totalframes, int numPolys, int numVertexsperframe,
                      const std::vector<vertex> &vertexs, const std::vector<mesh> &polyIndexs,
                      const std::vector<texstcoord> &sts);

public:
    GLuint mProgramId     = -1;
    GLuint mCurPosAttrib  = -1;
    GLuint mNextPosAttrib = -1;
    GLuint mTexCoordAttrib= -1;
};

#endif //CPPMD2VIEWER_SHADEROBJ_H

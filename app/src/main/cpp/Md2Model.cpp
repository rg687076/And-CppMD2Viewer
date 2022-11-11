#include <vector>
#include <android/log.h>
#include "Md2Model.h"

Md2Model::~Md2Model() {
}

/* binデータからMd2データを読込む */
bool Md2Model::loadModel(std::vector<char> &md2bindata) {
    /* MD2ヘッダ */
    md2header *header = (md2header*)md2bindata.data();

    /* MD2形式チェック */
    if(header->magicnumber != MD2_IDENT) { /* "IDP2"じゃないとエラー */
        const union { int i; char b[4]; } ngno = {header->magicnumber};
        __android_log_print(ANDROID_LOG_INFO, "aaaaa", "MD2フォーマット不正(magicnumber=%s) %s %s(%d)", ngno.b, __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
        return false;
    }


    return true;
}

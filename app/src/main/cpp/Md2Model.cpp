#include <android/log.h>
#include "Md2Model.h"

/* Md2モデルデータ実体 */
std::map<std::string, Md2Model> gMd2models;

/* Md2モデルsetup */
void Md2Setup(std::map<std::string, Md2Model> &&md2models) {
    __android_log_print(ANDROID_LOG_INFO, "aaaaa", "%s %s(%d)", __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);

    return;
}

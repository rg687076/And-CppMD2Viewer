#include "AssetsData.h"

AssetsData &AssetsData::GetIns() {
    static AssetsData instance;
    return instance;
}

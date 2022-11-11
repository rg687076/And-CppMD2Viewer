//
// Created by jun on 2021/06/09.
//
#ifndef ANDCG3DVIEWER_APPDATA_H
#define ANDCG3DVIEWER_APPDATA_H
#include <map>
#include <string>
#include <vector>

class AppData {
public:
    std::map<std::string, std::vector<char>> mAssets;
    static AppData &GetIns() {
        static AppData instance;
        return instance;
    }
private:
    AppData() {}
    AppData(const AppData &) {}
    AppData &operator=(const AppData&){ return *this; }
};

#endif //ANDCG3DVIEWER_APPDATA_H

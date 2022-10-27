#ifndef CPPMD2VIEWER_MD2MODEL_H
#define CPPMD2VIEWER_MD2MODEL_H

#include <map>
#include <string>
#include <vector>

class Md2Model {
public:
    std::string         name;
    std::string         verfilename;
    std::string         texfilename;
    std::vector<char>   verbindata;
    std::vector<char>   texbindata;
    void	loadModel(std::string Md2file, std::string Tgafile, float _size, float _fps );
};

/* Md2モデルs */
extern std::map<std::string, Md2Model> gMd2models;

/* プロトタイプ宣言 */
extern void Md2Setup(std::map<std::string, Md2Model> &&map);

#endif //CPPMD2VIEWER_MD2MODEL_H

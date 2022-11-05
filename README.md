# And-CppMD2Viewer
MD2形式の3DCGファイルのビューア(Android,C++版)

# 制限事項
- TGAとBMPのみ
- BMPは、ビットサイズ=24, 圧縮タイプ=非圧縮しか対応しない。
- 補完係数の計算は、ズルしてる。(ただのインクリメントで対応) Md2Obj.cpp(196)らへんのコメント参照
- テクスチャは1枚しか対応してない。複数枚に対応するにはMd2ModelInfo::GLuint mTexIdをstd::vetorにして複数保持する様にすると幸せになれるかと。実装自体は、And-CppMqoViewerが参考になる。(理解するには気合が必要だけど...)


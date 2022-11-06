# And-CppMD2Viewer
MD2形式の3DCGファイルのビューア(Android,C++版)

# 制限事項
- TGAとBMPのみ
- 画像フォーマット(JPG/PNG/GIF/PSD/PIC/PNM/HDR)は、将来対応。個別対応なら、下記サイトが参考になる。
  https://github.com/raydelto/opengles-md2loader-android.git
- 補完係数の計算は、ズルしてる。(ただのインクリメントで対応) Md2Obj.cpp(196)らへんのコメント参照
- テクスチャは1枚しか対応してない。複数枚に対応するにはMd2ModelInfo::GLuint mTexIdをstd::vetorにして複数保持する様にすると幸せになれるかと。実装自体は、And-CppMqoViewerが参考になる。(理解するには気合が必要だけど...)


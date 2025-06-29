# textunpack

`textunpack` は自作のアーカイブフォーマット用のユーティリティです。[[[code(textunpack)]]] を使うと、複数のテキストファイルを一つのテキストファイルで管理できます。

```
$ git clone https://github.com/inzkyk/textunpack.git
$ cd textunpack
$ cat sample.txt
#+EXPORT_BEGIN foo.txt
This is foo.

#+EXPORT_END foo.txt

#+EXPORT_BEGIN bar.txt
This is bar.

#+EXPORT_END bar.txt
$ clang++ amalgam.cpp -o textunpack -std=c++17 -O3
$ ./textunpack sample.txt
$ cat foo.txt
This is foo.
$ cat bar.txt
This is bar.
```

詳しい解説は [textunpack: テキストベースのアーカイブユーティリティ](https://inzkyk.xyz/software/textunpack/) に書きました。

`textunpack` は [https://inzkyk.xyz/](https://inzkyk.xyz/) 用に作られたプログラムであり、次の書籍の翻訳で利用されました:

- [JavaScript: 最初の 20 年]((https://inzkyk.xyz/js_20_years/))
- [グラフ理論入門](https://inzkyk.xyz/graph/)
- [オープンソースソフトウェアのパフォーマンス](https://inzkyk.xyz/posa/)
- [コンピューターネットワーク: システム的アプローチ](https://inzkyk.xyz/network/)
- [500 行のコードで作るプチアプリケーション](https://inzkyk.xyz/500-lines/)


## 使い方

実行形式は GitHub のリリースで公開されています。基本的な使い方は次の通りです:

- `textunpack FILE`: `FILE` を展開する。
- `textunpack --stdin DIR`: `stdin` から読み込んだデータを展開する。`DIR` が相対パスの起点として使われる。
- `textunpack FILE -l`: `FILE` に含まれるエントリの名前を出力する。

## ビルド

全てのソースコードを一つにまとめた `amalgam.cpp` を用意してあるので、次のコマンドでビルドできます:

```bash
clang++ .\amalgam.cpp -o textunpack.exe -O3 -std=c++17
```

次のコマンドでもいいです:

```bash
clang++ .\amalgam.cpp -o textunpack.exe -O3 -std=c++17 -fno-exceptions -fno-rtti -march=native
```

`textunpack` の実行を詳しく追いたい場合は、通常のソースツリーを CMake でビルドしてテストの実行をデバッガで追うといいでしょう。次のコマンドでビルドできます:

```bash
cmake -S . -B build -G Ninja
cmake --build build
./build/textunpack --test
```

ソースツリー内のソースコードを変更した後に `amalgam.cpp` を生成するには、次のコマンドを実行してください:

```bash
python amalgamator.py
```

なお、ソースコードを大きく変更すると `amalgamator.py` の手に負えなくなって生成される `amalgam.cpp` がコンパイルできなくなる可能性があります。

## ソースコードについて

このレポジトリで公開されるソースコードはプライベートのコードベースから切り出したものです。そのため、使用されていないコードや不自然なコードが含まれます。

## 開発について

`textunpack` の開発は終了しています。今後アップデートやバグフィックスの予定はありません。

## ライセンス

`textunpack` は MIT ライセンスで公開されます。

```text
MIT License

Copyright (c) 2025 inzkyk

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

`textunpack` が利用する外部ライブラリは次の通りです:

| ライブラリ名    | URL                                    | ライセンス                              | ライセンスファイルなど                                                                      |
|-----------------|----------------------------------------|-----------------------------------------|---------------------------------------------------------------------------------------------|
| bx              | https://github.com/bkaradzic/bx        | BSD 2-Clause                            | [LICENSE.txt](./external/bx/LICENSE.txt)                                                    |
| Microsoft's STL | https://github.com/microsoft/STL       | Apache License v2.0 with LLVM Exception | [LICENSE.txt](./external/Microsoft_STL/LICENSE.txt), [NOTICE.txt](Microsoft_STL/NOTICE.txt) |
| pdqsort         | https://github.com/orlp/pdqsort        | Zlib                                    | [LICENSE.txt](./external/pdqsort/LICENSE.txt)                                               |
| sokol_time.h    | https://github.com/floooh/sokol        | Zlib                                    | [LICENSE.txt](./external/sokol_time/LICENSE.txt)                                            |
| wyhash          | https://github.com/wangyi-fudan/wyhash | The Unlicense                           | [LICENSE.txt](./external/wyhash/LICENSE.txt)                                                |

また、[cpprefjp](https://cpprefjp.github.io/index.html) にて [クリエイティブ・コモンズ 表示 3.0 非移植 ライセンス](https://creativecommons.org/licenses/by/3.0/) で公開されている情報を一部利用しています。

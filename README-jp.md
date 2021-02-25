# addlicense

c/c++に似たソースコードのライセンス表記を調べたり書き換えたりします。

## usage

```shell
addlicense LICENSE.TXT directory [options...]
```

一つ目の引数は追加したいライセンス表記を書いたテキストファイルを指定します。
２つめの引数はソースコードを検索するディレクトリを指定します。

## --filter

ファイルのマッチ条件です。複数ある場合は ; で繋げます。

Default ```*.hpp;*.h;*.cpp;*.c;*.java;*.m;*.mm```

```shell
addlicense LICENSE.TXT directory --filter "*.hpp;*.h;*.cpp;*.c;*.java"
```

## --test

実際の書き込みは行わずテストのみを行います。

## --listup-unmatch

--filter と一致しなかったファイルのみをリストアップします。書き込みは行いません。

## --without-directory

除外するディレクトリを指定します。

```shell
addlicense LICENSE.txt testdir --without "*.hg;*.git;*.svn" --print-license
```

## --print-modified

変換した時、変換したファイル名を表示します。

## --print-unmodified

変換しなかった時、変換しなかったファイル名を表示します。

## --print-license

ファイルにライセンスが記載されていた場合、ファイル名とライセンス表記の位置とサイズ、ライセンス表記部分を表示します。ライセンス表記を変更した場合は、変更前の表記になります。


```shell
addlicense LICENSE.txt testdir --filter "*.cpp" --print-license
test.cpp : (0,220)
/* Copyright (c) 2019, isaponsoft (Isao Shibuya) All rights reserved. *
 * Use of this source code is governed by a BSD-style  license that   *
 * can be found in the LICENSE file.                                  */
```

#### test.cpp

```c++
/* Copyright (c) 2019, isaponsoft (Isao Shibuya) All rights reserved. *
 * Use of this source code is governed by a BSD-style  license that   *
 * can be found in the LICENSE file.                                  */
#include <stdio.h>

int main();
```

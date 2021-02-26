# addlicense

c/c++に似たソースコードのライセンス表記を調べたり書き換えたりします。

## usage

```shell
addlicense [options...] target-files or target-directories
```

一つ目の引数は追加したいライセンス表記を書いたテキストファイルを指定します。
２つめの引数はソースコードを検索するディレクトリを指定します。

## --filter

コマンドの引数でディレクトリを指定した場合に変換対象にするファイルのマッチ条件です。複数ある場合は ; で繋げます。ファイルを直接指定した場合は無視されます。

Default ```*.hpp;*.h;*.cpp;*.c;*.java;*.m;*.mm```

```shell
addlicense --filter "*.hpp;*.h;*.cpp;*.c;*.java" targets...
```

## --test

実際の書き込みは行わずテストのみを行います。

## --listup-unmatch

--filter と一致しなかったファイルのみをリストアップします。書き込みは行いません。

## --without-directory

コマンドの引数でディレクトリを指定した場合に除外するディレクトリを指定します。ファイルを直接指定した場合は無視されます。

```shell
addlicense --without "*.hg;*.git;*.svn" target-directories...
```

## --print-license

ファイルにライセンスが記載されていた場合、ファイル名とライセンス表記の位置とサイズ、ライセンス表記部分を表示します。ライセンス表記を変更した場合は、変更前の表記になります。

#### test.cpp

```c++
/* Copyright (c) 2019, isaponsoft (Isao Shibuya) All rights reserved. *
 * Use of this source code is governed by a BSD-style  license that   *
 * can be found in the LICENSE file.                                  */
#include <stdio.h>

int main();
```

### LICENSE.txt

```shell
/* Public domain. */
```

```shell
% addlicense --license LICENSE.txt --print-license test.cpp
test.cpp : (0,220)
/* Copyright (c) 2019, isaponsoft (Isao Shibuya) All rights reserved. *
 * Use of this source code is governed by a BSD-style  license that   *
 * can be found in the LICENSE file.                                  */
% cat test.cpp
/* Public domain. */
#include <stdio.h>

int main();
```



## --bom

ファイルの先頭に UTF-8 BOM をつけるか削除します。```--bom add``` だと BOMを追加し、```--bom remove```だとBOMを削除します。

```shell
addlicense --bom add tatgets...
```

## --return

改行コードを統一します。```--return n```, ```--return rn``` のように指定します。

```shell
# Unix
addlicense --return n targets...
```

```shell
# Windows
addlicense --return rn targets...
```

```shell
# mac
addlicense --return r targets...
```

# addlicense

Check and rewrite license notations for c/c++-like source code.

[English manual](README-en.md)


c/c++に似たソースコードのライセンス表記を調べたり書き換えたりします。

[日本語マニュアル](README-jp.md)


```shell
addlicense LICENSE.TXT project-dir \
  --filter "*.cpp;*.hpp;*.h;*.hpp;*.java" \
  --print-license
project-dir/main.cpp : (0,220)
/* Copyright (c) 2019, isaponsoft (Isao Shibuya) All rights reserved. *
 * Use of this source code is governed by a BSD-style  license that   *
 * can be found in the LICENSE file.                                  */
```

```c++
/* Copyright (c) 2019, isaponsoft (Isao Shibuya) All rights reserved. *
 * Use of this source code is governed by a BSD-style  license that   *
 * can be found in the LICENSE file.                                  */
#include <stdio.h>

int main();
```

## Platform

FreeBSD, Linux, Windows, macos

## License (2-clause BSD License)

```
Copyright (c) 2019, isaponsoft (Isao Shibuya) All rights reserved.
Use of this source code is governed by a BSD-style  license that 
can be found in the LICENSE file.
```

## Build - Windows

```shell
git clone git://
cd addlicense
build.bat
Release\addlicense.exe
```

## Build - OtherOS

```shell
git clone git://
cd addlicense
build.sh
./addlicense
```

## Depenency

* libamtrs [github](https://github.com/isaponsoft/libamtrs)
* cmake [cmake.org](https://cmake.org/)

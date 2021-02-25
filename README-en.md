# addlicense

Check and rewrite license notations for c/c++-like source code.

## usage

```shell
addlicense LICENSE.TXT directory [options...]
```

The first argument specifies the text file that describes the license notation you want to add.
The second argument specifies the directory to search for source code.

## --filter

File match conditions. If there are more than one, connect them with ';'.

Default ```*.hpp;*.h;*.cpp;*.c;*.java;*.m;*.mm```

```shell
addlicense LICENSE.TXT directory --filter "*.hpp;*.h;*.cpp;*.c;*.java"
```

## --test

Only test, not actual writing.

## --listup-unmatch

List only files that do not match --filter. It does not write.

## --print-modified

When converted, the converted file name is displayed.

## --print-unmodified

When not converted, the file name that was not converted is displayed.

## --without-directory

Without directory filter;

```shell
addlicense LICENSE.txt testdir --without "*.hg;*.git;*.svn" --print-license
```


## --print-license

If the license is described in the file, the file name, the position and size of the license notation, and the license notation part are displayed. If you change the license notation, it will be the same as before the change.

```shell
addlicense LICENSE.txt testdir --filter "*.cpp" --print-license
test.cpp : (0,220)
/* Copyright (c) 2019, isaponsoft (Isao Shibuya) All rights reserved. *
 * Use of this source code is governed by a BSD-style  license that   *
 * can be found in the LICENSE file.                                  */
```

### test.cpp

```c++
/* Copyright (c) 2019, isaponsoft (Isao Shibuya) All rights reserved. *
 * Use of this source code is governed by a BSD-style  license that   *
 * can be found in the LICENSE file.                                  */
#include <stdio.h>

int main();
```

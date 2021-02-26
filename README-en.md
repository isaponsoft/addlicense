# addlicense

Check and rewrite license notations for c/c++-like source code.

## usage

```shell
addlicense [options...] target-files or target-directories
```

## --filter

This is the match condition for files to be converted when a directory is specified in the command argument. If there are multiple files, join them with ;. If you specify a file directly, it will be ignored.

Default ```*.hpp;*.h;*.cpp;*.c;*.java;*.m;*.mm```

```shell
addlicense --filter "*.hpp;*.h;*.cpp;*.c;*.java" targets...
```

## --test

Only test, not actual writing.

## --listup-unmatch

List only files that do not match --filter. It does not write.


## --without-directory

Specifies the directory to be excluded when a directory is specified as the command argument. If you specify a file directly, it will be ignored.

```shell
addlicense LICENSE.txt testdir --without "*.hg;*.git;*.svn" --print-license
```


## --print-license

If the license is described in the file, the file name, the position and size of the license notation, and the license notation part are displayed. If you change the license notation, it will be the same as before the change.

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

Adds or removes a UTF-8 BOM at the beginning of a file. ```--bom add``` adds a BOM, ```--bom remove``` removes a BOM.

```shell
addlicense --bom add tatgets...
```

## --return

Unify line feed codes. ```--return n```, ```--return rn```.

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

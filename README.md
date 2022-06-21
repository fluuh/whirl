<h1 align="center">Whirl</h1>
<div align="center">
	<a href="https://mozilla.org/MPL/2.0/">
		<img src = "https://img.shields.io/badge/License-MPL--2.0-red"/>
	</a>
	<a href="https://github.com/fluuh/whirl/graphs/commit-activity">
		<img src = "https://img.shields.io/github/commit-activity/m/fluuh/whirl" \>
	</a>
</div>
<div>&nbsp;</div>

## Introduction

Whirl is a powerful LISP programming language.

```
(define (makeHello from)
		(join "Hello from " from "!"))
(print (makeHello "Whirl"))
; loop over a list
(define list [1 2 3])
(each (i v) list
	(print (join i ": " v)))
```

## How to build

To build with CMake:
```
$ mkdir build
$ cmake --build build
```

## How to use

```
$ ./build/wrl help
Usage:
    ./build/wrl <command> [options..]
Commands:
    init        initialize an empty project
    build       build project
    help        show usage information
    version     show compiler version
Options:
    -p <dir>    project directory
```
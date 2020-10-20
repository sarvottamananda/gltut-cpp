# GLTut  - Simple Code Bits for OpenGL/Linux/C++

[![PRs welcome](https://img.shields.io/badge/PRs-welcome-brightgreen.svg?style=flat)](http://makeapullrequest.com)
[![First timers only friendly](https://img.shields.io/badge/first--timers--only-friendly-blue.svg?style=flat)](https://github.com/sarvottamananda/)
[![Open Source Love](https://firstcontributions.github.io/open-source-badges/badges/open-source-v1/open-source.svg)](https://github.com/firstcontributions/open-source-badges)
[![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)](https://opensource.org/licenses/MIT)
[![Build Status](https://travis-ci.com/sarvottamananda/gltut-cpp.svg?branch=master)](https://travis-ci.com/sarvottamananda/gltut-cpp)

***

This project contains beginner level OpenGL code snippets. The purpose is to learn OpenGL through
practical programming. 

The code snippets use OpenGL 4.0+

## Compiling snippets in Linux

To compile all the code snippets in the directory run the following

```
$ meson build
$ meson compile -C build
```

The executables are created as ```${projectdir}/build/src/*/*```
Run individual execuatable for each snippet.

## Running all snippets

The destination for compiled programs is mirrored structure in build directory.
You can just change directory to the required location and run the compiled programs.

## Testing all targets

To run all the snippets from the directory do the following too.

```
$ meson test -C build
```



## Examples

#### Tutorial 0 - OpenGL Sap Green window

```
$ ./build/src/cs0/cs0-window 
```

![Tutorial 0](https://raw.githubusercontent.com/wiki/sarvottamananda/glbits-cpp/imgs/cs0-window.png)



#### Tutorial 1 - Simple OpenGL Triangles

```
$ ./build/src/cs1/cs1-triangles
```

![Tutorial 1](https://raw.githubusercontent.com/wiki/sarvottamananda/glbits-cpp/imgs/cs1-triangles.png)


#### Tutorial 2 - Moving Triangles

```
$ ./build/src/cs2/cs2-moving
```

![Tutorial 2](https://raw.githubusercontent.com/wiki/sarvottamananda/glbits-cpp/imgs/cs2-moving.png)

#### Tutorial 3 - Skybox

```
$ ./build/src/cs3/cs3-skybox
```

![Tutorial 3](https://raw.githubusercontent.com/wiki/sarvottamananda/glbits-cpp/imgs/cs3-skybox.png)

Contol the camera and the cube with following keys

0, 1, ..., 9 : change cubemap

w, s : move camera to / away from cube

a, d : rotate cube clockwise / counter-clockwise

left, right, up, down : move camera

space : Reset camera and cube

Options : 

-    **--fullscreen** for fullscreen
-    **--windowed** for window
-    **--height** _h_ for window height _h_
-    **--width** _w_ for window width _w_

#### Tutorial 4 - Terrain

```
$ ./build/src/cs4/cs4-ground
```

![Tutorial 1](https://raw.githubusercontent.com/wiki/sarvottamananda/glbits-cpp/imgs/cs4-ground.png)

Same options and controls as before.

***


<details>
<summary>
<a class="btnfire small stroke"><em class="fas fa-chevron-circle-down"></em>&nbsp;&nbsp;Frequently Asked Questions</a>    
</summary>

<ul>
<li>

[CODE_OF_CONDUCT](CONDUCT.md)
</li>


</ul>
</details>




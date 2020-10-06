# OpenGL Snippets - Simple Code Snippets for OpenGL/C++ in Linux for Students

<p align="center">
	<img src="https://img.shields.io/badge/PRs-welcome-brightgreen.svg?style=flat">
	<img src="https://img.shields.io/badge/first--timers--only-friendly-blue.svg">
	<img src="https://travis-ci.org/freeCodeCamp/how-to-contribute-to-open-source.svg?branch=master">
</p>

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
$ ./build/src/cs0-window/cs0-window 
```

![Tutorial 0](https://raw.githubusercontent.com/wiki/sarvottamananda/opengl-snippets/imgs/cs0-window.png)



#### Tutorial 1 - Simple OpenGL Triangles

```
$ ./build/src/cs1-triangles/cs1-triangles
```

![Tutorial 1](https://raw.githubusercontent.com/wiki/sarvottamananda/opengl-snippets/imgs/cs1-triangles.png)


#### Tutorial 2 - Moving Triangles

```
$ ./build/src/cs2-moving/cs2-moving
```

![Tutorial 2](https://raw.githubusercontent.com/wiki/sarvottamananda/opengl-snippets/imgs/cs2-moving.png)

#### Tutorial 3 - Skybox

```
$ ./build/src/cs3-skybox/cs3-skybox
```

![Tutorial 3](https://raw.githubusercontent.com/wiki/sarvottamananda/opengl-snippets/imgs/cs3-skybox.png)

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
$ ./build/src/cs4-ground/cs4-ground
```

![Tutorial 1](https://raw.githubusercontent.com/wiki/sarvottamananda/opengl-snippets/imgs/cs4-ground.png)

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




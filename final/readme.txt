Readme for improved GLM .obj file format loader.
------------------------------------------------

General
-------
This file includes a portion of my library, libeden, for loading Wavefront .obj format files. The code is based on Nate Robins glm library sample code.

Usage
-----
All the required usage to load and manipulate .obj models should be able to be determined by reading the comments in the glm.h file. It should not be necessary to examine the other files unless you wish to modify or extend the code, or understand how it works.

The most basic level of usage to load and draw a model would proceed thus:
	#include "eden/glm.h"
	GLMmodel* myModel;
	myModel = glmReadOBJ("mymodelname.obj", 0);
	glmDraw(myModel, GLM_NONE, 0);

Changes from original glm code
------------------------------
My changes to Nate's code include support for .obj files with textures defined in the accompanying .mtl file. At present, only support for .sgi format textures is defined. Here is an example of a material definition file which includes a texture map:
	newmtl metal_white
	Ka 0.20725 0.20725 0.20725
	Kd 1.0 1.0 1.0
	Ks 0.296648 0.296648 0.296648
	Ns 88.0
	map_Kd my_white_metal_texture.sgi
Support for loading these textures is provided by the included files edenSurfaces.c and readtex.c. If you wish to build the glm library without this texture support, comment out the line
	#define GLM_MATERIAL_TEXTURES
where it occurs in glm.h.

Building
--------
The files which need to be compiled are
	glm.c
	readtex.c
	edenSurfaces.c
and ou should include this with the project in which you wish to load .obj files. You will also need to place the header files:
	glm.h
	readtex.h
	eden.h
	edenSurfaces.h
into a folder or directory named "eden" and place the eden folder on the include path for your project.

Further documentation
---------------------
At present there is no further documentation for the code, although interested parties are referred to Nate Robins' original gl tutors files which include sample usage. At the time of writing, these could be found at http://www.xmission.com/~nate/tutors.html

Feel free to contact me with comments, improvements and bugs related to my version.

Philip Lamb
phil@eden.net.nz
2004-01-19
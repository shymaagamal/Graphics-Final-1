/* readtex.h */
/* 
 * Read an SGI .rgb image file and generate a mipmap texture set.
 */

#ifndef READTEX_H
#define READTEX_H

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

/*
* Load an SGI .rgb file and generate a set of 2-D mipmaps from it.
 * Input:  imageFile - name of .rgb to read
 *         intFormat - internal texture format to use, or number of components
 *         makeMipmaps - if set to TRUE, build a set of mipmaps from the texture
 * Return:  GL_TRUE if success, GL_FALSE if error.
 */
extern GLboolean ReadTex(const char *imageFile, const GLint intFormat, const GLboolean makeMipmaps);

#endif	/* !READTEX_H */

/*
      glm.h
      Nate Robins, 1997, 2000
      nate@pobox.com, http://www.pobox.com/~nate

      Wavefront OBJ model file format reader/writer/manipulator.

      Includes routines for generating smooth normals with
      preservation of edges, welding redundant vertices & texture
      coordinate generation (spheremap and planar projections) + more.
*/

//
//	Updated by Philip Lamb (PRL) phil@eden.net.nz. All rights reserved.
//
//	Rev		Date		Who		Changes
//  1.0.0   ????????    NR      Original version released as part of Nate Robin's OpenGL tutors package.
//	1.0.1	20011114	PRL		Fix linear texture coordinate generation.
//	1.0.2	20011115	PRL		Correct glmUnitize & glmDimensions for model with no -ve vertices,
//								and correct glmUnitize to use unit cube, not cube with sides=2.0.
//	1.1.0	20020128	PRL		Add glmDrawArrays to use compiled vertex arrays.
//	1.2.0	20020130	PRL		glmDraw, glmDrawArrays and glmList are now OpenGL context-specific.
//	1.2.1	20020203	PRL		New glmVertexNormals() routine to handle models with polys with > 3 sides more
//                              consistantly.
//	1.3.0	20030806	PRL		Now handles materials with dissolve factors for transparency and texture maps
//                              for diffuse colour via changes to glmReadMTL/glmWriteMTL. glmReadOBJ & glmDelete
//                              are now OpenGL context-specific.
//
#include <iostream>
#include <string>
#ifndef __glm_h__
#define __glm_h__

#define GLM_MATERIAL_TEXTURES	// PRL 20030806: Uncomment to build glm with provision for texture mapping in materials.

#ifdef __APPLE__
#  include <GLUT/glut.h>
#elif defined(GLUTLOCAL)
#  include "glut.h"
#else
#  include <GL/glut.h>
#endif

#ifdef GLM_MATERIAL_TEXTURES
#include"edenSurfaces.h"		// PRL 20030806: TEXTURE_INFO_t, TEXTURE_INDEX_t, SurfacesTextureLoad(), SurfacesTextureSet(), SurfacesTextureUnload()
#endif // GLM_MATERIAL_TEXTURES

#include <math.h>

#ifndef M_PI
#define M_PI 3.141592653589793238462643
#endif

#define GLM_NONE     (0)            /* render with only vertices */
#define GLM_FLAT     (1 << 0)       /* render with facet normals */
#define GLM_SMOOTH   (1 << 1)       /* render with vertex normals */
#define GLM_TEXTURE  (1 << 2)       /* render with texture coords */
#define GLM_COLOR    (1 << 3)       /* render with colors */
#define GLM_MATERIAL (1 << 4)       /* render with materials */

/* GLMmaterial: Structure that defines a material in a model.
 */
typedef struct _GLMmaterial
{
  char* name;                   /* name of material */
  GLfloat diffuse[4];           /* diffuse component */
  GLfloat ambient[4];           /* ambient component */
  GLfloat specular[4];          /* specular component */
  GLfloat emmissive[4];         /* emmissive component */
  GLfloat shininess;            /* specular exponent */
#ifdef GLM_MATERIAL_TEXTURES
  char* texturemap;				// PRL 20030806: name of texture.
  TEXTURE_INDEX_t texturemap_index; // PRL 20030806: index to loaded texture.
#endif // GLM_MATERIAL_TEXTURES
} GLMmaterial;

/* GLMtriangle: Structure that defines a triangle in a model.
 */
typedef struct _GLMtriangle {
  GLuint vindices[3];           /* array of triangle vertex indices */
  GLuint nindices[3];           /* array of triangle normal indices */
  GLuint tindices[3];           /* array of triangle texcoord indices*/
  GLuint findex;                /* index of triangle facet normal */
} GLMtriangle;

/* GLMgroup: Structure that defines a group in a model.
 */
typedef struct _GLMgroup {
  char*             name;           /* name of this group */
  GLuint            numtriangles;   /* number of triangles in this group */
  GLuint*           triangles;      /* array of triangle _indices_ */
  GLuint            material;       /* index to material for group */
  struct _GLMgroup* next;           /* pointer to next group in model */
} GLMgroup;

/* GLMmodel: Structure that defines a model.
 */
typedef struct _GLMmodel {
  char*    pathname;            /* path to this model */
  char*    mtllibname;          /* name of the material library */

  GLuint   numvertices;         /* number of vertices in model */
  GLfloat* vertices;            /* array of vertices  */

  GLuint   numnormals;          /* number of normals in model */
  GLfloat* normals;             /* array of normals */

  GLuint   numtexcoords;        /* number of texcoords in model */
  GLfloat* texcoords;           /* array of texture coordinates */

  GLuint   numfacetnorms;       /* number of facetnorms in model */
  GLfloat* facetnorms;          /* array of facetnorms */

  GLuint       numtriangles;    /* number of triangles in model */
  GLMtriangle* triangles;       /* array of triangles */

  GLuint       nummaterials;    /* number of materials in model */
  GLMmaterial* materials;       /* array of materials */

  GLuint       numgroups;       /* number of groups in model */
  GLMgroup*    groups;          /* linked list of groups */

  GLfloat position[3];          /* position of the model */

} GLMmodel;


/* glmUnitize: "unitize" a model by translating it to the origin and
 * scaling it to fit in a unit cube around the origin.  Returns the
 * scalefactor used.
 *
 * model - properly initialized GLMmodel structure
 */
GLfloat
glmUnitize(GLMmodel* model);

/* glmDimensions: Calculates the dimensions (width, height, depth) of
 * a model.
 *
 * model      - initialized GLMmodel structure
 * dimensions - array of 3 GLfloats (GLfloat dimensions[3])
 */
GLvoid
glmDimensions(GLMmodel* model, GLfloat* dimensions);

/* glmScale: Scales a model by a given amount.
 *
 * model - properly initialized GLMmodel structure
 * scale - scalefactor (0.5 = half as large, 2.0 = twice as large)
 */
GLvoid
glmScale(GLMmodel* model, GLfloat scale);

/* glmTranslate: Translates (moves) a model by a given amount in x, y, z.
*
* model - properly initialized GLMmodel structure
* offset- vector specifying amount to translate model by.
*/
GLvoid
glmTranslate(GLMmodel* model, const GLfloat offset[3]);

/* glmReverseWinding: Reverse the polygon winding for all polygons in
 * this model.  Default winding is counter-clockwise.  Also changes
 * the direction of the normals.
 *
 * model - properly initialized GLMmodel structure
 */
GLvoid
glmReverseWinding(GLMmodel* model);

/* glmFacetNormals: Generates facet normals for a model (by taking the
 * cross product of the two vectors derived from the sides of each
 * triangle).  Assumes a counter-clockwise winding.
 *
 * model - initialized GLMmodel structure
 */
GLvoid
glmFacetNormals(GLMmodel* model);

/* glmVertexNormals: Generates smooth vertex normals for a model.
 * First builds a list of all the triangles each vertex is in.  Then
 * loops through each vertex in the the list averaging all the facet
 * normals of the triangles each vertex is in.  Finally, sets the
 * normal index in the triangle for the vertex to the generated smooth
 * normal.  If the dot product of a facet normal and the facet normal
 * associated with the first triangle in the list of triangles the
 * current vertex is in is greater than the cosine of the angle
 * parameter to the function, that facet normal is not added into the
 * average normal calculation and the corresponding vertex is given
 * the facet normal.  This tends to preserve hard edges.  The angle to
 * use depends on the model, but 90 degrees is usually a good start.
 *
 * model - initialized GLMmodel structure
 * angle - maximum angle (in degrees) to smooth across
 */
GLvoid
glmVertexNormals(GLMmodel* model, GLfloat angle);

/* glmLinearTexture: Generates texture coordinates according to a
 * linear projection of the texture map.  It generates these by
 * linearly mapping the vertices onto a square.
 *
 * PRL, 20011114: The square lies in the x-z plane and is of the same
 * dimension as the model in this plane. Note that this mapping will
 * produce highly distored textures on polygons whose projections
 * onto the x-z plane have areas approaching zero, i.e. whose facet
 * normals have little or no y component.
 *
 * model - pointer to initialized GLMmodel structure
 */
GLvoid
glmLinearTexture(GLMmodel* model);

/* glmSpheremapTexture: Generates texture coordinates according to a
 * spherical projection of the texture map.  Sometimes referred to as
 * spheremap, or reflection map texture coordinates.  It generates
 * these by using the normal to calculate where that vertex would map
 * onto a sphere.  Since it is impossible to map something flat
 * perfectly onto something spherical, there is distortion at the
 * poles.  This particular implementation causes the poles along the X
 * axis to be distorted.
 *
 * model - pointer to initialized GLMmodel structure
 */
GLvoid
glmSpheremapTexture(GLMmodel* model);

/* glmDelete: Deletes a GLMmodel structure.
 *
 * model - initialized GLMmodel structure
 * contextIndex - PRL: index to the current OpenGL context (for texturing.) If you have only
 *             one OpenGL context (the most common case) set this parameter to 0.
 */
GLvoid
glmDelete(GLMmodel* model, const int contextIndex);

/* glmReadOBJ: Reads a model description from a Wavefront .OBJ file.
 * Returns a pointer to the created object which should be free'd with
 * glmDelete().
 *
 * filename - name of the file containing the Wavefront .OBJ format data.
 * contextIndex - PRL: index to the current OpenGL context (for texturing.)
 */
GLMmodel*
glmReadOBJ(char* filename,  const int contextIndex);

/* glmWriteOBJ: Writes a model description in Wavefront .OBJ format to
 * a file.
 *
 * model    - initialized GLMmodel structure
 * filename - name of the file to write the Wavefront .OBJ format data to
 * mode     - a bitwise or of values describing what is written to the file
 *            GLM_NONE    -  write only vertices
 *            GLM_FLAT    -  write facet normals
 *            GLM_SMOOTH  -  write vertex normals
 *            GLM_TEXTURE -  write texture coords
 *            GLM_FLAT and GLM_SMOOTH should not both be specified.
 */
GLvoid
glmWriteOBJ(GLMmodel* model, char* filename, GLuint mode);

/* glmDraw: Renders the model to the current OpenGL context using the
 * mode specified.
 *
 * model - initialized GLMmodel structure
 * mode  - a bitwise OR of values describing what is to be rendered.
 *             GLM_NONE     -  render with only vertices
 *             GLM_FLAT     -  render with facet normals
 *             GLM_SMOOTH   -  render with vertex normals
 *             GLM_TEXTURE  -  render with texture coords
 *             GLM_COLOR    -  render with colors (color material)
 *             GLM_MATERIAL -  render with materials
 *             GLM_COLOR and GLM_MATERIAL should not both be specified.
 *             GLM_FLAT and GLM_SMOOTH should not both be specified.
 * contextIndex - PRL: index to the current OpenGL context (for texturing.) If you have only
 *             one OpenGL context (the most common case) set this parameter to 0.
 */
GLvoid
glmDraw(GLMmodel* model, GLuint mode, const int contextIndex);

/* glmList: Generates and returns a display list for the model using
 * the mode specified.
 *
 * model - initialized GLMmodel structure
 * mode  - a bitwise OR of values describing what is to be rendered.
 *             GLM_NONE     -  render with only vertices
 *             GLM_FLAT     -  render with facet normals
 *             GLM_SMOOTH   -  render with vertex normals
 *             GLM_TEXTURE  -  render with texture coords
 *             GLM_COLOR    -  render with colors (color material)
 *             GLM_MATERIAL -  render with materials
 *             GLM_COLOR and GLM_MATERIAL should not both be specified.
 *             GLM_FLAT and GLM_SMOOTH should not both be specified.
 * contextIndex - PRL: index to the current OpenGL context (for texturing.) If you have only
 *             one OpenGL context (the most common case) set this parameter to 0.
 */
GLuint
glmList(GLMmodel* model, GLuint mode, const int contextIndex);

/* glmWeld: eliminate (weld) vectors that are within an epsilon of
 * each other.
 *
 * model      - initialized GLMmodel structure
 * epsilon    - maximum difference between vertices
 *              ( 0.00001 is a good start for a unitized model)
 *
 */
GLvoid
glmWeld(GLMmodel* model, GLfloat epsilon);


/* glmReadPPM: read a PPM raw (type P6) file.  The PPM file has a header
 * that should look something like:
 *
 *    P6
 *    # comment
 *    width height max_value
 *    rgbrgbrgb...
 *
 * where "P6" is the magic cookie which identifies the file type and
 * should be the only characters on the first line followed by a
 * carriage return.  Any line starting with a # mark will be treated
 * as a comment and discarded.   After the magic cookie, three integer
 * values are expected: width, height of the image and the maximum
 * value for a pixel (max_value must be < 256 for PPM raw files).  The
 * data section consists of width*height rgb triplets (one byte each)
 * in binary format (i.e., such as that written with fwrite() or
 * equivalent).
 *
 * The rgb data is returned as an array of unsigned chars (packed
 * rgb).  The malloc()'d memory should be free()'d by the caller.  If
 * an error occurs, an error message is sent to stderr and NULL is
 * returned.
 *
 * filename   - name of the .ppm file.
 * width      - will contain the width of the image on return.
 * height     - will contain the height of the image on return.
 *
 */

GLubyte*
glmReadPPM(const char* filename, int* width, int* height);

/* glmDrawArrays: Creates vertex, normal, and texcoord arrays of the model using the
 * mode specified.
 *
 * By Philip Lamb, phil@eden.net.nz, 20020128
 *
 * Uses model->vertices and model->normals and model->texcoords for data, assuming vertex n
 * has normal n and texcoord n. This will be the case for normals if the normals were created
 * by calling glmSmoothNormals() and the texcoords were created by calling glmLinearTexture().
 *
 * model - initialized GLMmodel structure
 * mode  - a bitwise OR of values describing what is to be rendered.
 *             GLM_NONE     -  render with only vertices
 *             GLM_FLAT     -  render with facet normals
 *             GLM_SMOOTH   -  render with vertex normals
 *             GLM_TEXTURE  -  render with texture coords
 *             GLM_COLOR    -  render with colors (color material)
 *             GLM_MATERIAL -  render with materials
 *             GLM_COLOR and GLM_MATERIAL should not both be specified.
 *             GLM_FLAT and GLM_SMOOTH should not both be specified.
 * contextIndex - PRL: index to the current OpenGL context (for texturing.) If you have only
 *             one OpenGL context (the most common case) set this parameter to 0.
 */

GLvoid
glmDrawArrays(GLMmodel* model, GLuint mode, const int contextIndex);

#endif                  /* !__glm_h__ */

/*
 *  edenSurfaces.c
 *
 *	Copyright (c) 2001-2003 Philip Lamb (PRL) phil@eden.net.nz. All rights reserved.
 *	
 *	Rev		Date		Who		Changes
 *	1.0.0	20011111	Initial version for The SRMS simulator.
 */

// ============================================================================
//	private includes
// ============================================================================
#include "eden/edenSurfaces.h"

#include <stdio.h>
#include <string.h>				// strcmp()
#include <stdlib.h>				// malloc(), calloc(), free()
#include "eden/readtex.h"			// ReadTex()

// ============================================================================
//	private types
// ============================================================================
#define SURFACE_ALPHA_TRANSLUCENCY	0.35
#define SURFACES_DEBUG			// Uncomment to build version that outputs debug info to stderr.

typedef struct {
	GLfloat Ambient[4];
	GLfloat Diffuse[4];
	GLfloat Specular[4];
	GLfloat Shininess[1];
} MATERIAL;

typedef struct {
	GLuint		name;
	GLint		env_mode;
	//GLfloat	env_color[4];
} TEXTURE_t;

// ============================================================================
//	global variables
// ============================================================================
// materials.
	
static const MATERIAL materials[]=	
{
	// blackmetal.
	{
		{0.1, 0.1, 0.1, 1.0},
		{0.1, 0.1, 0.1, 1.0},
		{0.508273, 0.508273, 0.508373, 1.0},
		{128.0 * 0.4}
	},
	
	// gold.
	{
		{0.24725, 0.1995, 0.0745, 1.0},
		{0.75164, 0.60648, 0.22648, 1.0},
		{0.628281, 0.555802, 0.366065, 1.0},
		{128.0 * 0.4}
	},

	// gold translucent.
	{
		{0.24725, 0.1995, 0.0745, SURFACE_ALPHA_TRANSLUCENCY},
		{0.75164, 0.60648, 0.22648, SURFACE_ALPHA_TRANSLUCENCY},
		{0.628281, 0.555802, 0.366065, SURFACE_ALPHA_TRANSLUCENCY},
		{128.0 * 0.4}
	},
	
	// silver.
	{
		{0.19225, 0.19225, 0.19225, 1.0},
		{0.50754, 0.50754, 0.50754, 1.0},
		{0.508273, 0.508273, 0.508373, 1.0},
		{128.0 * 0.6}
	},
	
	// whitemetal.	
	{
		{0.20725, 0.20725, 0.20725, 1.0},
		{1.0, 1.0, 1.0, 1.0},
		{0.296648, 0.296648, 0.296648, 1.0},
		{128.0 * 0.088}
	},

	// whitemetal translucent.
	{
		{0.20725, 0.20725, 0.20725, SURFACE_ALPHA_TRANSLUCENCY},
		{1.0, 1.0, 1.0, SURFACE_ALPHA_TRANSLUCENCY},
		{0.296648, 0.296648, 0.296648, SURFACE_ALPHA_TRANSLUCENCY},
		{128.0 * 0.088}
	},
	
	// greenmetal.	
	{
		{0.0215, 0.1745, 0.0215, 1.0},
		{0.07568, 0.61424, 0.07568, 1.0},
		{0.633, 0.727811, 0.633, 1.0},
		{128.0 * 0.6}
	},
	
	// redmetal.
	{
		{0.20725, 0.20725, 0.20725, 1.0},
		{1.0, 0.1, 0.1, 1.0},
		{0.296648, 0.296648, 0.296648, 1.0},
		{128.0 * 0.6}
	},
	
	// redmetal translucent.
	{
		{0.20725, 0.20725, 0.20725, SURFACE_ALPHA_TRANSLUCENCY},
		{1.0, 0.1, 0.1, SURFACE_ALPHA_TRANSLUCENCY},
		{0.296648, 0.296648, 0.296648, SURFACE_ALPHA_TRANSLUCENCY},
		{128.0 * 0.6}
	},
};
#define NUMMATERIALS ((int)(sizeof(materials) / sizeof(materials[0])))

static TEXTURE_t *gTextures = NULL;	// For our texture heaps (one per context.)
static unsigned int *gTextureIndexPtr = NULL; // For pointer into each heap.
static unsigned int gTextureIndexMax = 0;
static int gSurfacesContextsActiveCount = 0;


// ============================================================================
//	public functions
// ============================================================================

//
// Initialise the Surfaces library.
// This function must be called prior to any other Surfaces*() functions being called.
// Parameter contextsActiveCount is maximum number of OpenGL contexts that will
// be in use. Parameter textureIndexMax is maximum number of textures that can
// be loaded.
//
BOOL SurfacesInit(const int contextsActiveCount, const int textureIndexMax)
{
	gTextures = (TEXTURE_t *)malloc(sizeof(TEXTURE_t) * textureIndexMax * contextsActiveCount);
	gTextureIndexPtr = (unsigned int *)calloc(contextsActiveCount, sizeof(unsigned int));
	gTextureIndexMax = textureIndexMax;	

	gSurfacesContextsActiveCount = contextsActiveCount;

	return (TRUE);
}

//
// Set the OpenGL material to one of a few predefined materials.
//
BOOL SurfacesMaterialSet(int m)
{
#ifdef DEBUG_SURFACES_BACKFACES
	GLfloat red[4] = {1.0, 0.0, 0.0, 1.0};
	GLfloat reds[1] = {128*0.6};
#endif
	
	if ((m < 0) || (m >= NUMMATERIALS)) {
		fprintf(stderr, "SurfacesMaterialSet: invalid material index %d\n",m);
		return (FALSE);
	}
	glMaterialfv(GL_FRONT, GL_AMBIENT, materials[m].Ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materials[m].Diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materials[m].Specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, materials[m].Shininess);

#ifdef DEBUG_SURFACES_BACKFACES
	glMaterialfv(GL_BACK, GL_AMBIENT, red);
	glMaterialfv(GL_BACK, GL_DIFFUSE, red);
	glMaterialfv(GL_BACK, GL_SPECULAR, red);
	glMaterialfv(GL_BACK, GL_SHININESS, reds);
#endif
	
	return (TRUE);
}

//
// Finalise the Surfaces library.
// This function should be called once no more calls to any other Surfaces*()
// functions are required, to free up allocated memory.
//
BOOL SurfacesFinal(void)
{
	gTextureIndexMax = 0;
	free(gTextureIndexPtr);
	free(gTextures);
	
	gSurfacesContextsActiveCount = 0;
	return (TRUE);
}

//
// Loads numTextures textures into OpenGL texture objects, for the specified OpenGL
// context (the first context has a contextIndex of 0).
// The info for each texture is supplied in records in the array textureInfo[].
// After loading, the array textureIndices[] holds the index
// numbers assigned to each texture. These index numbers may then be used in calls to
// SurfacesTextureSet() and SurfacesTextureUnload(). (These indices are of type
// TEXTURE_INDEX_t, an unsigned int, and are only valid per-OpenGL context.)
// Returns TRUE if all textures were loaded correctly, FALSE if one or more were not.
// The index value of any textures not loaded will be equal to 0.
//
BOOL SurfacesTextureLoad(const int contextIndex, const int numTextures,
						 const TEXTURE_INFO_t *textureInfo, TEXTURE_INDEX_t *textureIndices)
{
	int i;
	unsigned int ptr;
	BOOL ok = TRUE;

	if (contextIndex < 0 || contextIndex >= gSurfacesContextsActiveCount) return (FALSE); // Sanity check.

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);	// ReadTex returns tightly packed texture data.

	for (i = 0; i < numTextures; i++) {

		// Find a free slot in the texture array.
		ptr =  gTextureIndexPtr[contextIndex];
		while (1) {
			ptr++;
			if (ptr == gTextureIndexMax) ptr = 0; // Loop around.
			if (ptr == gTextureIndexPtr[contextIndex]) {
				fprintf(stderr, "SurfacesTextureLoad(): Error, maximum number of textures already loaded.\n");
				return (FALSE);
			}
			if (gTextures[contextIndex * gTextureIndexMax + ptr].name == 0)
				break; // Free slots have zero GL texture names.
		}
		gTextureIndexPtr[contextIndex] = ptr;

		glGenTextures(1, &(gTextures[contextIndex * gTextureIndexMax + ptr].name));
		if (glGetError() != GL_NO_ERROR) {
			fprintf(stderr, "SurfacesTextureLoad(): GL error while generating texture name.\n");
			return (FALSE);
		}
		gTextures[contextIndex * gTextureIndexMax + ptr].env_mode = textureInfo[i].env_mode;

		// Bind the texture object and set its initial state.
		glBindTexture(GL_TEXTURE_2D, gTextures[contextIndex * gTextureIndexMax + ptr].name);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureInfo[i].wrap_s); 		// What to do with pixels outside [0,1].
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureInfo[i].wrap_t); 		// What to do with pixels outside [0,1].
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureInfo[i].mag_filter); 	// Interpolation when magnifying.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureInfo[i].mag_filter); 	// Subsampling when minifying.
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_PRIORITY, textureInfo[i].priority);

		// Read and send the actual texture data to GL.
#ifdef SURFACES_DEBUG
		fprintf(stderr, "SurfacesTextureLoad(): loading file %s\n", textureInfo[i].pathname);
#endif
		if (!ReadTex(textureInfo[i].pathname, textureInfo[i].internalformat, textureInfo[i].mipmaps)) {	// Load image and send to GL.
			fprintf(stderr,"SurfacesTextureLoad(): Unable to read file and/or upload texture image to OpenGL for file %s.\n", textureInfo[i].pathname);
			textureIndices[i] = 0;	// Assign no name if we couldn't load the image.
			ok = FALSE;
		} else {
			textureIndices[i] = ptr + 1;
		}
	}

	return (ok);
}

//
// Binds the OpenGL texture.
// The texture bound is specified by parameter textureIndex. This index is only valid
// if the current OpenGL context is contextIndex, and the index was returned by 
// SurfacesTextureLoad() with the same OpenGL context.
//
void SurfacesTextureSet(const int contextIndex, TEXTURE_INDEX_t textureIndex)
{
	unsigned int offset;

	if (textureIndex > 0 && textureIndex <= gTextureIndexMax) {
		offset = contextIndex * gTextureIndexMax + textureIndex - 1;
		glBindTexture(GL_TEXTURE_2D, gTextures[offset].name);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, gTextures[offset].env_mode);	// Environment mode specific to this texture.
		//glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, gTextures[offset].env_color);	// Environment colour specific to this texture.
	}
}

//
// Unloads numTextures textures from OpenGL texture objects, for the specified OpenGL context.
// After unloading, the corresponding index in textureIndices for each record in textureInfo[]
// is set to 0.
//
BOOL SurfacesTextureUnload(const int contextIndex, const int numTextures, TEXTURE_INDEX_t *textureIndices)
{
	BOOL ok = TRUE;
	int i;

	if (contextIndex < 0 || contextIndex >= gSurfacesContextsActiveCount) return (FALSE); // Sanity check.

	for (i = 0; i < numTextures; i++) {
		glDeleteTextures(1, &(gTextures[contextIndex * gTextureIndexMax + textureIndices[i] - 1].name));
		gTextures[contextIndex * gTextureIndexMax + textureIndices[i] - 1].name = 0;
		textureIndices[i] = 0;
	}
	return (ok);
}

/*
 *  edenSurfaces.h
 *
 *	Copyright (c) 2001-2003 Philip Lamb (PRL) phil@eden.net.nz. All rights reserved.
 *
 *	Rev		Date		Who		Changes
 *	1.0.0	20011111	Initial version for The SRMS simulator.
 */

#ifndef __edenSurfaces_h__
#define __edenSurfaces_h__

/* ============================================================================ *
 *	includes
 * ============================================================================ */
//#include "eden/eden.h"

#ifdef MACOSX
#include <GLUT/glut.h>
#elif defined(UNIX)
#include "glut.h"
#else
#include <GL/glut.h>
#endif

/* ============================================================================ *
 *	defines
 * ============================================================================ */
// Symbolic names for materials.
#define MATERIAL_BLACKMETAL			0
#define MATERIAL_GOLD				1
#define MATERIAL_GOLD_TRANSLUCENT	2
#define MATERIAL_SILVER				3
#define MATERIAL_WHITEMETAL			4
#define MATERIAL_WHITEFABRIC		4			/* not yet defined, so just use white metal */
#define MATERIAL_WHITEMETAL_TRANSLUCENT		5
#define MATERIAL_WHITEFABRIC_TRANSLUCENT	5	/* not yet defined, so just use white metal */
#define MATERIAL_GREENMETAL			6
#define MATERIAL_RED				7
#define MATERIAL_RED_TRANSLUCENT	8

#if 0
// Symbolic names for faces of objects to texture.
#define TEXTURE_FACE_TOP_BIT	0x01
#define TEXTURE_FACE_RIGHT_BIT	0x02
#define TEXTURE_FACE_BACK_BIT	0x04
#define TEXTURE_FACE_LEFT_BIT	0x08
#define TEXTURE_FACE_FRONT_BIT	0x10
#define TEXTURE_FACE_BOTTOM_BIT	0x20
#endif

// Debugging.
//#define DEBUG_SURFACES_BACKFACES		/* Uncomment to colour back faces of polygons red. */

/* ============================================================================ *
*	public types
* ============================================================================ */

typedef unsigned int TEXTURE_INDEX_t;

typedef struct {
	char *		pathname;
	GLboolean	mipmaps;
	GLint		internalformat;
	GLint		min_filter;
	GLint		mag_filter;
	GLint		wrap_s;
	GLint		wrap_t;
	GLclampf	priority;
	GLint		env_mode;
	//GLfloat	env_color[4];
} TEXTURE_INFO_t;

/* ============================================================================ *
 *	function prototypes for public functions
 * ============================================================================ */
BOOL SurfacesInit(const int contextsActiveCount, const int textureIndicesMax);
BOOL SurfacesMaterialSet(int m);
BOOL SurfacesTextureLoad(const int contextIndex, const int numTextures,
						 const TEXTURE_INFO_t *textureInfo, TEXTURE_INDEX_t *textureIndices);
void SurfacesTextureSet(const int contextIndex, TEXTURE_INDEX_t textureIndex);
BOOL SurfacesTextureUnload(const int contextIndex, const int numTextures, TEXTURE_INDEX_t *textureIndices);
BOOL SurfacesFinal(void);

#endif		/* !__edenSurfaces_h__ */

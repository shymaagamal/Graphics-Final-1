/* readtex.c */

/* 
 * Read an SGI .rgb image file and generate a mipmap texture set.
 * Much of this code was borrowed from SGI's tk OpenGL toolkit.
 *
 *	By Mark Kilgard (MK) mkilgard@nvidia.com
 *	Revised by Geoff Stahl (GGS) gstahl@apple.com
 *	Revised by Philip Lamb (PRL) phil@eden.net.nz
 *
 *	Rev		Date		Who		Changes
 *	1.0.1	19990420	GGS		Fixed memory leaks.
 *	1.0.2	20011111	PRL		Improved commenting.
 *	1.0.3	20011219	PRL		Fixed more memory leaks.
 *	1.0.4	20020103	PRL		Fixed more memory leaks in case of error and improved error handling.
 */

#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "eden/readtex.h"

#ifndef SEEK_SET
#define SEEK_SET 0
#endif

/******************************************************************************/

/* RGB Image Structure */
typedef struct _TK_RGBImageRec {
  GLint sizeX, sizeY;
  GLint components;
  unsigned char *data;
} TK_RGBImageRec;


typedef struct _rawImageRec {
  unsigned short imagic;				// Filled in with bytes 0-1 of file by RawImageOpen.
  unsigned short type;					// Filled in with bytes 2-3 of file by RawImageOpen.
  unsigned short dim;					// Filled in with bytes 4-5 of file by RawImageOpen.
  unsigned short sizeX, sizeY, sizeZ;	// Filled in with bytes 6-11 of file by RawImageOpen.
  unsigned long min, max;
  unsigned long wasteBytes;
  char name[80];
  unsigned long colorMap;
  FILE *file;
  unsigned char *tmp, *tmpR, *tmpG, *tmpB, *tmpA;
  unsigned long rleEnd;
  GLuint *rowStart;
  GLint *rowSize;
} rawImageRec;

/******************************************************************************/

static void ConvertShort(unsigned short *array, size_t length)
{
  unsigned short b1, b2;
  unsigned char *ptr;

  ptr = (unsigned char *) array;
  while (length--) {
    b1 = *ptr++;
    b2 = *ptr++;
    *array++ = (b1 << 8) | (b2);
  }
}

static void ConvertLong(GLuint * array, size_t length)
{
  unsigned long b1, b2, b3, b4;
  unsigned char *ptr;

  ptr = (unsigned char *) array;
  while (length--) {
    b1 = *ptr++;
    b2 = *ptr++;
    b3 = *ptr++;
    b4 = *ptr++;
    *array++ = (b1 << 24) | (b2 << 16) | (b3 << 8) | (b4);
  }
}

static rawImageRec *RawImageOpen(const char *fileName)
{
	union {
		int testWord;
		char testByte[4];
	} endianTest;
	rawImageRec *raw;
	GLenum swapFlag;
	int x;

	endianTest.testWord = 1;
	if (endianTest.testByte[0] == 1) {
		swapFlag = GL_TRUE;
	} else {
		swapFlag = GL_FALSE;
	}

	raw = (rawImageRec *) malloc(sizeof(rawImageRec));
	if (raw == NULL) {
		fprintf(stderr, "RawImageOpen(): Out of memory!\n");
		return NULL;
	}
	if ((raw->file = fopen(fileName, "rb")) == NULL) {
		fprintf(stderr,"RawImageOpen(): Unable to open file %s.\n    ", fileName);
		perror(fileName);
		/* GGS 4/20/99: should free allocate memory for raw if we can't open the file. */
		goto freeRaw;
	}
	/* Read 12 bytes, filling in imagic, type, dim, and sizex, sizey, and sizez. */
	fread(raw, 1, 12, raw->file);

	if (swapFlag) {
		ConvertShort(&raw->imagic, 6);
	}
	raw->tmp = (unsigned char *) malloc(raw->sizeX * 256);
	if (raw->tmp == NULL) {
		fprintf(stderr, "RawImageOpen(): Out of memory!\n");
		/* GGS 4/20/99: should free memory for raw if we can't allocate. */
		goto freeRaw;
	}
	raw->tmpR = (unsigned char *) malloc(raw->sizeX * 256);
	if (raw->tmpR == NULL) {
		fprintf(stderr, "RawImageOpen(): Out of memory!\n");
		/* GGS 4/20/99: should free memory for raw->tmp and raw if we can't allocate. */
		goto freeRawTmp;
	}
	if (raw->sizeZ > 1) {
		raw->tmpB = (unsigned char *) malloc(raw->sizeX * 256);
		if (raw->tmpB == NULL) {
			fprintf(stderr, "RawImageOpen(): Out of memory!\n");
			/* GGS 4/20/99: should free memory for raw-tmpR, raw->tmp and raw if we can't allocate. */
			goto freeRawTmpR;
		}
		raw->tmpG = (unsigned char *) malloc(raw->sizeX * 256);
		if (raw->tmpG == NULL) {
			fprintf(stderr, "RawImageOpen(): Out of memory!\n");
			/* GGS 4/20/99: should free memory for ..., raw->tmpR, raw->tmp and raw if we can't allocate. */
			goto freeRawTmpB;
		}
		if (raw->sizeZ == 4) {
			raw->tmpA = (unsigned char *) malloc(raw->sizeX * 256);
			if (raw->tmpA == NULL) {
				fprintf(stderr, "RawImageOpen(): Out of memory!\n");
				/* GGS 4/20/99: should free memory for ..., raw->tmpR, raw->tmp and raw if we can't allocate. */
				goto freeRawTmpG;
			}
		}
	}
	if ((raw->type & 0xFF00) == 0x0100) {
		x = raw->sizeY * raw->sizeZ * sizeof(GLuint);
		raw->rowStart = (GLuint *) malloc(x);
		if (raw->rowStart == NULL) {
			fprintf(stderr, "RawImageOpen(): Out of memory!\n");
			/* GGS 4/20/99: should free memory for ..., raw->tmpR,
			raw->tmp and raw if we can't allocate. */
			goto freeRawTmpA;
		}
		raw->rowSize = (GLint *) malloc(x);
		if (raw->rowSize == NULL) {
			fprintf(stderr, "RawImageOpen(): Out of memory!\n");
			/* GGS 4/20/99: should free memory for ..., raw->tmpR, 
			raw->tmp and raw if we can't allocate. */
			goto freeRawRowStart;
		}
		raw->rleEnd = 512 + (2 * x);
		fseek(raw->file, 512, SEEK_SET);
		fread(raw->rowStart, 1, x, raw->file);
		fread(raw->rowSize, 1, x, raw->file);
		if (swapFlag) {
			ConvertLong(raw->rowStart, x / sizeof(GLuint));
			ConvertLong((GLuint *) raw->rowSize, x / sizeof(GLint));
		}
	}
	return raw;
	
	/* PRL 20011219: When bailing out, do the correct free()s. */
	freeRawRowStart:
		free (raw->rowStart);
	freeRawTmpA:
		if (raw->sizeZ == 4) free (raw->tmpA);
	freeRawTmpB:
		if (raw->sizeZ > 1) free (raw->tmpB);
	freeRawTmpG:
		if (raw->sizeZ > 1) free (raw->tmpG);
	freeRawTmpR:
		free (raw->tmpR);
	freeRawTmp:
		free (raw->tmp);
	freeRaw:
		free(raw);
		raw = NULL;
		return (NULL);
}

static void 
RawImageClose(rawImageRec * raw)
{

	fclose(raw->file);
	free(raw->tmp);
	free(raw->tmpR);
	/* PRL 20011219: Only free these other structures for RGB / RGBA images. */
	if (raw->sizeZ > 1) {
		free(raw->tmpG);
		free(raw->tmpB);
	}
	if (raw->sizeZ == 4) {
		free(raw->tmpA);
	}
	if ((raw->type & 0xFF00) == 0x0100) {
		free(raw->rowStart);
		free(raw->rowSize);
	}
	free(raw);
}

static void 
RawImageGetRow(rawImageRec *raw, unsigned char *buf, int y, int z)
{
  unsigned char *iPtr, *oPtr, pixel;
  int count;

  if ((raw->type & 0xFF00) == 0x0100) {
    fseek(raw->file, (long) raw->rowStart[y + z * raw->sizeY], SEEK_SET);
    fread(raw->tmp, 1, (size_t) raw->rowSize[y + z * raw->sizeY],
      raw->file);

    iPtr = raw->tmp;
    oPtr = buf;
    for (;;) {
      pixel = *iPtr++;
      count = (int) (pixel & 0x7F);
      if (!count) {
        return;
      }
      if (pixel & 0x80) {
        while (count--) {
          *oPtr++ = *iPtr++;
        }
      } else {
        pixel = *iPtr++;
        while (count--) {
          *oPtr++ = pixel;
        }
      }
    }
  } else {
    fseek(raw->file, 512 + (y * raw->sizeX) + (z * raw->sizeX * raw->sizeY),
      SEEK_SET);
    fread(buf, 1, raw->sizeX, raw->file);
  }
}

static void 
RawImageGetData(rawImageRec * raw, TK_RGBImageRec * final)
{
  unsigned char *ptr;
  int i, j;

  final->data = (unsigned char *) malloc((raw->sizeX + 1) * (raw->sizeY + 1) * 4);
  if (final->data == NULL) {
    fprintf(stderr, "RawImageGetData(): Out of memory!\n");
    free (final);			// PRL 20020103: Clean up and bail out if we can't allocate.
    return;
  }
  ptr = final->data;
  for (i = 0; i < (int) (raw->sizeY); i++) {
    RawImageGetRow(raw, raw->tmpR, i, 0);
    if (raw->sizeZ > 1) {
      RawImageGetRow(raw, raw->tmpG, i, 1);
      RawImageGetRow(raw, raw->tmpB, i, 2);
      if (raw->sizeZ > 3) {
        RawImageGetRow(raw, raw->tmpA, i, 3);
      }
    }
    for (j = 0; j < (int) (raw->sizeX); j++) {
      *ptr++ = *(raw->tmpR + j);
      if (raw->sizeZ > 1) {
        *ptr++ = *(raw->tmpG + j);
        *ptr++ = *(raw->tmpB + j);
        if (raw->sizeZ > 3) {
          *ptr++ = *(raw->tmpA + j);
        }
      }
    }
  }
}

static TK_RGBImageRec *tkRGBImageLoad(const char *fileName)
{
	rawImageRec *raw;
	TK_RGBImageRec *final;

	raw = RawImageOpen(fileName);
	if (raw == NULL) {
		return (NULL);			// PRL 20020103: Bail out if we couldn't even open the image.
	}
	final = (TK_RGBImageRec *) malloc(sizeof(TK_RGBImageRec));
	if (final == NULL) {
		fprintf(stderr, "tkRGBImageLoad(): Out of memory!\n");
		RawImageClose(raw);		// PRL 20020103: Free raw image if bailing out.
		return (NULL);
	}
	final->sizeX = raw->sizeX;
	final->sizeY = raw->sizeY;
	final->components = raw->sizeZ;
	RawImageGetData(raw, final);
	RawImageClose(raw);
	return final;
}

static void FreeImage(TK_RGBImageRec * image)
{
  free(image->data);
  free(image);
}

/* 
 * Load an SGI .rgb file and generate a set of 2-D mipmaps from it.
 * Input:  imageFile - name of .rgb to read
 *         intFormat - internal texture format to use, or number of components
 *         makeMipmaps - if set to TRUE, build a set of mipmaps from the texture
 * Return:  GL_TRUE if success, GL_FALSE if error.
 */
GLboolean 
ReadTex(const char *imageFile, const GLint intFormat, const GLboolean makeMipmaps)
{
	GLint error;
	GLenum format;
	TK_RGBImageRec *image;

	image = tkRGBImageLoad(imageFile);
	if (!image) {
		return GL_FALSE;
	}
	if (image->components == 3) {
		format = GL_RGB;
	} else if (image->components == 4) {
		format = GL_RGBA;
	} else if (image->components == 1) {
		format = GL_LUMINANCE;
	} else {
		/* not implemented */
		fprintf(stderr, "ReadTex(): %d-component images not implemented.\n", (int)(image->components));
  		FreeImage(image);		// PRL 20020103
		return GL_FALSE;
	}

	if (makeMipmaps) {
		if ((error = gluBuild2DMipmaps(GL_TEXTURE_2D,
							intFormat,
							image->sizeX, image->sizeY,
							format,
							GL_UNSIGNED_BYTE,
							image->data)))
			fprintf(stderr, "ReadTex(): gluBuild2DMipmaps returned error %d.\n", (int)(error));
	} else {
		glTexImage2D(GL_TEXTURE_2D,
			   0,									// mimmap level, 0 = base
			   intFormat,
			   image->sizeX, image->sizeY,
			   0,									// border
			   format,
			   GL_UNSIGNED_BYTE,
			   image->data);
		if ((error = glGetError()) != GL_NO_ERROR)
			fprintf(stderr, "ReadTex(): glTexImage2D returned error %d.\n", (int)(error));
	}
	FreeImage(image);
	return (error ? GL_FALSE : GL_TRUE);
}

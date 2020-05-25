//
//  eden.h
//
//	Compiler and platform specific constants, needed for entire projects.
//  This file should be included with the project as a prefix header
//  (i.e., implicitly included from every source file.)
//
//  Copyright (c) 2001-2003 Philip Lamb (PRL) phil@eden.net.nz. All rights reserved.
//	
//	Rev		Date		Who		Changes
//	0.0.1	20011012	PRL		Initial version for The SRMS simulator.
//

#ifndef __eden_h__
#define __eden_h__

// I'd rather not use <stdbool.h> actually.
typedef int BOOL;
#define TRUE  1
#define FALSE 0
#define ON 	1
#define OFF	0

// Unsigned character-valued primary colours, without delimeters, for use with glColor3ub */
#define BLACK	0,0,0
#define WHITE	255,255,255
#define RED		255,0,0
#define GREEN	0,255,0
#define BLUE	0,0,255
#define CYAN	0,255,255
#define MAGENTA	255,0,255
#define YELLOW	255,255,0

// ASCII keycodes */
#define ASCII_ESC	27
#define ASCII_TAB	9
#define ASCII_CR	13
#define ASCII_DEL	127

// Test for platform capabilities. 
//#if defined(something) || defined (somethingelse)
//#elif
//#endif
//#define DARWINONLY
#ifdef __APPLE__	// gcc on Mac OS X.
#  ifdef DARWINONLY
#    define DARWIN	// Uses Darwin Unix.
#    define UNIX		// Its a Unix system too!
#    define BIGENDIAN	// Most Significant Byte is highest in memory.
#    define GLUT37	// Has GLUT library v3.7.
#  else
#    define DARWIN	// Uses Darwin Unix.
#    define UNIX		// Its a Unix system too!
#    define MACOSX	// Running under Mac OS X.
#    define BIGENDIAN	// Most Significant Byte is highest in memory.
#    define GLUT37	// Has GLUT library v3.7.
#    define QUARTZ	// Has Quartz API available.
#    define HID		// Has HID API available (for joystick).
#    define CARBON	// Has Carbon API available.
#    define ARTOOLKIT	// Has ARToolKit available.
//#  define MAC_OS_X_VERSION_MIN_REQUIRED MAC_OS_X_VERSION_10_1
#  endif // DARWINONLY
#endif	/* __APPLE__ */

#ifdef __MRC__		// MrC on Mac OS Classic.
#  define MACOS
#  define BIGENDIAN	// Most Significant Byte is highest in memory.
#  define GLUT37	// Has GLUT library v3.7.
#  define HID		// Has HID API available (for joystick).
#  define CARBON	// Has Carbon API available.
#endif	/* __MRC__ */


#ifdef _WIN32 		// Generic Windows.
#  undef BIGENDIAN	// Least Significant Byte is highest in memory.
#  define GLUT37	// Has GLUT library v3.7.
#  define ARTOOLKIT	// Has ARToolKit available.
#endif	/* _WIN32 */

#ifdef _POSIX 		// Generic POSIX-compliant Unix.
#  define UNIX
#  define GLUT37	// Has GLUT library v3.7.
#endif	/* _POSIX */

#ifdef __sgi		// Irix.
#  define BIGENDIAN	// Most Significant Byte is highest in memory.
#  define UNIX		// Its a Unix system.
#  define GLUT37	// Has GLUT library v3.7.
#endif	/* __sgi */

#ifdef __linux__	// gcc on Linux.
#  define UNIX		// Its a Unix-like system.
#  define GLUT37	// Has GLUT library v3.7.
#endif	/* __linux__ */

#ifdef __UNIX__		// gcc on Unix, including Solaris & HP-UX, excluding Mac OS X.
#  define UNIX
#  define GLUT37	// Has GLUT library v3.7.
#endif	/* __UNIX__ */

// Double check architecture endianess, using gcc's macro.
#if defined (__BIG_ENDIAN__) && !defined(BIGENDIAN)
#  warning BIGENDIAN should be defined for this platform in eden.h
#  define BIGENDIAN
#elif defined (__LITTLE_ENDIAN__) && defined(BIGENDIAN)
#  warning BIGENDIAN should be undefined for this platform in eden.h
#  undef BIGENDIAN
#endif

//
// Application code which is dependent on platform capabilities.
//
// Generate a callback when keys are released if GLUT version 3.7 is available.
#ifdef GLUT37
#  define KEYUPS
#endif // GLUT37

// GLUT on Windows requires callbacks to be declared type CALLBACK.
// Define it here as NULL for portability.
#ifndef CALLBACK
#  define CALLBACK
#endif

// The pthreads API is available on UNIX systems.
#ifdef UNIX
#  define PTHREADS
#endif

#endif		/* !__eden_h__ */


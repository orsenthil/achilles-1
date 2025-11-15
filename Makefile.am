bin_PROGRAMS = achilles
achilles_SOURCES = universe.cc angle.cc braininfo.cc color.cc energy.cc gene.cc  hebbian.cc id.cc idserver.cc license.cc main.cc opengl.cc org.cc orglist.cc quicksort.cc screenshot.cc vector.cc world.cc angle.h confdefs.h gene.h idserver.h orglist.h vector.h braininfo.h defines.h hebbian.h opengl.h quicksort.h world.h color.h energy.h id.h org.h universe.h

# Use pkg-config detected flags for SDL2, OpenGL, and GLU
# Debug flags: Add -D_DEBUG_FOOD_VISION to enable food vision debugging
# Option 1: Uncomment the line below to always enable debug output
AM_CPPFLAGS = @SDL2_CFLAGS@ @GL_CFLAGS@ @GLU_CFLAGS@ -D_DEBUG_FOOD_VISION
# Option 2: Use CPPFLAGS environment variable: CPPFLAGS=-D_DEBUG_FOOD_VISION ./configure
#AM_CPPFLAGS = @SDL2_CFLAGS@ @GL_CFLAGS@ @GLU_CFLAGS@ @DEBUG_CPPFLAGS@
AM_LDFLAGS = @SDL2_LIBS@ @GL_LIBS@ @GLU_LIBS@
achilles_LDADD = @SDL2_LIBS@ @GL_LIBS@ @GLU_LIBS@

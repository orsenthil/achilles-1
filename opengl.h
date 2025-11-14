/* 

Copyright (C) 2000 Matthew Danish

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#ifndef OPENGL_H_67548
#define OPENGL_H_67548
#include<stack>
using namespace std;

#include"conf.h"
#ifdef HAVE_SDL_SDL_H
#include<SDL/SDL.h>
#elif defined(HAVE_SDL_H)
#include<SDL.h>
#else
// Try to find SDL - check multiple possible locations
#if __has_include(<SDL2/SDL.h>)
#include<SDL2/SDL.h>
#elif __has_include(<SDL/SDL.h>)
#include<SDL/SDL.h>
#elif __has_include(<SDL.h>)
#include<SDL.h>
#else
// Last resort: try SDL2 in common non-standard locations
#include</home/linuxbrew/.linuxbrew/include/SDL2/SDL.h>
#endif
#endif

class OrganismClass;
struct EventStack;

class OpenGLClass {
private:
#ifdef SDL_VERSION_ATLEAST
  SDL_Window *window;
  SDL_GLContext glcontext;
  SDL_Surface *surface; // Keep for compatibility, not used in SDL2
#else
  SDL_Surface *surface;
#endif
  bool took_screenshot;
public:
  OpenGLClass(int,char **);
  ~OpenGLClass();
  bool InitGL(int,int);
  bool DrawLandscape(double,double,double);
  bool Transform(double,double,double,double);
  bool DrawOrganism(OrganismClass *);
  bool SwapBuffers();
  bool Clear();
  bool DrawStack(stack<EventStack> &);
  bool CheckInput(double &,double &,double &,double &,int &,int &);
  bool Screenshot(char *);
};

#endif

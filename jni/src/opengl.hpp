#ifndef OPENGL_HPP
#define OPENGL_HPP

#if defined(__IPHONEOS__) || defined(__ANDROID__)
#define HAVE_OPENGLES
#endif

#ifdef HAVE_OPENGLES
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif

#define OFFSET_OF(type, field) ((void*)offsetof(type, field))
#define STRINGIFY(str) #str

#endif // OPENGL_HPP

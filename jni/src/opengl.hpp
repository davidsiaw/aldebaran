#ifndef OPENGL_HPP
#define OPENGL_HPP

#if defined(__IPHONEOS__) || defined(__ANDROID__)
#define HAVE_OPENGLES
#endif

// For Ubuntu
#define GL_GLEXT_PROTOTYPES

// Switch for OpenGL ES
#ifdef HAVE_OPENGLES
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif

// Open GL Math tools
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#define OFFSET_OF(type, field) ((void*)offsetof(type, field))
#define STRINGIFY(str) #str

#endif // OPENGL_HPP

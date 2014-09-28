#pragma once

#include "GL/glew.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

#if defined(__APPLE__)   
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <OpenGL/glext.h>
#elif defined(WIN32)
#include <GL/glew.h>
#include <GL/gl.h>
#endif

#ifdef _WIN32
#define NOMINMAX
#include "windows.h"
#include <sstream>
#define TRACE(aExp) { std::ostringstream gloOutStream; gloOutStream << aExp << std::endl; OutputDebugStringA(gloOutStream.str().c_str()); }
#else
#include <iostream>
#define TRACE(aExp) { std::cout << aExp << std::endl; }
#endif

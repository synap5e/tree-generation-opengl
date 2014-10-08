
SET(GLEW_SOURCES
	glew.c
)

SET(GLEW_HEADERS
	"${GLEW_SOURCE_DIR}/include/GL/glew.h"
	"${GLEW_SOURCE_DIR}/include/GL/glxew.h"
	"${GLEW_SOURCE_DIR}/include/GL/wglew.h"
)

add_library(glew ${GLEW_SOURCES} ${GLEW_HEADERS})

# list libraries that must be linked with this build of GLEW (empty)
list(APPEND GLEW_LIBRARIES_impl)

# list macros that must be defined when using this build of GLEW
list(APPEND GLEW_DEFINITIONS_impl -DGLEW_STATIC )

# export
set(GLEW_LIBRARIES ${GLEW_LIBRARIES_impl} CACHE STRING "GLEW required libraries" FORCE)
set(GLEW_DEFINITIONS ${GLEW_DEFINITIONS_impl} CACHE STRING "GLEW required definitions" FORCE)

add_definitions(${GLEW_DEFINITIONS})

set_target_properties(glew
    PROPERTIES
    LINKER_LANGUAGE CXX
)

project(MUPARSER)

include_directories("${MUPARSER_DIR}/include")



SET(MUPARSER_SOURCES
	"${MUPARSER_SOURCE_DIR}/src/muParserBase.cpp"
	"${MUPARSER_SOURCE_DIR}/src/muParserBytecode.cpp"
	"${MUPARSER_SOURCE_DIR}/src/muParserCallback.cpp"
	"${MUPARSER_SOURCE_DIR}/src/muParser.cpp"
	"${MUPARSER_SOURCE_DIR}/src/muParserError.cpp"
	"${MUPARSER_SOURCE_DIR}/src/muParserInt.cpp"
	"${MUPARSER_SOURCE_DIR}/src/muParserTest.cpp"
	"${MUPARSER_SOURCE_DIR}/src/muParserTokenReader.cpp"
)

SET(MUPARSER_HEADERS
	"${MUPARSER_SOURCE_DIR}/include/muParserBase.h"
	"${MUPARSER_SOURCE_DIR}/include/muParserBytecode.h"
	"${MUPARSER_SOURCE_DIR}/include/muParserCallback.h"
	"${MUPARSER_SOURCE_DIR}/include/muParserDef.h"
	"${MUPARSER_SOURCE_DIR}/include/muParserDLL.h"
	"${MUPARSER_SOURCE_DIR}/include/muParserError.h"
	"${MUPARSER_SOURCE_DIR}/include/muParserFixes.h"
	"${MUPARSER_SOURCE_DIR}/include/muParser.h"
	"${MUPARSER_SOURCE_DIR}/include/muParserInt.h"
	"${MUPARSER_SOURCE_DIR}/include/muParserStack.h"
	"${MUPARSER_SOURCE_DIR}/include/muParserTemplateMagic.h"
	"${MUPARSER_SOURCE_DIR}/include/muParserTest.h"
	"${MUPARSER_SOURCE_DIR}/include/muParserToken.h"
	"${MUPARSER_SOURCE_DIR}/include/muParserTokenReader.h"
)

add_library(muparser ${MUPARSER_SOURCES} ${MUPARSER_HEADERS})

set_target_properties(muparser
    PROPERTIES
    LINKER_LANGUAGE CXX
)


cmake_minimum_required (VERSION 2.8)
project (json)

if (CMAKE_COMPILER_IS_GNUCXX)
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall -Werror=switch -Werror=uninitialized -Werror=address -Werror=return-type -Werror=maybe-uninitialized -Werror=format -Werror=double-promotion -Werror=return-local-addr -std=c++0x")
endif()

set (JSON_SOURCES
	src/format.cc
	src/json.cc
	src/jsonparser.cc
	src/lexer.cc
	src/lexerscanner.cc
	src/main.cc
	src/string.cc
	src/variant.cc
)

set (JSON_HEADERS
	src/containers.h
	src/format.h
	src/json.h
	src/jsonparser.h
	src/lexer.h
	src/lexerscanner.h
	src/macros.h
	src/main.h
	src/string.h
	src/tokens.h
	src/types.h
	src/variant.h
)

add_executable (json ${JSON_SOURCES})
install (TARGETS json RUNTIME DESTINATION bin)

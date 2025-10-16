cmake_minimum_required(VERSION 3.18)
project(graytool)

include_directories(include deps)

set(SRC
src/main.c
src/cli.c
src/generator.c
src/image.c
)

add_executable(graytool ${SRC})

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	target_link_options(graytool PRIVATE -static -static-libgcc)
endif()

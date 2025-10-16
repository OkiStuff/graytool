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

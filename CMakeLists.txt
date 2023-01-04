cmake_minimum_required(VERSION 3.20)
project(SHELL_SCANF)


set(PROJECT_ROOT_PATH "${SHELL_SCANF_SOURCE_DIR}")   
set(LIB_SRC ${PROJECT_ROOT_PATH}/core/shell.c ${PROJECT_ROOT_PATH}/lib/list.c)
set(EX_SCANF ${PROJECT_ROOT_PATH}/example/shell_scanf_test/main.c)

include_directories(${PROJECT_ROOT_PATH}/include)

add_library(shell_scanf SHARED ${LIB_SRC})
add_library(shell_scanf_static STATIC ${LIB_SRC})

add_executable(ex_scanf ${EX_SCANF})

target_link_libraries(ex_scanf ${PROJECT_BINARY_DIR}/libshell_scanf.so)

cmake_minimum_required(VERSION 3.23)
project(MovieServer C)


if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)
else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -ansi -pedantic -Wparentheses -Wformat=2 -Wshadow -Wwrite-strings
            -Wredundant-decls -Wmissing-include-dirs -pedantic-errors -Wuninitialized -Wmissing-declarations -Wconversion
            -Wdouble-promotion -Wfloat-equal  -Wstrict-prototypes -Wnested-externs -Wold-style-definition
            -Wbad-function-cast -Wno-unknown-pragmas -Wno-unused-function -Wjump-misses-init -Wlogical-op)
endif()

set(CMAKE_C_STANDARD 90)

add_executable(MovieServer main.c MovieEntities/Movie.h MovieEntities/Movie.c MovieEntities/MovieDatabase.c MovieEntities/MovieDatabase.h util/cJSON.c util/cJSON.h info/LoginInfo.c info/LoginInfo.h info/SessionInfo.c info/SessionInfo.h info/CreateInfo.c info/CreateInfo.h info/TransactionInfo.c info/TransactionInfo.h util/socket_helper.c util/socket_helper.h Server.c Server.h)
target_link_libraries(MovieServer ws2_32)

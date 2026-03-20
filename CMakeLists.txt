cmake_minimum_required(VERSION 3.10)
project(derivator LANGUAGES C)

add_executable(derivator
    src/ast/node.c
    src/ast/tree.c
    src/frontend/lexer.c
    src/frontend/parser.c
    src/output/dump.c
    src/derivator.c
    src/simplifier.c
    src/main.c
)

target_include_directories(derivator PRIVATE include)
target_compile_features(derivator PRIVATE c_std_11)
target_link_libraries(derivator m)
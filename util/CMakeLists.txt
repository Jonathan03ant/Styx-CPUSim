# Utility library - error handling and bit operations
# Creates libstyx-util.a

add_library(styx-util OBJECT
    errors.c
)

# This library needs common types
target_include_directories(styx-util PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)
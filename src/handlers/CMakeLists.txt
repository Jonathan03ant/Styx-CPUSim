# Handlers library - instruction execution handlers
# Creates libstyx-handlers.a

add_library(styx-handlers OBJECT
    exec_arith.c
    exec_memory.c
    exec_control.c
    exec_misc.c
)

# Handlers depend on:
# - Public API headers (include/)
# - isa_handlers.h (in this directory)
# - Core library (CPU, registers, memory)
# - ISA library (ALU, decoder)
target_include_directories(styx-handlers PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}  # For isa_handlers.h
)

# Link against both core and ISA
target_link_libraries(styx-handlers PUBLIC
    styx-core
    styx-isa
    styx-util
)
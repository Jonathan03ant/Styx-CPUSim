# Core library - CPU, registers, memory, control unit
# Creates libstyx-core.a

add_library(styx-core OBJECT
    cpu.c
    control.c
    reg_file.c
    mem_storage.c
    mem_loader.c
)

# Core depends on:
# - Public API headers (include/)
# - Utilities (util/errors.h, util/bitops.h)
target_include_directories(styx-core PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}
)

# Link against utilities
target_link_libraries(styx-core PUBLIC
    styx-util
)
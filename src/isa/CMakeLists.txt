# ISA library - instruction decoder and ALU
# Creates libstyx-isa.a

add_library(styx-isa OBJECT
    decoder.c
    alu.c
)

# ISA depends on:
# - Public API headers (include/decoder.h, include/alu.h)
# - Common definitions (include/common/*)
# - Utilities (util/errors.h, util/bitops.h)
target_include_directories(styx-isa PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}
)

# Link against utilities
target_link_libraries(styx-isa PUBLIC
    styx-util
)
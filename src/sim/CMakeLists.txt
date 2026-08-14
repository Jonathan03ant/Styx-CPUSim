# Simulator API facade library
add_library(styx-sim OBJECT
    styx_api.c
)

target_link_libraries(styx-sim
    styx-core
    styx-isa
)
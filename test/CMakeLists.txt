# Test executable - Styx simulator
# Links against all libraries

add_executable(styx_sim styx_sim.c)

# Include paths for headers
target_include_directories(styx_sim PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}
)

# Link all libraries (in dependency order)
# Handlers depends on core/isa, core depends on isa, isa depends on util
target_link_libraries(styx_sim PRIVATE
    styx-handlers
    styx-core
    styx-isa
    styx-util
)
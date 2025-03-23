
set(Name AStarCities2)

cmake_minimum_required(VERSION 3.10)

project(${Name} VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "\
        ${CMAKE_CXX_FLAGS} \
        -Wall \
        -Wextra \
        -pedantic \
        -Wnon-virtual-dtor \
        -Wold-style-cast \
        -Wcast-align \
        -Wcast-qual \
        -Wunused \
        -Woverloaded-virtual \
        -Wpedantic \
        -Wconversion \
        -Wsign-conversion \
        -Wmisleading-indentation \
        -Wduplicated-cond \
        -Wduplicated-branches \
        -Wnull-dereference \
        -Wfloat-equal \
        -Wformat=2 \
        -Wlogical-op \
        -Wmissing-declarations \
        -Wmissing-include-dirs \
        -Wpointer-arith \
        -Wredundant-decls \
        -Wsequence-point \
        -Wswitch \
        -Wundef \
        -Wuseless-cast \
        -Wimplicit-fallthrough \
        -Wunreachable-code \
        -Wunused-but-set-parameter \
        -Wwrite-strings")
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-g -Og")
set(CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG -O3")

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc -static-libstdc++")

configure_file(AStarCities.h.in AStarCities.h)

# List of source files
set(SOURCE_FILES
    src/main.cpp
    src/Map/Map.cpp
    src/Map/Node.cpp
    src/Map/Way.cpp
    src/Map/MapFeatureType.cpp
    src/Map/RoadType.cpp
    src/Map/Road.cpp
    src/Map/BuildingType.cpp
    src/Map/Building.cpp
    src/Map/Intersection.cpp
    src/MapParser/MapParser.cpp
    src/MapParser/NodeParser.cpp
    src/MapParser/WayParser.cpp
    src/MapParser/RelationParser.cpp
    src/MapParser/ParserUtils.cpp
    src/MapParser/RoadParser.cpp
    src/MapParser/BuildingParser.cpp
    src/MapRenderer/LineBuffer.cpp
    src/MapRenderer/Window.cpp
    src/MapRenderer/MapRenderer.cpp
    src/MapRenderer/ShapeRenderer.cpp
    src/MapRenderer/ShapeBuffer.cpp
    src/MapRenderer/NodeRenderer.cpp
    src/MapRenderer/LineRenderer.cpp
    src/MapUtilities/NetworkFinder.cpp
    src/AStar/AStarWindow.cpp
    src/AStar/LoadingWindow.cpp
)

add_executable(${Name} ${SOURCE_FILES})

target_include_directories(${Name} PUBLIC "${PROJECT_BINARY_DIR}")

# SFML
add_subdirectory(libs/SFML SYSTEM)
target_link_libraries(${Name} sfml-system sfml-window sfml-graphics sfml-network sfml-audio)
include_directories(SYSTEM libs/SFML/include)

# nlohmann json parser
include_directories(SYSTEM libs/json/single_include)

# earcut shape generator
include_directories(SYSTEM libs/earcut/include)

# ImGui-SFML
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/libs/ImGui)
set(IMGUI_SFML_FIND_SFML OFF)
set(IMGUI_SFML_IMGUI_DEMO ON)
add_subdirectory(libs/ImGui_SFML SYSTEM)
target_link_libraries(${Name} ImGui-SFML)
include_directories(SYSTEM libs/ImGui)
include_directories(SYSTEM libs/ImGui_SFML)

# resources
add_subdirectory(resources)
add_dependencies(${Name} generateFonts)
target_link_libraries(${Name} ${CMAKE_BINARY_DIR}/resources/fontData.o)

# Custom target to run the application
add_custom_target(test
    COMMAND $<TARGET_FILE:${Name}>
)
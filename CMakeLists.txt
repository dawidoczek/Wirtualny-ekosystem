cmake_minimum_required(VERSION 3.10)
project(Wirtualny-ekosystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

set(SOURCES
    src/GeneratorLosowy.cpp
    src/main.cpp
    src/mieszkaniec.cpp
    src/nisza.cpp
    src/organizm.cpp
    src/osobniki.cpp
    src/rysuj3d.cpp
    src/sasiedztwo.cpp
    src/srodowisko.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -fexceptions)

target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/PDCurses.a)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-static-libgcc -static-libstdc++ -static")
endif()
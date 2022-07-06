cmake_minimum_required(VERSION 3.15)
project(Jumpy LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_GetProperties(SFML)
if(NOT SFML_POPULATED)
  FetchContent_Populate(SFML)
  add_subdirectory(${sfml_SOURCE_DIR} ${sfml_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# Find all source files
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/src/*.hpp)

add_executable(Jumpy ${SOURCE_FILES})
target_link_libraries(Jumpy PRIVATE sfml-graphics)
target_compile_features(Jumpy PRIVATE cxx_std_17)
target_include_directories(Jumpy PRIVATE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>)

install(TARGETS Jumpy DESTINATION .)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/bin/res DESTINATION .)
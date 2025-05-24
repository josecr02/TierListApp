cmake_minimum_required(VERSION 3.14)
project(SceneEditor)

set(CMAKE_CXX_STANDARD 17)

# Enable automoc and autouic for Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt
find_package(Qt5 REQUIRED COMPONENTS Widgets Gui)
# Include headers
include_directories(include)

# Sources
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS include/*.h)
file(COPY ${CMAKE_SOURCE_DIR}/resources DESTINATION ${CMAKE_BINARY_DIR})

add_executable(SceneEditor ${SOURCES} ${HEADERS})

target_link_libraries(SceneEditor PRIVATE Qt5::Widgets Qt5::Gui)
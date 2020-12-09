cmake_minimum_required(VERSION 3.17.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
qt5_add_resources(SOURCES resources/res.qrc)

include_directories(
        ./src
        ../3dparty/Scintilla/Qt4Qt5
)

add_compile_options(
-Wall
-Wextra
-Werror
-Wpedantic
)

add_subdirectory(src)
add_executable(utext main.cpp ${SOURCES})
target_link_libraries(utext src_lib Qt5::Core Qt5::Widgets )

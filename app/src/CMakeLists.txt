set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FileObserver.cpp
        ObserverModel.cpp ObserverModel.h)
set(HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FileObserver.h
        ObserverModel.cpp ObserverModel.h)

set(UI
    ${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.ui
        )

add_library(src_lib STATIC ${SOURCES} ${HEADERS} ${UI})

target_link_libraries(src_lib Qt5::Core Qt5::Widgets)

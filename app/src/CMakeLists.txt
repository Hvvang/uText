set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FileObserver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ObserverModel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FileTab.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FileBrowser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/PopupDialog.cpp
)

set(HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FileObserver.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ObserverModel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FileTab.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FileBrowser.h
    ${CMAKE_CURRENT_SOURCE_DIR}/PopupDialog.h
        )

set(UI
    ${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/popup.ui
)

add_library(src_lib STATIC ${SOURCES} ${HEADERS} ${UI})

target_link_libraries(src_lib Qt5::Core Qt5::Widgets)

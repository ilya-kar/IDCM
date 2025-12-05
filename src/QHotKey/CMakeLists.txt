add_library(QHotKey STATIC
        qhotkey.cpp
        qhotkey.h
        qhotkey_p.h
        qhotkey_win.cpp
)

target_link_libraries(QHotKey PUBLIC
        Qt::Core
        Qt::Gui
)

target_include_directories(QHotKey PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
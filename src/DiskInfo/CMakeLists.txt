add_library(DiskInfo STATIC
        DiskInfo.cpp
        DiskInfo.h
)

target_link_libraries(DiskInfo
        PUBLIC
            Qt::Core
            Qt::Gui
            Qt::Widgets
        PRIVATE
            SpriteWidget
)

target_include_directories(DiskInfo PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
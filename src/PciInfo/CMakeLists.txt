add_library(PciInfo STATIC
        PciInfo.cpp
        PciInfo.h
        SpriteWidget.cpp
        SpriteWidget.h
)

target_link_libraries(PciInfo PUBLIC
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

target_include_directories(PciInfo PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
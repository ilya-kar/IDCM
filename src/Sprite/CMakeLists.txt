add_library(SpriteWidget STATIC
        SpriteWidget.cpp
        SpriteWidget.h
)

target_link_libraries(SpriteWidget PUBLIC
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

target_include_directories(SpriteWidget PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
add_library(CameraInfo STATIC
        CameraInfo.cpp
        CameraInfo.h
)

target_link_libraries(CameraInfo
        PUBLIC
            Qt::Core
            Qt::Gui
            Qt::Widgets
            Qt::Multimedia
            Qt::MultimediaWidgets
        PRIVATE
            QHotKey
)

target_include_directories(CameraInfo PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
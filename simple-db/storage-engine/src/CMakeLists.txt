add_library(storage_engine
    storage/page.cpp
    storage/disk_manager.cpp
)

target_include_directories(storage_engine PUBLIC ${CMAKE_SOURCE_DIR}/src)

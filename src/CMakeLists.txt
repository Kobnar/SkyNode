include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable (SkyNode main.cpp)

target_link_libraries (SkyNode ${OpenCV_LIBS} INI INIReader)

add_subdirectory(lib)
cmake_minimum_required(VERSION 3.20.0)

set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)

project(my_project_name VERSION 0.0.1 LANGUAGES C CXX)


if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
    message("Setting default build type to Release")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_compile_definitions(__MY_DEBUG__)
endif()

# =======================================================
# include directoies recursivly.
MACRO(HEADER_DIRECTORIES return_list)
    FILE(GLOB_RECURSE new_list *.h)
    SET(dir_list "")
    FOREACH(file_path ${new_list})
        GET_FILENAME_COMPONENT(dir_path ${file_path} PATH)
        SET(dir_list ${dir_list} ${dir_path})
    ENDFOREACH()
    LIST(REMOVE_DUPLICATES dir_list)
    SET(${return_list} ${dir_list})
ENDMACRO()

HEADER_DIRECTORIES(header_dir_list)

list(LENGTH header_dir_list header_dir_list_count)
message(STATUS "[INFO] Found ${header_dir_list_count} header directories.")
# ==========================================================

# include_directories(${PROJECT_SOURCE_DIR}/include/)
include_directories(${header_dir_list})


# 设置要遍历的文件夹路径
set(src "./src")

# 使用 file(GLOB ...) 遍历文件夹并保存文件列表
file(GLOB_RECURSE SRC_LIST "${src}/*.cpp")


add_subdirectory(./src)

add_subdirectory(./test)

add_subdirectory(./example)

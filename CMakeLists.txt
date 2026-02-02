cmake_minimum_required(VERSION 3.20)

project(ITK_Demo)

include(cmake/package.cmake)
include(cmake/directory.cmake)
include(cmake/binary_finder.cmake)


# 查找 ITK，并指定需要的模块
find_package(ITK REQUIRED
    COMPONENTS
    ITKSmoothing # 平滑滤波模块
    ITKIOImageBase # I/O 基础模块
    ITKIOPNG # PNG 读写
    ITKIOJPEG # JPEG 读写
    ITKImageFilterBase # 过滤器基类
    ITKCommon # 核心模块
    ITKMathematicalMorphology
    ITKIOGDCM # GDCM 模块
    ITKIODCMTK
)

include(${ITK_USE_FILE})

find_package(OpenCV CONFIG REQUIRED)

find_package(GDCM CONFIG REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_subdirectory(src/ImageViewer)

get_direct_subdirectories(${CMAKE_SOURCE_DIR}/src SUBDIRS)
foreach(SUBDIR ${SUBDIRS})
    get_filename_component(SUBDIR_NAME ${SUBDIR} NAME)
    if(NOT ${SUBDIR_NAME} STREQUAL "ImageViewer" OR ${SUBDIR_NAME} STREQUAL "GDCMReadDemo" )
        get_direct_subdirectories(${SUBDIR} NEXT_SUBDIRS)
        foreach(NEXT_SUBDIR ${NEXT_SUBDIRS})
            get_filename_component(NEXT_SUBDIR_NAME ${NEXT_SUBDIR} NAME)
            file(GLOB ${NEXT_SUBDIR_NAME}_SRCS
                "${NEXT_SUBDIR}/*.cpp"
                "${NEXT_SUBDIR}/*.h"
                "${NEXT_SUBDIR}/*.hpp"
                "${NEXT_SUBDIR}/*.cxx"
            )
            if(${NEXT_SUBDIR_NAME}_SRCS)
                add_executable(${NEXT_SUBDIR_NAME} ${${NEXT_SUBDIR_NAME}_SRCS})
                target_link_libraries(${NEXT_SUBDIR_NAME} PRIVATE ImageCore)

            endif()
        endforeach()
    endif()
endforeach(SUBDIR IN SUBDIRS)

set(BINARIES "")
get_nth_parent_directory(${ITK_DIR} 3 ITK_ROOT)
find_and_add_dlls(${ITK_ROOT} BINARIES)

get_nth_parent_directory(${OpenCV_DIR} 1 OpenCV_ROOT)
find_and_add_dlls(${OpenCV_ROOT} BINARIES "bin" TURE)

get_nth_parent_directory(${GDCM_DIR} 2 GDCM_ROOT)
find_and_add_dlls(${GDCM_ROOT} BINARIES "bin" TURE)

get_nth_parent_directory(${DCMTK_DIR} 1 DCMTK_ROOT)
find_and_add_dlls(${DCMTK_ROOT} BINARIES "bin")

file(COPY ${BINARIES} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE})

#创建文件夹
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/output)

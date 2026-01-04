set(LIB_NAME ImageCore)

file(GLOB SOURCES "*.cpp" "*.h")

add_library(${LIB_NAME} STATIC ${SOURCES})

target_link_libraries(${LIB_NAME} PUBLIC opencv_world ${ITK_LIBRARIES})
target_include_directories(${LIB_NAME} PUBLIC ${ITK_INCLUDE_DIRS} ${CMAKE_CURRENT_LIST_DIR})

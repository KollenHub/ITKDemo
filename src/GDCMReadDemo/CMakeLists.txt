set(EXE_NAME GDCMReader)

file(GLOB SOURCES "*.cpp" "*.h")

add_executable(${EXE_NAME} ${SOURCES})

# 如果是 GDCM 相关示例，额外链接 GDCM 库
# target_link_libraries(${EXE_NAME} PRIVATE
  
# )

target_include_directories(${EXE_NAME} PRIVATE ${GDCM_INCLUDE_DIRS})


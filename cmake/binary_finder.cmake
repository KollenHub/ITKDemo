# 查找并添加 DLL 文件到 BINARIES 列表
#
# 用法:
#   find_and_add_dlls(<LIB_ROOT> [BIN_SUBDIR] [OUTPUT_VAR])
#
# 参数:
#   LIB_ROOT    - 库的根目录 (如 SPDLOG_ROOT)
#   BIN_SUBDIR  - bin 子目录名称，默认为 "bin"
#   OUTPUT_VAR  - 输出变量名，默认为 "BINARIES"
#
# 示例:
#   find_and_add_dlls(${SPDLOG_ROOT} BINARIES "bin")
#   find_and_add_dlls(${SPDLOG_ROOT})              # 使用默认值
#   find_and_add_dlls(${SPDLOG_ROOT} BINARIES "bin" TRUE)        # 输出到默认 BINARIES
#
function(find_and_add_dlls LIB_ROOT)
    # 解析参数
    set(BIN_SUBDIR "bin")
    set(OUTPUT_VAR "BINARIES")
    set(ALL_DLL FALSE)


    if(ARGC GREATER_EQUAL 2)
        set(OUTPUT_VAR ${ARGV1})

    endif()

    if(ARGC GREATER_EQUAL 3)
        set(BIN_SUBDIR ${ARGV2})
    endif()

    if(ARGC GREATER_EQUAL 4)
        set(ALL_DLL ${ARGV3})
    endif()

    set(BIN_DIR ${LIB_ROOT}/${BIN_SUBDIR})

    if(ALL_DLL)
        file(GLOB DLL_FILES "${BIN_DIR}/*.dll")
    elseif(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
        file(GLOB DLL_FILES "${BIN_DIR}/*d.dll")
        file(GLOB PDB_FILES "${BIN_DIR}/*.pdb")
    else()
        # Release 模式：排除以 'd' 结尾的 debug dll
        file(GLOB DLL_FILES "${BIN_DIR}/*[^d].dll")
    endif()

    # 将找到的 DLL 添加到输出变量
    list(APPEND ${OUTPUT_VAR} ${DLL_FILES})
    list(APPEND ${OUTPUT_VAR} ${PDB_FILES})

    # 将结果返回给调用者
    set(${OUTPUT_VAR} ${${OUTPUT_VAR}} PARENT_SCOPE)
endfunction()

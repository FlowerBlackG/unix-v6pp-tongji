#[[
    启动引导构建文件
    创建于 2024年4月28日 上海市嘉定区安亭镇

    by 2051565 GTY
]]


set(BOOT_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/../target/objs/boot")

# 使用 Ninja 时，输出路径会出现问题。
set(BOOT_OUTPUT_DIR_NINJA "${CMAKE_SOURCE_DIR}/../target/objs")

file(MAKE_DIRECTORY ${BOOT_OUTPUT_DIR})
set(CMAKE_ASM_NASM_LINK_EXECUTABLE "cp <OBJECTS> ${BOOT_OUTPUT_DIR_NINJA}/<TARGET>.bin")
set(CMAKE_ASM_NASM_OBJECT_FORMAT bin)
if (USE_VESA)
    set(CMAKE_ASM_NASM_FLAGS "${CMAKE_ASM_NASM_FLAGS} -d USE_VESA")
endif()
enable_language(ASM_NASM)
add_executable(boot boot.asm)


# sector2 要作为整个内核代码的入口。需要单独构建。
# 链接时，它会被指定放到开头位置。
add_custom_command(
    OUTPUT ${BOOT_OUTPUT_DIR}/sector2.o
    COMMAND nasm -f elf32 ${CMAKE_CURRENT_SOURCE_DIR}/sector2.asm -o ${BOOT_OUTPUT_DIR}/sector2.o
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/sector2.asm
)
set(NULL_CPP_FILE_PATH "${CMAKE_BINARY_DIR}/_yros_cmake_hack/null.cpp")
file(WRITE ${NULL_CPP_FILE_PATH} "")
add_library(sector2 ${BOOT_OUTPUT_DIR}/sector2.o ${NULL_CPP_FILE_PATH})
set_target_properties(sector2 PROPERTIES PREFIX "")
set_target_properties(sector2 PROPERTIES SUFFIX ".o")

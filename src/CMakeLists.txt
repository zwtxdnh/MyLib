
# 并将名称保存到 DIR_LIB_SRCS 变量
include_directories(${PROJECT_SOURCE_DIR}/include)
aux_source_directory(. DIR_LIB_SRCS)
# 生成链接库
add_library (mylib ${DIR_LIB_SRCS})
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/./lib)           #设置库文件的输出目录

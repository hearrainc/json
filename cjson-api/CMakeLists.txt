set(TARGET cjson)


#业务依赖的头文件
include_directories(
                ./include
                )
                
#业务源文件
aux_source_directory(./source/ SRC_FILES)


#生成业务lib库
add_library(${TARGET} STATIC ${SRC_FILES})

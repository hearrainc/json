set(TARGET com_json)

#业务依赖的头文件
include_directories(
                ../cjson-api/include
                )


#业务源文件
aux_source_directory(./source SRC_FILES)

#生成业务lib库
if(LINT)
    #add_lint(${TARGET} ${SRC_FILES})
else ()
    add_library(${TARGET}_OBJ OBJECT ${SRC_FILES})
    add_library(${TARGET} STATIC $<TARGET_OBJECTS:${TARGET}_OBJ>)
endif()


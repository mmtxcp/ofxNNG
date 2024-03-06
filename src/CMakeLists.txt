#KQServerHandler
PROJECT(ofxNNG)
MESSAGE(STATUS "Project ${PROJECT_NAME} ...")
# Directories
#set(SDK_BASE_DIR "${PROJECT_SOURCE_DIR}")
#set(SDK_GEOMETRY_DIR "${SDK_BASE_DIR}")
set(CMAKE_CXX_STANDARD 11)
#添加宏
ADD_DEFINITIONS(
    -DDISABLE_LOG
    -DNOMINMAX
)

IF(NOT MSVC)
   set(CMAKE_TOOLCHAIN_FILE $ENV{VCPKG_ROOT})
   MESSAGE(STATUS "CMAKE_TOOLCHAIN_FILE=" ${CMAKE_TOOLCHAIN_FILE})
   SET(THIRD_DIR $ENV{THIRD_DIR})
   SET(THIRD_INC $ENV{THIRD_INC})
   SET(THIRD_LIB $ENV{THIRD_LIB})	
ELSE()
   SET(THIRD_DIR ${CMAKE_SOURCE_DIR}/libs)
   SET(THIRD_INC ${THIRD_DIR}/nng/include)
   SET(THIRD_LIB ${THIRD_DIR}/nng/lib/vs/${CMAKE_GENERATOR_PLATFORM}/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>$<$<CONFIG:RelWithDebInfo>:RelWithDebInfo>)
ENDIF()
MESSAGE(STATUS "THIRD_DIR=${THIRD_DIR}")
MESSAGE(STATUS "THIRD_INC=${THIRD_INC}")
MESSAGE(STATUS "THIRD_LIB=${THIRD_LIB}")

FILE(GLOB_RECURSE HEADER_FILES ${PROJECT_SOURCE_DIR}/*.h ${PROJECT_SOURCE_DIR}/*.hpp)
FILE(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/*.c ${PROJECT_SOURCE_DIR}/*.cpp ${PROJECT_SOURCE_DIR}/*.cc)
#SOURCE_GROUP("Header Files" FILES ${HEADER_FILES})
#SOURCE_GROUP("Source Files" FILES ${SRC_FILES})
#source_group(TREE "${PROJECT_SOURCE_DIR}" FILES ${SRC_FILES})

#SOURCE_GROUP("Base Files" FILES ${INC_FILES})

#head file path
INCLUDE_DIRECTORIES(
        #${INC_BASE_DIR}
        #${PROJECT_SOURCE_DIR}
        ${THIRD_INC}
        #${THIRD_INC}/protocol
		#${INC_BASE_DIR}/detail
		#${INC_BASE_DIR}/message
		#${INC_BASE_DIR}/socket
		#${INC_BASE_DIR}/util
		#${INC_BASE_DIR}/ofx/utils
		#${INC_BASE_DIR}/ofx/events
)
function(INCLUDE_AND_GROUP_FILES SOURCE_DIR)
    include_directories(${SOURCE_DIR})
    file(GLOB_RECURSE source_files "${SOURCE_DIR}/*.c*" "${SOURCE_DIR}/*.h*")
    foreach(source_file ${source_files})
        file(RELATIVE_PATH relative_path ${SOURCE_DIR} ${source_file})
        get_filename_component(group_name ${relative_path} DIRECTORY)
        if(NOT "${group_name}" STREQUAL "")
            string(REPLACE "/" "\\" group_name ${group_name})
            source_group(${group_name} FILES ${source_file})
        endif()     
    endforeach()
endfunction()
# 调用示例
INCLUDE_AND_GROUP_FILES("${PROJECT_SOURCE_DIR}")
# SDK files
# file(GLOB SDK_GEOMETRY_FILES
#         "${PROJECT_SOURCE_DIR}/*.cpp"
#         "${PROJECT_SOURCE_DIR}/*.h"
#         )

#库搜索目录
LINK_DIRECTORIES(
        ${THIRD_LIB}
)
#设置为控制台应用程序
IF(MSVC)
    SET(CMAKE_EXE_LINKER_FLAGS         "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:CONSOLE")
     SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS} /DEBUG")
ENDIF()
#source directory
#aux_source_directory(. SRC_LIST) # 搜索当前目录下的所有.cpp文件
#生成动态库
#ADD_LIBRARY (${PROJECT_NAME} SHARED "")
#SET(OUTPUT_FILE_TYPE "LIBRARY")
#生成可执行文件
#ADD_EXECUTABLE (${PROJECT_NAME})
#SET(OUTPUT_FILE_TYPE "RUNTIME")
# 生成静态库
add_library(${PROJECT_NAME} STATIC "")
# 设置输出文件类型为静态库
SET(OUTPUT_FILE_TYPE "ARCHIVE")

TARGET_SOURCES(${PROJECT_NAME}
    PRIVATE
    ${SRC_FILES}
    ${HEADER_FILES}
    ${INC_FILES}
	${INC_BASE_DIR}
)
#find_package(ZLIB REQUIRED)

#find_package(unofficial-brotli CONFIG REQUIRED)
#find_package(RocksDB CONFIG REQUIRED)
#find_package(ghc_filesystem CONFIG REQUIRED)

#添加依赖项
if(CMAKE_HOST_UNIX)
    target_link_libraries(${PROJECT_NAME}
        PRIVATE
        nng
       
        pthread
    )
else()
    target_link_libraries(${PROJECT_NAME}
        PRIVATE
		nng
        shlwapi
        rpcrt4
        ws2_32
        advapi32
        iphlpapi
        Wldap32
    )     
endif(CMAKE_HOST_UNIX)
#指定插件后缀
#set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ${SHARED_LIBRARY_SUFFIX})


MESSAGE(STATUS "PROJECT_SOURCE_DIR= ${PROJECT_SOURCE_DIR} ")

function(COPY_FILES_WITHOUT_MATCHING source_dir dest_dir patterns)
    file(GLOB_RECURSE files "${source_dir}/*")
    foreach(file ${files})
        get_filename_component(relative_path ${file} PATH)
        file(RELATIVE_PATH relative_path ${source_dir} ${relative_path})
        get_filename_component(file_name ${file} NAME)
        foreach(pattern ${patterns})
            string(REGEX MATCH ${pattern} match_result ${file_name})
            if(NOT match_result)
                file(COPY ${file} DESTINATION "${dest_dir}/${relative_path}")
            endif()
        endforeach()
    endforeach()
endfunction()

set(ignore_suffixes ".cpp" ".c")
COPY_FILES_WITHOUT_MATCHING("${INC_BASE_DIR}" "${EXPORT_INCLUDE}" ${ignore_suffixes})
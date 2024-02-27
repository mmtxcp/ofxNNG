#KQServerHandler
PROJECT(Demo)
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
SOURCE_GROUP("Header Files" FILES ${HEADER_FILES})
SOURCE_GROUP("Source Files" FILES ${SRC_FILES})
#source_group(TREE "${PROJECT_SOURCE_DIR}" FILES ${SRC_FILES})


FILE(GLOB_RECURSE OFX_FILES ${INC_BASE_DIR}/ofx/utils/*.c* ${INC_BASE_DIR}/ofx/utils/*.h* 
    ${INC_BASE_DIR}/ofx/events/*.c* ${INC_BASE_DIR}/ofx/events/*.h*)
SOURCE_GROUP("OFX Files" FILES ${OFX_FILES})
SOURCE_GROUP("Base Files" FILES ${INC_FILES})

#head file path
INCLUDE_DIRECTORIES(
        ${INC_BASE_DIR}
        ${PROJECT_SOURCE_DIR}
        ${THIRD_INC}
        ${THIRD_INC}/protocol
		${INC_BASE_DIR}/detail
		${INC_BASE_DIR}/message
		${INC_BASE_DIR}/socket
		${INC_BASE_DIR}/util
		${INC_BASE_DIR}/ofx/utils
		${INC_BASE_DIR}/ofx/events
)
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
ADD_EXECUTABLE (${PROJECT_NAME})
SET(OUTPUT_FILE_TYPE "RUNTIME")

TARGET_SOURCES(${PROJECT_NAME}
    PRIVATE
    ${SRC_FILES}
    ${HEADER_FILES}
    ${INC_FILES}
	${INC_BASE_DIR}
    ${OFX_FILES}
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
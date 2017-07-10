cmake_minimum_required(VERSION 2.8.3)
project(easy_profiler)

find_package(catkin REQUIRED)

add_definitions( -std=c++11 )


message("------------------------------------------")
message("easy_profiler is being built using CATKIN.")
message("------------------------------------------")

catkin_package(
    INCLUDE_DIRS include
    LIBRARIES easy_profiler
    DEPENDS pthread
)


if(NOT DEFINED EASY_PROGRAM_VERSION_MAJOR)
    file (STRINGS ${CMAKE_CURRENT_LIST_DIR}/version.info EASY_PRODUCT_VERSION_STRING)
    string(REPLACE "." ";" VERSION_LIST ${EASY_PRODUCT_VERSION_STRING})

    list(GET VERSION_LIST 0 EASY_PROGRAM_VERSION_MAJOR)
    list(GET VERSION_LIST 1 EASY_PROGRAM_VERSION_MINOR)
    list(GET VERSION_LIST 2 EASY_PROGRAM_VERSION_PATCH)

    add_definitions(
        -DEASY_PROFILER_VERSION_MAJOR=${EASY_PROGRAM_VERSION_MAJOR}
        -DEASY_PROFILER_VERSION_MINOR=${EASY_PROGRAM_VERSION_MINOR}
        -DEASY_PROFILER_VERSION_PATCH=${EASY_PROGRAM_VERSION_PATCH}
    )
    set(EASY_PROGRAM_VERSION_MAJOR ${EASY_PROGRAM_VERSION_MAJOR} PARENT_SCOPE)
    set(EASY_PROGRAM_VERSION_MINOR ${EASY_PROGRAM_VERSION_MINOR} PARENT_SCOPE)
    set(EASY_PROGRAM_VERSION_PATCH ${EASY_PROGRAM_VERSION_PATCH} PARENT_SCOPE)

endif(NOT DEFINED EASY_PROGRAM_VERSION_MAJOR)

# EasyProfiler options:----------------------------------------------
set(EASY_DEFAULT_PORT 28077) # Default listening port
set(EASY_OPTION_LISTEN ON) # Enable automatic startListen on startup
set(EASY_OPTION_PROFILE_SELF OFF) # Enable self profiling (measure time for internal storage expand)
set(EASY_OPTION_PROFILE_SELF_BLOCKS_ON OFF) # Storage expand default status (profiler::ON or profiler::OFF)
set(EASY_OPTION_LOG OFF) # Print errors to stderr
set(EASY_OPTION_PREDEFINED_COLORS ON) # Use predefined set of colors (see profiler_colors.h)
                                      # If you want to use your own colors palette you can turn this option OFF

MESSAGE(STATUS "EASY_PROFILER OPTIONS:--------------")
MESSAGE(STATUS "  Default listening port = ${EASY_DEFAULT_PORT}")
MESSAGE(STATUS "  Auto-start listening = ${EASY_OPTION_LISTEN}")
MESSAGE(STATUS "  Profile self = ${EASY_OPTION_PROFILE_SELF}")
MESSAGE(STATUS "  Profile self blocks initial status = ${EASY_OPTION_PROFILE_SELF_BLOCKS_ON}")
MESSAGE(STATUS "  Log messages = ${EASY_OPTION_LOG}")
MESSAGE(STATUS "  Use EasyProfiler colors palette = ${EASY_OPTION_PREDEFINED_COLORS}")
MESSAGE(STATUS "END EASY_PROFILER OPTIONS.----------")
MESSAGE(STATUS "")
# END EasyProfiler options.~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

add_definitions(-DEASY_DEFAULT_PORT=${EASY_DEFAULT_PORT})
if(EASY_OPTION_LISTEN)
 add_definitions(-DEASY_OPTION_START_LISTEN_ON_STARTUP=1)
else()
 add_definitions(-DEASY_OPTION_START_LISTEN_ON_STARTUP=0)
endif(EASY_OPTION_LISTEN)

if(EASY_OPTION_PROFILE_SELF)
 add_definitions(-DEASY_OPTION_MEASURE_STORAGE_EXPAND=1)
 if(EASY_OPTION_PROFILE_SELF_BLOCKS_ON)
  add_definitions(-DEASY_OPTION_STORAGE_EXPAND_BLOCKS_ON=true)
 else()
  add_definitions(-DEASY_OPTION_STORAGE_EXPAND_BLOCKS_ON=false)
 endif(EASY_OPTION_PROFILE_SELF_BLOCKS_ON)
else()
 add_definitions(-DEASY_OPTION_MEASURE_STORAGE_EXPAND=0)
endif(EASY_OPTION_PROFILE_SELF)

if(EASY_OPTION_LOG)
 add_definitions(-DEASY_OPTION_LOG_ENABLED=1)
else()
 add_definitions(-DEASY_OPTION_LOG_ENABLED=0)
endif(EASY_OPTION_LOG)

if(EASY_OPTION_PREDEFINED_COLORS)
 add_definitions(-DEASY_OPTION_BUILTIN_COLORS=1)
else()
 add_definitions(-DEASY_OPTION_BUILTIN_COLORS=0)
endif(EASY_OPTION_PREDEFINED_COLORS)

set(CPP_FILES
    src/block.cpp
    src/profile_manager.cpp
    src/reader.cpp
    src/easy_socket.cpp
)

set(H_FILES
    src/profile_manager.h
    src/spin_lock.h
    src/current_time.h
)

set(INCLUDE_FILES
  include/easy_profiler/profiler.h
  include/easy_profiler/reader.h
  include/easy_profiler/easy_net.h
  include/easy_profiler/easy_socket.h
  include/easy_profiler/easy_compiler_support.h
  include/easy_profiler/profiler_aux.h
  include/easy_profiler/profiler_colors.h
  include/easy_profiler/reader.h
  include/easy_profiler/serialized_block.h
)
source_group(include FILES ${INCLUDE_FILES})

set(SOURCES
    ${CPP_FILES}
    ${H_FILES}
    ${INCLUDE_FILES}
)

###########
## Build ##
###########


include_directories(
 include
 ${catkin_INCLUDE_DIRS}
)

add_library(${PROJECT_NAME} STATIC ${SOURCES} resources.rc)
target_link_libraries(${PROJECT_NAME} pthread )


#############
## Install ##
#############


install(TARGETS ${PROJECT_NAME}
    ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
    LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
    RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
    )

## Mark cpp header files for installation
install(DIRECTORY include/${PROJECT_NAME}/
    DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
    FILES_MATCHING PATTERN "*.h"
    PATTERN ".svn" EXCLUDE
    )




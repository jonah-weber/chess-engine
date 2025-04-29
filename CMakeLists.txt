cmake_minimum_required(VERSION 4.0.1)

project(chess_engine)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "src/*.h")

set(ENGINE_NAME "chess-engine")
set(ENGINE_NAME_STR "\"${ENGINE_NAME}\"")
set(ENGINE_AUTHOR_STR "\"jonah-weber\"")
set(LOG_FILE_NAME "${ENGINE_NAME}_debug_log.txt")

if(APPLE)
    cmake_path(SET LOG_FILE_DIR "/tmp")
endif()
cmake_path(SET LOG_FILE "${LOG_FILE_DIR}/${LOG_FILE_NAME}")
cmake_path(NATIVE_PATH LOG_FILE LOG_FILE_PATH_NATIVE)
set(LOG_FILE_PATH_STR "\"${LOG_FILE_PATH_NATIVE}\"")

if(APPLE)
    set(START_DEBUG_MONITOR_CMD_STR "\"osascript -e 'tell app \\\"Terminal\\\" to do script \\\"tail -f ${LOG_FILE_PATH_NATIVE}\\\"'\"")
    set(KILL_DEBUG_MONITOR_CMD_STR "\"pkill -f 'tail -f ${LOG_FILE_PATH_NATIVE}'\"")
endif()

configure_file("src/config.h.in" "include/config.h")

add_executable(chess_engine ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE
	"${CMAKE_CURRENT_BINARY_DIR}/include")

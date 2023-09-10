cmake_minimum_required(VERSION 3.6.0)
project(cli)


add_definitions(-DHOSTS="${CMAKE_SYSTEM_NAME}")
message("${CMAKE_SYSTEM_NAME}")
include_directories(
    ${PROJECT_SOURCE_DIR}/cliba/include
    ${PROJECT_SOURCE_DIR}/cliba/src
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/src
)
file(GLOB SRCS
	"${PROJECT_SOURCE_DIR}/include/*"
	"${PROJECT_SOURCE_DIR}/src/*"
	"${PROJECT_SOURCE_DIR}/cliba/include/*"
    "${PROJECT_SOURCE_DIR}/cliba/src/*"
)
add_executable(cli ${SRCS})

find_package(Curses REQUIRED)
target_link_libraries(cli ${CURSES_LIBRARIES})

find_package(Boost REQUIRED)

target_link_libraries(cli ${Boost_LIBRARIES})
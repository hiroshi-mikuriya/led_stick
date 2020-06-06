cmake_minimum_required(VERSION 3.6)

project(show_pics CXX)

find_package(Threads REQUIRED)
find_package(OpenCV REQUIRED)

file(GLOB SOURCES
  *.cpp
)

add_executable(${PROJECT_NAME}
  ${SOURCES}
)

target_link_libraries(${PROJECT_NAME}
  PRIVATE
    ${CMAKE_THREAD_LIBS_INIT}
    ${OpenCV_LIBS}
    stick
)

target_include_directories(${PROJECT_NAME}
  PRIVATE
    ${OpenCV_INCLUDE_DIRS}
)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
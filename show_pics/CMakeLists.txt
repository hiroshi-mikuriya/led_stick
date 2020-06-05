cmake_minimum_required(VERSION 3.6)

project(show_pics CXX)

find_package(Threads REQUIRED)

file(GLOB SOURCES
  *.cpp
)

add_executable(${PROJECT_NAME}
  ${SOURCES}
)

target_link_libraries(${PROJECT_NAME}
  PRIVATE
    ${CMAKE_THREAD_LIBS_INIT}
)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
# CMakeList.txt: C_DataStructures에 대한 CMake 프로젝트, 여기에 소스를 포함하고
# 프로젝트 특정 논리를 정의합니다.
#
cmake_minimum_required (VERSION 3.8)

# 지원되는 경우 MSVC 컴파일러에 대해 핫 다시 로드 사용하도록 설정합니다.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("C_DataStructures")

# 이 프로젝트의 실행 파일에 소스를 추가합니다.

# src/c 디렉터리의 모든 .c 파일
file(GLOB C_FILES "${CMAKE_SOURCE_DIR}/src/c/*.c")

# tests/cpp 디렉터리의 모든 .cpp 파일
file(GLOB CPP_FILES
        "${CMAKE_SOURCE_DIR}/tests/cpp/01_linkedlist/*.cpp"
        "${CMAKE_SOURCE_DIR}/tests/cpp/02_stack/*.cpp"
        "${CMAKE_SOURCE_DIR}/tests/cpp/03_queue/*.cpp"
        "${CMAKE_SOURCE_DIR}/tests/cpp/04_tree/*.cpp"
)

# main.cpp (사용예제) 디렉터리
file(GLOB EXAMPLE_FILES "${CMAKE_SOURCE_DIR}/examples/*.cpp")

############################################################

# 헤더 파일이 들어있는 디렉터리 추가 (인터페이스 헤더들)
include_directories("${CMAKE_SOURCE_DIR}/include/mylib/interfaces")

# 헤더 파일이 들어있는 디렉터리 추가 (템플릿 구현 포함 헤더들)
include_directories(
        "${CMAKE_SOURCE_DIR}/src/hpp"
        "${CMAKE_SOURCE_DIR}/src/hpp/01_linked_list"
        "${CMAKE_SOURCE_DIR}/src/hpp/02_stack"
        "${CMAKE_SOURCE_DIR}/src/hpp/03_queue"
        "${CMAKE_SOURCE_DIR}/src/hpp/04_tree"
)

# 실행 파일에 소스 추가
add_executable(C_DataStructures ${C_FILES} ${CPP_FILES} ${EXAMPLE_FILES} "C_DataStructures.cpp" "C_DataStructures.h"
        include/mylib/interfaces/i_linked_list.hpp
        src/hpp/01_linked_list/single_linked_list.hpp
        src/hpp/01_linked_list/double_linked_list.hpp
        src/hpp/01_linked_list/circular_linked_list.hpp
        tests/cpp/01_linked_list/single_linked_list_test.cpp
        tests/cpp/01_linked_list/double_linked_list_test.cpp
        tests/cpp/01_linked_list/circular_linked_list.cpp
)
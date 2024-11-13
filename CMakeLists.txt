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
add_executable (C_DataStructures "C_DataStructures.cpp" "C_DataStructures.h"                 "01_single_linkedlist.c" "02_doubly_linkedlist.c" "03_circular_linkedlist.c" "04_array_stack.c" "05_linkedlist_stack.c" "06_circular_queue.c" "07_linked_queue.c" "08_binary_tree.c" "09_binary_search_tree.c" "10_expression_tree.c" "11_disjoint_set.c" "12_bubble_sort.c" "13_insertion_sort.c" "14_selection_sort.c")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET C_DataStructures PROPERTY CXX_STANDARD 20)
endif()

# TODO: 필요한 경우 테스트를 추가하고 대상을 설치합니다.

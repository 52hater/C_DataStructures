/* main.cpp는 보통:

각 자료구조별 간단한 사용 예제
실제 문제 해결을 위한 자료구조 활용 예시
성능 비교 데모
알고리즘 구현에서 자료구조 활용

이런 내용들이 포함됨. */

#include <iostream>
#include <chrono>
#include "single_linked_list.hpp"
#include "double_linked_list.hpp"
#include "circular_linked_list.hpp"

// 성능 측정을 위한 타이머 클래스
class Timer {
private:
    std::chrono::high_resolution_clock::time_point start_time;

public:
    Timer() : start_time(std::chrono::high_resolution_clock::now()) {}

    double elapsed() {
        auto end_time = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double, std::milli>(end_time
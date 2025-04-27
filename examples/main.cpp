/* main.cpp는 보통:

각 자료구조별 간단한 사용 예제
실제 문제 해결을 위한 자료구조 활용 예시
성능 비교 데모
알고리즘 구현에서 자료구조 활용

이런 내용들이 포함됨.


성능 측정 도구

Timer 클래스를 사용한 연산 시간 측정
각 리스트 타입별 성능 비교


실제 활용 예제

다항식 계산: 단일 연결 리스트 활용
명령어 히스토리: 원형 연결 리스트 활용
양방향 데이터 탐색: 이중 연결 리스트 활용


주요 특징

템플릿을 활용한 일반화된 성능 테스트
각 리스트 타입의 장점을 살린 실제 사용 사례

*/

#include <iostream>
#include <chrono>
#include <vector>
#include "single_linked_list.hpp"
#include "double_linked_list.hpp"
#include "circular_linked_list.hpp"

/**
 * @brief 성능 측정을 위한 타이머 클래스
 * 각 자료구조의 연산 시간을 측정하는데 사용
 */
class Timer {
private:
    std::chrono::high_resolution_clock::time_point start_time;

public:
    Timer() : start_time(std::chrono::high_resolution_clock::now()) {}

    // 경과 시간을 밀리초 단위로 반환
    double elapsed() {
        auto end_time = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double, std::milli>(end_time - start_time).count();
    }
};

/**
 * @brief 리스트 성능 테스트 함수
 * 각각의 리스트에 대해 동일한 작업을 수행하고 시간을 측정
 */
template<typename List>
void performanceTest(const std::string& list_name) {
    std::cout << "\n=== " << list_name << " 성능 테스트 ===\n";
    List list;
    Timer timer;

    // 앞쪽 삽입 테스트
    for (int i = 0; i < 10000; ++i) {
        list.push_front(i);
    }
    std::cout << "10000개 앞쪽 삽입 시간: " << timer.elapsed() << "ms\n";

    // 뒤쪽 삽입 테스트
    timer = Timer();
    for (int i = 0; i < 10000; ++i) {
        list.push_back(i);
    }
    std::cout << "10000개 뒤쪽 삽입 시간: " << timer.elapsed() << "ms\n";
}

/**
 * @brief 실제 활용 예시 - 다항식 계산
 * x^2 + 2x + 1 과 같은 다항식을 리스트로 표현하고 계산
 */
void polynomialExample() {
    std::cout << "\n=== 다항식 계산 예제 ===\n";
    SingleLinkedList<int> coefficients;  // 계수를 저장하는 리스트

    // 다항식 x^2 + 2x + 1 의 계수를 저장 (상수항부터)
    coefficients.push_back(1);  // 상수항
    coefficients.push_back(2);  // x의 계수
    coefficients.push_back(1);  // x^2의 계수

    // x = 2일 때의 값 계산
    int x = 2;
    int result = 0;
    int power = 1;

    for (size_t i = 0; i < coefficients.size(); ++i) {
        result += coefficients.at(i) * power;
        power *= x;
    }

    std::cout << "x = 2일 때, x^2 + 2x + 1 = " << result << "\n";
}

/**
 * @brief 실제 활용 예시 - 히스토리 관리
 * 원형 연결 리스트를 사용한 명령어 히스토리 관리
 */
void historyExample() {
    std::cout << "\n=== 명령어 히스토리 관리 예제 ===\n";
    CircularLinkedList<std::string> history;

    // 최근 5개의 명령어만 저장
    const size_t MAX_HISTORY = 5;

    // 명령어 추가
    std::vector<std::string> commands = {
        "ls", "cd ..", "mkdir test", "touch file.txt",
        "vim file.txt", "gcc main.c", "./a.out"
    };

    for (const auto& cmd : commands) {
        history.push_back(cmd);
        if (history.size() > MAX_HISTORY) {
            history.pop_front();
        }
        std::cout << "명령어 실행: " << cmd << "\n";
    }

    // 최근 명령어 출력
    std::cout << "\n최근 " << MAX_HISTORY << "개의 명령어:\n";
    history.print();
}

/**
 * @brief 실제 활용 예시 - 양방향 탐색
 * 이중 연결 리스트를 사용한 양방향 데이터 탐색
 */
void bidirectionalSearchExample() {
    std::cout << "\n=== 양방향 탐색 예제 ===\n";
    DoubleLinkedList<int> numbers;

    // 1부터 10까지의 숫자 저장
    for (int i = 1; i <= 10; ++i) {
        numbers.push_back(i);
    }

    // 앞에서부터 5보다 큰 첫 번째 짝수 찾기
    for (size_t i = 0; i < numbers.size(); ++i) {
        int num = numbers.at(i);
        if (num > 5 && num % 2 == 0) {
            std::cout << "5보다 큰 첫 번째 짝수: " << num << "\n";
            break;
        }
    }
}

int main() {
    // 성능 테스트
    performanceTest<SingleLinkedList<int>>("단일 연결 리스트");
    performanceTest<DoubleLinkedList<int>>("이중 연결 리스트");
    performanceTest<CircularLinkedList<int>>("원형 연결 리스트");

    // 실제 활용 예제
    polynomialExample();        // 다항식 계산 예제
    historyExample();          // 히스토리 관리 예제
    bidirectionalSearchExample(); // 양방향 탐색 예제

    return 0;
}
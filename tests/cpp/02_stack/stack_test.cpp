#include <iostream>
#include <limits>
#include "array_stack.hpp"
#include "linked_stack.hpp"

// 사용자 메뉴 선택을 위한 열거형
enum class MenuOption {
    Push = 1,        // 스택에 데이터 추가
    Pop,            // 스택에서 데이터 제거
    Peek,           // 최상단 데이터 확인
    Print,          // 스택 전체 출력
    Size,           // 스택 크기 확인
    Capacity,       // 스택 용량 확인
    Clear,          // 스택 초기화
    Empty,          // 스택 공백 상태 확인
    ChangeImpl,     // 스택 구현 방식 변경
    Exit            // 프로그램 종료
};

// 스택 구현 방식을 위한 열거형
enum class StackType {
    Array,          // 배열 기반 스택
    Linked          // 연결 리스트 기반 스택
};

// 입력 버퍼 비우기 함수
void clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// 메뉴 출력 함수
void printMenu(StackType type) {
    std::cout << "\n=== Stack Test Menu (" 
              << (type == StackType::Array ? "Array-based" : "Linked-list-based")
              << ") ===\n"
              << "1. Push        - 스택에 데이터 추가\n"
              << "2. Pop         - 스택에서 데이터 제거\n"
              << "3. Peek        - 최상단 데이터 확인\n"
              << "4. Print       - 스택 전체 출력\n"
              << "5. Size        - 스택 크기 확인\n"
              << "6. Capacity    - 스택 용량 확인\n"
              << "7. Clear       - 스택 초기화\n"
              << "8. Empty       - 스택 공백 상태 확인\n"
              << "9. Change Impl - 스택 구현 방식 변경\n"
              << "0. Exit        - 프로그램 종료\n"
              << "메뉴 선택: ";
}

// 테스트 실행 함수 템플릿
template<typename Stack>
void runStackTest(Stack& stack, StackType type) {
    int choice;
    int value;

    do {
        printMenu(type);
        std::cin >> choice;

        try {
            switch (static_cast<MenuOption>(choice)) {
                case MenuOption::Push:
                    std::cout << "추가할 값 입력: ";
                    std::cin >> value;
                    stack.push(value);
                    std::cout << "값 " << value << " 이(가) 스택에 추가됨\n";
                    break;

                case MenuOption::Pop:
                    stack.pop();
                    std::cout << "스택 최상단 요소가 제거됨\n";
                    break;

                case MenuOption::Peek:
                    value = stack.peek();
                    std::cout << "스택 최상단 값: " << value << "\n";
                    break;

                case MenuOption::Print:
                    stack.print();
                    break;

                case MenuOption::Size:
                    std::cout << "스택 크기: " << stack.size() << "\n";
                    break;

                case MenuOption::Capacity:
                    std::cout << "스택 용량: ";
                    if (stack.capacity() == size_t(-1)) {
                        std::cout << "무제한\n";
                    } else {
                        std::cout << stack.capacity() << "\n";
                    }
                    break;

                case MenuOption::Clear:
                    stack.clear();
                    std::cout << "스택이 초기화됨\n";
                    break;

                case MenuOption::Empty:
                    std::cout << "스택 상태: " 
                             << (stack.empty() ? "비어있음" : "비어있지 않음") 
                             << "\n";
                    break;

                case MenuOption::ChangeImpl:
                    return;  // 현재 테스트 종료

                case MenuOption::Exit:
                    std::cout << "프로그램을 종료합니다.\n";
                    exit(0);

                default:
                    std::cout << "잘못된 메뉴 선택입니다.\n";
            }
        }
        catch (const std::exception& e) {
            std::cout << "오류 발생: " << e.what() << "\n";
        }

        clearInputBuffer();

    } while (true);
}

int main() {
    ArrayStack<int> array_stack;
    LinkedStack<int> linked_stack;
    StackType current_type = StackType::Array;

    std::cout << "스택 테스트 프로그램을 시작합니다.\n";

    while (true) {
        if (current_type == StackType::Array) {
            runStackTest(array_stack, current_type);
            current_type = StackType::Linked;
        } else {
            runStackTest(linked_stack, current_type);
            current_type = StackType::Array;
        }

        std::cout << "\n구현 방식이 ";
        if (current_type == StackType::Array) {
            std::cout << "배열 기반";
        } else {
            std::cout << "연결 리스트 기반";
        }
        std::cout << "으로 변경되었습니다.\n";
    }

    return 0;
}
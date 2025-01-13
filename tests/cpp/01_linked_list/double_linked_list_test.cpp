#include <iostream>
#include <limits>
#include "double_linked_list.hpp"

// 사용자 메뉴 선택을 위한 열거형
enum class MenuOption {
    PushFront = 1,    // 리스트 앞에 데이터 추가
    PushBack,         // 리스트 뒤에 데이터 추가
    InsertAfter,      // 특정 위치 다음에 데이터 삽입
    PopFront,         // 리스트 앞의 데이터 제거
    RemoveAt,         // 특정 위치의 데이터 제거
    GetElement,       // 특정 위치의 데이터 조회
    Print,            // 리스트 전체 출력
    Size,             // 리스트 크기 확인
    IsEmpty,          // 리스트 공백 상태 확인
    Exit              // 프로그램 종료
};

// 입력 버퍼 비우기 함수
void clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// 사용자 메뉴 출력 함수
void printMenu() {
    std::cout << "\n=== Double Linked List Test Menu ===\n"
              << "1. Push Front    - 리스트 앞에 데이터 추가\n"
              << "2. Push Back     - 리스트 뒤에 데이터 추가\n"
              << "3. Insert After  - 특정 위치 다음에 데이터 삽입\n"
              << "4. Pop Front     - 리스트 앞의 데이터 제거\n"
              << "5. Remove At     - 특정 위치의 데이터 제거\n"
              << "6. Get Element   - 특정 위치의 데이터 조회\n"
              << "7. Print List    - 리스트 전체 출력\n"
              << "8. Get Size      - 리스트 크기 확인\n"
              << "9. Check Empty   - 리스트 공백 상태 확인\n"
              << "10. Exit         - 프로그램 종료\n"
              << "메뉴 선택: ";
}

int main() {
    // 정수를 저장하는 이중 연결 리스트 생성
    DoubleLinkedList<int> list;
    int choice, value, index;

    // 사용자가 종료를 선택할 때까지 반복
    do {
        printMenu();
        std::cin >> choice;

        try {
            switch (static_cast<MenuOption>(choice)) {
                case MenuOption::PushFront:
                    std::cout << "추가할 값 입력: ";
                    std::cin >> value;
                    list.push_front(value);
                    std::cout << "값 " << value << " 이(가) 리스트 앞에 추가됨\n";
                    break;

                case MenuOption::PushBack:
                    std::cout << "추가할 값 입력: ";
                    std::cin >> value;
                    list.push_back(value);
                    std::cout << "값 " << value << " 이(가) 리스트 뒤에 추가됨\n";
                    break;

                case MenuOption::InsertAfter:
                    std::cout << "삽입할 위치 입력 (0부터 시작): ";
                    std::cin >> index;
                    std::cout << "삽입할 값 입력: ";
                    std::cin >> value;
                    list.insert_after(index, value);
                    std::cout << "값 " << value << " 이(가) 위치 " << index << " 다음에 삽입됨\n";
                    break;

                case MenuOption::PopFront:
                    list.pop_front();
                    std::cout << "리스트 앞의 요소가 제거됨\n";
                    break;

                case MenuOption::RemoveAt:
                    std::cout << "제거할 위치 입력 (0부터 시작): ";
                    std::cin >> index;
                    list.remove_at(index);
                    std::cout << "위치 " << index << "의 요소가 제거됨\n";
                    break;

                case MenuOption::GetElement:
                    std::cout << "조회할 위치 입력 (0부터 시작): ";
                    std::cin >> index;
                    value = list.at(index);
                    std::cout << "위치 " << index << "의 값: " << value << "\n";
                    break;

                case MenuOption::Print:
                    list.print();
                    break;

                case MenuOption::Size:
                    std::cout << "리스트 크기: " << list.size() << "\n";
                    break;

                case MenuOption::IsEmpty:
                    std::cout << "리스트 상태: " << (list.empty() ? "비어있음" : "비어있지 않음") << "\n";
                    break;

                case MenuOption::Exit:
                    std::cout << "프로그램을 종료합니다.\n";
                    break;

                default:
                    std::cout << "잘못된 메뉴 선택입니다.\n";
            }
        }
        catch (const std::exception& e) {
            std::cout << "오류 발생: " << e.what() << "\n";
        }

        clearInputBuffer();

    } while (static_cast<MenuOption>(choice) != MenuOption::Exit);

    return 0;
}
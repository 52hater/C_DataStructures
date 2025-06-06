#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef int DataType;

// 노드 구조체 정의
typedef struct Node {
    DataType data;
    struct Node* prev;  // 이전 노드를 가리키는 포인터
    struct Node* next;  // 다음 노드를 가리키는 포인터
} Node;

// 이중 연결 리스트 구조체 정의
typedef struct {
    Node* head;     // 첫 노드를 가리키는 포인터
    Node* tail;     // 마지막 노드를 가리키는 포인터
    size_t size;    // 현재 리스트의 크기
} DoublyLinkedList;

/* 리스트 초기화 함수
 * - 새로운 빈 이중 연결 리스트를 생성하고 초기화
 * - 반환값: 초기화된 DoublyLinkedList 구조체의 포인터
 */
DoublyLinkedList* list_create(void) {
    DoublyLinkedList* list = (DoublyLinkedList*)malloc(sizeof(DoublyLinkedList));
    if (list == NULL) {
        return NULL;
    }
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}

/* 새로운 노드 생성 함수
 * - 데이터를 담은 새로운 노드를 동적 할당
 * - 매개변수: data - 저장할 데이터 값
 * - 반환값: 생성된 새 노드의 포인터
 */
Node* node_create(DataType data) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        return NULL;
    }
    new_node->data = data;
    new_node->prev = NULL;
    new_node->next = NULL;
    return new_node;
}

/* 리스트가 비어있는지 확인
 * - 매개변수: list - 확인할 리스트
 * - 반환값: 비어있으면 true, 아니면 false
 */
bool list_is_empty(const DoublyLinkedList* list) {
    return (list->size == 0);
}

/* 리스트의 크기 반환
 * - 매개변수: list - 대상 리스트
 * - 반환값: 리스트의 현재 노드 개수
 */
size_t list_size(const DoublyLinkedList* list) {
    return list->size;
}

/* 리스트의 맨 앞에 노드 삽입
 * - 매개변수: list - 대상 리스트, data - 저장할 데이터
 * - 반환값: 성공 시 true, 실패 시 false
 */
bool list_push_front(DoublyLinkedList* list, DataType data) {
    Node* new_node = node_create(data);
    if (new_node == NULL) {
        return false;
    }

    if (list_is_empty(list)) {
        list->head = new_node;
        list->tail = new_node;
    }
    else {
        new_node->next = list->head;
        list->head->prev = new_node;
        list->head = new_node;
    }
    list->size++;
    return true;
}

/* 리스트의 맨 뒤에 노드 삽입
 * - 매개변수: list - 대상 리스트, data - 저장할 데이터
 * - 반환값: 성공 시 true, 실패 시 false
 */
bool list_push_back(DoublyLinkedList* list, DataType data) {
    Node* new_node = node_create(data);
    if (new_node == NULL) {
        return false;
    }

    if (list_is_empty(list)) {
        list->head = new_node;
        list->tail = new_node;
    }
    else {
        new_node->prev = list->tail;
        list->tail->next = new_node;
        list->tail = new_node;
    }
    list->size++;
    return true;
}

/* 특정 위치에 노드 삽입
 * - 매개변수: list - 대상 리스트, position - 삽입할 위치(0-based), data - 저장할 데이터
 * - 반환값: 성공 시 true, 실패 시 false
 */
bool list_insert_at(DoublyLinkedList* list, size_t position, DataType data) {
    if (position > list->size) {
        return false;
    }

    if (position == 0) {
        return list_push_front(list, data);
    }

    if (position == list->size) {
        return list_push_back(list, data);
    }

    Node* new_node = node_create(data);
    if (new_node == NULL) {
        return false;
    }

    Node* current = list->head;
    for (size_t i = 0; i < position; i++) {
        current = current->next;
    }

    new_node->prev = current->prev;
    new_node->next = current;
    current->prev->next = new_node;
    current->prev = new_node;
    list->size++;
    return true;
}

/* 특정 위치의 노드 삭제
 * - 매개변수: list - 대상 리스트, position - 삭제할 위치(0-based)
 * - 반환값: 성공 시 true, 실패 시 false
 */
bool list_remove_at(DoublyLinkedList* list, size_t position) {
    if (list_is_empty(list) || position >= list->size) {
        return false;
    }

    Node* target;
    if (position == 0) {
        target = list->head;
        list->head = target->next;
        if (list->head) {
            list->head->prev = NULL;
        }
        else {
            list->tail = NULL;
        }
    }
    else if (position == list->size - 1) {
        target = list->tail;
        list->tail = target->prev;
        list->tail->next = NULL;
    }
    else {
        target = list->head;
        for (size_t i = 0; i < position; i++) {
            target = target->next;
        }
        target->prev->next = target->next;
        target->next->prev = target->prev;
    }

    free(target);
    list->size--;
    return true;
}

/* 특정 값을 가진 노드 검색
 * - 매개변수: list - 대상 리스트, data - 찾을 값
 * - 반환값: 찾은 노드의 인덱스, 없으면 -1
 */
int list_find(const DoublyLinkedList* list, DataType data) {
    Node* current = list->head;
    int index = 0;

    while (current != NULL) {
        if (current->data == data) {
            return index;
        }
        current = current->next;
        index++;
    }
    return -1;
}

/* 리스트 출력 함수
 * - 매개변수: list - 출력할 리스트
 */
void list_print(const DoublyLinkedList* list) {
    if (list_is_empty(list)) {
        printf("List is empty\n");
        return;
    }

    printf("List (size=%zu): ", list->size);
    Node* current = list->head;
    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}

/* 리스트 메모리 해제
 * - 매개변수: list - 삭제할 리스트
 */
void list_destroy(DoublyLinkedList* list) {
    Node* current = list->head;
    while (current != NULL) {
        Node* next = current->next;
        free(current);
        current = next;
    }
    free(list);
}

/* 메뉴 출력 함수 */
void print_menu(void) {
    printf("\n=== Doubly Linked List Menu ===\n");
    printf("1. Insert at front\n");
    printf("2. Insert at back\n");
    printf("3. Insert at position\n");
    printf("4. Remove at position\n");
    printf("5. Search value\n");
    printf("6. Print list\n");
    printf("7. Check if empty\n");
    printf("8. Get size\n");
    printf("0. Exit\n");
    printf("Choice: ");
}

int main(void) {
    DoublyLinkedList* list = list_create();
    if (list == NULL) {
        printf("Failed to create list\n");
        return 1;
    }

    int choice;
    DataType data;
    size_t position;

    do {
        print_menu();
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');  // 입력 버퍼 비우기
            continue;
        }

        switch (choice) {
        case 1:  // Insert at front
            printf("Enter value to insert: ");
            scanf("%d", &data);
            if (list_push_front(list, data)) {
                printf("Successfully inserted %d at front\n", data);
            }
            else {
                printf("Failed to insert\n");
            }
            break;

        case 2:  // Insert at back
            printf("Enter value to insert: ");
            scanf("%d", &data);
            if (list_push_back(list, data)) {
                printf("Successfully inserted %d at back\n", data);
            }
            else {
                printf("Failed to insert\n");
            }
            break;

        case 3:  // Insert at position
            printf("Enter position: ");
            scanf("%zu", &position);
            printf("Enter value to insert: ");
            scanf("%d", &data);
            if (list_insert_at(list, position, data)) {
                printf("Successfully inserted %d at position %zu\n", data, position);
            }
            else {
                printf("Failed to insert\n");
            }
            break;

        case 4:  // Remove at position
            printf("Enter position to remove: ");
            scanf("%zu", &position);
            if (list_remove_at(list, position)) {
                printf("Successfully removed element at position %zu\n", position);
            }
            else {
                printf("Failed to remove\n");
            }
            break;

        case 5:  // Search value
            printf("Enter value to search: ");
            scanf("%d", &data);
            int index = list_find(list, data);
            if (index != -1) {
                printf("Value %d found at position %d\n", data, index);
            }
            else {
                printf("Value %d not found in the list\n", data);
            }
            break;

        case 6:  // Print list
            list_print(list);
            break;

        case 7:  // Check if empty
            printf("List is %sempty\n", list_is_empty(list) ? "" : "not ");
            break;

        case 8:  // Get size
            printf("List size: %zu\n", list_size(list));
            break;

        case 0:  // Exit
            printf("Exiting program\n");
            break;

        default:
            printf("Invalid choice\n");
        }
    } while (choice != 0);

    list_destroy(list);
    return 0;
}

/*
==========================================
상세 설명 및 주요 개념
==========================================

1. 이중 연결 리스트의 특징
------------------------
- 각 노드가 이전 노드와 다음 노드의 포인터를 모두 가짐
- 양방향 순회 가능
- 삽입/삭제 연산이 단일 연결 리스트보다 효율적
- tail 포인터로 끝 노드에 즉시 접근 가능

2. 주요 구조체
------------
Node:
- data: 실제 데이터 저장
- prev: 이전 노드 포인터
- next: 다음 노드 포인터

DoublyLinkedList:
- head: 첫 노드 포인터
- tail: 마지막 노드 포인터
- size: 현재 노드 개수

3. 구현된 주요 기능
----------------
- 노드 삽입: 앞/뒤/중간 위치
- 노드 삭제: 위치 기반
- 값 검색
- 리스트 출력
- 빈 리스트 확인
- 크기 확인

4. 메모리 관리
------------
- 동적 할당: malloc() 사용
- 할당 실패 처리: NULL 체크
- 메모리 해제: free() 사용
- 메모리 누수 방지

5. 에러 처리
----------
- 경계 조건 검사
- 메모리 할당 실패 처리
- 잘못된 위치 접근 방지
- 빈 리스트 처리

6. 사용자 인터페이스
-----------------
- 메뉴 기반 대화형 인터페이스
- 입력 검증
- 명확한 피드백 제공
- 잘못된 입력 처리

7. 최적화 특징
------------
- O(1) 시간 복잡도:
  * 앞/뒤 삽입
  * 크기 확인
  * 빈 리스트 확인
- O(n) 시간 복잡도:
  * 중간 삽입/삭제
  * 값 검색

8. 코드 스타일
------------
- 일관된 명명 규칙
- 자세한 주석
- 모듈화된 설계
- 직관적인 인터페이스

9. 안전성 고려사항
---------------
- NULL 포인터 검사
- 메모리 누수 방지
- 경계 조건 처리
- 사용자 입력 검증

10. 교육적 가치
-------------
- 포인터 개념 학습
- 메모리 관리 실습
- 자료구조 이해
- 사용자 인터페

*/
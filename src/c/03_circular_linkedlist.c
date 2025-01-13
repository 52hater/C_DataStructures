#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef int DataType;

// 노드 구조체 정의
typedef struct Node {
    DataType data;
    struct Node* next;  // 다음 노드를 가리키는 포인터
} Node;

// 원형 연결 리스트 구조체 정의
typedef struct {
    Node* tail;     // 마지막 노드를 가리키는 포인터 (tail->next는 head를 가리킴)
    size_t size;    // 현재 리스트의 크기
} CircularLinkedList;

/* 리스트 초기화 함수
 * - 새로운 빈 원형 연결 리스트를 생성하고 초기화
 * - 반환값: 초기화된 CircularLinkedList 구조체의 포인터
 */
CircularLinkedList* list_create(void) {
    CircularLinkedList* list = (CircularLinkedList*)malloc(sizeof(CircularLinkedList));
    if (list == NULL) {
        return NULL;
    }
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
    new_node->next = NULL;
    return new_node;
}

/* 리스트가 비어있는지 확인
 * - 매개변수: list - 확인할 리스트
 * - 반환값: 비어있으면 true, 아니면 false
 */
bool list_is_empty(const CircularLinkedList* list) {
    return (list->size == 0);
}

/* 리스트의 크기 반환
 * - 매개변수: list - 대상 리스트
 * - 반환값: 리스트의 현재 노드 개수
 */
size_t list_size(const CircularLinkedList* list) {
    return list->size;
}

/* 리스트의 맨 앞에 노드 삽입
 * - 매개변수: list - 대상 리스트, data - 저장할 데이터
 * - 반환값: 성공 시 true, 실패 시 false
 */
bool list_push_front(CircularLinkedList* list, DataType data) {
    Node* new_node = node_create(data);
    if (new_node == NULL) {
        return false;
    }

    if (list_is_empty(list)) {
        new_node->next = new_node;  // 자기 자신을 가리킴
        list->tail = new_node;
    }
    else {
        new_node->next = list->tail->next;  // 현재 head를 가리킴
        list->tail->next = new_node;        // tail이 새로운 head를 가리킴
    }
    list->size++;
    return true;
}

/* 리스트의 맨 뒤에 노드 삽입
 * - 매개변수: list - 대상 리스트, data - 저장할 데이터
 * - 반환값: 성공 시 true, 실패 시 false
 */
bool list_push_back(CircularLinkedList* list, DataType data) {
    Node* new_node = node_create(data);
    if (new_node == NULL) {
        return false;
    }

    if (list_is_empty(list)) {
        new_node->next = new_node;  // 자기 자신을 가리킴
    }
    else {
        new_node->next = list->tail->next;  // 새 노드가 head를 가리킴
        list->tail->next = new_node;        // 이전 tail이 새 노드를 가리킴
    }
    list->tail = new_node;  // 새 노드가 tail이 됨
    list->size++;
    return true;
}

/* 특정 위치에 노드 삽입
 * - 매개변수: list - 대상 리스트, position - 삽입할 위치(0-based), data - 저장할 데이터
 * - 반환값: 성공 시 true, 실패 시 false
 */
bool list_insert_at(CircularLinkedList* list, size_t position, DataType data) {
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

    Node* current = list->tail->next;  // head부터 시작
    for (size_t i = 0; i < position - 1; i++) {
        current = current->next;
    }

    new_node->next = current->next;
    current->next = new_node;
    list->size++;
    return true;
}

/* 특정 위치의 노드 삭제
 * - 매개변수: list - 대상 리스트, position - 삭제할 위치(0-based)
 * - 반환값: 성공 시 true, 실패 시 false
 */
bool list_remove_at(CircularLinkedList* list, size_t position) {
    if (list_is_empty(list) || position >= list->size) {
        return false;
    }

    Node* head = list->tail->next;
    Node* target;

    if (position == 0) {
        target = head;
        if (list->size == 1) {
            list->tail = NULL;
        }
        else {
            list->tail->next = head->next;
        }
    }
    else {
        Node* previous = head;
        for (size_t i = 0; i < position - 1; i++) {
            previous = previous->next;
        }
        target = previous->next;
        previous->next = target->next;
        if (target == list->tail) {
            list->tail = previous;
        }
    }

    free(target);
    list->size--;
    return true;
}

/* 특정 값을 가진 노드 검색
 * - 매개변수: list - 대상 리스트, data - 찾을 값
 * - 반환값: 찾은 노드의 인덱스, 없으면 -1
 */
int list_find(const CircularLinkedList* list, DataType data) {
    if (list_is_empty(list)) {
        return -1;
    }

    Node* current = list->tail->next;  // head부터 시작
    int index = 0;

    do {
        if (current->data == data) {
            return index;
        }
        current = current->next;
        index++;
    } while (current != list->tail->next);

    return -1;
}

/* 리스트 출력 함수
 * - 매개변수: list - 출력할 리스트
 */
void list_print(const CircularLinkedList* list) {
    if (list_is_empty(list)) {
        printf("List is empty\n");
        return;
    }

    printf("List (size=%zu): ", list->size);
    Node* current = list->tail->next;  // head부터 시작
    do {
        printf("%d ", current->data);
        current = current->next;
    } while (current != list->tail->next);
    printf("\n");
}

/* 리스트 메모리 해제
 * - 매개변수: list - 삭제할 리스트
 */
void list_destroy(CircularLinkedList* list) {
    if (!list_is_empty(list)) {
        Node* current = list->tail->next;  // head부터 시작
        Node* next;
        do {
            next = current->next;
            free(current);
            current = next;
        } while (current != list->tail->next);
    }
    free(list);
}

/* 메뉴 출력 함수 */
void print_menu(void) {
    printf("\n=== Circular Linked List Menu ===\n");
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
    CircularLinkedList* list = list_create();
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

1. 원형 연결 리스트의 특징
------------------------
- 마지막 노드가 첫 번째 노드를 가리킴
- 순환 구조로 인한 무한 순회 가능
- tail->next가 head를 가리킴
- 단일 포인터(tail)만으로도 리스트 관리 가능

2. 구조체 설계
------------
Node:
- data: 실제 데이터
- next: 다음 노드 포인터

CircularLinkedList:
- tail: 마지막 노드 포인터
- size: 현재 노드 개수

3. 주요 알고리즘 특징
------------------
삽입 연산:
- 앞/뒤 삽입 시 순환 구조 유지
- tail->next 갱신으로 head 접근
- 빈 리스트 처리 (자기 자신을 가리키도록 설정)

삭제 연산:
- 순환 구조 유지를 위한 포인터 조정
- 마지막 노드 삭제 시 tail 갱신
- 단일 노드 삭제 시 특별 처리

순회 연산:
- do-while 루프 사용
- head(tail->next)부터 시작하여 한바퀴 순회
- tail->next로 순회 종료 조건 체크

4. 메모리 관리
------------
- 동적 할당 및 해제
- 순환 구조에서의 메모리 누수 방지
- NULL 포인터 검사

5. 주요 최적화
------------
- O(1) 시간 복잡도:
  * 앞/뒤 삽입
  * 크기 확인
- tail 포인터 활용:
  * head 접근 최적화
  * 마지막 노드 즉시 접근

6. 안전성 고려사항
---------------
- 순환 참조 유지
- 메모리 누수 방지
- 경계 조건 처리:
  * 빈 리스트
  * 단일 노드
  * 마지막 노드

7. 원형 리스트의 장점
------------------
- 순환 구조로 인한 연속 처리 용이
- 마지막 노드에서 첫 노드 즉시 접근
- tail만으로 전체 리스트

*/
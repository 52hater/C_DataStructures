#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef int ElementType;

// 노드 구조체 정의
typedef struct Node {
    ElementType data;
    struct Node* next;
} Node;

// 스택 구조체 정의
typedef struct {
    Node* top;      // 스택의 최상위 노드를 가리키는 포인터
    size_t size;    // 현재 스택에 저장된 요소의 수
} Stack;

/* 오류 처리를 위한 열거형 정의
 * - 스택 연산의 다양한 결과 상태를 표현
 */
typedef enum {
    STACK_OK,
    STACK_EMPTY,
    STACK_MEMORY_ERROR
} StackResult;

/* 새로운 노드 생성
 * - 매개변수: data - 저장할 데이터
 * - 반환값: 생성된 노드의 포인터 또는 실패 시 NULL
 */
static Node* create_node(ElementType data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        return NULL;
    }
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

/* 스택 생성
 * - 새로운 빈 스택을 동적으로 할당하고 초기화
 * - 반환값: 생성된 스택의 포인터 또는 실패 시 NULL
 */
Stack* stack_create(void) {
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    if (stack == NULL) {
        return NULL;
    }
    stack->top = NULL;
    stack->size = 0;
    return stack;
}

/* 스택이 비어있는지 확인
 * - 매개변수: stack - 검사할 스택
 * - 반환값: 비어있으면 true, 아니면 false
 */
bool stack_is_empty(const Stack* stack) {
    return (stack->size == 0);
}

/* 스택의 현재 크기 반환
 * - 매개변수: stack - 대상 스택
 * - 반환값: 스택에 저장된 요소의 수
 */
size_t stack_size(const Stack* stack) {
    return stack->size;
}

/* 스택에 요소 추가 (push)
 * - 매개변수: stack - 대상 스택, value - 추가할 값
 * - 반환값: 연산 결과를 나타내는 StackResult
 */
StackResult stack_push(Stack* stack, ElementType value) {
    Node* newNode = create_node(value);
    if (newNode == NULL) {
        return STACK_MEMORY_ERROR;
    }

    newNode->next = stack->top;
    stack->top = newNode;
    stack->size++;
    return STACK_OK;
}

/* 스택에서 요소 제거 (pop)
 * - 매개변수: stack - 대상 스택, value - 제거된 값을 저장할 포인터
 * - 반환값: 연산 결과를 나타내는 StackResult
 */
StackResult stack_pop(Stack* stack, ElementType* value) {
    if (stack_is_empty(stack)) {
        return STACK_EMPTY;
    }

    Node* temp = stack->top;
    *value = temp->data;
    stack->top = temp->next;
    free(temp);
    stack->size--;
    return STACK_OK;
}

/* 스택의 맨 위 요소 확인 (peek)
 * - 매개변수: stack - 대상 스택, value - 값을 저장할 포인터
 * - 반환값: 연산 결과를 나타내는 StackResult
 */
StackResult stack_peek(const Stack* stack, ElementType* value) {
    if (stack_is_empty(stack)) {
        return STACK_EMPTY;
    }

    *value = stack->top->data;
    return STACK_OK;
}

/* 스택 초기화 (clear)
 * - 매개변수: stack - 초기화할 스택
 */
void stack_clear(Stack* stack) {
    ElementType dummy;
    while (!stack_is_empty(stack)) {
        stack_pop(stack, &dummy);
    }
}

/* 스택 메모리 해제
 * - 매개변수: stack - 해제할 스택
 */
void stack_destroy(Stack* stack) {
    stack_clear(stack);
    free(stack);
}

/* 스택의 모든 요소 출력
 * - 매개변수: stack - 출력할 스택
 */
void stack_print(const Stack* stack) {
    if (stack_is_empty(stack)) {
        printf("Stack is empty\n");
        return;
    }

    printf("Stack (size=%zu): TOP [ ", stack->size);
    Node* current = stack->top;
    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("] BOTTOM\n");
}

/* 스택의 상태 정보 출력
 * - 매개변수: stack - 상태를 확인할 스택
 */
void stack_status(const Stack* stack) {
    printf("\nStack Status:\n");
    printf("- Size: %zu\n", stack->size);
    printf("- Empty: %s\n", stack_is_empty(stack) ? "Yes" : "No");

    // 메모리 사용량 계산 (대략적)
    size_t stack_memory = sizeof(Stack);
    size_t node_memory = sizeof(Node) * stack->size;
    printf("- Memory usage: %zu bytes (overhead: %zu bytes, nodes: %zu bytes)\n",
        stack_memory + node_memory, stack_memory, node_memory);
}

/* 메뉴 출력 함수 */
void print_menu(void) {
    printf("\n=== Linked List-based Stack Menu ===\n");
    printf("1. Push\n");
    printf("2. Pop\n");
    printf("3. Peek\n");
    printf("4. Print stack\n");
    printf("5. Check if empty\n");
    printf("6. Get size\n");
    printf("7. Clear stack\n");
    printf("8. Show stack status\n");
    printf("0. Exit\n");
    printf("Choice: ");
}

/* 에러 메시지 출력 함수
 * - 매개변수: result - 스택 연산 결과
 */
void print_error(StackResult result) {
    switch (result) {
    case STACK_EMPTY:
        printf("Error: Stack is empty\n");
        break;
    case STACK_MEMORY_ERROR:
        printf("Error: Memory allocation failed\n");
        break;
    default:
        break;
    }
}

int main(void) {
    Stack* stack = stack_create();
    if (stack == NULL) {
        printf("Failed to create stack\n");
        return 1;
    }

    int choice;
    ElementType value;
    StackResult result;

    do {
        print_menu();
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');  // 입력 버퍼 비우기
            continue;
        }

        switch (choice) {
        case 1:  // Push
            printf("Enter value to push: ");
            scanf("%d", &value);
            result = stack_push(stack, value);
            if (result == STACK_OK) {
                printf("Successfully pushed %d\n", value);
            }
            else {
                print_error(result);
            }
            break;

        case 2:  // Pop
            result = stack_pop(stack, &value);
            if (result == STACK_OK) {
                printf("Popped value: %d\n", value);
            }
            else {
                print_error(result);
            }
            break;

        case 3:  // Peek
            result = stack_peek(stack, &value);
            if (result == STACK_OK) {
                printf("Top value: %d\n", value);
            }
            else {
                print_error(result);
            }
            break;

        case 4:  // Print stack
            stack_print(stack);
            break;

        case 5:  // Check if empty
            printf("Stack is %sempty\n", stack_is_empty(stack) ? "" : "not ");
            break;

        case 6:  // Get size
            printf("Stack size: %zu\n", stack_size(stack));
            break;

        case 7:  // Clear stack
            stack_clear(stack);
            printf("Stack cleared\n");
            break;

        case 8:  // Show stack status
            stack_status(stack);
            break;

        case 0:  // Exit
            printf("Exiting program\n");
            break;

        default:
            printf("Invalid choice\n");
        }
    } while (choice != 0);

    stack_destroy(stack);
    return 0;
}

/*
==========================================
상세 설명 및 주요 개념
==========================================

1. 연결 리스트 기반 스택의 특징
---------------------------
- 동적 메모리 할당 사용
- 노드 단위의 메모리 관리
- 크기 제한 없음
- 포인터 기반 구조

2. 배열 기반 vs 연결 리스트 기반 비교
--------------------------------
장점:
- 크기 제한이 없음
- 메모리를 필요한 만큼만 사용
- 삽입/삭제가 항상 O(1)
- 재할당 오버헤드 없음

단점:
- 추가 메모리 사용 (포인터)
- 캐시 지역성 낮음
- 메모리 단편화 가능성
- 개별 노드 할당/해제 오버헤드

3. 구현 특징
----------
Node 구조체:
- data: 실제 데이터
- next: 다음 노드 포인터

Stack 구조체:
- top: 최상위 노드 포인터
- size: 현재 요소 수

4. 주요 연산
----------
기본 연산:
- Push: O(1)
- Pop: O(1)
- Peek: O(1)

보조 연산:
- Clear: O(n)
- IsEmpty: O(1)
- Size: O(1)

5. 메모리 관리
-----------
- 노드 단위 동적 할당
- 개별 노드 해제
- 스택 구조체 관리
- 메모리 누수 방지

6. 오류 처리
----------
- 메모리 할당 실패
- 빈 스택 처리
- NULL 포인터 검사
- 안전한 메모리 해제

7. 최적화 특징
-----------
- 상수 시간 삽입/삭제
- 최소한의 메모리 이동
- 효율적인 메모리 사용
- 간단한 구현

8. 안전성 고려사항
--------------
- 포인터 유효성 검사
- 메모리 누수 방지
- 단계적 자원 해제
- 경계 조건 처리

9. 교육적 가치
-----------
- 포인터 개념 학습
- 동적 메모리 관리
- 연결 리스트 이해
- LIFO 원리 학습

10. 실무적 특징
------------
- 견고한 구현
- 확장 가능한 구조
- 명확한 에러 처리
- 유지보수 용이성

이 구현은 실제 프로젝트에서 사용할 수 있는
안정적이고 효율적인 연결 리스트 기반 스택을
제공합니다. 특히 크기가 가변적이거나
예측하기 어려운 경우에 유용합니다.
*/
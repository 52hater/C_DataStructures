#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// 스택의 기본 크기와 확장 비율 설정
#define INITIAL_CAPACITY 8
#define GROWTH_FACTOR 2
#define SHRINK_THRESHOLD 0.25

typedef int ElementType;

// 스택 구조체 정의
typedef struct {
    ElementType* elements;  // 요소를 저장할 동적 배열
    size_t size;           // 현재 저장된 요소의 수
    size_t capacity;       // 현재 할당된 배열의 크기
} Stack;

/* 오류 처리를 위한 열거형 정의
 * - 스택 연산의 다양한 결과 상태를 표현
 */
typedef enum {
    STACK_OK,
    STACK_EMPTY,
    STACK_FULL,
    STACK_MEMORY_ERROR
} StackResult;

/* 스택 생성 함수
 * - 초기 크기의 스택을 동적으로 할당하고 초기화
 * - 반환값: 생성된 스택의 포인터 또는 실패 시 NULL
 */
Stack* stack_create(void) {
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    if (stack == NULL) {
        return NULL;
    }

    stack->elements = (ElementType*)malloc(INITIAL_CAPACITY * sizeof(ElementType));
    if (stack->elements == NULL) {
        free(stack);
        return NULL;
    }

    stack->size = 0;
    stack->capacity = INITIAL_CAPACITY;
    return stack;
}

/* 스택이 비어있는지 확인
 * - 매개변수: stack - 검사할 스택
 * - 반환값: 비어있으면 true, 아니면 false
 */
bool stack_is_empty(const Stack* stack) {
    return (stack->size == 0);
}

/* 스택이 가득 찼는지 확인
 * - 매개변수: stack - 검사할 스택
 * - 반환값: 가득 찼으면 true, 아니면 false
 */
bool stack_is_full(const Stack* stack) {
    return (stack->size == stack->capacity);
}

/* 스택의 현재 크기 반환
 * - 매개변수: stack - 대상 스택
 * - 반환값: 스택에 저장된 요소의 수
 */
size_t stack_size(const Stack* stack) {
    return stack->size;
}

/* 스택의 용량 반환
 * - 매개변수: stack - 대상 스택
 * - 반환값: 스택의 현재 최대 용량
 */
size_t stack_capacity(const Stack* stack) {
    return stack->capacity;
}

/* 스택 배열 크기 조정
 * - 매개변수: stack - 대상 스택, new_capacity - 새로운 용량
 * - 반환값: 성공 시 true, 실패 시 false
 */
static bool stack_resize(Stack* stack, size_t new_capacity) {
    ElementType* new_elements = (ElementType*)realloc(stack->elements,
        new_capacity * sizeof(ElementType));
    if (new_elements == NULL) {
        return false;
    }

    stack->elements = new_elements;
    stack->capacity = new_capacity;
    return true;
}

/* 스택에 요소 추가 (push)
 * - 매개변수: stack - 대상 스택, value - 추가할 값
 * - 반환값: 연산 결과를 나타내는 StackResult
 */
StackResult stack_push(Stack* stack, ElementType value) {
    if (stack_is_full(stack)) {
        size_t new_capacity = stack->capacity * GROWTH_FACTOR;
        if (!stack_resize(stack, new_capacity)) {
            return STACK_MEMORY_ERROR;
        }
    }

    stack->elements[stack->size++] = value;
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

    *value = stack->elements[--stack->size];

    // 스택 크기가 충분히 작아지면 배열 크기 축소
    if (stack->size < stack->capacity * SHRINK_THRESHOLD &&
        stack->capacity > INITIAL_CAPACITY) {
        size_t new_capacity = stack->capacity / GROWTH_FACTOR;
        if (new_capacity < INITIAL_CAPACITY) {
            new_capacity = INITIAL_CAPACITY;
        }
        stack_resize(stack, new_capacity);  // 실패해도 계속 진행
    }

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

    *value = stack->elements[stack->size - 1];
    return STACK_OK;
}

/* 스택 초기화 (clear)
 * - 매개변수: stack - 초기화할 스택
 */
void stack_clear(Stack* stack) {
    stack->size = 0;
    if (stack->capacity > INITIAL_CAPACITY) {
        stack_resize(stack, INITIAL_CAPACITY);  // 실패해도 계속 진행
    }
}

/* 스택 메모리 해제
 * - 매개변수: stack - 해제할 스택
 */
void stack_destroy(Stack* stack) {
    free(stack->elements);
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

    printf("Stack (size=%zu, capacity=%zu): ", stack->size, stack->capacity);
    printf("TOP [ ");
    for (size_t i = stack->size; i > 0; i--) {
        printf("%d ", stack->elements[i - 1]);
    }
    printf("] BOTTOM\n");
}

/* 스택의 상태 정보 출력
 * - 매개변수: stack - 상태를 확인할 스택
 */
void stack_status(const Stack* stack) {
    printf("\nStack Status:\n");
    printf("- Size: %zu\n", stack->size);
    printf("- Capacity: %zu\n", stack->capacity);
    printf("- Empty: %s\n", stack_is_empty(stack) ? "Yes" : "No");
    printf("- Full: %s\n", stack_is_full(stack) ? "Yes" : "No");
    float usage = stack->size / (float)stack->capacity * 100;
    printf("- Usage: %.1f%%\n", usage);
}

/* 메뉴 출력 함수 */
void print_menu(void) {
    printf("\n=== Array-based Stack Menu ===\n");
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
    case STACK_FULL:
        printf("Error: Stack is full\n");
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

1. 배열 기반 스택의 특징
----------------------
- 연속된 메모리 공간 사용
- 동적 크기 조정 가능
- 인덱스 기반 빠른 접근
- 메모리 효율적 관리

2. 주요 구현 특징
--------------
- 동적 배열 사용
- 자동 크기 조정
- 효율적인 메모리 관리
- 상세한 오류 처리

3. 핵심 데이터 구조
----------------
Stack 구조체:
- elements: 요소 저장 배열
- size: 현재 요소 수
- capacity: 현재 용량

4. 주요 연산
----------
기본 연산:
- Push: O(1) - 최악의 경우 O(n)
- Pop: O(1) - 최악의 경우 O(n)
- Peek: O(1)
보조 연산:
- Clear: O(1)
- IsEmpty: O(1)
- IsFull: O(1)

5. 메모리 관리 전략
----------------
- 동적 크기 조정
  * 성장 계수: 2배
  * 축소 임계값: 25%
- 초기 용량: 8
- 최소 용량 유지

6. 오류 처리
----------
- 세분화된 오류 상태
- NULL 포인터 검사
- 메모리 할당 실패 처리
- 경계 조건 처리

7. 최적화 특징
------------
- 상수 시간 접근
- 효율적인 메모리 사용
- 자동 크기 조정
- 캐시 친화적 구조

8. 안전성 고려사항
---------------
- 메모리 누수 방지
- 배열 경계 검사
- 포인터 유효성 검사
- 자원 정리 보장

9. 교육적 가치
------------
- 배열 동작 이해
- 메모리 관리 학습
- LIFO 원리 학습
- 동적 할당 이해

10. 실무적 특징
-------------
- 견고한 구현
- 확장 가능한 구조
- 성능 최적화
- 유지보수 용이성

이 구현은 교육용으로도 적합하며,
실제 프로젝트에서도 활용할 수 있는
안정적이고 효율적인 스택을 제공합니다.
*/
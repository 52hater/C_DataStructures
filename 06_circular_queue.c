#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// 큐의 기본 크기 설정
#define INITIAL_CAPACITY 8

typedef int ElementType;

// 큐 구조체 정의
typedef struct {
    ElementType* elements;  // 요소를 저장할 동적 배열
    size_t capacity;       // 큐의 최대 용량
    size_t front;          // 첫 번째 요소의 인덱스
    size_t rear;           // 마지막 요소 다음의 인덱스
    size_t size;           // 현재 저장된 요소의 수
} CircularQueue;

/* 오류 처리를 위한 열거형 정의
 * - 큐 연산의 다양한 결과 상태를 표현
 */
typedef enum {
    QUEUE_OK,
    QUEUE_EMPTY,
    QUEUE_FULL,
    QUEUE_MEMORY_ERROR
} QueueResult;

/* 큐 생성 함수
 * - 초기 크기의 원형 큐를 동적으로 할당하고 초기화
 * - 반환값: 생성된 큐의 포인터 또는 실패 시 NULL
 */
CircularQueue* queue_create(void) {
    CircularQueue* queue = (CircularQueue*)malloc(sizeof(CircularQueue));
    if (queue == NULL) {
        return NULL;
    }

    queue->elements = (ElementType*)malloc(INITIAL_CAPACITY * sizeof(ElementType));
    if (queue->elements == NULL) {
        free(queue);
        return NULL;
    }

    queue->capacity = INITIAL_CAPACITY;
    queue->front = 0;
    queue->rear = 0;
    queue->size = 0;
    return queue;
}

/* 큐가 비어있는지 확인
 * - 매개변수: queue - 검사할 큐
 * - 반환값: 비어있으면 true, 아니면 false
 */
bool queue_is_empty(const CircularQueue* queue) {
    return (queue->size == 0);
}

/* 큐가 가득 찼는지 확인
 * - 매개변수: queue - 검사할 큐
 * - 반환값: 가득 찼으면 true, 아니면 false
 */
bool queue_is_full(const CircularQueue* queue) {
    return (queue->size == queue->capacity);
}

/* 큐의 현재 크기 반환
 * - 매개변수: queue - 대상 큐
 * - 반환값: 큐에 저장된 요소의 수
 */
size_t queue_size(const CircularQueue* queue) {
    return queue->size;
}

/* 큐의 용량 반환
 * - 매개변수: queue - 대상 큐
 * - 반환값: 큐의 최대 용량
 */
size_t queue_capacity(const CircularQueue* queue) {
    return queue->capacity;
}

/* 다음 위치 계산
 * - 매개변수: current - 현재 위치, capacity - 큐의 용량
 * - 반환값: 다음 순환 위치
 */
static size_t next_position(size_t current, size_t capacity) {
    return (current + 1) % capacity;
}

/* 큐 확장
 * - 매개변수: queue - 대상 큐
 * - 반환값: 성공 시 true, 실패 시 false
 */
static bool queue_resize(CircularQueue* queue) {
    size_t new_capacity = queue->capacity * 2;
    ElementType* new_elements = (ElementType*)malloc(new_capacity * sizeof(ElementType));
    if (new_elements == NULL) {
        return false;
    }

    // 기존 요소들을 새 배열로 복사 (순서 유지)
    size_t j = 0;
    size_t i = queue->front;
    for (size_t count = 0; count < queue->size; count++) {
        new_elements[j] = queue->elements[i];
        i = next_position(i, queue->capacity);
        j++;
    }

    free(queue->elements);
    queue->elements = new_elements;
    queue->capacity = new_capacity;
    queue->front = 0;
    queue->rear = queue->size;
    return true;
}

/* 큐에 요소 추가 (enqueue)
 * - 매개변수: queue - 대상 큐, value - 추가할 값
 * - 반환값: 연산 결과를 나타내는 QueueResult
 */
QueueResult queue_enqueue(CircularQueue* queue, ElementType value) {
    if (queue_is_full(queue)) {
        if (!queue_resize(queue)) {
            return QUEUE_MEMORY_ERROR;
        }
    }

    queue->elements[queue->rear] = value;
    queue->rear = next_position(queue->rear, queue->capacity);
    queue->size++;
    return QUEUE_OK;
}

/* 큐에서 요소 제거 (dequeue)
 * - 매개변수: queue - 대상 큐, value - 제거된 값을 저장할 포인터
 * - 반환값: 연산 결과를 나타내는 QueueResult
 */
QueueResult queue_dequeue(CircularQueue* queue, ElementType* value) {
    if (queue_is_empty(queue)) {
        return QUEUE_EMPTY;
    }

    *value = queue->elements[queue->front];
    queue->front = next_position(queue->front, queue->capacity);
    queue->size--;
    return QUEUE_OK;
}

/* 큐의 맨 앞 요소 확인 (peek)
 * - 매개변수: queue - 대상 큐, value - 값을 저장할 포인터
 * - 반환값: 연산 결과를 나타내는 QueueResult
 */
QueueResult queue_peek(const CircularQueue* queue, ElementType* value) {
    if (queue_is_empty(queue)) {
        return QUEUE_EMPTY;
    }

    *value = queue->elements[queue->front];
    return QUEUE_OK;
}

/* 큐 초기화 (clear)
 * - 매개변수: queue - 초기화할 큐
 */
void queue_clear(CircularQueue* queue) {
    queue->front = 0;
    queue->rear = 0;
    queue->size = 0;
}

/* 큐 메모리 해제
 * - 매개변수: queue - 해제할 큐
 */
void queue_destroy(CircularQueue* queue) {
    free(queue->elements);
    free(queue);
}

/* 큐의 모든 요소 출력
 * - 매개변수: queue - 출력할 큐
 */
void queue_print(const CircularQueue* queue) {
    if (queue_is_empty(queue)) {
        printf("Queue is empty\n");
        return;
    }

    printf("Queue (size=%zu, capacity=%zu): FRONT [ ",
        queue->size, queue->capacity);

    size_t i = queue->front;
    for (size_t count = 0; count < queue->size; count++) {
        printf("%d ", queue->elements[i]);
        i = next_position(i, queue->capacity);
    }
    printf("] REAR\n");
}

/* 큐의 내부 상태 출력
 * - 매개변수: queue - 상태를 확인할 큐
 */
void queue_status(const CircularQueue* queue) {
    printf("\nQueue Status:\n");
    printf("- Size: %zu\n", queue->size);
    printf("- Capacity: %zu\n", queue->capacity);
    printf("- Front Index: %zu\n", queue->front);
    printf("- Rear Index: %zu\n", queue->rear);
    printf("- Empty: %s\n", queue_is_empty(queue) ? "Yes" : "No");
    printf("- Full: %s\n", queue_is_full(queue) ? "Yes" : "No");
    float usage = (float)queue->size / queue->capacity * 100;
    printf("- Usage: %.1f%%\n", usage);

    // 내부 배열 상태 시각화
    printf("- Internal Array: [");
    for (size_t i = 0; i < queue->capacity; i++) {
        if (i == queue->front) printf("F");
        if (i == queue->rear) printf("R");

        bool is_used = false;
        if (queue->front <= queue->rear) {
            is_used = (i >= queue->front && i < queue->rear);
        }
        else {
            is_used = (i >= queue->front || i < queue->rear);
        }

        if (is_used) {
            printf("*");
        }
        else {
            printf("_");
        }
    }
    printf("]\n");
}

/* 메뉴 출력 함수 */
void print_menu(void) {
    printf("\n=== Circular Queue Menu ===\n");
    printf("1. Enqueue\n");
    printf("2. Dequeue\n");
    printf("3. Peek\n");
    printf("4. Print queue\n");
    printf("5. Check if empty\n");
    printf("6. Check if full\n");
    printf("7. Get size\n");
    printf("8. Clear queue\n");
    printf("9. Show queue status\n");
    printf("0. Exit\n");
    printf("Choice: ");
}

/* 에러 메시지 출력 함수
 * - 매개변수: result - 큐 연산 결과
 */
void print_error(QueueResult result) {
    switch (result) {
    case QUEUE_EMPTY:
        printf("Error: Queue is empty\n");
        break;
    case QUEUE_FULL:
        printf("Error: Queue is full\n");
        break;
    case QUEUE_MEMORY_ERROR:
        printf("Error: Memory allocation failed\n");
        break;
    default:
        break;
    }
}

int main(void) {
    CircularQueue* queue = queue_create();
    if (queue == NULL) {
        printf("Failed to create queue\n");
        return 1;
    }

    int choice;
    ElementType value;
    QueueResult result;

    do {
        print_menu();
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');  // 입력 버퍼 비우기
            continue;
        }

        switch (choice) {
        case 1:  // Enqueue
            printf("Enter value to enqueue: ");
            scanf("%d", &value);
            result = queue_enqueue(queue, value);
            if (result == QUEUE_OK) {
                printf("Successfully enqueued %d\n", value);
            }
            else {
                print_error(result);
            }
            break;

        case 2:  // Dequeue
            result = queue_dequeue(queue, &value);
            if (result == QUEUE_OK) {
                printf("Dequeued value: %d\n", value);
            }
            else {
                print_error(result);
            }
            break;

        case 3:  // Peek
            result = queue_peek(queue, &value);
            if (result == QUEUE_OK) {
                printf("Front value: %d\n", value);
            }
            else {
                print_error(result);
            }
            break;

        case 4:  // Print queue
            queue_print(queue);
            break;

        case 5:  // Check if empty
            printf("Queue is %sempty\n", queue_is_empty(queue) ? "" : "not ");
            break;

        case 6:  // Check if full
            printf("Queue is %sfull\n", queue_is_full(queue) ? "" : "not ");
            break;

        case 7:  // Get size
            printf("Queue size: %zu\n", queue_size(queue));
            break;

        case 8:  // Clear queue
            queue_clear(queue);
            printf("Queue cleared\n");
            break;

        case 9:  // Show queue status
            queue_status(queue);
            break;

        case 0:  // Exit
            printf("Exiting program\n");
            break;

        default:
            printf("Invalid choice\n");
        }
    } while (choice != 0);

    queue_destroy(queue);
    return 0;
}

/*
==========================================
상세 설명 및 주요 개념
==========================================

1. 원형 큐의 특징
--------------
- 배열의 순환적 사용
- front와 rear 포인터 관리
- 효율적인 메모리 활용
- 동적 크기 조정 가능

2. 주요 구현 특징
--------------
CircularQueue 구조체:
- elements: 데이터 저장 배열
- capacity: 최대 용량
- front: 첫 요소 위치
- rear: 다음 삽입 위치
- size: 현재 요소 수

3. 핵심 연산
----------
기본 연산:
- Enqueue: O(1)
- Dequeue: O(1)
- Peek: O(1)

보조 연산:
- IsEmpty: O(1)
- IsFull: O(1)
- Clear: O(1)
- Resize: O(n)

4. 메모리 관리 전략
----------------
- 동적 배열 사용
- 자동 크기 조정
- 순환적 인덱스 관리
- 메모리 효율성 고려

5. 인덱스 관리
-----------
- 모듈로 연산 사용
- front와 rear 순환
- 배열 경계 처리
- 요소 수 추적

6. 안전성 고려사항
---------------
- 메모리 누수 방지
- NULL 포인터 검사
- 경계 조건 처리
- 크기 조정 실패 처리

7. 최적화 특징
-----------
- 상수 시간 연산
- 효율적인 메모리 사용
- 캐시 친화적 구조
- 최소한의 메모리 이동

8. 확장성
-------
- 동적 크기
*/
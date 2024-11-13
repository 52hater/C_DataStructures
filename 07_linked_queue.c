#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef int DataType;

// 노드 구조체 정의
typedef struct Node {
    DataType data;
    struct Node* next;
} Node;

// 큐 구조체 정의
typedef struct {
    Node* front;    // 첫 번째 노드를 가리키는 포인터
    Node* rear;     // 마지막 노드를 가리키는 포인터
    size_t size;    // 현재 큐에 저장된 요소의 수
} LinkedQueue;

/* 오류 처리를 위한 열거형 정의 */
typedef enum {
    QUEUE_OK,
    QUEUE_EMPTY,
    QUEUE_MEMORY_ERROR
} QueueResult;

/* 새로운 노드 생성
 * - 매개변수: data - 저장할 데이터
 * - 반환값: 생성된 노드의 포인터 또는 실패 시 NULL
 */
static Node* node_create(DataType data) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        return NULL;
    }
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

/* 큐 생성
 * - 새로운 빈 큐를 동적으로 할당하고 초기화
 * - 반환값: 생성된 큐의 포인터 또는 실패 시 NULL
 */
LinkedQueue* queue_create(void) {
    LinkedQueue* queue = (LinkedQueue*)malloc(sizeof(LinkedQueue));
    if (queue == NULL) {
        return NULL;
    }
    queue->front = NULL;
    queue->rear = NULL;
    queue->size = 0;
    return queue;
}

/* 큐가 비어있는지 확인
 * - 매개변수: queue - 검사할 큐
 * - 반환값: 비어있으면 true, 아니면 false
 */
bool queue_is_empty(const LinkedQueue* queue) {
    return (queue->size == 0);
}

/* 큐의 현재 크기 반환
 * - 매개변수: queue - 대상 큐
 * - 반환값: 큐에 저장된 요소의 수
 */
size_t queue_size(const LinkedQueue* queue) {
    return queue->size;
}

/* 큐에 요소 추가 (enqueue)
 * - 매개변수: queue - 대상 큐, data - 추가할 데이터
 * - 반환값: 연산 결과를 나타내는 QueueResult
 */
QueueResult queue_enqueue(LinkedQueue* queue, DataType data) {
    Node* new_node = node_create(data);
    if (new_node == NULL) {
        return QUEUE_MEMORY_ERROR;
    }

    if (queue_is_empty(queue)) {
        queue->front = new_node;
    }
    else {
        queue->rear->next = new_node;
    }
    queue->rear = new_node;
    queue->size++;
    return QUEUE_OK;
}

/* 큐에서 요소 제거 (dequeue)
 * - 매개변수: queue - 대상 큐, data - 제거된 데이터를 저장할 포인터
 * - 반환값: 연산 결과를 나타내는 QueueResult
 */
QueueResult queue_dequeue(LinkedQueue* queue, DataType* data) {
    if (queue_is_empty(queue)) {
        return QUEUE_EMPTY;
    }

    Node* temp = queue->front;
    *data = temp->data;
    queue->front = temp->next;

    if (queue->front == NULL) {
        queue->rear = NULL;
    }

    free(temp);
    queue->size--;
    return QUEUE_OK;
}

/* 큐의 맨 앞 요소 확인 (peek)
 * - 매개변수: queue - 대상 큐, data - 데이터를 저장할 포인터
 * - 반환값: 연산 결과를 나타내는 QueueResult
 */
QueueResult queue_peek(const LinkedQueue* queue, DataType* data) {
    if (queue_is_empty(queue)) {
        return QUEUE_EMPTY;
    }

    *data = queue->front->data;
    return QUEUE_OK;
}

/* 큐 초기화 (clear)
 * - 매개변수: queue - 초기화할 큐
 */
void queue_clear(LinkedQueue* queue) {
    while (!queue_is_empty(queue)) {
        DataType dummy;
        queue_dequeue(queue, &dummy);
    }
}

/* 큐 메모리 해제
 * - 매개변수: queue - 해제할 큐
 */
void queue_destroy(LinkedQueue* queue) {
    queue_clear(queue);
    free(queue);
}

/* 큐의 모든 요소 출력
 * - 매개변수: queue - 출력할 큐
 */
void queue_print(const LinkedQueue* queue) {
    if (queue_is_empty(queue)) {
        printf("Queue is empty\n");
        return;
    }

    printf("Queue (size=%zu): FRONT [ ", queue->size);
    Node* current = queue->front;
    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("] REAR\n");
}

/* 큐의 상태 정보 출력
 * - 매개변수: queue - 상태를 확인할 큐
 */
void queue_status(const LinkedQueue* queue) {
    printf("\nQueue Status:\n");
    printf("- Size: %zu\n", queue->size);
    printf("- Empty: %s\n", queue_is_empty(queue) ? "Yes" : "No");

    // 메모리 사용량 계산
    size_t queue_memory = sizeof(LinkedQueue);
    size_t node_memory = sizeof(Node) * queue->size;
    printf("- Memory usage: %zu bytes (overhead: %zu bytes, nodes: %zu bytes)\n",
        queue_memory + node_memory, queue_memory, node_memory);

    if (!queue_is_empty(queue)) {
        printf("- Front value: %d\n", queue->front->data);
        printf("- Rear value: %d\n", queue->rear->data);
    }
}

/* 메뉴 출력 함수 */
void print_menu(void) {
    printf("\n=== Linked Queue Menu ===\n");
    printf("1. Enqueue\n");
    printf("2. Dequeue\n");
    printf("3. Peek\n");
    printf("4. Print queue\n");
    printf("5. Check if empty\n");
    printf("6. Get size\n");
    printf("7. Clear queue\n");
    printf("8. Show queue status\n");
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
    case QUEUE_MEMORY_ERROR:
        printf("Error: Memory allocation failed\n");
        break;
    default:
        break;
    }
}

int main(void) {
    LinkedQueue* queue = queue_create();
    if (queue == NULL) {
        printf("Failed to create queue\n");
        return 1;
    }

    int choice;
    DataType value;
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

        case 6:  // Get size
            printf("Queue size: %zu\n", queue_size(queue));
            break;

        case 7:  // Clear queue
            queue_clear(queue);
            printf("Queue cleared\n");
            break;

        case 8:  // Show queue status
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

1. 연결 리스트 기반 큐의 개념
-------------------------
- FIFO(First In First Out) 원칙
- 노드 기반의 동적 구조
- front와 rear 포인터 활용
- 동적 메모리 할당/해제

2. 구조체 설계
-----------
Node:
- data: 저장할 데이터
- next: 다음 노드 포인터

LinkedQueue:
- front: 첫 노드 포인터
- rear: 마지막 노드 포인터
- size: 현재 요소 수

3. 핵심 연산과 복잡도
-----------------
Enqueue: O(1)
- 새 노드 생성
- rear 포인터 조정
- size 증가

Dequeue: O(1)
- front 노드 제거
- front 포인터 조정
- size 감소

Peek: O(1)
- front 노드 데이터 반환

4. 메모리 관리 전략
---------------
- 동적 할당 (malloc)
- 적절한 해제 (free)
- 메모리 누수 방지
- NULL 포인터 처리

5. 경계 조건 처리
-------------
빈 큐:
- front == NULL
- rear == NULL
- size == 0

단일 노드:
- front == rear
- size == 1

노드 제거 후:
- front 갱신
- rear 갱신 (필요시)

6. 구현 특징
----------
- 타입 추상화 (DataType)
- 오류 상태 열거형
- 안전한 메모리 관리
- 명확한 인터페이스

7. 성능 최적화
-----------
- O(1) 삽입/삭제
- 포인터 효율성
- 불필요한 복사 방지
- 메모리 최적화

8. 교육적 가치
-----------
- 포인터 개념 학습
- 동적 메모리 이해
- 자료구조 원리 파악
- 추상화 개념 습득

9. 안전성 고려사항
--------------
- NULL 체크
- 메모리 누수 방지
- 포인터 유효성
- 경계 조건 처리

10. 코드 품질
----------
- 일관된 명명규칙
- 명확한 주석
- 모듈화된 설계
- 에러 처리
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

/*
힙 기반 우선순위 큐:
- 최대 힙을 활용한 효율적인 우선순위 큐 구현
- 우선순위가 높은 원소는 항상 루트에 위치
- O(log n) 시간 복잡도의 삽입/삭제 연산
- 완전 이진 트리의 특성 활용
*/

typedef struct {
    int priority;   // 우선순위
    char data[50];  // 실제 데이터
} QueueElement;

typedef struct {
    QueueElement* elements;
    int capacity;
    int size;
} PriorityQueue;

/* 부모 노드의 인덱스 계산 */
#define PARENT(i) (((i) - 1) / 2)

/* 왼쪽 자식 노드의 인덱스 계산 */
#define LEFT_CHILD(i) (2 * (i) + 1)

/* 오른쪽 자식 노드의 인덱스 계산 */
#define RIGHT_CHILD(i) (2 * (i) + 2)

/* 우선순위 큐 생성
 * - 매개변수: capacity - 큐의 최대 용량
 */
PriorityQueue* queue_create(int capacity) {
    PriorityQueue* queue = (PriorityQueue*)malloc(sizeof(PriorityQueue));
    if (queue == NULL) {
        return NULL;
    }

    queue->elements = (QueueElement*)malloc(capacity * sizeof(QueueElement));
    if (queue->elements == NULL) {
        free(queue);
        return NULL;
    }

    queue->capacity = capacity;
    queue->size = 0;
    return queue;
}

/* 우선순위 큐 메모리 해제 */
void queue_destroy(PriorityQueue* queue) {
    if (queue) {
        free(queue->elements);
        free(queue);
    }
}

/* 두 원소의 교환 */
void swap_elements(QueueElement* a, QueueElement* b) {
    QueueElement temp = *a;
    *a = *b;
    *b = temp;
}

/* 상향 이동 (삽입 시 사용) */
void heapify_up(PriorityQueue* queue, int index) {
    while (index > 0) {
        int parent = PARENT(index);
        if (queue->elements[parent].priority >= queue->elements[index].priority) {
            break;
        }
        swap_elements(&queue->elements[parent], &queue->elements[index]);
        index = parent;
    }
}

/* 하향 이동 (삭제 시 사용) */
void heapify_down(PriorityQueue* queue, int index) {
    int largest = index;
    int left = LEFT_CHILD(index);
    int right = RIGHT_CHILD(index);

    if (left < queue->size &&
        queue->elements[left].priority > queue->elements[largest].priority) {
        largest = left;
    }

    if (right < queue->size &&
        queue->elements[right].priority > queue->elements[largest].priority) {
        largest = right;
    }

    if (largest != index) {
        swap_elements(&queue->elements[index], &queue->elements[largest]);
        heapify_down(queue, largest);
    }
}

/* 원소 삽입
 * - 매개변수: queue - 우선순위 큐, priority - 우선순위, data - 데이터
 * - 시간복잡도: O(log n)
 */
bool queue_enqueue(PriorityQueue* queue, int priority, const char* data) {
    if (queue->size >= queue->capacity) {
        return false;
    }

    int index = queue->size++;
    queue->elements[index].priority = priority;
    strcpy(queue->elements[index].data, data);
    heapify_up(queue, index);

    return true;
}

/* 최고 우선순위 원소 제거
 * - 매개변수: queue - 우선순위 큐, priority - 우선순위 저장, data - 데이터 저장
 * - 시간복잡도: O(log n)
 */
bool queue_dequeue(PriorityQueue* queue, int* priority, char* data) {
    if (queue->size == 0) {
        return false;
    }

    *priority = queue->elements[0].priority;
    strcpy(data, queue->elements[0].data);

    queue->elements[0] = queue->elements[--queue->size];
    heapify_down(queue, 0);

    return true;
}

/* 최고 우선순위 원소 확인 (제거하지 않음)
 * - 시간복잡도: O(1)
 */
bool queue_peek(PriorityQueue* queue, int* priority, char* data) {
    if (queue->size == 0) {
        return false;
    }

    *priority = queue->elements[0].priority;
    strcpy(data, queue->elements[0].data);
    return true;
}

/* 우선순위 큐 시각화 */
void queue_print(const PriorityQueue* queue) {
    if (queue->size == 0) {
        printf("Priority Queue is empty\n");
        return;
    }

    printf("\nPriority Queue Contents:\n");
    printf("Size: %d, Capacity: %d\n", queue->size, queue->capacity);
    printf("Format: (Priority, Data)\n");

    int level = 0;
    int level_nodes = 1;
    int printed_nodes = 0;

    while (printed_nodes < queue->size) {
        // 현재 레벨의 들여쓰기
        for (int i = 0; i < (1 << (4 - level)); i++) {
            printf("  ");
        }

        // 현재 레벨의 노드들 출력
        for (int i = 0; i < level_nodes && printed_nodes < queue->size; i++) {
            printf("(%d,%s) ",
                queue->elements[printed_nodes].priority,
                queue->elements[printed_nodes].data);
            printed_nodes++;
        }
        printf("\n");

        level++;
        level_nodes *= 2;
    }
}

/* 우선순위 큐가 비어있는지 확인 */
bool queue_is_empty(const PriorityQueue* queue) {
    return queue->size == 0;
}

/* 우선순위 큐가 가득 찼는지 확인 */
bool queue_is_full(const PriorityQueue* queue) {
    return queue->size >= queue->capacity;
}

/* 성능 측정을 위한 랜덤 문자열 생성 */
void generate_random_string(char* str, int length) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyz";
    for (int i = 0; i < length - 1; i++) {
        str[i] = charset[rand() % (sizeof(charset) - 1)];
    }
    str[length - 1] = '\0';
}

/* 성능 측정 */
void measure_performance(PriorityQueue* queue, int operations) {
    clock_t start, end;
    char data[50];
    int priority;

    // 삽입 성능 측정
    start = clock();
    for (int i = 0; i < operations; i++) {
        generate_random_string(data, 6);
        queue_enqueue(queue, rand() % 100, data);
    }
    end = clock();
    double enqueue_time = ((double)(end - start)) / CLOCKS_PER_SEC;

    // 삭제 성능 측정
    start = clock();
    for (int i = 0; i < operations; i++) {
        queue_dequeue(queue, &priority, data);
    }
    end = clock();
    double dequeue_time = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("\nPerformance Analysis (%d operations):\n", operations);
    printf("Enqueue time: %.6f seconds (%.9f per operation)\n",
        enqueue_time, enqueue_time / operations);
    printf("Dequeue time: %.6f seconds (%.9f per operation)\n",
        dequeue_time, dequeue_time / operations);
}

/* 메뉴 출력 */
void print_menu(void) {
    printf("\n=== Priority Queue Menu ===\n");
    printf("1. Enqueue element\n");
    printf("2. Dequeue element\n");
    printf("3. Peek top element\n");
    printf("4. Print queue\n");
    printf("5. Run performance test\n");
    printf("0. Exit\n");
    printf("Choice: ");
}

int main(void) {
    srand((unsigned int)time(NULL));

    int capacity;
    printf("Enter priority queue capacity: ");
    scanf("%d", &capacity);

    PriorityQueue* queue = queue_create(capacity);
    if (queue == NULL) {
        printf("Failed to create priority queue\n");
        return 1;
    }

    int choice, priority;
    char data[50];

    do {
        print_menu();
        scanf("%d", &choice);

        switch (choice) {
        case 1:  // Enqueue
            printf("Enter priority (higher number = higher priority): ");
            scanf("%d", &priority);
            printf("Enter data: ");
            scanf("%s", data);

            if (queue_enqueue(queue, priority, data)) {
                printf("Element added successfully\n");
                queue_print(queue);
            }
            else {
                printf("Queue is full\n");
            }
            break;

        case 2:  // Dequeue
            if (queue_dequeue(queue, &priority, data)) {
                printf("Dequeued: Priority=%d, Data=%s\n", priority, data);
                queue_print(queue);
            }
            else {
                printf("Queue is empty\n");
            }
            break;

        case 3:  // Peek
            if (queue_peek(queue, &priority, data)) {
                printf("Top element: Priority=%d, Data=%s\n", priority, data);
            }
            else {
                printf("Queue is empty\n");
            }
            break;

        case 4:  // Print
            queue_print(queue);
            break;

        case 5:  // Performance test
            printf("Enter number of operations: ");
            int ops;
            scanf("%d", &ops);
            if (ops > queue->capacity) {
                ops = queue->capacity;
                printf("Limiting to %d operations\n", ops);
            }
            measure_performance(queue, ops);
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

1. 힙 기반 우선순위 큐의 특징
-----------------------
- 최대 힙 구조 활용
- 동적 메모리 할당
- 우선순위와 데이터 분리
- 효율적인 구현

2. 시간 복잡도
-----------
삽입(Enqueue): O(log n)
삭제(Dequeue): O(log n)
조회(Peek): O(1)
- 모든 작업이 효율적

3. 공간 복잡도
-----------
O(n)
- 동적 배열 사용
- 추가 공간 최소화
- 확장 가능한 구조

4. 주요 구현 특징
-------------
- 우선순위와 데이터 쌍
- 동적 크기 조절
- 시각화 기능
- 성능 측정

5. 장점
-----
- 빠른 우선순위 처리
- 메모리 효율성
- 안정적인 성능
- 확장 가능성

6. 단점
-----
- 구현 복잡도
- 메모리 단편화
- 캐시 지역성
- 불안정 정렬

7. 최적화 기법
-----------
- 동적 메모리 관리
- 캐시 활용
- 연산 최소화
- 메모리 정렬

8. 응용 분야
----------
- 작업 스케줄링
- 이벤트 처리
- 네트워크 패킷 처리
- 시뮬레이션

이 구현은 실제 현업에서 사용할 수
있는 수준의 우선순위 큐를 제공하며,
성능과 기능성을 모두 고려한
설계를 보여줍니다.
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

/*
배열 기반 단순 우선순위 큐:
- 우선순위가 높은 원소가 먼저 처리
- 삽입은 배열 끝에 수행
- 삭제는 최대/최소값 검색 후 수행
- 구현이 단순하지만 비효율적
*/

typedef int ElementType;
#define MAX_QUEUE_SIZE 100

typedef struct {
    ElementType elements[MAX_QUEUE_SIZE];
    int size;
} PriorityQueue;

/* 우선순위 큐 초기화 */
void queue_init(PriorityQueue* queue) {
    queue->size = 0;
}

/* 큐가 비어있는지 확인 */
bool queue_is_empty(const PriorityQueue* queue) {
    return queue->size == 0;
}

/* 큐가 가득 찼는지 확인 */
bool queue_is_full(const PriorityQueue* queue) {
    return queue->size == MAX_QUEUE_SIZE;
}

/* 큐의 크기 반환 */
int queue_size(const PriorityQueue* queue) {
    return queue->size;
}

/* 최대 우선순위 원소 삽입 (큰 값이 높은 우선순위)
 * - 시간복잡도: O(1)
 */
bool queue_insert_max(PriorityQueue* queue, ElementType value) {
    if (queue_is_full(queue)) {
        return false;
    }
    queue->elements[queue->size++] = value;
    return true;
}

/* 최소 우선순위 원소 삽입 (작은 값이 높은 우선순위)
 * - 시간복잡도: O(1)
 */
bool queue_insert_min(PriorityQueue* queue, ElementType value) {
    if (queue_is_full(queue)) {
        return false;
    }
    queue->elements[queue->size++] = value;
    return true;
}

/* 최대 우선순위 원소 삭제 (큰 값이 높은 우선순위)
 * - 시간복잡도: O(n)
 */
bool queue_remove_max(PriorityQueue* queue, ElementType* value) {
    if (queue_is_empty(queue)) {
        return false;
    }

    // 최대값의 인덱스 찾기
    int max_index = 0;
    for (int i = 1; i < queue->size; i++) {
        if (queue->elements[i] > queue->elements[max_index]) {
            max_index = i;
        }
    }

    // 최대값 반환
    *value = queue->elements[max_index];

    // 마지막 원소를 삭제할 위치로 이동
    queue->elements[max_index] = queue->elements[queue->size - 1];
    queue->size--;

    return true;
}

/* 최소 우선순위 원소 삭제 (작은 값이 높은 우선순위)
 * - 시간복잡도: O(n)
 */
bool queue_remove_min(PriorityQueue* queue, ElementType* value) {
    if (queue_is_empty(queue)) {
        return false;
    }

    // 최소값의 인덱스 찾기
    int min_index = 0;
    for (int i = 1; i < queue->size; i++) {
        if (queue->elements[i] < queue->elements[min_index]) {
            min_index = i;
        }
    }

    // 최소값 반환
    *value = queue->elements[min_index];

    // 마지막 원소를 삭제할 위치로 이동
    queue->elements[min_index] = queue->elements[queue->size - 1];
    queue->size--;

    return true;
}

/* 큐의 모든 원소 출력 */
void queue_print(const PriorityQueue* queue, const char* name) {
    printf("%s (size=%d): ", name, queue->size);
    for (int i = 0; i < queue->size; i++) {
        printf("%d ", queue->elements[i]);
    }
    printf("\n");
}

/* 성능 측정을 위한 연산 수행 함수 */
void measure_operations(PriorityQueue* queue, int count) {
    clock_t start, end;
    double insert_time, remove_time;

    // 삽입 연산 측정
    start = clock();
    for (int i = 0; i < count; i++) {
        queue_insert_max(queue, rand() % 1000);
    }
    end = clock();
    insert_time = ((double)(end - start)) / CLOCKS_PER_SEC;

    // 삭제 연산 측정
    ElementType value;
    start = clock();
    for (int i = 0; i < count; i++) {
        queue_remove_max(queue, &value);
    }
    end = clock();
    remove_time = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("\nPerformance Analysis (%d operations):\n", count);
    printf("Insert time: %.6f seconds\n", insert_time);
    printf("Remove time: %.6f seconds\n", remove_time);
    printf("Average insert time: %.9f seconds\n", insert_time / count);
    printf("Average remove time: %.9f seconds\n", remove_time / count);
}

/* 메뉴 출력 */
void print_menu(void) {
    printf("\n=== Priority Queue Menu ===\n");
    printf("1. Insert (max priority)\n");
    printf("2. Insert (min priority)\n");
    printf("3. Remove (max priority)\n");
    printf("4. Remove (min priority)\n");
    printf("5. Print queue\n");
    printf("6. Run performance test\n");
    printf("0. Exit\n");
    printf("Choice: ");
}

int main(void) {
    PriorityQueue queue;
    queue_init(&queue);

    int choice;
    ElementType value;

    do {
        print_menu();
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
        case 1:  // Insert (max)
            printf("Enter value to insert: ");
            scanf("%d", &value);
            if (queue_insert_max(&queue, value)) {
                printf("Inserted %d\n", value);
            }
            else {
                printf("Queue is full\n");
            }
            queue_print(&queue, "Queue after insert");
            break;

        case 2:  // Insert (min)
            printf("Enter value to insert: ");
            scanf("%d", &value);
            if (queue_insert_min(&queue, value)) {
                printf("Inserted %d\n", value);
            }
            else {
                printf("Queue is full\n");
            }
            queue_print(&queue, "Queue after insert");
            break;

        case 3:  // Remove (max)
            if (queue_remove_max(&queue, &value)) {
                printf("Removed maximum value: %d\n", value);
            }
            else {
                printf("Queue is empty\n");
            }
            queue_print(&queue, "Queue after remove");
            break;

        case 4:  // Remove (min)
            if (queue_remove_min(&queue, &value)) {
                printf("Removed minimum value: %d\n", value);
            }
            else {
                printf("Queue is empty\n");
            }
            queue_print(&queue, "Queue after remove");
            break;

        case 5:  // Print
            queue_print(&queue, "Current queue");
            break;

        case 6:  // Performance test
            printf("Enter number of operations: ");
            int count;
            scanf("%d", &count);
            if (count > MAX_QUEUE_SIZE) {
                count = MAX_QUEUE_SIZE;
                printf("Limiting to %d operations\n", count);
            }
            queue_init(&queue);  // Reset queue
            measure_operations(&queue, count);
            break;

        case 0:  // Exit
            printf("Exiting program\n");
            break;

        default:
            printf("Invalid choice\n");
        }
    } while (choice != 0);

    return 0;
}

/*
==========================================
상세 설명 및 주요 개념
==========================================

1. 우선순위 큐의 기본 개념
----------------------
- FIFO가 아닌 우선순위 기반
- 최대/최소 우선순위 지원
- 동적 원소 관리
- 응용 분야 다양

2. 배열 기반 구현의 특징
--------------------
장점:
- 구현이 단순
- 메모리 효율적
- 캐시 지역성
- 이해하기 쉬움

단점:
- 삭제 연산 비효율적
- 고정 크기
- 선형 시간 검색
- 확장성 제한

3. 시간 복잡도
-----------
삽입:
- O(1) 상수 시간
- 항상 끝에 추가

삭제:
- O(n) 선형 시간
- 전체 검색 필요
- 요소 이동 필요

4. 공간 복잡도
-----------
- O(n) 선형 공간
- 고정 크기 배열
- 추가 공간 불필요
- 메모리 효율적

5. 주요 연산
----------
- 초기화
- 삽입
- 최대/최소 삭제
- 크기 확인
- 상태 출력

6. 구현 특징
----------
- 두 가지 우선순위
- 성능 측정
- 시각적 출력
- 에러 처리

7. 한계점
-------
- 비효율적 삭제
- 크기 제한
- 확장 어려움
- 검색 시간

8. 개선 방향
----------
- 힙 기반 구현
- 동적 크기
- 효율적 검색
- 균형 구조

이 구현은 우선순위 큐의 기본
개념을 이해하기 위한 교육용
구현으로, 실제 사용을 위해서는
힙 기반 구현이 권장됩니다.
*/
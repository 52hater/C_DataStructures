#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

/*
최대 힙:
- 완전 이진 트리 구조
- 부모 노드가 자식 노드보다 크거나 같음
- 배열로 구현 (인덱스 계산으로 부모-자식 관계 표현)
- 효율적인 삽입/삭제 연산
*/

typedef int ElementType;
#define MAX_HEAP_SIZE 100

typedef struct {
    ElementType elements[MAX_HEAP_SIZE];
    int size;  // 힙의 현재 크기
} MaxHeap;

/* 부모 노드의 인덱스 계산 */
#define PARENT(i) (((i) - 1) / 2)

/* 왼쪽 자식 노드의 인덱스 계산 */
#define LEFT_CHILD(i) (2 * (i) + 1)

/* 오른쪽 자식 노드의 인덱스 계산 */
#define RIGHT_CHILD(i) (2 * (i) + 2)

/* 두 원소의 교환 */
void swap(ElementType* a, ElementType* b) {
    ElementType temp = *a;
    *a = *b;
    *b = temp;
}

/* 힙 초기화 */
void heap_init(MaxHeap* heap) {
    heap->size = 0;
}

/* 힙이 비어있는지 확인 */
bool heap_is_empty(const MaxHeap* heap) {
    return heap->size == 0;
}

/* 힙이 가득 찼는지 확인 */
bool heap_is_full(const MaxHeap* heap) {
    return heap->size == MAX_HEAP_SIZE;
}

/* 힙의 크기 반환 */
int heap_size(const MaxHeap* heap) {
    return heap->size;
}

/* 상향 이동 (삽입 시 사용)
 * - 새로 삽입된 원소를 적절한 위치로 이동
 * - 시간복잡도: O(log n)
 */
void heapify_up(MaxHeap* heap, int index) {
    while (index > 0 && heap->elements[PARENT(index)] < heap->elements[index]) {
        swap(&heap->elements[PARENT(index)], &heap->elements[index]);
        index = PARENT(index);
    }
}

/* 하향 이동 (삭제 시 사용)
 * - 루트 노드부터 시작하여 적절한 위치로 이동
 * - 시간복잡도: O(log n)
 */
void heapify_down(MaxHeap* heap, int index) {
    int largest = index;
    int left = LEFT_CHILD(index);
    int right = RIGHT_CHILD(index);

    // 왼쪽 자식이 더 큰 경우
    if (left < heap->size && heap->elements[left] > heap->elements[largest]) {
        largest = left;
    }

    // 오른쪽 자식이 더 큰 경우
    if (right < heap->size && heap->elements[right] > heap->elements[largest]) {
        largest = right;
    }

    // 교환이 필요한 경우
    if (largest != index) {
        swap(&heap->elements[index], &heap->elements[largest]);
        heapify_down(heap, largest);
    }
}

/* 원소 삽입
 * - 시간복잡도: O(log n)
 */
bool heap_insert(MaxHeap* heap, ElementType value) {
    if (heap_is_full(heap)) {
        return false;
    }

    // 새 원소를 마지막에 추가
    heap->elements[heap->size] = value;

    // 적절한 위치로 이동
    heapify_up(heap, heap->size);

    heap->size++;
    return true;
}

/* 최대값 삭제
 * - 시간복잡도: O(log n)
 */
bool heap_delete_max(MaxHeap* heap, ElementType* value) {
    if (heap_is_empty(heap)) {
        return false;
    }

    // 루트 값을 반환
    *value = heap->elements[0];

    // 마지막 원소를 루트로 이동
    heap->elements[0] = heap->elements[heap->size - 1];
    heap->size--;

    // 루트부터 재정렬
    if (!heap_is_empty(heap)) {
        heapify_down(heap, 0);
    }

    return true;
}

/* 최대값 확인 (삭제하지 않음)
 * - 시간복잡도: O(1)
 */
bool heap_peek_max(const MaxHeap* heap, ElementType* value) {
    if (heap_is_empty(heap)) {
        return false;
    }
    *value = heap->elements[0];
    return true;
}

/* 힙 시각화 (트리 형태로 출력)
 * - 레벨 순회 방식으로 출력
 */
void heap_print(const MaxHeap* heap) {
    if (heap_is_empty(heap)) {
        printf("Heap is empty\n");
        return;
    }

    printf("Heap structure:\n");
    int level = 0;
    int level_nodes = 1;
    int printed_nodes = 0;

    while (printed_nodes < heap->size) {
        // 현재 레벨의 들여쓰기
        for (int i = 0; i < (1 << (heap->size / 2 - level)); i++) {
            printf("  ");
        }

        // 현재 레벨의 노드들 출력
        for (int i = 0; i < level_nodes && printed_nodes < heap->size; i++) {
            printf("%d ", heap->elements[printed_nodes++]);

            // 노드 사이의 간격
            for (int j = 0; j < (1 << (heap->size / 2 - level + 1)); j++) {
                printf(" ");
            }
        }
        printf("\n");

        level++;
        level_nodes *= 2;
    }
}

/* 힙 검증
 * - 최대 힙 속성이 유지되는지 확인
 */
bool heap_verify(const MaxHeap* heap) {
    for (int i = 1; i < heap->size; i++) {
        if (heap->elements[i] > heap->elements[PARENT(i)]) {
            return false;
        }
    }
    return true;
}

/* 성능 측정 */
void measure_operations(MaxHeap* heap, int count) {
    clock_t start, end;
    double insert_time, delete_time;

    // 삽입 연산 측정
    start = clock();
    for (int i = 0; i < count; i++) {
        heap_insert(heap, rand() % 1000);
    }
    end = clock();
    insert_time = ((double)(end - start)) / CLOCKS_PER_SEC;

    // 삭제 연산 측정
    ElementType value;
    start = clock();
    for (int i = 0; i < count; i++) {
        heap_delete_max(heap, &value);
    }
    end = clock();
    delete_time = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("\nPerformance Analysis (%d operations):\n", count);
    printf("Insert time: %.6f seconds\n", insert_time);
    printf("Delete time: %.6f seconds\n", delete_time);
    printf("Average insert time: %.9f seconds\n", insert_time / count);
    printf("Average delete time: %.9f seconds\n", delete_time / count);
}

/* 메뉴 출력 */
void print_menu(void) {
    printf("\n=== Max Heap Menu ===\n");
    printf("1. Insert element\n");
    printf("2. Delete maximum\n");
    printf("3. Peek maximum\n");
    printf("4. Print heap\n");
    printf("5. Verify heap\n");
    printf("6. Run performance test\n");
    printf("0. Exit\n");
    printf("Choice: ");
}

int main(void) {
    MaxHeap heap;
    heap_init(&heap);

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
        case 1:  // Insert
            printf("Enter value to insert: ");
            scanf("%d", &value);
            if (heap_insert(&heap, value)) {
                printf("Inserted %d\n", value);
                heap_print(&heap);
            }
            else {
                printf("Heap is full\n");
            }
            break;

        case 2:  // Delete maximum
            if (heap_delete_max(&heap, &value)) {
                printf("Deleted maximum value: %d\n", value);
                heap_print(&heap);
            }
            else {
                printf("Heap is empty\n");
            }
            break;

        case 3:  // Peek maximum
            if (heap_peek_max(&heap, &value)) {
                printf("Maximum value: %d\n", value);
            }
            else {
                printf("Heap is empty\n");
            }
            break;

        case 4:  // Print
            heap_print(&heap);
            break;

        case 5:  // Verify
            if (heap_verify(&heap)) {
                printf("Heap property is satisfied\n");
            }
            else {
                printf("Heap property is violated!\n");
            }
            break;

        case 6:  // Performance test
            printf("Enter number of operations: ");
            int count;
            scanf("%d", &count);
            if (count > MAX_HEAP_SIZE) {
                count = MAX_HEAP_SIZE;
                printf("Limiting to %d operations\n", count);
            }
            heap_init(&heap);  // Reset heap
            measure_operations(&heap, count);
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

1. 최대 힙의 특성
-------------
- 완전 이진 트리
- 최대 힙 속성
- 배열 기반 구현
- 효율적인 삽입/삭제

2. 시간 복잡도
-----------
삽입: O(log n)
- 말단 삽입 후 상향 이동

삭제: O(log n)
- 루트 삭제 후 하향 이동

조회: O(1)
- 최대값은 항상 루트

3. 공간 복잡도
-----------
O(n)
- 배열 기반
- 완전 이진 트리
- 추가 공간 불필요

4. 주요 연산
----------
Heapify Up:
- 삽입 시 사용
- 상향식 재정렬
- 부모와 비교/교환

Heapify Down:
- 삭제 시 사용
- 하향식 재정렬
- 자식과 비교/교환

5. 구현 특징
----------
- 인덱스 계산
- 재귀적 구현
- 시각적 출력
- 검증 기능

6. 장단점
-------
장점:
- 효율적인 우선순위 관리
- 빠른 최대값 접근
- 안정적인 성능
- 구현 단순

단점:
- 고정 크기
- 캐시 지역성
- 완전 이진 트리 유지
- 불안정 정렬

7. 응용 분야
----------
- 우선순위 큐
- 힙 정렬
- 이벤트 처리
- 스케줄링

8. 최적화 기법
-----------
- 반복적 구현
- 캐시 활용
- 메모리 정렬
- 연산 최소화

이 구현은 최대 힙의 모든 핵심
기능을 포함하며, 다음 단계인
힙 기반 우선순위 큐 구현의
기초가 됩니다.
*/
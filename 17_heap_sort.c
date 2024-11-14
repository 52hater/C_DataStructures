#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

typedef int DataType;

/* 두 원소의 교환
 * - 매개변수: a, b - 교환할 두 원소의 포인터
 */
void swap(DataType* a, DataType* b) {
    DataType temp = *a;
    *a = *b;
    *b = temp;
}

/* 최대 힙 속성 유지 (재귀 버전)
 * - 매개변수: arr - 힙 배열, size - 힙 크기, index - 현재 노드
 */
void heapify_recursive(DataType arr[], size_t size, size_t index) {
    size_t largest = index;       // 최대값 위치
    size_t left = 2 * index + 1;  // 왼쪽 자식
    size_t right = 2 * index + 2; // 오른쪽 자식

    // 왼쪽 자식이 더 크면 largest 갱신
    if (left < size && arr[left] > arr[largest]) {
        largest = left;
    }

    // 오른쪽 자식이 더 크면 largest 갱신
    if (right < size && arr[right] > arr[largest]) {
        largest = right;
    }

    // largest가 변경되었으면 교환하고 재귀 호출
    if (largest != index) {
        swap(&arr[index], &arr[largest]);
        heapify_recursive(arr, size, largest);
    }
}

/* 최대 힙 속성 유지 (반복 버전)
 * - 매개변수: arr - 힙 배열, size - 힙 크기, index - 현재 노드
 */
void heapify_iterative(DataType arr[], size_t size, size_t index) {
    size_t current = index;

    while (true) {
        size_t largest = current;
        size_t left = 2 * current + 1;
        size_t right = 2 * current + 2;

        if (left < size && arr[left] > arr[largest]) {
            largest = left;
        }
        if (right < size && arr[right] > arr[largest]) {
            largest = right;
        }

        if (largest == current) {
            break;
        }

        swap(&arr[current], &arr[largest]);
        current = largest;
    }
}

/* 힙 생성 (상향식)
 * - 매개변수: arr - 배열, size - 배열 크기
 * - 설명: 마지막 비단말 노드부터 역순으로 heapify
 */
void build_heap_bottom_up(DataType arr[], size_t size,
    void (*heapify)(DataType[], size_t, size_t)) {
    // 마지막 비단말 노드부터 역순으로 heapify 수행
    for (int i = (size / 2) - 1; i >= 0; i--) {
        heapify(arr, size, i);
    }
}

/* 기본 힙 정렬
 * - 매개변수: arr - 정렬할 배열, size - 배열 크기
 */
void heap_sort_basic(DataType arr[], size_t size) {
    // 최대 힙 생성
    build_heap_bottom_up(arr, size, heapify_recursive);

    // 힙에서 하나씩 추출하며 정렬
    for (size_t i = size - 1; i > 0; i--) {
        swap(&arr[0], &arr[i]);  // 최대값을 끝으로 이동
        heapify_recursive(arr, i, 0);  // 힙 크기를 줄이고 heapify
    }
}

/* 최적화된 힙 정렬 (반복적 heapify 사용)
 * - 매개변수: arr - 정렬할 배열, size - 배열 크기
 */
void heap_sort_optimized(DataType arr[], size_t size) {
    build_heap_bottom_up(arr, size, heapify_iterative);

    for (size_t i = size - 1; i > 0; i--) {
        swap(&arr[0], &arr[i]);
        heapify_iterative(arr, i, 0);
    }
}

/* 배열 출력
 * - 매개변수: arr - 출력할 배열, size - 배열의 크기
 */
void print_array(const DataType arr[], size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

/* 힙 구조 시각화
 * - 매개변수: arr - 힙 배열, size - 힙 크기, index - 현재 노드
 *            level - 현재 레벨, prefix - 출력 접두사
 */
void print_heap_recursive(const DataType arr[], size_t size, size_t index,
    int level, char* prefix) {
    if (index >= size) {
        return;
    }

    printf("%s", prefix);
    printf("%s", level ? "├── " : "");
    printf("%d\n", arr[index]);

    char new_prefix[256];
    sprintf(new_prefix, "%s%s", prefix, level ? "│   " : "");

    print_heap_recursive(arr, size, 2 * index + 1, 1, new_prefix); // 왼쪽 자식
    print_heap_recursive(arr, size, 2 * index + 2, 1, new_prefix); // 오른쪽 자식
}

/* 힙 구조 출력
 * - 매개변수: arr - 힙 배열, size - 힙 크기
 */
void print_heap(const DataType arr[], size_t size) {
    printf("Heap Structure:\n");
    print_heap_recursive(arr, size, 0, 0, "");
}

/* 배열이 정렬되었는지 검증
 * - 매개변수: arr - 검증할 배열, size - 배열의 크기
 * - 반환값: 정렬되었으면 true, 아니면 false
 */
bool is_sorted(const DataType arr[], size_t size) {
    for (size_t i = 0; i < size - 1; i++) {
        if (arr[i] > arr[i + 1]) {
            return false;
        }
    }
    return true;
}

/* 배열이 힙 속성을 만족하는지 검증
 * - 매개변수: arr - 검증할 배열, size - 배열의 크기
 * - 반환값: 힙 속성을 만족하면 true, 아니면 false
 */
bool is_heap(const DataType arr[], size_t size) {
    for (size_t i = 0; i < size / 2; i++) {
        size_t left = 2 * i + 1;
        size_t right = 2 * i + 2;

        if (left < size && arr[i] < arr[left]) {
            return false;
        }
        if (right < size && arr[i] < arr[right]) {
            return false;
        }
    }
    return true;
}

/* 배열 복사
 * - 매개변수: dest - 목적지 배열, src - 원본 배열, size - 배열의 크기
 */
void copy_array(DataType dest[], const DataType src[], size_t size) {
    for (size_t i = 0; i < size; i++) {
        dest[i] = src[i];
    }
}

/* 알고리즘 실행 시간 측정
 * - 매개변수: sort_func - 정렬 함수 포인터, arr - 정렬할 배열,
 *            size - 배열의 크기, name - 알고리즘 이름
 */
void measure_time(void (*sort_func)(DataType[], size_t),
    DataType arr[], size_t size, const char* name) {
    clock_t start = clock();
    sort_func(arr, size);
    clock_t end = clock();

    double cpu_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("%s: %.6f seconds\n", name, cpu_time);
}

/* 새로운 무작위 배열 생성
 * - 매개변수: arr - 생성할 배열, size - 배열의 크기
 */
void generate_random_array(DataType arr[], size_t size) {
    for (size_t i = 0; i < size; i++) {
        arr[i] = rand() % 1000;  // 0-999 범위의 무작위 값
    }
}

/* 메뉴 출력 */
void print_menu(void) {
    printf("\n=== Heap Sort Menu ===\n");
    printf("1. Run basic heap sort\n");
    printf("2. Run optimized heap sort\n");
    printf("3. Compare both versions\n");
    printf("4. Show heap structure\n");
    printf("5. Generate new random array\n");
    printf("6. Verify heap property\n");
    printf("0. Exit\n");
    printf("Choice: ");
}

int main(void) {
    srand((unsigned int)time(NULL));  // 난수 발생기 초기화

    size_t size;
    printf("Enter array size: ");
    if (scanf("%zu", &size) != 1 || size == 0) {
        printf("Invalid size\n");
        return 1;
    }

    DataType* arr = (DataType*)malloc(size * sizeof(DataType));
    DataType* temp_arr = (DataType*)malloc(size * sizeof(DataType));
    if (arr == NULL || temp_arr == NULL) {
        printf("Memory allocation failed\n");
        free(arr);
        free(temp_arr);
        return 1;
    }

    generate_random_array(arr, size);
    int choice;

    do {
        print_menu();
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
        case 1:  // Basic heap sort
            copy_array(temp_arr, arr, size);
            printf("\nArray before sorting:\n");
            print_array(temp_arr, size);

            measure_time(heap_sort_basic, temp_arr, size,
                "Basic heap sort");

            printf("Array after sorting:\n");
            print_array(temp_arr, size);
            printf("Verification: %s\n",
                is_sorted(temp_arr, size) ? "PASSED" : "FAILED");
            break;

        case 2:  // Optimized heap sort
            copy_array(temp_arr, arr, size);
            printf("\nArray before sorting:\n");
            print_array(temp_arr, size);

            measure_time(heap_sort_optimized, temp_arr, size,
                "Optimized heap sort");

            printf("Array after sorting:\n");
            print_array(temp_arr, size);
            printf("Verification: %s\n",
                is_sorted(temp_arr, size) ? "PASSED" : "FAILED");
            break;

        case 3:  // Compare both versions
            printf("\nComparing both versions with the same input:\n");

            // 기본 힙 정렬
            copy_array(temp_arr, arr, size);
            measure_time(heap_sort_basic, temp_arr, size,
                "Basic heap sort");

            // 최적화된 힙 정렬
            copy_array(temp_arr, arr, size);
            measure_time(heap_sort_optimized, temp_arr, size,
                "Optimized heap sort");
            break;

        case 4:  // Show heap structure
            copy_array(temp_arr, arr, size);
            build_heap_bottom_up(temp_arr, size, heapify_recursive);
            print_heap(temp_arr, size);
            break;

        case 5:  // Generate random array
            generate_random_array(arr, size);
            printf("New random array generated\n");
            printf("Array contents:\n");
            print_array(arr, size);
            break;

        case 6:  // Verify heap property
            copy_array(temp_arr, arr, size);
            build_heap_bottom_up(temp_arr, size, heapify_recursive);
            printf("Heap property check: %s\n",
                is_heap(temp_arr, size) ? "Valid" : "Invalid");
            break;

        case 0:  // Exit
            printf("Exiting program\n");
            break;

        default:
            printf("Invalid choice\n");
        }
    } while (choice != 0);

    free(arr);
    free(temp_arr);
    return 0;
}

/*
==========================================
상세 설명 및 주요 개념
==========================================

1. 힙 정렬의 기본 원리
-----------------
- 최대 힙 구성
- 루트 추출
- 힙 재구성
- 제자리 정렬

2. 시간 복잡도
-----------
힙 생성: O(n)
정렬: O(n log n)
전체: O(n log n)
- 모든 경우 동일
- 입력에 독립적

3. 공간 복잡도
-----------
O(1)
- 추가 메모리 불필요
- 제자리 정렬
- 스택 공간만 사용

4. 구현 방식
----------
재귀적 구현:
- 직관적 이해
- 콜 스택 사용
- 간단한 구현

반복적 구현:
- 스택 오버헤드 없음
- 성능 향상
- 복잡한 구현

5. 최적화 기법
-----------
반복적 heapify:
- 스택 오버헤드 제거
- 캐시 지역성 향상
- 함수 호출 감소

힙 생성 최적화:
- 상향식 접근
- 불필요한 비교 감소
- 효율적인 구성

6. 장단점
-------
장점:
- 최적 성능 보장
- 제자리 정렬
- 안정적 성능
- 우선순위 큐 응용

단점:
- 불안정 정렬
- 캐시 지역성 낮음
- 실제 구현 복잡
- 분기 예측 어려움
- 연속 메모리 접근 불규칙

7. 응용 분야
----------
- 우선순위 큐 구현
- K개 최대/최소값 찾기
- 시스템 프로그래밍
- 실시간 스케줄링

8. 구현 특징
----------
- 재귀/반복 두 가지 방식
- 힙 속성 검증
- 시각적 표현
- 성능 측정

9. 개선 가능성
-----------
- 병렬화
- 캐시 최적화
- 분기 예측 개선
- 메모리 접근 패턴 최적화

10. 교육적 가치
------------
- 완전 이진 트리 이해
- 힙 자료구조 학습
- 재귀/반복 비교
- 최적화 기법 학습

이 구현은 힙 정렬의 기본 개념과
다양한 최적화 기법을 포함하며,
힙 구조의 시각화 기능도 제공하는
교육용 프로그램입니다.
*/
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

/* 선택 정렬
 * - 매개변수: arr - 정렬할 배열, size - 배열의 크기
 * - 설명: 각 패스마다 최소값을 찾아 맨 앞으로 이동
 */
void selection_sort(DataType arr[], size_t size) {
    for (size_t i = 0; i < size - 1; i++) {
        size_t min_idx = i;

        // i부터 끝까지 최소값 찾기
        for (size_t j = i + 1; j < size; j++) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }

        // 최소값이 현재 위치가 아니면 교환
        if (min_idx != i) {
            swap(&arr[i], &arr[min_idx]);
        }
    }
}

/* 최적화된 선택 정렬
 * - 매개변수: arr - 정렬할 배열, size - 배열의 크기
 * - 설명: 각 패스마다 최소값과 최대값을 동시에 찾아 양쪽에서 정렬
 */
void selection_sort_optimized(DataType arr[], size_t size) {
    for (size_t i = 0; i < size / 2; i++) {
        size_t min_idx = i;
        size_t max_idx = i;

        // 최소값과 최대값을 동시에 찾기
        for (size_t j = i + 1; j < size - i; j++) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
            if (arr[j] > arr[max_idx]) {
                max_idx = j;
            }
        }

        // 최소값을 앞으로
        if (min_idx != i) {
            swap(&arr[i], &arr[min_idx]);
            // 최대값이 처음 위치에 있었다면 인덱스 조정
            if (max_idx == i) {
                max_idx = min_idx;
            }
        }

        // 최대값을 뒤로
        if (max_idx != size - 1 - i) {
            swap(&arr[size - 1 - i], &arr[max_idx]);
        }
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
    printf("\n=== Selection Sort Menu ===\n");
    printf("1. Run basic selection sort\n");
    printf("2. Run optimized selection sort\n");
    printf("3. Compare both versions\n");
    printf("4. Generate new random array\n");
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
        case 1:  // Basic selection sort
            copy_array(temp_arr, arr, size);
            printf("\nArray before sorting:\n");
            print_array(temp_arr, size);

            measure_time(selection_sort, temp_arr, size, "Basic selection sort");

            printf("Array after sorting:\n");
            print_array(temp_arr, size);
            printf("Verification: %s\n",
                is_sorted(temp_arr, size) ? "PASSED" : "FAILED");
            break;

        case 2:  // Optimized selection sort
            copy_array(temp_arr, arr, size);
            printf("\nArray before sorting:\n");
            print_array(temp_arr, size);

            measure_time(selection_sort_optimized, temp_arr, size,
                "Optimized selection sort");

            printf("Array after sorting:\n");
            print_array(temp_arr, size);
            printf("Verification: %s\n",
                is_sorted(temp_arr, size) ? "PASSED" : "FAILED");
            break;

        case 3:  // Compare both versions
            printf("\nComparing both versions with the same input:\n");

            // 기본 선택 정렬
            copy_array(temp_arr, arr, size);
            measure_time(selection_sort, temp_arr, size, "Basic selection sort");

            // 최적화된 선택 정렬
            copy_array(temp_arr, arr, size);
            measure_time(selection_sort_optimized, temp_arr, size,
                "Optimized selection sort");
            break;

        case 4:  // Generate new array
            generate_random_array(arr, size);
            printf("New random array generated\n");
            printf("Array contents:\n");
            print_array(arr, size);
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

1. 선택 정렬의 기본 원리
-------------------
- 전체에서 최소값을 선택
- 선택된 값을 맨 앞으로 이동
- 정렬된 부분을 제외하고 반복
- 불안정 정렬 (Unstable Sort)

2. 시간 복잡도
-----------
최악의 경우: O(n²)
- 모든 경우 동일
- n(n-1)/2번의 비교

평균의 경우: O(n²)
- 입력에 관계없이 일정

최선의 경우: O(n²)
- 이미 정렬되어 있어도 동일

3. 공간 복잡도
-----------
O(1)
- 추가 메모리 불필요
- 제자리 정렬 (In-place Sort)

4. 최적화 기법
-----------
양방향 선택:
- 최소값과 최대값 동시 탐색
- 비교 횟수는 동일하나 교환 최적화
- 반복 횟수 절반으로 감소

불필요한 교환 방지:
- 현재 위치가 최소값일 때 교환 생략
- 메모리 쓰기 횟수 감소

5. 버블 정렬과의 비교
-----------------
장점:
- 교환 횟수가 적음 (O(n))
- 메모리 쓰기 감소
- 성능이 일정함

단점:
- 항상 O(n²) 비교 필요
- 불안정 정렬
- 최적화 여지가 적음

6. 장단점
-------
장점:
- 구현이 단순
- 교환 횟수가 적음
- 추가 메모리 불필요

단점:
- 느린 수행 속도
- 불안정 정렬
- 입력에 민감하지 않음

7. 교육적 가치
-----------
- 선형 탐색 이해
- 정렬 알고리즘 분석
- 최적화 기법 학습

8. 구현 특징
----------
- 양방향 최적화
- 성능 측정
- 결과 검증
- 대화형 인터페이스

9. 활용 분야
----------
- 작은 데이터셋
- 메모리 쓰기가 비싼 경우
- 간단한 정렬 작업
- 교육용 예제

이 구현은 선택 정렬의 기본적인 특성과
최적화 방법을 포함하며, 성능을
실제로 측정하고 비교할 수 있는
교육용 프로그램을 제공합니다.
*/
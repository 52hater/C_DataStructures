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

/* 중앙값 피봇 선택
 * - 매개변수: arr - 배열, left, right - 구간의 시작과 끝
 * - 반환값: 중앙값의 인덱스
 */
size_t median_of_three(DataType arr[], size_t left, size_t right) {
    size_t mid = (left + right) / 2;

    if (arr[left] > arr[mid])
        swap(&arr[left], &arr[mid]);
    if (arr[left] > arr[right])
        swap(&arr[left], &arr[right]);
    if (arr[mid] > arr[right])
        swap(&arr[mid], &arr[right]);

    return mid;
}

/* Lomuto 분할 방식
 * - 매개변수: arr - 배열, left, right - 분할할 구간
 * - 반환값: 피봇의 최종 위치
 */
size_t partition_lomuto(DataType arr[], size_t left, size_t right) {
    size_t pivot_idx = median_of_three(arr, left, right);
    DataType pivot = arr[pivot_idx];
    swap(&arr[pivot_idx], &arr[right]);  // 피봇을 끝으로 이동

    size_t i = left;  // 작은 원소들의 경계

    for (size_t j = left; j < right; j++) {
        if (arr[j] <= pivot) {
            swap(&arr[i], &arr[j]);
            i++;
        }
    }

    swap(&arr[i], &arr[right]);  // 피봇을 최종 위치로
    return i;
}

/* Hoare 분할 방식
 * - 매개변수: arr - 배열, left, right - 분할할 구간
 * - 반환값: 분할 위치
 */
size_t partition_hoare(DataType arr[], size_t left, size_t right) {
    size_t pivot_idx = median_of_three(arr, left, right);
    DataType pivot = arr[pivot_idx];

    size_t i = left - 1;
    size_t j = right + 1;

    while (true) {
        do {
            i++;
        } while (arr[i] < pivot);

        do {
            j--;
        } while (arr[j] > pivot);

        if (i >= j)
            return j;

        swap(&arr[i], &arr[j]);
    }
}

/* 퀵 정렬 (Lomuto 분할)
 * - 매개변수: arr - 정렬할 배열, left, right - 정렬할 구간
 */
void quick_sort_lomuto(DataType arr[], size_t left, size_t right) {
    if (left < right) {
        size_t pivot = partition_lomuto(arr, left, right);
        if (pivot > 0)
            quick_sort_lomuto(arr, left, pivot - 1);
        quick_sort_lomuto(arr, pivot + 1, right);
    }
}

/* 퀵 정렬 (Hoare 분할)
 * - 매개변수: arr - 정렬할 배열, left, right - 정렬할 구간
 */
void quick_sort_hoare(DataType arr[], size_t left, size_t right) {
    if (left < right) {
        size_t pivot = partition_hoare(arr, left, right);
        quick_sort_hoare(arr, left, pivot);
        quick_sort_hoare(arr, pivot + 1, right);
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

/* Wrapper 함수 - Lomuto 퀵 정렬
 * - 매개변수: arr - 정렬할 배열, size - 배열의 크기
 */
void quick_sort_lomuto_wrapper(DataType arr[], size_t size) {
    if (size > 0) {
        quick_sort_lomuto(arr, 0, size - 1);
    }
}

/* Wrapper 함수 - Hoare 퀵 정렬
 * - 매개변수: arr - 정렬할 배열, size - 배열의 크기
 */
void quick_sort_hoare_wrapper(DataType arr[], size_t size) {
    if (size > 0) {
        quick_sort_hoare(arr, 0, size - 1);
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

/* 거의 정렬된 배열 생성
 * - 매개변수: arr - 생성할 배열, size - 배열의 크기
 */
void generate_nearly_sorted_array(DataType arr[], size_t size) {
    for (size_t i = 0; i < size; i++) {
        arr[i] = i;
    }
    // 약 5%의 원소만 무작위로 섞음
    for (size_t i = 0; i < size / 20; i++) {
        size_t idx1 = rand() % size;
        size_t idx2 = rand() % size;
        swap(&arr[idx1], &arr[idx2]);
    }
}

/* 많은 중복값이 있는 배열 생성
 * - 매개변수: arr - 생성할 배열, size - 배열의 크기
 */
void generate_many_duplicates(DataType arr[], size_t size) {
    for (size_t i = 0; i < size; i++) {
        arr[i] = rand() % 10;  // 0-9 범위의 값만 사용
    }
}

/* 메뉴 출력 */
void print_menu(void) {
    printf("\n=== Quick Sort Menu ===\n");
    printf("1. Run Lomuto partition quick sort\n");
    printf("2. Run Hoare partition quick sort\n");
    printf("3. Compare both versions\n");
    printf("4. Generate new random array\n");
    printf("5. Generate nearly sorted array\n");
    printf("6. Generate array with many duplicates\n");
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
        case 1:  // Lomuto partition
            copy_array(temp_arr, arr, size);
            printf("\nArray before sorting:\n");
            print_array(temp_arr, size);

            measure_time(quick_sort_lomuto_wrapper, temp_arr, size,
                "Quick sort (Lomuto)");

            printf("Array after sorting:\n");
            print_array(temp_arr, size);
            printf("Verification: %s\n",
                is_sorted(temp_arr, size) ? "PASSED" : "FAILED");
            break;

        case 2:  // Hoare partition
            copy_array(temp_arr, arr, size);
            printf("\nArray before sorting:\n");
            print_array(temp_arr, size);

            measure_time(quick_sort_hoare_wrapper, temp_arr, size,
                "Quick sort (Hoare)");

            printf("Array after sorting:\n");
            print_array(temp_arr, size);
            printf("Verification: %s\n",
                is_sorted(temp_arr, size) ? "PASSED" : "FAILED");
            break;

        case 3:  // Compare both versions
            printf("\nComparing both versions with the same input:\n");

            // Lomuto partition
            copy_array(temp_arr, arr, size);
            measure_time(quick_sort_lomuto_wrapper, temp_arr, size,
                "Quick sort (Lomuto)");

            // Hoare partition
            copy_array(temp_arr, arr, size);
            measure_time(quick_sort_hoare_wrapper, temp_arr, size,
                "Quick sort (Hoare)");
            break;

        case 4:  // Generate random array
            generate_random_array(arr, size);
            printf("New random array generated\n");
            printf("Array contents:\n");
            print_array(arr, size);
            break;

        case 5:  // Generate nearly sorted array
            generate_nearly_sorted_array(arr, size);
            printf("New nearly sorted array generated\n");
            printf("Array contents:\n");
            print_array(arr, size);
            break;

        case 6:  // Generate array with duplicates
            generate_many_duplicates(arr, size);
            printf("New array with duplicates generated\n");
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

1. 퀵 정렬의 기본 원리
------------------
- 분할 정복 (Divide and Conquer) 방식
- 피봇을 기준으로 배열 분할
- 재귀적 해결
- 불안정 정렬 (Unstable Sort)

2. 분할 방식
----------
Lomuto 분할:
- 마지막 원소를 피봇으로
- 단방향 순회
- 구현이 단순
- 중복값에 약함

Hoare 분할:
- 양방향에서 접근
- 더 적은 교환
- 더 효율적
- 구현이 복잡

3. 시간 복잡도
-----------
최선의 경우: O(n log n)
- 균형잡힌 분할
- 이상적인 피봇 선택

평균의 경우: O(n log n)
- 무작위 데이터
- 실용적인 성능

최악의 경우: O(n²)
- 이미 정렬된 경우
- 피봇이 최소/최대값

4. 공간 복잡도
-----------
평균: O(log n)
- 재귀 호출 스택

최악: O(n)
- 불균형 분할시

5. 최적화 기법
-----------
피봇 선택:
- 중앙값 선택
- 세 값의 중앙값
- 무작위 선택

작은 부분배열:
- 삽입 정렬 활용
- 임계값 이하 처리
- 재귀 깊이 감소

6. 장단점
-------
장점:
- 평균적으로 빠름
- 제자리 정렬
- 캐시 효율적

단점:
- 불안정 정렬
- 피봇 선택 중요
- 최악의 경우 느림

7. 특징
------
- 분할 정복 알고리즘
- 다양한 변형 존재
- 실제로 많이 사용됨
- 병렬화 가능

8. 구현 특징
----------
- 최적화된 피봇 선택
- 다양한 테스트 데이터
- 성능 측정 기능
- 결과 검증

9. 실제 응용
----------
- 표준 라이브러리 정렬
- 대용량 데이터 처리
- 외부 정렬의 기초
- 하이브리드 정렬

10. 개선 가능성
------------
- 삽입 정렬 혼합
- 3-way 분할
- 반복적 구현
- 병렬 처리

이 구현은 퀵 정렬의 두 가지 주요
분할 방식을 모두 포함하며, 다양한
입력 데이터에 대한 성능 비교가
가능한 교육용 프로그램입니다.
*/
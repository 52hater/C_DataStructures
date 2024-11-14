#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

typedef int DataType;

/* 병합 함수 (기본 버전)
 * - 매개변수: arr - 원본 배열, left, mid, right - 병합할 구간
 *            temp - 임시 배열
 */
void merge(DataType arr[], size_t left, size_t mid, size_t right, DataType temp[]) {
    size_t i = left;    // 왼쪽 배열 인덱스
    size_t j = mid + 1; // 오른쪽 배열 인덱스
    size_t k = left;    // 임시 배열 인덱스

    // 두 배열을 비교하며 임시 배열에 병합
    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) {
            temp[k++] = arr[i++];
        }
        else {
            temp[k++] = arr[j++];
        }
    }

    // 왼쪽 배열에 남은 요소 복사
    while (i <= mid) {
        temp[k++] = arr[i++];
    }

    // 오른쪽 배열에 남은 요소 복사
    while (j <= right) {
        temp[k++] = arr[j++];
    }

    // 임시 배열의 내용을 원본 배열로 복사
    for (k = left; k <= right; k++) {
        arr[k] = temp[k];
    }
}

/* 병합 함수 (최적화 버전)
 * - 매개변수: arr - 원본 배열, left, mid, right - 병합할 구간
 *            temp - 임시 배열
 */
void merge_optimized(DataType arr[], size_t left, size_t mid, size_t right,
    DataType temp[]) {
    size_t i = left;
    size_t j = mid + 1;
    size_t k = left;

    // 미리 비교하여 병합이 필요 없는 경우 처리
    if (arr[mid] <= arr[j]) {
        return;  // 이미 정렬된 상태
    }

    // 두 배열을 비교하며 임시 배열에 병합
    while (i <= mid && j <= right) {
        temp[k++] = (arr[i] <= arr[j]) ? arr[i++] : arr[j++];
    }

    // 왼쪽 배열에 남은 요소 복사
    while (i <= mid) {
        temp[k++] = arr[i++];
    }

    // 오른쪽 배열에 남은 요소 복사
    while (j <= right) {
        temp[k++] = arr[j++];
    }

    // 임시 배열의 내용을 원본 배열로 복사
    for (k = left; k <= right; k++) {
        arr[k] = temp[k];
    }
}

/* 기본 병합 정렬 (재귀 버전)
 * - 매개변수: arr - 정렬할 배열, left, right - 정렬할 구간
 *            temp - 임시 배열
 */
void merge_sort_recursive(DataType arr[], size_t left, size_t right,
    DataType temp[]) {
    if (left < right) {
        size_t mid = (left + right) / 2;
        merge_sort_recursive(arr, left, mid, temp);
        merge_sort_recursive(arr, mid + 1, right, temp);
        merge(arr, left, mid, right, temp);
    }
}

/* 최적화된 병합 정렬 (재귀 버전)
 * - 매개변수: arr - 정렬할 배열, left, right - 정렬할 구간
 *            temp - 임시 배열
 */
void merge_sort_optimized_recursive(DataType arr[], size_t left, size_t right,
    DataType temp[]) {
    // 작은 배열은 삽입 정렬 사용
    if (right - left <= 10) {
        // 삽입 정렬 구현
        for (size_t i = left + 1; i <= right; i++) {
            DataType key = arr[i];
            size_t j = i - 1;
            while (j >= left && arr[j] > key) {
                arr[j + 1] = arr[j];
                j--;
            }
            arr[j + 1] = key;
        }
        return;
    }

    if (left < right) {
        size_t mid = (left + right) / 2;
        merge_sort_optimized_recursive(arr, left, mid, temp);
        merge_sort_optimized_recursive(arr, mid + 1, right, temp);
        merge_optimized(arr, left, mid, right, temp);
    }
}

/* 반복적 병합 정렬
 * - 매개변수: arr - 정렬할 배열, size - 배열의 크기
 *            temp - 임시 배열
 */
void merge_sort_iterative(DataType arr[], size_t size, DataType temp[]) {
    for (size_t width = 1; width < size; width *= 2) {
        for (size_t left = 0; left < size; left += 2 * width) {
            size_t mid = (left + width < size) ? left + width - 1 : size - 1;
            size_t right = (left + 2 * width < size) ? left + 2 * width - 1 : size - 1;
            merge(arr, left, mid, right, temp);
        }
    }
}

/* Wrapper 함수 - 기본 병합 정렬
 * - 매개변수: arr - 정렬할 배열, size - 배열의 크기
 */
void merge_sort_basic(DataType arr[], size_t size) {
    DataType* temp = (DataType*)malloc(size * sizeof(DataType));
    if (temp == NULL) {
        printf("Memory allocation failed\n");
        return;
    }
    merge_sort_recursive(arr, 0, size - 1, temp);
    free(temp);
}

/* Wrapper 함수 - 최적화된 병합 정렬
 * - 매개변수: arr - 정렬할 배열, size - 배열의 크기
 */
void merge_sort_optimized(DataType arr[], size_t size) {
    DataType* temp = (DataType*)malloc(size * sizeof(DataType));
    if (temp == NULL) {
        printf("Memory allocation failed\n");
        return;
    }
    merge_sort_optimized_recursive(arr, 0, size - 1, temp);
    free(temp);
}

/* Wrapper 함수 - 반복적 병합 정렬
 * - 매개변수: arr - 정렬할 배열, size - 배열의 크기
 */
void merge_sort_iterative_wrapper(DataType arr[], size_t size) {
    DataType* temp = (DataType*)malloc(size * sizeof(DataType));
    if (temp == NULL) {
        printf("Memory allocation failed\n");
        return;
    }
    merge_sort_iterative(arr, size, temp);
    free(temp);
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
        DataType temp = arr[idx1];
        arr[idx1] = arr[idx2];
        arr[idx2] = temp;
    }
}

/* 메뉴 출력 */
void print_menu(void) {
    printf("\n=== Merge Sort Menu ===\n");
    printf("1. Run basic merge sort\n");
    printf("2. Run optimized merge sort\n");
    printf("3. Run iterative merge sort\n");
    printf("4. Compare all versions\n");
    printf("5. Generate new random array\n");
    printf("6. Generate nearly sorted array\n");
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
        case 1:  // Basic merge sort
            copy_array(temp_arr, arr, size);
            printf("\nArray before sorting:\n");
            print_array(temp_arr, size);

            measure_time(merge_sort_basic, temp_arr, size,
                "Basic merge sort");

            printf("Array after sorting:\n");
            print_array(temp_arr, size);
            printf("Verification: %s\n",
                is_sorted(temp_arr, size) ? "PASSED" : "FAILED");
            break;

        case 2:  // Optimized merge sort
            copy_array(temp_arr, arr, size);
            printf("\nArray before sorting:\n");
            print_array(temp_arr, size);

            measure_time(merge_sort_optimized, temp_arr, size,
                "Optimized merge sort");

            printf("Array after sorting:\n");
            print_array(temp_arr, size);
            printf("Verification: %s\n",
                is_sorted(temp_arr, size) ? "PASSED" : "FAILED");
            break;

        case 3:  // Iterative merge sort
            copy_array(temp_arr, arr, size);
            printf("\nArray before sorting:\n");
            print_array(temp_arr, size);

            measure_time(merge_sort_iterative_wrapper, temp_arr, size,
                "Iterative merge sort");

            printf("Array after sorting:\n");
            print_array(temp_arr, size);
            printf("Verification: %s\n",
                is_sorted(temp_arr, size) ? "PASSED" : "FAILED");
            break;

        case 4:  // Compare all versions
            printf("\nComparing all versions with the same input:\n");

            // 기본 병합 정렬
            copy_array(temp_arr, arr, size);
            measure_time(merge_sort_basic, temp_arr, size,
                "Basic merge sort");

            // 최적화된 병합 정렬
            copy_array(temp_arr, arr, size);
            measure_time(merge_sort_optimized, temp_arr, size,
                "Optimized merge sort");

            // 반복적 병합 정렬
            copy_array(temp_arr, arr, size);
            measure_time(merge_sort_iterative_wrapper, temp_arr, size,
                "Iterative merge sort");
            break;

        case 5:  // Generate random array
            generate_random_array(arr, size);
            printf("New random array generated\n");
            printf("Array contents:\n");
            print_array(arr, size);
            break;

        case 6:  // Generate nearly sorted array
            generate_nearly_sorted_array(arr, size);
            printf("New nearly sorted array generated\n");
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

1. 병합 정렬의 기본 원리
-------------------
- 분할 정복 방식
- 배열을 반으로 나눔
- 재귀적 정렬
- 병합 과정에서 정렬
- 안정 정렬 (Stable Sort)

2. 시간 복잡도
-----------
최선의 경우: O(n log n)
평균의 경우: O(n log n)
최악의 경우: O(n log n)
- 모든 경우 동일한 성능
- 입력에 독립적

3. 공간 복잡도
-----------
O(n)
- 임시 배열 필요
- 제자리 정렬 아님
- 추가 메모리 사용

4. 구현 방식
----------
재귀적 구현:
- 직관적인 이해
- 콜 스택 사용
- 깊이 제한 있음

반복적 구현:
- 콜 스택 오버헤드 없음
- 구현이 복잡
- 메모리 효율적

5. 최적화 기법
-----------
작은 부분배열:
- 삽입 정렬 활용
- 임계값 이하 처리
- 오버헤드 감소

병합 최적화:
- 이미 정렬된 경우 검사
- 복사 횟수 감소
- 캐시 효율성 개선

6. 장단점
-------
장점:
- 안정 정렬
- 예측 가능한 성능
- 연결 리스트 정렬에 적합
- 외부 정렬의 기초

단점:
- 추가 메모리 필요
- 캐시 지역성 낮음
- 작은 배열에 비효율적
- 공간 오버헤드

7. 응용 분야
----------
- 외부 정렬
- 연결 리스트 정렬
- 병렬 정렬
- 안정성이 필요한 경우

8. 구현 특징
----------
- 세 가지 구현 방식
- 최적화 기법 적용
- 성능 측정 기능
- 결과 검증

9. 개선 가능성
-----------
- 병렬화
- 캐시 최적화
- 하이브리드 방식
- 메모리 재사용

10. 교육적 가치
------------
- 분할 정복 이해
- 안정 정렬 개념
- 시간/공간 트레이드오프
- 최적화 기법 학습

이 구현은 병합 정렬의 다양한
버전을 포함하며, 각각의 장단점을
비교할 수 있는 교육용 프로그램을
제공합니다.
*/
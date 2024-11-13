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

/* 기본 버블 정렬
 * - 매개변수: arr - 정렬할 배열, size - 배열의 크기
 * - 설명: 인접한 두 원소를 비교하여 교환
 */
void bubble_sort_basic(DataType arr[], size_t size) {
    for (size_t i = 0; i < size - 1; i++) {
        for (size_t j = 0; j < size - 1 - i; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(&arr[j], &arr[j + 1]);
            }
        }
    }
}

/* 최적화된 버블 정렬
 * - 매개변수: arr - 정렬할 배열, size - 배열의 크기
 * - 설명: 교환이 일어나지 않으면 조기 종료
 */
void bubble_sort_optimized(DataType arr[], size_t size) {
    bool swapped;
    for (size_t i = 0; i < size - 1; i++) {
        swapped = false;
        for (size_t j = 0; j < size - 1 - i; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(&arr[j], &arr[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) {
            break;  // 교환이 없으면 이미 정렬된 상태
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

/* 배열을 무작위로 섞기
 * - 매개변수: arr - 섞을 배열, size - 배열의 크기
 */
void shuffle_array(DataType arr[], size_t size) {
    for (size_t i = size - 1; i > 0; i--) {
        size_t j = rand() % (i + 1);
        swap(&arr[i], &arr[j]);
    }
}

/* 성능 측정을 위한 배열 복사
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

/* 메뉴 출력 */
void print_menu(void) {
    printf("\n=== Bubble Sort Menu ===\n");
    printf("1. Run basic bubble sort\n");
    printf("2. Run optimized bubble sort\n");
    printf("3. Compare both versions\n");
    printf("4. Generate new random array\n");
    printf("0. Exit\n");
    printf("Choice: ");
}

/* 새로운 무작위 배열 생성
 * - 매개변수: arr - 생성할 배열, size - 배열의 크기
 */
void generate_random_array(DataType arr[], size_t size) {
    for (size_t i = 0; i < size; i++) {
        arr[i] = rand() % 1000;  // 0-999 범위의 무작위 값
    }
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
        case 1:  // Basic bubble sort
            copy_array(temp_arr, arr, size);
            printf("\nArray before sorting:\n");
            print_array(temp_arr, size);

            measure_time(bubble_sort_basic, temp_arr, size, "Basic bubble sort");

            printf("Array after sorting:\n");
            print_array(temp_arr, size);
            printf("Verification: %s\n",
                is_sorted(temp_arr, size) ? "PASSED" : "FAILED");
            break;

        case 2:  // Optimized bubble sort
            copy_array(temp_arr, arr, size);
            printf("\nArray before sorting:\n");
            print_array(temp_arr, size);

            measure_time(bubble_sort_optimized, temp_arr, size, "Optimized bubble sort");

            printf("Array after sorting:\n");
            print_array(temp_arr, size);
            printf("Verification: %s\n",
                is_sorted(temp_arr, size) ? "PASSED" : "FAILED");
            break;

        case 3:  // Compare both versions
            printf("\nComparing both versions with the same input:\n");

            // 기본 버블 정렬
            copy_array(temp_arr, arr, size);
            measure_time(bubble_sort_basic, temp_arr, size, "Basic bubble sort");

            // 최적화된 버블 정렬
            copy_array(temp_arr, arr, size);
            measure_time(bubble_sort_optimized, temp_arr, size, "Optimized bubble sort");
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

1. 버블 정렬의 기본 원리
-------------------
- 인접한 두 원소를 비교하여 교환
- 한 패스마다 최대값이 끝으로 이동
- n-1번의 패스 필요
- 안정 정렬 (Stable Sort)

2. 시간 복잡도
-----------
최악의 경우: O(n²)
- 역순으로 정렬된 경우
- n(n-1)/2번의 비교

평균의 경우: O(n²)
- 무작위로 배치된 경우

최선의 경우: O(n)
- 이미 정렬된 경우 (최적화 버전)

3. 공간 복잡도
-----------
O(1)
- 추가 메모리 불필요
- 제자리 정렬 (In-place Sort)

4. 최적화 기법
-----------
Early Termination:
- 교환이 없으면 종료
- 이미 정렬된 경우 유용
- 평균 실행 시간 개선

내부 루프 최적화:
- 정렬된 부분 제외
- 불필요한 비교 감소

5. 장단점
-------
장점:
- 구현이 단순
- 안정 정렬
- 추가 메모리 불필요

단점:
- 느린 수행 속도
- 많은 교환 연산
- 큰 데이터에 비효율적

6. 교육적 가치
-----------
- 기본 정렬 개념 이해
- 알고리즘 최적화 학습
- 시간복잡도 분석 연습

7. 구현 특징
----------
- 타입 추상화 (DataType)
- 성능 측정 기능
- 결과 검증 기능
- 대화형 인터페이스

8. 활용 분야
----------
- 교육용 예제
- 작은 데이터셋
- 거의 정렬된 데이터
- 간단한 정렬 작업

이 구현은 버블 정렬의 기본 개념과
최적화 방법을 모두 포함하며,
실제 성능을 측정하고 비교할 수 있는
교육용 프로그램을 제공합니다.
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

typedef int DataType;

/* 삽입 정렬
 * - 매개변수: arr - 정렬할 배열, size - 배열의 크기
 * - 설명: 각 원소를 정렬된 부분의 적절한 위치에 삽입
 */
void insertion_sort(DataType arr[], size_t size) {
    for (size_t i = 1; i < size; i++) {
        DataType key = arr[i];
        int j = i - 1;

        // key보다 큰 원소들을 뒤로 이동
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

/* 이진 탐색을 이용한 삽입 정렬
 * - 매개변수: arr - 정렬할 배열, size - 배열의 크기
 * - 설명: 이진 탐색으로 삽입 위치를 찾아 삽입
 */
void binary_insertion_sort(DataType arr[], size_t size) {
    for (size_t i = 1; i < size; i++) {
        DataType key = arr[i];
        int left = 0;
        int right = i - 1;

        // 이진 탐색으로 삽입 위치 찾기
        while (left <= right) {
            int mid = (left + right) / 2;
            if (arr[mid] > key) {
                right = mid - 1;
            }
            else {
                left = mid + 1;
            }
        }

        // 원소들을 뒤로 이동
        for (int j = i - 1; j >= left; j--) {
            arr[j + 1] = arr[j];
        }
        arr[left] = key;
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
    printf("\n=== Insertion Sort Menu ===\n");
    printf("1. Run basic insertion sort\n");
    printf("2. Run binary insertion sort\n");
    printf("3. Compare both versions\n");
    printf("4. Generate new random array\n");
    printf("5. Generate nearly sorted array\n");
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
        case 1:  // Basic insertion sort
            copy_array(temp_arr, arr, size);
            printf("\nArray before sorting:\n");
            print_array(temp_arr, size);

            measure_time(insertion_sort, temp_arr, size, "Basic insertion sort");

            printf("Array after sorting:\n");
            print_array(temp_arr, size);
            printf("Verification: %s\n",
                is_sorted(temp_arr, size) ? "PASSED" : "FAILED");
            break;

        case 2:  // Binary insertion sort
            copy_array(temp_arr, arr, size);
            printf("\nArray before sorting:\n");
            print_array(temp_arr, size);

            measure_time(binary_insertion_sort, temp_arr, size,
                "Binary insertion sort");

            printf("Array after sorting:\n");
            print_array(temp_arr, size);
            printf("Verification: %s\n",
                is_sorted(temp_arr, size) ? "PASSED" : "FAILED");
            break;

        case 3:  // Compare both versions
            printf("\nComparing both versions with the same input:\n");

            // 기본 삽입 정렬
            copy_array(temp_arr, arr, size);
            measure_time(insertion_sort, temp_arr, size, "Basic insertion sort");

            // 이진 삽입 정렬
            copy_array(temp_arr, arr, size);
            measure_time(binary_insertion_sort, temp_arr, size,
                "Binary insertion sort");
            break;

        case 4:  // Generate new random array
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

1. 삽입 정렬의 기본 원리
-------------------
- 정렬된 부분과 미정렬 부분 구분
- 새 원소를 정렬된 부분의 적절한 위치에 삽입
- 카드 정렬하기와 유사한 방식
- 안정 정렬 (Stable Sort)

2. 시간 복잡도
-----------
최악의 경우: O(n²)
- 역순으로 정렬된 경우
- n(n-1)/2번의 비교와 이동

최선의 경우: O(n)
- 이미 정렬된 경우
- n-1번의 비교만 필요

평균의 경우: O(n²)
- 무작위 데이터의 경우

3. 공간 복잡도
-----------
O(1)
- 추가 메모리 불필요
- 제자리 정렬 (In-place Sort)

4. 최적화 기법
-----------
이진 탐색 활용:
- 삽입 위치 탐색 최적화
- 비교 횟수 감소: O(n log n)
- 이동 횟수는 동일

부분 정렬 활용:
- 정렬된 부분 유지
- 적은 이동 횟수
- 안정성 보장

5. 특징
------
장점:
- 안정 정렬
- 적응적 정렬
- 온라인 정렬 가능
- 부분 정렬에 효율적

단점:
- 큰 데이터에 비효율적
- 많은 이동 연산
- 원소 이동 비용 큼

6. 다른 정렬과의 비교
-----------------
버블 정렬 대비:
- 더 효율적
- 더 적은 교환
- 실제 응용에 더 적합

선택 정렬 대비:
- 더 적응적
- 부분 정렬에 강점
- 안정 정렬

7. 응용 분야
----------
- 작은 데이터셋
- 거의 정렬된 데이터
- 온라인 정렬
- 적응적 정렬 필요 시

8. 구현 특징
----------
- 이진 탐색 최적화
- 성능 측정 기능
- 다양한 입력 생성
- 결과 검증

이 구현은 삽입 정렬의 기본 특성과
최적화 방법을 포함하며, 특히
거의 정렬된 데이터에서의 성능을
테스트할 수 있는 기능을 제공합니다.
*/
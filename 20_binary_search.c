#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

typedef int DataType;

/*
재귀적 이진 탐색:
- 중간값과 비교하여 탐색 범위를 절반으로 줄임
- 분할 정복 방식으로 구현
- 콜 스택 사용
*/
int binary_search_recursive(const DataType arr[], int left, int right, DataType key) {
    if (left > right) {
        return -1;
    }

    int mid = left + (right - left) / 2;  // 오버플로우 방지

    if (arr[mid] == key) {
        return mid;
    }

    if (arr[mid] > key) {
        return binary_search_recursive(arr, left, mid - 1, key);
    }

    return binary_search_recursive(arr, mid + 1, right, key);
}

/*
반복적 이진 탐색:
- while 루프를 사용하여 구현
- 재귀 호출 오버헤드 제거
- 스택 오버플로우 위험 없음
*/
int binary_search_iterative(const DataType arr[], int size, DataType key) {
    int left = 0;
    int right = size - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (arr[mid] == key) {
            return mid;
        }

        if (arr[mid] > key) {
            right = mid - 1;
        }
        else {
            left = mid + 1;
        }
    }

    return -1;
}

/* Wrapper 함수 - 재귀적 이진 탐색
 * - 매개변수: arr - 검색할 배열, size - 배열의 크기, key - 찾을 값
 */
int binary_search_recursive_wrapper(const DataType arr[], int size, DataType key) {
    return binary_search_recursive(arr, 0, size - 1, key);
}

/* 배열이 정렬되어 있는지 확인
 * - 매개변수: arr - 확인할 배열, size - 배열의 크기
 */
bool is_sorted(const DataType arr[], size_t size) {
    for (size_t i = 1; i < size; i++) {
        if (arr[i] < arr[i - 1]) {
            return false;
        }
    }
    return true;
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

/* 배열 복사
 * - 매개변수: dest - 목적지 배열, src - 원본 배열, size - 배열의 크기
 */
void copy_array(DataType dest[], const DataType src[], size_t size) {
    for (size_t i = 0; i < size; i++) {
        dest[i] = src[i];
    }
}

/* 알고리즘 실행 시간 측정
 * - 매개변수: search_func - 검색 함수 포인터, arr - 검색할 배열,
 *            size - 배열의 크기, key - 찾을 값, name - 알고리즘 이름
 */
void measure_time(int (*search_func)(const DataType[], int, DataType),
    const DataType arr[], size_t size, DataType key,
    const char* name) {
    clock_t start = clock();
    int index = search_func(arr, size, key);
    clock_t end = clock();

    double cpu_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("%s: %.6f seconds\n", name, cpu_time);
    if (index != -1) {
        printf("Found at index: %d\n", index);
    }
    else {
        printf("Not found\n");
    }
}

/* 정렬된 무작위 배열 생성
 * - 매개변수: arr - 생성할 배열, size - 배열의 크기
 */
void generate_sorted_array(DataType arr[], size_t size) {
    // 첫 번째 요소는 0-9 사이의 값
    arr[0] = rand() % 10;

    // 나머지 요소들은 이전 요소보다 1-10 큰 값
    for (size_t i = 1; i < size; i++) {
        arr[i] = arr[i - 1] + (rand() % 10) + 1;
    }
}

/* 이진 탐색 과정 시각화
 * - 매개변수: arr - 배열, left, right - 현재 범위, mid - 중간 위치
 */
void visualize_step(const DataType arr[], int size, int left, int right, int mid) {
    printf("\nArray: ");
    for (int i = 0; i < size; i++) {
        if (i == mid) {
            printf("[%d] ", arr[i]);  // 중간 값
        }
        else if (i >= left && i <= right) {
            printf("%d ", arr[i]);    // 현재 범위
        }
        else {
            printf("_ ");             // 제외된 범위
        }
    }
    printf("\n");

    // 포인터 표시
    printf("       ");
    for (int i = 0; i < size; i++) {
        if (i == left && i == right) {
            printf("^   ");
        }
        else if (i == left) {
            printf("L   ");
        }
        else if (i == right) {
            printf("R   ");
        }
        else if (i == mid) {
            printf("M   ");
        }
        else {
            printf("    ");
        }
    }
    printf("\n");
}

/* 시각화된 이진 탐색
 * - 매개변수: arr - 배열, size - 크기, key - 찾을 값
 */
int binary_search_visual(const DataType arr[], int size, DataType key) {
    int left = 0;
    int right = size - 1;
    int step = 1;

    printf("\nSearching for value: %d\n", key);

    while (left <= right) {
        int mid = left + (right - left) / 2;

        printf("\nStep %d:\n", step++);
        visualize_step(arr, size, left, right, mid);
        printf("Comparing with middle element: %d\n", arr[mid]);

        if (arr[mid] == key) {
            printf("Found the value!\n");
            return mid;
        }

        if (arr[mid] > key) {
            printf("Value is smaller, searching left half\n");
            right = mid - 1;
        }
        else {
            printf("Value is larger, searching right half\n");
            left = mid + 1;
        }
    }

    printf("Value not found\n");
    return -1;
}

/* 메뉴 출력 */
void print_menu(void) {
    printf("\n=== Binary Search Menu ===\n");
    printf("1. Run recursive binary search\n");
    printf("2. Run iterative binary search\n");
    printf("3. Run visualized binary search\n");
    printf("4. Compare recursive and iterative\n");
    printf("5. Generate new sorted array\n");
    printf("0. Exit\n");
    printf("Choice: ");
}

int main(void) {
    srand((unsigned int)time(NULL));

    size_t size;
    printf("Enter array size: ");
    if (scanf("%zu", &size) != 1 || size == 0) {
        printf("Invalid size\n");
        return 1;
    }

    DataType* arr = (DataType*)malloc(size * sizeof(DataType));
    if (arr == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    generate_sorted_array(arr, size);
    int choice, search_key;

    do {
        print_menu();
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
        case 1:  // Recursive binary search
            printf("Enter value to search: ");
            scanf("%d", &search_key);

            printf("\nArray contents:\n");
            print_array(arr, size);

            measure_time(binary_search_recursive_wrapper, arr, size, search_key,
                "Recursive binary search");
            break;

        case 2:  // Iterative binary search
            printf("Enter value to search: ");
            scanf("%d", &search_key);

            printf("\nArray contents:\n");
            print_array(arr, size);

            measure_time(binary_search_iterative, arr, size, search_key,
                "Iterative binary search");
            break;

        case 3:  // Visualized binary search
            printf("Enter value to search: ");
            scanf("%d", &search_key);

            printf("\nArray contents:\n");
            print_array(arr, size);

            binary_search_visual(arr, size, search_key);
            break;

        case 4:  // Compare both methods
            printf("Enter value to search: ");
            scanf("%d", &search_key);

            printf("\nComparing both methods:\n");
            printf("Array contents:\n");
            print_array(arr, size);

            measure_time(binary_search_recursive_wrapper, arr, size, search_key,
                "Recursive binary search");
            measure_time(binary_search_iterative, arr, size, search_key,
                "Iterative binary search");
            break;

        case 5:  // Generate new array
            generate_sorted_array(arr, size);
            printf("New sorted array generated\n");
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
    return 0;
}

/*
==========================================
상세 설명 및 주요 개념
==========================================

1. 이진 탐색의 기본 원리
-------------------
- 정렬된 배열 필수
- 중간값과 비교
- 탐색 범위 절반으로 감소
- 분할 정복 방식

2. 시간 복잡도
-----------
최선: O(1)
평균: O(log n)
최악: O(log n)
- 매 단계마다 크기 절반으로
- 로그 시간 보장
- 정렬된 상태 전제

3. 구현 방식
----------
재귀적 구현:
- 직관적 이해 용이
- 콜 스택 사용
- 함수 호출 오버헤드

반복적 구현:
- 메모리 효율적
- 스택 오버플로우 없음
- 실행 속도 약간 빠름

4. 장단점
-------
장점:
- 빠른 검색 속도
- 안정적인 성능
- 메모리 효율적

단점:
- 정렬 필요
- 정적 크기
- 삽입/삭제 비효율

5. 최적화 기법
-----------
- 중간값 계산 최적화
- 경계 조건 처리
- 재귀/반복 선택
- 캐시 활용

6. 시각화 특징
-----------
- 단계별 진행 표시
- 탐색 범위 표시
- 중간값 하이라이트
- 의사 결정 과정

7. 응용 분야
----------
- 데이터베이스 검색
- 정렬된 배열 검색
- 이진 탐색 트리
- 최적화 문제

8. 교육적 가치
-----------
- 분할 정복 이해
- 알고리즘 복잡도
- 재귀/반복 비교
- 최적화 기법

이 구현은 이진 탐색의 두 가지
주요 방식을 포함하며, 시각화
기능을 통해 알고리즘의 동작을
단계별로 이해할 수 있게 합니다.
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

typedef int DataType;

/*
전진이동법(Move-to-Front):
- 탐색한 요소를 배열의 맨 앞으로 이동시키는 방법
- 자주 검색되는 요소의 접근 속도를 향상
- 시간적 지역성(temporal locality) 활용
*/
int sequential_search_mtf(DataType arr[], size_t size, DataType key) {
    for (size_t i = 0; i < size; i++) {
        if (arr[i] == key) {
            // 찾은 요소를 맨 앞으로 이동
            if (i > 0) {
                DataType temp = arr[i];
                for (size_t j = i; j > 0; j--) {
                    arr[j] = arr[j - 1];
                }
                arr[0] = temp;
            }
            return 0;  // 첫 번째 위치로 이동됨
        }
    }
    return -1;
}

/*
보초법(Sentinel):
- 배열 끝에 찾는 값을 추가하여 경계 검사 생략
- 반복문의 조건 검사 횟수 감소
- 성능 약간 향상
*/
int sequential_search_sentinel(DataType arr[], size_t size, DataType key) {
    DataType last = arr[size - 1];  // 마지막 요소 저장
    arr[size - 1] = key;  // 보초 설정

    size_t i = 0;
    while (arr[i] != key) {
        i++;
    }

    arr[size - 1] = last;  // 원래 값 복원
    if (i < size - 1 || arr[size - 1] == key) {
        return i;
    }
    return -1;
}

/* 기본 순차 탐색
 * - 배열의 처음부터 끝까지 순차적으로 검색
 */
int sequential_search_basic(const DataType arr[], size_t size, DataType key) {
    for (size_t i = 0; i < size; i++) {
        if (arr[i] == key) {
            return i;
        }
    }
    return -1;
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
void measure_time(int (*search_func)(DataType[], size_t, DataType),
    DataType arr[], size_t size, DataType key,
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

/* 무작위 배열 생성
 * - 매개변수: arr - 생성할 배열, size - 배열의 크기
 */
void generate_random_array(DataType arr[], size_t size) {
    for (size_t i = 0; i < size; i++) {
        arr[i] = rand() % 1000;  // 0-999 범위의 무작위 값
    }
}

/* 메뉴 출력 */
void print_menu(void) {
    printf("\n=== Sequential Search Menu ===\n");
    printf("1. Basic sequential search\n");
    printf("2. Move-to-Front search\n");
    printf("3. Sentinel search\n");
    printf("4. Compare all methods\n");
    printf("5. Generate new random array\n");
    printf("0. Exit\n");
    printf("Choice: ");
}

int main(void) {
    srand((unsigned int)time(NULL));

    size_t size;
    printf("Enter array size: ");
    if (scanf("%zu", &size) != 1 || size < 2) {  // 최소 2개 필요
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
    int choice, search_key;

    do {
        print_menu();
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
        case 1:  // Basic sequential search
            printf("Enter value to search: ");
            scanf("%d", &search_key);
            printf("\nArray contents:\n");
            print_array(arr, size);

            measure_time(sequential_search_basic, arr, size, search_key,
                "Basic sequential search");
            break;

        case 2:  // Move-to-Front
            printf("Enter value to search: ");
            scanf("%d", &search_key);
            copy_array(temp_arr, arr, size);

            printf("\nArray before search:\n");
            print_array(temp_arr, size);

            measure_time(sequential_search_mtf, temp_arr, size, search_key,
                "Move-to-Front search");

            printf("Array after search:\n");
            print_array(temp_arr, size);
            break;

        case 3:  // Sentinel
            printf("Enter value to search: ");
            scanf("%d", &search_key);
            copy_array(temp_arr, arr, size);

            printf("\nArray contents:\n");
            print_array(temp_arr, size);

            measure_time(sequential_search_sentinel, temp_arr, size, search_key,
                "Sentinel search");
            break;

        case 4:  // Compare all methods
            printf("Enter value to search: ");
            scanf("%d", &search_key);

            printf("\nComparing all methods:\n");

            copy_array(temp_arr, arr, size);
            measure_time(sequential_search_basic, temp_arr, size, search_key,
                "Basic sequential search");

            copy_array(temp_arr, arr, size);
            measure_time(sequential_search_mtf, temp_arr, size, search_key,
                "Move-to-Front search");

            copy_array(temp_arr, arr, size);
            measure_time(sequential_search_sentinel, temp_arr, size, search_key,
                "Sentinel search");
            break;

        case 5:  // Generate new array
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

1. 순차 탐색의 기본 원리
-------------------
- 처음부터 끝까지 순차적 비교
- 발견 즉시 인덱스 반환
- 최악의 경우 전체 탐색

2. 시간 복잡도
-----------
기본 순차 탐색:
- 평균: O(n/2)
- 최악: O(n)

전진이동법:
- 최선: O(1)
- 최악: O(n)
- 자주 검색되는 요소에 유리

보초법:
- 평균: O(n/2)
- 최악: O(n)
- 비교 연산 감소

3. 최적화 기법
-----------
전진이동법:
- 시간적 지역성 활용
- 자주 접근하는 데이터 최적화
- 이동 오버헤드 존재

보초법:
- 경계 검사 제거
- 루프 오버헤드 감소
- 구현 단순화

4. 장단점
-------
기본 순차 탐색:
장점:
- 구현 단순
- 정렬 불필요
- 작은 데이터에 효과적
단점:
- 느린 검색 속도
- 대용량 데이터에 부적합

전진이동법:
장점:
- 반복 검색 효율적
- 캐시 활용도 높음
단점:
- 이동 오버헤드
- 원본 순서 변경

보초법:
장점:
- 비교 연산 감소
- 구현 단순
단점:
- 추가 공간 필요
- 제한된 성능 향상

5. 응용 분야
----------
- 소규모 데이터 검색
- 정렬되지 않은 데이터
- 임시 데이터 구조
- 캐시 메모리 활용

6. 구현 특징
----------
- 세 가지 방식 비교
- 성능 측정
- 결과 검증
- 시각적 확인

이 구현은 순차 탐색의 세 가지
주요 방식을 모두 포함하며,
각각의 특성과 성능을 비교할 수
있는 교육용 프로그램입니다.
*/
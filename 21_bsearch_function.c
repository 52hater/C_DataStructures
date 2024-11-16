#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

/*
bsearch() 함수:
- C 표준 라이브러리의 이진 탐색 구현
- 정렬된 배열에서 키 값을 검색
- 다양한 데이터 타입 지원
- void 포인터와 비교 함수 사용
*/

typedef struct {
    int id;
    char name[50];
    float score;
} Student;

typedef struct {
    int x;
    int y;
} Point;

/* 정수 비교 함수
 * - bsearch()에서 사용할 기본 타입 비교
 */
int compare_int(const void* a, const void* b) {
    return (*(const int*)a - *(const int*)b);
}

/* 실수 비교 함수
 * - 부동소수점 비교를 위한 안전한 구현
 */
int compare_float(const void* a, const void* b) {
    float fa = *(const float*)a;
    float fb = *(const float*)b;
    if (fa < fb) return -1;
    if (fa > fb) return 1;
    return 0;
}

/* 문자열 비교 함수
 * - 문자열 포인터 배열을 위한 비교
 */
int compare_string(const void* a, const void* b) {
    return strcmp(*(const char**)a, *(const char**)b);
}

/* 학생 ID 비교 함수
 * - 구조체의 특정 필드(ID) 기준 비교
 */
int compare_student_by_id(const void* a, const void* b) {
    const Student* sa = (const Student*)a;
    const Student* sb = (const Student*)b;
    return (sa->id - sb->id);
}

/* 학생 이름 비교 함수
 * - 구조체의 문자열 필드 비교
 */
int compare_student_by_name(const void* a, const void* b) {
    const Student* sa = (const Student*)a;
    const Student* sb = (const Student*)b;
    return strcmp(sa->name, sb->name);
}

/* 점 거리 비교 함수 (원점으로부터)
 * - 복합 계산을 통한 비교
 */
int compare_point_distance(const void* a, const void* b) {
    const Point* pa = (const Point*)a;
    const Point* pb = (const Point*)b;
    int dist_a = pa->x * pa->x + pa->y * pa->y;
    int dist_b = pb->x * pb->x + pb->y * pb->y;
    return dist_a - dist_b;
}

/* 배열 정렬 상태 출력 함수들 */
void print_int_array(const int arr[], size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void print_float_array(const float arr[], size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%.2f ", arr[i]);
    }
    printf("\n");
}

void print_string_array(char* const arr[], size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%s ", arr[i]);
    }
    printf("\n");
}

void print_student_array(const Student arr[], size_t size) {
    printf("\nID\tName\t\tScore\n");
    printf("--------------------------------\n");
    for (size_t i = 0; i < size; i++) {
        printf("%d\t%-15s\t%.2f\n",
            arr[i].id, arr[i].name, arr[i].score);
    }
    printf("--------------------------------\n");
}

void print_point_array(const Point arr[], size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("(%d,%d) ", arr[i].x, arr[i].y);
    }
    printf("\n");
}

/* 시간 측정을 위한 함수
 * - 타입에 따른 bsearch 수행 시간 측정
 */
void measure_search_time(const void* key, const void* array, size_t size,
    size_t elem_size, int (*compare)(const void*, const void*),
    const char* search_type) {
    clock_t start = clock();
    void* result = bsearch(key, array, size, elem_size, compare);
    clock_t end = clock();

    double cpu_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Search time (%s): %.6f seconds\n", search_type, cpu_time);

    if (result != NULL) {
        printf("Element found!\n");
    }
    else {
        printf("Element not found\n");
    }
}

/* 메뉴 출력 */
void print_menu(void) {
    printf("\n=== Bsearch Examples Menu ===\n");
    printf("1. Search in integer array\n");
    printf("2. Search in float array\n");
    printf("3. Search in string array\n");
    printf("4. Search student by ID\n");
    printf("5. Search student by name\n");
    printf("6. Search point by distance\n");
    printf("0. Exit\n");
    printf("Choice: ");
}

int main(void) {
    // 정수 배열
    int int_arr[] = { 1, 3, 5, 7, 9, 11, 13, 15, 17, 19 };
    size_t int_size = sizeof(int_arr) / sizeof(int_arr[0]);

    // 실수 배열
    float float_arr[] = { 1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9 };
    size_t float_size = sizeof(float_arr) / sizeof(float_arr[0]);

    // 문자열 배열
    char* string_arr[] = { "apple", "banana", "grape", "orange", "pear" };
    size_t string_size = sizeof(string_arr) / sizeof(string_arr[0]);

    // 학생 배열
    Student student_arr[] = {
        {101, "Alice", 92.3},
        {102, "Bob", 78.9},
        {103, "Charlie", 85.5},
        {104, "David", 95.2},
        {105, "Eve", 88.7}
    };
    size_t student_size = sizeof(student_arr) / sizeof(student_arr[0]);

    // 점 배열
    Point point_arr[] = {
        {1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}
    };
    size_t point_size = sizeof(point_arr) / sizeof(point_arr[0]);

    // 모든 배열을 미리 정렬
    qsort(int_arr, int_size, sizeof(int), compare_int);
    qsort(float_arr, float_size, sizeof(float), compare_float);
    qsort(string_arr, string_size, sizeof(char*), compare_string);
    qsort(student_arr, student_size, sizeof(Student), compare_student_by_id);
    qsort(point_arr, point_size, sizeof(Point), compare_point_distance);

    int choice;
    do {
        print_menu();
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
        case 1: {  // Integer search
            int key;
            printf("\nSorted integer array: ");
            print_int_array(int_arr, int_size);
            printf("Enter integer to search: ");
            scanf("%d", &key);

            measure_search_time(&key, int_arr, int_size, sizeof(int),
                compare_int, "Integer");
            break;
        }

        case 2: {  // Float search
            float key;
            printf("\nSorted float array: ");
            print_float_array(float_arr, float_size);
            printf("Enter float to search: ");
            scanf("%f", &key);

            measure_search_time(&key, float_arr, float_size, sizeof(float),
                compare_float, "Float");
            break;
        }

        case 3: {  // String search
            char key[50];
            printf("\nSorted string array: ");
            print_string_array(string_arr, string_size);
            printf("Enter string to search: ");
            scanf("%s", key);

            char* key_ptr = key;
            measure_search_time(&key_ptr, string_arr, string_size,
                sizeof(char*), compare_string, "String");
            break;
        }

        case 4: {  // Student ID search
            int key_id;
            printf("\nSorted student array by ID:\n");
            print_student_array(student_arr, student_size);
            printf("Enter student ID to search: ");
            scanf("%d", &key_id);

            Student key = { key_id, "", 0 };
            measure_search_time(&key, student_arr, student_size,
                sizeof(Student), compare_student_by_id,
                "Student ID");
            break;
        }

        case 5: {  // Student name search
            char key_name[50];
            printf("\nSorted student array by name:\n");
            qsort(student_arr, student_size, sizeof(Student),
                compare_student_by_name);
            print_student_array(student_arr, student_size);
            printf("Enter student name to search: ");
            scanf("%s", key_name);

            Student key = { 0, "", 0 };
            strcpy(key.name, key_name);
            measure_search_time(&key, student_arr, student_size,
                sizeof(Student), compare_student_by_name,
                "Student Name");
            break;
        }

        case 6: {  // Point search
            int x, y;
            printf("\nSorted point array by distance: ");
            print_point_array(point_arr, point_size);
            printf("Enter point coordinates (x y): ");
            scanf("%d %d", &x, &y);

            Point key = { x, y };
            measure_search_time(&key, point_arr, point_size, sizeof(Point),
                compare_point_distance, "Point");
            break;
        }

        case 0:
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

1. bsearch 함수의 원형
------------------
void* bsearch(const void* key,
              const void* base,
              size_t nmemb,
              size_t size,
              int (*compar)(const void*, const void*));

- key: 찾을 값의 포인터
- base: 검색할 배열의 포인터
- nmemb: 배열의 요소 개수
- size: 각 요소의 크기
- compar: 비교 함수 포인터

2. 비교 함수 요구사항
-----------------
반환값:
< 0: 첫 번째 요소가 작음
= 0: 두 요소가 같음
> 0: 첫 번째 요소가 큼

주의사항:
- 일관된 결과 반환
- 대칭성 유지
- 전이성 유지

3. 주요 특징
----------
장점:
- 표준 라이브러리 제공
- 타입 독립적
- 최적화된 구현
- 범용성

단점:
- void* 캐스팅 필요
- 비교 함수 오버헤드
- 정렬된 배열 필요
- 불안정 검색

4. 구현 특징
----------
- 다양한 데이터 타입
- 구조체 검색
- 복합 조건 검색
- 성능 측정

5. 활용 분야
----------
- 정렬된 데이터 검색
- 데이터베이스 인덱싱
- 구조체 배열 검색
- 표준 라이브러리 활용

이 구현은 bsearch 함수의 다양한
활용 예제를 제공하며, 실제
프로그래밍에서 자주 사용되는
검색 시나리오를 다룹니다.
*/
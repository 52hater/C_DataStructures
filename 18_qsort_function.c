#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

// 다양한 데이터 타입을 위한 구조체 정의
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
 * - 매개변수: a, b - 비교할 두 요소의 포인터
 * - 반환값: 음수(a<b), 0(a=b), 양수(a>b)
 */
int compare_int(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

/* 실수 비교 함수
 * - 매개변수: a, b - 비교할 두 요소의 포인터
 */
int compare_float(const void* a, const void* b) {
    float diff = (*(float*)a - *(float*)b);
    if (diff < 0) return -1;
    if (diff > 0) return 1;
    return 0;
}

/* 문자열 비교 함수
 * - 매개변수: a, b - 비교할 두 요소의 포인터
 */
int compare_string(const void* a, const void* b) {
    return strcmp(*(const char**)a, *(const char**)b);
}

/* 학생 ID 기준 비교 함수
 * - 매개변수: a, b - 비교할 두 요소의 포인터
 */
int compare_student_by_id(const void* a, const void* b) {
    return ((Student*)a)->id - ((Student*)b)->id;
}

/* 학생 이름 기준 비교 함수
 * - 매개변수: a, b - 비교할 두 요소의 포인터
 */
int compare_student_by_name(const void* a, const void* b) {
    return strcmp(((Student*)a)->name, ((Student*)b)->name);
}

/* 학생 성적 기준 비교 함수 (내림차순)
 * - 매개변수: a, b - 비교할 두 요소의 포인터
 */
int compare_student_by_score_desc(const void* a, const void* b) {
    float diff = ((Student*)b)->score - ((Student*)a)->score;
    if (diff < 0) return -1;
    if (diff > 0) return 1;
    return 0;
}

/* 점 거리 기준 비교 함수 (원점으로부터)
 * - 매개변수: a, b - 비교할 두 요소의 포인터
 */
int compare_point_distance(const void* a, const void* b) {
    Point* p1 = (Point*)a;
    Point* p2 = (Point*)b;
    int dist1 = p1->x * p1->x + p1->y * p1->y;
    int dist2 = p2->x * p2->x + p2->y * p2->y;
    return dist1 - dist2;
}

/* 배열 출력 함수들 */
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

void print_string_array(char* arr[], size_t size) {
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
        printf("(%d, %d) ", arr[i].x, arr[i].y);
    }
    printf("\n");
}

/* 알고리즘 실행 시간 측정
 * - 매개변수: arr - 정렬할 배열, size - 배열 크기,
 *            elem_size - 요소 크기, compare - 비교 함수
 */
void measure_sort_time(void* arr, size_t size, size_t elem_size,
    int (*compare)(const void*, const void*)) {
    clock_t start = clock();
    qsort(arr, size, elem_size, compare);
    clock_t end = clock();

    double cpu_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Sorting time: %.6f seconds\n", cpu_time);
}

/* 메뉴 출력 */
void print_menu(void) {
    printf("\n=== Qsort Examples Menu ===\n");
    printf("1. Sort integers\n");
    printf("2. Sort floating points\n");
    printf("3. Sort strings\n");
    printf("4. Sort students by ID\n");
    printf("5. Sort students by name\n");
    printf("6. Sort students by score (descending)\n");
    printf("7. Sort points by distance from origin\n");
    printf("0. Exit\n");
    printf("Choice: ");
}

int main(void) {
    int choice;
    srand((unsigned int)time(NULL));

    // 테스트 데이터 준비
    int int_arr[] = { 64, 34, 25, 12, 22, 11, 90 };
    float float_arr[] = { 64.5, 34.2, 25.7, 12.3, 22.8, 11.1, 90.9 };
    char* string_arr[] = { "banana", "apple", "orange", "grape", "kiwi" };

    Student student_arr[] = {
        {103, "Charlie", 85.5},
        {101, "Alice", 92.3},
        {105, "Eve", 88.7},
        {102, "Bob", 78.9},
        {104, "David", 95.2}
    };

    Point point_arr[] = {
        {3, 4}, {1, 2}, {5, 12}, {2, 2}, {8, 15}
    };

    size_t int_size = sizeof(int_arr) / sizeof(int_arr[0]);
    size_t float_size = sizeof(float_arr) / sizeof(float_arr[0]);
    size_t string_size = sizeof(string_arr) / sizeof(string_arr[0]);
    size_t student_size = sizeof(student_arr) / sizeof(student_arr[0]);
    size_t point_size = sizeof(point_arr) / sizeof(point_arr[0]);

    do {
        print_menu();
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');
            continue;
        }

        printf("\n");
        switch (choice) {
        case 1:  // Sort integers
            printf("Before sorting:\n");
            print_int_array(int_arr, int_size);

            measure_sort_time(int_arr, int_size, sizeof(int), compare_int);

            printf("After sorting:\n");
            print_int_array(int_arr, int_size);
            break;

        case 2:  // Sort floating points
            printf("Before sorting:\n");
            print_float_array(float_arr, float_size);

            measure_sort_time(float_arr, float_size, sizeof(float),
                compare_float);

            printf("After sorting:\n");
            print_float_array(float_arr, float_size);
            break;

        case 3:  // Sort strings
            printf("Before sorting:\n");
            print_string_array(string_arr, string_size);

            measure_sort_time(string_arr, string_size, sizeof(char*),
                compare_string);

            printf("After sorting:\n");
            print_string_array(string_arr, string_size);
            break;

        case 4:  // Sort students by ID
            printf("Before sorting:\n");
            print_student_array(student_arr, student_size);

            measure_sort_time(student_arr, student_size, sizeof(Student),
                compare_student_by_id);

            printf("After sorting:\n");
            print_student_array(student_arr, student_size);
            break;

        case 5:  // Sort students by name
            printf("Before sorting:\n");
            print_student_array(student_arr, student_size);

            measure_sort_time(student_arr, student_size, sizeof(Student),
                compare_student_by_name);

            printf("After sorting:\n");
            print_student_array(student_arr, student_size);
            break;

        case 6:  // Sort students by score
            printf("Before sorting:\n");
            print_student_array(student_arr, student_size);

            measure_sort_time(student_arr, student_size, sizeof(Student),
                compare_student_by_score_desc);

            printf("After sorting:\n");
            print_student_array(student_arr, student_size);
            break;

        case 7:  // Sort points
            printf("Before sorting:\n");
            print_point_array(point_arr, point_size);

            measure_sort_time(point_arr, point_size, sizeof(Point),
                compare_point_distance);

            printf("After sorting:\n");
            print_point_array(point_arr, point_size);
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

1. qsort 함수의 원형
-----------------
void qsort(void* base,
           size_t nmemb,
           size_t size,
           int (*compar)(const void*, const void*));

- base: 정렬할 배열의 시작 주소
- nmemb: 배열의 요소 개수
- size: 각 요소의 크기 (바이트)
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
- 반환값의 부호만 중요

3. 주요 특징
----------
장점:
- 타입 독립적
- 표준 라이브러리 제공
- 최적화된 구현
- 범용성

단점:
- 비교 함수 오버헤드
- void* 캐스팅 필요
- 불안정 정렬
- 특수화 어려움

4. 활용 분야
----------
- 범용 정렬
- 복합 데이터 정렬
- 다중 기준 정렬
- 표준 라이브러리 활용

5. 구현 특징
----------
- 다양한 데이터 타입 지원
- 여러 정렬 기준 제공
- 성능 측정 기능
- 시각적 결과 확인

6. 교육적 가치
-----------
- 함수 포인터 이해
- void 포인터 활용
- 타입 독립적 프로그래밍
- 표준 라이브러리 사용법

7. 실무적 특징
-----------
- 재사용성 높음
- 유지보수 용이
- 표준 준수
- 이식성 좋음

8. 주의사항
---------
- 포인터 타입 주의
- 메모리 정렬 고려
- 비교 함수 정확성
- 안정성 불보장

이 구현은 qsort 함수의 다양한
활용 예제를 제공하며, 실제
프로그래밍에서 자주 사용되는
정렬 시나리오를 다룹니다.
*/
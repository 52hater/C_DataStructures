#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

// �پ��� ������ Ÿ���� ���� ����ü ����
typedef struct {
    int id;
    char name[50];
    float score;
} Student;

typedef struct {
    int x;
    int y;
} Point;

/* ���� �� �Լ�
 * - �Ű�����: a, b - ���� �� ����� ������
 * - ��ȯ��: ����(a<b), 0(a=b), ���(a>b)
 */
int compare_int(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

/* �Ǽ� �� �Լ�
 * - �Ű�����: a, b - ���� �� ����� ������
 */
int compare_float(const void* a, const void* b) {
    float diff = (*(float*)a - *(float*)b);
    if (diff < 0) return -1;
    if (diff > 0) return 1;
    return 0;
}

/* ���ڿ� �� �Լ�
 * - �Ű�����: a, b - ���� �� ����� ������
 */
int compare_string(const void* a, const void* b) {
    return strcmp(*(const char**)a, *(const char**)b);
}

/* �л� ID ���� �� �Լ�
 * - �Ű�����: a, b - ���� �� ����� ������
 */
int compare_student_by_id(const void* a, const void* b) {
    return ((Student*)a)->id - ((Student*)b)->id;
}

/* �л� �̸� ���� �� �Լ�
 * - �Ű�����: a, b - ���� �� ����� ������
 */
int compare_student_by_name(const void* a, const void* b) {
    return strcmp(((Student*)a)->name, ((Student*)b)->name);
}

/* �л� ���� ���� �� �Լ� (��������)
 * - �Ű�����: a, b - ���� �� ����� ������
 */
int compare_student_by_score_desc(const void* a, const void* b) {
    float diff = ((Student*)b)->score - ((Student*)a)->score;
    if (diff < 0) return -1;
    if (diff > 0) return 1;
    return 0;
}

/* �� �Ÿ� ���� �� �Լ� (�������κ���)
 * - �Ű�����: a, b - ���� �� ����� ������
 */
int compare_point_distance(const void* a, const void* b) {
    Point* p1 = (Point*)a;
    Point* p2 = (Point*)b;
    int dist1 = p1->x * p1->x + p1->y * p1->y;
    int dist2 = p2->x * p2->x + p2->y * p2->y;
    return dist1 - dist2;
}

/* �迭 ��� �Լ��� */
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

/* �˰��� ���� �ð� ����
 * - �Ű�����: arr - ������ �迭, size - �迭 ũ��,
 *            elem_size - ��� ũ��, compare - �� �Լ�
 */
void measure_sort_time(void* arr, size_t size, size_t elem_size,
    int (*compare)(const void*, const void*)) {
    clock_t start = clock();
    qsort(arr, size, elem_size, compare);
    clock_t end = clock();

    double cpu_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Sorting time: %.6f seconds\n", cpu_time);
}

/* �޴� ��� */
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

    // �׽�Ʈ ������ �غ�
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
�� ���� �� �ֿ� ����
==========================================

1. qsort �Լ��� ����
-----------------
void qsort(void* base,
           size_t nmemb,
           size_t size,
           int (*compar)(const void*, const void*));

- base: ������ �迭�� ���� �ּ�
- nmemb: �迭�� ��� ����
- size: �� ����� ũ�� (����Ʈ)
- compar: �� �Լ� ������

2. �� �Լ� �䱸����
-----------------
��ȯ��:
< 0: ù ��° ��Ұ� ����
= 0: �� ��Ұ� ����
> 0: ù ��° ��Ұ� ŭ

���ǻ���:
- �ϰ��� ��� ��ȯ
- ��Ī�� ����
- ��ȯ���� ��ȣ�� �߿�

3. �ֿ� Ư¡
----------
����:
- Ÿ�� ������
- ǥ�� ���̺귯�� ����
- ����ȭ�� ����
- ���뼺

����:
- �� �Լ� �������
- void* ĳ���� �ʿ�
- �Ҿ��� ����
- Ư��ȭ �����

4. Ȱ�� �о�
----------
- ���� ����
- ���� ������ ����
- ���� ���� ����
- ǥ�� ���̺귯�� Ȱ��

5. ���� Ư¡
----------
- �پ��� ������ Ÿ�� ����
- ���� ���� ���� ����
- ���� ���� ���
- �ð��� ��� Ȯ��

6. ������ ��ġ
-----------
- �Լ� ������ ����
- void ������ Ȱ��
- Ÿ�� ������ ���α׷���
- ǥ�� ���̺귯�� ����

7. �ǹ��� Ư¡
-----------
- ���뼺 ����
- �������� ����
- ǥ�� �ؼ�
- �̽ļ� ����

8. ���ǻ���
---------
- ������ Ÿ�� ����
- �޸� ���� ���
- �� �Լ� ��Ȯ��
- ������ �Һ���

�� ������ qsort �Լ��� �پ���
Ȱ�� ������ �����ϸ�, ����
���α׷��ֿ��� ���� ���Ǵ�
���� �ó������� �ٷ�ϴ�.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

/*
bsearch() �Լ�:
- C ǥ�� ���̺귯���� ���� Ž�� ����
- ���ĵ� �迭���� Ű ���� �˻�
- �پ��� ������ Ÿ�� ����
- void �����Ϳ� �� �Լ� ���
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

/* ���� �� �Լ�
 * - bsearch()���� ����� �⺻ Ÿ�� ��
 */
int compare_int(const void* a, const void* b) {
    return (*(const int*)a - *(const int*)b);
}

/* �Ǽ� �� �Լ�
 * - �ε��Ҽ��� �񱳸� ���� ������ ����
 */
int compare_float(const void* a, const void* b) {
    float fa = *(const float*)a;
    float fb = *(const float*)b;
    if (fa < fb) return -1;
    if (fa > fb) return 1;
    return 0;
}

/* ���ڿ� �� �Լ�
 * - ���ڿ� ������ �迭�� ���� ��
 */
int compare_string(const void* a, const void* b) {
    return strcmp(*(const char**)a, *(const char**)b);
}

/* �л� ID �� �Լ�
 * - ����ü�� Ư�� �ʵ�(ID) ���� ��
 */
int compare_student_by_id(const void* a, const void* b) {
    const Student* sa = (const Student*)a;
    const Student* sb = (const Student*)b;
    return (sa->id - sb->id);
}

/* �л� �̸� �� �Լ�
 * - ����ü�� ���ڿ� �ʵ� ��
 */
int compare_student_by_name(const void* a, const void* b) {
    const Student* sa = (const Student*)a;
    const Student* sb = (const Student*)b;
    return strcmp(sa->name, sb->name);
}

/* �� �Ÿ� �� �Լ� (�������κ���)
 * - ���� ����� ���� ��
 */
int compare_point_distance(const void* a, const void* b) {
    const Point* pa = (const Point*)a;
    const Point* pb = (const Point*)b;
    int dist_a = pa->x * pa->x + pa->y * pa->y;
    int dist_b = pb->x * pb->x + pb->y * pb->y;
    return dist_a - dist_b;
}

/* �迭 ���� ���� ��� �Լ��� */
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

/* �ð� ������ ���� �Լ�
 * - Ÿ�Կ� ���� bsearch ���� �ð� ����
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

/* �޴� ��� */
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
    // ���� �迭
    int int_arr[] = { 1, 3, 5, 7, 9, 11, 13, 15, 17, 19 };
    size_t int_size = sizeof(int_arr) / sizeof(int_arr[0]);

    // �Ǽ� �迭
    float float_arr[] = { 1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9 };
    size_t float_size = sizeof(float_arr) / sizeof(float_arr[0]);

    // ���ڿ� �迭
    char* string_arr[] = { "apple", "banana", "grape", "orange", "pear" };
    size_t string_size = sizeof(string_arr) / sizeof(string_arr[0]);

    // �л� �迭
    Student student_arr[] = {
        {101, "Alice", 92.3},
        {102, "Bob", 78.9},
        {103, "Charlie", 85.5},
        {104, "David", 95.2},
        {105, "Eve", 88.7}
    };
    size_t student_size = sizeof(student_arr) / sizeof(student_arr[0]);

    // �� �迭
    Point point_arr[] = {
        {1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}
    };
    size_t point_size = sizeof(point_arr) / sizeof(point_arr[0]);

    // ��� �迭�� �̸� ����
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
�� ���� �� �ֿ� ����
==========================================

1. bsearch �Լ��� ����
------------------
void* bsearch(const void* key,
              const void* base,
              size_t nmemb,
              size_t size,
              int (*compar)(const void*, const void*));

- key: ã�� ���� ������
- base: �˻��� �迭�� ������
- nmemb: �迭�� ��� ����
- size: �� ����� ũ��
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
- ���̼� ����

3. �ֿ� Ư¡
----------
����:
- ǥ�� ���̺귯�� ����
- Ÿ�� ������
- ����ȭ�� ����
- ���뼺

����:
- void* ĳ���� �ʿ�
- �� �Լ� �������
- ���ĵ� �迭 �ʿ�
- �Ҿ��� �˻�

4. ���� Ư¡
----------
- �پ��� ������ Ÿ��
- ����ü �˻�
- ���� ���� �˻�
- ���� ����

5. Ȱ�� �о�
----------
- ���ĵ� ������ �˻�
- �����ͺ��̽� �ε���
- ����ü �迭 �˻�
- ǥ�� ���̺귯�� Ȱ��

�� ������ bsearch �Լ��� �پ���
Ȱ�� ������ �����ϸ�, ����
���α׷��ֿ��� ���� ���Ǵ�
�˻� �ó������� �ٷ�ϴ�.
*/
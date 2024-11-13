#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

typedef int DataType;

/* �� ������ ��ȯ
 * - �Ű�����: a, b - ��ȯ�� �� ������ ������
 */
void swap(DataType* a, DataType* b) {
    DataType temp = *a;
    *a = *b;
    *b = temp;
}

/* �⺻ ���� ����
 * - �Ű�����: arr - ������ �迭, size - �迭�� ũ��
 * - ����: ������ �� ���Ҹ� ���Ͽ� ��ȯ
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

/* ����ȭ�� ���� ����
 * - �Ű�����: arr - ������ �迭, size - �迭�� ũ��
 * - ����: ��ȯ�� �Ͼ�� ������ ���� ����
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
            break;  // ��ȯ�� ������ �̹� ���ĵ� ����
        }
    }
}

/* �迭 ���
 * - �Ű�����: arr - ����� �迭, size - �迭�� ũ��
 */
void print_array(const DataType arr[], size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

/* �迭�� ���ĵǾ����� ����
 * - �Ű�����: arr - ������ �迭, size - �迭�� ũ��
 * - ��ȯ��: ���ĵǾ����� true, �ƴϸ� false
 */
bool is_sorted(const DataType arr[], size_t size) {
    for (size_t i = 0; i < size - 1; i++) {
        if (arr[i] > arr[i + 1]) {
            return false;
        }
    }
    return true;
}

/* �迭�� �������� ����
 * - �Ű�����: arr - ���� �迭, size - �迭�� ũ��
 */
void shuffle_array(DataType arr[], size_t size) {
    for (size_t i = size - 1; i > 0; i--) {
        size_t j = rand() % (i + 1);
        swap(&arr[i], &arr[j]);
    }
}

/* ���� ������ ���� �迭 ����
 * - �Ű�����: dest - ������ �迭, src - ���� �迭, size - �迭�� ũ��
 */
void copy_array(DataType dest[], const DataType src[], size_t size) {
    for (size_t i = 0; i < size; i++) {
        dest[i] = src[i];
    }
}

/* �˰��� ���� �ð� ����
 * - �Ű�����: sort_func - ���� �Լ� ������, arr - ������ �迭,
 *            size - �迭�� ũ��, name - �˰��� �̸�
 */
void measure_time(void (*sort_func)(DataType[], size_t),
    DataType arr[], size_t size, const char* name) {
    clock_t start = clock();
    sort_func(arr, size);
    clock_t end = clock();

    double cpu_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("%s: %.6f seconds\n", name, cpu_time);
}

/* �޴� ��� */
void print_menu(void) {
    printf("\n=== Bubble Sort Menu ===\n");
    printf("1. Run basic bubble sort\n");
    printf("2. Run optimized bubble sort\n");
    printf("3. Compare both versions\n");
    printf("4. Generate new random array\n");
    printf("0. Exit\n");
    printf("Choice: ");
}

/* ���ο� ������ �迭 ����
 * - �Ű�����: arr - ������ �迭, size - �迭�� ũ��
 */
void generate_random_array(DataType arr[], size_t size) {
    for (size_t i = 0; i < size; i++) {
        arr[i] = rand() % 1000;  // 0-999 ������ ������ ��
    }
}

int main(void) {
    srand((unsigned int)time(NULL));  // ���� �߻��� �ʱ�ȭ

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

            // �⺻ ���� ����
            copy_array(temp_arr, arr, size);
            measure_time(bubble_sort_basic, temp_arr, size, "Basic bubble sort");

            // ����ȭ�� ���� ����
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
�� ���� �� �ֿ� ����
==========================================

1. ���� ������ �⺻ ����
-------------------
- ������ �� ���Ҹ� ���Ͽ� ��ȯ
- �� �н����� �ִ밪�� ������ �̵�
- n-1���� �н� �ʿ�
- ���� ���� (Stable Sort)

2. �ð� ���⵵
-----------
�־��� ���: O(n��)
- �������� ���ĵ� ���
- n(n-1)/2���� ��

����� ���: O(n��)
- �������� ��ġ�� ���

�ּ��� ���: O(n)
- �̹� ���ĵ� ��� (����ȭ ����)

3. ���� ���⵵
-----------
O(1)
- �߰� �޸� ���ʿ�
- ���ڸ� ���� (In-place Sort)

4. ����ȭ ���
-----------
Early Termination:
- ��ȯ�� ������ ����
- �̹� ���ĵ� ��� ����
- ��� ���� �ð� ����

���� ���� ����ȭ:
- ���ĵ� �κ� ����
- ���ʿ��� �� ����

5. �����
-------
����:
- ������ �ܼ�
- ���� ����
- �߰� �޸� ���ʿ�

����:
- ���� ���� �ӵ�
- ���� ��ȯ ����
- ū �����Ϳ� ��ȿ����

6. ������ ��ġ
-----------
- �⺻ ���� ���� ����
- �˰��� ����ȭ �н�
- �ð����⵵ �м� ����

7. ���� Ư¡
----------
- Ÿ�� �߻�ȭ (DataType)
- ���� ���� ���
- ��� ���� ���
- ��ȭ�� �������̽�

8. Ȱ�� �о�
----------
- ������ ����
- ���� �����ͼ�
- ���� ���ĵ� ������
- ������ ���� �۾�

�� ������ ���� ������ �⺻ �����
����ȭ ����� ��� �����ϸ�,
���� ������ �����ϰ� ���� �� �ִ�
������ ���α׷��� �����մϴ�.
*/
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

/* ���� ����
 * - �Ű�����: arr - ������ �迭, size - �迭�� ũ��
 * - ����: �� �н����� �ּҰ��� ã�� �� ������ �̵�
 */
void selection_sort(DataType arr[], size_t size) {
    for (size_t i = 0; i < size - 1; i++) {
        size_t min_idx = i;

        // i���� ������ �ּҰ� ã��
        for (size_t j = i + 1; j < size; j++) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }

        // �ּҰ��� ���� ��ġ�� �ƴϸ� ��ȯ
        if (min_idx != i) {
            swap(&arr[i], &arr[min_idx]);
        }
    }
}

/* ����ȭ�� ���� ����
 * - �Ű�����: arr - ������ �迭, size - �迭�� ũ��
 * - ����: �� �н����� �ּҰ��� �ִ밪�� ���ÿ� ã�� ���ʿ��� ����
 */
void selection_sort_optimized(DataType arr[], size_t size) {
    for (size_t i = 0; i < size / 2; i++) {
        size_t min_idx = i;
        size_t max_idx = i;

        // �ּҰ��� �ִ밪�� ���ÿ� ã��
        for (size_t j = i + 1; j < size - i; j++) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
            if (arr[j] > arr[max_idx]) {
                max_idx = j;
            }
        }

        // �ּҰ��� ������
        if (min_idx != i) {
            swap(&arr[i], &arr[min_idx]);
            // �ִ밪�� ó�� ��ġ�� �־��ٸ� �ε��� ����
            if (max_idx == i) {
                max_idx = min_idx;
            }
        }

        // �ִ밪�� �ڷ�
        if (max_idx != size - 1 - i) {
            swap(&arr[size - 1 - i], &arr[max_idx]);
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

/* �迭 ����
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

/* ���ο� ������ �迭 ����
 * - �Ű�����: arr - ������ �迭, size - �迭�� ũ��
 */
void generate_random_array(DataType arr[], size_t size) {
    for (size_t i = 0; i < size; i++) {
        arr[i] = rand() % 1000;  // 0-999 ������ ������ ��
    }
}

/* �޴� ��� */
void print_menu(void) {
    printf("\n=== Selection Sort Menu ===\n");
    printf("1. Run basic selection sort\n");
    printf("2. Run optimized selection sort\n");
    printf("3. Compare both versions\n");
    printf("4. Generate new random array\n");
    printf("0. Exit\n");
    printf("Choice: ");
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
        case 1:  // Basic selection sort
            copy_array(temp_arr, arr, size);
            printf("\nArray before sorting:\n");
            print_array(temp_arr, size);

            measure_time(selection_sort, temp_arr, size, "Basic selection sort");

            printf("Array after sorting:\n");
            print_array(temp_arr, size);
            printf("Verification: %s\n",
                is_sorted(temp_arr, size) ? "PASSED" : "FAILED");
            break;

        case 2:  // Optimized selection sort
            copy_array(temp_arr, arr, size);
            printf("\nArray before sorting:\n");
            print_array(temp_arr, size);

            measure_time(selection_sort_optimized, temp_arr, size,
                "Optimized selection sort");

            printf("Array after sorting:\n");
            print_array(temp_arr, size);
            printf("Verification: %s\n",
                is_sorted(temp_arr, size) ? "PASSED" : "FAILED");
            break;

        case 3:  // Compare both versions
            printf("\nComparing both versions with the same input:\n");

            // �⺻ ���� ����
            copy_array(temp_arr, arr, size);
            measure_time(selection_sort, temp_arr, size, "Basic selection sort");

            // ����ȭ�� ���� ����
            copy_array(temp_arr, arr, size);
            measure_time(selection_sort_optimized, temp_arr, size,
                "Optimized selection sort");
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
- ��ü���� �ּҰ��� ����
- ���õ� ���� �� ������ �̵�
- ���ĵ� �κ��� �����ϰ� �ݺ�
- �Ҿ��� ���� (Unstable Sort)

2. �ð� ���⵵
-----------
�־��� ���: O(n��)
- ��� ��� ����
- n(n-1)/2���� ��

����� ���: O(n��)
- �Է¿� ������� ����

�ּ��� ���: O(n��)
- �̹� ���ĵǾ� �־ ����

3. ���� ���⵵
-----------
O(1)
- �߰� �޸� ���ʿ�
- ���ڸ� ���� (In-place Sort)

4. ����ȭ ���
-----------
����� ����:
- �ּҰ��� �ִ밪 ���� Ž��
- �� Ƚ���� �����ϳ� ��ȯ ����ȭ
- �ݺ� Ƚ�� �������� ����

���ʿ��� ��ȯ ����:
- ���� ��ġ�� �ּҰ��� �� ��ȯ ����
- �޸� ���� Ƚ�� ����

5. ���� ���İ��� ��
-----------------
����:
- ��ȯ Ƚ���� ���� (O(n))
- �޸� ���� ����
- ������ ������

����:
- �׻� O(n��) �� �ʿ�
- �Ҿ��� ����
- ����ȭ ������ ����

6. �����
-------
����:
- ������ �ܼ�
- ��ȯ Ƚ���� ����
- �߰� �޸� ���ʿ�

����:
- ���� ���� �ӵ�
- �Ҿ��� ����
- �Է¿� �ΰ����� ����

7. ������ ��ġ
-----------
- ���� Ž�� ����
- ���� �˰��� �м�
- ����ȭ ��� �н�

8. ���� Ư¡
----------
- ����� ����ȭ
- ���� ����
- ��� ����
- ��ȭ�� �������̽�

9. Ȱ�� �о�
----------
- ���� �����ͼ�
- �޸� ���Ⱑ ��� ���
- ������ ���� �۾�
- ������ ����

�� ������ ���� ������ �⺻���� Ư����
����ȭ ����� �����ϸ�, ������
������ �����ϰ� ���� �� �ִ�
������ ���α׷��� �����մϴ�.
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

typedef int DataType;

/* ���� �Լ� (�⺻ ����)
 * - �Ű�����: arr - ���� �迭, left, mid, right - ������ ����
 *            temp - �ӽ� �迭
 */
void merge(DataType arr[], size_t left, size_t mid, size_t right, DataType temp[]) {
    size_t i = left;    // ���� �迭 �ε���
    size_t j = mid + 1; // ������ �迭 �ε���
    size_t k = left;    // �ӽ� �迭 �ε���

    // �� �迭�� ���ϸ� �ӽ� �迭�� ����
    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) {
            temp[k++] = arr[i++];
        }
        else {
            temp[k++] = arr[j++];
        }
    }

    // ���� �迭�� ���� ��� ����
    while (i <= mid) {
        temp[k++] = arr[i++];
    }

    // ������ �迭�� ���� ��� ����
    while (j <= right) {
        temp[k++] = arr[j++];
    }

    // �ӽ� �迭�� ������ ���� �迭�� ����
    for (k = left; k <= right; k++) {
        arr[k] = temp[k];
    }
}

/* ���� �Լ� (����ȭ ����)
 * - �Ű�����: arr - ���� �迭, left, mid, right - ������ ����
 *            temp - �ӽ� �迭
 */
void merge_optimized(DataType arr[], size_t left, size_t mid, size_t right,
    DataType temp[]) {
    size_t i = left;
    size_t j = mid + 1;
    size_t k = left;

    // �̸� ���Ͽ� ������ �ʿ� ���� ��� ó��
    if (arr[mid] <= arr[j]) {
        return;  // �̹� ���ĵ� ����
    }

    // �� �迭�� ���ϸ� �ӽ� �迭�� ����
    while (i <= mid && j <= right) {
        temp[k++] = (arr[i] <= arr[j]) ? arr[i++] : arr[j++];
    }

    // ���� �迭�� ���� ��� ����
    while (i <= mid) {
        temp[k++] = arr[i++];
    }

    // ������ �迭�� ���� ��� ����
    while (j <= right) {
        temp[k++] = arr[j++];
    }

    // �ӽ� �迭�� ������ ���� �迭�� ����
    for (k = left; k <= right; k++) {
        arr[k] = temp[k];
    }
}

/* �⺻ ���� ���� (��� ����)
 * - �Ű�����: arr - ������ �迭, left, right - ������ ����
 *            temp - �ӽ� �迭
 */
void merge_sort_recursive(DataType arr[], size_t left, size_t right,
    DataType temp[]) {
    if (left < right) {
        size_t mid = (left + right) / 2;
        merge_sort_recursive(arr, left, mid, temp);
        merge_sort_recursive(arr, mid + 1, right, temp);
        merge(arr, left, mid, right, temp);
    }
}

/* ����ȭ�� ���� ���� (��� ����)
 * - �Ű�����: arr - ������ �迭, left, right - ������ ����
 *            temp - �ӽ� �迭
 */
void merge_sort_optimized_recursive(DataType arr[], size_t left, size_t right,
    DataType temp[]) {
    // ���� �迭�� ���� ���� ���
    if (right - left <= 10) {
        // ���� ���� ����
        for (size_t i = left + 1; i <= right; i++) {
            DataType key = arr[i];
            size_t j = i - 1;
            while (j >= left && arr[j] > key) {
                arr[j + 1] = arr[j];
                j--;
            }
            arr[j + 1] = key;
        }
        return;
    }

    if (left < right) {
        size_t mid = (left + right) / 2;
        merge_sort_optimized_recursive(arr, left, mid, temp);
        merge_sort_optimized_recursive(arr, mid + 1, right, temp);
        merge_optimized(arr, left, mid, right, temp);
    }
}

/* �ݺ��� ���� ����
 * - �Ű�����: arr - ������ �迭, size - �迭�� ũ��
 *            temp - �ӽ� �迭
 */
void merge_sort_iterative(DataType arr[], size_t size, DataType temp[]) {
    for (size_t width = 1; width < size; width *= 2) {
        for (size_t left = 0; left < size; left += 2 * width) {
            size_t mid = (left + width < size) ? left + width - 1 : size - 1;
            size_t right = (left + 2 * width < size) ? left + 2 * width - 1 : size - 1;
            merge(arr, left, mid, right, temp);
        }
    }
}

/* Wrapper �Լ� - �⺻ ���� ����
 * - �Ű�����: arr - ������ �迭, size - �迭�� ũ��
 */
void merge_sort_basic(DataType arr[], size_t size) {
    DataType* temp = (DataType*)malloc(size * sizeof(DataType));
    if (temp == NULL) {
        printf("Memory allocation failed\n");
        return;
    }
    merge_sort_recursive(arr, 0, size - 1, temp);
    free(temp);
}

/* Wrapper �Լ� - ����ȭ�� ���� ����
 * - �Ű�����: arr - ������ �迭, size - �迭�� ũ��
 */
void merge_sort_optimized(DataType arr[], size_t size) {
    DataType* temp = (DataType*)malloc(size * sizeof(DataType));
    if (temp == NULL) {
        printf("Memory allocation failed\n");
        return;
    }
    merge_sort_optimized_recursive(arr, 0, size - 1, temp);
    free(temp);
}

/* Wrapper �Լ� - �ݺ��� ���� ����
 * - �Ű�����: arr - ������ �迭, size - �迭�� ũ��
 */
void merge_sort_iterative_wrapper(DataType arr[], size_t size) {
    DataType* temp = (DataType*)malloc(size * sizeof(DataType));
    if (temp == NULL) {
        printf("Memory allocation failed\n");
        return;
    }
    merge_sort_iterative(arr, size, temp);
    free(temp);
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

/* ���� ���ĵ� �迭 ����
 * - �Ű�����: arr - ������ �迭, size - �迭�� ũ��
 */
void generate_nearly_sorted_array(DataType arr[], size_t size) {
    for (size_t i = 0; i < size; i++) {
        arr[i] = i;
    }
    // �� 5%�� ���Ҹ� �������� ����
    for (size_t i = 0; i < size / 20; i++) {
        size_t idx1 = rand() % size;
        size_t idx2 = rand() % size;
        DataType temp = arr[idx1];
        arr[idx1] = arr[idx2];
        arr[idx2] = temp;
    }
}

/* �޴� ��� */
void print_menu(void) {
    printf("\n=== Merge Sort Menu ===\n");
    printf("1. Run basic merge sort\n");
    printf("2. Run optimized merge sort\n");
    printf("3. Run iterative merge sort\n");
    printf("4. Compare all versions\n");
    printf("5. Generate new random array\n");
    printf("6. Generate nearly sorted array\n");
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
        case 1:  // Basic merge sort
            copy_array(temp_arr, arr, size);
            printf("\nArray before sorting:\n");
            print_array(temp_arr, size);

            measure_time(merge_sort_basic, temp_arr, size,
                "Basic merge sort");

            printf("Array after sorting:\n");
            print_array(temp_arr, size);
            printf("Verification: %s\n",
                is_sorted(temp_arr, size) ? "PASSED" : "FAILED");
            break;

        case 2:  // Optimized merge sort
            copy_array(temp_arr, arr, size);
            printf("\nArray before sorting:\n");
            print_array(temp_arr, size);

            measure_time(merge_sort_optimized, temp_arr, size,
                "Optimized merge sort");

            printf("Array after sorting:\n");
            print_array(temp_arr, size);
            printf("Verification: %s\n",
                is_sorted(temp_arr, size) ? "PASSED" : "FAILED");
            break;

        case 3:  // Iterative merge sort
            copy_array(temp_arr, arr, size);
            printf("\nArray before sorting:\n");
            print_array(temp_arr, size);

            measure_time(merge_sort_iterative_wrapper, temp_arr, size,
                "Iterative merge sort");

            printf("Array after sorting:\n");
            print_array(temp_arr, size);
            printf("Verification: %s\n",
                is_sorted(temp_arr, size) ? "PASSED" : "FAILED");
            break;

        case 4:  // Compare all versions
            printf("\nComparing all versions with the same input:\n");

            // �⺻ ���� ����
            copy_array(temp_arr, arr, size);
            measure_time(merge_sort_basic, temp_arr, size,
                "Basic merge sort");

            // ����ȭ�� ���� ����
            copy_array(temp_arr, arr, size);
            measure_time(merge_sort_optimized, temp_arr, size,
                "Optimized merge sort");

            // �ݺ��� ���� ����
            copy_array(temp_arr, arr, size);
            measure_time(merge_sort_iterative_wrapper, temp_arr, size,
                "Iterative merge sort");
            break;

        case 5:  // Generate random array
            generate_random_array(arr, size);
            printf("New random array generated\n");
            printf("Array contents:\n");
            print_array(arr, size);
            break;

        case 6:  // Generate nearly sorted array
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
�� ���� �� �ֿ� ����
==========================================

1. ���� ������ �⺻ ����
-------------------
- ���� ���� ���
- �迭�� ������ ����
- ����� ����
- ���� �������� ����
- ���� ���� (Stable Sort)

2. �ð� ���⵵
-----------
�ּ��� ���: O(n log n)
����� ���: O(n log n)
�־��� ���: O(n log n)
- ��� ��� ������ ����
- �Է¿� ������

3. ���� ���⵵
-----------
O(n)
- �ӽ� �迭 �ʿ�
- ���ڸ� ���� �ƴ�
- �߰� �޸� ���

4. ���� ���
----------
����� ����:
- �������� ����
- �� ���� ���
- ���� ���� ����

�ݺ��� ����:
- �� ���� ������� ����
- ������ ����
- �޸� ȿ����

5. ����ȭ ���
-----------
���� �κй迭:
- ���� ���� Ȱ��
- �Ӱ谪 ���� ó��
- ������� ����

���� ����ȭ:
- �̹� ���ĵ� ��� �˻�
- ���� Ƚ�� ����
- ĳ�� ȿ���� ����

6. �����
-------
����:
- ���� ����
- ���� ������ ����
- ���� ����Ʈ ���Ŀ� ����
- �ܺ� ������ ����

����:
- �߰� �޸� �ʿ�
- ĳ�� ������ ����
- ���� �迭�� ��ȿ����
- ���� �������

7. ���� �о�
----------
- �ܺ� ����
- ���� ����Ʈ ����
- ���� ����
- �������� �ʿ��� ���

8. ���� Ư¡
----------
- �� ���� ���� ���
- ����ȭ ��� ����
- ���� ���� ���
- ��� ����

9. ���� ���ɼ�
-----------
- ����ȭ
- ĳ�� ����ȭ
- ���̺긮�� ���
- �޸� ����

10. ������ ��ġ
------------
- ���� ���� ����
- ���� ���� ����
- �ð�/���� Ʈ���̵����
- ����ȭ ��� �н�

�� ������ ���� ������ �پ���
������ �����ϸ�, ������ �������
���� �� �ִ� ������ ���α׷���
�����մϴ�.
*/
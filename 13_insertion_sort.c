#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

typedef int DataType;

/* ���� ����
 * - �Ű�����: arr - ������ �迭, size - �迭�� ũ��
 * - ����: �� ���Ҹ� ���ĵ� �κ��� ������ ��ġ�� ����
 */
void insertion_sort(DataType arr[], size_t size) {
    for (size_t i = 1; i < size; i++) {
        DataType key = arr[i];
        int j = i - 1;

        // key���� ū ���ҵ��� �ڷ� �̵�
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

/* ���� Ž���� �̿��� ���� ����
 * - �Ű�����: arr - ������ �迭, size - �迭�� ũ��
 * - ����: ���� Ž������ ���� ��ġ�� ã�� ����
 */
void binary_insertion_sort(DataType arr[], size_t size) {
    for (size_t i = 1; i < size; i++) {
        DataType key = arr[i];
        int left = 0;
        int right = i - 1;

        // ���� Ž������ ���� ��ġ ã��
        while (left <= right) {
            int mid = (left + right) / 2;
            if (arr[mid] > key) {
                right = mid - 1;
            }
            else {
                left = mid + 1;
            }
        }

        // ���ҵ��� �ڷ� �̵�
        for (int j = i - 1; j >= left; j--) {
            arr[j + 1] = arr[j];
        }
        arr[left] = key;
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

            // �⺻ ���� ����
            copy_array(temp_arr, arr, size);
            measure_time(insertion_sort, temp_arr, size, "Basic insertion sort");

            // ���� ���� ����
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
�� ���� �� �ֿ� ����
==========================================

1. ���� ������ �⺻ ����
-------------------
- ���ĵ� �κа� ������ �κ� ����
- �� ���Ҹ� ���ĵ� �κ��� ������ ��ġ�� ����
- ī�� �����ϱ�� ������ ���
- ���� ���� (Stable Sort)

2. �ð� ���⵵
-----------
�־��� ���: O(n��)
- �������� ���ĵ� ���
- n(n-1)/2���� �񱳿� �̵�

�ּ��� ���: O(n)
- �̹� ���ĵ� ���
- n-1���� �񱳸� �ʿ�

����� ���: O(n��)
- ������ �������� ���

3. ���� ���⵵
-----------
O(1)
- �߰� �޸� ���ʿ�
- ���ڸ� ���� (In-place Sort)

4. ����ȭ ���
-----------
���� Ž�� Ȱ��:
- ���� ��ġ Ž�� ����ȭ
- �� Ƚ�� ����: O(n log n)
- �̵� Ƚ���� ����

�κ� ���� Ȱ��:
- ���ĵ� �κ� ����
- ���� �̵� Ƚ��
- ������ ����

5. Ư¡
------
����:
- ���� ����
- ������ ����
- �¶��� ���� ����
- �κ� ���Ŀ� ȿ����

����:
- ū �����Ϳ� ��ȿ����
- ���� �̵� ����
- ���� �̵� ��� ŭ

6. �ٸ� ���İ��� ��
-----------------
���� ���� ���:
- �� ȿ����
- �� ���� ��ȯ
- ���� ���뿡 �� ����

���� ���� ���:
- �� ������
- �κ� ���Ŀ� ����
- ���� ����

7. ���� �о�
----------
- ���� �����ͼ�
- ���� ���ĵ� ������
- �¶��� ����
- ������ ���� �ʿ� ��

8. ���� Ư¡
----------
- ���� Ž�� ����ȭ
- ���� ���� ���
- �پ��� �Է� ����
- ��� ����

�� ������ ���� ������ �⺻ Ư����
����ȭ ����� �����ϸ�, Ư��
���� ���ĵ� �����Ϳ����� ������
�׽�Ʈ�� �� �ִ� ����� �����մϴ�.
*/
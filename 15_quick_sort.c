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

/* �߾Ӱ� �Ǻ� ����
 * - �Ű�����: arr - �迭, left, right - ������ ���۰� ��
 * - ��ȯ��: �߾Ӱ��� �ε���
 */
size_t median_of_three(DataType arr[], size_t left, size_t right) {
    size_t mid = (left + right) / 2;

    if (arr[left] > arr[mid])
        swap(&arr[left], &arr[mid]);
    if (arr[left] > arr[right])
        swap(&arr[left], &arr[right]);
    if (arr[mid] > arr[right])
        swap(&arr[mid], &arr[right]);

    return mid;
}

/* Lomuto ���� ���
 * - �Ű�����: arr - �迭, left, right - ������ ����
 * - ��ȯ��: �Ǻ��� ���� ��ġ
 */
size_t partition_lomuto(DataType arr[], size_t left, size_t right) {
    size_t pivot_idx = median_of_three(arr, left, right);
    DataType pivot = arr[pivot_idx];
    swap(&arr[pivot_idx], &arr[right]);  // �Ǻ��� ������ �̵�

    size_t i = left;  // ���� ���ҵ��� ���

    for (size_t j = left; j < right; j++) {
        if (arr[j] <= pivot) {
            swap(&arr[i], &arr[j]);
            i++;
        }
    }

    swap(&arr[i], &arr[right]);  // �Ǻ��� ���� ��ġ��
    return i;
}

/* Hoare ���� ���
 * - �Ű�����: arr - �迭, left, right - ������ ����
 * - ��ȯ��: ���� ��ġ
 */
size_t partition_hoare(DataType arr[], size_t left, size_t right) {
    size_t pivot_idx = median_of_three(arr, left, right);
    DataType pivot = arr[pivot_idx];

    size_t i = left - 1;
    size_t j = right + 1;

    while (true) {
        do {
            i++;
        } while (arr[i] < pivot);

        do {
            j--;
        } while (arr[j] > pivot);

        if (i >= j)
            return j;

        swap(&arr[i], &arr[j]);
    }
}

/* �� ���� (Lomuto ����)
 * - �Ű�����: arr - ������ �迭, left, right - ������ ����
 */
void quick_sort_lomuto(DataType arr[], size_t left, size_t right) {
    if (left < right) {
        size_t pivot = partition_lomuto(arr, left, right);
        if (pivot > 0)
            quick_sort_lomuto(arr, left, pivot - 1);
        quick_sort_lomuto(arr, pivot + 1, right);
    }
}

/* �� ���� (Hoare ����)
 * - �Ű�����: arr - ������ �迭, left, right - ������ ����
 */
void quick_sort_hoare(DataType arr[], size_t left, size_t right) {
    if (left < right) {
        size_t pivot = partition_hoare(arr, left, right);
        quick_sort_hoare(arr, left, pivot);
        quick_sort_hoare(arr, pivot + 1, right);
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

/* Wrapper �Լ� - Lomuto �� ����
 * - �Ű�����: arr - ������ �迭, size - �迭�� ũ��
 */
void quick_sort_lomuto_wrapper(DataType arr[], size_t size) {
    if (size > 0) {
        quick_sort_lomuto(arr, 0, size - 1);
    }
}

/* Wrapper �Լ� - Hoare �� ����
 * - �Ű�����: arr - ������ �迭, size - �迭�� ũ��
 */
void quick_sort_hoare_wrapper(DataType arr[], size_t size) {
    if (size > 0) {
        quick_sort_hoare(arr, 0, size - 1);
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
        swap(&arr[idx1], &arr[idx2]);
    }
}

/* ���� �ߺ����� �ִ� �迭 ����
 * - �Ű�����: arr - ������ �迭, size - �迭�� ũ��
 */
void generate_many_duplicates(DataType arr[], size_t size) {
    for (size_t i = 0; i < size; i++) {
        arr[i] = rand() % 10;  // 0-9 ������ ���� ���
    }
}

/* �޴� ��� */
void print_menu(void) {
    printf("\n=== Quick Sort Menu ===\n");
    printf("1. Run Lomuto partition quick sort\n");
    printf("2. Run Hoare partition quick sort\n");
    printf("3. Compare both versions\n");
    printf("4. Generate new random array\n");
    printf("5. Generate nearly sorted array\n");
    printf("6. Generate array with many duplicates\n");
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
        case 1:  // Lomuto partition
            copy_array(temp_arr, arr, size);
            printf("\nArray before sorting:\n");
            print_array(temp_arr, size);

            measure_time(quick_sort_lomuto_wrapper, temp_arr, size,
                "Quick sort (Lomuto)");

            printf("Array after sorting:\n");
            print_array(temp_arr, size);
            printf("Verification: %s\n",
                is_sorted(temp_arr, size) ? "PASSED" : "FAILED");
            break;

        case 2:  // Hoare partition
            copy_array(temp_arr, arr, size);
            printf("\nArray before sorting:\n");
            print_array(temp_arr, size);

            measure_time(quick_sort_hoare_wrapper, temp_arr, size,
                "Quick sort (Hoare)");

            printf("Array after sorting:\n");
            print_array(temp_arr, size);
            printf("Verification: %s\n",
                is_sorted(temp_arr, size) ? "PASSED" : "FAILED");
            break;

        case 3:  // Compare both versions
            printf("\nComparing both versions with the same input:\n");

            // Lomuto partition
            copy_array(temp_arr, arr, size);
            measure_time(quick_sort_lomuto_wrapper, temp_arr, size,
                "Quick sort (Lomuto)");

            // Hoare partition
            copy_array(temp_arr, arr, size);
            measure_time(quick_sort_hoare_wrapper, temp_arr, size,
                "Quick sort (Hoare)");
            break;

        case 4:  // Generate random array
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

        case 6:  // Generate array with duplicates
            generate_many_duplicates(arr, size);
            printf("New array with duplicates generated\n");
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

1. �� ������ �⺻ ����
------------------
- ���� ���� (Divide and Conquer) ���
- �Ǻ��� �������� �迭 ����
- ����� �ذ�
- �Ҿ��� ���� (Unstable Sort)

2. ���� ���
----------
Lomuto ����:
- ������ ���Ҹ� �Ǻ�����
- �ܹ��� ��ȸ
- ������ �ܼ�
- �ߺ����� ����

Hoare ����:
- ����⿡�� ����
- �� ���� ��ȯ
- �� ȿ����
- ������ ����

3. �ð� ���⵵
-----------
�ּ��� ���: O(n log n)
- �������� ����
- �̻����� �Ǻ� ����

����� ���: O(n log n)
- ������ ������
- �ǿ����� ����

�־��� ���: O(n��)
- �̹� ���ĵ� ���
- �Ǻ��� �ּ�/�ִ밪

4. ���� ���⵵
-----------
���: O(log n)
- ��� ȣ�� ����

�־�: O(n)
- �ұ��� ���ҽ�

5. ����ȭ ���
-----------
�Ǻ� ����:
- �߾Ӱ� ����
- �� ���� �߾Ӱ�
- ������ ����

���� �κй迭:
- ���� ���� Ȱ��
- �Ӱ谪 ���� ó��
- ��� ���� ����

6. �����
-------
����:
- ��������� ����
- ���ڸ� ����
- ĳ�� ȿ����

����:
- �Ҿ��� ����
- �Ǻ� ���� �߿�
- �־��� ��� ����

7. Ư¡
------
- ���� ���� �˰���
- �پ��� ���� ����
- ������ ���� ����
- ����ȭ ����

8. ���� Ư¡
----------
- ����ȭ�� �Ǻ� ����
- �پ��� �׽�Ʈ ������
- ���� ���� ���
- ��� ����

9. ���� ����
----------
- ǥ�� ���̺귯�� ����
- ��뷮 ������ ó��
- �ܺ� ������ ����
- ���̺긮�� ����

10. ���� ���ɼ�
------------
- ���� ���� ȥ��
- 3-way ����
- �ݺ��� ����
- ���� ó��

�� ������ �� ������ �� ���� �ֿ�
���� ����� ��� �����ϸ�, �پ���
�Է� �����Ϳ� ���� ���� �񱳰�
������ ������ ���α׷��Դϴ�.
*/
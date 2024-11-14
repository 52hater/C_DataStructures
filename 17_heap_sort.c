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

/* �ִ� �� �Ӽ� ���� (��� ����)
 * - �Ű�����: arr - �� �迭, size - �� ũ��, index - ���� ���
 */
void heapify_recursive(DataType arr[], size_t size, size_t index) {
    size_t largest = index;       // �ִ밪 ��ġ
    size_t left = 2 * index + 1;  // ���� �ڽ�
    size_t right = 2 * index + 2; // ������ �ڽ�

    // ���� �ڽ��� �� ũ�� largest ����
    if (left < size && arr[left] > arr[largest]) {
        largest = left;
    }

    // ������ �ڽ��� �� ũ�� largest ����
    if (right < size && arr[right] > arr[largest]) {
        largest = right;
    }

    // largest�� ����Ǿ����� ��ȯ�ϰ� ��� ȣ��
    if (largest != index) {
        swap(&arr[index], &arr[largest]);
        heapify_recursive(arr, size, largest);
    }
}

/* �ִ� �� �Ӽ� ���� (�ݺ� ����)
 * - �Ű�����: arr - �� �迭, size - �� ũ��, index - ���� ���
 */
void heapify_iterative(DataType arr[], size_t size, size_t index) {
    size_t current = index;

    while (true) {
        size_t largest = current;
        size_t left = 2 * current + 1;
        size_t right = 2 * current + 2;

        if (left < size && arr[left] > arr[largest]) {
            largest = left;
        }
        if (right < size && arr[right] > arr[largest]) {
            largest = right;
        }

        if (largest == current) {
            break;
        }

        swap(&arr[current], &arr[largest]);
        current = largest;
    }
}

/* �� ���� (�����)
 * - �Ű�����: arr - �迭, size - �迭 ũ��
 * - ����: ������ ��ܸ� ������ �������� heapify
 */
void build_heap_bottom_up(DataType arr[], size_t size,
    void (*heapify)(DataType[], size_t, size_t)) {
    // ������ ��ܸ� ������ �������� heapify ����
    for (int i = (size / 2) - 1; i >= 0; i--) {
        heapify(arr, size, i);
    }
}

/* �⺻ �� ����
 * - �Ű�����: arr - ������ �迭, size - �迭 ũ��
 */
void heap_sort_basic(DataType arr[], size_t size) {
    // �ִ� �� ����
    build_heap_bottom_up(arr, size, heapify_recursive);

    // ������ �ϳ��� �����ϸ� ����
    for (size_t i = size - 1; i > 0; i--) {
        swap(&arr[0], &arr[i]);  // �ִ밪�� ������ �̵�
        heapify_recursive(arr, i, 0);  // �� ũ�⸦ ���̰� heapify
    }
}

/* ����ȭ�� �� ���� (�ݺ��� heapify ���)
 * - �Ű�����: arr - ������ �迭, size - �迭 ũ��
 */
void heap_sort_optimized(DataType arr[], size_t size) {
    build_heap_bottom_up(arr, size, heapify_iterative);

    for (size_t i = size - 1; i > 0; i--) {
        swap(&arr[0], &arr[i]);
        heapify_iterative(arr, i, 0);
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

/* �� ���� �ð�ȭ
 * - �Ű�����: arr - �� �迭, size - �� ũ��, index - ���� ���
 *            level - ���� ����, prefix - ��� ���λ�
 */
void print_heap_recursive(const DataType arr[], size_t size, size_t index,
    int level, char* prefix) {
    if (index >= size) {
        return;
    }

    printf("%s", prefix);
    printf("%s", level ? "������ " : "");
    printf("%d\n", arr[index]);

    char new_prefix[256];
    sprintf(new_prefix, "%s%s", prefix, level ? "��   " : "");

    print_heap_recursive(arr, size, 2 * index + 1, 1, new_prefix); // ���� �ڽ�
    print_heap_recursive(arr, size, 2 * index + 2, 1, new_prefix); // ������ �ڽ�
}

/* �� ���� ���
 * - �Ű�����: arr - �� �迭, size - �� ũ��
 */
void print_heap(const DataType arr[], size_t size) {
    printf("Heap Structure:\n");
    print_heap_recursive(arr, size, 0, 0, "");
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

/* �迭�� �� �Ӽ��� �����ϴ��� ����
 * - �Ű�����: arr - ������ �迭, size - �迭�� ũ��
 * - ��ȯ��: �� �Ӽ��� �����ϸ� true, �ƴϸ� false
 */
bool is_heap(const DataType arr[], size_t size) {
    for (size_t i = 0; i < size / 2; i++) {
        size_t left = 2 * i + 1;
        size_t right = 2 * i + 2;

        if (left < size && arr[i] < arr[left]) {
            return false;
        }
        if (right < size && arr[i] < arr[right]) {
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
    printf("\n=== Heap Sort Menu ===\n");
    printf("1. Run basic heap sort\n");
    printf("2. Run optimized heap sort\n");
    printf("3. Compare both versions\n");
    printf("4. Show heap structure\n");
    printf("5. Generate new random array\n");
    printf("6. Verify heap property\n");
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
        case 1:  // Basic heap sort
            copy_array(temp_arr, arr, size);
            printf("\nArray before sorting:\n");
            print_array(temp_arr, size);

            measure_time(heap_sort_basic, temp_arr, size,
                "Basic heap sort");

            printf("Array after sorting:\n");
            print_array(temp_arr, size);
            printf("Verification: %s\n",
                is_sorted(temp_arr, size) ? "PASSED" : "FAILED");
            break;

        case 2:  // Optimized heap sort
            copy_array(temp_arr, arr, size);
            printf("\nArray before sorting:\n");
            print_array(temp_arr, size);

            measure_time(heap_sort_optimized, temp_arr, size,
                "Optimized heap sort");

            printf("Array after sorting:\n");
            print_array(temp_arr, size);
            printf("Verification: %s\n",
                is_sorted(temp_arr, size) ? "PASSED" : "FAILED");
            break;

        case 3:  // Compare both versions
            printf("\nComparing both versions with the same input:\n");

            // �⺻ �� ����
            copy_array(temp_arr, arr, size);
            measure_time(heap_sort_basic, temp_arr, size,
                "Basic heap sort");

            // ����ȭ�� �� ����
            copy_array(temp_arr, arr, size);
            measure_time(heap_sort_optimized, temp_arr, size,
                "Optimized heap sort");
            break;

        case 4:  // Show heap structure
            copy_array(temp_arr, arr, size);
            build_heap_bottom_up(temp_arr, size, heapify_recursive);
            print_heap(temp_arr, size);
            break;

        case 5:  // Generate random array
            generate_random_array(arr, size);
            printf("New random array generated\n");
            printf("Array contents:\n");
            print_array(arr, size);
            break;

        case 6:  // Verify heap property
            copy_array(temp_arr, arr, size);
            build_heap_bottom_up(temp_arr, size, heapify_recursive);
            printf("Heap property check: %s\n",
                is_heap(temp_arr, size) ? "Valid" : "Invalid");
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
-----------------
- �ִ� �� ����
- ��Ʈ ����
- �� �籸��
- ���ڸ� ����

2. �ð� ���⵵
-----------
�� ����: O(n)
����: O(n log n)
��ü: O(n log n)
- ��� ��� ����
- �Է¿� ������

3. ���� ���⵵
-----------
O(1)
- �߰� �޸� ���ʿ�
- ���ڸ� ����
- ���� ������ ���

4. ���� ���
----------
����� ����:
- ������ ����
- �� ���� ���
- ������ ����

�ݺ��� ����:
- ���� ������� ����
- ���� ���
- ������ ����

5. ����ȭ ���
-----------
�ݺ��� heapify:
- ���� ������� ����
- ĳ�� ������ ���
- �Լ� ȣ�� ����

�� ���� ����ȭ:
- ����� ����
- ���ʿ��� �� ����
- ȿ������ ����

6. �����
-------
����:
- ���� ���� ����
- ���ڸ� ����
- ������ ����
- �켱���� ť ����

����:
- �Ҿ��� ����
- ĳ�� ������ ����
- ���� ���� ����
- �б� ���� �����
- ���� �޸� ���� �ұ�Ģ

7. ���� �о�
----------
- �켱���� ť ����
- K�� �ִ�/�ּҰ� ã��
- �ý��� ���α׷���
- �ǽð� �����ٸ�

8. ���� Ư¡
----------
- ���/�ݺ� �� ���� ���
- �� �Ӽ� ����
- �ð��� ǥ��
- ���� ����

9. ���� ���ɼ�
-----------
- ����ȭ
- ĳ�� ����ȭ
- �б� ���� ����
- �޸� ���� ���� ����ȭ

10. ������ ��ġ
------------
- ���� ���� Ʈ�� ����
- �� �ڷᱸ�� �н�
- ���/�ݺ� ��
- ����ȭ ��� �н�

�� ������ �� ������ �⺻ �����
�پ��� ����ȭ ����� �����ϸ�,
�� ������ �ð�ȭ ��ɵ� �����ϴ�
������ ���α׷��Դϴ�.
*/
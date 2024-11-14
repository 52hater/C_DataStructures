#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

typedef int DataType;

/*
����� ���� Ž��:
- �߰����� ���Ͽ� Ž�� ������ �������� ����
- ���� ���� ������� ����
- �� ���� ���
*/
int binary_search_recursive(const DataType arr[], int left, int right, DataType key) {
    if (left > right) {
        return -1;
    }

    int mid = left + (right - left) / 2;  // �����÷ο� ����

    if (arr[mid] == key) {
        return mid;
    }

    if (arr[mid] > key) {
        return binary_search_recursive(arr, left, mid - 1, key);
    }

    return binary_search_recursive(arr, mid + 1, right, key);
}

/*
�ݺ��� ���� Ž��:
- while ������ ����Ͽ� ����
- ��� ȣ�� ������� ����
- ���� �����÷ο� ���� ����
*/
int binary_search_iterative(const DataType arr[], int size, DataType key) {
    int left = 0;
    int right = size - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (arr[mid] == key) {
            return mid;
        }

        if (arr[mid] > key) {
            right = mid - 1;
        }
        else {
            left = mid + 1;
        }
    }

    return -1;
}

/* Wrapper �Լ� - ����� ���� Ž��
 * - �Ű�����: arr - �˻��� �迭, size - �迭�� ũ��, key - ã�� ��
 */
int binary_search_recursive_wrapper(const DataType arr[], int size, DataType key) {
    return binary_search_recursive(arr, 0, size - 1, key);
}

/* �迭�� ���ĵǾ� �ִ��� Ȯ��
 * - �Ű�����: arr - Ȯ���� �迭, size - �迭�� ũ��
 */
bool is_sorted(const DataType arr[], size_t size) {
    for (size_t i = 1; i < size; i++) {
        if (arr[i] < arr[i - 1]) {
            return false;
        }
    }
    return true;
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

/* �迭 ����
 * - �Ű�����: dest - ������ �迭, src - ���� �迭, size - �迭�� ũ��
 */
void copy_array(DataType dest[], const DataType src[], size_t size) {
    for (size_t i = 0; i < size; i++) {
        dest[i] = src[i];
    }
}

/* �˰��� ���� �ð� ����
 * - �Ű�����: search_func - �˻� �Լ� ������, arr - �˻��� �迭,
 *            size - �迭�� ũ��, key - ã�� ��, name - �˰��� �̸�
 */
void measure_time(int (*search_func)(const DataType[], int, DataType),
    const DataType arr[], size_t size, DataType key,
    const char* name) {
    clock_t start = clock();
    int index = search_func(arr, size, key);
    clock_t end = clock();

    double cpu_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("%s: %.6f seconds\n", name, cpu_time);
    if (index != -1) {
        printf("Found at index: %d\n", index);
    }
    else {
        printf("Not found\n");
    }
}

/* ���ĵ� ������ �迭 ����
 * - �Ű�����: arr - ������ �迭, size - �迭�� ũ��
 */
void generate_sorted_array(DataType arr[], size_t size) {
    // ù ��° ��Ҵ� 0-9 ������ ��
    arr[0] = rand() % 10;

    // ������ ��ҵ��� ���� ��Һ��� 1-10 ū ��
    for (size_t i = 1; i < size; i++) {
        arr[i] = arr[i - 1] + (rand() % 10) + 1;
    }
}

/* ���� Ž�� ���� �ð�ȭ
 * - �Ű�����: arr - �迭, left, right - ���� ����, mid - �߰� ��ġ
 */
void visualize_step(const DataType arr[], int size, int left, int right, int mid) {
    printf("\nArray: ");
    for (int i = 0; i < size; i++) {
        if (i == mid) {
            printf("[%d] ", arr[i]);  // �߰� ��
        }
        else if (i >= left && i <= right) {
            printf("%d ", arr[i]);    // ���� ����
        }
        else {
            printf("_ ");             // ���ܵ� ����
        }
    }
    printf("\n");

    // ������ ǥ��
    printf("       ");
    for (int i = 0; i < size; i++) {
        if (i == left && i == right) {
            printf("^   ");
        }
        else if (i == left) {
            printf("L   ");
        }
        else if (i == right) {
            printf("R   ");
        }
        else if (i == mid) {
            printf("M   ");
        }
        else {
            printf("    ");
        }
    }
    printf("\n");
}

/* �ð�ȭ�� ���� Ž��
 * - �Ű�����: arr - �迭, size - ũ��, key - ã�� ��
 */
int binary_search_visual(const DataType arr[], int size, DataType key) {
    int left = 0;
    int right = size - 1;
    int step = 1;

    printf("\nSearching for value: %d\n", key);

    while (left <= right) {
        int mid = left + (right - left) / 2;

        printf("\nStep %d:\n", step++);
        visualize_step(arr, size, left, right, mid);
        printf("Comparing with middle element: %d\n", arr[mid]);

        if (arr[mid] == key) {
            printf("Found the value!\n");
            return mid;
        }

        if (arr[mid] > key) {
            printf("Value is smaller, searching left half\n");
            right = mid - 1;
        }
        else {
            printf("Value is larger, searching right half\n");
            left = mid + 1;
        }
    }

    printf("Value not found\n");
    return -1;
}

/* �޴� ��� */
void print_menu(void) {
    printf("\n=== Binary Search Menu ===\n");
    printf("1. Run recursive binary search\n");
    printf("2. Run iterative binary search\n");
    printf("3. Run visualized binary search\n");
    printf("4. Compare recursive and iterative\n");
    printf("5. Generate new sorted array\n");
    printf("0. Exit\n");
    printf("Choice: ");
}

int main(void) {
    srand((unsigned int)time(NULL));

    size_t size;
    printf("Enter array size: ");
    if (scanf("%zu", &size) != 1 || size == 0) {
        printf("Invalid size\n");
        return 1;
    }

    DataType* arr = (DataType*)malloc(size * sizeof(DataType));
    if (arr == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    generate_sorted_array(arr, size);
    int choice, search_key;

    do {
        print_menu();
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
        case 1:  // Recursive binary search
            printf("Enter value to search: ");
            scanf("%d", &search_key);

            printf("\nArray contents:\n");
            print_array(arr, size);

            measure_time(binary_search_recursive_wrapper, arr, size, search_key,
                "Recursive binary search");
            break;

        case 2:  // Iterative binary search
            printf("Enter value to search: ");
            scanf("%d", &search_key);

            printf("\nArray contents:\n");
            print_array(arr, size);

            measure_time(binary_search_iterative, arr, size, search_key,
                "Iterative binary search");
            break;

        case 3:  // Visualized binary search
            printf("Enter value to search: ");
            scanf("%d", &search_key);

            printf("\nArray contents:\n");
            print_array(arr, size);

            binary_search_visual(arr, size, search_key);
            break;

        case 4:  // Compare both methods
            printf("Enter value to search: ");
            scanf("%d", &search_key);

            printf("\nComparing both methods:\n");
            printf("Array contents:\n");
            print_array(arr, size);

            measure_time(binary_search_recursive_wrapper, arr, size, search_key,
                "Recursive binary search");
            measure_time(binary_search_iterative, arr, size, search_key,
                "Iterative binary search");
            break;

        case 5:  // Generate new array
            generate_sorted_array(arr, size);
            printf("New sorted array generated\n");
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
    return 0;
}

/*
==========================================
�� ���� �� �ֿ� ����
==========================================

1. ���� Ž���� �⺻ ����
-------------------
- ���ĵ� �迭 �ʼ�
- �߰����� ��
- Ž�� ���� �������� ����
- ���� ���� ���

2. �ð� ���⵵
-----------
�ּ�: O(1)
���: O(log n)
�־�: O(log n)
- �� �ܰ踶�� ũ�� ��������
- �α� �ð� ����
- ���ĵ� ���� ����

3. ���� ���
----------
����� ����:
- ������ ���� ����
- �� ���� ���
- �Լ� ȣ�� �������

�ݺ��� ����:
- �޸� ȿ����
- ���� �����÷ο� ����
- ���� �ӵ� �ణ ����

4. �����
-------
����:
- ���� �˻� �ӵ�
- �������� ����
- �޸� ȿ����

����:
- ���� �ʿ�
- ���� ũ��
- ����/���� ��ȿ��

5. ����ȭ ���
-----------
- �߰��� ��� ����ȭ
- ��� ���� ó��
- ���/�ݺ� ����
- ĳ�� Ȱ��

6. �ð�ȭ Ư¡
-----------
- �ܰ躰 ���� ǥ��
- Ž�� ���� ǥ��
- �߰��� ���̶���Ʈ
- �ǻ� ���� ����

7. ���� �о�
----------
- �����ͺ��̽� �˻�
- ���ĵ� �迭 �˻�
- ���� Ž�� Ʈ��
- ����ȭ ����

8. ������ ��ġ
-----------
- ���� ���� ����
- �˰��� ���⵵
- ���/�ݺ� ��
- ����ȭ ���

�� ������ ���� Ž���� �� ����
�ֿ� ����� �����ϸ�, �ð�ȭ
����� ���� �˰����� ������
�ܰ躰�� ������ �� �ְ� �մϴ�.
*/
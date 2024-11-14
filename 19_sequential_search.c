#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

typedef int DataType;

/*
�����̵���(Move-to-Front):
- Ž���� ��Ҹ� �迭�� �� ������ �̵���Ű�� ���
- ���� �˻��Ǵ� ����� ���� �ӵ��� ���
- �ð��� ������(temporal locality) Ȱ��
*/
int sequential_search_mtf(DataType arr[], size_t size, DataType key) {
    for (size_t i = 0; i < size; i++) {
        if (arr[i] == key) {
            // ã�� ��Ҹ� �� ������ �̵�
            if (i > 0) {
                DataType temp = arr[i];
                for (size_t j = i; j > 0; j--) {
                    arr[j] = arr[j - 1];
                }
                arr[0] = temp;
            }
            return 0;  // ù ��° ��ġ�� �̵���
        }
    }
    return -1;
}

/*
���ʹ�(Sentinel):
- �迭 ���� ã�� ���� �߰��Ͽ� ��� �˻� ����
- �ݺ����� ���� �˻� Ƚ�� ����
- ���� �ణ ���
*/
int sequential_search_sentinel(DataType arr[], size_t size, DataType key) {
    DataType last = arr[size - 1];  // ������ ��� ����
    arr[size - 1] = key;  // ���� ����

    size_t i = 0;
    while (arr[i] != key) {
        i++;
    }

    arr[size - 1] = last;  // ���� �� ����
    if (i < size - 1 || arr[size - 1] == key) {
        return i;
    }
    return -1;
}

/* �⺻ ���� Ž��
 * - �迭�� ó������ ������ ���������� �˻�
 */
int sequential_search_basic(const DataType arr[], size_t size, DataType key) {
    for (size_t i = 0; i < size; i++) {
        if (arr[i] == key) {
            return i;
        }
    }
    return -1;
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
void measure_time(int (*search_func)(DataType[], size_t, DataType),
    DataType arr[], size_t size, DataType key,
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

/* ������ �迭 ����
 * - �Ű�����: arr - ������ �迭, size - �迭�� ũ��
 */
void generate_random_array(DataType arr[], size_t size) {
    for (size_t i = 0; i < size; i++) {
        arr[i] = rand() % 1000;  // 0-999 ������ ������ ��
    }
}

/* �޴� ��� */
void print_menu(void) {
    printf("\n=== Sequential Search Menu ===\n");
    printf("1. Basic sequential search\n");
    printf("2. Move-to-Front search\n");
    printf("3. Sentinel search\n");
    printf("4. Compare all methods\n");
    printf("5. Generate new random array\n");
    printf("0. Exit\n");
    printf("Choice: ");
}

int main(void) {
    srand((unsigned int)time(NULL));

    size_t size;
    printf("Enter array size: ");
    if (scanf("%zu", &size) != 1 || size < 2) {  // �ּ� 2�� �ʿ�
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
    int choice, search_key;

    do {
        print_menu();
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
        case 1:  // Basic sequential search
            printf("Enter value to search: ");
            scanf("%d", &search_key);
            printf("\nArray contents:\n");
            print_array(arr, size);

            measure_time(sequential_search_basic, arr, size, search_key,
                "Basic sequential search");
            break;

        case 2:  // Move-to-Front
            printf("Enter value to search: ");
            scanf("%d", &search_key);
            copy_array(temp_arr, arr, size);

            printf("\nArray before search:\n");
            print_array(temp_arr, size);

            measure_time(sequential_search_mtf, temp_arr, size, search_key,
                "Move-to-Front search");

            printf("Array after search:\n");
            print_array(temp_arr, size);
            break;

        case 3:  // Sentinel
            printf("Enter value to search: ");
            scanf("%d", &search_key);
            copy_array(temp_arr, arr, size);

            printf("\nArray contents:\n");
            print_array(temp_arr, size);

            measure_time(sequential_search_sentinel, temp_arr, size, search_key,
                "Sentinel search");
            break;

        case 4:  // Compare all methods
            printf("Enter value to search: ");
            scanf("%d", &search_key);

            printf("\nComparing all methods:\n");

            copy_array(temp_arr, arr, size);
            measure_time(sequential_search_basic, temp_arr, size, search_key,
                "Basic sequential search");

            copy_array(temp_arr, arr, size);
            measure_time(sequential_search_mtf, temp_arr, size, search_key,
                "Move-to-Front search");

            copy_array(temp_arr, arr, size);
            measure_time(sequential_search_sentinel, temp_arr, size, search_key,
                "Sentinel search");
            break;

        case 5:  // Generate new array
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

1. ���� Ž���� �⺻ ����
-------------------
- ó������ ������ ������ ��
- �߰� ��� �ε��� ��ȯ
- �־��� ��� ��ü Ž��

2. �ð� ���⵵
-----------
�⺻ ���� Ž��:
- ���: O(n/2)
- �־�: O(n)

�����̵���:
- �ּ�: O(1)
- �־�: O(n)
- ���� �˻��Ǵ� ��ҿ� ����

���ʹ�:
- ���: O(n/2)
- �־�: O(n)
- �� ���� ����

3. ����ȭ ���
-----------
�����̵���:
- �ð��� ������ Ȱ��
- ���� �����ϴ� ������ ����ȭ
- �̵� ������� ����

���ʹ�:
- ��� �˻� ����
- ���� ������� ����
- ���� �ܼ�ȭ

4. �����
-------
�⺻ ���� Ž��:
����:
- ���� �ܼ�
- ���� ���ʿ�
- ���� �����Ϳ� ȿ����
����:
- ���� �˻� �ӵ�
- ��뷮 �����Ϳ� ������

�����̵���:
����:
- �ݺ� �˻� ȿ����
- ĳ�� Ȱ�뵵 ����
����:
- �̵� �������
- ���� ���� ����

���ʹ�:
����:
- �� ���� ����
- ���� �ܼ�
����:
- �߰� ���� �ʿ�
- ���ѵ� ���� ���

5. ���� �о�
----------
- �ұԸ� ������ �˻�
- ���ĵ��� ���� ������
- �ӽ� ������ ����
- ĳ�� �޸� Ȱ��

6. ���� Ư¡
----------
- �� ���� ��� ��
- ���� ����
- ��� ����
- �ð��� Ȯ��

�� ������ ���� Ž���� �� ����
�ֿ� ����� ��� �����ϸ�,
������ Ư���� ������ ���� ��
�ִ� ������ ���α׷��Դϴ�.
*/
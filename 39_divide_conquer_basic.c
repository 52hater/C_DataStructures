#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
���� ����(Divide and Conquer):
- ������ �� ���� ���� ������ ����(Divide)
- ���� �������� ��������� �ذ�(Conquer)
- �ذ�� ����� ����(Combine)

�Ϲ����� ����:
if (������ ����� ����) {
    ���� �ذ�
} else {
    ����: ������ ���� ���� ������ ����
    ����: ���� �������� ��������� �ذ�
    ����: �ذ�� ���� �������� ����
}
*/

/* �⺻���� ���� ���� ������ */

/* 1. �ִ밪 ã��
 * - �迭�� �ѷ� ������ ������ �ִ밪�� ã�� ��
 * - �ð����⵵: O(n)
 * - ���� ������ �⺻ ������ �����ִ� ������ ����
 */
int find_max(int arr[], int left, int right) {
    // ���� ���: ���Ұ� �ϳ��� ���
    if (left == right) {
        return arr[left];
    }

    // ����: �迭�� �ѷ� ����
    int mid = (left + right) / 2;

    // ����: �� �κ��� �ִ밪�� ��������� ã��
    int left_max = find_max(arr, left, mid);
    int right_max = find_max(arr, mid + 1, right);

    // ����: �� �ִ밪�� ���Ͽ� �� ū �� ��ȯ
    return (left_max > right_max) ? left_max : right_max;
}

/* 2. ���� �˻� (���� ���� ����)
 * - ���ĵ� �迭���� �߰����� ���Ͽ� Ž�� ������ �������� ����
 * - �ð����⵵: O(log n)
 */
int binary_search(int arr[], int left, int right, int target) {
    if (left > right) {
        return -1;  // ã�� ����
    }

    // ����: �߰� ���� ���
    int mid = left + (right - left) / 2;  // �����÷ο� ����

    // ���� ���: ã�� ���
    if (arr[mid] == target) {
        return mid;
    }

    // ����: ������ ���ݿ��� ��������� �˻�
    if (arr[mid] > target) {
        return binary_search(arr, left, mid - 1, target);
    }
    else {
        return binary_search(arr, mid + 1, right, target);
    }
}

/* 3. �迭�� �� ���
 * - �迭�� �ѷ� ������ ������ ���� ����ϰ� ����
 * - �ð����⵵: O(n)
 * - ���� ó���� ���ɼ��� �����ִ� ����
 */
int array_sum(int arr[], int left, int right) {
    // ���� ���: ���Ұ� �ϳ��� ���
    if (left == right) {
        return arr[left];
    }

    // ����: �迭�� �ѷ� ����
    int mid = (left + right) / 2;

    // ����: �� �κ��� ���� ��������� ���
    int left_sum = array_sum(arr, left, mid);
    int right_sum = array_sum(arr, mid + 1, right);

    // ����: �� �κ��� ���� ����
    return left_sum + right_sum;
}

/* ���� �ð� ���� �Լ� */
double measure_time(void (*func)(), void* args) {
    clock_t start = clock();
    func(args);
    clock_t end = clock();
    return ((double)(end - start)) / CLOCKS_PER_SEC;
}

/* �迭 ��� */
void print_array(int arr[], int size) {
    printf("[");
    for (int i = 0; i < size; i++) {
        printf("%d", arr[i]);
        if (i < size - 1) printf(", ");
    }
    printf("]\n");
}

/* �޴� ��� */
void print_menu(void) {
    printf("\n=== Divide and Conquer Examples ===\n");
    printf("1. Find maximum value\n");
    printf("2. Binary search\n");
    printf("3. Calculate array sum\n");
    printf("4. Generate new random array\n");
    printf("0. Exit\n");
    printf("Choice: ");
}

int main(void) {
    int size;
    printf("Enter array size: ");
    scanf("%d", &size);

    int* arr = (int*)malloc(size * sizeof(int));
    if (!arr) {
        printf("Memory allocation failed\n");
        return 1;
    }

    // �ʱ� �迭 ����
    for (int i = 0; i < size; i++) {
        arr[i] = rand() % 100;  // 0-99 ������ ����
    }

    int choice;
    do {
        print_menu();
        scanf("%d", &choice);

        switch (choice) {
        case 1: {  // Find maximum
            printf("\nArray: ");
            print_array(arr, size);

            clock_t start = clock();
            int max = find_max(arr, 0, size - 1);
            clock_t end = clock();

            printf("Maximum value: %d\n", max);
            printf("Time taken: %f seconds\n",
                ((double)(end - start)) / CLOCKS_PER_SEC);
            break;
        }

        case 2: {  // Binary search
            // ���� �˻��� ���� �迭 ����
            for (int i = 0; i < size - 1; i++) {
                for (int j = 0; j < size - i - 1; j++) {
                    if (arr[j] > arr[j + 1]) {
                        int temp = arr[j];
                        arr[j] = arr[j + 1];
                        arr[j + 1] = temp;
                    }
                }
            }

            printf("\nSorted array: ");
            print_array(arr, size);

            int target;
            printf("Enter value to search: ");
            scanf("%d", &target);

            clock_t start = clock();
            int index = binary_search(arr, 0, size - 1, target);
            clock_t end = clock();

            if (index != -1) {
                printf("Value %d found at index %d\n", target, index);
            }
            else {
                printf("Value %d not found\n", target);
            }
            printf("Time taken: %f seconds\n",
                ((double)(end - start)) / CLOCKS_PER_SEC);
            break;
        }

        case 3: {  // Array sum
            printf("\nArray: ");
            print_array(arr, size);

            clock_t start = clock();
            int sum = array_sum(arr, 0, size - 1);
            clock_t end = clock();

            printf("Array sum: %d\n", sum);
            printf("Time taken: %f seconds\n",
                ((double)(end - start)) / CLOCKS_PER_SEC);
            break;
        }

        case 4: {  // Generate new array
            for (int i = 0; i < size; i++) {
                arr[i] = rand() % 100;
            }
            printf("New array generated: ");
            print_array(arr, size);
            break;
        }

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

1. ���� ������ �⺻ ����
-------------------
����(Divide):
- ������ �� ���� ���� ������ ����
- ���� ũ�Ⱑ ����� �κе�� ����
- ����� �ذ��� �����ؾ� ��

����(Conquer):
- ���� �������� ��������� �ذ�
- ���� ���(base case) �ʿ�
- ������ �ذ� �����ؾ� ��

����(Combine):
- ���� ������ �ش��� ����
- ��ü ������ �ش� ����
- ȿ������ ���� ��� �ʿ�

2. �ð� ���⵵ �м�
---------------
������ ���� ���:
T(n) = aT(n/b) + f(n)
- a: ���� ���� ����
- b: ũ�� ���� ����
- f(n): ���Ұ� ���� ���

�Ϲ����� ���:
- ���� ����: O(n log n)
- ���� ����: O(n log�� n)
- ���� ����: O(n)

3. �����
-------
����:
- ���� ó�� ����
- ĳ�� ȿ����
- ���� ũ�� ����
- �������� ����

����:
- ��� ȣ�� �������
- ���� ���� ���
- �ڵ� ���⵵ ����
- ���� ��� ����

4. ����ȭ ���
-----------
- ���� ��� ũ�� ����
- �ݺ��� ���� ��ȯ
- ĳ�� ������ Ȱ��
- ���� ó�� ����

5. ���� �о�
----------
- ���� �˰���
- ��ġ ���
- ���� �˰���
- ��ȯ �˰���

�� ������ ���� ������ �⺻ ������
���� ������ ���� �����ָ�, ����
�ܰ��� ���� ������ ���ʰ� �˴ϴ�.
*/
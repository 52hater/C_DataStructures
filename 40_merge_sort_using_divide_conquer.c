#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
���� ����(Merge Sort):
- ��ǥ���� ���� ���� ���� �˰���
- ���� ����(Stable Sort)
- 3�ܰ�: ����, ����, ����
- �׻� O(n log n) �ð����⵵ ����

����(Divide): �迭�� �� ���� �κ� �迭�� ����
����(Conquer): �κ� �迭���� ��������� ����
����(Merge): ���ĵ� �κ� �迭���� �ϳ��� �迭�� ����
*/

/* �� ���ĵ� �κ� �迭�� �����ϴ� �Լ�
 * - left ~ mid: ù ��° �κ� �迭
 * - mid+1 ~ right: �� ��° �κ� �迭
 * - �ð����⵵: O(n), �������⵵: O(n)
 */
void merge(int arr[], int left, int mid, int right, bool print_steps) {
    int i, j, k;
    int n1 = mid - left + 1;  // ���� �κ� �迭�� ũ��
    int n2 = right - mid;     // ������ �κ� �迭�� ũ��

    // �ӽ� �迭 ���� �Ҵ�
    int* L = (int*)malloc(n1 * sizeof(int));
    int* R = (int*)malloc(n2 * sizeof(int));

    // �����͸� �ӽ� �迭�� ����
    for (i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    if (print_steps) {
        printf("Merging subarrays:\n");
        printf("Left:  ");
        for (i = 0; i < n1; i++)
            printf("%d ", L[i]);
        printf("\nRight: ");
        for (j = 0; j < n2; j++)
            printf("%d ", R[j]);
        printf("\n");
    }

    // �� �κ� �迭�� ����
    i = 0;  // ù ��° �κ� �迭�� �ε���
    j = 0;  // �� ��° �κ� �迭�� �ε���
    k = left;  // ���յ� �迭�� �ε���

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;

        if (print_steps) {
            printf("Current array: ");
            for (int x = left; x <= right; x++)
                printf("%d ", arr[x]);
            printf("\n");
        }
    }

    // ���� ���ҵ��� ����
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }

    free(L);
    free(R);

    if (print_steps) {
        printf("Merged result: ");
        for (i = left; i <= right; i++)
            printf("%d ", arr[i]);
        printf("\n\n");
    }
}

/* ���� ���� ����
 * - �ð����⵵: O(n log n)
 * - �������⵵: O(n)
 */
void merge_sort(int arr[], int left, int right, bool print_steps) {
    if (left < right) {
        if (print_steps) {
            printf("Dividing array: ");
            for (int i = left; i <= right; i++)
                printf("%d ", arr[i]);
            printf("\n");
        }

        int mid = left + (right - left) / 2;  // �����÷ο� ����

        // ���� �� ����
        merge_sort(arr, left, mid, print_steps);      // ���� �κ� �迭 ����
        merge_sort(arr, mid + 1, right, print_steps); // ������ �κ� �迭 ����

        // ����
        merge(arr, left, mid, right, print_steps);
    }
}

/* �迭�� ���ĵǾ����� ���� */
bool is_sorted(const int arr[], int size) {
    for (int i = 0; i < size - 1; i++) {
        if (arr[i] > arr[i + 1]) {
            return false;
        }
    }
    return true;
}

/* �迭 ��� */
void print_array(const int arr[], int size) {
    printf("[");
    for (int i = 0; i < size; i++) {
        printf("%d", arr[i]);
        if (i < size - 1) printf(", ");
    }
    printf("]\n");
}

/* ���� �м��� ���� �Լ� */
void analyze_performance(void) {
    printf("\nPerformance Analysis:\n");
    printf("-------------------\n");

    int sizes[] = { 100, 1000, 10000, 100000 };
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    for (int i = 0; i < num_sizes; i++) {
        int size = sizes[i];
        int* arr = (int*)malloc(size * sizeof(int));

        // ���� �迭 ����
        for (int j = 0; j < size; j++) {
            arr[j] = rand() % 1000;
        }

        // �ð� ����
        clock_t start = clock();
        merge_sort(arr, 0, size - 1, false);
        clock_t end = clock();

        double time = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf("Size: %d, Time: %f seconds\n", size, time);

        free(arr);
    }
}

/* �޴� ��� */
void print_menu(void) {
    printf("\n=== Merge Sort Menu ===\n");
    printf("1. Sort new array\n");
    printf("2. Sort with steps\n");
    printf("3. Analyze performance\n");
    printf("4. Sort custom array\n");
    printf("0. Exit\n");
    printf("Choice: ");
}

int main(void) {
    srand(time(NULL));
    int size;
    int* arr = NULL;

    int choice;
    do {
        print_menu();
        scanf("%d", &choice);

        switch (choice) {
        case 1:  // Sort new array
        case 2: {  // Sort with steps
            printf("Enter array size: ");
            scanf("%d", &size);

            arr = (int*)realloc(arr, size * sizeof(int));
            if (!arr) {
                printf("Memory allocation failed\n");
                return 1;
            }

            for (int i = 0; i < size; i++) {
                arr[i] = rand() % 100;
            }

            printf("\nOriginal array: ");
            print_array(arr, size);

            merge_sort(arr, 0, size - 1, choice == 2);

            printf("Sorted array: ");
            print_array(arr, size);

            printf("Verification: %s\n",
                is_sorted(arr, size) ? "PASSED" : "FAILED");
            break;
        }

        case 3:  // Analyze performance
            analyze_performance();
            break;

        case 4: {  // Sort custom array
            printf("Enter array size: ");
            scanf("%d", &size);

            arr = (int*)realloc(arr, size * sizeof(int));
            if (!arr) {
                printf("Memory allocation failed\n");
                return 1;
            }

            printf("Enter %d integers: ", size);
            for (int i = 0; i < size; i++) {
                scanf("%d", &arr[i]);
            }

            printf("\nOriginal array: ");
            print_array(arr, size);

            merge_sort(arr, 0, size - 1, false);

            printf("Sorted array: ");
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

1. ���� ������ ����
---------------
����:
- �迭�� �� ���� �κ� �迭�� ����
- �߰� ������ �������� ����
- ũ�Ⱑ 1�� �� ������ �ݺ�

����:
- �κ� �迭���� ��������� ����
- ���� ���: ũ�Ⱑ 1�� �迭
- ���������� ó�� ����

����:
- ���ĵ� �� �迭�� �ϳ��� ����
- �߰� ���� �ʿ�
- ������ ����

2. �ð� ���⵵
-----------
����: O(1)
����: 2T(n/2)
����: O(n)

��ü: O(n log n)
- ��� ��� ����
- �Է¿� ������
- ��� Ʈ���� �� ����: O(n)
- Ʈ���� ����: O(log n)

3. ���� ���⵵
-----------
O(n)
- �ӽ� �迭 �ʿ�
- ��� ����: O(log n)
- ���ڸ� ���� �ƴ�

4. �����
-------
����:
- ���� ����
- ���� ���� ����
- ���� ����Ʈ�� ����
- ��뷮 �����Ϳ� ȿ����

����:
- �߰� �޸� �ʿ�
- ���� �迭�� �������
- ĳ�� ������ ����
- ���� ����

5. ����ȭ ���
-----------
- ���� �迭�� ���� ����
- ���ڸ� ����
- ĳ�� ����ȭ
- ����ȭ

6. ���� �о�
----------
- �ܺ� ����
- ������ �ʿ��
- ���� ����Ʈ ����
- ���� ����

�� ������ ���� ������ ��� �ܰ踦
��Ȯ�� �����ָ�, ���� ���� �м���
�����ϰ� �ֽ��ϴ�. ���� �ܰ���
�ŵ����� ����� ���ʰ� �˴ϴ�.
*/
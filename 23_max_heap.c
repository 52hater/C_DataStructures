#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

/*
�ִ� ��:
- ���� ���� Ʈ�� ����
- �θ� ��尡 �ڽ� ��庸�� ũ�ų� ����
- �迭�� ���� (�ε��� ������� �θ�-�ڽ� ���� ǥ��)
- ȿ������ ����/���� ����
*/

typedef int ElementType;
#define MAX_HEAP_SIZE 100

typedef struct {
    ElementType elements[MAX_HEAP_SIZE];
    int size;  // ���� ���� ũ��
} MaxHeap;

/* �θ� ����� �ε��� ��� */
#define PARENT(i) (((i) - 1) / 2)

/* ���� �ڽ� ����� �ε��� ��� */
#define LEFT_CHILD(i) (2 * (i) + 1)

/* ������ �ڽ� ����� �ε��� ��� */
#define RIGHT_CHILD(i) (2 * (i) + 2)

/* �� ������ ��ȯ */
void swap(ElementType* a, ElementType* b) {
    ElementType temp = *a;
    *a = *b;
    *b = temp;
}

/* �� �ʱ�ȭ */
void heap_init(MaxHeap* heap) {
    heap->size = 0;
}

/* ���� ����ִ��� Ȯ�� */
bool heap_is_empty(const MaxHeap* heap) {
    return heap->size == 0;
}

/* ���� ���� á���� Ȯ�� */
bool heap_is_full(const MaxHeap* heap) {
    return heap->size == MAX_HEAP_SIZE;
}

/* ���� ũ�� ��ȯ */
int heap_size(const MaxHeap* heap) {
    return heap->size;
}

/* ���� �̵� (���� �� ���)
 * - ���� ���Ե� ���Ҹ� ������ ��ġ�� �̵�
 * - �ð����⵵: O(log n)
 */
void heapify_up(MaxHeap* heap, int index) {
    while (index > 0 && heap->elements[PARENT(index)] < heap->elements[index]) {
        swap(&heap->elements[PARENT(index)], &heap->elements[index]);
        index = PARENT(index);
    }
}

/* ���� �̵� (���� �� ���)
 * - ��Ʈ ������ �����Ͽ� ������ ��ġ�� �̵�
 * - �ð����⵵: O(log n)
 */
void heapify_down(MaxHeap* heap, int index) {
    int largest = index;
    int left = LEFT_CHILD(index);
    int right = RIGHT_CHILD(index);

    // ���� �ڽ��� �� ū ���
    if (left < heap->size && heap->elements[left] > heap->elements[largest]) {
        largest = left;
    }

    // ������ �ڽ��� �� ū ���
    if (right < heap->size && heap->elements[right] > heap->elements[largest]) {
        largest = right;
    }

    // ��ȯ�� �ʿ��� ���
    if (largest != index) {
        swap(&heap->elements[index], &heap->elements[largest]);
        heapify_down(heap, largest);
    }
}

/* ���� ����
 * - �ð����⵵: O(log n)
 */
bool heap_insert(MaxHeap* heap, ElementType value) {
    if (heap_is_full(heap)) {
        return false;
    }

    // �� ���Ҹ� �������� �߰�
    heap->elements[heap->size] = value;

    // ������ ��ġ�� �̵�
    heapify_up(heap, heap->size);

    heap->size++;
    return true;
}

/* �ִ밪 ����
 * - �ð����⵵: O(log n)
 */
bool heap_delete_max(MaxHeap* heap, ElementType* value) {
    if (heap_is_empty(heap)) {
        return false;
    }

    // ��Ʈ ���� ��ȯ
    *value = heap->elements[0];

    // ������ ���Ҹ� ��Ʈ�� �̵�
    heap->elements[0] = heap->elements[heap->size - 1];
    heap->size--;

    // ��Ʈ���� ������
    if (!heap_is_empty(heap)) {
        heapify_down(heap, 0);
    }

    return true;
}

/* �ִ밪 Ȯ�� (�������� ����)
 * - �ð����⵵: O(1)
 */
bool heap_peek_max(const MaxHeap* heap, ElementType* value) {
    if (heap_is_empty(heap)) {
        return false;
    }
    *value = heap->elements[0];
    return true;
}

/* �� �ð�ȭ (Ʈ�� ���·� ���)
 * - ���� ��ȸ ������� ���
 */
void heap_print(const MaxHeap* heap) {
    if (heap_is_empty(heap)) {
        printf("Heap is empty\n");
        return;
    }

    printf("Heap structure:\n");
    int level = 0;
    int level_nodes = 1;
    int printed_nodes = 0;

    while (printed_nodes < heap->size) {
        // ���� ������ �鿩����
        for (int i = 0; i < (1 << (heap->size / 2 - level)); i++) {
            printf("  ");
        }

        // ���� ������ ���� ���
        for (int i = 0; i < level_nodes && printed_nodes < heap->size; i++) {
            printf("%d ", heap->elements[printed_nodes++]);

            // ��� ������ ����
            for (int j = 0; j < (1 << (heap->size / 2 - level + 1)); j++) {
                printf(" ");
            }
        }
        printf("\n");

        level++;
        level_nodes *= 2;
    }
}

/* �� ����
 * - �ִ� �� �Ӽ��� �����Ǵ��� Ȯ��
 */
bool heap_verify(const MaxHeap* heap) {
    for (int i = 1; i < heap->size; i++) {
        if (heap->elements[i] > heap->elements[PARENT(i)]) {
            return false;
        }
    }
    return true;
}

/* ���� ���� */
void measure_operations(MaxHeap* heap, int count) {
    clock_t start, end;
    double insert_time, delete_time;

    // ���� ���� ����
    start = clock();
    for (int i = 0; i < count; i++) {
        heap_insert(heap, rand() % 1000);
    }
    end = clock();
    insert_time = ((double)(end - start)) / CLOCKS_PER_SEC;

    // ���� ���� ����
    ElementType value;
    start = clock();
    for (int i = 0; i < count; i++) {
        heap_delete_max(heap, &value);
    }
    end = clock();
    delete_time = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("\nPerformance Analysis (%d operations):\n", count);
    printf("Insert time: %.6f seconds\n", insert_time);
    printf("Delete time: %.6f seconds\n", delete_time);
    printf("Average insert time: %.9f seconds\n", insert_time / count);
    printf("Average delete time: %.9f seconds\n", delete_time / count);
}

/* �޴� ��� */
void print_menu(void) {
    printf("\n=== Max Heap Menu ===\n");
    printf("1. Insert element\n");
    printf("2. Delete maximum\n");
    printf("3. Peek maximum\n");
    printf("4. Print heap\n");
    printf("5. Verify heap\n");
    printf("6. Run performance test\n");
    printf("0. Exit\n");
    printf("Choice: ");
}

int main(void) {
    MaxHeap heap;
    heap_init(&heap);

    int choice;
    ElementType value;

    do {
        print_menu();
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
        case 1:  // Insert
            printf("Enter value to insert: ");
            scanf("%d", &value);
            if (heap_insert(&heap, value)) {
                printf("Inserted %d\n", value);
                heap_print(&heap);
            }
            else {
                printf("Heap is full\n");
            }
            break;

        case 2:  // Delete maximum
            if (heap_delete_max(&heap, &value)) {
                printf("Deleted maximum value: %d\n", value);
                heap_print(&heap);
            }
            else {
                printf("Heap is empty\n");
            }
            break;

        case 3:  // Peek maximum
            if (heap_peek_max(&heap, &value)) {
                printf("Maximum value: %d\n", value);
            }
            else {
                printf("Heap is empty\n");
            }
            break;

        case 4:  // Print
            heap_print(&heap);
            break;

        case 5:  // Verify
            if (heap_verify(&heap)) {
                printf("Heap property is satisfied\n");
            }
            else {
                printf("Heap property is violated!\n");
            }
            break;

        case 6:  // Performance test
            printf("Enter number of operations: ");
            int count;
            scanf("%d", &count);
            if (count > MAX_HEAP_SIZE) {
                count = MAX_HEAP_SIZE;
                printf("Limiting to %d operations\n", count);
            }
            heap_init(&heap);  // Reset heap
            measure_operations(&heap, count);
            break;

        case 0:  // Exit
            printf("Exiting program\n");
            break;

        default:
            printf("Invalid choice\n");
        }
    } while (choice != 0);

    return 0;
}

/*
==========================================
�� ���� �� �ֿ� ����
==========================================

1. �ִ� ���� Ư��
-------------
- ���� ���� Ʈ��
- �ִ� �� �Ӽ�
- �迭 ��� ����
- ȿ������ ����/����

2. �ð� ���⵵
-----------
����: O(log n)
- ���� ���� �� ���� �̵�

����: O(log n)
- ��Ʈ ���� �� ���� �̵�

��ȸ: O(1)
- �ִ밪�� �׻� ��Ʈ

3. ���� ���⵵
-----------
O(n)
- �迭 ���
- ���� ���� Ʈ��
- �߰� ���� ���ʿ�

4. �ֿ� ����
----------
Heapify Up:
- ���� �� ���
- ����� ������
- �θ�� ��/��ȯ

Heapify Down:
- ���� �� ���
- ����� ������
- �ڽİ� ��/��ȯ

5. ���� Ư¡
----------
- �ε��� ���
- ����� ����
- �ð��� ���
- ���� ���

6. �����
-------
����:
- ȿ������ �켱���� ����
- ���� �ִ밪 ����
- �������� ����
- ���� �ܼ�

����:
- ���� ũ��
- ĳ�� ������
- ���� ���� Ʈ�� ����
- �Ҿ��� ����

7. ���� �о�
----------
- �켱���� ť
- �� ����
- �̺�Ʈ ó��
- �����ٸ�

8. ����ȭ ���
-----------
- �ݺ��� ����
- ĳ�� Ȱ��
- �޸� ����
- ���� �ּ�ȭ

�� ������ �ִ� ���� ��� �ٽ�
����� �����ϸ�, ���� �ܰ���
�� ��� �켱���� ť ������
���ʰ� �˴ϴ�.
*/
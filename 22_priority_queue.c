#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
�迭 ��� �ܼ� �켱���� ť:
- �켱������ ���� ���Ұ� ���� ó��
- ������ �迭 ���� ����
- ������ �ִ�/�ּҰ� �˻� �� ����
- ������ �ܼ������� ��ȿ����
*/

typedef int ElementType;
#define MAX_QUEUE_SIZE 100

typedef struct {
    ElementType elements[MAX_QUEUE_SIZE];
    int size;
} PriorityQueue;

/* �켱���� ť �ʱ�ȭ */
void queue_init(PriorityQueue* queue) {
    queue->size = 0;
}

/* ť�� ����ִ��� Ȯ�� */
bool queue_is_empty(const PriorityQueue* queue) {
    return queue->size == 0;
}

/* ť�� ���� á���� Ȯ�� */
bool queue_is_full(const PriorityQueue* queue) {
    return queue->size == MAX_QUEUE_SIZE;
}

/* ť�� ũ�� ��ȯ */
int queue_size(const PriorityQueue* queue) {
    return queue->size;
}

/* �ִ� �켱���� ���� ���� (ū ���� ���� �켱����)
 * - �ð����⵵: O(1)
 */
bool queue_insert_max(PriorityQueue* queue, ElementType value) {
    if (queue_is_full(queue)) {
        return false;
    }
    queue->elements[queue->size++] = value;
    return true;
}

/* �ּ� �켱���� ���� ���� (���� ���� ���� �켱����)
 * - �ð����⵵: O(1)
 */
bool queue_insert_min(PriorityQueue* queue, ElementType value) {
    if (queue_is_full(queue)) {
        return false;
    }
    queue->elements[queue->size++] = value;
    return true;
}

/* �ִ� �켱���� ���� ���� (ū ���� ���� �켱����)
 * - �ð����⵵: O(n)
 */
bool queue_remove_max(PriorityQueue* queue, ElementType* value) {
    if (queue_is_empty(queue)) {
        return false;
    }

    // �ִ밪�� �ε��� ã��
    int max_index = 0;
    for (int i = 1; i < queue->size; i++) {
        if (queue->elements[i] > queue->elements[max_index]) {
            max_index = i;
        }
    }

    // �ִ밪 ��ȯ
    *value = queue->elements[max_index];

    // ������ ���Ҹ� ������ ��ġ�� �̵�
    queue->elements[max_index] = queue->elements[queue->size - 1];
    queue->size--;

    return true;
}

/* �ּ� �켱���� ���� ���� (���� ���� ���� �켱����)
 * - �ð����⵵: O(n)
 */
bool queue_remove_min(PriorityQueue* queue, ElementType* value) {
    if (queue_is_empty(queue)) {
        return false;
    }

    // �ּҰ��� �ε��� ã��
    int min_index = 0;
    for (int i = 1; i < queue->size; i++) {
        if (queue->elements[i] < queue->elements[min_index]) {
            min_index = i;
        }
    }

    // �ּҰ� ��ȯ
    *value = queue->elements[min_index];

    // ������ ���Ҹ� ������ ��ġ�� �̵�
    queue->elements[min_index] = queue->elements[queue->size - 1];
    queue->size--;

    return true;
}

/* ť�� ��� ���� ��� */
void queue_print(const PriorityQueue* queue, const char* name) {
    printf("%s (size=%d): ", name, queue->size);
    for (int i = 0; i < queue->size; i++) {
        printf("%d ", queue->elements[i]);
    }
    printf("\n");
}

/* ���� ������ ���� ���� ���� �Լ� */
void measure_operations(PriorityQueue* queue, int count) {
    clock_t start, end;
    double insert_time, remove_time;

    // ���� ���� ����
    start = clock();
    for (int i = 0; i < count; i++) {
        queue_insert_max(queue, rand() % 1000);
    }
    end = clock();
    insert_time = ((double)(end - start)) / CLOCKS_PER_SEC;

    // ���� ���� ����
    ElementType value;
    start = clock();
    for (int i = 0; i < count; i++) {
        queue_remove_max(queue, &value);
    }
    end = clock();
    remove_time = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("\nPerformance Analysis (%d operations):\n", count);
    printf("Insert time: %.6f seconds\n", insert_time);
    printf("Remove time: %.6f seconds\n", remove_time);
    printf("Average insert time: %.9f seconds\n", insert_time / count);
    printf("Average remove time: %.9f seconds\n", remove_time / count);
}

/* �޴� ��� */
void print_menu(void) {
    printf("\n=== Priority Queue Menu ===\n");
    printf("1. Insert (max priority)\n");
    printf("2. Insert (min priority)\n");
    printf("3. Remove (max priority)\n");
    printf("4. Remove (min priority)\n");
    printf("5. Print queue\n");
    printf("6. Run performance test\n");
    printf("0. Exit\n");
    printf("Choice: ");
}

int main(void) {
    PriorityQueue queue;
    queue_init(&queue);

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
        case 1:  // Insert (max)
            printf("Enter value to insert: ");
            scanf("%d", &value);
            if (queue_insert_max(&queue, value)) {
                printf("Inserted %d\n", value);
            }
            else {
                printf("Queue is full\n");
            }
            queue_print(&queue, "Queue after insert");
            break;

        case 2:  // Insert (min)
            printf("Enter value to insert: ");
            scanf("%d", &value);
            if (queue_insert_min(&queue, value)) {
                printf("Inserted %d\n", value);
            }
            else {
                printf("Queue is full\n");
            }
            queue_print(&queue, "Queue after insert");
            break;

        case 3:  // Remove (max)
            if (queue_remove_max(&queue, &value)) {
                printf("Removed maximum value: %d\n", value);
            }
            else {
                printf("Queue is empty\n");
            }
            queue_print(&queue, "Queue after remove");
            break;

        case 4:  // Remove (min)
            if (queue_remove_min(&queue, &value)) {
                printf("Removed minimum value: %d\n", value);
            }
            else {
                printf("Queue is empty\n");
            }
            queue_print(&queue, "Queue after remove");
            break;

        case 5:  // Print
            queue_print(&queue, "Current queue");
            break;

        case 6:  // Performance test
            printf("Enter number of operations: ");
            int count;
            scanf("%d", &count);
            if (count > MAX_QUEUE_SIZE) {
                count = MAX_QUEUE_SIZE;
                printf("Limiting to %d operations\n", count);
            }
            queue_init(&queue);  // Reset queue
            measure_operations(&queue, count);
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

1. �켱���� ť�� �⺻ ����
----------------------
- FIFO�� �ƴ� �켱���� ���
- �ִ�/�ּ� �켱���� ����
- ���� ���� ����
- ���� �о� �پ�

2. �迭 ��� ������ Ư¡
--------------------
����:
- ������ �ܼ�
- �޸� ȿ����
- ĳ�� ������
- �����ϱ� ����

����:
- ���� ���� ��ȿ����
- ���� ũ��
- ���� �ð� �˻�
- Ȯ�强 ����

3. �ð� ���⵵
-----------
����:
- O(1) ��� �ð�
- �׻� ���� �߰�

����:
- O(n) ���� �ð�
- ��ü �˻� �ʿ�
- ��� �̵� �ʿ�

4. ���� ���⵵
-----------
- O(n) ���� ����
- ���� ũ�� �迭
- �߰� ���� ���ʿ�
- �޸� ȿ����

5. �ֿ� ����
----------
- �ʱ�ȭ
- ����
- �ִ�/�ּ� ����
- ũ�� Ȯ��
- ���� ���

6. ���� Ư¡
----------
- �� ���� �켱����
- ���� ����
- �ð��� ���
- ���� ó��

7. �Ѱ���
-------
- ��ȿ���� ����
- ũ�� ����
- Ȯ�� �����
- �˻� �ð�

8. ���� ����
----------
- �� ��� ����
- ���� ũ��
- ȿ���� �˻�
- ���� ����

�� ������ �켱���� ť�� �⺻
������ �����ϱ� ���� ������
��������, ���� ����� ���ؼ���
�� ��� ������ ����˴ϴ�.
*/
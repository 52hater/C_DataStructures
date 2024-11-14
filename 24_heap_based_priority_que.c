#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

/*
�� ��� �켱���� ť:
- �ִ� ���� Ȱ���� ȿ������ �켱���� ť ����
- �켱������ ���� ���Ҵ� �׻� ��Ʈ�� ��ġ
- O(log n) �ð� ���⵵�� ����/���� ����
- ���� ���� Ʈ���� Ư�� Ȱ��
*/

typedef struct {
    int priority;   // �켱����
    char data[50];  // ���� ������
} QueueElement;

typedef struct {
    QueueElement* elements;
    int capacity;
    int size;
} PriorityQueue;

/* �θ� ����� �ε��� ��� */
#define PARENT(i) (((i) - 1) / 2)

/* ���� �ڽ� ����� �ε��� ��� */
#define LEFT_CHILD(i) (2 * (i) + 1)

/* ������ �ڽ� ����� �ε��� ��� */
#define RIGHT_CHILD(i) (2 * (i) + 2)

/* �켱���� ť ����
 * - �Ű�����: capacity - ť�� �ִ� �뷮
 */
PriorityQueue* queue_create(int capacity) {
    PriorityQueue* queue = (PriorityQueue*)malloc(sizeof(PriorityQueue));
    if (queue == NULL) {
        return NULL;
    }

    queue->elements = (QueueElement*)malloc(capacity * sizeof(QueueElement));
    if (queue->elements == NULL) {
        free(queue);
        return NULL;
    }

    queue->capacity = capacity;
    queue->size = 0;
    return queue;
}

/* �켱���� ť �޸� ���� */
void queue_destroy(PriorityQueue* queue) {
    if (queue) {
        free(queue->elements);
        free(queue);
    }
}

/* �� ������ ��ȯ */
void swap_elements(QueueElement* a, QueueElement* b) {
    QueueElement temp = *a;
    *a = *b;
    *b = temp;
}

/* ���� �̵� (���� �� ���) */
void heapify_up(PriorityQueue* queue, int index) {
    while (index > 0) {
        int parent = PARENT(index);
        if (queue->elements[parent].priority >= queue->elements[index].priority) {
            break;
        }
        swap_elements(&queue->elements[parent], &queue->elements[index]);
        index = parent;
    }
}

/* ���� �̵� (���� �� ���) */
void heapify_down(PriorityQueue* queue, int index) {
    int largest = index;
    int left = LEFT_CHILD(index);
    int right = RIGHT_CHILD(index);

    if (left < queue->size &&
        queue->elements[left].priority > queue->elements[largest].priority) {
        largest = left;
    }

    if (right < queue->size &&
        queue->elements[right].priority > queue->elements[largest].priority) {
        largest = right;
    }

    if (largest != index) {
        swap_elements(&queue->elements[index], &queue->elements[largest]);
        heapify_down(queue, largest);
    }
}

/* ���� ����
 * - �Ű�����: queue - �켱���� ť, priority - �켱����, data - ������
 * - �ð����⵵: O(log n)
 */
bool queue_enqueue(PriorityQueue* queue, int priority, const char* data) {
    if (queue->size >= queue->capacity) {
        return false;
    }

    int index = queue->size++;
    queue->elements[index].priority = priority;
    strcpy(queue->elements[index].data, data);
    heapify_up(queue, index);

    return true;
}

/* �ְ� �켱���� ���� ����
 * - �Ű�����: queue - �켱���� ť, priority - �켱���� ����, data - ������ ����
 * - �ð����⵵: O(log n)
 */
bool queue_dequeue(PriorityQueue* queue, int* priority, char* data) {
    if (queue->size == 0) {
        return false;
    }

    *priority = queue->elements[0].priority;
    strcpy(data, queue->elements[0].data);

    queue->elements[0] = queue->elements[--queue->size];
    heapify_down(queue, 0);

    return true;
}

/* �ְ� �켱���� ���� Ȯ�� (�������� ����)
 * - �ð����⵵: O(1)
 */
bool queue_peek(PriorityQueue* queue, int* priority, char* data) {
    if (queue->size == 0) {
        return false;
    }

    *priority = queue->elements[0].priority;
    strcpy(data, queue->elements[0].data);
    return true;
}

/* �켱���� ť �ð�ȭ */
void queue_print(const PriorityQueue* queue) {
    if (queue->size == 0) {
        printf("Priority Queue is empty\n");
        return;
    }

    printf("\nPriority Queue Contents:\n");
    printf("Size: %d, Capacity: %d\n", queue->size, queue->capacity);
    printf("Format: (Priority, Data)\n");

    int level = 0;
    int level_nodes = 1;
    int printed_nodes = 0;

    while (printed_nodes < queue->size) {
        // ���� ������ �鿩����
        for (int i = 0; i < (1 << (4 - level)); i++) {
            printf("  ");
        }

        // ���� ������ ���� ���
        for (int i = 0; i < level_nodes && printed_nodes < queue->size; i++) {
            printf("(%d,%s) ",
                queue->elements[printed_nodes].priority,
                queue->elements[printed_nodes].data);
            printed_nodes++;
        }
        printf("\n");

        level++;
        level_nodes *= 2;
    }
}

/* �켱���� ť�� ����ִ��� Ȯ�� */
bool queue_is_empty(const PriorityQueue* queue) {
    return queue->size == 0;
}

/* �켱���� ť�� ���� á���� Ȯ�� */
bool queue_is_full(const PriorityQueue* queue) {
    return queue->size >= queue->capacity;
}

/* ���� ������ ���� ���� ���ڿ� ���� */
void generate_random_string(char* str, int length) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyz";
    for (int i = 0; i < length - 1; i++) {
        str[i] = charset[rand() % (sizeof(charset) - 1)];
    }
    str[length - 1] = '\0';
}

/* ���� ���� */
void measure_performance(PriorityQueue* queue, int operations) {
    clock_t start, end;
    char data[50];
    int priority;

    // ���� ���� ����
    start = clock();
    for (int i = 0; i < operations; i++) {
        generate_random_string(data, 6);
        queue_enqueue(queue, rand() % 100, data);
    }
    end = clock();
    double enqueue_time = ((double)(end - start)) / CLOCKS_PER_SEC;

    // ���� ���� ����
    start = clock();
    for (int i = 0; i < operations; i++) {
        queue_dequeue(queue, &priority, data);
    }
    end = clock();
    double dequeue_time = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("\nPerformance Analysis (%d operations):\n", operations);
    printf("Enqueue time: %.6f seconds (%.9f per operation)\n",
        enqueue_time, enqueue_time / operations);
    printf("Dequeue time: %.6f seconds (%.9f per operation)\n",
        dequeue_time, dequeue_time / operations);
}

/* �޴� ��� */
void print_menu(void) {
    printf("\n=== Priority Queue Menu ===\n");
    printf("1. Enqueue element\n");
    printf("2. Dequeue element\n");
    printf("3. Peek top element\n");
    printf("4. Print queue\n");
    printf("5. Run performance test\n");
    printf("0. Exit\n");
    printf("Choice: ");
}

int main(void) {
    srand((unsigned int)time(NULL));

    int capacity;
    printf("Enter priority queue capacity: ");
    scanf("%d", &capacity);

    PriorityQueue* queue = queue_create(capacity);
    if (queue == NULL) {
        printf("Failed to create priority queue\n");
        return 1;
    }

    int choice, priority;
    char data[50];

    do {
        print_menu();
        scanf("%d", &choice);

        switch (choice) {
        case 1:  // Enqueue
            printf("Enter priority (higher number = higher priority): ");
            scanf("%d", &priority);
            printf("Enter data: ");
            scanf("%s", data);

            if (queue_enqueue(queue, priority, data)) {
                printf("Element added successfully\n");
                queue_print(queue);
            }
            else {
                printf("Queue is full\n");
            }
            break;

        case 2:  // Dequeue
            if (queue_dequeue(queue, &priority, data)) {
                printf("Dequeued: Priority=%d, Data=%s\n", priority, data);
                queue_print(queue);
            }
            else {
                printf("Queue is empty\n");
            }
            break;

        case 3:  // Peek
            if (queue_peek(queue, &priority, data)) {
                printf("Top element: Priority=%d, Data=%s\n", priority, data);
            }
            else {
                printf("Queue is empty\n");
            }
            break;

        case 4:  // Print
            queue_print(queue);
            break;

        case 5:  // Performance test
            printf("Enter number of operations: ");
            int ops;
            scanf("%d", &ops);
            if (ops > queue->capacity) {
                ops = queue->capacity;
                printf("Limiting to %d operations\n", ops);
            }
            measure_performance(queue, ops);
            break;

        case 0:  // Exit
            printf("Exiting program\n");
            break;

        default:
            printf("Invalid choice\n");
        }
    } while (choice != 0);

    queue_destroy(queue);
    return 0;
}

/*
==========================================
�� ���� �� �ֿ� ����
==========================================

1. �� ��� �켱���� ť�� Ư¡
-----------------------
- �ִ� �� ���� Ȱ��
- ���� �޸� �Ҵ�
- �켱������ ������ �и�
- ȿ������ ����

2. �ð� ���⵵
-----------
����(Enqueue): O(log n)
����(Dequeue): O(log n)
��ȸ(Peek): O(1)
- ��� �۾��� ȿ����

3. ���� ���⵵
-----------
O(n)
- ���� �迭 ���
- �߰� ���� �ּ�ȭ
- Ȯ�� ������ ����

4. �ֿ� ���� Ư¡
-------------
- �켱������ ������ ��
- ���� ũ�� ����
- �ð�ȭ ���
- ���� ����

5. ����
-----
- ���� �켱���� ó��
- �޸� ȿ����
- �������� ����
- Ȯ�� ���ɼ�

6. ����
-----
- ���� ���⵵
- �޸� ����ȭ
- ĳ�� ������
- �Ҿ��� ����

7. ����ȭ ���
-----------
- ���� �޸� ����
- ĳ�� Ȱ��
- ���� �ּ�ȭ
- �޸� ����

8. ���� �о�
----------
- �۾� �����ٸ�
- �̺�Ʈ ó��
- ��Ʈ��ũ ��Ŷ ó��
- �ùķ��̼�

�� ������ ���� �������� ����� ��
�ִ� ������ �켱���� ť�� �����ϸ�,
���ɰ� ��ɼ��� ��� �����
���踦 �����ݴϴ�.
*/
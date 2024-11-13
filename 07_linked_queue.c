#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef int DataType;

// ��� ����ü ����
typedef struct Node {
    DataType data;
    struct Node* next;
} Node;

// ť ����ü ����
typedef struct {
    Node* front;    // ù ��° ��带 ����Ű�� ������
    Node* rear;     // ������ ��带 ����Ű�� ������
    size_t size;    // ���� ť�� ����� ����� ��
} LinkedQueue;

/* ���� ó���� ���� ������ ���� */
typedef enum {
    QUEUE_OK,
    QUEUE_EMPTY,
    QUEUE_MEMORY_ERROR
} QueueResult;

/* ���ο� ��� ����
 * - �Ű�����: data - ������ ������
 * - ��ȯ��: ������ ����� ������ �Ǵ� ���� �� NULL
 */
static Node* node_create(DataType data) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        return NULL;
    }
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

/* ť ����
 * - ���ο� �� ť�� �������� �Ҵ��ϰ� �ʱ�ȭ
 * - ��ȯ��: ������ ť�� ������ �Ǵ� ���� �� NULL
 */
LinkedQueue* queue_create(void) {
    LinkedQueue* queue = (LinkedQueue*)malloc(sizeof(LinkedQueue));
    if (queue == NULL) {
        return NULL;
    }
    queue->front = NULL;
    queue->rear = NULL;
    queue->size = 0;
    return queue;
}

/* ť�� ����ִ��� Ȯ��
 * - �Ű�����: queue - �˻��� ť
 * - ��ȯ��: ��������� true, �ƴϸ� false
 */
bool queue_is_empty(const LinkedQueue* queue) {
    return (queue->size == 0);
}

/* ť�� ���� ũ�� ��ȯ
 * - �Ű�����: queue - ��� ť
 * - ��ȯ��: ť�� ����� ����� ��
 */
size_t queue_size(const LinkedQueue* queue) {
    return queue->size;
}

/* ť�� ��� �߰� (enqueue)
 * - �Ű�����: queue - ��� ť, data - �߰��� ������
 * - ��ȯ��: ���� ����� ��Ÿ���� QueueResult
 */
QueueResult queue_enqueue(LinkedQueue* queue, DataType data) {
    Node* new_node = node_create(data);
    if (new_node == NULL) {
        return QUEUE_MEMORY_ERROR;
    }

    if (queue_is_empty(queue)) {
        queue->front = new_node;
    }
    else {
        queue->rear->next = new_node;
    }
    queue->rear = new_node;
    queue->size++;
    return QUEUE_OK;
}

/* ť���� ��� ���� (dequeue)
 * - �Ű�����: queue - ��� ť, data - ���ŵ� �����͸� ������ ������
 * - ��ȯ��: ���� ����� ��Ÿ���� QueueResult
 */
QueueResult queue_dequeue(LinkedQueue* queue, DataType* data) {
    if (queue_is_empty(queue)) {
        return QUEUE_EMPTY;
    }

    Node* temp = queue->front;
    *data = temp->data;
    queue->front = temp->next;

    if (queue->front == NULL) {
        queue->rear = NULL;
    }

    free(temp);
    queue->size--;
    return QUEUE_OK;
}

/* ť�� �� �� ��� Ȯ�� (peek)
 * - �Ű�����: queue - ��� ť, data - �����͸� ������ ������
 * - ��ȯ��: ���� ����� ��Ÿ���� QueueResult
 */
QueueResult queue_peek(const LinkedQueue* queue, DataType* data) {
    if (queue_is_empty(queue)) {
        return QUEUE_EMPTY;
    }

    *data = queue->front->data;
    return QUEUE_OK;
}

/* ť �ʱ�ȭ (clear)
 * - �Ű�����: queue - �ʱ�ȭ�� ť
 */
void queue_clear(LinkedQueue* queue) {
    while (!queue_is_empty(queue)) {
        DataType dummy;
        queue_dequeue(queue, &dummy);
    }
}

/* ť �޸� ����
 * - �Ű�����: queue - ������ ť
 */
void queue_destroy(LinkedQueue* queue) {
    queue_clear(queue);
    free(queue);
}

/* ť�� ��� ��� ���
 * - �Ű�����: queue - ����� ť
 */
void queue_print(const LinkedQueue* queue) {
    if (queue_is_empty(queue)) {
        printf("Queue is empty\n");
        return;
    }

    printf("Queue (size=%zu): FRONT [ ", queue->size);
    Node* current = queue->front;
    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("] REAR\n");
}

/* ť�� ���� ���� ���
 * - �Ű�����: queue - ���¸� Ȯ���� ť
 */
void queue_status(const LinkedQueue* queue) {
    printf("\nQueue Status:\n");
    printf("- Size: %zu\n", queue->size);
    printf("- Empty: %s\n", queue_is_empty(queue) ? "Yes" : "No");

    // �޸� ��뷮 ���
    size_t queue_memory = sizeof(LinkedQueue);
    size_t node_memory = sizeof(Node) * queue->size;
    printf("- Memory usage: %zu bytes (overhead: %zu bytes, nodes: %zu bytes)\n",
        queue_memory + node_memory, queue_memory, node_memory);

    if (!queue_is_empty(queue)) {
        printf("- Front value: %d\n", queue->front->data);
        printf("- Rear value: %d\n", queue->rear->data);
    }
}

/* �޴� ��� �Լ� */
void print_menu(void) {
    printf("\n=== Linked Queue Menu ===\n");
    printf("1. Enqueue\n");
    printf("2. Dequeue\n");
    printf("3. Peek\n");
    printf("4. Print queue\n");
    printf("5. Check if empty\n");
    printf("6. Get size\n");
    printf("7. Clear queue\n");
    printf("8. Show queue status\n");
    printf("0. Exit\n");
    printf("Choice: ");
}

/* ���� �޽��� ��� �Լ�
 * - �Ű�����: result - ť ���� ���
 */
void print_error(QueueResult result) {
    switch (result) {
    case QUEUE_EMPTY:
        printf("Error: Queue is empty\n");
        break;
    case QUEUE_MEMORY_ERROR:
        printf("Error: Memory allocation failed\n");
        break;
    default:
        break;
    }
}

int main(void) {
    LinkedQueue* queue = queue_create();
    if (queue == NULL) {
        printf("Failed to create queue\n");
        return 1;
    }

    int choice;
    DataType value;
    QueueResult result;

    do {
        print_menu();
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');  // �Է� ���� ����
            continue;
        }

        switch (choice) {
        case 1:  // Enqueue
            printf("Enter value to enqueue: ");
            scanf("%d", &value);
            result = queue_enqueue(queue, value);
            if (result == QUEUE_OK) {
                printf("Successfully enqueued %d\n", value);
            }
            else {
                print_error(result);
            }
            break;

        case 2:  // Dequeue
            result = queue_dequeue(queue, &value);
            if (result == QUEUE_OK) {
                printf("Dequeued value: %d\n", value);
            }
            else {
                print_error(result);
            }
            break;

        case 3:  // Peek
            result = queue_peek(queue, &value);
            if (result == QUEUE_OK) {
                printf("Front value: %d\n", value);
            }
            else {
                print_error(result);
            }
            break;

        case 4:  // Print queue
            queue_print(queue);
            break;

        case 5:  // Check if empty
            printf("Queue is %sempty\n", queue_is_empty(queue) ? "" : "not ");
            break;

        case 6:  // Get size
            printf("Queue size: %zu\n", queue_size(queue));
            break;

        case 7:  // Clear queue
            queue_clear(queue);
            printf("Queue cleared\n");
            break;

        case 8:  // Show queue status
            queue_status(queue);
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

1. ���� ����Ʈ ��� ť�� ����
-------------------------
- FIFO(First In First Out) ��Ģ
- ��� ����� ���� ����
- front�� rear ������ Ȱ��
- ���� �޸� �Ҵ�/����

2. ����ü ����
-----------
Node:
- data: ������ ������
- next: ���� ��� ������

LinkedQueue:
- front: ù ��� ������
- rear: ������ ��� ������
- size: ���� ��� ��

3. �ٽ� ����� ���⵵
-----------------
Enqueue: O(1)
- �� ��� ����
- rear ������ ����
- size ����

Dequeue: O(1)
- front ��� ����
- front ������ ����
- size ����

Peek: O(1)
- front ��� ������ ��ȯ

4. �޸� ���� ����
---------------
- ���� �Ҵ� (malloc)
- ������ ���� (free)
- �޸� ���� ����
- NULL ������ ó��

5. ��� ���� ó��
-------------
�� ť:
- front == NULL
- rear == NULL
- size == 0

���� ���:
- front == rear
- size == 1

��� ���� ��:
- front ����
- rear ���� (�ʿ��)

6. ���� Ư¡
----------
- Ÿ�� �߻�ȭ (DataType)
- ���� ���� ������
- ������ �޸� ����
- ��Ȯ�� �������̽�

7. ���� ����ȭ
-----------
- O(1) ����/����
- ������ ȿ����
- ���ʿ��� ���� ����
- �޸� ����ȭ

8. ������ ��ġ
-----------
- ������ ���� �н�
- ���� �޸� ����
- �ڷᱸ�� ���� �ľ�
- �߻�ȭ ���� ����

9. ������ �������
--------------
- NULL üũ
- �޸� ���� ����
- ������ ��ȿ��
- ��� ���� ó��

10. �ڵ� ǰ��
----------
- �ϰ��� ����Ģ
- ��Ȯ�� �ּ�
- ���ȭ�� ����
- ���� ó��
*/
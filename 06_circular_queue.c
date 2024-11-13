#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// ť�� �⺻ ũ�� ����
#define INITIAL_CAPACITY 8

typedef int ElementType;

// ť ����ü ����
typedef struct {
    ElementType* elements;  // ��Ҹ� ������ ���� �迭
    size_t capacity;       // ť�� �ִ� �뷮
    size_t front;          // ù ��° ����� �ε���
    size_t rear;           // ������ ��� ������ �ε���
    size_t size;           // ���� ����� ����� ��
} CircularQueue;

/* ���� ó���� ���� ������ ����
 * - ť ������ �پ��� ��� ���¸� ǥ��
 */
typedef enum {
    QUEUE_OK,
    QUEUE_EMPTY,
    QUEUE_FULL,
    QUEUE_MEMORY_ERROR
} QueueResult;

/* ť ���� �Լ�
 * - �ʱ� ũ���� ���� ť�� �������� �Ҵ��ϰ� �ʱ�ȭ
 * - ��ȯ��: ������ ť�� ������ �Ǵ� ���� �� NULL
 */
CircularQueue* queue_create(void) {
    CircularQueue* queue = (CircularQueue*)malloc(sizeof(CircularQueue));
    if (queue == NULL) {
        return NULL;
    }

    queue->elements = (ElementType*)malloc(INITIAL_CAPACITY * sizeof(ElementType));
    if (queue->elements == NULL) {
        free(queue);
        return NULL;
    }

    queue->capacity = INITIAL_CAPACITY;
    queue->front = 0;
    queue->rear = 0;
    queue->size = 0;
    return queue;
}

/* ť�� ����ִ��� Ȯ��
 * - �Ű�����: queue - �˻��� ť
 * - ��ȯ��: ��������� true, �ƴϸ� false
 */
bool queue_is_empty(const CircularQueue* queue) {
    return (queue->size == 0);
}

/* ť�� ���� á���� Ȯ��
 * - �Ű�����: queue - �˻��� ť
 * - ��ȯ��: ���� á���� true, �ƴϸ� false
 */
bool queue_is_full(const CircularQueue* queue) {
    return (queue->size == queue->capacity);
}

/* ť�� ���� ũ�� ��ȯ
 * - �Ű�����: queue - ��� ť
 * - ��ȯ��: ť�� ����� ����� ��
 */
size_t queue_size(const CircularQueue* queue) {
    return queue->size;
}

/* ť�� �뷮 ��ȯ
 * - �Ű�����: queue - ��� ť
 * - ��ȯ��: ť�� �ִ� �뷮
 */
size_t queue_capacity(const CircularQueue* queue) {
    return queue->capacity;
}

/* ���� ��ġ ���
 * - �Ű�����: current - ���� ��ġ, capacity - ť�� �뷮
 * - ��ȯ��: ���� ��ȯ ��ġ
 */
static size_t next_position(size_t current, size_t capacity) {
    return (current + 1) % capacity;
}

/* ť Ȯ��
 * - �Ű�����: queue - ��� ť
 * - ��ȯ��: ���� �� true, ���� �� false
 */
static bool queue_resize(CircularQueue* queue) {
    size_t new_capacity = queue->capacity * 2;
    ElementType* new_elements = (ElementType*)malloc(new_capacity * sizeof(ElementType));
    if (new_elements == NULL) {
        return false;
    }

    // ���� ��ҵ��� �� �迭�� ���� (���� ����)
    size_t j = 0;
    size_t i = queue->front;
    for (size_t count = 0; count < queue->size; count++) {
        new_elements[j] = queue->elements[i];
        i = next_position(i, queue->capacity);
        j++;
    }

    free(queue->elements);
    queue->elements = new_elements;
    queue->capacity = new_capacity;
    queue->front = 0;
    queue->rear = queue->size;
    return true;
}

/* ť�� ��� �߰� (enqueue)
 * - �Ű�����: queue - ��� ť, value - �߰��� ��
 * - ��ȯ��: ���� ����� ��Ÿ���� QueueResult
 */
QueueResult queue_enqueue(CircularQueue* queue, ElementType value) {
    if (queue_is_full(queue)) {
        if (!queue_resize(queue)) {
            return QUEUE_MEMORY_ERROR;
        }
    }

    queue->elements[queue->rear] = value;
    queue->rear = next_position(queue->rear, queue->capacity);
    queue->size++;
    return QUEUE_OK;
}

/* ť���� ��� ���� (dequeue)
 * - �Ű�����: queue - ��� ť, value - ���ŵ� ���� ������ ������
 * - ��ȯ��: ���� ����� ��Ÿ���� QueueResult
 */
QueueResult queue_dequeue(CircularQueue* queue, ElementType* value) {
    if (queue_is_empty(queue)) {
        return QUEUE_EMPTY;
    }

    *value = queue->elements[queue->front];
    queue->front = next_position(queue->front, queue->capacity);
    queue->size--;
    return QUEUE_OK;
}

/* ť�� �� �� ��� Ȯ�� (peek)
 * - �Ű�����: queue - ��� ť, value - ���� ������ ������
 * - ��ȯ��: ���� ����� ��Ÿ���� QueueResult
 */
QueueResult queue_peek(const CircularQueue* queue, ElementType* value) {
    if (queue_is_empty(queue)) {
        return QUEUE_EMPTY;
    }

    *value = queue->elements[queue->front];
    return QUEUE_OK;
}

/* ť �ʱ�ȭ (clear)
 * - �Ű�����: queue - �ʱ�ȭ�� ť
 */
void queue_clear(CircularQueue* queue) {
    queue->front = 0;
    queue->rear = 0;
    queue->size = 0;
}

/* ť �޸� ����
 * - �Ű�����: queue - ������ ť
 */
void queue_destroy(CircularQueue* queue) {
    free(queue->elements);
    free(queue);
}

/* ť�� ��� ��� ���
 * - �Ű�����: queue - ����� ť
 */
void queue_print(const CircularQueue* queue) {
    if (queue_is_empty(queue)) {
        printf("Queue is empty\n");
        return;
    }

    printf("Queue (size=%zu, capacity=%zu): FRONT [ ",
        queue->size, queue->capacity);

    size_t i = queue->front;
    for (size_t count = 0; count < queue->size; count++) {
        printf("%d ", queue->elements[i]);
        i = next_position(i, queue->capacity);
    }
    printf("] REAR\n");
}

/* ť�� ���� ���� ���
 * - �Ű�����: queue - ���¸� Ȯ���� ť
 */
void queue_status(const CircularQueue* queue) {
    printf("\nQueue Status:\n");
    printf("- Size: %zu\n", queue->size);
    printf("- Capacity: %zu\n", queue->capacity);
    printf("- Front Index: %zu\n", queue->front);
    printf("- Rear Index: %zu\n", queue->rear);
    printf("- Empty: %s\n", queue_is_empty(queue) ? "Yes" : "No");
    printf("- Full: %s\n", queue_is_full(queue) ? "Yes" : "No");
    float usage = (float)queue->size / queue->capacity * 100;
    printf("- Usage: %.1f%%\n", usage);

    // ���� �迭 ���� �ð�ȭ
    printf("- Internal Array: [");
    for (size_t i = 0; i < queue->capacity; i++) {
        if (i == queue->front) printf("F");
        if (i == queue->rear) printf("R");

        bool is_used = false;
        if (queue->front <= queue->rear) {
            is_used = (i >= queue->front && i < queue->rear);
        }
        else {
            is_used = (i >= queue->front || i < queue->rear);
        }

        if (is_used) {
            printf("*");
        }
        else {
            printf("_");
        }
    }
    printf("]\n");
}

/* �޴� ��� �Լ� */
void print_menu(void) {
    printf("\n=== Circular Queue Menu ===\n");
    printf("1. Enqueue\n");
    printf("2. Dequeue\n");
    printf("3. Peek\n");
    printf("4. Print queue\n");
    printf("5. Check if empty\n");
    printf("6. Check if full\n");
    printf("7. Get size\n");
    printf("8. Clear queue\n");
    printf("9. Show queue status\n");
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
    case QUEUE_FULL:
        printf("Error: Queue is full\n");
        break;
    case QUEUE_MEMORY_ERROR:
        printf("Error: Memory allocation failed\n");
        break;
    default:
        break;
    }
}

int main(void) {
    CircularQueue* queue = queue_create();
    if (queue == NULL) {
        printf("Failed to create queue\n");
        return 1;
    }

    int choice;
    ElementType value;
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

        case 6:  // Check if full
            printf("Queue is %sfull\n", queue_is_full(queue) ? "" : "not ");
            break;

        case 7:  // Get size
            printf("Queue size: %zu\n", queue_size(queue));
            break;

        case 8:  // Clear queue
            queue_clear(queue);
            printf("Queue cleared\n");
            break;

        case 9:  // Show queue status
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

1. ���� ť�� Ư¡
--------------
- �迭�� ��ȯ�� ���
- front�� rear ������ ����
- ȿ������ �޸� Ȱ��
- ���� ũ�� ���� ����

2. �ֿ� ���� Ư¡
--------------
CircularQueue ����ü:
- elements: ������ ���� �迭
- capacity: �ִ� �뷮
- front: ù ��� ��ġ
- rear: ���� ���� ��ġ
- size: ���� ��� ��

3. �ٽ� ����
----------
�⺻ ����:
- Enqueue: O(1)
- Dequeue: O(1)
- Peek: O(1)

���� ����:
- IsEmpty: O(1)
- IsFull: O(1)
- Clear: O(1)
- Resize: O(n)

4. �޸� ���� ����
----------------
- ���� �迭 ���
- �ڵ� ũ�� ����
- ��ȯ�� �ε��� ����
- �޸� ȿ���� ���

5. �ε��� ����
-----------
- ���� ���� ���
- front�� rear ��ȯ
- �迭 ��� ó��
- ��� �� ����

6. ������ �������
---------------
- �޸� ���� ����
- NULL ������ �˻�
- ��� ���� ó��
- ũ�� ���� ���� ó��

7. ����ȭ Ư¡
-----------
- ��� �ð� ����
- ȿ������ �޸� ���
- ĳ�� ģȭ�� ����
- �ּ����� �޸� �̵�

8. Ȯ�强
-------
- ���� ũ��
*/
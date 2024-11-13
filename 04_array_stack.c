#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// ������ �⺻ ũ��� Ȯ�� ���� ����
#define INITIAL_CAPACITY 8
#define GROWTH_FACTOR 2
#define SHRINK_THRESHOLD 0.25

typedef int ElementType;

// ���� ����ü ����
typedef struct {
    ElementType* elements;  // ��Ҹ� ������ ���� �迭
    size_t size;           // ���� ����� ����� ��
    size_t capacity;       // ���� �Ҵ�� �迭�� ũ��
} Stack;

/* ���� ó���� ���� ������ ����
 * - ���� ������ �پ��� ��� ���¸� ǥ��
 */
typedef enum {
    STACK_OK,
    STACK_EMPTY,
    STACK_FULL,
    STACK_MEMORY_ERROR
} StackResult;

/* ���� ���� �Լ�
 * - �ʱ� ũ���� ������ �������� �Ҵ��ϰ� �ʱ�ȭ
 * - ��ȯ��: ������ ������ ������ �Ǵ� ���� �� NULL
 */
Stack* stack_create(void) {
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    if (stack == NULL) {
        return NULL;
    }

    stack->elements = (ElementType*)malloc(INITIAL_CAPACITY * sizeof(ElementType));
    if (stack->elements == NULL) {
        free(stack);
        return NULL;
    }

    stack->size = 0;
    stack->capacity = INITIAL_CAPACITY;
    return stack;
}

/* ������ ����ִ��� Ȯ��
 * - �Ű�����: stack - �˻��� ����
 * - ��ȯ��: ��������� true, �ƴϸ� false
 */
bool stack_is_empty(const Stack* stack) {
    return (stack->size == 0);
}

/* ������ ���� á���� Ȯ��
 * - �Ű�����: stack - �˻��� ����
 * - ��ȯ��: ���� á���� true, �ƴϸ� false
 */
bool stack_is_full(const Stack* stack) {
    return (stack->size == stack->capacity);
}

/* ������ ���� ũ�� ��ȯ
 * - �Ű�����: stack - ��� ����
 * - ��ȯ��: ���ÿ� ����� ����� ��
 */
size_t stack_size(const Stack* stack) {
    return stack->size;
}

/* ������ �뷮 ��ȯ
 * - �Ű�����: stack - ��� ����
 * - ��ȯ��: ������ ���� �ִ� �뷮
 */
size_t stack_capacity(const Stack* stack) {
    return stack->capacity;
}

/* ���� �迭 ũ�� ����
 * - �Ű�����: stack - ��� ����, new_capacity - ���ο� �뷮
 * - ��ȯ��: ���� �� true, ���� �� false
 */
static bool stack_resize(Stack* stack, size_t new_capacity) {
    ElementType* new_elements = (ElementType*)realloc(stack->elements,
        new_capacity * sizeof(ElementType));
    if (new_elements == NULL) {
        return false;
    }

    stack->elements = new_elements;
    stack->capacity = new_capacity;
    return true;
}

/* ���ÿ� ��� �߰� (push)
 * - �Ű�����: stack - ��� ����, value - �߰��� ��
 * - ��ȯ��: ���� ����� ��Ÿ���� StackResult
 */
StackResult stack_push(Stack* stack, ElementType value) {
    if (stack_is_full(stack)) {
        size_t new_capacity = stack->capacity * GROWTH_FACTOR;
        if (!stack_resize(stack, new_capacity)) {
            return STACK_MEMORY_ERROR;
        }
    }

    stack->elements[stack->size++] = value;
    return STACK_OK;
}

/* ���ÿ��� ��� ���� (pop)
 * - �Ű�����: stack - ��� ����, value - ���ŵ� ���� ������ ������
 * - ��ȯ��: ���� ����� ��Ÿ���� StackResult
 */
StackResult stack_pop(Stack* stack, ElementType* value) {
    if (stack_is_empty(stack)) {
        return STACK_EMPTY;
    }

    *value = stack->elements[--stack->size];

    // ���� ũ�Ⱑ ����� �۾����� �迭 ũ�� ���
    if (stack->size < stack->capacity * SHRINK_THRESHOLD &&
        stack->capacity > INITIAL_CAPACITY) {
        size_t new_capacity = stack->capacity / GROWTH_FACTOR;
        if (new_capacity < INITIAL_CAPACITY) {
            new_capacity = INITIAL_CAPACITY;
        }
        stack_resize(stack, new_capacity);  // �����ص� ��� ����
    }

    return STACK_OK;
}

/* ������ �� �� ��� Ȯ�� (peek)
 * - �Ű�����: stack - ��� ����, value - ���� ������ ������
 * - ��ȯ��: ���� ����� ��Ÿ���� StackResult
 */
StackResult stack_peek(const Stack* stack, ElementType* value) {
    if (stack_is_empty(stack)) {
        return STACK_EMPTY;
    }

    *value = stack->elements[stack->size - 1];
    return STACK_OK;
}

/* ���� �ʱ�ȭ (clear)
 * - �Ű�����: stack - �ʱ�ȭ�� ����
 */
void stack_clear(Stack* stack) {
    stack->size = 0;
    if (stack->capacity > INITIAL_CAPACITY) {
        stack_resize(stack, INITIAL_CAPACITY);  // �����ص� ��� ����
    }
}

/* ���� �޸� ����
 * - �Ű�����: stack - ������ ����
 */
void stack_destroy(Stack* stack) {
    free(stack->elements);
    free(stack);
}

/* ������ ��� ��� ���
 * - �Ű�����: stack - ����� ����
 */
void stack_print(const Stack* stack) {
    if (stack_is_empty(stack)) {
        printf("Stack is empty\n");
        return;
    }

    printf("Stack (size=%zu, capacity=%zu): ", stack->size, stack->capacity);
    printf("TOP [ ");
    for (size_t i = stack->size; i > 0; i--) {
        printf("%d ", stack->elements[i - 1]);
    }
    printf("] BOTTOM\n");
}

/* ������ ���� ���� ���
 * - �Ű�����: stack - ���¸� Ȯ���� ����
 */
void stack_status(const Stack* stack) {
    printf("\nStack Status:\n");
    printf("- Size: %zu\n", stack->size);
    printf("- Capacity: %zu\n", stack->capacity);
    printf("- Empty: %s\n", stack_is_empty(stack) ? "Yes" : "No");
    printf("- Full: %s\n", stack_is_full(stack) ? "Yes" : "No");
    float usage = stack->size / (float)stack->capacity * 100;
    printf("- Usage: %.1f%%\n", usage);
}

/* �޴� ��� �Լ� */
void print_menu(void) {
    printf("\n=== Array-based Stack Menu ===\n");
    printf("1. Push\n");
    printf("2. Pop\n");
    printf("3. Peek\n");
    printf("4. Print stack\n");
    printf("5. Check if empty\n");
    printf("6. Get size\n");
    printf("7. Clear stack\n");
    printf("8. Show stack status\n");
    printf("0. Exit\n");
    printf("Choice: ");
}

/* ���� �޽��� ��� �Լ�
 * - �Ű�����: result - ���� ���� ���
 */
void print_error(StackResult result) {
    switch (result) {
    case STACK_EMPTY:
        printf("Error: Stack is empty\n");
        break;
    case STACK_FULL:
        printf("Error: Stack is full\n");
        break;
    case STACK_MEMORY_ERROR:
        printf("Error: Memory allocation failed\n");
        break;
    default:
        break;
    }
}

int main(void) {
    Stack* stack = stack_create();
    if (stack == NULL) {
        printf("Failed to create stack\n");
        return 1;
    }

    int choice;
    ElementType value;
    StackResult result;

    do {
        print_menu();
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');  // �Է� ���� ����
            continue;
        }

        switch (choice) {
        case 1:  // Push
            printf("Enter value to push: ");
            scanf("%d", &value);
            result = stack_push(stack, value);
            if (result == STACK_OK) {
                printf("Successfully pushed %d\n", value);
            }
            else {
                print_error(result);
            }
            break;

        case 2:  // Pop
            result = stack_pop(stack, &value);
            if (result == STACK_OK) {
                printf("Popped value: %d\n", value);
            }
            else {
                print_error(result);
            }
            break;

        case 3:  // Peek
            result = stack_peek(stack, &value);
            if (result == STACK_OK) {
                printf("Top value: %d\n", value);
            }
            else {
                print_error(result);
            }
            break;

        case 4:  // Print stack
            stack_print(stack);
            break;

        case 5:  // Check if empty
            printf("Stack is %sempty\n", stack_is_empty(stack) ? "" : "not ");
            break;

        case 6:  // Get size
            printf("Stack size: %zu\n", stack_size(stack));
            break;

        case 7:  // Clear stack
            stack_clear(stack);
            printf("Stack cleared\n");
            break;

        case 8:  // Show stack status
            stack_status(stack);
            break;

        case 0:  // Exit
            printf("Exiting program\n");
            break;

        default:
            printf("Invalid choice\n");
        }
    } while (choice != 0);

    stack_destroy(stack);
    return 0;
}

/*
==========================================
�� ���� �� �ֿ� ����
==========================================

1. �迭 ��� ������ Ư¡
----------------------
- ���ӵ� �޸� ���� ���
- ���� ũ�� ���� ����
- �ε��� ��� ���� ����
- �޸� ȿ���� ����

2. �ֿ� ���� Ư¡
--------------
- ���� �迭 ���
- �ڵ� ũ�� ����
- ȿ������ �޸� ����
- ���� ���� ó��

3. �ٽ� ������ ����
----------------
Stack ����ü:
- elements: ��� ���� �迭
- size: ���� ��� ��
- capacity: ���� �뷮

4. �ֿ� ����
----------
�⺻ ����:
- Push: O(1) - �־��� ��� O(n)
- Pop: O(1) - �־��� ��� O(n)
- Peek: O(1)
���� ����:
- Clear: O(1)
- IsEmpty: O(1)
- IsFull: O(1)

5. �޸� ���� ����
----------------
- ���� ũ�� ����
  * ���� ���: 2��
  * ��� �Ӱ谪: 25%
- �ʱ� �뷮: 8
- �ּ� �뷮 ����

6. ���� ó��
----------
- ����ȭ�� ���� ����
- NULL ������ �˻�
- �޸� �Ҵ� ���� ó��
- ��� ���� ó��

7. ����ȭ Ư¡
------------
- ��� �ð� ����
- ȿ������ �޸� ���
- �ڵ� ũ�� ����
- ĳ�� ģȭ�� ����

8. ������ �������
---------------
- �޸� ���� ����
- �迭 ��� �˻�
- ������ ��ȿ�� �˻�
- �ڿ� ���� ����

9. ������ ��ġ
------------
- �迭 ���� ����
- �޸� ���� �н�
- LIFO ���� �н�
- ���� �Ҵ� ����

10. �ǹ��� Ư¡
-------------
- �߰��� ����
- Ȯ�� ������ ����
- ���� ����ȭ
- �������� ���̼�

�� ������ ���������ε� �����ϸ�,
���� ������Ʈ������ Ȱ���� �� �ִ�
�������̰� ȿ������ ������ �����մϴ�.
*/
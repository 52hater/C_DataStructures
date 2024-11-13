#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef int ElementType;

// ��� ����ü ����
typedef struct Node {
    ElementType data;
    struct Node* next;
} Node;

// ���� ����ü ����
typedef struct {
    Node* top;      // ������ �ֻ��� ��带 ����Ű�� ������
    size_t size;    // ���� ���ÿ� ����� ����� ��
} Stack;

/* ���� ó���� ���� ������ ����
 * - ���� ������ �پ��� ��� ���¸� ǥ��
 */
typedef enum {
    STACK_OK,
    STACK_EMPTY,
    STACK_MEMORY_ERROR
} StackResult;

/* ���ο� ��� ����
 * - �Ű�����: data - ������ ������
 * - ��ȯ��: ������ ����� ������ �Ǵ� ���� �� NULL
 */
static Node* create_node(ElementType data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        return NULL;
    }
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

/* ���� ����
 * - ���ο� �� ������ �������� �Ҵ��ϰ� �ʱ�ȭ
 * - ��ȯ��: ������ ������ ������ �Ǵ� ���� �� NULL
 */
Stack* stack_create(void) {
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    if (stack == NULL) {
        return NULL;
    }
    stack->top = NULL;
    stack->size = 0;
    return stack;
}

/* ������ ����ִ��� Ȯ��
 * - �Ű�����: stack - �˻��� ����
 * - ��ȯ��: ��������� true, �ƴϸ� false
 */
bool stack_is_empty(const Stack* stack) {
    return (stack->size == 0);
}

/* ������ ���� ũ�� ��ȯ
 * - �Ű�����: stack - ��� ����
 * - ��ȯ��: ���ÿ� ����� ����� ��
 */
size_t stack_size(const Stack* stack) {
    return stack->size;
}

/* ���ÿ� ��� �߰� (push)
 * - �Ű�����: stack - ��� ����, value - �߰��� ��
 * - ��ȯ��: ���� ����� ��Ÿ���� StackResult
 */
StackResult stack_push(Stack* stack, ElementType value) {
    Node* newNode = create_node(value);
    if (newNode == NULL) {
        return STACK_MEMORY_ERROR;
    }

    newNode->next = stack->top;
    stack->top = newNode;
    stack->size++;
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

    Node* temp = stack->top;
    *value = temp->data;
    stack->top = temp->next;
    free(temp);
    stack->size--;
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

    *value = stack->top->data;
    return STACK_OK;
}

/* ���� �ʱ�ȭ (clear)
 * - �Ű�����: stack - �ʱ�ȭ�� ����
 */
void stack_clear(Stack* stack) {
    ElementType dummy;
    while (!stack_is_empty(stack)) {
        stack_pop(stack, &dummy);
    }
}

/* ���� �޸� ����
 * - �Ű�����: stack - ������ ����
 */
void stack_destroy(Stack* stack) {
    stack_clear(stack);
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

    printf("Stack (size=%zu): TOP [ ", stack->size);
    Node* current = stack->top;
    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("] BOTTOM\n");
}

/* ������ ���� ���� ���
 * - �Ű�����: stack - ���¸� Ȯ���� ����
 */
void stack_status(const Stack* stack) {
    printf("\nStack Status:\n");
    printf("- Size: %zu\n", stack->size);
    printf("- Empty: %s\n", stack_is_empty(stack) ? "Yes" : "No");

    // �޸� ��뷮 ��� (�뷫��)
    size_t stack_memory = sizeof(Stack);
    size_t node_memory = sizeof(Node) * stack->size;
    printf("- Memory usage: %zu bytes (overhead: %zu bytes, nodes: %zu bytes)\n",
        stack_memory + node_memory, stack_memory, node_memory);
}

/* �޴� ��� �Լ� */
void print_menu(void) {
    printf("\n=== Linked List-based Stack Menu ===\n");
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

1. ���� ����Ʈ ��� ������ Ư¡
---------------------------
- ���� �޸� �Ҵ� ���
- ��� ������ �޸� ����
- ũ�� ���� ����
- ������ ��� ����

2. �迭 ��� vs ���� ����Ʈ ��� ��
--------------------------------
����:
- ũ�� ������ ����
- �޸𸮸� �ʿ��� ��ŭ�� ���
- ����/������ �׻� O(1)
- ���Ҵ� ������� ����

����:
- �߰� �޸� ��� (������)
- ĳ�� ������ ����
- �޸� ����ȭ ���ɼ�
- ���� ��� �Ҵ�/���� �������

3. ���� Ư¡
----------
Node ����ü:
- data: ���� ������
- next: ���� ��� ������

Stack ����ü:
- top: �ֻ��� ��� ������
- size: ���� ��� ��

4. �ֿ� ����
----------
�⺻ ����:
- Push: O(1)
- Pop: O(1)
- Peek: O(1)

���� ����:
- Clear: O(n)
- IsEmpty: O(1)
- Size: O(1)

5. �޸� ����
-----------
- ��� ���� ���� �Ҵ�
- ���� ��� ����
- ���� ����ü ����
- �޸� ���� ����

6. ���� ó��
----------
- �޸� �Ҵ� ����
- �� ���� ó��
- NULL ������ �˻�
- ������ �޸� ����

7. ����ȭ Ư¡
-----------
- ��� �ð� ����/����
- �ּ����� �޸� �̵�
- ȿ������ �޸� ���
- ������ ����

8. ������ �������
--------------
- ������ ��ȿ�� �˻�
- �޸� ���� ����
- �ܰ��� �ڿ� ����
- ��� ���� ó��

9. ������ ��ġ
-----------
- ������ ���� �н�
- ���� �޸� ����
- ���� ����Ʈ ����
- LIFO ���� �н�

10. �ǹ��� Ư¡
------------
- �߰��� ����
- Ȯ�� ������ ����
- ��Ȯ�� ���� ó��
- �������� ���̼�

�� ������ ���� ������Ʈ���� ����� �� �ִ�
�������̰� ȿ������ ���� ����Ʈ ��� ������
�����մϴ�. Ư�� ũ�Ⱑ �������̰ų�
�����ϱ� ����� ��쿡 �����մϴ�.
*/
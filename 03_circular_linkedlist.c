#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef int DataType;

// ��� ����ü ����
typedef struct Node {
    DataType data;
    struct Node* next;  // ���� ��带 ����Ű�� ������
} Node;

// ���� ���� ����Ʈ ����ü ����
typedef struct {
    Node* tail;     // ������ ��带 ����Ű�� ������ (tail->next�� head�� ����Ŵ)
    size_t size;    // ���� ����Ʈ�� ũ��
} CircularLinkedList;

/* ����Ʈ �ʱ�ȭ �Լ�
 * - ���ο� �� ���� ���� ����Ʈ�� �����ϰ� �ʱ�ȭ
 * - ��ȯ��: �ʱ�ȭ�� CircularLinkedList ����ü�� ������
 */
CircularLinkedList* list_create(void) {
    CircularLinkedList* list = (CircularLinkedList*)malloc(sizeof(CircularLinkedList));
    if (list == NULL) {
        return NULL;
    }
    list->tail = NULL;
    list->size = 0;
    return list;
}

/* ���ο� ��� ���� �Լ�
 * - �����͸� ���� ���ο� ��带 ���� �Ҵ�
 * - �Ű�����: data - ������ ������ ��
 * - ��ȯ��: ������ �� ����� ������
 */
Node* node_create(DataType data) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        return NULL;
    }
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

/* ����Ʈ�� ����ִ��� Ȯ��
 * - �Ű�����: list - Ȯ���� ����Ʈ
 * - ��ȯ��: ��������� true, �ƴϸ� false
 */
bool list_is_empty(const CircularLinkedList* list) {
    return (list->size == 0);
}

/* ����Ʈ�� ũ�� ��ȯ
 * - �Ű�����: list - ��� ����Ʈ
 * - ��ȯ��: ����Ʈ�� ���� ��� ����
 */
size_t list_size(const CircularLinkedList* list) {
    return list->size;
}

/* ����Ʈ�� �� �տ� ��� ����
 * - �Ű�����: list - ��� ����Ʈ, data - ������ ������
 * - ��ȯ��: ���� �� true, ���� �� false
 */
bool list_push_front(CircularLinkedList* list, DataType data) {
    Node* new_node = node_create(data);
    if (new_node == NULL) {
        return false;
    }

    if (list_is_empty(list)) {
        new_node->next = new_node;  // �ڱ� �ڽ��� ����Ŵ
        list->tail = new_node;
    }
    else {
        new_node->next = list->tail->next;  // ���� head�� ����Ŵ
        list->tail->next = new_node;        // tail�� ���ο� head�� ����Ŵ
    }
    list->size++;
    return true;
}

/* ����Ʈ�� �� �ڿ� ��� ����
 * - �Ű�����: list - ��� ����Ʈ, data - ������ ������
 * - ��ȯ��: ���� �� true, ���� �� false
 */
bool list_push_back(CircularLinkedList* list, DataType data) {
    Node* new_node = node_create(data);
    if (new_node == NULL) {
        return false;
    }

    if (list_is_empty(list)) {
        new_node->next = new_node;  // �ڱ� �ڽ��� ����Ŵ
    }
    else {
        new_node->next = list->tail->next;  // �� ��尡 head�� ����Ŵ
        list->tail->next = new_node;        // ���� tail�� �� ��带 ����Ŵ
    }
    list->tail = new_node;  // �� ��尡 tail�� ��
    list->size++;
    return true;
}

/* Ư�� ��ġ�� ��� ����
 * - �Ű�����: list - ��� ����Ʈ, position - ������ ��ġ(0-based), data - ������ ������
 * - ��ȯ��: ���� �� true, ���� �� false
 */
bool list_insert_at(CircularLinkedList* list, size_t position, DataType data) {
    if (position > list->size) {
        return false;
    }

    if (position == 0) {
        return list_push_front(list, data);
    }

    if (position == list->size) {
        return list_push_back(list, data);
    }

    Node* new_node = node_create(data);
    if (new_node == NULL) {
        return false;
    }

    Node* current = list->tail->next;  // head���� ����
    for (size_t i = 0; i < position - 1; i++) {
        current = current->next;
    }

    new_node->next = current->next;
    current->next = new_node;
    list->size++;
    return true;
}

/* Ư�� ��ġ�� ��� ����
 * - �Ű�����: list - ��� ����Ʈ, position - ������ ��ġ(0-based)
 * - ��ȯ��: ���� �� true, ���� �� false
 */
bool list_remove_at(CircularLinkedList* list, size_t position) {
    if (list_is_empty(list) || position >= list->size) {
        return false;
    }

    Node* head = list->tail->next;
    Node* target;

    if (position == 0) {
        target = head;
        if (list->size == 1) {
            list->tail = NULL;
        }
        else {
            list->tail->next = head->next;
        }
    }
    else {
        Node* previous = head;
        for (size_t i = 0; i < position - 1; i++) {
            previous = previous->next;
        }
        target = previous->next;
        previous->next = target->next;
        if (target == list->tail) {
            list->tail = previous;
        }
    }

    free(target);
    list->size--;
    return true;
}

/* Ư�� ���� ���� ��� �˻�
 * - �Ű�����: list - ��� ����Ʈ, data - ã�� ��
 * - ��ȯ��: ã�� ����� �ε���, ������ -1
 */
int list_find(const CircularLinkedList* list, DataType data) {
    if (list_is_empty(list)) {
        return -1;
    }

    Node* current = list->tail->next;  // head���� ����
    int index = 0;

    do {
        if (current->data == data) {
            return index;
        }
        current = current->next;
        index++;
    } while (current != list->tail->next);

    return -1;
}

/* ����Ʈ ��� �Լ�
 * - �Ű�����: list - ����� ����Ʈ
 */
void list_print(const CircularLinkedList* list) {
    if (list_is_empty(list)) {
        printf("List is empty\n");
        return;
    }

    printf("List (size=%zu): ", list->size);
    Node* current = list->tail->next;  // head���� ����
    do {
        printf("%d ", current->data);
        current = current->next;
    } while (current != list->tail->next);
    printf("\n");
}

/* ����Ʈ �޸� ����
 * - �Ű�����: list - ������ ����Ʈ
 */
void list_destroy(CircularLinkedList* list) {
    if (!list_is_empty(list)) {
        Node* current = list->tail->next;  // head���� ����
        Node* next;
        do {
            next = current->next;
            free(current);
            current = next;
        } while (current != list->tail->next);
    }
    free(list);
}

/* �޴� ��� �Լ� */
void print_menu(void) {
    printf("\n=== Circular Linked List Menu ===\n");
    printf("1. Insert at front\n");
    printf("2. Insert at back\n");
    printf("3. Insert at position\n");
    printf("4. Remove at position\n");
    printf("5. Search value\n");
    printf("6. Print list\n");
    printf("7. Check if empty\n");
    printf("8. Get size\n");
    printf("0. Exit\n");
    printf("Choice: ");
}

int main(void) {
    CircularLinkedList* list = list_create();
    if (list == NULL) {
        printf("Failed to create list\n");
        return 1;
    }

    int choice;
    DataType data;
    size_t position;

    do {
        print_menu();
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');  // �Է� ���� ����
            continue;
        }

        switch (choice) {
        case 1:  // Insert at front
            printf("Enter value to insert: ");
            scanf("%d", &data);
            if (list_push_front(list, data)) {
                printf("Successfully inserted %d at front\n", data);
            }
            else {
                printf("Failed to insert\n");
            }
            break;

        case 2:  // Insert at back
            printf("Enter value to insert: ");
            scanf("%d", &data);
            if (list_push_back(list, data)) {
                printf("Successfully inserted %d at back\n", data);
            }
            else {
                printf("Failed to insert\n");
            }
            break;

        case 3:  // Insert at position
            printf("Enter position: ");
            scanf("%zu", &position);
            printf("Enter value to insert: ");
            scanf("%d", &data);
            if (list_insert_at(list, position, data)) {
                printf("Successfully inserted %d at position %zu\n", data, position);
            }
            else {
                printf("Failed to insert\n");
            }
            break;

        case 4:  // Remove at position
            printf("Enter position to remove: ");
            scanf("%zu", &position);
            if (list_remove_at(list, position)) {
                printf("Successfully removed element at position %zu\n", position);
            }
            else {
                printf("Failed to remove\n");
            }
            break;

        case 5:  // Search value
            printf("Enter value to search: ");
            scanf("%d", &data);
            int index = list_find(list, data);
            if (index != -1) {
                printf("Value %d found at position %d\n", data, index);
            }
            else {
                printf("Value %d not found in the list\n", data);
            }
            break;

        case 6:  // Print list
            list_print(list);
            break;

        case 7:  // Check if empty
            printf("List is %sempty\n", list_is_empty(list) ? "" : "not ");
            break;

        case 8:  // Get size
            printf("List size: %zu\n", list_size(list));
            break;

        case 0:  // Exit
            printf("Exiting program\n");
            break;

        default:
            printf("Invalid choice\n");
        }
    } while (choice != 0);

    list_destroy(list);
    return 0;
}

/*
==========================================
�� ���� �� �ֿ� ����
==========================================

1. ���� ���� ����Ʈ�� Ư¡
------------------------
- ������ ��尡 ù ��° ��带 ����Ŵ
- ��ȯ ������ ���� ���� ��ȸ ����
- tail->next�� head�� ����Ŵ
- ���� ������(tail)�����ε� ����Ʈ ���� ����

2. ����ü ����
------------
Node:
- data: ���� ������
- next: ���� ��� ������

CircularLinkedList:
- tail: ������ ��� ������
- size: ���� ��� ����

3. �ֿ� �˰��� Ư¡
------------------
���� ����:
- ��/�� ���� �� ��ȯ ���� ����
- tail->next �������� head ����
- �� ����Ʈ ó�� (�ڱ� �ڽ��� ����Ű���� ����)

���� ����:
- ��ȯ ���� ������ ���� ������ ����
- ������ ��� ���� �� tail ����
- ���� ��� ���� �� Ư�� ó��

��ȸ ����:
- do-while ���� ���
- head(tail->next)���� �����Ͽ� �ѹ��� ��ȸ
- tail->next�� ��ȸ ���� ���� üũ

4. �޸� ����
------------
- ���� �Ҵ� �� ����
- ��ȯ ���������� �޸� ���� ����
- NULL ������ �˻�

5. �ֿ� ����ȭ
------------
- O(1) �ð� ���⵵:
  * ��/�� ����
  * ũ�� Ȯ��
- tail ������ Ȱ��:
  * head ���� ����ȭ
  * ������ ��� ��� ����

6. ������ �������
---------------
- ��ȯ ���� ����
- �޸� ���� ����
- ��� ���� ó��:
  * �� ����Ʈ
  * ���� ���
  * ������ ���

7. ���� ����Ʈ�� ����
------------------
- ��ȯ ������ ���� ���� ó�� ����
- ������ ��忡�� ù ��� ��� ����
- tail������ ��ü ����Ʈ

*/
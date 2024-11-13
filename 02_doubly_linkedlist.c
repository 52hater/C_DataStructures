#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef int DataType;

// ��� ����ü ����
typedef struct Node {
    DataType data;
    struct Node* prev;  // ���� ��带 ����Ű�� ������
    struct Node* next;  // ���� ��带 ����Ű�� ������
} Node;

// ���� ���� ����Ʈ ����ü ����
typedef struct {
    Node* head;     // ù ��带 ����Ű�� ������
    Node* tail;     // ������ ��带 ����Ű�� ������
    size_t size;    // ���� ����Ʈ�� ũ��
} DoublyLinkedList;

/* ����Ʈ �ʱ�ȭ �Լ�
 * - ���ο� �� ���� ���� ����Ʈ�� �����ϰ� �ʱ�ȭ
 * - ��ȯ��: �ʱ�ȭ�� DoublyLinkedList ����ü�� ������
 */
DoublyLinkedList* list_create(void) {
    DoublyLinkedList* list = (DoublyLinkedList*)malloc(sizeof(DoublyLinkedList));
    if (list == NULL) {
        return NULL;
    }
    list->head = NULL;
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
    new_node->prev = NULL;
    new_node->next = NULL;
    return new_node;
}

/* ����Ʈ�� ����ִ��� Ȯ��
 * - �Ű�����: list - Ȯ���� ����Ʈ
 * - ��ȯ��: ��������� true, �ƴϸ� false
 */
bool list_is_empty(const DoublyLinkedList* list) {
    return (list->size == 0);
}

/* ����Ʈ�� ũ�� ��ȯ
 * - �Ű�����: list - ��� ����Ʈ
 * - ��ȯ��: ����Ʈ�� ���� ��� ����
 */
size_t list_size(const DoublyLinkedList* list) {
    return list->size;
}

/* ����Ʈ�� �� �տ� ��� ����
 * - �Ű�����: list - ��� ����Ʈ, data - ������ ������
 * - ��ȯ��: ���� �� true, ���� �� false
 */
bool list_push_front(DoublyLinkedList* list, DataType data) {
    Node* new_node = node_create(data);
    if (new_node == NULL) {
        return false;
    }

    if (list_is_empty(list)) {
        list->head = new_node;
        list->tail = new_node;
    }
    else {
        new_node->next = list->head;
        list->head->prev = new_node;
        list->head = new_node;
    }
    list->size++;
    return true;
}

/* ����Ʈ�� �� �ڿ� ��� ����
 * - �Ű�����: list - ��� ����Ʈ, data - ������ ������
 * - ��ȯ��: ���� �� true, ���� �� false
 */
bool list_push_back(DoublyLinkedList* list, DataType data) {
    Node* new_node = node_create(data);
    if (new_node == NULL) {
        return false;
    }

    if (list_is_empty(list)) {
        list->head = new_node;
        list->tail = new_node;
    }
    else {
        new_node->prev = list->tail;
        list->tail->next = new_node;
        list->tail = new_node;
    }
    list->size++;
    return true;
}

/* Ư�� ��ġ�� ��� ����
 * - �Ű�����: list - ��� ����Ʈ, position - ������ ��ġ(0-based), data - ������ ������
 * - ��ȯ��: ���� �� true, ���� �� false
 */
bool list_insert_at(DoublyLinkedList* list, size_t position, DataType data) {
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

    Node* current = list->head;
    for (size_t i = 0; i < position; i++) {
        current = current->next;
    }

    new_node->prev = current->prev;
    new_node->next = current;
    current->prev->next = new_node;
    current->prev = new_node;
    list->size++;
    return true;
}

/* Ư�� ��ġ�� ��� ����
 * - �Ű�����: list - ��� ����Ʈ, position - ������ ��ġ(0-based)
 * - ��ȯ��: ���� �� true, ���� �� false
 */
bool list_remove_at(DoublyLinkedList* list, size_t position) {
    if (list_is_empty(list) || position >= list->size) {
        return false;
    }

    Node* target;
    if (position == 0) {
        target = list->head;
        list->head = target->next;
        if (list->head) {
            list->head->prev = NULL;
        }
        else {
            list->tail = NULL;
        }
    }
    else if (position == list->size - 1) {
        target = list->tail;
        list->tail = target->prev;
        list->tail->next = NULL;
    }
    else {
        target = list->head;
        for (size_t i = 0; i < position; i++) {
            target = target->next;
        }
        target->prev->next = target->next;
        target->next->prev = target->prev;
    }

    free(target);
    list->size--;
    return true;
}

/* Ư�� ���� ���� ��� �˻�
 * - �Ű�����: list - ��� ����Ʈ, data - ã�� ��
 * - ��ȯ��: ã�� ����� �ε���, ������ -1
 */
int list_find(const DoublyLinkedList* list, DataType data) {
    Node* current = list->head;
    int index = 0;

    while (current != NULL) {
        if (current->data == data) {
            return index;
        }
        current = current->next;
        index++;
    }
    return -1;
}

/* ����Ʈ ��� �Լ�
 * - �Ű�����: list - ����� ����Ʈ
 */
void list_print(const DoublyLinkedList* list) {
    if (list_is_empty(list)) {
        printf("List is empty\n");
        return;
    }

    printf("List (size=%zu): ", list->size);
    Node* current = list->head;
    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}

/* ����Ʈ �޸� ����
 * - �Ű�����: list - ������ ����Ʈ
 */
void list_destroy(DoublyLinkedList* list) {
    Node* current = list->head;
    while (current != NULL) {
        Node* next = current->next;
        free(current);
        current = next;
    }
    free(list);
}

/* �޴� ��� �Լ� */
void print_menu(void) {
    printf("\n=== Doubly Linked List Menu ===\n");
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
    DoublyLinkedList* list = list_create();
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
- �� ��尡 ���� ���� ���� ����� �����͸� ��� ����
- ����� ��ȸ ����
- ����/���� ������ ���� ���� ����Ʈ���� ȿ����
- tail �����ͷ� �� ��忡 ��� ���� ����

2. �ֿ� ����ü
------------
Node:
- data: ���� ������ ����
- prev: ���� ��� ������
- next: ���� ��� ������

DoublyLinkedList:
- head: ù ��� ������
- tail: ������ ��� ������
- size: ���� ��� ����

3. ������ �ֿ� ���
----------------
- ��� ����: ��/��/�߰� ��ġ
- ��� ����: ��ġ ���
- �� �˻�
- ����Ʈ ���
- �� ����Ʈ Ȯ��
- ũ�� Ȯ��

4. �޸� ����
------------
- ���� �Ҵ�: malloc() ���
- �Ҵ� ���� ó��: NULL üũ
- �޸� ����: free() ���
- �޸� ���� ����

5. ���� ó��
----------
- ��� ���� �˻�
- �޸� �Ҵ� ���� ó��
- �߸��� ��ġ ���� ����
- �� ����Ʈ ó��

6. ����� �������̽�
-----------------
- �޴� ��� ��ȭ�� �������̽�
- �Է� ����
- ��Ȯ�� �ǵ�� ����
- �߸��� �Է� ó��

7. ����ȭ Ư¡
------------
- O(1) �ð� ���⵵:
  * ��/�� ����
  * ũ�� Ȯ��
  * �� ����Ʈ Ȯ��
- O(n) �ð� ���⵵:
  * �߰� ����/����
  * �� �˻�

8. �ڵ� ��Ÿ��
------------
- �ϰ��� ��� ��Ģ
- �ڼ��� �ּ�
- ���ȭ�� ����
- �������� �������̽�

9. ������ �������
---------------
- NULL ������ �˻�
- �޸� ���� ����
- ��� ���� ó��
- ����� �Է� ����

10. ������ ��ġ
-------------
- ������ ���� �н�
- �޸� ���� �ǽ�
- �ڷᱸ�� ����
- ����� ������

*/
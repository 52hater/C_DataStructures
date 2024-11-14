#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*
ü�̴� ��� �ؽ� ���̺�:
- �浹 �߻��� ���� ����Ʈ�� ó��
- ���� ũ�� �������� ���� ����
- �پ��� �ؽ� �Լ� ����
- ���� ���뿡 ���� �θ� ���
*/

#define INITIAL_SIZE 7  // �Ҽ��� ����Ͽ� �� ���� ����
#define MAX_LOAD_FACTOR 0.75

typedef struct {
    char* key;
    int value;
} KeyValuePair;

typedef struct Node {
    KeyValuePair data;
    struct Node* next;
} Node;

typedef struct {
    Node** buckets;       // ��Ŷ �迭
    size_t size;         // ���� ����� ���� ��
    size_t capacity;     // �ؽ� ���̺� ũ��
} HashTable;

/* ���ڿ� �ؽ� �Լ� (djb2)
 * - ���� ������Ʈ���� ���� ���Ǵ� �ؽ� �Լ�
 */
unsigned long hash_function(const char* str) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }

    return hash;
}

/* �ؽ� ���̺� ���� */
HashTable* hash_table_create(size_t initial_capacity) {
    HashTable* table = (HashTable*)malloc(sizeof(HashTable));
    if (!table) return NULL;

    table->buckets = (Node**)calloc(initial_capacity, sizeof(Node*));
    if (!table->buckets) {
        free(table);
        return NULL;
    }

    table->size = 0;
    table->capacity = initial_capacity;
    return table;
}

/* ��� ���� */
Node* create_node(const char* key, int value) {
    Node* node = (Node*)malloc(sizeof(Node));
    if (!node) return NULL;

    node->data.key = strdup(key);
    if (!node->data.key) {
        free(node);
        return NULL;
    }

    node->data.value = value;
    node->next = NULL;
    return node;
}

/* ��� ���� */
void free_node(Node* node) {
    free(node->data.key);
    free(node);
}

/* ���� �ε� ���� ��� */
double get_load_factor(const HashTable* table) {
    return (double)table->size / table->capacity;
}

/* �ؽ� ���̺� ���ؽ�
 * - ���̺� ũ�⸦ �� ��� �ø��� ��� ���� �����
 */
bool hash_table_resize(HashTable* table) {
    size_t new_capacity = table->capacity * 2;
    Node** new_buckets = (Node**)calloc(new_capacity, sizeof(Node*));
    if (!new_buckets) return false;

    // ��� ���Ҹ� �� ��Ŷ���� ���ġ
    for (size_t i = 0; i < table->capacity; i++) {
        Node* current = table->buckets[i];
        while (current) {
            Node* next = current->next;

            // �� ��ġ ���
            unsigned long hash = hash_function(current->data.key);
            size_t new_index = hash % new_capacity;

            // �� ��Ŷ�� �տ� ����
            current->next = new_buckets[new_index];
            new_buckets[new_index] = current;

            current = next;
        }
    }

    // ���� ��Ŷ �迭 ����
    free(table->buckets);

    // �� ��Ŷ���� ��ü
    table->buckets = new_buckets;
    table->capacity = new_capacity;

    return true;
}

/* ���� ����
 * - �ð����⵵: ��� O(1), �־� O(n)
 */
bool hash_table_insert(HashTable* table, const char* key, int value) {
    // �ε� ���� �˻�
    if (get_load_factor(table) >= MAX_LOAD_FACTOR) {
        if (!hash_table_resize(table)) {
            return false;
        }
    }

    unsigned long hash = hash_function(key);
    size_t index = hash % table->capacity;

    // Ű�� �̹� �����ϴ��� �˻�
    Node* current = table->buckets[index];
    while (current) {
        if (strcmp(current->data.key, key) == 0) {
            current->data.value = value;  // �� ����
            return true;
        }
        current = current->next;
    }

    // �� ��� ����
    Node* new_node = create_node(key, value);
    if (!new_node) return false;

    // ��Ŷ�� �տ� ����
    new_node->next = table->buckets[index];
    table->buckets[index] = new_node;
    table->size++;

    return true;
}

/* �˻� ����
 * - �ð����⵵: ��� O(1), �־� O(n)
 */
bool hash_table_get(const HashTable* table, const char* key, int* value) {
    unsigned long hash = hash_function(key);
    size_t index = hash % table->capacity;

    Node* current = table->buckets[index];
    while (current) {
        if (strcmp(current->data.key, key) == 0) {
            *value = current->data.value;
            return true;
        }
        current = current->next;
    }

    return false;
}

/* ���� ����
 * - �ð����⵵: ��� O(1), �־� O(n)
 */
bool hash_table_remove(HashTable* table, const char* key) {
    unsigned long hash = hash_function(key);
    size_t index = hash % table->capacity;

    Node* current = table->buckets[index];
    Node* prev = NULL;

    while (current) {
        if (strcmp(current->data.key, key) == 0) {
            if (prev) {
                prev->next = current->next;
            }
            else {
                table->buckets[index] = current->next;
            }

            free_node(current);
            table->size--;
            return true;
        }
        prev = current;
        current = current->next;
    }

    return false;
}

/* �ؽ� ���̺� �޸� ���� */
void hash_table_destroy(HashTable* table) {
    if (!table) return;

    for (size_t i = 0; i < table->capacity; i++) {
        Node* current = table->buckets[i];
        while (current) {
            Node* next = current->next;
            free_node(current);
            current = next;
        }
    }

    free(table->buckets);
    free(table);
}

/* �ؽ� ���̺� ���� ��� */
void hash_table_print(const HashTable* table) {
    printf("\nHash Table Status:\n");
    printf("Size: %zu\n", table->size);
    printf("Capacity: %zu\n", table->capacity);
    printf("Load factor: %.2f\n", get_load_factor(table));

    for (size_t i = 0; i < table->capacity; i++) {
        printf("\nBucket %zu: ", i);
        Node* current = table->buckets[i];

        if (!current) {
            printf("Empty");
        }
        else {
            while (current) {
                printf("[%s: %d] -> ", current->data.key, current->data.value);
                current = current->next;
            }
            printf("NULL");
        }
    }
    printf("\n");
}

/* �浹 ��� ��� */
void print_collision_stats(const HashTable* table) {
    size_t empty_buckets = 0;
    size_t max_chain = 0;
    size_t total_chain = 0;
    size_t chains = 0;

    for (size_t i = 0; i < table->capacity; i++) {
        size_t chain_length = 0;
        Node* current = table->buckets[i];

        if (!current) {
            empty_buckets++;
        }
        else {
            while (current) {
                chain_length++;
                current = current->next;
            }
            if (chain_length > 1) chains++;
            if (chain_length > max_chain) max_chain = chain_length;
            total_chain += chain_length;
        }
    }

    printf("\nCollision Statistics:\n");
    printf("Empty buckets: %zu (%.1f%%)\n",
        empty_buckets, (float)empty_buckets / table->capacity * 100);
    printf("Buckets with collisions: %zu\n", chains);
    printf("Maximum chain length: %zu\n", max_chain);
    printf("Average chain length: %.2f\n",
        (float)total_chain / (table->capacity - empty_buckets));
}

/* �޴� ��� */
void print_menu(void) {
    printf("\n=== Hash Table Menu ===\n");
    printf("1. Insert key-value pair\n");
    printf("2. Get value by key\n");
    printf("3. Remove key-value pair\n");
    printf("4. Print hash table\n");
    printf("5. Print collision statistics\n");
    printf("0. Exit\n");
    printf("Choice: ");
}

int main(void) {
    HashTable* table = hash_table_create(INITIAL_SIZE);
    if (!table) {
        printf("Failed to create hash table\n");
        return 1;
    }

    int choice;
    char key[256];
    int value;

    do {
        print_menu();
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');  // ���� ����

        switch (choice) {
        case 1:  // Insert
            printf("Enter key: ");
            if (!fgets(key, sizeof(key), stdin)) continue;
            key[strcspn(key, "\n")] = 0;  // ���� ����

            printf("Enter value: ");
            if (scanf("%d", &value) != 1) {
                printf("Invalid value\n");
                continue;
            }

            if (hash_table_insert(table, key, value)) {
                printf("Successfully inserted (%s: %d)\n", key, value);
            }
            else {
                printf("Failed to insert\n");
            }
            break;

        case 2:  // Get
            printf("Enter key: ");
            if (!fgets(key, sizeof(key), stdin)) continue;
            key[strcspn(key, "\n")] = 0;

            if (hash_table_get(table, key, &value)) {
                printf("Value for key '%s': %d\n", key, value);
            }
            else {
                printf("Key not found\n");
            }
            break;

        case 3:  // Remove
            printf("Enter key: ");
            if (!fgets(key, sizeof(key), stdin)) continue;
            key[strcspn(key, "\n")] = 0;

            if (hash_table_remove(table, key)) {
                printf("Successfully removed key '%s'\n", key);
            }
            else {
                printf("Key not found\n");
            }
            break;

        case 4:  // Print
            hash_table_print(table);
            break;

        case 5:  // Statistics
            print_collision_stats(table);
            break;

        case 0:  // Exit
            printf("Exiting program\n");
            break;

        default:
            printf("Invalid choice\n");
        }
    } while (choice != 0);

    hash_table_destroy(table);
    return 0;
}

/*
==========================================
�� ���� �� �ֿ� ����
==========================================

1. ü�̴��� Ư¡
------------
- �浹 ó���� ����
- �޸� ������� �߻�
- ĳ�� ������ ����
- ������ ������

2. �ð� ���⵵
-----------
��� ���̽�:
- ����: O(1)
- �˻�: O(1)
- ����: O(1)

�־� ���̽�:
- ��� ���� O(n)
- �ؽ� �浹 ��

3. ���� ���⵵
-----------
O(n)
- �߰� ���� ����Ʈ ����
- ���� ũ�� ����
- �ε� ���� ����

4. �ֿ� ���� ���
-------------
�ؽ� �Լ�:
- �յ� ����
- ��� ȿ����
- �浹 �ּ�ȭ

�浹 ó��:
- ���� ����Ʈ ���
- ���� �޸� ����
- ȿ������ Ž��

5. ����ȭ ���
-----------
- �ε� ���� ����
- ���� ũ�� ����
- ȿ���� �ؽ� �Լ�
- �޸� ����

6. �����
-------
����:
- ���� ����
- ���� ����
- ���� ������
- Ȯ�� ����

����:
- �߰� �޸�
- ĳ�� ����
- ������ �������
- Ŭ�����͸�

7. ���� �о�
----------
- �ɺ� ���̺�
- ĳ�� ����
- �����ͺ��̽� �ε���
- �޸� Ǯ

8. ���� Ư¡
----------
- ���� ũ�� ����
- �浹 ���
- ������ �޸� ����
- ����� �������̽�

�� ������ ���� ������Ʈ����
����� �� �ִ� ������ �ؽ� ���̺���
�����ϸ�, ���� �ܰ��� ���� �ּҹ�
������ �� ������ �˴ϴ�.
*/
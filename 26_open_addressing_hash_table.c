#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*
���� �ּҹ� �ؽ� ���̺�:
- �浹 �� �ٸ� ��Ŷ�� Ž��
- ���� �����, ���� �����, ���� �ؽ� ����
- ���� ó���� ���� ǥ��(Tombstone) ���
- �� ���� ĳ�� ������
*/

#define INITIAL_SIZE 7  // �Ҽ� ���
#define MAX_LOAD_FACTOR 0.75
#define DELETED_NODE (void*)(~0)  // Tombstone ��

typedef enum {
    PROBE_LINEAR,      // ���� �����
    PROBE_QUADRATIC,   // ���� �����
    PROBE_DOUBLE_HASH  // ���� �ؽ�
} ProbeType;

typedef struct {
    char* key;    // Ű (NULL: �� ��Ŷ, DELETED_NODE: ������ ��Ŷ)
    int value;    // ��
} Entry;

typedef struct {
    Entry* entries;     // ��Ʈ�� �迭
    size_t capacity;   // ���̺� ũ��
    size_t size;       // ����� ��� ��
    size_t tombstones; // ���� ǥ�� ��
    ProbeType type;    // ���� ���
} HashTable;

/* ���ڿ� �ؽ� �Լ� (djb2) */
unsigned long hash_function(const char* str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

/* ���� �ؽ� �Լ� (���� �ؽ̿�) */
unsigned long hash_function2(const char* str) {
    unsigned long hash = 0;
    int c;
    while ((c = *str++)) {
        hash = hash * 31 + c;
    }
    return hash | 1;  // �׻� Ȧ�� ��ȯ
}

/* �ؽ� ���̺� ���� */
HashTable* hash_table_create(size_t capacity, ProbeType type) {
    HashTable* table = (HashTable*)malloc(sizeof(HashTable));
    if (!table) return NULL;

    table->entries = (Entry*)calloc(capacity, sizeof(Entry));
    if (!table->entries) {
        free(table);
        return NULL;
    }

    table->capacity = capacity;
    table->size = 0;
    table->tombstones = 0;
    table->type = type;
    return table;
}

/* ���� �ε� ���� ��� */
double get_load_factor(const HashTable* table) {
    return (double)(table->size + table->tombstones) / table->capacity;
}

/* Ž�� ��ġ ��� */
size_t get_probe_position(const HashTable* table, const char* key, size_t i) {
    unsigned long hash = hash_function(key);

    switch (table->type) {
    case PROBE_LINEAR:  // ���� �����
        return (hash + i) % table->capacity;

    case PROBE_QUADRATIC:  // ���� �����
        return (hash + i * i) % table->capacity;

    case PROBE_DOUBLE_HASH:  // ���� �ؽ�
        unsigned long hash2 = hash_function2(key);
        return (hash + i * hash2) % table->capacity;

    default:
        return hash % table->capacity;
    }
}

/* �ؽ� ���̺� ���ؽ� */
bool hash_table_resize(HashTable* table) {
    size_t old_capacity = table->capacity;
    Entry* old_entries = table->entries;

    // ���ο� ũ���� ���̺� ����
    size_t new_capacity = old_capacity * 2;
    table->entries = (Entry*)calloc(new_capacity, sizeof(Entry));
    if (!table->entries) {
        table->entries = old_entries;
        return false;
    }

    table->capacity = new_capacity;
    table->size = 0;
    table->tombstones = 0;

    // ���� ��Ʈ������ �� ���̺�� ����
    for (size_t i = 0; i < old_capacity; i++) {
        if (old_entries[i].key && old_entries[i].key != DELETED_NODE) {
            hash_table_insert(table, old_entries[i].key, old_entries[i].value);
            free(old_entries[i].key);
        }
    }

    free(old_entries);
    return true;
}

/* ���� ����
 * - �ð����⵵: ��� O(1), �־� O(n)
 */
bool hash_table_insert(HashTable* table, const char* key, int value) {
    if (get_load_factor(table) >= MAX_LOAD_FACTOR) {
        if (!hash_table_resize(table)) {
            return false;
        }
    }

    size_t i = 0;
    size_t index;

    do {
        index = get_probe_position(table, key, i);

        // �� ��Ŷ�̳� ������ ��Ŷ�� ã��
        if (!table->entries[index].key ||
            table->entries[index].key == DELETED_NODE) {
            // �̹� �ִ� Ű���� Ȯ��
            size_t j = 0;
            size_t check_index;
            do {
                check_index = get_probe_position(table, key, j);
                if (table->entries[check_index].key &&
                    table->entries[check_index].key != DELETED_NODE &&
                    strcmp(table->entries[check_index].key, key) == 0) {
                    table->entries[check_index].value = value;
                    return true;
                }
                j++;
            } while (table->entries[check_index].key &&
                j < table->capacity);

            // ���ο� Ű ����
            if (table->entries[index].key == DELETED_NODE) {
                table->tombstones--;
            }

            table->entries[index].key = strdup(key);
            if (!table->entries[index].key) {
                return false;
            }

            table->entries[index].value = value;
            table->size++;
            return true;
        }

        // �̹� �ִ� Ű�� �� ����
        if (strcmp(table->entries[index].key, key) == 0) {
            table->entries[index].value = value;
            return true;
        }

        i++;
    } while (i < table->capacity);

    return false;  // ���̺��� ���� �� ���
}

/* �˻� ����
 * - �ð����⵵: ��� O(1), �־� O(n)
 */
bool hash_table_get(const HashTable* table, const char* key, int* value) {
    size_t i = 0;
    size_t index;

    do {
        index = get_probe_position(table, key, i);

        if (!table->entries[index].key) {
            return false;  // �� ��Ŷ�� ������ ����
        }

        if (table->entries[index].key != DELETED_NODE &&
            strcmp(table->entries[index].key, key) == 0) {
            *value = table->entries[index].value;
            return true;
        }

        i++;
    } while (i < table->capacity);

    return false;
}

/* ���� ����
 * - �ð����⵵: ��� O(1), �־� O(n)
 */
bool hash_table_remove(HashTable* table, const char* key) {
    size_t i = 0;
    size_t index;

    do {
        index = get_probe_position(table, key, i);

        if (!table->entries[index].key) {
            return false;  // �� ��Ŷ�� ������ ����
        }

        if (table->entries[index].key != DELETED_NODE &&
            strcmp(table->entries[index].key, key) == 0) {
            free(table->entries[index].key);
            table->entries[index].key = DELETED_NODE;
            table->size--;
            table->tombstones++;
            return true;
        }

        i++;
    } while (i < table->capacity);

    return false;
}

/* �ؽ� ���̺� �޸� ���� */
void hash_table_destroy(HashTable* table) {
    if (!table) return;

    for (size_t i = 0; i < table->capacity; i++) {
        if (table->entries[i].key && table->entries[i].key != DELETED_NODE) {
            free(table->entries[i].key);
        }
    }

    free(table->entries);
    free(table);
}

/* �ؽ� ���̺� ���� ��� */
void hash_table_print(const HashTable* table) {
    printf("\nHash Table Status:\n");
    printf("Size: %zu\n", table->size);
    printf("Capacity: %zu\n", table->capacity);
    printf("Tombstones: %zu\n", table->tombstones);
    printf("Load factor: %.2f\n", get_load_factor(table));

    printf("\nTable contents:\n");
    for (size_t i = 0; i < table->capacity; i++) {
        printf("[%zu] ", i);
        if (!table->entries[i].key) {
            printf("Empty\n");
        }
        else if (table->entries[i].key == DELETED_NODE) {
            printf("Deleted\n");
        }
        else {
            printf("%s: %d\n", table->entries[i].key, table->entries[i].value);
        }
    }
}

/* Ž�� ����� ���ڿ��� ��ȯ */
const char* probe_type_to_string(ProbeType type) {
    switch (type) {
    case PROBE_LINEAR:
        return "Linear Probing";
    case PROBE_QUADRATIC:
        return "Quadratic Probing";
    case PROBE_DOUBLE_HASH:
        return "Double Hashing";
    default:
        return "Unknown";
    }
}

/* �޴� ��� */
void print_menu(void) {
    printf("\n=== Hash Table Menu ===\n");
    printf("1. Insert key-value pair\n");
    printf("2. Get value by key\n");
    printf("3. Remove key-value pair\n");
    printf("4. Print hash table\n");
    printf("5. Change probe type\n");
    printf("0. Exit\n");
    printf("Choice: ");
}

int main(void) {
    HashTable* table = hash_table_create(INITIAL_SIZE, PROBE_LINEAR);
    if (!table) {
        printf("Failed to create hash table\n");
        return 1;
    }

    printf("Initial probe type: %s\n", probe_type_to_string(table->type));

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
        while (getchar() != '\n');

        switch (choice) {
        case 1:  // Insert
            printf("Enter key: ");
            if (!fgets(key, sizeof(key), stdin)) continue;
            key[strcspn(key, "\n")] = 0;

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

        case 5:  // Change probe type
            printf("Select probe type:\n");
            printf("1. Linear Probing\n");
            printf("2. Quadratic Probing\n");
            printf("3. Double Hashing\n");
            printf("Choice: ");

            int probe_choice;
            if (scanf("%d", &probe_choice) == 1 &&
                probe_choice >= 1 && probe_choice <= 3) {
                table->type = (ProbeType)(probe_choice - 1);
                printf("Changed to %s\n",
                    probe_type_to_string(table->type));
            }
            else {
                printf("Invalid choice\n");
            }
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

1. ���� �ּҹ��� Ư¡
-----------------
- ��� ���Ұ� �� ���̺� ����
- �� ���� ĳ�� ������
- ������ ������� ����
- Ŭ�����͸� ���� �߻�

2. Ž�� ���
----------
���� �����:
- ���� �ܼ��� ���
- �⺻ Ŭ�����͸� �߻�
- ĳ�� ȿ���� ���

���� �����:
- 2�� Ŭ�����͸� �߻�
- �� ���� �л�
- ��ü Ž�� ���� �����

���� �ؽ�:
- ���� ���� �л�
- �߰� �ؽ� �Լ� �ʿ�
- Ŭ�����͸� �ּ�

3. �ð� ���⵵
-----------
��� ���̽�:
- ����: O(1)
- �˻�: O(1)
- ����: O(1)

�־� ���̽�:
- ��� ���� O(n)
- Ŭ�����͸� �߻� ��

4. ���� ���⵵
-----------
O(n)
- �߰� ������ ����
- Tombstone ����
- ���� �ε� ���� �ʿ�
- �޸� ȿ����

5. Tombstone ó��
--------------
- ���� �� Ư�� ��Ŀ ���
- �˻� �� �ǳʶٱ� ����
- ���� �� ���� ����
- �ֱ��� ���ؽ� �ʿ�

6. �����
-------
����:
- ĳ�� ������
- ������ ����
- �޸� ȿ��
- ���� �ܼ�

����:
- Ŭ�����͸�
- �ε� ���� ����
- ���ؽ� �ʿ�
- ���� ó�� ����

7. ����ȭ ���
-----------
- ȿ���� �ؽ� �Լ�
- ������ ũ�� ����
- ���ؽ� ����
- Ž�� ��� ����

8. ���� �о�
----------
- ĳ�� ����
- �޸� ����
- ���� �����ͼ�
- �ǽð� ó��

�� ������ �� ���� Ž�� �����
��� �����ϸ�, ���� ȯ�濡��
����� �� �ִ� ������ ���� �ּҹ�
�ؽ� ���̺��� �����մϴ�.
*/
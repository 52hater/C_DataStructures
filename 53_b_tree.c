#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
B-Ʈ��:
- �ڰ� ���� ���� Ž�� Ʈ��
- �� ��尡 ���� Ű�� ����
- ��� ���� ��尡 ���� ����
- ��ũ ��� �ڷᱸ���� ����
*/

#define MAX_KEYS 3  // �ִ� Ű ���� (���� = 4)
#define MIN_KEYS MAX_KEYS/2  // �ּ� Ű ����

typedef struct BTreeNode {
    int keys[MAX_KEYS];         // Ű �迭
    struct BTreeNode* children[MAX_KEYS + 1];  // �ڽ� ������ �迭
    int num_keys;               // ���� Ű ����
    bool is_leaf;              // ���� ��� ����
} BTreeNode;

// �� ��� ����
BTreeNode* create_node(bool is_leaf) {
    BTreeNode* node = (BTreeNode*)malloc(sizeof(BTreeNode));
    node->num_keys = 0;
    node->is_leaf = is_leaf;

    for (int i = 0; i <= MAX_KEYS; i++) {
        node->children[i] = NULL;
    }

    return node;
}

// ��� ����
void split_child(BTreeNode* parent, int index, BTreeNode* child) {
    BTreeNode* new_node = create_node(child->is_leaf);

    // �� ���� Ű �̵�
    for (int j = 0; j < MIN_KEYS; j++) {
        new_node->keys[j] = child->keys[j + MIN_KEYS + 1];
    }

    // �ڽ� ������ �̵� (������ �ƴ� ���)
    if (!child->is_leaf) {
        for (int j = 0; j <= MIN_KEYS; j++) {
            new_node->children[j] = child->children[j + MIN_KEYS + 1];
        }
    }

    new_node->num_keys = MIN_KEYS;
    child->num_keys = MIN_KEYS;

    // �θ� ��� ����
    for (int j = parent->num_keys; j >= index + 1; j--) {
        parent->children[j + 1] = parent->children[j];
    }

    parent->children[index + 1] = new_node;

    for (int j = parent->num_keys - 1; j >= index; j--) {
        parent->keys[j + 1] = parent->keys[j];
    }

    parent->keys[index] = child->keys[MIN_KEYS];
    parent->num_keys++;

    printf("\n��� ���� �߻�:\n");
    printf("�߰� Ű %d�� �θ�� �̵�\n", child->keys[MIN_KEYS]);
}

// Ű ���� (�����)
void insert_non_full(BTreeNode* node, int key) {
    int i = node->num_keys - 1;

    if (node->is_leaf) {
        while (i >= 0 && node->keys[i] > key) {
            node->keys[i + 1] = node->keys[i];
            i--;
        }

        node->keys[i + 1] = key;
        node->num_keys++;
        printf("Ű %d�� ���� ��忡 ����\n", key);
    }
    else {
        while (i >= 0 && node->keys[i] > key) {
            i--;
        }
        i++;

        if (node->children[i]->num_keys == MAX_KEYS) {
            split_child(node, i, node->children[i]);

            if (key > node->keys[i]) {
                i++;
            }
        }
        insert_non_full(node->children[i], key);
    }
}

// Ʈ���� Ű ����
BTreeNode* insert(BTreeNode* root, int key) {
    printf("\nŰ %d ���� ����\n", key);

    // ��Ʈ�� ���� �� ���
    if (root->num_keys == MAX_KEYS) {
        BTreeNode* new_root = create_node(false);
        new_root->children[0] = root;
        split_child(new_root, 0, root);

        int i = 0;
        if (new_root->keys[0] < key) {
            i++;
        }
        insert_non_full(new_root->children[i], key);

        printf("���ο� ��Ʈ ����\n");
        return new_root;
    }

    insert_non_full(root, key);
    return root;
}

// Ʈ�� ���
void print_tree(BTreeNode* root, int level) {
    if (!root) return;

    printf("Level %d: ", level);
    for (int i = 0; i < root->num_keys; i++) {
        printf("%d ", root->keys[i]);
    }
    printf("\n");

    if (!root->is_leaf) {
        for (int i = 0; i <= root->num_keys; i++) {
            print_tree(root->children[i], level + 1);
        }
    }
}

// Ű �˻�
bool search(BTreeNode* root, int key, int* level) {
    int i = 0;
    (*level)++;

    while (i < root->num_keys && key > root->keys[i]) {
        i++;
    }

    if (i < root->num_keys && key == root->keys[i]) {
        printf("Ű %d�� ���� %d���� ã��\n", key, *level);
        return true;
    }

    if (root->is_leaf) {
        printf("Ű %d�� ã�� ����\n", key);
        return false;
    }

    return search(root->children[i], key, level);
}

// �޸� ����
void free_tree(BTreeNode* root) {
    if (!root) return;

    if (!root->is_leaf) {
        for (int i = 0; i <= root->num_keys; i++) {
            free_tree(root->children[i]);
        }
    }

    free(root);
}

// Ʈ�� ��� ���
void print_tree_stats(BTreeNode* root, int* total_nodes, int* total_keys, int* height, int current_height) {
    if (!root) return;

    (*total_nodes)++;
    *total_keys += root->num_keys;
    *height = current_height > *height ? current_height : *height;

    if (!root->is_leaf) {
        for (int i = 0; i <= root->num_keys; i++) {
            print_tree_stats(root->children[i], total_nodes, total_keys, height, current_height + 1);
        }
    }
}

int main(void) {
    BTreeNode* root = create_node(true);

    printf("=== B-Ʈ�� �׽�Ʈ (���� %d) ===\n", MAX_KEYS + 1);
    printf("1: Ű ����\n");
    printf("2: Ű �˻�\n");
    printf("3: Ʈ�� ���\n");
    printf("4: Ʈ�� ���\n");
    printf("0: ����\n");

    while (1) {
        int choice, value;
        printf("\n����: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            printf("������ Ű: ");
            scanf("%d", &value);
            root = insert(root, value);
            printf("\n���� Ʈ�� ����:\n");
            print_tree(root, 0);
            break;

        case 2:
            printf("�˻��� Ű: ");
            scanf("%d", &value);
            int level = 0;
            search(root, value, &level);
            break;

        case 3:
            printf("\n���� Ʈ�� ����:\n");
            print_tree(root, 0);
            break;

        case 4: {
            int total_nodes = 0, total_keys = 0, height = 0;
            print_tree_stats(root, &total_nodes, &total_keys, &height, 0);
            printf("\n=== Ʈ�� ��� ===\n");
            printf("�� ��� ��: %d\n", total_nodes);
            printf("�� Ű ��: %d\n", total_keys);
            printf("Ʈ�� ����: %d\n", height);
            printf("��� Ű/���: %.2f\n", (float)total_keys / total_nodes);
            break;
        }

        case 0:
            free_tree(root);
            return 0;

        default:
            printf("�߸��� ����\n");
        }
    }

    return 0;
}

/*
B-Ʈ�� �м�
========

1. �ð� ���⵵
-----------
- �˻�: O(log_t n), t�� ����
- ����: O(log_t n)
- ����: O(log_t n)

2. ���� Ȱ��
---------
- �ּ� 50% ���� Ȱ��
- ���� t-1 ~ 2t-1�� Ű
- ���� ������ ������ ����

3. ��ũ ����ȭ
------------
- ��� ũ�� = ��ũ ���
- IO Ƚ�� �ּ�ȭ
- ���� ���� ȿ����

4. Ȱ�� �о�
---------
- �����ͺ��̽� �ε���
- ���� �ý���
- ��뷮 ������ ó��

�� ������ ���� �����ͺ��̽�
�ý��ۿ��� ���Ǵ� �ε�����
�⺻ ������ �����ݴϴ�.
*/
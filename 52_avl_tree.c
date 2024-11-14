#include <stdio.h>
#include <stdlib.h>

/*
AVL Ʈ��:
- �ڰ� ���� ���� Ž�� Ʈ��
- ��� ����� ����/������ ����Ʈ�� ���� ���̰� �ִ� 1
- ����/���� �� �ڵ� �����ȭ
*/

typedef struct Node {
    int key;
    int height;
    struct Node* left;
    struct Node* right;
} Node;

// ��� ���� ��ȯ
int get_height(Node* node) {
    return node ? node->height : -1;
}

// ���� �μ� ���
int get_balance(Node* node) {
    return node ? get_height(node->left) - get_height(node->right) : 0;
}

// ��� ���� ������Ʈ
void update_height(Node* node) {
    node->height = 1 + (get_height(node->left) > get_height(node->right) ?
        get_height(node->left) : get_height(node->right));
}

// ��ȸ��
Node* rotate_right(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

    x->right = y;
    y->left = T2;

    update_height(y);
    update_height(x);

    return x;
}

// ��ȸ��
Node* rotate_left(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    y->left = x;
    x->right = T2;

    update_height(x);
    update_height(y);

    return y;
}

// �� ��� ����
Node* create_node(int key) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->key = key;
    node->height = 0;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// ��� ����
Node* insert(Node* node, int key, bool* height_changed) {
    if (!node) {
        *height_changed = true;
        return create_node(key);
    }

    if (key < node->key)
        node->left = insert(node->left, key, height_changed);
    else if (key > node->key)
        node->right = insert(node->right, key, height_changed);
    else
        return node;  // �ߺ� Ű ������� ����

    update_height(node);
    int balance = get_balance(node);

    printf("\n���� �� ���� (Ű: %d):\n", key);
    printf("���: %d, ����: %d\n", node->key, balance);

    // LL Case
    if (balance > 1 && key < node->left->key) {
        printf("LL ȸ�� ���� (��� %d)\n", node->key);
        return rotate_right(node);
    }

    // RR Case
    if (balance < -1 && key > node->right->key) {
        printf("RR ȸ�� ���� (��� %d)\n", node->key);
        return rotate_left(node);
    }

    // LR Case
    if (balance > 1 && key > node->left->key) {
        printf("LR ȸ�� ���� (��� %d)\n", node->key);
        node->left = rotate_left(node->left);
        return rotate_right(node);
    }

    // RL Case
    if (balance < -1 && key < node->right->key) {
        printf("RL ȸ�� ���� (��� %d)\n", node->key);
        node->right = rotate_right(node->right);
        return rotate_left(node);
    }

    return node;
}

// �ּҰ� ��� ã��
Node* find_min(Node* node) {
    return node->left ? find_min(node->left) : node;
}

// ��� ����
Node* delete(Node* root, int key, bool* height_changed) {
    if (!root) return NULL;

    if (key < root->key)
        root->left = delete(root->left, key, height_changed);
    else if (key > root->key)
        root->right = delete(root->right, key, height_changed);
    else {
        // ������ ��� ã��
        if (!root->left || !root->right) {
            Node* temp = root->left ? root->left : root->right;
            if (!temp) {
                temp = root;
                root = NULL;
            }
            else {
                *root = *temp;
            }
            free(temp);
            *height_changed = true;
        }
        else {
            Node* temp = find_min(root->right);
            root->key = temp->key;
            root->right = delete(root->right, temp->key, height_changed);
        }
    }

    if (!root) return NULL;

    update_height(root);
    int balance = get_balance(root);

    printf("\n���� �� ���� (Ű: %d):\n", key);
    printf("���: %d, ����: %d\n", root->key, balance);

    // �����ȭ
    if (balance > 1) {
        if (get_balance(root->left) >= 0) {
            printf("LL ȸ�� ���� (��� %d)\n", root->key);
            return rotate_right(root);
        }
        else {
            printf("LR ȸ�� ���� (��� %d)\n", root->key);
            root->left = rotate_left(root->left);
            return rotate_right(root);
        }
    }

    if (balance < -1) {
        if (get_balance(root->right) <= 0) {
            printf("RR ȸ�� ���� (��� %d)\n", root->key);
            return rotate_left(root);
        }
        else {
            printf("RL ȸ�� ���� (��� %d)\n", root->key);
            root->right = rotate_right(root->right);
            return rotate_left(root);
        }
    }

    return root;
}

// Ʈ�� �ð�ȭ
void print_tree(Node* root, int level) {
    if (!root) return;

    print_tree(root->right, level + 1);

    for (int i = 0; i < level; i++)
        printf("    ");
    printf("%d\n", root->key);

    print_tree(root->left, level + 1);
}

// Ʈ�� ���� ���
void print_tree_state(Node* root) {
    printf("\n=== ���� Ʈ�� ���� ===\n");
    print_tree(root, 0);
    printf("\n");
}

// �޸� ����
void free_tree(Node* root) {
    if (root) {
        free_tree(root->left);
        free_tree(root->right);
        free(root);
    }
}

int main(void) {
    Node* root = NULL;
    bool height_changed = false;

    printf("=== AVL Ʈ�� �׽�Ʈ ===\n");
    printf("1: ����\n");
    printf("2: ����\n");
    printf("3: Ʈ�� ���\n");
    printf("0: ����\n");

    while (1) {
        int choice, value;
        printf("\n����: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            printf("������ ��: ");
            scanf("%d", &value);
            root = insert(root, value, &height_changed);
            print_tree_state(root);
            break;

        case 2:
            printf("������ ��: ");
            scanf("%d", &value);
            root = delete(root, value, &height_changed);
            print_tree_state(root);
            break;

        case 3:
            print_tree_state(root);
            break;

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
AVL Ʈ�� �м�
===========

1. �ð� ���⵵
-----------
- ����: O(log n)
- ����: O(log n)
- �˻�: O(log n)

2. ���� ���⵵
-----------
- O(n) ���� ����
- O(log n) ��� ����

3. ����ȭ ����
-----------
- ���� ȸ��: LL, RR
- ���� ȸ��: LR, RL
- ���� ���� �ִ� 1 ����

4. Ȱ�� �о�
---------
- �����ͺ��̽� �ε���
- �޸� ����
- �ǽð� �ý���

�� ������ �ڰ� ���� Ʈ����
�⺻ ������ ���� ������
�����ݴϴ�.
*/
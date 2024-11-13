#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef int DataType;

// ��� ����ü ����
typedef struct TreeNode {
    DataType data;             // ������
    struct TreeNode* left;     // ���� �ڽ� ���
    struct TreeNode* right;    // ������ �ڽ� ���
} TreeNode;

// ���� Ž�� Ʈ�� ����ü ����
typedef struct {
    TreeNode* root;    // ��Ʈ ���
    size_t size;       // ��ü ����� ��
} BSTree;

/* ���� ó���� ���� ������ ���� */
typedef enum {
    TREE_OK,
    TREE_MEMORY_ERROR,
    TREE_EMPTY,
    TREE_NOT_FOUND
} TreeResult;

/* ���ο� ��� ����
 * - �Ű�����: data - ������ ������
 * - ��ȯ��: ������ ����� ������ �Ǵ� ���� �� NULL
 */
static TreeNode* node_create(DataType data) {
    TreeNode* new_node = (TreeNode*)malloc(sizeof(TreeNode));
    if (new_node == NULL) {
        return NULL;
    }
    new_node->data = data;
    new_node->left = NULL;
    new_node->right = NULL;
    return new_node;
}

/* Ʈ�� ����
 * - ���ο� �� ���� Ž�� Ʈ���� �������� �Ҵ��ϰ� �ʱ�ȭ
 * - ��ȯ��: ������ Ʈ���� ������ �Ǵ� ���� �� NULL
 */
BSTree* tree_create(void) {
    BSTree* tree = (BSTree*)malloc(sizeof(BSTree));
    if (tree == NULL) {
        return NULL;
    }
    tree->root = NULL;
    tree->size = 0;
    return tree;
}

/* Ʈ���� ����ִ��� Ȯ��
 * - �Ű�����: tree - �˻��� Ʈ��
 * - ��ȯ��: ��������� true, �ƴϸ� false
 */
bool tree_is_empty(const BSTree* tree) {
    return (tree->root == NULL);
}

/* ����� ��� ���� (���� �Լ�)
 * - �Ű�����: node - ���� ���, data - ������ ������
 * - ��ȯ��: ���� ���� (����)Ʈ�� ��Ʈ ���
 */
static TreeNode* insert_recursive(TreeNode* node, DataType data) {
    if (node == NULL) {
        return node_create(data);
    }

    if (data < node->data) {
        node->left = insert_recursive(node->left, data);
    }
    else if (data > node->data) {
        node->right = insert_recursive(node->right, data);
    }

    return node;
}

/* ��� ���� (�ܺ� �������̽�)
 * - �Ű�����: tree - ��� Ʈ��, data - ������ ������
 * - ��ȯ��: ���� �� TREE_OK, ���� �� ���� �ڵ�
 */
TreeResult tree_insert(BSTree* tree, DataType data) {
    TreeNode* new_root = insert_recursive(tree->root, data);
    if (new_root == NULL && tree->root == NULL) {
        return TREE_MEMORY_ERROR;
    }

    if (tree->root == NULL) {
        tree->root = new_root;
    }
    tree->size++;
    return TREE_OK;
}

/* �ּҰ��� ���� ��� ã�� (���� �Լ�)
 * - �Ű�����: node - ���� ���
 * - ��ȯ��: �ּҰ��� ���� ����� ������
 */
static TreeNode* find_min_node(TreeNode* node) {
    TreeNode* current = node;
    while (current && current->left != NULL) {
        current = current->left;
    }
    return current;
}

/* ��� ���� (���� �Լ�)
 * - �Ű�����: node - ���� ���, data - ������ ������
 * - ��ȯ��: ���� ���� (����)Ʈ�� ��Ʈ ���
 */
static TreeNode* delete_recursive(TreeNode* node, DataType data, bool* success) {
    if (node == NULL) {
        *success = false;
        return NULL;
    }

    if (data < node->data) {
        node->left = delete_recursive(node->left, data, success);
    }
    else if (data > node->data) {
        node->right = delete_recursive(node->right, data, success);
    }
    else {
        *success = true;
        // ��� 1: ���� ���
        if (node->left == NULL && node->right == NULL) {
            free(node);
            return NULL;
        }
        // ��� 2: ���� �ڽĸ� �ִ� ���
        if (node->left == NULL) {
            TreeNode* temp = node->right;
            free(node);
            return temp;
        }
        if (node->right == NULL) {
            TreeNode* temp = node->left;
            free(node);
            return temp;
        }
        // ��� 3: �� �ڽ��� ��� �ִ� ���
        TreeNode* temp = find_min_node(node->right);
        node->data = temp->data;
        node->right = delete_recursive(node->right, temp->data, success);
    }
    return node;
}

/* ��� ���� (�ܺ� �������̽�)
 * - �Ű�����: tree - ��� Ʈ��, data - ������ ������
 * - ��ȯ��: ���� �� TREE_OK, ���� �� ���� �ڵ�
 */
TreeResult tree_delete(BSTree* tree, DataType data) {
    if (tree_is_empty(tree)) {
        return TREE_EMPTY;
    }

    bool success = false;
    tree->root = delete_recursive(tree->root, data, &success);

    if (success) {
        tree->size--;
        return TREE_OK;
    }
    return TREE_NOT_FOUND;
}

/* ��� �˻� (���� �Լ�)
 * - �Ű�����: node - ���� ���, data - �˻��� ������
 * - ��ȯ��: ã�� ����� ������ �Ǵ� NULL
 */
static TreeNode* search_recursive(TreeNode* node, DataType data) {
    if (node == NULL || node->data == data) {
        return node;
    }

    if (data < node->data) {
        return search_recursive(node->left, data);
    }
    return search_recursive(node->right, data);
}

/* ��� �˻� (�ܺ� �������̽�)
 * - �Ű�����: tree - ��� Ʈ��, data - �˻��� ������
 * - ��ȯ��: ���� �� true, ���� �� false
 */
bool tree_search(const BSTree* tree, DataType data) {
    return search_recursive(tree->root, data) != NULL;
}

/* �ּҰ� ã��
 * - �Ű�����: tree - ��� Ʈ��, value - ����� ������ ������
 * - ��ȯ��: ���� �� TREE_OK, ���� �� ���� �ڵ�
 */
TreeResult tree_find_min(const BSTree* tree, DataType* value) {
    if (tree_is_empty(tree)) {
        return TREE_EMPTY;
    }

    TreeNode* min_node = find_min_node(tree->root);
    *value = min_node->data;
    return TREE_OK;
}

/* �ִ밪 ã�� (���� �Լ�)
 * - �Ű�����: node - ���� ���
 * - ��ȯ��: �ִ밪�� ���� ����� ������
 */
static TreeNode* find_max_node(TreeNode* node) {
    TreeNode* current = node;
    while (current && current->right != NULL) {
        current = current->right;
    }
    return current;
}

/* �ִ밪 ã�� (�ܺ� �������̽�)
 * - �Ű�����: tree - ��� Ʈ��, value - ����� ������ ������
 * - ��ȯ��: ���� �� TREE_OK, ���� �� ���� �ڵ�
 */
TreeResult tree_find_max(const BSTree* tree, DataType* value) {
    if (tree_is_empty(tree)) {
        return TREE_EMPTY;
    }

    TreeNode* max_node = find_max_node(tree->root);
    *value = max_node->data;
    return TREE_OK;
}

/* ���� ��ȸ (���� �Լ�)
 * - �Ű�����: node - ���� ���
 */
static void inorder_traversal(TreeNode* node) {
    if (node != NULL) {
        inorder_traversal(node->left);
        printf("%d ", node->data);
        inorder_traversal(node->right);
    }
}

/* ���� ��ȸ (�ܺ� �������̽�)
 * - �Ű�����: tree - ��ȸ�� Ʈ��
 */
void tree_print_sorted(const BSTree* tree) {
    if (tree_is_empty(tree)) {
        printf("Tree is empty\n");
        return;
    }
    printf("Sorted elements: ");
    inorder_traversal(tree->root);
    printf("\n");
}

/* Ʈ�� �ð�ȭ�� ���� ���� �Լ�
 * - �Ű�����: node - ����� ���, level - ���� ����, prefix - ��� ���λ�
 */
static void print_tree_recursive(const TreeNode* node, int level, char* prefix) {
    if (node == NULL) {
        return;
    }

    printf("%s", prefix);
    printf("%s", level ? "������ " : "");
    printf("%d\n", node->data);

    char new_prefix[256];
    sprintf(new_prefix, "%s%s", prefix, level ? "��   " : "");

    print_tree_recursive(node->left, 1, new_prefix);
    print_tree_recursive(node->right, 1, new_prefix);
}

/* Ʈ�� �ð�ȭ (�ܺ� �������̽�)
 * - �Ű�����: tree - ����� Ʈ��
 */
void tree_print(const BSTree* tree) {
    if (tree_is_empty(tree)) {
        printf("Tree is empty\n");
        return;
    }
    printf("Binary Search Tree Structure:\n");
    print_tree_recursive(tree->root, 0, "");
}

/* ����Ʈ�� ���� (���� �Լ�)
 * - �Ű�����: node - ������ ����Ʈ���� ��Ʈ ���
 */
static void delete_subtree(TreeNode* node) {
    if (node == NULL) {
        return;
    }
    delete_subtree(node->left);
    delete_subtree(node->right);
    free(node);
}

/* Ʈ�� �޸� ����
 * - �Ű�����: tree - ������ Ʈ��
 */
void tree_destroy(BSTree* tree) {
    if (tree != NULL) {
        delete_subtree(tree->root);
        free(tree);
    }
}

/* Ʈ���� ũ�� ��ȯ
 * - �Ű�����: tree - ��� Ʈ��
 * - ��ȯ��: Ʈ���� ��� ����
 */
size_t tree_size(const BSTree* tree) {
    return tree->size;
}

/* �޴� ��� �Լ� */
void print_menu(void) {
    printf("\n=== Binary Search Tree Menu ===\n");
    printf("1. Insert element\n");
    printf("2. Delete element\n");
    printf("3. Search element\n");
    printf("4. Find minimum\n");
    printf("5. Find maximum\n");
    printf("6. Print sorted elements\n");
    printf("7. Print tree structure\n");
    printf("8. Get tree size\n");
    printf("0. Exit\n");
    printf("Choice: ");
}

/* ���� �޽��� ��� �Լ� */
void print_error(TreeResult result) {
    switch (result) {
    case TREE_MEMORY_ERROR:
        printf("Error: Memory allocation failed\n");
        break;
    case TREE_EMPTY:
        printf("Error: Tree is empty\n");
        break;
    case TREE_NOT_FOUND:
        printf("Error: Element not found\n");
        break;
    default:
        break;
    }
}

int main(void) {
    BSTree* tree = tree_create();
    if (tree == NULL) {
        printf("Failed to create tree\n");
        return 1;
    }

    int choice;
    DataType value;
    TreeResult result;

    do {
        print_menu();
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');  // �Է� ���� ����
            continue;
        }

        switch (choice) {
        case 1:  // Insert
            printf("Enter value to insert: ");
            scanf("%d", &value);
            result = tree_insert(tree, value);
            if (result == TREE_OK) {
                printf("Successfully inserted %d\n", value);
            }
            else {
                print_error(result);
            }
            break;

        case 2:  // Delete
            printf("Enter value to delete: ");
            scanf("%d", &value);
            result = tree_delete(tree, value);
            if (result == TREE_OK) {
                printf("Successfully deleted %d\n", value);
            }
            else {
                print_error(result);
            }
            break;

        case 3:  // Search
            printf("Enter value to search: ");
            scanf("%d", &value);
            if (tree_search(tree, value)) {
                printf("Found %d in the tree\n", value);
            }
            else {
                printf("Did not find %d in the tree\n", value);
            }
            break;

        case 4:  // Find minimum
            result = tree_find_min(tree, &value);
            if (result == TREE_OK) {
                printf("Minimum value: %d\n", value);
            }
            else {
                print_error(result);
            }
            break;

        case 5:  // Find maximum
            result = tree_find_max(tree, &value);
            if (result == TREE_OK) {
                printf("Maximum value: %d\n", value);
            }
            else {
                print_error(result);
            }
            break;

        case 6:  // Print sorted
            tree_print_sorted(tree);
            break;

        case 7:  // Print structure
            tree_print(tree);
            break;

        case 8:  // Get size
            printf("Tree size: %zu\n", tree_size(tree));
            break;

        case 0:  // Exit
            printf("Exiting program\n");
            break;

        default:
            printf("Invalid choice\n");
        }
    } while (choice != 0);

    tree_destroy(tree);
    return 0;
}

/*
==========================================
�� ���� �� �ֿ� ����
==========================================

1. ���� Ž�� Ʈ���� Ư��
--------------------
- ���� ����Ʈ���� ��� ��� < ���� ���
- ������ ����Ʈ���� ��� ��� > ���� ���
- ���� ��ȸ�� ���ĵ� ���
- ȿ������ �˻� ����

2. �ֿ� ����� �ð� ���⵵
---------------------
����/����/�˻�:
- ���: O(log n)
- �־�: O(n) (�ұ��� Ʈ��)

�ּ�/�ִ밪 ã��:
- O(h) (h�� Ʈ���� ����)
- ���� �������θ� �̵�

3. ��� ������ �� ���� ���
----------------------
Case 1: ���� ���
- �ܼ��� ��� ����

Case 2: �� �ڽ�
- �ڽ��� �θ� ���� ����

Case 3: �� �ڽ�
- �İ���(successor) ã��
- ���� �ļ��ڷ� ��ü

4. ���� Ư¡
----------
- ����� ����
- ������ ��� ����
- ���� �޸� ����
- ���ȭ�� ����

5. �޸� ���� ����
---------------
- ��� ���� �Ҵ�/����
- ����� Ʈ�� �Ҹ�
- �޸� ���� ����
- ������ ������ ó��

6. ��ȸ�� ���
-----------
- ���� ��ȸ
  * ���ĵ� ���� ���
  * ���� Ž�� Ʈ�� ����

- �ð��� ���
  * Ʈ�� ���� ǥ��
  * ����� ����
  * �н� ����

7. ���� ó��
----------
- �޸� �Ҵ� ����
- �� Ʈ�� ó��
- ��� �̹߰�
- �߸��� �Է�

8. ���� �о�
----------
- �����ͺ��̽� �ε���
- �ɺ� ���̺�
- �켱���� ť
- ���� �˰���

9. ����ȭ �������
--------------
- Ʈ�� ���� ����
- �ߺ� Ű ó��
- ��� ���� ����
- ĳ�� ȿ����

10. ������ ��ġ
------------
- ��� ���� �н�
- ������ ����
- Ž�� �˰���
- Ʈ�� ���� ����

�� ������ ���������� ����ȭ�Ǿ� ������,
���� Ž�� Ʈ���� ��� �ٽ� ������
�ٷ�� �ֽ��ϴ�. ���� ������ ����
���ʰ� �Ǵ� ǥ������ ������ �����մϴ�.
*/
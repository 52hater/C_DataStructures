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

// ���� Ʈ�� ����ü ����
typedef struct {
    TreeNode* root;    // ��Ʈ ���
    size_t size;       // ��ü ����� ��
} BinaryTree;

/* ���� ó���� ���� ������ ���� */
typedef enum {
    TREE_OK,
    TREE_MEMORY_ERROR,
    TREE_EMPTY
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
 * - ���ο� �� ���� Ʈ���� �������� �Ҵ��ϰ� �ʱ�ȭ
 * - ��ȯ��: ������ Ʈ���� ������ �Ǵ� ���� �� NULL
 */
BinaryTree* tree_create(void) {
    BinaryTree* tree = (BinaryTree*)malloc(sizeof(BinaryTree));
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
bool tree_is_empty(const BinaryTree* tree) {
    return (tree->root == NULL);
}

/* ���� ��ȸ (���� �Լ�)
 * - Root -> Left -> Right ������ ��ȸ
 */
static void preorder_traversal(TreeNode* node) {
    if (node != NULL) {
        printf("%d ", node->data);      // ���� ��� ó��
        preorder_traversal(node->left);  // ���� ����Ʈ�� ��ȸ
        preorder_traversal(node->right); // ������ ����Ʈ�� ��ȸ
    }
}

/* ���� ��ȸ (���� �Լ�)
 * - Left -> Root -> Right ������ ��ȸ
 */
static void inorder_traversal(TreeNode* node) {
    if (node != NULL) {
        inorder_traversal(node->left);   // ���� ����Ʈ�� ��ȸ
        printf("%d ", node->data);       // ���� ��� ó��
        inorder_traversal(node->right);  // ������ ����Ʈ�� ��ȸ
    }
}

/* ���� ��ȸ (���� �Լ�)
 * - Left -> Right -> Root ������ ��ȸ
 */
static void postorder_traversal(TreeNode* node) {
    if (node != NULL) {
        postorder_traversal(node->left);  // ���� ����Ʈ�� ��ȸ
        postorder_traversal(node->right); // ������ ����Ʈ�� ��ȸ
        printf("%d ", node->data);        // ���� ��� ó��
    }
}

/* ���� ��ȸ (�ܺ� �������̽�)
 * - �Ű�����: tree - ��ȸ�� Ʈ��
 */
void tree_preorder(const BinaryTree* tree) {
    if (tree_is_empty(tree)) {
        printf("Tree is empty\n");
        return;
    }
    printf("Preorder traversal: ");
    preorder_traversal(tree->root);
    printf("\n");
}

/* ���� ��ȸ (�ܺ� �������̽�)
 * - �Ű�����: tree - ��ȸ�� Ʈ��
 */
void tree_inorder(const BinaryTree* tree) {
    if (tree_is_empty(tree)) {
        printf("Tree is empty\n");
        return;
    }
    printf("Inorder traversal: ");
    inorder_traversal(tree->root);
    printf("\n");
}

/* ���� ��ȸ (�ܺ� �������̽�)
 * - �Ű�����: tree - ��ȸ�� Ʈ��
 */
void tree_postorder(const BinaryTree* tree) {
    if (tree_is_empty(tree)) {
        printf("Tree is empty\n");
        return;
    }
    printf("Postorder traversal: ");
    postorder_traversal(tree->root);
    printf("\n");
}

/* ����� ���� ��� (���� �Լ�)
 * - �Ű�����: node - ���̸� ����� ���
 * - ��ȯ��: �ش� ��带 ��Ʈ�� �ϴ� ����Ʈ���� ����
 */
static int calculate_height(const TreeNode* node) {
    if (node == NULL) {
        return -1;
    }
    int left_height = calculate_height(node->left);
    int right_height = calculate_height(node->right);
    return (left_height > right_height ? left_height : right_height) + 1;
}

/* Ʈ���� ���� ��ȯ (�ܺ� �������̽�)
 * - �Ű�����: tree - ���̸� ����� Ʈ��
 * - ��ȯ��: Ʈ���� ����
 */
int tree_height(const BinaryTree* tree) {
    if (tree_is_empty(tree)) {
        return -1;
    }
    return calculate_height(tree->root);
}

/* ����� ���� ��� (���� �Լ�)
 * - �Ű�����: node - ������ ����� ���
 * - ��ȯ��: �ش� ��带 ��Ʈ�� �ϴ� ����Ʈ���� ��� ����
 */
static size_t count_nodes(const TreeNode* node) {
    if (node == NULL) {
        return 0;
    }
    return 1 + count_nodes(node->left) + count_nodes(node->right);
}

/* Ʈ���� ��� ���� ��ȯ (�ܺ� �������̽�)
 * - �Ű�����: tree - ��� ������ ����� Ʈ��
 * - ��ȯ��: Ʈ���� ��ü ��� ����
 */
size_t tree_size(const BinaryTree* tree) {
    return tree->size;
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

    if (node->left || node->right) {
        print_tree_recursive(node->left, 1, new_prefix);
        print_tree_recursive(node->right, 1, new_prefix);
    }
}

/* Ʈ�� �ð�ȭ (�ܺ� �������̽�)
 * - �Ű�����: tree - ����� Ʈ��
 */
void tree_print(const BinaryTree* tree) {
    if (tree_is_empty(tree)) {
        printf("Tree is empty\n");
        return;
    }
    printf("Binary Tree Structure:\n");
    print_tree_recursive(tree->root, 0, "");
}

/* ��� ���� (���� �ڽ�)
 * - �Ű�����: parent - �θ� ���, data - ������ ������
 * - ��ȯ��: ���� �� TREE_OK, ���� �� ���� �ڵ�
 */
TreeResult tree_insert_left(BinaryTree* tree, TreeNode* parent, DataType data) {
    TreeNode* new_node = node_create(data);
    if (new_node == NULL) {
        return TREE_MEMORY_ERROR;
    }

    if (parent == NULL) {
        if (tree->root == NULL) {
            tree->root = new_node;
        }
        else {
            free(new_node);
            return TREE_MEMORY_ERROR;
        }
    }
    else {
        if (parent->left != NULL) {
            new_node->left = parent->left;
        }
        parent->left = new_node;
    }

    tree->size++;
    return TREE_OK;
}

/* ��� ���� (������ �ڽ�)
 * - �Ű�����: parent - �θ� ���, data - ������ ������
 * - ��ȯ��: ���� �� TREE_OK, ���� �� ���� �ڵ�
 */
TreeResult tree_insert_right(BinaryTree* tree, TreeNode* parent, DataType data) {
    TreeNode* new_node = node_create(data);
    if (new_node == NULL) {
        return TREE_MEMORY_ERROR;
    }

    if (parent == NULL) {
        if (tree->root == NULL) {
            tree->root = new_node;
        }
        else {
            free(new_node);
            return TREE_MEMORY_ERROR;
        }
    }
    else {
        if (parent->right != NULL) {
            new_node->right = parent->right;
        }
        parent->right = new_node;
    }

    tree->size++;
    return TREE_OK;
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
void tree_destroy(BinaryTree* tree) {
    if (tree != NULL) {
        delete_subtree(tree->root);
        free(tree);
    }
}

/* �޴� ��� �Լ� */
void print_menu(void) {
    printf("\n=== Binary Tree Menu ===\n");
    printf("1. Insert left child\n");
    printf("2. Insert right child\n");
    printf("3. Print tree\n");
    printf("4. Tree traversals\n");
    printf("5. Get tree height\n");
    printf("6. Get number of nodes\n");
    printf("7. Clear tree\n");
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
    default:
        break;
    }
}

/* �׽�Ʈ�� ���� Ʈ�� ���� */
void create_sample_tree(BinaryTree* tree) {
    // ��Ʈ ��� ����
    tree_insert_left(tree, NULL, 1);
    TreeNode* root = tree->root;

    // ���� ����Ʈ��
    tree_insert_left(tree, root, 2);
    tree_insert_right(tree, root, 3);

    // 2�� �ڽĵ�
    tree_insert_left(tree, root->left, 4);
    tree_insert_right(tree, root->left, 5);

    // 3�� �ڽĵ�
    tree_insert_left(tree, root->right, 6);
    tree_insert_right(tree, root->right, 7);
}

int main(void) {
    BinaryTree* tree = tree_create();
    if (tree == NULL) {
        printf("Failed to create tree\n");
        return 1;
    }

    // ���� Ʈ�� ���� (�׽�Ʈ��)
    create_sample_tree(tree);
    printf("Sample binary tree created.\n");

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
        case 1:  // Insert left child
            printf("Enter value to insert: ");
            scanf("%d", &value);
            // Note: In a real implementation, you would need to specify the parent node
            printf("Note: This is a simplified version. Using root as parent.\n");
            result = tree_insert_left(tree, tree->root, value);
            if (result == TREE_OK) {
                printf("Successfully inserted %d as left child\n", value);
            }
            else {
                print_error(result);
            }
            break;

        case 2:  // Insert right child
            printf("Enter value to insert: ");
            scanf("%d", &value);
            // Note: In a real implementation, you would need to specify the parent node
            printf("Note: This is a simplified version. Using root as parent.\n");
            result = tree_insert_right(tree, tree->root, value);
            if (result == TREE_OK) {
                printf("Successfully inserted %d as right child\n", value);
            }
            else {
                print_error(result);
            }
            break;

        case 3:  // Print tree
            tree_print(tree);
            break;

        case 4:  // Tree traversals
            tree_preorder(tree);
            tree_inorder(tree);
            tree_postorder(tree);
            break;

        case 5:  // Get tree height
            printf("Tree height: %d\n", tree_height(tree));
            break;

        case 6:  // Get number of nodes
            printf("Number of nodes: %zu\n", tree_size(tree));
            break;

        case 7:  // Clear tree
            tree_destroy(tree);
            tree = tree_create();
            printf("Tree cleared\n");
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

1. ���� Ʈ���� �⺻ ����
--------------------
- �� ��尡 �ִ� 2���� �ڽ��� ����
- ���� �ڽİ� ������ �ڽ����� ����
- ����� �ڷᱸ��
- ������ ������ ǥ���� ����

2. ����ü ����
-----------
TreeNode:
- data: ������
- left: ���� �ڽ� ������
- right: ������ �ڽ� ������

BinaryTree:
- root: ��Ʈ ��� ������
- size: ��ü ��� ��

3. Ʈ�� ��ȸ
----------
���� ��ȸ (Preorder):
- Root -> Left -> Right
- Ʈ�� ���翡 ����

���� ��ȸ (Inorder):
- Left -> Root -> Right
- ���ĵ� ���� ���

���� ��ȸ (Postorder):
- Left -> Right -> Root
- �޸� ������ ����

4. �ֿ� ����� ���⵵
-----------------
����:
- O(1) (������ �θ� ���� ��)
- �θ� ��� ã��� ����

��ȸ:
- O(n) (��� ��� �湮)

���� ���:
- O(n) (��� ��� �湮)

ũ�� ���:
- O(1) (size �ʵ� �̿�)

5. �޸� ����
-----------
- ��� ���� ���� �Ҵ�
- ����� �޸� ����
- �޸� ���� ����
- ������ ����

6. ���� Ư¡
----------
- ����� �˰��� Ȱ��
- ������ ��� ����
- Ÿ�� �߻�ȭ (DataType)
- ���� ó��

7. �ð�ȭ ���
-----------
- Ʈ�� ���� ASCII ��Ʈ
- ���� ���� ǥ��
- ������ ���� ����
- ����� ����

8. ������ �������
--------------
- NULL ������ �˻�
- �޸� �Ҵ� ����
- ��� ���� ó��
- ��ȯ ���� ����

9. ������ ��ġ
-----------
- ��� ���� �н�
- ������ ����
- ���� �޸� ����
- Ʈ�� ���� ����

10. Ȱ�� �о�
----------
- ���� �ý���
- ���� �м�
- �˻� �˰���
- ���� Ʈ��

�� ������ ���������� ����ȭ�Ǿ� ������,
�⺻���� ���� Ʈ���� ��� �ٽ� ������
�����ϰ� �ֽ��ϴ�. ���� ������ ����
���ʰ� �Ǵ� ǥ������ ������ �����մϴ�.
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
�˰��� �з�: Ʈ�� �ڷᱸ��
���� �з�: �ڰ� ���� ���� Ž�� Ʈ��
���̵�: ��
���� �ڷᱸ��: AVL Ʈ��, B-Ʈ��

����-�� Ʈ�� Ư��:
1. ��� ���� ������ �Ǵ� ������
2. ��Ʈ�� ������
3. ��� ����(NIL)�� ������
4. ���� ����� �ڽ��� ��� ������
5. ������ ��忡�� ��� ���������� ���� ��� �� ����
*/

typedef enum { RED, BLACK } Color;

typedef struct Node {
    int key;
    Color color;
    struct Node* left, * right, * parent;
} Node;

typedef struct {
    Node* root;
    Node* NIL;  // �� ���� ���
} RBTree;

// Ʈ�� ����
RBTree* create_tree(void) {
    RBTree* tree = (RBTree*)malloc(sizeof(RBTree));
    tree->NIL = (Node*)malloc(sizeof(Node));
    tree->NIL->color = BLACK;
    tree->NIL->left = tree->NIL->right = tree->NIL->parent = NULL;
    tree->root = tree->NIL;
    return tree;
}

// ��� ����
Node* create_node(RBTree* tree, int key) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->key = key;
    node->color = RED;
    node->left = node->right = tree->NIL;
    node->parent = NULL;
    return node;
}

// ��ȸ��
void left_rotate(RBTree* tree, Node* x) {
    Node* y = x->right;
    x->right = y->left;

    if (y->left != tree->NIL)
        y->left->parent = x;

    y->parent = x->parent;

    if (x->parent == tree->NIL)
        tree->root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;

    y->left = x;
    x->parent = y;

    printf("��ȸ�� ���� (��� %d)\n", x->key);
}

// ��ȸ��
void right_rotate(RBTree* tree, Node* y) {
    Node* x = y->left;
    y->left = x->right;

    if (x->right != tree->NIL)
        x->right->parent = y;

    x->parent = y->parent;

    if (y->parent == tree->NIL)
        tree->root = x;
    else if (y == y->parent->right)
        y->parent->right = x;
    else
        y->parent->left = x;

    x->right = y;
    y->parent = x;

    printf("��ȸ�� ���� (��� %d)\n", y->key);
}

// ���� �� ����
void insert_fixup(RBTree* tree, Node* z) {
    while (z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            Node* y = z->parent->parent->right;

            // Case 1: ������ ������
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
                printf("Case 1 ���� (��� %d)\n", z->key);
            }
            else {
                // Case 2: ������ ������ (�ﰢ��)
                if (z == z->parent->right) {
                    z = z->parent;
                    left_rotate(tree, z);
                    printf("Case 2 ���� (��� %d)\n", z->key);
                }
                // Case 3: ������ ������ (����)
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                right_rotate(tree, z->parent->parent);
                printf("Case 3 ���� (��� %d)\n", z->key);
            }
        }
        else {
            Node* y = z->parent->parent->left;

            // Case 1: ������ ������
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
                printf("Case 1 ���� (��� %d)\n", z->key);
            }
            else {
                // Case 2: ������ ������ (�ﰢ��)
                if (z == z->parent->left) {
                    z = z->parent;
                    right_rotate(tree, z);
                    printf("Case 2 ���� (��� %d)\n", z->key);
                }
                // Case 3: ������ ������ (����)
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                left_rotate(tree, z->parent->parent);
                printf("Case 3 ���� (��� %d)\n", z->key);
            }
        }

        if (z == tree->root)
            break;
    }
    tree->root->color = BLACK;
}

// ��� ����
void insert(RBTree* tree, int key) {
    Node* z = create_node(tree, key);
    Node* y = tree->NIL;
    Node* x = tree->root;

    while (x != tree->NIL) {
        y = x;
        if (z->key < x->key)
            x = x->left;
        else
            x = x->right;
    }

    z->parent = y;

    if (y == tree->NIL)
        tree->root = z;
    else if (z->key < y->key)
        y->left = z;
    else
        y->right = z;

    printf("��� %d ����\n", key);
    insert_fixup(tree, z);
}

// Ʈ�� ���
void print_tree_recursive(RBTree* tree, Node* root, int level) {
    if (root == tree->NIL) return;

    print_tree_recursive(tree, root->right, level + 1);

    for (int i = 0; i < level; i++)
        printf("    ");
    printf("%d(%c)\n", root->key, root->color == RED ? 'R' : 'B');

    print_tree_recursive(tree, root->left, level + 1);
}

void print_tree(RBTree* tree) {
    printf("\n=== ���� Ʈ�� ���� ===\n");
    print_tree_recursive(tree, tree->root, 0);
    printf("\n");
}

// Ư�� ����
bool validate_property_4(Node* node, RBTree* tree) {
    if (node == tree->NIL) return true;

    if (node->color == RED) {
        if (node->left->color == RED || node->right->color == RED)
            return false;
    }

    return validate_property_4(node->left, tree) &&
        validate_property_4(node->right, tree);
}

int get_black_height(Node* node, RBTree* tree) {
    if (node == tree->NIL)
        return 0;

    int leftHeight = get_black_height(node->left, tree);
    int rightHeight = get_black_height(node->right, tree);

    if (leftHeight == -1 || rightHeight == -1 || leftHeight != rightHeight)
        return -1;

    return leftHeight + (node->color == BLACK ? 1 : 0);
}

void validate_tree(RBTree* tree) {
    printf("\n=== Ʈ�� Ư�� ���� ===\n");

    // Ư�� 2: ��Ʈ�� ������
    if (tree->root->color != BLACK) {
        printf("Ư�� 2 ����: ��Ʈ�� �������Դϴ�.\n");
        return;
    }

    // Ư�� 4: ���� ����� �ڽ��� ������
    if (!validate_property_4(tree->root, tree)) {
        printf("Ư�� 4 ����: ���� ����� �ڽ��� �������Դϴ�.\n");
        return;
    }

    // Ư�� 5: ���� ���� ����
    if (get_black_height(tree->root, tree) == -1) {
        printf("Ư�� 5 ����: ���� ���̰� �ٸ��ϴ�.\n");
        return;
    }

    printf("��� ����-�� Ʈ�� Ư���� �����˴ϴ�.\n");
}

// �޸� ����
void free_tree_recursive(RBTree* tree, Node* root) {
    if (root == tree->NIL) return;
    free_tree_recursive(tree, root->left);
    free_tree_recursive(tree, root->right);
    free(root);
}

void free_tree(RBTree* tree) {
    free_tree_recursive(tree, tree->root);
    free(tree->NIL);
    free(tree);
}

int main(void) {
    RBTree* tree = create_tree();
    int choice, key;

    printf("=== ����-�� Ʈ�� �׽�Ʈ ===\n");

    while (1) {
        printf("\n1. ��� ����\n");
        printf("2. Ʈ�� ���\n");
        printf("3. Ư�� ����\n");
        printf("0. ����\n");
        printf("����: ");

        scanf("%d", &choice);

        switch (choice) {
        case 1:
            printf("������ Ű ��: ");
            scanf("%d", &key);
            insert(tree, key);
            print_tree(tree);
            break;

        case 2:
            print_tree(tree);
            break;

        case 3:
            validate_tree(tree);
            break;

        case 0:
            free_tree(tree);
            return 0;

        default:
            printf("�߸��� ����\n");
        }
    }

    return 0;
}

/*
����-�� Ʈ�� �м�
===============

1. �ٽ� Ư��
----------
- ��� ���� R/B
- ��Ʈ�� ����(NIL)�� BLACK
- RED ����� �ڽ��� BLACK
- ��� ����� BLACK ��� �� ����

2. �ð� ���⵵
-----------
- ����: O(log n)
- ����: O(log n)
- �˻�: O(log n)

3. AVL Ʈ������ ����
----------------
����-�� Ʈ��:
- ������ ����
- ����/���� �� ȸ�� ����
- ���� ���뿡 �� ����

AVL Ʈ��:
- �Ϻ��� ����
- �� ���� ȸ�� �ʿ�
- �˻��� �� ����

4. ���� ����
---------
- C++ STL�� map/set
- Java TreeMap/TreeSet
- ���μ��� �����ٷ�
- �޸� ����

5. ���� Ư¡
---------
- NIL ��� ���
- �θ� ������ �ʿ�
- ���� ���� ����
- ����� Ư�� ����

�� ������ �ڰ� ���� Ʈ����
��� ���¸� �����ָ�,
���� �ý��ۿ��� ���� ���Ǵ�
�ڷᱸ���� ���ʸ� �����մϴ�.
*/
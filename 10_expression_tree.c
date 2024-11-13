#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

// �����ڿ� �ǿ����ڸ� �����ϱ� ���� ������
typedef enum {
    NODE_OPERATOR,
    NODE_OPERAND
} NodeType;

// ��� ����ü ����
typedef struct TreeNode {
    NodeType type;            // ��� Ÿ��(������/�ǿ�����)
    union {
        char operator;        // ������(+, -, *, /, %)
        double operand;       // �ǿ�����(����)
    } data;
    struct TreeNode* left;    // ���� �ڽ� ���
    struct TreeNode* right;   // ������ �ڽ� ���
} TreeNode;

// ���� Ʈ�� ����ü ����
typedef struct {
    TreeNode* root;          // ��Ʈ ���
} ExpressionTree;

// ���� ��� ����ü
typedef struct StackNode {
    TreeNode* data;
    struct StackNode* next;
} StackNode;

// ���� ����ü
typedef struct {
    StackNode* top;
} Stack;

/* ���� ���� */
Stack* create_stack(void) {
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    if (stack) {
        stack->top = NULL;
    }
    return stack;
}

/* ������ ����ִ��� Ȯ�� */
bool is_stack_empty(Stack* stack) {
    return (stack->top == NULL);
}

/* ���ÿ� ��� Ǫ�� */
void push(Stack* stack, TreeNode* node) {
    StackNode* new_node = (StackNode*)malloc(sizeof(StackNode));
    if (new_node) {
        new_node->data = node;
        new_node->next = stack->top;
        stack->top = new_node;
    }
}

/* ���ÿ��� ��� �� */
TreeNode* pop(Stack* stack) {
    if (is_stack_empty(stack)) {
        return NULL;
    }
    StackNode* temp = stack->top;
    TreeNode* node = temp->data;
    stack->top = temp->next;
    free(temp);
    return node;
}

/* ���� �޸� ���� */
void destroy_stack(Stack* stack) {
    while (!is_stack_empty(stack)) {
        pop(stack);
    }
    free(stack);
}

/* ������ ��� ���� */
TreeNode* create_operator_node(char op) {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    if (node) {
        node->type = NODE_OPERATOR;
        node->data.operator = op;
        node->left = NULL;
        node->right = NULL;
    }
    return node;
}

/* �ǿ����� ��� ���� */
TreeNode* create_operand_node(double value) {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    if (node) {
        node->type = NODE_OPERAND;
        node->data.operand = value;
        node->left = NULL;
        node->right = NULL;
    }
    return node;
}

/* �������� �켱���� ��ȯ */
int get_precedence(char op) {
    switch (op) {
    case '+':
    case '-':
        return 1;
    case '*':
    case '/':
        return 2;
    default:
        return 0;
    }
}

/* ���� ǥ������κ��� ���� Ʈ�� ���� */
TreeNode* build_tree_from_postfix(const char* postfix) {
    Stack* stack = create_stack();
    TreeNode* node, * op1, * op2;
    char* token;
    char expr[256];

    strcpy(expr, postfix);
    token = strtok(expr, " ");

    while (token != NULL) {
        // �������� ���
        if (strlen(token) == 1 && strchr("+-*/", token[0])) {
            node = create_operator_node(token[0]);
            op2 = pop(stack);
            op1 = pop(stack);
            node->left = op1;
            node->right = op2;
            push(stack, node);
        }
        // �ǿ������� ���
        else {
            node = create_operand_node(atof(token));
            push(stack, node);
        }
        token = strtok(NULL, " ");
    }

    node = pop(stack);
    destroy_stack(stack);
    return node;
}

/* ���� Ʈ�� ���� */
ExpressionTree* create_expression_tree(const char* postfix) {
    ExpressionTree* tree = (ExpressionTree*)malloc(sizeof(ExpressionTree));
    if (tree) {
        tree->root = build_tree_from_postfix(postfix);
    }
    return tree;
}

/* ���� ��ȸ (���� �Լ�) */
static void prefix_traversal(TreeNode* node) {
    if (node != NULL) {
        if (node->type == NODE_OPERATOR) {
            printf("%c ", node->data.operator);
        }
        else {
            printf("%.2f ", node->data.operand);
        }
        prefix_traversal(node->left);
        prefix_traversal(node->right);
    }
}

/* ���� ��ȸ (���� �Լ�) */
static void infix_traversal(TreeNode* node) {
    if (node != NULL) {
        if (node->type == NODE_OPERATOR) {
            printf("( ");
        }
        infix_traversal(node->left);
        if (node->type == NODE_OPERATOR) {
            printf("%c ", node->data.operator);
        }
        else {
            printf("%.2f ", node->data.operand);
        }
        infix_traversal(node->right);
        if (node->type == NODE_OPERATOR) {
            printf(") ");
        }
    }
}

/* ���� ��ȸ (���� �Լ�) */
static void postfix_traversal(TreeNode* node) {
    if (node != NULL) {
        postfix_traversal(node->left);
        postfix_traversal(node->right);
        if (node->type == NODE_OPERATOR) {
            printf("%c ", node->data.operator);
        }
        else {
            printf("%.2f ", node->data.operand);
        }
    }
}

/* ���� ǥ��� ��� */
void print_prefix(ExpressionTree* tree) {
    printf("Prefix notation: ");
    prefix_traversal(tree->root);
    printf("\n");
}

/* ���� ǥ��� ��� */
void print_infix(ExpressionTree* tree) {
    printf("Infix notation: ");
    infix_traversal(tree->root);
    printf("\n");
}

/* ���� ǥ��� ��� */
void print_postfix(ExpressionTree* tree) {
    printf("Postfix notation: ");
    postfix_traversal(tree->root);
    printf("\n");
}

/* ���� ��� (���� �Լ�) */
static double evaluate_recursive(TreeNode* node) {
    if (node == NULL) {
        return 0;
    }

    if (node->type == NODE_OPERAND) {
        return node->data.operand;
    }

    double left_val = evaluate_recursive(node->left);
    double right_val = evaluate_recursive(node->right);

    switch (node->data.operator) {
    case '+': return left_val + right_val;
    case '-': return left_val - right_val;
    case '*': return left_val * right_val;
    case '/': return right_val != 0 ? left_val / right_val : 0;
    default: return 0;
    }
}

/* ���� ��� (�ܺ� �������̽�) */
double evaluate_expression(ExpressionTree* tree) {
    return evaluate_recursive(tree->root);
}

/* Ʈ�� �ð�ȭ (���� �Լ�) */
static void print_tree_recursive(TreeNode* node, int level, char* prefix) {
    if (node == NULL) {
        return;
    }

    printf("%s", prefix);
    printf("%s", level ? "������ " : "");

    if (node->type == NODE_OPERATOR) {
        printf("%c\n", node->data.operator);
    }
    else {
        printf("%.2f\n", node->data.operand);
    }

    char new_prefix[256];
    sprintf(new_prefix, "%s%s", prefix, level ? "��   " : "");

    print_tree_recursive(node->left, 1, new_prefix);
    print_tree_recursive(node->right, 1, new_prefix);
}

/* Ʈ�� �ð�ȭ (�ܺ� �������̽�) */
void print_tree(ExpressionTree* tree) {
    printf("Expression Tree Structure:\n");
    print_tree_recursive(tree->root, 0, "");
}

/* Ʈ�� �޸� ���� (���� �Լ�) */
static void destroy_tree_recursive(TreeNode* node) {
    if (node != NULL) {
        destroy_tree_recursive(node->left);
        destroy_tree_recursive(node->right);
        free(node);
    }
}

/* Ʈ�� �޸� ���� (�ܺ� �������̽�) */
void destroy_expression_tree(ExpressionTree* tree) {
    if (tree != NULL) {
        destroy_tree_recursive(tree->root);
        free(tree);
    }
}

/* �޴� ��� */
void print_menu(void) {
    printf("\n=== Expression Tree Menu ===\n");
    printf("1. Create new expression tree\n");
    printf("2. Print prefix notation\n");
    printf("3. Print infix notation\n");
    printf("4. Print postfix notation\n");
    printf("5. Evaluate expression\n");
    printf("6. Print tree structure\n");
    printf("0. Exit\n");
    printf("Choice: ");
}

int main(void) {
    ExpressionTree* tree = NULL;
    int choice;
    char postfix[256];

    do {
        print_menu();
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');

        switch (choice) {
        case 1:
            printf("Enter postfix expression (space between tokens): ");
            fgets(postfix, sizeof(postfix), stdin);
            postfix[strcspn(postfix, "\n")] = 0;

            if (tree != NULL) {
                destroy_expression_tree(tree);
            }
            tree = create_expression_tree(postfix);
            printf("Expression tree created\n");
            break;

        case 2:
            if (tree != NULL) {
                print_prefix(tree);
            }
            else {
                printf("Tree is empty\n");
            }
            break;

        case 3:
            if (tree != NULL) {
                print_infix(tree);
            }
            else {
                printf("Tree is empty\n");
            }
            break;

        case 4:
            if (tree != NULL) {
                print_postfix(tree);
            }
            else {
                printf("Tree is empty\n");
            }
            break;

        case 5:
            if (tree != NULL) {
                printf("Result: %.2f\n", evaluate_expression(tree));
            }
            else {
                printf("Tree is empty\n");
            }
            break;

        case 6:
            if (tree != NULL) {
                print_tree(tree);
            }
            else {
                printf("Tree is empty\n");
            }
            break;

        case 0:
            printf("Exiting program\n");
            break;

        default:
            printf("Invalid choice\n");
        }
    } while (choice != 0);

    if (tree != NULL) {
        destroy_expression_tree(tree);
    }
    return 0;
}

/*
==========================================
�� ���� �� �ֿ� ����
==========================================

1. ���� Ʈ���� Ư¡
----------------
- ���� ���: ������
- ���� ���: �ǿ�����
- ���� ǥ������� ����
- Ʈ�� ��ȸ�� ǥ��� ��ȯ

2. ��� ����
----------
- ��� Ÿ�� ����
- Union Ȱ��
- ���� Ʈ�� ����
- ���� �Ҵ�

3. �ֿ� ���
----------
- ���� Ʈ�� ����
- �� ���� ǥ��� ��ȯ
- ���� ���
- Ʈ�� �ð�ȭ

4. ǥ��� ��ȯ
-----------
����(prefix):
- ��Ʈ-����-������
- �����ڰ� �տ� ��ġ

����(infix):
- ����-��Ʈ-������
- �Ϲ����� ���� ����

����(postfix):
- ����-������-��Ʈ
- ���� ��� ��

5. ���� Ȱ��
----------
- Ʈ�� ���� �� Ȱ��
- ���� ǥ��� ó��
- ���� �޸� ����
- LIFO ����

6. �޸� ����
-----------
- ��� ���� �Ҵ�
- ����� ����
- ���� �޸� ����
- �޸� ���� ����

7. ��ȸ �˰���
------------
���� ��ȸ:
- ������ �켱 ó��
- ���� ǥ��� ����

���� ��ȸ:
- ��ȣ ó�� �ʿ�
- �Ϲ� ���� ����

���� ��ȸ:
- �ǿ����� �켱 ó��
- ���� ǥ��� ����

8. ������ ��ġ
-----------
- Ʈ�� ���� ����
- ��� �˰���
- ǥ��� ��ȯ
- ���� Ȱ��

9. Ȱ�� �о�
----------
- �����Ϸ� ����
- ���� ����
- ���� �м�
- �ڵ� ����

10. ���� Ư¡
-----------
- ���ȭ�� ����
- ������ �޸� ����
- ����� �������̽�
- �ð��� ǥ��

�� ������ ���� Ʈ���� ǥ������ �����
��� �����ϸ�, ���� ��������
����ȭ�Ǿ� �ֽ��ϴ�.
*/
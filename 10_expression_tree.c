#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

// 연산자와 피연산자를 구분하기 위한 열거형
typedef enum {
    NODE_OPERATOR,
    NODE_OPERAND
} NodeType;

// 노드 구조체 정의
typedef struct TreeNode {
    NodeType type;            // 노드 타입(연산자/피연산자)
    union {
        char operator;        // 연산자(+, -, *, /, %)
        double operand;       // 피연산자(숫자)
    } data;
    struct TreeNode* left;    // 왼쪽 자식 노드
    struct TreeNode* right;   // 오른쪽 자식 노드
} TreeNode;

// 수식 트리 구조체 정의
typedef struct {
    TreeNode* root;          // 루트 노드
} ExpressionTree;

// 스택 노드 구조체
typedef struct StackNode {
    TreeNode* data;
    struct StackNode* next;
} StackNode;

// 스택 구조체
typedef struct {
    StackNode* top;
} Stack;

/* 스택 생성 */
Stack* create_stack(void) {
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    if (stack) {
        stack->top = NULL;
    }
    return stack;
}

/* 스택이 비어있는지 확인 */
bool is_stack_empty(Stack* stack) {
    return (stack->top == NULL);
}

/* 스택에 노드 푸시 */
void push(Stack* stack, TreeNode* node) {
    StackNode* new_node = (StackNode*)malloc(sizeof(StackNode));
    if (new_node) {
        new_node->data = node;
        new_node->next = stack->top;
        stack->top = new_node;
    }
}

/* 스택에서 노드 팝 */
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

/* 스택 메모리 해제 */
void destroy_stack(Stack* stack) {
    while (!is_stack_empty(stack)) {
        pop(stack);
    }
    free(stack);
}

/* 연산자 노드 생성 */
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

/* 피연산자 노드 생성 */
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

/* 연산자의 우선순위 반환 */
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

/* 후위 표기식으로부터 수식 트리 생성 */
TreeNode* build_tree_from_postfix(const char* postfix) {
    Stack* stack = create_stack();
    TreeNode* node, * op1, * op2;
    char* token;
    char expr[256];

    strcpy(expr, postfix);
    token = strtok(expr, " ");

    while (token != NULL) {
        // 연산자인 경우
        if (strlen(token) == 1 && strchr("+-*/", token[0])) {
            node = create_operator_node(token[0]);
            op2 = pop(stack);
            op1 = pop(stack);
            node->left = op1;
            node->right = op2;
            push(stack, node);
        }
        // 피연산자인 경우
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

/* 수식 트리 생성 */
ExpressionTree* create_expression_tree(const char* postfix) {
    ExpressionTree* tree = (ExpressionTree*)malloc(sizeof(ExpressionTree));
    if (tree) {
        tree->root = build_tree_from_postfix(postfix);
    }
    return tree;
}

/* 전위 순회 (내부 함수) */
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

/* 중위 순회 (내부 함수) */
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

/* 후위 순회 (내부 함수) */
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

/* 전위 표기식 출력 */
void print_prefix(ExpressionTree* tree) {
    printf("Prefix notation: ");
    prefix_traversal(tree->root);
    printf("\n");
}

/* 중위 표기식 출력 */
void print_infix(ExpressionTree* tree) {
    printf("Infix notation: ");
    infix_traversal(tree->root);
    printf("\n");
}

/* 후위 표기식 출력 */
void print_postfix(ExpressionTree* tree) {
    printf("Postfix notation: ");
    postfix_traversal(tree->root);
    printf("\n");
}

/* 수식 계산 (내부 함수) */
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

/* 수식 계산 (외부 인터페이스) */
double evaluate_expression(ExpressionTree* tree) {
    return evaluate_recursive(tree->root);
}

/* 트리 시각화 (내부 함수) */
static void print_tree_recursive(TreeNode* node, int level, char* prefix) {
    if (node == NULL) {
        return;
    }

    printf("%s", prefix);
    printf("%s", level ? "├── " : "");

    if (node->type == NODE_OPERATOR) {
        printf("%c\n", node->data.operator);
    }
    else {
        printf("%.2f\n", node->data.operand);
    }

    char new_prefix[256];
    sprintf(new_prefix, "%s%s", prefix, level ? "│   " : "");

    print_tree_recursive(node->left, 1, new_prefix);
    print_tree_recursive(node->right, 1, new_prefix);
}

/* 트리 시각화 (외부 인터페이스) */
void print_tree(ExpressionTree* tree) {
    printf("Expression Tree Structure:\n");
    print_tree_recursive(tree->root, 0, "");
}

/* 트리 메모리 해제 (내부 함수) */
static void destroy_tree_recursive(TreeNode* node) {
    if (node != NULL) {
        destroy_tree_recursive(node->left);
        destroy_tree_recursive(node->right);
        free(node);
    }
}

/* 트리 메모리 해제 (외부 인터페이스) */
void destroy_expression_tree(ExpressionTree* tree) {
    if (tree != NULL) {
        destroy_tree_recursive(tree->root);
        free(tree);
    }
}

/* 메뉴 출력 */
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
상세 설명 및 주요 개념
==========================================

1. 수식 트리의 특징
----------------
- 내부 노드: 연산자
- 리프 노드: 피연산자
- 후위 표기식으로 생성
- 트리 순회로 표기법 변환

2. 노드 구조
----------
- 노드 타입 구분
- Union 활용
- 이진 트리 구조
- 동적 할당

3. 주요 기능
----------
- 수식 트리 생성
- 세 가지 표기법 변환
- 수식 계산
- 트리 시각화

4. 표기법 변환
-----------
전위(prefix):
- 루트-왼쪽-오른쪽
- 연산자가 앞에 위치

중위(infix):
- 왼쪽-루트-오른쪽
- 일반적인 수식 형태

후위(postfix):
- 왼쪽-오른쪽-루트
- 스택 기반 평가

5. 스택 활용
----------
- 트리 생성 시 활용
- 후위 표기식 처리
- 동적 메모리 관리
- LIFO 구조

6. 메모리 관리
-----------
- 노드 단위 할당
- 재귀적 해제
- 스택 메모리 관리
- 메모리 누수 방지

7. 순회 알고리즘
------------
전위 순회:
- 연산자 우선 처리
- 전위 표기법 생성

중위 순회:
- 괄호 처리 필요
- 일반 수식 형태

후위 순회:
- 피연산자 우선 처리
- 후위 표기법 생성

8. 교육적 가치
-----------
- 트리 구조 이해
- 재귀 알고리즘
- 표기법 변환
- 스택 활용

9. 활용 분야
----------
- 컴파일러 설계
- 수식 계산기
- 구문 분석
- 코드 생성

10. 구현 특징
-----------
- 모듈화된 설계
- 안전한 메모리 관리
- 사용자 인터페이스
- 시각적 표현

이 구현은 수식 트리의 표준적인 기능을
모두 포함하며, 교육 목적으로
최적화되어 있습니다.
*/
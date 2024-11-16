#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef int DataType;

// 노드 구조체 정의
typedef struct TreeNode {
    DataType data;             // 데이터
    struct TreeNode* left;     // 왼쪽 자식 노드
    struct TreeNode* right;    // 오른쪽 자식 노드
} TreeNode;

// 이진 트리 구조체 정의
typedef struct {
    TreeNode* root;    // 루트 노드
    size_t size;       // 전체 노드의 수
} BinaryTree;

/* 오류 처리를 위한 열거형 정의 */
typedef enum {
    TREE_OK,
    TREE_MEMORY_ERROR,
    TREE_EMPTY
} TreeResult;

/* 새로운 노드 생성
 * - 매개변수: data - 저장할 데이터
 * - 반환값: 생성된 노드의 포인터 또는 실패 시 NULL
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

/* 트리 생성
 * - 새로운 빈 이진 트리를 동적으로 할당하고 초기화
 * - 반환값: 생성된 트리의 포인터 또는 실패 시 NULL
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

/* 트리가 비어있는지 확인
 * - 매개변수: tree - 검사할 트리
 * - 반환값: 비어있으면 true, 아니면 false
 */
bool tree_is_empty(const BinaryTree* tree) {
    return (tree->root == NULL);
}

/* 전위 순회 (내부 함수)
 * - Root -> Left -> Right 순서로 순회
 */
static void preorder_traversal(TreeNode* node) {
    if (node != NULL) {
        printf("%d ", node->data);      // 현재 노드 처리
        preorder_traversal(node->left);  // 왼쪽 서브트리 순회
        preorder_traversal(node->right); // 오른쪽 서브트리 순회
    }
}

/* 중위 순회 (내부 함수)
 * - Left -> Root -> Right 순서로 순회
 */
static void inorder_traversal(TreeNode* node) {
    if (node != NULL) {
        inorder_traversal(node->left);   // 왼쪽 서브트리 순회
        printf("%d ", node->data);       // 현재 노드 처리
        inorder_traversal(node->right);  // 오른쪽 서브트리 순회
    }
}

/* 후위 순회 (내부 함수)
 * - Left -> Right -> Root 순서로 순회
 */
static void postorder_traversal(TreeNode* node) {
    if (node != NULL) {
        postorder_traversal(node->left);  // 왼쪽 서브트리 순회
        postorder_traversal(node->right); // 오른쪽 서브트리 순회
        printf("%d ", node->data);        // 현재 노드 처리
    }
}

/* 전위 순회 (외부 인터페이스)
 * - 매개변수: tree - 순회할 트리
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

/* 중위 순회 (외부 인터페이스)
 * - 매개변수: tree - 순회할 트리
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

/* 후위 순회 (외부 인터페이스)
 * - 매개변수: tree - 순회할 트리
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

/* 노드의 높이 계산 (내부 함수)
 * - 매개변수: node - 높이를 계산할 노드
 * - 반환값: 해당 노드를 루트로 하는 서브트리의 높이
 */
static int calculate_height(const TreeNode* node) {
    if (node == NULL) {
        return -1;
    }
    int left_height = calculate_height(node->left);
    int right_height = calculate_height(node->right);
    return (left_height > right_height ? left_height : right_height) + 1;
}

/* 트리의 높이 반환 (외부 인터페이스)
 * - 매개변수: tree - 높이를 계산할 트리
 * - 반환값: 트리의 높이
 */
int tree_height(const BinaryTree* tree) {
    if (tree_is_empty(tree)) {
        return -1;
    }
    return calculate_height(tree->root);
}

/* 노드의 개수 계산 (내부 함수)
 * - 매개변수: node - 개수를 계산할 노드
 * - 반환값: 해당 노드를 루트로 하는 서브트리의 노드 개수
 */
static size_t count_nodes(const TreeNode* node) {
    if (node == NULL) {
        return 0;
    }
    return 1 + count_nodes(node->left) + count_nodes(node->right);
}

/* 트리의 노드 개수 반환 (외부 인터페이스)
 * - 매개변수: tree - 노드 개수를 계산할 트리
 * - 반환값: 트리의 전체 노드 개수
 */
size_t tree_size(const BinaryTree* tree) {
    return tree->size;
}

/* 트리 시각화를 위한 내부 함수
 * - 매개변수: node - 출력할 노드, level - 현재 레벨, prefix - 출력 접두사
 */
static void print_tree_recursive(const TreeNode* node, int level, char* prefix) {
    if (node == NULL) {
        return;
    }

    printf("%s", prefix);
    printf("%s", level ? "├── " : "");
    printf("%d\n", node->data);

    char new_prefix[256];
    sprintf(new_prefix, "%s%s", prefix, level ? "│   " : "");

    if (node->left || node->right) {
        print_tree_recursive(node->left, 1, new_prefix);
        print_tree_recursive(node->right, 1, new_prefix);
    }
}

/* 트리 시각화 (외부 인터페이스)
 * - 매개변수: tree - 출력할 트리
 */
void tree_print(const BinaryTree* tree) {
    if (tree_is_empty(tree)) {
        printf("Tree is empty\n");
        return;
    }
    printf("Binary Tree Structure:\n");
    print_tree_recursive(tree->root, 0, "");
}

/* 노드 삽입 (왼쪽 자식)
 * - 매개변수: parent - 부모 노드, data - 삽입할 데이터
 * - 반환값: 성공 시 TREE_OK, 실패 시 에러 코드
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

/* 노드 삽입 (오른쪽 자식)
 * - 매개변수: parent - 부모 노드, data - 삽입할 데이터
 * - 반환값: 성공 시 TREE_OK, 실패 시 에러 코드
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

/* 서브트리 삭제 (내부 함수)
 * - 매개변수: node - 삭제할 서브트리의 루트 노드
 */
static void delete_subtree(TreeNode* node) {
    if (node == NULL) {
        return;
    }
    delete_subtree(node->left);
    delete_subtree(node->right);
    free(node);
}

/* 트리 메모리 해제
 * - 매개변수: tree - 해제할 트리
 */
void tree_destroy(BinaryTree* tree) {
    if (tree != NULL) {
        delete_subtree(tree->root);
        free(tree);
    }
}

/* 메뉴 출력 함수 */
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

/* 에러 메시지 출력 함수 */
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

/* 테스트용 샘플 트리 생성 */
void create_sample_tree(BinaryTree* tree) {
    // 루트 노드 생성
    tree_insert_left(tree, NULL, 1);
    TreeNode* root = tree->root;

    // 왼쪽 서브트리
    tree_insert_left(tree, root, 2);
    tree_insert_right(tree, root, 3);

    // 2의 자식들
    tree_insert_left(tree, root->left, 4);
    tree_insert_right(tree, root->left, 5);

    // 3의 자식들
    tree_insert_left(tree, root->right, 6);
    tree_insert_right(tree, root->right, 7);
}

int main(void) {
    BinaryTree* tree = tree_create();
    if (tree == NULL) {
        printf("Failed to create tree\n");
        return 1;
    }

    // 샘플 트리 생성 (테스트용)
    create_sample_tree(tree);
    printf("Sample binary tree created.\n");

    int choice;
    DataType value;
    TreeResult result;

    do {
        print_menu();
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');  // 입력 버퍼 비우기
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
상세 설명 및 주요 개념
==========================================

1. 이진 트리의 기본 개념
--------------------
- 각 노드가 최대 2개의 자식을 가짐
- 왼쪽 자식과 오른쪽 자식으로 구분
- 재귀적 자료구조
- 계층적 데이터 표현에 적합

2. 구조체 설계
-----------
TreeNode:
- data: 데이터
- left: 왼쪽 자식 포인터
- right: 오른쪽 자식 포인터

BinaryTree:
- root: 루트 노드 포인터
- size: 전체 노드 수

3. 트리 순회
----------
전위 순회 (Preorder):
- Root -> Left -> Right
- 트리 복사에 유용

중위 순회 (Inorder):
- Left -> Root -> Right
- 정렬된 순서 출력

후위 순회 (Postorder):
- Left -> Right -> Root
- 메모리 해제에 유용

4. 주요 연산과 복잡도
-----------------
삽입:
- O(1) (지정된 부모에 삽입 시)
- 부모 노드 찾기는 별도

순회:
- O(n) (모든 노드 방문)

높이 계산:
- O(n) (모든 노드 방문)

크기 계산:
- O(1) (size 필드 이용)

5. 메모리 관리
-----------
- 노드 단위 동적 할당
- 재귀적 메모리 해제
- 메모리 누수 방지
- 포인터 관리

6. 구현 특징
----------
- 재귀적 알고리즘 활용
- 포인터 기반 구조
- 타입 추상화 (DataType)
- 에러 처리

7. 시각화 기능
-----------
- 트리 구조 ASCII 아트
- 계층 구조 표현
- 직관적 이해 도움
- 디버깅 용이

8. 안전성 고려사항
--------------
- NULL 포인터 검사
- 메모리 할당 검증
- 경계 조건 처리
- 순환 참조 방지

9. 교육적 가치
-----------
- 재귀 개념 학습
- 포인터 이해
- 동적 메모리 관리
- 트리 구조 이해

10. 활용 분야
----------
- 파일 시스템
- 구문 분석
- 검색 알고리즘
- 결정 트리

이 구현은 교육용으로 최적화되어 있으며,
기본적인 이진 트리의 모든 핵심 개념을
포함하고 있습니다. 실제 응용을 위한
기초가 되는 표준적인 구현을 제공합니다.
*/
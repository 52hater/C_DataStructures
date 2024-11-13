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

// 이진 탐색 트리 구조체 정의
typedef struct {
    TreeNode* root;    // 루트 노드
    size_t size;       // 전체 노드의 수
} BSTree;

/* 오류 처리를 위한 열거형 정의 */
typedef enum {
    TREE_OK,
    TREE_MEMORY_ERROR,
    TREE_EMPTY,
    TREE_NOT_FOUND
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
 * - 새로운 빈 이진 탐색 트리를 동적으로 할당하고 초기화
 * - 반환값: 생성된 트리의 포인터 또는 실패 시 NULL
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

/* 트리가 비어있는지 확인
 * - 매개변수: tree - 검사할 트리
 * - 반환값: 비어있으면 true, 아니면 false
 */
bool tree_is_empty(const BSTree* tree) {
    return (tree->root == NULL);
}

/* 재귀적 노드 삽입 (내부 함수)
 * - 매개변수: node - 현재 노드, data - 삽입할 데이터
 * - 반환값: 삽입 후의 (서브)트리 루트 노드
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

/* 노드 삽입 (외부 인터페이스)
 * - 매개변수: tree - 대상 트리, data - 삽입할 데이터
 * - 반환값: 성공 시 TREE_OK, 실패 시 에러 코드
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

/* 최소값을 가진 노드 찾기 (내부 함수)
 * - 매개변수: node - 시작 노드
 * - 반환값: 최소값을 가진 노드의 포인터
 */
static TreeNode* find_min_node(TreeNode* node) {
    TreeNode* current = node;
    while (current && current->left != NULL) {
        current = current->left;
    }
    return current;
}

/* 노드 삭제 (내부 함수)
 * - 매개변수: node - 현재 노드, data - 삭제할 데이터
 * - 반환값: 삭제 후의 (서브)트리 루트 노드
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
        // 경우 1: 리프 노드
        if (node->left == NULL && node->right == NULL) {
            free(node);
            return NULL;
        }
        // 경우 2: 한쪽 자식만 있는 경우
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
        // 경우 3: 두 자식이 모두 있는 경우
        TreeNode* temp = find_min_node(node->right);
        node->data = temp->data;
        node->right = delete_recursive(node->right, temp->data, success);
    }
    return node;
}

/* 노드 삭제 (외부 인터페이스)
 * - 매개변수: tree - 대상 트리, data - 삭제할 데이터
 * - 반환값: 성공 시 TREE_OK, 실패 시 에러 코드
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

/* 노드 검색 (내부 함수)
 * - 매개변수: node - 현재 노드, data - 검색할 데이터
 * - 반환값: 찾은 노드의 포인터 또는 NULL
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

/* 노드 검색 (외부 인터페이스)
 * - 매개변수: tree - 대상 트리, data - 검색할 데이터
 * - 반환값: 성공 시 true, 실패 시 false
 */
bool tree_search(const BSTree* tree, DataType data) {
    return search_recursive(tree->root, data) != NULL;
}

/* 최소값 찾기
 * - 매개변수: tree - 대상 트리, value - 결과를 저장할 포인터
 * - 반환값: 성공 시 TREE_OK, 실패 시 에러 코드
 */
TreeResult tree_find_min(const BSTree* tree, DataType* value) {
    if (tree_is_empty(tree)) {
        return TREE_EMPTY;
    }

    TreeNode* min_node = find_min_node(tree->root);
    *value = min_node->data;
    return TREE_OK;
}

/* 최대값 찾기 (내부 함수)
 * - 매개변수: node - 시작 노드
 * - 반환값: 최대값을 가진 노드의 포인터
 */
static TreeNode* find_max_node(TreeNode* node) {
    TreeNode* current = node;
    while (current && current->right != NULL) {
        current = current->right;
    }
    return current;
}

/* 최대값 찾기 (외부 인터페이스)
 * - 매개변수: tree - 대상 트리, value - 결과를 저장할 포인터
 * - 반환값: 성공 시 TREE_OK, 실패 시 에러 코드
 */
TreeResult tree_find_max(const BSTree* tree, DataType* value) {
    if (tree_is_empty(tree)) {
        return TREE_EMPTY;
    }

    TreeNode* max_node = find_max_node(tree->root);
    *value = max_node->data;
    return TREE_OK;
}

/* 중위 순회 (내부 함수)
 * - 매개변수: node - 현재 노드
 */
static void inorder_traversal(TreeNode* node) {
    if (node != NULL) {
        inorder_traversal(node->left);
        printf("%d ", node->data);
        inorder_traversal(node->right);
    }
}

/* 중위 순회 (외부 인터페이스)
 * - 매개변수: tree - 순회할 트리
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

    print_tree_recursive(node->left, 1, new_prefix);
    print_tree_recursive(node->right, 1, new_prefix);
}

/* 트리 시각화 (외부 인터페이스)
 * - 매개변수: tree - 출력할 트리
 */
void tree_print(const BSTree* tree) {
    if (tree_is_empty(tree)) {
        printf("Tree is empty\n");
        return;
    }
    printf("Binary Search Tree Structure:\n");
    print_tree_recursive(tree->root, 0, "");
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
void tree_destroy(BSTree* tree) {
    if (tree != NULL) {
        delete_subtree(tree->root);
        free(tree);
    }
}

/* 트리의 크기 반환
 * - 매개변수: tree - 대상 트리
 * - 반환값: 트리의 노드 개수
 */
size_t tree_size(const BSTree* tree) {
    return tree->size;
}

/* 메뉴 출력 함수 */
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

/* 에러 메시지 출력 함수 */
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
            while (getchar() != '\n');  // 입력 버퍼 비우기
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
상세 설명 및 주요 개념
==========================================

1. 이진 탐색 트리의 특성
--------------------
- 왼쪽 서브트리의 모든 노드 < 현재 노드
- 오른쪽 서브트리의 모든 노드 > 현재 노드
- 중위 순회시 정렬된 결과
- 효율적인 검색 구조

2. 주요 연산과 시간 복잡도
---------------------
삽입/삭제/검색:
- 평균: O(log n)
- 최악: O(n) (불균형 트리)

최소/최대값 찾기:
- O(h) (h는 트리의 높이)
- 한쪽 방향으로만 이동

3. 노드 삭제의 세 가지 경우
----------------------
Case 1: 리프 노드
- 단순히 노드 제거

Case 2: 한 자식
- 자식을 부모에 직접 연결

Case 3: 두 자식
- 후계자(successor) 찾기
- 중위 후속자로 대체

4. 구현 특징
----------
- 재귀적 구현
- 포인터 기반 구조
- 동적 메모리 관리
- 모듈화된 설계

5. 메모리 관리 전략
---------------
- 노드 단위 할당/해제
- 재귀적 트리 소멸
- 메모리 누수 방지
- 안전한 포인터 처리

6. 순회와 출력
-----------
- 중위 순회
  * 정렬된 순서 출력
  * 이진 탐색 트리 검증

- 시각적 출력
  * 트리 구조 표현
  * 디버깅 용이
  * 학습 도구

7. 에러 처리
----------
- 메모리 할당 실패
- 빈 트리 처리
- 요소 미발견
- 잘못된 입력

8. 응용 분야
----------
- 데이터베이스 인덱싱
- 심볼 테이블
- 우선순위 큐
- 정렬 알고리즘

9. 최적화 고려사항
--------------
- 트리 균형 유지
- 중복 키 처리
- 재귀 깊이 제한
- 캐시 효율성

10. 교육적 가치
------------
- 재귀 개념 학습
- 포인터 조작
- 탐색 알고리즘
- 트리 구조 이해

이 구현은 교육용으로 최적화되어 있으며,
이진 탐색 트리의 모든 핵심 개념을
다루고 있습니다. 실제 응용을 위한
기초가 되는 표준적인 구현을 제공합니다.
*/
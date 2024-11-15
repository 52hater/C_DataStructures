#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
알고리즘 분류: 트리 자료구조
하위 분류: 자가 균형 이진 탐색 트리
난이도: 상
관련 자료구조: AVL 트리, B-트리

레드-블랙 트리 특성:
1. 모든 노드는 빨간색 또는 검은색
2. 루트는 검은색
3. 모든 리프(NIL)는 검은색
4. 빨간 노드의 자식은 모두 검은색
5. 임의의 노드에서 모든 리프까지의 검은 노드 수 동일
*/

typedef enum { RED, BLACK } Color;

typedef struct Node {
    int key;
    Color color;
    struct Node* left, * right, * parent;
} Node;

typedef struct {
    Node* root;
    Node* NIL;  // 널 리프 노드
} RBTree;

// 트리 생성
RBTree* create_tree(void) {
    RBTree* tree = (RBTree*)malloc(sizeof(RBTree));
    tree->NIL = (Node*)malloc(sizeof(Node));
    tree->NIL->color = BLACK;
    tree->NIL->left = tree->NIL->right = tree->NIL->parent = NULL;
    tree->root = tree->NIL;
    return tree;
}

// 노드 생성
Node* create_node(RBTree* tree, int key) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->key = key;
    node->color = RED;
    node->left = node->right = tree->NIL;
    node->parent = NULL;
    return node;
}

// 좌회전
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

    printf("좌회전 수행 (노드 %d)\n", x->key);
}

// 우회전
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

    printf("우회전 수행 (노드 %d)\n", y->key);
}

// 삽입 후 조정
void insert_fixup(RBTree* tree, Node* z) {
    while (z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            Node* y = z->parent->parent->right;

            // Case 1: 삼촌이 빨간색
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
                printf("Case 1 적용 (노드 %d)\n", z->key);
            }
            else {
                // Case 2: 삼촌이 검은색 (삼각형)
                if (z == z->parent->right) {
                    z = z->parent;
                    left_rotate(tree, z);
                    printf("Case 2 적용 (노드 %d)\n", z->key);
                }
                // Case 3: 삼촌이 검은색 (직선)
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                right_rotate(tree, z->parent->parent);
                printf("Case 3 적용 (노드 %d)\n", z->key);
            }
        }
        else {
            Node* y = z->parent->parent->left;

            // Case 1: 삼촌이 빨간색
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
                printf("Case 1 적용 (노드 %d)\n", z->key);
            }
            else {
                // Case 2: 삼촌이 검은색 (삼각형)
                if (z == z->parent->left) {
                    z = z->parent;
                    right_rotate(tree, z);
                    printf("Case 2 적용 (노드 %d)\n", z->key);
                }
                // Case 3: 삼촌이 검은색 (직선)
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                left_rotate(tree, z->parent->parent);
                printf("Case 3 적용 (노드 %d)\n", z->key);
            }
        }

        if (z == tree->root)
            break;
    }
    tree->root->color = BLACK;
}

// 노드 삽입
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

    printf("노드 %d 삽입\n", key);
    insert_fixup(tree, z);
}

// 트리 출력
void print_tree_recursive(RBTree* tree, Node* root, int level) {
    if (root == tree->NIL) return;

    print_tree_recursive(tree, root->right, level + 1);

    for (int i = 0; i < level; i++)
        printf("    ");
    printf("%d(%c)\n", root->key, root->color == RED ? 'R' : 'B');

    print_tree_recursive(tree, root->left, level + 1);
}

void print_tree(RBTree* tree) {
    printf("\n=== 현재 트리 상태 ===\n");
    print_tree_recursive(tree, tree->root, 0);
    printf("\n");
}

// 특성 검증
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
    printf("\n=== 트리 특성 검증 ===\n");

    // 특성 2: 루트는 검은색
    if (tree->root->color != BLACK) {
        printf("특성 2 위반: 루트가 빨간색입니다.\n");
        return;
    }

    // 특성 4: 빨간 노드의 자식은 검은색
    if (!validate_property_4(tree->root, tree)) {
        printf("특성 4 위반: 빨간 노드의 자식이 빨간색입니다.\n");
        return;
    }

    // 특성 5: 검은 높이 동일
    if (get_black_height(tree->root, tree) == -1) {
        printf("특성 5 위반: 검은 높이가 다릅니다.\n");
        return;
    }

    printf("모든 레드-블랙 트리 특성이 만족됩니다.\n");
}

// 메모리 해제
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

    printf("=== 레드-블랙 트리 테스트 ===\n");

    while (1) {
        printf("\n1. 노드 삽입\n");
        printf("2. 트리 출력\n");
        printf("3. 특성 검증\n");
        printf("0. 종료\n");
        printf("선택: ");

        scanf("%d", &choice);

        switch (choice) {
        case 1:
            printf("삽입할 키 값: ");
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
            printf("잘못된 선택\n");
        }
    }

    return 0;
}

/*
레드-블랙 트리 분석
===============

1. 핵심 특성
----------
- 모든 노드는 R/B
- 루트와 리프(NIL)는 BLACK
- RED 노드의 자식은 BLACK
- 모든 경로의 BLACK 노드 수 동일

2. 시간 복잡도
-----------
- 삽입: O(log n)
- 삭제: O(log n)
- 검색: O(log n)

3. AVL 트리와의 차이
----------------
레드-블랙 트리:
- 느슨한 균형
- 삽입/삭제 시 회전 적음
- 실제 응용에 더 적합

AVL 트리:
- 완벽한 균형
- 더 많은 회전 필요
- 검색이 더 빠름

4. 실제 응용
---------
- C++ STL의 map/set
- Java TreeMap/TreeSet
- 프로세스 스케줄러
- 메모리 관리

5. 구현 특징
---------
- NIL 노드 사용
- 부모 포인터 필요
- 색상 정보 저장
- 재귀적 특성 검증

이 구현은 자가 균형 트리의
고급 형태를 보여주며,
실제 시스템에서 많이 사용되는
자료구조의 기초를 제공합니다.
*/
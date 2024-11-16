#include <stdio.h>
#include <stdlib.h>

/*
AVL 트리:
- 자가 균형 이진 탐색 트리
- 모든 노드의 왼쪽/오른쪽 서브트리 높이 차이가 최대 1
- 삽입/삭제 시 자동 재균형화
*/

typedef struct Node {
    int key;
    int height;
    struct Node* left;
    struct Node* right;
} Node;

// 노드 높이 반환
int get_height(Node* node) {
    return node ? node->height : -1;
}

// 균형 인수 계산
int get_balance(Node* node) {
    return node ? get_height(node->left) - get_height(node->right) : 0;
}

// 노드 높이 업데이트
void update_height(Node* node) {
    node->height = 1 + (get_height(node->left) > get_height(node->right) ?
        get_height(node->left) : get_height(node->right));
}

// 우회전
Node* rotate_right(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

    x->right = y;
    y->left = T2;

    update_height(y);
    update_height(x);

    return x;
}

// 좌회전
Node* rotate_left(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    y->left = x;
    x->right = T2;

    update_height(x);
    update_height(y);

    return y;
}

// 새 노드 생성
Node* create_node(int key) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->key = key;
    node->height = 0;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// 노드 삽입
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
        return node;  // 중복 키 허용하지 않음

    update_height(node);
    int balance = get_balance(node);

    printf("\n삽입 후 상태 (키: %d):\n", key);
    printf("노드: %d, 균형: %d\n", node->key, balance);

    // LL Case
    if (balance > 1 && key < node->left->key) {
        printf("LL 회전 수행 (노드 %d)\n", node->key);
        return rotate_right(node);
    }

    // RR Case
    if (balance < -1 && key > node->right->key) {
        printf("RR 회전 수행 (노드 %d)\n", node->key);
        return rotate_left(node);
    }

    // LR Case
    if (balance > 1 && key > node->left->key) {
        printf("LR 회전 수행 (노드 %d)\n", node->key);
        node->left = rotate_left(node->left);
        return rotate_right(node);
    }

    // RL Case
    if (balance < -1 && key < node->right->key) {
        printf("RL 회전 수행 (노드 %d)\n", node->key);
        node->right = rotate_right(node->right);
        return rotate_left(node);
    }

    return node;
}

// 최소값 노드 찾기
Node* find_min(Node* node) {
    return node->left ? find_min(node->left) : node;
}

// 노드 삭제
Node* delete(Node* root, int key, bool* height_changed) {
    if (!root) return NULL;

    if (key < root->key)
        root->left = delete(root->left, key, height_changed);
    else if (key > root->key)
        root->right = delete(root->right, key, height_changed);
    else {
        // 삭제할 노드 찾음
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

    printf("\n삭제 후 상태 (키: %d):\n", key);
    printf("노드: %d, 균형: %d\n", root->key, balance);

    // 재균형화
    if (balance > 1) {
        if (get_balance(root->left) >= 0) {
            printf("LL 회전 수행 (노드 %d)\n", root->key);
            return rotate_right(root);
        }
        else {
            printf("LR 회전 수행 (노드 %d)\n", root->key);
            root->left = rotate_left(root->left);
            return rotate_right(root);
        }
    }

    if (balance < -1) {
        if (get_balance(root->right) <= 0) {
            printf("RR 회전 수행 (노드 %d)\n", root->key);
            return rotate_left(root);
        }
        else {
            printf("RL 회전 수행 (노드 %d)\n", root->key);
            root->right = rotate_right(root->right);
            return rotate_left(root);
        }
    }

    return root;
}

// 트리 시각화
void print_tree(Node* root, int level) {
    if (!root) return;

    print_tree(root->right, level + 1);

    for (int i = 0; i < level; i++)
        printf("    ");
    printf("%d\n", root->key);

    print_tree(root->left, level + 1);
}

// 트리 상태 출력
void print_tree_state(Node* root) {
    printf("\n=== 현재 트리 상태 ===\n");
    print_tree(root, 0);
    printf("\n");
}

// 메모리 해제
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

    printf("=== AVL 트리 테스트 ===\n");
    printf("1: 삽입\n");
    printf("2: 삭제\n");
    printf("3: 트리 출력\n");
    printf("0: 종료\n");

    while (1) {
        int choice, value;
        printf("\n선택: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            printf("삽입할 값: ");
            scanf("%d", &value);
            root = insert(root, value, &height_changed);
            print_tree_state(root);
            break;

        case 2:
            printf("삭제할 값: ");
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
            printf("잘못된 선택\n");
        }
    }

    return 0;
}

/*
AVL 트리 분석
===========

1. 시간 복잡도
-----------
- 삽입: O(log n)
- 삭제: O(log n)
- 검색: O(log n)

2. 공간 복잡도
-----------
- O(n) 저장 공간
- O(log n) 재귀 스택

3. 균형화 전략
-----------
- 단일 회전: LL, RR
- 이중 회전: LR, RL
- 높이 차이 최대 1 유지

4. 활용 분야
---------
- 데이터베이스 인덱싱
- 메모리 관리
- 실시간 시스템

이 구현은 자가 균형 트리의
기본 원리와 실제 응용을
보여줍니다.
*/
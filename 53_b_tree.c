#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
B-트리:
- 자가 균형 다진 탐색 트리
- 한 노드가 여러 키를 가짐
- 모든 리프 노드가 같은 레벨
- 디스크 기반 자료구조의 기초
*/

#define MAX_KEYS 3  // 최대 키 개수 (차수 = 4)
#define MIN_KEYS MAX_KEYS/2  // 최소 키 개수

typedef struct BTreeNode {
    int keys[MAX_KEYS];         // 키 배열
    struct BTreeNode* children[MAX_KEYS + 1];  // 자식 포인터 배열
    int num_keys;               // 현재 키 개수
    bool is_leaf;              // 리프 노드 여부
} BTreeNode;

// 새 노드 생성
BTreeNode* create_node(bool is_leaf) {
    BTreeNode* node = (BTreeNode*)malloc(sizeof(BTreeNode));
    node->num_keys = 0;
    node->is_leaf = is_leaf;

    for (int i = 0; i <= MAX_KEYS; i++) {
        node->children[i] = NULL;
    }

    return node;
}

// 노드 분할
void split_child(BTreeNode* parent, int index, BTreeNode* child) {
    BTreeNode* new_node = create_node(child->is_leaf);

    // 새 노드로 키 이동
    for (int j = 0; j < MIN_KEYS; j++) {
        new_node->keys[j] = child->keys[j + MIN_KEYS + 1];
    }

    // 자식 포인터 이동 (리프가 아닌 경우)
    if (!child->is_leaf) {
        for (int j = 0; j <= MIN_KEYS; j++) {
            new_node->children[j] = child->children[j + MIN_KEYS + 1];
        }
    }

    new_node->num_keys = MIN_KEYS;
    child->num_keys = MIN_KEYS;

    // 부모 노드 조정
    for (int j = parent->num_keys; j >= index + 1; j--) {
        parent->children[j + 1] = parent->children[j];
    }

    parent->children[index + 1] = new_node;

    for (int j = parent->num_keys - 1; j >= index; j--) {
        parent->keys[j + 1] = parent->keys[j];
    }

    parent->keys[index] = child->keys[MIN_KEYS];
    parent->num_keys++;

    printf("\n노드 분할 발생:\n");
    printf("중간 키 %d를 부모로 이동\n", child->keys[MIN_KEYS]);
}

// 키 삽입 (비재귀)
void insert_non_full(BTreeNode* node, int key) {
    int i = node->num_keys - 1;

    if (node->is_leaf) {
        while (i >= 0 && node->keys[i] > key) {
            node->keys[i + 1] = node->keys[i];
            i--;
        }

        node->keys[i + 1] = key;
        node->num_keys++;
        printf("키 %d를 리프 노드에 삽입\n", key);
    }
    else {
        while (i >= 0 && node->keys[i] > key) {
            i--;
        }
        i++;

        if (node->children[i]->num_keys == MAX_KEYS) {
            split_child(node, i, node->children[i]);

            if (key > node->keys[i]) {
                i++;
            }
        }
        insert_non_full(node->children[i], key);
    }
}

// 트리에 키 삽입
BTreeNode* insert(BTreeNode* root, int key) {
    printf("\n키 %d 삽입 시작\n", key);

    // 루트가 가득 찬 경우
    if (root->num_keys == MAX_KEYS) {
        BTreeNode* new_root = create_node(false);
        new_root->children[0] = root;
        split_child(new_root, 0, root);

        int i = 0;
        if (new_root->keys[0] < key) {
            i++;
        }
        insert_non_full(new_root->children[i], key);

        printf("새로운 루트 생성\n");
        return new_root;
    }

    insert_non_full(root, key);
    return root;
}

// 트리 출력
void print_tree(BTreeNode* root, int level) {
    if (!root) return;

    printf("Level %d: ", level);
    for (int i = 0; i < root->num_keys; i++) {
        printf("%d ", root->keys[i]);
    }
    printf("\n");

    if (!root->is_leaf) {
        for (int i = 0; i <= root->num_keys; i++) {
            print_tree(root->children[i], level + 1);
        }
    }
}

// 키 검색
bool search(BTreeNode* root, int key, int* level) {
    int i = 0;
    (*level)++;

    while (i < root->num_keys && key > root->keys[i]) {
        i++;
    }

    if (i < root->num_keys && key == root->keys[i]) {
        printf("키 %d를 레벨 %d에서 찾음\n", key, *level);
        return true;
    }

    if (root->is_leaf) {
        printf("키 %d를 찾지 못함\n", key);
        return false;
    }

    return search(root->children[i], key, level);
}

// 메모리 해제
void free_tree(BTreeNode* root) {
    if (!root) return;

    if (!root->is_leaf) {
        for (int i = 0; i <= root->num_keys; i++) {
            free_tree(root->children[i]);
        }
    }

    free(root);
}

// 트리 통계 출력
void print_tree_stats(BTreeNode* root, int* total_nodes, int* total_keys, int* height, int current_height) {
    if (!root) return;

    (*total_nodes)++;
    *total_keys += root->num_keys;
    *height = current_height > *height ? current_height : *height;

    if (!root->is_leaf) {
        for (int i = 0; i <= root->num_keys; i++) {
            print_tree_stats(root->children[i], total_nodes, total_keys, height, current_height + 1);
        }
    }
}

int main(void) {
    BTreeNode* root = create_node(true);

    printf("=== B-트리 테스트 (차수 %d) ===\n", MAX_KEYS + 1);
    printf("1: 키 삽입\n");
    printf("2: 키 검색\n");
    printf("3: 트리 출력\n");
    printf("4: 트리 통계\n");
    printf("0: 종료\n");

    while (1) {
        int choice, value;
        printf("\n선택: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            printf("삽입할 키: ");
            scanf("%d", &value);
            root = insert(root, value);
            printf("\n현재 트리 상태:\n");
            print_tree(root, 0);
            break;

        case 2:
            printf("검색할 키: ");
            scanf("%d", &value);
            int level = 0;
            search(root, value, &level);
            break;

        case 3:
            printf("\n현재 트리 상태:\n");
            print_tree(root, 0);
            break;

        case 4: {
            int total_nodes = 0, total_keys = 0, height = 0;
            print_tree_stats(root, &total_nodes, &total_keys, &height, 0);
            printf("\n=== 트리 통계 ===\n");
            printf("총 노드 수: %d\n", total_nodes);
            printf("총 키 수: %d\n", total_keys);
            printf("트리 높이: %d\n", height);
            printf("평균 키/노드: %.2f\n", (float)total_keys / total_nodes);
            break;
        }

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
B-트리 분석
========

1. 시간 복잡도
-----------
- 검색: O(log_t n), t는 차수
- 삽입: O(log_t n)
- 삭제: O(log_t n)

2. 공간 활용
---------
- 최소 50% 공간 활용
- 노드당 t-1 ~ 2t-1개 키
- 균형 유지로 일정한 성능

3. 디스크 최적화
------------
- 노드 크기 = 디스크 블록
- IO 횟수 최소화
- 순차 접근 효율적

4. 활용 분야
---------
- 데이터베이스 인덱싱
- 파일 시스템
- 대용량 데이터 처리

이 구현은 실제 데이터베이스
시스템에서 사용되는 인덱싱의
기본 원리를 보여줍니다.
*/
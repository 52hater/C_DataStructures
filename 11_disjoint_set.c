#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef int ElementType;

// 분리 집합의 노드를 표현하는 구조체
typedef struct {
    ElementType* parent;  // 부모 노드 배열
    int* rank;           // 트리의 높이(랭크) 배열
    size_t size;         // 전체 원소의 개수
} DisjointSet;

/* 분리 집합 초기화
 * - 매개변수: size - 집합의 크기
 * - 반환값: 생성된 분리 집합 또는 실패 시 NULL
 */
DisjointSet* disjoint_set_create(size_t size) {
    DisjointSet* set = (DisjointSet*)malloc(sizeof(DisjointSet));
    if (set == NULL) {
        return NULL;
    }

    set->parent = (ElementType*)malloc(size * sizeof(ElementType));
    set->rank = (int*)malloc(size * sizeof(int));
    if (set->parent == NULL || set->rank == NULL) {
        free(set->parent);
        free(set->rank);
        free(set);
        return NULL;
    }

    // 각 원소를 독립된 집합으로 초기화
    for (size_t i = 0; i < size; i++) {
        set->parent[i] = i;  // 자기 자신이 대표 원소
        set->rank[i] = 0;    // 초기 랭크는 0
    }
    set->size = size;

    return set;
}

/* 원소가 속한 집합의 대표 원소 찾기 (경로 압축 포함)
 * - 매개변수: set - 분리 집합, x - 찾을 원소
 * - 반환값: 대표 원소의 인덱스
 */
ElementType find_set(DisjointSet* set, ElementType x) {
    if (x != set->parent[x]) {
        set->parent[x] = find_set(set, set->parent[x]);  // 경로 압축
    }
    return set->parent[x];
}

/* 두 집합을 합치기 (랭크를 이용한 합집합)
 * - 매개변수: set - 분리 집합, x, y - 합칠 두 원소
 * - 반환값: 합치기 성공 시 true, 실패 시 false
 */
bool union_sets(DisjointSet* set, ElementType x, ElementType y) {
    ElementType root_x = find_set(set, x);
    ElementType root_y = find_set(set, y);

    if (root_x == root_y) {
        return false;  // 이미 같은 집합
    }

    // 랭크가 더 큰 트리에 작은 트리를 붙임
    if (set->rank[root_x] < set->rank[root_y]) {
        set->parent[root_x] = root_y;
    }
    else if (set->rank[root_x] > set->rank[root_y]) {
        set->parent[root_y] = root_x;
    }
    else {
        set->parent[root_y] = root_x;
        set->rank[root_x]++;
    }

    return true;
}

/* 두 원소가 같은 집합에 속하는지 확인
 * - 매개변수: set - 분리 집합, x, y - 확인할 두 원소
 * - 반환값: 같은 집합이면 true, 다른 집합이면 false
 */
bool is_same_set(DisjointSet* set, ElementType x, ElementType y) {
    return find_set(set, x) == find_set(set, y);
}

/* 현재 분리 집합의 상태 출력
 * - 매개변수: set - 출력할 분리 집합
 */
void print_sets(DisjointSet* set) {
    printf("\nDisjoint Set Status:\n");
    printf("Element: ");
    for (size_t i = 0; i < set->size; i++) {
        printf("%2zu ", i);
    }
    printf("\nParent:  ");
    for (size_t i = 0; i < set->size; i++) {
        printf("%2d ", set->parent[i]);
    }
    printf("\nRank:    ");
    for (size_t i = 0; i < set->size; i++) {
        printf("%2d ", set->rank[i]);
    }
    printf("\n");
}

/* 각 집합의 원소들을 그룹화하여 출력
 * - 매개변수: set - 출력할 분리 집합
 */
void print_set_groups(DisjointSet* set) {
    bool* visited = (bool*)calloc(set->size, sizeof(bool));
    if (visited == NULL) {
        return;
    }

    printf("\nSet Groups:\n");
    for (size_t i = 0; i < set->size; i++) {
        if (!visited[i]) {
            ElementType root = find_set(set, i);
            printf("Group %d: ", root);

            // 같은 집합에 속한 원소들 출력
            for (size_t j = 0; j < set->size; j++) {
                if (!visited[j] && find_set(set, j) == root) {
                    printf("%zu ", j);
                    visited[j] = true;
                }
            }
            printf("\n");
        }
    }

    free(visited);
}

/* 분리 집합의 메모리 해제
 * - 매개변수: set - 해제할 분리 집합
 */
void disjoint_set_destroy(DisjointSet* set) {
    if (set != NULL) {
        free(set->parent);
        free(set->rank);
        free(set);
    }
}

/* 메뉴 출력 함수 */
void print_menu(void) {
    printf("\n=== Disjoint Set Menu ===\n");
    printf("1. Union two elements\n");
    printf("2. Find set of element\n");
    printf("3. Check if same set\n");
    printf("4. Print set status\n");
    printf("5. Print set groups\n");
    printf("0. Exit\n");
    printf("Choice: ");
}

int main(void) {
    size_t size;
    printf("Enter the size of disjoint set: ");
    if (scanf("%zu", &size) != 1 || size == 0) {
        printf("Invalid size\n");
        return 1;
    }

    DisjointSet* set = disjoint_set_create(size);
    if (set == NULL) {
        printf("Failed to create disjoint set\n");
        return 1;
    }

    int choice;
    ElementType x, y;

    do {
        print_menu();
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
        case 1:  // Union
            printf("Enter two elements to union (0 to %zu): ", size - 1);
            if (scanf("%d %d", &x, &y) != 2 ||
                x < 0 || x >= size || y < 0 || y >= size) {
                printf("Invalid elements\n");
                break;
            }
            if (union_sets(set, x, y)) {
                printf("Successfully united %d and %d\n", x, y);
            }
            else {
                printf("Elements %d and %d are already in the same set\n", x, y);
            }
            break;

        case 2:  // Find
            printf("Enter element to find (0 to %zu): ", size - 1);
            if (scanf("%d", &x) != 1 || x < 0 || x >= size) {
                printf("Invalid element\n");
                break;
            }
            printf("Representative of %d is %d\n", x, find_set(set, x));
            break;

        case 3:  // Check if same set
            printf("Enter two elements to check (0 to %zu): ", size - 1);
            if (scanf("%d %d", &x, &y) != 2 ||
                x < 0 || x >= size || y < 0 || y >= size) {
                printf("Invalid elements\n");
                break;
            }
            if (is_same_set(set, x, y)) {
                printf("Elements %d and %d are in the same set\n", x, y);
            }
            else {
                printf("Elements %d and %d are in different sets\n", x, y);
            }
            break;

        case 4:  // Print status
            print_sets(set);
            break;

        case 5:  // Print groups
            print_set_groups(set);
            break;

        case 0:  // Exit
            printf("Exiting program\n");
            break;

        default:
            printf("Invalid choice\n");
        }
    } while (choice != 0);

    disjoint_set_destroy(set);
    return 0;
}

/*
==========================================
상세 설명 및 주요 개념
==========================================

1. 분리 집합의 특징
----------------
- 서로소인 부분집합들의 집합
- 동적 연결성 관리
- 트리 기반 구현
- Union-Find 연산 지원

2. 최적화 기법
-----------
경로 압축(Path Compression):
- find_set 연산 시 적용
- 모든 노드를 루트에 직접 연결
- 평균 시간복잡도 개선

랭크 기반 합치기(Union by Rank):
- 트리의 높이를 최소화
- 작은 트리를 큰 트리에 붙임
- 균형 잡힌 트리 유지

3. 주요 연산과 복잡도
-----------------
Make-Set: O(1)
- 초기 단일 원소 집합 생성

Find-Set: O(α(n))
- 원소의 대표 원소 찾기
- α(n)은 애커만 함수의 역함수

Union: O(α(n))
- 두 집합을 합치기
- 랭크 기반 최적화

4. 구현 특징
----------
- 배열 기반 구현
- 랭크 배열 관리
- 경로 압축 적용
- 메모리 효율성

5. 메모리 관리
-----------
- 동적 배열 할당
- 안전한 메모리 해제
- 메모리 누수 방지
- 초기화 검증

6. 응용 분야
----------
- 크루스칼 알고리즘
- 사이클 검출
- 네트워크 연결성
- 동적 그래프

7. 교육적 가치
-----------
- 트리 구조 이해
- 최적화 기법 학습
- 시간복잡도 분석
- 자료구조 활용

8. 최적화 고려사항
--------------
- 경로 압축 구현
- 랭크 기반 합치기
- 메모리 접근 최적화
- 캐시 효율성

이 구현은 분리 집합의 표준적인 기능을
모두 포함하며, 최적화 기법이 적용된
효율적인 구현을 제공합니다.
*/
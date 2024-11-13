#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef int ElementType;

// �и� ������ ��带 ǥ���ϴ� ����ü
typedef struct {
    ElementType* parent;  // �θ� ��� �迭
    int* rank;           // Ʈ���� ����(��ũ) �迭
    size_t size;         // ��ü ������ ����
} DisjointSet;

/* �и� ���� �ʱ�ȭ
 * - �Ű�����: size - ������ ũ��
 * - ��ȯ��: ������ �и� ���� �Ǵ� ���� �� NULL
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

    // �� ���Ҹ� ������ �������� �ʱ�ȭ
    for (size_t i = 0; i < size; i++) {
        set->parent[i] = i;  // �ڱ� �ڽ��� ��ǥ ����
        set->rank[i] = 0;    // �ʱ� ��ũ�� 0
    }
    set->size = size;

    return set;
}

/* ���Ұ� ���� ������ ��ǥ ���� ã�� (��� ���� ����)
 * - �Ű�����: set - �и� ����, x - ã�� ����
 * - ��ȯ��: ��ǥ ������ �ε���
 */
ElementType find_set(DisjointSet* set, ElementType x) {
    if (x != set->parent[x]) {
        set->parent[x] = find_set(set, set->parent[x]);  // ��� ����
    }
    return set->parent[x];
}

/* �� ������ ��ġ�� (��ũ�� �̿��� ������)
 * - �Ű�����: set - �и� ����, x, y - ��ĥ �� ����
 * - ��ȯ��: ��ġ�� ���� �� true, ���� �� false
 */
bool union_sets(DisjointSet* set, ElementType x, ElementType y) {
    ElementType root_x = find_set(set, x);
    ElementType root_y = find_set(set, y);

    if (root_x == root_y) {
        return false;  // �̹� ���� ����
    }

    // ��ũ�� �� ū Ʈ���� ���� Ʈ���� ����
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

/* �� ���Ұ� ���� ���տ� ���ϴ��� Ȯ��
 * - �Ű�����: set - �и� ����, x, y - Ȯ���� �� ����
 * - ��ȯ��: ���� �����̸� true, �ٸ� �����̸� false
 */
bool is_same_set(DisjointSet* set, ElementType x, ElementType y) {
    return find_set(set, x) == find_set(set, y);
}

/* ���� �и� ������ ���� ���
 * - �Ű�����: set - ����� �и� ����
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

/* �� ������ ���ҵ��� �׷�ȭ�Ͽ� ���
 * - �Ű�����: set - ����� �и� ����
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

            // ���� ���տ� ���� ���ҵ� ���
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

/* �и� ������ �޸� ����
 * - �Ű�����: set - ������ �и� ����
 */
void disjoint_set_destroy(DisjointSet* set) {
    if (set != NULL) {
        free(set->parent);
        free(set->rank);
        free(set);
    }
}

/* �޴� ��� �Լ� */
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
�� ���� �� �ֿ� ����
==========================================

1. �и� ������ Ư¡
----------------
- ���μ��� �κ����յ��� ����
- ���� ���Ἲ ����
- Ʈ�� ��� ����
- Union-Find ���� ����

2. ����ȭ ���
-----------
��� ����(Path Compression):
- find_set ���� �� ����
- ��� ��带 ��Ʈ�� ���� ����
- ��� �ð����⵵ ����

��ũ ��� ��ġ��(Union by Rank):
- Ʈ���� ���̸� �ּ�ȭ
- ���� Ʈ���� ū Ʈ���� ����
- ���� ���� Ʈ�� ����

3. �ֿ� ����� ���⵵
-----------------
Make-Set: O(1)
- �ʱ� ���� ���� ���� ����

Find-Set: O(��(n))
- ������ ��ǥ ���� ã��
- ��(n)�� ��Ŀ�� �Լ��� ���Լ�

Union: O(��(n))
- �� ������ ��ġ��
- ��ũ ��� ����ȭ

4. ���� Ư¡
----------
- �迭 ��� ����
- ��ũ �迭 ����
- ��� ���� ����
- �޸� ȿ����

5. �޸� ����
-----------
- ���� �迭 �Ҵ�
- ������ �޸� ����
- �޸� ���� ����
- �ʱ�ȭ ����

6. ���� �о�
----------
- ũ�罺Į �˰���
- ����Ŭ ����
- ��Ʈ��ũ ���Ἲ
- ���� �׷���

7. ������ ��ġ
-----------
- Ʈ�� ���� ����
- ����ȭ ��� �н�
- �ð����⵵ �м�
- �ڷᱸ�� Ȱ��

8. ����ȭ �������
--------------
- ��� ���� ����
- ��ũ ��� ��ġ��
- �޸� ���� ����ȭ
- ĳ�� ȿ����

�� ������ �и� ������ ǥ������ �����
��� �����ϸ�, ����ȭ ����� �����
ȿ������ ������ �����մϴ�.
*/
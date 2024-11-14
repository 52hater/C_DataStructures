#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
�賶 ���� (0/1 Knapsack Problem)
==============================

Ư¡:
1. �� ������ ��°�� �ְų� �� �ְų� (0 �Ǵ� 1)
2. ���ѵ� ���� ������ �ִ� ��ġ ����
3. ���� �Ұ����� ���ǵ��� ���� ����ȭ

�⺻ ����:
- ���� i�� ����: w[i]
- ���� i�� ��ġ: v[i]
- �賶 �뷮: W
- ������: dp[i][w]
*/

// === ���� ����ü ===
typedef struct {
    int weight;     // ����
    int value;      // ��ġ
    char* name;     // ���� �̸�
} Item;

// === �賶 ���̺� ����ü ===
typedef struct {
    int** table;    // DP ���̺�
    bool** selected; // ���õ� ���� ����
    int rows;       // ���� �� + 1
    int cols;       // �賶 �뷮 + 1
} KnapsackTable;

// === �賶 ��� ����ü ===
typedef struct {
    int maxValue;           // �ִ� ��ġ
    bool* selectedItems;    // ���õ� ���ǵ�
    int numSelected;        // ���õ� ���� ��
} KnapsackResult;

// === �賶 ���̺� ���� ===
KnapsackTable* create_knapsack_table(int numItems, int capacity) {
    KnapsackTable* table = (KnapsackTable*)malloc(sizeof(KnapsackTable));
    table->rows = numItems + 1;
    table->cols = capacity + 1;

    // DP ���̺� �Ҵ�
    table->table = (int**)malloc(table->rows * sizeof(int*));
    table->selected = (bool**)malloc(table->rows * sizeof(bool*));

    for (int i = 0; i < table->rows; i++) {
        table->table[i] = (int*)calloc(table->cols, sizeof(int));
        table->selected[i] = (bool*)calloc(table->cols, sizeof(bool));
    }

    return table;
}

// === �賶 ���̺� �޸� ���� ===
void destroy_knapsack_table(KnapsackTable* table) {
    for (int i = 0; i < table->rows; i++) {
        free(table->table[i]);
        free(table->selected[i]);
    }
    free(table->table);
    free(table->selected);
    free(table);
}

// === �賶 ���̺� ��� ===
void print_knapsack_table(KnapsackTable* table, Item* items) {
    printf("\n=== �賶 ���� DP ���̺� ===\n");
    printf("    ");
    for (int w = 0; w < table->cols; w++) {
        printf("%4d ", w);
    }
    printf("\n");

    for (int i = 0; i < table->rows; i++) {
        if (i == 0) printf("0   ");
        else printf("%-3s ", items[i - 1].name);

        for (int w = 0; w < table->cols; w++) {
            printf("%4d ", table->table[i][w]);
        }
        printf("\n");
    }
}

// === �賶 ���� �ذ� ===
KnapsackResult* solve_knapsack(Item* items, int numItems, int capacity, bool print_steps) {
    KnapsackTable* table = create_knapsack_table(numItems, capacity);

    if (print_steps) {
        printf("\n=== �賶 ���� �ذ� ���� ===\n");
    }

    // Bottom-up ������� ���̺� ä���
    for (int i = 1; i <= numItems; i++) {
        for (int w = 0; w <= capacity; w++) {
            if (items[i - 1].weight <= w) {
                int value_with_item = table->table[i - 1][w - items[i - 1].weight]
                    + items[i - 1].value;
                int value_without_item = table->table[i - 1][w];

                if (value_with_item > value_without_item) {
                    table->table[i][w] = value_with_item;
                    table->selected[i][w] = true;

                    if (print_steps) {
                        printf("���� %s ���� (����: %d, ��ġ: %d)\n",
                            items[i - 1].name, items[i - 1].weight, items[i - 1].value);
                    }
                }
                else {
                    table->table[i][w] = value_without_item;
                }
            }
            else {
                table->table[i][w] = table->table[i - 1][w];
            }
        }

        if (print_steps) {
            print_knapsack_table(table, items);
            printf("\n");
        }
    }

    // ��� ����
    KnapsackResult* result = (KnapsackResult*)malloc(sizeof(KnapsackResult));
    result->maxValue = table->table[numItems][capacity];
    result->selectedItems = (bool*)calloc(numItems, sizeof(bool));
    result->numSelected = 0;

    // ���õ� ���� ����
    int i = numItems;
    int w = capacity;
    while (i > 0 && w > 0) {
        if (table->selected[i][w]) {
            result->selectedItems[i - 1] = true;
            result->numSelected++;
            w -= items[i - 1].weight;
        }
        i--;
    }

    destroy_knapsack_table(table);
    return result;
}

// === ��� ��� ===
void print_knapsack_result(KnapsackResult* result, Item* items, int numItems) {
    printf("\n=== �賶 ���� �ذ� ��� ===\n");
    printf("�ִ� ��ġ: %d\n", result->maxValue);
    printf("\n���õ� ���ǵ�:\n");

    int totalWeight = 0;
    for (int i = 0; i < numItems; i++) {
        if (result->selectedItems[i]) {
            printf("- %s (����: %d, ��ġ: %d)\n",
                items[i].name, items[i].weight, items[i].value);
            totalWeight += items[i].weight;
        }
    }
    printf("\n�� ����: %d\n", totalWeight);
}

// === ���� �Լ� ===
int main(void) {
    printf("=== �賶 ���� �ذ� ���α׷� ===\n");

    int numItems, capacity;
    printf("\n���� ���� �Է�: ");
    scanf("%d", &numItems);

    Item* items = (Item*)malloc(numItems * sizeof(Item));

    for (int i = 0; i < numItems; i++) {
        items[i].name = (char*)malloc(20 * sizeof(char));
        printf("\n���� %d ���� �Է�:\n", i + 1);
        printf("�̸�: ");
        scanf("%s", items[i].name);
        printf("����: ");
        scanf("%d", &items[i].weight);
        printf("��ġ: ");
        scanf("%d", &items[i].value);
    }

    printf("\n�賶 �뷮 �Է�: ");
    scanf("%d", &capacity);

    bool print_steps;
    printf("\n������ ����Ͻðڽ��ϱ�? (1: ��, 0: �ƴϿ�): ");
    scanf("%d", &print_steps);

    KnapsackResult* result = solve_knapsack(items, numItems, capacity, print_steps);
    print_knapsack_result(result, items, numItems);

    // �޸� ����
    for (int i = 0; i < numItems; i++) {
        free(items[i].name);
    }
    free(items);
    free(result->selectedItems);
    free(result);

    return 0;
}

/*
�賶 ���� �м�
===========

1. �ð� ���⵵
-----------
- O(nW): n�� ���� ��, W�� �賶 �뷮
- ��� �κ� ������ �� ���� ���
- �� ������ O(1) ����

2. ���� ���⵵
-----------
- O(nW): 2���� DP ���̺�
- ���� ������ ���� �߰� �迭
- �Է� ������ ���� ����

3. ����ȭ ���ɼ�
------------
���� ����ȭ:
- O(W) �������� ���� �� ����
- �� �ุ ����ϴ� ���
- ���� ���� ���� �� ����

����ȭ:
- �� ���� ��� ���� ó��
- GPU Ȱ�� ����
- ��Ը� ������ ó��

4. ���� �о�
---------
- ���ҽ� �Ҵ�
- ��Ʈ������ ����ȭ
- ���� ��ȹ
- ȭ�� ����
- �ð� ����

5. ���� ����
---------
���� ������ �賶:
- Fractional Knapsack
- Greedy ���� ����
- �� �ܼ��� �ذ�

���� ����:
- Multiple Constraints
- �� ������ DP �ʿ�
- ���� ����

6. ���� �������
------------
���е�:
- ���� ����
- �����÷ο� ����
- �ε��Ҽ��� ó��

�޸� ����:
- ���� �Ҵ� ����
- �޸� ����
- ū �Է� ó��

�ð�ȭ:
- �ܰ躰 ���
- ��� ǥ��
- ����� �������̽�

�� ������ ���������� �����ϸ�
���� ���뿡�� Ȱ�� ������
������ �賶 ���� �ذ�⸦
�����մϴ�.
*/
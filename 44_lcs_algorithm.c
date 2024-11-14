#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*
���� ���� �κм��� (LCS: Longest Common Subsequence)
================================================

Ư¡:
1. �� ������ ���� �κм��� �� ���� �� ���� ã�� ����
2. �κм����� �������� �ʿ� ���� (Substring�� �ٸ�)
3. ���� ��ȹ���� ��ǥ���� ����

����:
- ���ڿ� 1: "ABCDE"
- ���ڿ� 2: "ACE"
- LCS: "ACE" (���� 3)
*/

// === LCS ��� ����� �����ϴ� ����ü ===
typedef struct {
    int** length;    // LCS ���� ���̺�
    char** path;     // ��� ������ ���� ���̺�
    int rows;        // �� �� (ù ��° ���ڿ� ���� + 1)
    int cols;        // �� �� (�� ��° ���ڿ� ���� + 1)
} LCSTable;

// === LCS ����� �����ϴ� ����ü ===
typedef struct {
    char* sequence;  // ���� LCS ���ڿ�
    int length;      // LCS ����
} LCSResult;

// === LCS ���̺� ���� ===
LCSTable* create_lcs_table(int rows, int cols) {
    LCSTable* table = (LCSTable*)malloc(sizeof(LCSTable));
    table->rows = rows;
    table->cols = cols;

    // ���� ���̺� �Ҵ�
    table->length = (int**)malloc(rows * sizeof(int*));
    table->path = (char**)malloc(rows * sizeof(char*));

    for (int i = 0; i < rows; i++) {
        table->length[i] = (int*)calloc(cols, sizeof(int));
        table->path[i] = (char*)malloc(cols * sizeof(char));
    }

    return table;
}

// === LCS ���̺� �޸� ���� ===
void destroy_lcs_table(LCSTable* table) {
    for (int i = 0; i < table->rows; i++) {
        free(table->length[i]);
        free(table->path[i]);
    }
    free(table->length);
    free(table->path);
    free(table);
}

// === LCS ���̺� ��� (�����/������) ===
void print_lcs_table(LCSTable* table, const char* str1, const char* str2) {
    printf("\n=== LCS ���̺� ===\n");
    printf("     ");
    for (int j = 0; j < strlen(str2); j++) {
        printf("%3c ", str2[j]);
    }
    printf("\n");

    for (int i = 0; i < table->rows; i++) {
        if (i == 0) printf("  ");
        else printf("%c ", str1[i - 1]);

        for (int j = 0; j < table->cols; j++) {
            printf("%3d ", table->length[i][j]);
        }
        printf("\n");
    }
}

// === LCS ��� ===
LCSTable* compute_lcs(const char* str1, const char* str2, bool print_steps) {
    int m = strlen(str1);
    int n = strlen(str2);
    LCSTable* table = create_lcs_table(m + 1, n + 1);

    if (print_steps) {
        printf("\n=== LCS ��� ���� ===\n");
    }

    // Bottom-up ������� ���̺� ä���
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (str1[i - 1] == str2[j - 1]) {
                table->length[i][j] = table->length[i - 1][j - 1] + 1;
                table->path[i][j] = 'D';  // Diagonal

                if (print_steps) {
                    printf("Match found: %c at positions %d,%d\n",
                        str1[i - 1], i - 1, j - 1);
                    printf("LCS length increased to %d\n",
                        table->length[i][j]);
                }
            }
            else if (table->length[i - 1][j] >= table->length[i][j - 1]) {
                table->length[i][j] = table->length[i - 1][j];
                table->path[i][j] = 'U';  // Up

                if (print_steps) {
                    printf("No match at %d,%d, taking up value: %d\n",
                        i - 1, j - 1, table->length[i][j]);
                }
            }
            else {
                table->length[i][j] = table->length[i][j - 1];
                table->path[i][j] = 'L';  // Left

                if (print_steps) {
                    printf("No match at %d,%d, taking left value: %d\n",
                        i - 1, j - 1, table->length[i][j]);
                }
            }
        }

        if (print_steps) {
            print_lcs_table(table, str1, str2);
            printf("\n");
        }
    }

    return table;
}

// === LCS ���� (����� ���) ===
void extract_lcs(LCSTable* table, const char* str1,
    int i, int j, char* result, int* pos) {
    if (i == 0 || j == 0) return;

    if (table->path[i][j] == 'D') {
        extract_lcs(table, str1, i - 1, j - 1, result, pos);
        result[(*pos)++] = str1[i - 1];
    }
    else if (table->path[i][j] == 'U') {
        extract_lcs(table, str1, i - 1, j, result, pos);
    }
    else {
        extract_lcs(table, str1, i, j - 1, result, pos);
    }
}

// === LCS ��� ���� ===
LCSResult* get_lcs_result(LCSTable* table, const char* str1, const char* str2) {
    LCSResult* result = (LCSResult*)malloc(sizeof(LCSResult));
    result->length = table->length[table->rows - 1][table->cols - 1];
    result->sequence = (char*)malloc((result->length + 1) * sizeof(char));

    int pos = 0;
    extract_lcs(table, str1, table->rows - 1, table->cols - 1,
        result->sequence, &pos);
    result->sequence[pos] = '\0';

    return result;
}

// === LCS ��� �޸� ���� ===
void destroy_lcs_result(LCSResult* result) {
    free(result->sequence);
    free(result);
}

// === LCS ��� �ð�ȭ ===
void visualize_lcs(const char* str1, const char* str2, LCSResult* result) {
    printf("\n=== LCS �ð�ȭ ===\n");
    printf("���ڿ� 1: %s\n", str1);
    printf("���ڿ� 2: %s\n", str2);
    printf("LCS: %s (����: %d)\n\n", result->sequence, result->length);

    // ���ڿ� 1���� LCS ���� ���̶���Ʈ
    printf("���ڿ� 1���� LCS: ");
    int lcs_pos = 0;
    for (int i = 0; i < strlen(str1); i++) {
        if (lcs_pos < result->length &&
            str1[i] == result->sequence[lcs_pos]) {
            printf("[%c]", str1[i]);
            lcs_pos++;
        }
        else {
            printf("%c", str1[i]);
        }
    }
    printf("\n");

    // ���ڿ� 2���� LCS ���� ���̶���Ʈ
    printf("���ڿ� 2���� LCS: ");
    lcs_pos = 0;
    for (int i = 0; i < strlen(str2); i++) {
        if (lcs_pos < result->length &&
            str2[i] == result->sequence[lcs_pos]) {
            printf("[%c]", str2[i]);
            lcs_pos++;
        }
        else {
            printf("%c", str2[i]);
        }
    }
    printf("\n");
}

int main(void) {
    printf("=== ���� ���� �κм���(LCS) ��� ���α׷� ===\n");

    char str1[100], str2[100];
    bool print_steps;

    while (1) {
        printf("\n1. LCS ���\n");
        printf("2. ����\n");
        printf("����: ");

        int choice;
        scanf("%d", &choice);
        getchar();  // ���� ����

        if (choice == 2) break;

        if (choice == 1) {
            printf("\nù ��° ���ڿ� �Է�: ");
            fgets(str1, sizeof(str1), stdin);
            str1[strcspn(str1, "\n")] = '\0';

            printf("�� ��° ���ڿ� �Է�: ");
            fgets(str2, sizeof(str2), stdin);
            str2[strcspn(str2, "\n")] = '\0';

            printf("��� ������ ����Ͻðڽ��ϱ�? (1: ��, 0: �ƴϿ�): ");
            scanf("%d", &print_steps);

            // LCS ���
            LCSTable* table = compute_lcs(str1, str2, print_steps);
            LCSResult* result = get_lcs_result(table, str1, str2);

            // ��� ���
            visualize_lcs(str1, str2, result);

            // �޸� ����
            destroy_lcs_table(table);
            destroy_lcs_result(result);
        }
    }

    return 0;
}

/*
LCS �˰��� �м�
===============

1. �ð� ���⵵
-----------
- O(mn): m, n�� �� ���ڿ��� ����
- 2�� ������ ��� �κ� ���� �ذ�
- ��� ������ O(m+n)

2. ���� ���⵵
-----------
- O(mn): 2���� �迭 ���
- ���� ���̺�� ��� ���̺� �ʿ�
- ����ȭ ���ɼ� ����

3. ����ȭ ���ɼ�
------------
���� ����ȭ:
- O(min(m,n)) �������� ���� ����
- �� �ุ �����ϴ� ���
- ��� ���� ���� �� ����

�ð� ����ȭ:
- ����ȭ ����
- ���� ���� ����
- Ư�� ���̽� ����ȭ

4. ���� �о�
---------
- ������ ���� �м�
- ���� �� (diff)
- ǥ�� �˻�
- �ڿ��� ó��
- ���� ���� �ý���

5. ���� �� �������
--------------
�޸� ����:
- ���� �Ҵ�/���� ����
- ū ���ڿ� ó��
- �޸� ���� ����

���� ó��:
- �� ���ڿ� ó��
- �޸� �Ҵ� ����
- �Է� ũ�� ����

�ð�ȭ:
- �ܰ躰 ���� ǥ��
- ��� ���̶���Ʈ
- ������ ���

�� ������ LCS�� �⺻ �������
���� ������� �����ϸ�, ������
�ǹ����� ��� Ȱ�� �����մϴ�.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*
KMP(Knuth-Morris-Pratt) �˰���:
- ���λ�� ���̻��� ������ Ȱ��
- ���� �Լ�(�κ���ġ ���̺�)�� �̸� ���
- ����ġ �߻� �� ������ ȿ�������� �̵�
- �ߺ� �񱳸� ���Ͽ� ���� ���

�κ���ġ ���̺�(failure function):
- �� ��ġ���� ��ġ�ϴ� ���λ�=���̻� ����
- ����ġ �߻� �� ���� ���ư��� ����
- ��: "ABCDABD"�� ���̺�: [0,0,0,0,1,2,0]
*/

typedef struct {
    int* positions;    // �߰ߵ� ��ġ���� �迭
    int count;         // �߰ߵ� �� Ƚ��
    int capacity;      // positions �迭�� ũ��
} SearchResult;

/* �˻� ��� �ʱ�ȭ */
SearchResult* create_result(int initial_capacity) {
    SearchResult* result = (SearchResult*)malloc(sizeof(SearchResult));
    if (!result) return NULL;

    result->positions = (int*)malloc(initial_capacity * sizeof(int));
    if (!result->positions) {
        free(result);
        return NULL;
    }

    result->count = 0;
    result->capacity = initial_capacity;
    return result;
}

/* �˻� ����� ���ο� ��ġ �߰� */
void add_position(SearchResult* result, int position) {
    if (result->count >= result->capacity) {
        int new_capacity = result->capacity * 2;
        int* new_positions = (int*)realloc(result->positions, new_capacity * sizeof(int));
        if (!new_positions) return;

        result->positions = new_positions;
        result->capacity = new_capacity;
    }

    result->positions[result->count++] = position;
}

/* �˻� ��� �޸� ���� */
void destroy_result(SearchResult* result) {
    if (result) {
        free(result->positions);
        free(result);
    }
}

/* �κ���ġ ���̺� ���
 * - �ð����⵵: O(m), m: ������ ����
 */
void compute_failure_function(const char* pattern, int* failure, bool print_steps) {
    int m = strlen(pattern);
    failure[0] = 0;  // ù ��° ���ڴ� �׻� 0

    // j: ������� ��ġ�� ����
    // i: ���� ���� ��ġ
    int j = 0;
    int i = 1;

    if (print_steps) {
        printf("\nComputing failure function:\n");
        printf("Pattern: %s\n", pattern);
    }

    while (i < m) {
        if (print_steps) {
            printf("\nComparing pattern[%d]='%c' with pattern[%d]='%c'\n",
                j, pattern[j], i, pattern[i]);
        }

        if (pattern[i] == pattern[j]) {
            failure[i] = j + 1;
            if (print_steps) {
                printf("Match! failure[%d] = %d\n", i, failure[i]);
            }
            i++;
            j++;
        }
        else if (j > 0) {
            j = failure[j - 1];
            if (print_steps) {
                printf("Mismatch! Going back to position %d\n", j);
            }
        }
        else {
            failure[i] = 0;
            if (print_steps) {
                printf("Mismatch! failure[%d] = 0\n", i);
            }
            i++;
        }

        if (print_steps) {
            printf("Current failure array: [");
            for (int k = 0; k <= i - 1; k++) {
                printf("%d", failure[k]);
                if (k < i - 1) printf(", ");
            }
            printf("]\n");
        }
    }

    if (print_steps) {
        printf("\nFinal failure function: [");
        for (int i = 0; i < m; i++) {
            printf("%d", failure[i]);
            if (i < m - 1) printf(", ");
        }
        printf("]\n");
    }
}

/* KMP ���ڿ� �˻�
 * - �ð����⵵: O(n + m), n: �ؽ�Ʈ ����, m: ���� ����
 */
SearchResult* kmp_search(const char* text, const char* pattern, bool print_steps) {
    SearchResult* result = create_result(10);
    if (!result) return NULL;

    int n = strlen(text);
    int m = strlen(pattern);
    int comparisons = 0;  // �� Ƚ�� ī��Ʈ

    // �κ���ġ ���̺� ���
    int* failure = (int*)malloc(m * sizeof(int));
    if (!failure) {
        destroy_result(result);
        return NULL;
    }

    compute_failure_function(pattern, failure, print_steps);

    if (print_steps) {
        printf("\nStarting KMP search:\n");
    }

    int i = 0;  // �ؽ�Ʈ �ε���
    int j = 0;  // ���� �ε���

    while (i < n) {
        comparisons++;

        if (print_steps) {
            printf("\nComparing text[%d]='%c' with pattern[%d]='%c'\n",
                i, text[i], j, pattern[j]);
            printf("Text:    %s\n", text);
            printf("Pattern: %*s%s\n", i - j, "", pattern);
        }

        if (text[i] == pattern[j]) {
            if (print_steps) {
                printf("Match!\n");
            }

            if (j == m - 1) {  // ������ ã��
                add_position(result, i - m + 1);
                if (print_steps) {
                    printf("Pattern found at position %d\n", i - m + 1);
                }
                j = failure[j - 1];
            }
            else {
                i++;
                j++;
            }
        }
        else if (j > 0) {
            if (print_steps) {
                printf("Mismatch! Going back using failure function: j = %d -> %d\n",
                    j, failure[j - 1]);
            }
            j = failure[j - 1];
        }
        else {
            if (print_steps) {
                printf("Mismatch at pattern start, moving to next text position\n");
            }
            i++;
        }
    }

    if (print_steps) {
        printf("\nTotal comparisons made: %d\n", comparisons);
    }

    free(failure);
    return result;
}

/* �˻� ��� ��� */
void print_search_result(const SearchResult* result, const char* text, const char* pattern) {
    if (result->count == 0) {
        printf("Pattern not found in text.\n");
        return;
    }

    printf("\nPattern \"%s\" found at %d position(s):\n", pattern, result->count);
    for (int i = 0; i < result->count; i++) {
        int pos = result->positions[i];
        printf("Position %d: ", pos);

        // ��Ī�� �κ� ��ü ���ڿ� ���
        printf("....");
        for (int j = pos - 4 >= 0 ? pos - 4 : 0; j < pos; j++) {
            printf("%c", text[j]);
        }
        printf("[%.*s]", (int)strlen(pattern), &text[pos]);
        for (int j = pos + strlen(pattern); j < pos + strlen(pattern) + 4 && text[j]; j++) {
            printf("%c", text[j]);
        }
        printf("....\n");

        // ��ġ ǥ��
        printf("         ");
        for (int j = pos - 4 >= 0 ? pos - 4 : 0; j < pos; j++) {
            printf(" ");
        }
        printf("^");
        for (int j = 1; j < strlen(pattern); j++) {
            printf("~");
        }
        printf("\n");
    }
}

/* �޴� ��� */
void print_menu(void) {
    printf("\n=== KMP String Search Menu ===\n");
    printf("1. Enter new text\n");
    printf("2. Enter new pattern\n");
    printf("3. Search (without steps)\n");
    printf("4. Search with steps\n");
    printf("0. Exit\n");
    printf("Choice: ");
}

int main(void) {
    char text[1000];
    char pattern[100];
    bool has_text = false;
    bool has_pattern = false;

    printf("Welcome to KMP String Search Algorithm Demo!\n");
    printf("This program demonstrates the Knuth-Morris-Pratt string matching algorithm.\n");
    printf("KMP uses a failure function to avoid redundant comparisons.\n\n");

    int choice;
    do {
        print_menu();
        scanf("%d", &choice);
        getchar();  // ���� ����

        switch (choice) {
        case 1:  // Enter text
            printf("Enter text: ");
            fgets(text, sizeof(text), stdin);
            text[strcspn(text, "\n")] = 0;
            has_text = true;
            printf("Text set to: \"%s\"\n", text);
            break;

        case 2:  // Enter pattern
            printf("Enter pattern to search: ");
            fgets(pattern, sizeof(pattern), stdin);
            pattern[strcspn(pattern, "\n")] = 0;
            has_pattern = true;
            printf("Pattern set to: \"%s\"\n", pattern);
            break;

        case 3:  // Search without steps
        case 4:  // Search with steps
            if (!has_text || !has_pattern) {
                printf("Please enter both text and pattern first.\n");
                break;
            }

            SearchResult* result = kmp_search(text, pattern, choice == 4);
            if (result) {
                print_search_result(result, text, pattern);
                destroy_result(result);
            }
            else {
                printf("Search failed due to memory allocation error.\n");
            }
            break;

        case 0:  // Exit
            printf("Thank you for using the program!\n");
            break;

        default:
            printf("Invalid choice\n");
        }
    } while (choice != 0);

    return 0;
}

/*
==========================================
�� ���� �� �ֿ� ����
==========================================

1. KMP �˰����� �ٽ� ���̵��
--------------------------
- �̹� ���� ���� ��Ȱ��
- ���λ�-���̻� ���� Ȱ��
- ���� �Լ��� �ߺ� �� ����
- ȿ������ ���� �̵�

2. ���� �Լ�(�κ���ġ ���̺�)
-------------------------
����:
- ����ġ �߻� �� �̵��� ��ġ ����
- ���� �� ��� Ȱ��
- �ߺ� �� ȸ��

��� ���:
- ���λ�=���̻� ���� ���
- ������ Ȯ��
- O(m) �ð� �ҿ�

3. �ð� ���⵵ �м�
---------------
��ó��: O(m)
- ���� �Լ� ���
- ���� ���̿� ���

�˻�: O(n)
- �ؽ�Ʈ ���̿� ���
- �� ���� �ִ� 1�� ��

��ü: O(n + m)
- ���� �ð� ���⵵
- ���Ʈ �������� ȿ����

4. ���� ���⵵
-----------
O(m)
- ���� �Լ� ����
- ���� ���̿� ���
- �߰� ���� �ʿ�

5. �����
-------
����:
- ���� �ð� ���⵵
- �־��� ��쵵 ����
- �������� ����
- �ؽ�Ʈ ���� ���ʿ�

����:
- ��ó�� �ʿ�
- �߰� �޸� ���
- ���� ����
- ª�� ���Ͽ� �������

6. Ȱ�� �о�
----------
- �ؽ�Ʈ ������
- ���̳ʸ� ���� �˻�
- DNA ���� �м�
- ��Ʈ��ũ ��Ŷ �˻�

�� ������ KMP �˰����� �ٽ���
���� �Լ� ���� �˻� ������
�ܰ躰�� �����ָ�, ���Ʈ ����
��� ���� ����� �����մϴ�.
*/
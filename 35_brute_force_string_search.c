#include <stdio.h>
#include <string.h>
#include <stdbool.h>

/*
�������� Ž��(Brute Force) �˰���:
- ���ڿ� ��Ī�� ���� �⺻���� �˰���
- �ؽ�Ʈ�� ��� ��ġ���� ������ ��
- �� ĭ�� �̵��ϸ鼭 ���������� ��
- ������ �ܼ��ϰ� �����ϱ� ����
*/

/* ���ڿ� Ž�� ��� ������ ���� ����ü */
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

/* �⺻ ���Ʈ ���� ���ڿ� �˻�
 * - �ð����⵵: O(mn), m: ���ϱ���, n: �ؽ�Ʈ����
 * - �������⵵: O(1)
 */
SearchResult* brute_force_search(const char* text, const char* pattern, bool print_steps) {
    SearchResult* result = create_result(10);  // �ʱ� �뷮 10
    if (!result) return NULL;

    int n = strlen(text);
    int m = strlen(pattern);
    int comparisons = 0;  // �� Ƚ�� ī��Ʈ

    // �ؽ�Ʈ�� �� ��ġ���� �õ�
    for (int i = 0; i <= n - m; i++) {
        bool match = true;

        if (print_steps) {
            printf("\nTrying position %d:\n", i);
            printf("Text:    %s\n", text);
            printf("Pattern: %*s%s\n", i, "", pattern);
        }

        // ���� ��ġ���� ���ϰ� ��
        for (int j = 0; j < m; j++) {
            comparisons++;

            if (print_steps) {
                printf("Comparing '%c' with '%c'\n", text[i + j], pattern[j]);
            }

            if (text[i + j] != pattern[j]) {
                match = false;
                break;
            }
        }

        if (match) {
            add_position(result, i);
            if (print_steps) {
                printf("Match found at position %d!\n", i);
            }
        }
    }

    if (print_steps) {
        printf("\nTotal comparisons made: %d\n", comparisons);
    }

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
    printf("\n=== Brute Force String Search Menu ===\n");
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

    printf("Welcome to String Search Algorithm Demo!\n");
    printf("This program demonstrates the Brute Force string matching algorithm.\n\n");

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

            SearchResult* result = brute_force_search(text, pattern, choice == 4);
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

1. ���Ʈ ���� �˰����� ����
-------------------------
- ������ ��� ��ġ �˻�
- �� ĭ�� �̵��ϸ� ��
- ���� Ž�� ���
- ���� ������ �ظ� ã��

2. �ð� ���⵵ �м�
---------------
�־��� ���: O(mn)
- ��� ��ġ���� ���� ���̸�ŭ ��
- n: �ؽ�Ʈ ����
- m: ���� ����

����� ���:
- �����δ� �� ���� ��
- ����ġ �߻� �� ���� ����

3. �����
-------
����:
- ������ �ܼ�
- �����ϱ� ����
- ���� �ؽ�Ʈ�� ȿ����
- ��ó�� ���ʿ�

����:
- ��ȿ������ ����
- ���� �ߺ� ��
- �� �ؽ�Ʈ�� ������
- ���� ���� ��Ȱ��

4. Ȱ�� �о�
----------
- ª�� ���ڿ� �˻�
- �ܼ��� �ؽ�Ʈ ������
- ������ ����
- �ٸ� �˰����� ����

5. ����ȭ ���ɼ�
------------
- ���� ���� ���� ��
- SIMD ��ɾ� Ȱ��
- ĳ�� ������ ����
- ����ȭ ����

6. ������ ��ġ
-----------
- �⺻ ���� ����
- ���� �м� �н�
- ����ȭ �ʿ伺
- �˰��� ��

�� ������ ���Ʈ ���� �����
���ڿ� �˻��� �����ϱ� ����
�����ָ�, ���� �ܰ��� KMP
�˰����� �ʿ伺�� �����մϴ�.
*/
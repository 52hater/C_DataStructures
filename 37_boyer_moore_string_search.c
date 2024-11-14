#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*
Boyer-Moore �˰���:
- ������ ���� ���� ���ڿ� �˻� �˰���
- �ڿ������� ������ ��
- �� ���� �޸���ƽ ���:
  1. Bad Character Rule (���� ���� ��Ģ)
     - ����ġ ���ڰ� ���Ͽ� ������ ���� ���̸�ŭ �̵�
     - �ִٸ� ������ ��ġ���� �̵�
  2. Good Suffix Rule (���� ���̺� ��Ģ)
     - ��ġ�ϴ� ���̺θ� ������� �̵�
     - �� �� �̵� �Ÿ� ����
*/

#define NO_OF_CHARS 256
#define MAX(a, b) ((a) > (b) ? (a) : (b))

typedef struct {
    int* positions;
    int count;
    int capacity;
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
        int* new_positions = (int*)realloc(result->positions,
            new_capacity * sizeof(int));
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

/* Bad Character ��Ģ ���̺� ����
 * - �� ������ ���� �� ������ ��ġ ����
 */
void compute_bad_char(const char* pattern, int m, int bad_char[NO_OF_CHARS]) {
    // ��� ���ڸ� -1�� �ʱ�ȭ
    for (int i = 0; i < NO_OF_CHARS; i++)
        bad_char[i] = -1;

    // ������ �� ������ ������ ��ġ ����
    for (int i = 0; i < m; i++)
        bad_char[(unsigned char)pattern[i]] = i;
}

/* ���̺� ���̺� ����
 * - ������ ���̺ο� ��ġ�ϴ� �ٸ� �κ��� ã��
 */
void compute_good_suffix(const char* pattern, int m, int suffix[], bool prefix[]) {
    // �ʱ�ȭ
    for (int i = 0; i < m; i++) {
        suffix[i] = -1;
        prefix[i] = false;
    }

    // Case 1: ���̺ΰ� �ٽ� ��Ÿ���� ���
    for (int i = 0; i < m - 1; i++) {
        int j = i;
        int len = 0;

        // ������ ���̺ο� ��ġ�ϴ� �κ� ã��
        while (j >= 0 && pattern[j] == pattern[m - 1 - len]) {
            len++;
            suffix[len] = j;
            j--;
        }

        if (j == -1)  // ��ü�� ��ġ�ϴ� ���
            prefix[len] = true;
    }
}

/* Good Suffix ��Ģ�� ���� �̵� �Ÿ� ��� */
int get_good_suffix_shift(int pos, int m, const int suffix[], const bool prefix[]) {
    int len = m - 1 - pos;  // ��ġ�ϴ� ���̺� ����

    // Case 1: ���̺ΰ� �ٽ� ��Ÿ���� ���
    if (suffix[len] != -1)
        return pos - suffix[len] + 1;

    // Case 2: �� ª�� ���̺ΰ� ���κη� ��Ÿ���� ���
    for (int r = pos + 2; r < m; r++) {
        if (prefix[m - r])
            return r;
    }

    // Case 3: �ƹ��͵� ã�� ���� ���
    return m;
}

/* Boyer-Moore ���ڿ� �˻�
 * - �ð����⵵: �ּ� O(n/m), ��� O(n), �־� O(nm)
 */
SearchResult* boyer_moore_search(const char* text, const char* pattern, bool print_steps) {
    SearchResult* result = create_result(10);
    if (!result) return NULL;

    int n = strlen(text);
    int m = strlen(pattern);
    int comparisons = 0;

    // Bad Character ��Ģ ���̺� ����
    int bad_char[NO_OF_CHARS];
    compute_bad_char(pattern, m, bad_char);

    // Good Suffix ��Ģ ���̺� ����
    int* suffix = (int*)malloc(m * sizeof(int));
    bool* prefix = (bool*)malloc(m * sizeof(bool));
    if (!suffix || !prefix) {
        free(suffix);
        free(prefix);
        destroy_result(result);
        return NULL;
    }
    compute_good_suffix(pattern, m, suffix, prefix);

    if (print_steps) {
        printf("\nPrecomputed tables:\n");
        printf("Bad Character Table:\n");
        for (int i = 0; i < NO_OF_CHARS; i++) {
            if (bad_char[i] != -1) {
                printf("'%c': %d\n", i, bad_char[i]);
            }
        }
    }

    int s = 0;  // ������ ���� ��ġ
    while (s <= n - m) {
        int j = m - 1;  // ������ ������ ���ں��� ��

        if (print_steps) {
            printf("\nCurrent position: %d\n", s);
            printf("Text:    %s\n", text);
            printf("Pattern: %*s%s\n", s, "", pattern);
        }

        // ���ϰ� �ؽ�Ʈ ��
        while (j >= 0 && pattern[j] == text[s + j]) {
            comparisons++;
            if (print_steps) {
                printf("Match at position %d\n", s + j);
            }
            j--;
        }

        if (j < 0) {  // ������ ã��
            add_position(result, s);
            if (print_steps) {
                printf("Pattern found at position %d\n", s);
            }
            s += (s + m < n) ? m - bad_char[text[s + m]] : 1;
        }
        else {
            comparisons++;
            if (print_steps) {
                printf("Mismatch at position %d\n", s + j);
            }

            // Bad Character�� Good Suffix ��Ģ �� �� ū �̵� �Ÿ� ����
            int bc_shift = j - bad_char[text[s + j]];
            int gs_shift = get_good_suffix_shift(j, m, suffix, prefix);
            int shift = MAX(bc_shift, gs_shift);

            if (print_steps) {
                printf("Bad Character shift: %d\n", bc_shift);
                printf("Good Suffix shift: %d\n", gs_shift);
                printf("Taking maximum shift: %d\n", shift);
            }

            s += shift;
        }
    }

    if (print_steps) {
        printf("\nTotal comparisons made: %d\n", comparisons);
    }

    free(suffix);
    free(prefix);
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

        printf("....");
        for (int j = pos - 4 >= 0 ? pos - 4 : 0; j < pos; j++) {
            printf("%c", text[j]);
        }
        printf("[%.*s]", (int)strlen(pattern), &text[pos]);
        for (int j = pos + strlen(pattern);
            j < pos + strlen(pattern) + 4 && text[j]; j++) {
            printf("%c", text[j]);
        }
        printf("....\n");

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
    printf("\n=== Boyer-Moore String Search Menu ===\n");
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

    printf("Welcome to Boyer-Moore String Search Algorithm Demo!\n");
    printf("This program demonstrates the Boyer-Moore string matching algorithm,\n");
    printf("which uses both Bad Character and Good Suffix rules for efficient searching.\n\n");

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

            SearchResult* result = boyer_moore_search(text, pattern, choice == 4);
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

1. Boyer-Moore �˰����� Ư¡
------------------------
- �ڿ��� ������ ��
- �� ���� �̵� ��Ģ ���
- ���� ���뿡�� ���� ����
- ��ó�� �ܰ� �ʿ�

2. Bad Character ��Ģ
-----------------
����:
- ����ġ ������ ������ ��ġ Ȱ��
- ���Ͽ� ���� ���ڴ� ���� ���̸�ŭ �̵�
- �ִٸ� ������ ��ġ���� �̵�

����:
- ������ �ܼ�
- ū ���ĺ����� ȿ����
- ���� �̵� ����

3. Good Suffix ��Ģ
---------------
����:
- ��ġ�� ���̺� Ȱ��
- �ٽ� ��Ÿ���� ��ġ�� �̵�
- �� ª�� ���̺ε� ���

����:
- �߰����� �̵� ��ȸ ����
- ���� ���� ���� Ȱ��
- Bad Character ����

4. �ð� ���⵵
-----------
��ó��: O(m + ��)
- m: ���� ����
- ��: ���ĺ� ũ��

�˻�:
- �ּ�: O(n/m)
- ���: O(n)
- �־�: O(nm)

5. ���� ���⵵
-----------
O(m + ��)
- Bad Character ���̺�
- Good Suffix ���̺�
- �߰� �޸� �ʿ�

6. �����
-------
����:
- ������ ���� ����
- �� ���Ͽ� ȿ����
- ���긮�Ͼ� ���� ����

����:
- ������ ����
- ��ó�� �������
- ª�� ���Ͽ� ��ȿ��
- �߰� �޸� �ʿ�

�� ������ Boyer-Moore �˰�����
�� ���� �ֿ� ��Ģ�� ��� �����ϸ�,
���� �˻� ������ �ܰ躰��
Ȯ���� �� �ֽ��ϴ�.
*/
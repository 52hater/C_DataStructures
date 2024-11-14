#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*
Rabin-Karp �˰���:
- �ؽ� �Լ��� ����� ���ڿ� �˻�
- �Ѹ� �ؽ÷� ���ӵ� �κ� ���ڿ��� �ؽð��� ȿ�������� ���
- ���� ���� �˻��� ȿ����
- �ؽ� �浹 ó�� �ʿ�

Ư¡:
- �ؽð� �� �� ���� ���ڿ� ��
- �����츦 �� ĭ�� �̵��ϸ� �ؽð� ����
- ��ⷯ �������� �����÷ο� ����
*/

#define PRIME 101  // �ؽ� ����� ���� �Ҽ�
#define D 256     // ���ڼ� ũ�� (ASCII)

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

/* �ŵ����� ��� (��ⷯ ���� ����)
 * - (a^b) % mod �� ȿ�������� ���
 */
unsigned long long mod_pow(int base, int exp, int mod) {
    unsigned long long result = 1;
    base %= mod;

    while (exp > 0) {
        if (exp & 1)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }

    return result;
}

/* Rabin-Karp ���ڿ� �˻�
 * - �ð����⵵: ��� O(n+m), �־� O(nm)
 */
SearchResult* rabin_karp_search(const char* text, const char* pattern, bool print_steps) {
    SearchResult* result = create_result(10);
    if (!result) return NULL;

    int n = strlen(text);
    int m = strlen(pattern);
    unsigned long long pattern_hash = 0;
    unsigned long long text_hash = 0;
    unsigned long long h = mod_pow(D, m - 1, PRIME);  // D^(m-1) % PRIME
    int comparisons = 0;  // �� Ƚ�� ī��Ʈ

    // ���ϰ� ù �������� �ؽð� ���
    for (int i = 0; i < m; i++) {
        pattern_hash = (D * pattern_hash + pattern[i]) % PRIME;
        text_hash = (D * text_hash + text[i]) % PRIME;
    }

    if (print_steps) {
        printf("\nInitial hash values:\n");
        printf("Pattern hash: %llu\n", pattern_hash);
        printf("First window hash: %llu\n", text_hash);
    }

    // �����̵� ������� �˻�
    for (int i = 0; i <= n - m; i++) {
        if (print_steps) {
            printf("\nWindow at position %d:\n", i);
            printf("Text:    %s\n", text);
            printf("Window:  %*s%.*s\n", i, "", m, &text[i]);
            printf("Pattern: %*s%s\n", i, "", pattern);
            printf("Current window hash: %llu\n", text_hash);
        }

        // �ؽð��� ���� ��� ���� ���ڿ� ��
        if (pattern_hash == text_hash) {
            bool match = true;
            for (int j = 0; j < m; j++) {
                comparisons++;
                if (pattern[j] != text[i + j]) {
                    match = false;
                    break;
                }
            }

            if (match) {
                add_position(result, i);
                if (print_steps) {
                    printf("Match found!\n");
                }
            }
            else if (print_steps) {
                printf("Hash collision - actual mismatch\n");
            }
        }

        // ���� �������� �ؽð� ���
        if (i < n - m) {
            text_hash = (D * (text_hash - text[i] * h) + text[i + m]) % PRIME;
            if (text_hash < 0) {
                text_hash += PRIME;
            }

            if (print_steps) {
                printf("Next window hash: %llu\n", text_hash);
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
    printf("\n=== Rabin-Karp String Search Menu ===\n");
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

    printf("Welcome to Rabin-Karp String Search Algorithm Demo!\n");
    printf("This program demonstrates the Rabin-Karp string matching algorithm,\n");
    printf("which uses rolling hash for efficient string matching.\n\n");

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

            SearchResult* result = rabin_karp_search(text, pattern, choice == 4);
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

1. Rabin-Karp �˰����� ����
-----------------------
- �ؽ� ��� ���ڿ� �˻�
- �Ѹ� �ؽ� ��� ���
- �ؽð� �� �� ���� ��
- ��ⷯ �������� �����÷ο� ����

2. �ؽ� �Լ�
---------
����:
- D: ���(���ڼ� ũ��)
- PRIME: ��ⷯ ����� �Ҽ�
- �Ѹ� �ؽ÷� ȿ���� ���

Ư¡:
- �浹 ���ɼ� ����
- ���� ��� ����
- ��ⷯ ���� �ʿ�

3. �ð� ���⵵
-----------
��ó��: O(m)
- ���� �ؽð� ���
- h = D^(m-1) ���

�˻�:
- ���: O(n + m)
- �־�: O(nm)
- �����δ� ���� ����

4. ���� ���⵵
-----------
O(1)
- ��� ������ �ʿ�
- �߰� �޸� �ּ�ȭ
- �ؽð� ���常 �ʿ�

5. Ư¡�� �����
------------
����:
- ���� ���� �˻� ����
- ��������� ����
- �߰� �޸� ����
- ��ó�� ����

����:
- �ؽ� �浹 ���ɼ�
- ������ ���� ���
- �����÷ο� ����
- �־��� ��� ����

6. Ȱ�� �о�
----------
- ǥ�� �˻�
- ���� ���� �˻�
- DNA ���� �м�
- ���� �ߺ� �˻�

�� ������ Rabin-Karp �˰�����
�ٽ��� �Ѹ� �ؽ� ����� �����ָ�,
�ؽ� ��� ������ �ܰ躰��
Ȯ���� �� �ֽ��ϴ�.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*
Boyer-Moore 알고리즘:
- 실제로 가장 빠른 문자열 검색 알고리즘
- 뒤에서부터 앞으로 비교
- 두 가지 휴리스틱 사용:
  1. Bad Character Rule (나쁜 문자 규칙)
     - 불일치 문자가 패턴에 없으면 패턴 길이만큼 이동
     - 있다면 마지막 위치까지 이동
  2. Good Suffix Rule (착한 접미부 규칙)
     - 일치하는 접미부를 기반으로 이동
     - 더 긴 이동 거리 선택
*/

#define NO_OF_CHARS 256
#define MAX(a, b) ((a) > (b) ? (a) : (b))

typedef struct {
    int* positions;
    int count;
    int capacity;
} SearchResult;

/* 검색 결과 초기화 */
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

/* 검색 결과에 새로운 위치 추가 */
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

/* 검색 결과 메모리 해제 */
void destroy_result(SearchResult* result) {
    if (result) {
        free(result->positions);
        free(result);
    }
}

/* Bad Character 규칙 테이블 생성
 * - 각 문자의 패턴 내 마지막 위치 저장
 */
void compute_bad_char(const char* pattern, int m, int bad_char[NO_OF_CHARS]) {
    // 모든 문자를 -1로 초기화
    for (int i = 0; i < NO_OF_CHARS; i++)
        bad_char[i] = -1;

    // 패턴의 각 문자의 마지막 위치 저장
    for (int i = 0; i < m; i++)
        bad_char[(unsigned char)pattern[i]] = i;
}

/* 접미부 테이블 생성
 * - 패턴의 접미부와 일치하는 다른 부분을 찾음
 */
void compute_good_suffix(const char* pattern, int m, int suffix[], bool prefix[]) {
    // 초기화
    for (int i = 0; i < m; i++) {
        suffix[i] = -1;
        prefix[i] = false;
    }

    // Case 1: 접미부가 다시 나타나는 경우
    for (int i = 0; i < m - 1; i++) {
        int j = i;
        int len = 0;

        // 패턴의 접미부와 일치하는 부분 찾기
        while (j >= 0 && pattern[j] == pattern[m - 1 - len]) {
            len++;
            suffix[len] = j;
            j--;
        }

        if (j == -1)  // 전체가 일치하는 경우
            prefix[len] = true;
    }
}

/* Good Suffix 규칙에 따른 이동 거리 계산 */
int get_good_suffix_shift(int pos, int m, const int suffix[], const bool prefix[]) {
    int len = m - 1 - pos;  // 일치하는 접미부 길이

    // Case 1: 접미부가 다시 나타나는 경우
    if (suffix[len] != -1)
        return pos - suffix[len] + 1;

    // Case 2: 더 짧은 접미부가 접두부로 나타나는 경우
    for (int r = pos + 2; r < m; r++) {
        if (prefix[m - r])
            return r;
    }

    // Case 3: 아무것도 찾지 못한 경우
    return m;
}

/* Boyer-Moore 문자열 검색
 * - 시간복잡도: 최선 O(n/m), 평균 O(n), 최악 O(nm)
 */
SearchResult* boyer_moore_search(const char* text, const char* pattern, bool print_steps) {
    SearchResult* result = create_result(10);
    if (!result) return NULL;

    int n = strlen(text);
    int m = strlen(pattern);
    int comparisons = 0;

    // Bad Character 규칙 테이블 생성
    int bad_char[NO_OF_CHARS];
    compute_bad_char(pattern, m, bad_char);

    // Good Suffix 규칙 테이블 생성
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

    int s = 0;  // 패턴의 시작 위치
    while (s <= n - m) {
        int j = m - 1;  // 패턴의 마지막 문자부터 비교

        if (print_steps) {
            printf("\nCurrent position: %d\n", s);
            printf("Text:    %s\n", text);
            printf("Pattern: %*s%s\n", s, "", pattern);
        }

        // 패턴과 텍스트 비교
        while (j >= 0 && pattern[j] == text[s + j]) {
            comparisons++;
            if (print_steps) {
                printf("Match at position %d\n", s + j);
            }
            j--;
        }

        if (j < 0) {  // 패턴을 찾음
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

            // Bad Character와 Good Suffix 규칙 중 더 큰 이동 거리 선택
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

/* 검색 결과 출력 */
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

/* 메뉴 출력 */
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
        getchar();  // 버퍼 비우기

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
상세 설명 및 주요 개념
==========================================

1. Boyer-Moore 알고리즘의 특징
------------------------
- 뒤에서 앞으로 비교
- 두 가지 이동 규칙 사용
- 실제 응용에서 가장 빠름
- 전처리 단계 필요

2. Bad Character 규칙
-----------------
원리:
- 불일치 문자의 마지막 위치 활용
- 패턴에 없는 문자는 패턴 길이만큼 이동
- 있다면 마지막 위치까지 이동

장점:
- 구현이 단순
- 큰 알파벳에서 효과적
- 빠른 이동 가능

3. Good Suffix 규칙
---------------
원리:
- 일치한 접미부 활용
- 다시 나타나는 위치로 이동
- 더 짧은 접미부도 고려

장점:
- 추가적인 이동 기회 제공
- 패턴 내부 정보 활용
- Bad Character 보완

4. 시간 복잡도
-----------
전처리: O(m + σ)
- m: 패턴 길이
- σ: 알파벳 크기

검색:
- 최선: O(n/m)
- 평균: O(n)
- 최악: O(nm)

5. 공간 복잡도
-----------
O(m + σ)
- Bad Character 테이블
- Good Suffix 테이블
- 추가 메모리 필요

6. 장단점
-------
장점:
- 실제로 가장 빠름
- 긴 패턴에 효율적
- 서브리니어 성능 가능

단점:
- 구현이 복잡
- 전처리 오버헤드
- 짧은 패턴에 비효율
- 추가 메모리 필요

이 구현은 Boyer-Moore 알고리즘의
두 가지 주요 규칙을 모두 포함하며,
실제 검색 과정을 단계별로
확인할 수 있습니다.
*/
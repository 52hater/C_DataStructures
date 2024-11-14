#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*
Rabin-Karp 알고리즘:
- 해시 함수를 사용한 문자열 검색
- 롤링 해시로 연속된 부분 문자열의 해시값을 효율적으로 계산
- 다중 패턴 검색에 효과적
- 해시 충돌 처리 필요

특징:
- 해시값 비교 후 실제 문자열 비교
- 윈도우를 한 칸씩 이동하며 해시값 갱신
- 모듈러 연산으로 오버플로우 방지
*/

#define PRIME 101  // 해시 계산을 위한 소수
#define D 256     // 문자셋 크기 (ASCII)

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

/* 거듭제곱 계산 (모듈러 연산 포함)
 * - (a^b) % mod 를 효율적으로 계산
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

/* Rabin-Karp 문자열 검색
 * - 시간복잡도: 평균 O(n+m), 최악 O(nm)
 */
SearchResult* rabin_karp_search(const char* text, const char* pattern, bool print_steps) {
    SearchResult* result = create_result(10);
    if (!result) return NULL;

    int n = strlen(text);
    int m = strlen(pattern);
    unsigned long long pattern_hash = 0;
    unsigned long long text_hash = 0;
    unsigned long long h = mod_pow(D, m - 1, PRIME);  // D^(m-1) % PRIME
    int comparisons = 0;  // 비교 횟수 카운트

    // 패턴과 첫 윈도우의 해시값 계산
    for (int i = 0; i < m; i++) {
        pattern_hash = (D * pattern_hash + pattern[i]) % PRIME;
        text_hash = (D * text_hash + text[i]) % PRIME;
    }

    if (print_steps) {
        printf("\nInitial hash values:\n");
        printf("Pattern hash: %llu\n", pattern_hash);
        printf("First window hash: %llu\n", text_hash);
    }

    // 슬라이딩 윈도우로 검색
    for (int i = 0; i <= n - m; i++) {
        if (print_steps) {
            printf("\nWindow at position %d:\n", i);
            printf("Text:    %s\n", text);
            printf("Window:  %*s%.*s\n", i, "", m, &text[i]);
            printf("Pattern: %*s%s\n", i, "", pattern);
            printf("Current window hash: %llu\n", text_hash);
        }

        // 해시값이 같은 경우 실제 문자열 비교
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

        // 다음 윈도우의 해시값 계산
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
상세 설명 및 주요 개념
==========================================

1. Rabin-Karp 알고리즘의 원리
-----------------------
- 해시 기반 문자열 검색
- 롤링 해시 기법 사용
- 해시값 비교 후 실제 비교
- 모듈러 연산으로 오버플로우 방지

2. 해시 함수
---------
구성:
- D: 기수(문자셋 크기)
- PRIME: 모듈러 연산용 소수
- 롤링 해시로 효율적 계산

특징:
- 충돌 가능성 존재
- 빠른 계산 가능
- 모듈러 연산 필요

3. 시간 복잡도
-----------
전처리: O(m)
- 패턴 해시값 계산
- h = D^(m-1) 계산

검색:
- 평균: O(n + m)
- 최악: O(nm)
- 실제로는 거의 선형

4. 공간 복잡도
-----------
O(1)
- 상수 공간만 필요
- 추가 메모리 최소화
- 해시값 저장만 필요

5. 특징과 장단점
------------
장점:
- 다중 패턴 검색 가능
- 평균적으로 빠름
- 추가 메모리 적음
- 전처리 간단

단점:
- 해시 충돌 가능성
- 복잡한 수식 사용
- 오버플로우 주의
- 최악의 경우 느림

6. 활용 분야
----------
- 표절 검사
- 다중 패턴 검색
- DNA 서열 분석
- 파일 중복 검사

이 구현은 Rabin-Karp 알고리즘의
핵심인 롤링 해시 기법을 보여주며,
해시 계산 과정을 단계별로
확인할 수 있습니다.
*/
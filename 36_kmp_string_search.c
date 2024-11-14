#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*
KMP(Knuth-Morris-Pratt) 알고리즘:
- 접두사와 접미사의 정보를 활용
- 실패 함수(부분일치 테이블)를 미리 계산
- 불일치 발생 시 패턴을 효율적으로 이동
- 중복 비교를 피하여 성능 향상

부분일치 테이블(failure function):
- 각 위치에서 일치하는 접두사=접미사 길이
- 불일치 발생 시 어디로 돌아갈지 결정
- 예: "ABCDABD"의 테이블: [0,0,0,0,1,2,0]
*/

typedef struct {
    int* positions;    // 발견된 위치들의 배열
    int count;         // 발견된 총 횟수
    int capacity;      // positions 배열의 크기
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
        int* new_positions = (int*)realloc(result->positions, new_capacity * sizeof(int));
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

/* 부분일치 테이블 계산
 * - 시간복잡도: O(m), m: 패턴의 길이
 */
void compute_failure_function(const char* pattern, int* failure, bool print_steps) {
    int m = strlen(pattern);
    failure[0] = 0;  // 첫 번째 문자는 항상 0

    // j: 현재까지 일치한 길이
    // i: 다음 비교할 위치
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

/* KMP 문자열 검색
 * - 시간복잡도: O(n + m), n: 텍스트 길이, m: 패턴 길이
 */
SearchResult* kmp_search(const char* text, const char* pattern, bool print_steps) {
    SearchResult* result = create_result(10);
    if (!result) return NULL;

    int n = strlen(text);
    int m = strlen(pattern);
    int comparisons = 0;  // 비교 횟수 카운트

    // 부분일치 테이블 계산
    int* failure = (int*)malloc(m * sizeof(int));
    if (!failure) {
        destroy_result(result);
        return NULL;
    }

    compute_failure_function(pattern, failure, print_steps);

    if (print_steps) {
        printf("\nStarting KMP search:\n");
    }

    int i = 0;  // 텍스트 인덱스
    int j = 0;  // 패턴 인덱스

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

            if (j == m - 1) {  // 패턴을 찾음
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

        // 매칭된 부분 전체 문자열 출력
        printf("....");
        for (int j = pos - 4 >= 0 ? pos - 4 : 0; j < pos; j++) {
            printf("%c", text[j]);
        }
        printf("[%.*s]", (int)strlen(pattern), &text[pos]);
        for (int j = pos + strlen(pattern); j < pos + strlen(pattern) + 4 && text[j]; j++) {
            printf("%c", text[j]);
        }
        printf("....\n");

        // 위치 표시
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
상세 설명 및 주요 개념
==========================================

1. KMP 알고리즘의 핵심 아이디어
--------------------------
- 이미 비교한 정보 재활용
- 접두사-접미사 관계 활용
- 실패 함수로 중복 비교 제거
- 효율적인 패턴 이동

2. 실패 함수(부분일치 테이블)
-------------------------
목적:
- 불일치 발생 시 이동할 위치 결정
- 이전 비교 결과 활용
- 중복 비교 회피

계산 방법:
- 접두사=접미사 길이 계산
- 점진적 확장
- O(m) 시간 소요

3. 시간 복잡도 분석
---------------
전처리: O(m)
- 실패 함수 계산
- 패턴 길이에 비례

검색: O(n)
- 텍스트 길이에 비례
- 각 문자 최대 1번 비교

전체: O(n + m)
- 선형 시간 복잡도
- 브루트 포스보다 효율적

4. 공간 복잡도
-----------
O(m)
- 실패 함수 저장
- 패턴 길이에 비례
- 추가 공간 필요

5. 장단점
-------
장점:
- 선형 시간 복잡도
- 최악의 경우도 보장
- 안정적인 성능
- 텍스트 수정 불필요

단점:
- 전처리 필요
- 추가 메모리 사용
- 구현 복잡
- 짧은 패턴에 오버헤드

6. 활용 분야
----------
- 텍스트 편집기
- 바이너리 파일 검색
- DNA 서열 분석
- 네트워크 패킷 검사

이 구현은 KMP 알고리즘의 핵심인
실패 함수 계산과 검색 과정을
단계별로 보여주며, 브루트 포스
대비 성능 향상을 설명합니다.
*/
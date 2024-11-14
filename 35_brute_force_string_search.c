#include <stdio.h>
#include <string.h>
#include <stdbool.h>

/*
고지식한 탐색(Brute Force) 알고리즘:
- 문자열 매칭의 가장 기본적인 알고리즘
- 텍스트의 모든 위치에서 패턴을 비교
- 한 칸씩 이동하면서 순차적으로 비교
- 구현이 단순하고 이해하기 쉬움
*/

/* 문자열 탐색 결과 저장을 위한 구조체 */
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

/* 기본 브루트 포스 문자열 검색
 * - 시간복잡도: O(mn), m: 패턴길이, n: 텍스트길이
 * - 공간복잡도: O(1)
 */
SearchResult* brute_force_search(const char* text, const char* pattern, bool print_steps) {
    SearchResult* result = create_result(10);  // 초기 용량 10
    if (!result) return NULL;

    int n = strlen(text);
    int m = strlen(pattern);
    int comparisons = 0;  // 비교 횟수 카운트

    // 텍스트의 각 위치에서 시도
    for (int i = 0; i <= n - m; i++) {
        bool match = true;

        if (print_steps) {
            printf("\nTrying position %d:\n", i);
            printf("Text:    %s\n", text);
            printf("Pattern: %*s%s\n", i, "", pattern);
        }

        // 현재 위치에서 패턴과 비교
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
상세 설명 및 주요 개념
==========================================

1. 브루트 포스 알고리즘의 원리
-------------------------
- 가능한 모든 위치 검사
- 한 칸씩 이동하며 비교
- 완전 탐색 방식
- 거의 무조건 해를 찾음

2. 시간 복잡도 분석
---------------
최악의 경우: O(mn)
- 모든 위치에서 패턴 길이만큼 비교
- n: 텍스트 길이
- m: 패턴 길이

평균의 경우:
- 실제로는 더 적은 비교
- 불일치 발생 시 조기 종료

3. 장단점
-------
장점:
- 구현이 단순
- 이해하기 쉬움
- 작은 텍스트에 효과적
- 전처리 불필요

단점:
- 비효율적인 성능
- 많은 중복 비교
- 긴 텍스트에 부적합
- 패턴 정보 미활용

4. 활용 분야
----------
- 짧은 문자열 검색
- 단순한 텍스트 에디터
- 교육용 예제
- 다른 알고리즘의 기준

5. 최적화 가능성
------------
- 여러 문자 동시 비교
- SIMD 명령어 활용
- 캐시 지역성 개선
- 병렬화 가능

6. 교육적 가치
-----------
- 기본 개념 이해
- 성능 분석 학습
- 최적화 필요성
- 알고리즘 비교

이 구현은 브루트 포스 방식의
문자열 검색을 이해하기 쉽게
보여주며, 다음 단계인 KMP
알고리즘의 필요성을 설명합니다.
*/
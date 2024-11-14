#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*
최장 공통 부분수열 (LCS: Longest Common Subsequence)
================================================

특징:
1. 두 수열의 공통 부분수열 중 가장 긴 것을 찾는 문제
2. 부분수열은 연속적일 필요 없음 (Substring과 다름)
3. 동적 계획법의 대표적인 예제

예시:
- 문자열 1: "ABCDE"
- 문자열 2: "ACE"
- LCS: "ACE" (길이 3)
*/

// === LCS 계산 결과를 저장하는 구조체 ===
typedef struct {
    int** length;    // LCS 길이 테이블
    char** path;     // 경로 추적용 방향 테이블
    int rows;        // 행 수 (첫 번째 문자열 길이 + 1)
    int cols;        // 열 수 (두 번째 문자열 길이 + 1)
} LCSTable;

// === LCS 결과를 저장하는 구조체 ===
typedef struct {
    char* sequence;  // 실제 LCS 문자열
    int length;      // LCS 길이
} LCSResult;

// === LCS 테이블 생성 ===
LCSTable* create_lcs_table(int rows, int cols) {
    LCSTable* table = (LCSTable*)malloc(sizeof(LCSTable));
    table->rows = rows;
    table->cols = cols;

    // 길이 테이블 할당
    table->length = (int**)malloc(rows * sizeof(int*));
    table->path = (char**)malloc(rows * sizeof(char*));

    for (int i = 0; i < rows; i++) {
        table->length[i] = (int*)calloc(cols, sizeof(int));
        table->path[i] = (char*)malloc(cols * sizeof(char));
    }

    return table;
}

// === LCS 테이블 메모리 해제 ===
void destroy_lcs_table(LCSTable* table) {
    for (int i = 0; i < table->rows; i++) {
        free(table->length[i]);
        free(table->path[i]);
    }
    free(table->length);
    free(table->path);
    free(table);
}

// === LCS 테이블 출력 (디버깅/교육용) ===
void print_lcs_table(LCSTable* table, const char* str1, const char* str2) {
    printf("\n=== LCS 테이블 ===\n");
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

// === LCS 계산 ===
LCSTable* compute_lcs(const char* str1, const char* str2, bool print_steps) {
    int m = strlen(str1);
    int n = strlen(str2);
    LCSTable* table = create_lcs_table(m + 1, n + 1);

    if (print_steps) {
        printf("\n=== LCS 계산 과정 ===\n");
    }

    // Bottom-up 방식으로 테이블 채우기
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

// === LCS 추출 (재귀적 방법) ===
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

// === LCS 결과 생성 ===
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

// === LCS 결과 메모리 해제 ===
void destroy_lcs_result(LCSResult* result) {
    free(result->sequence);
    free(result);
}

// === LCS 결과 시각화 ===
void visualize_lcs(const char* str1, const char* str2, LCSResult* result) {
    printf("\n=== LCS 시각화 ===\n");
    printf("문자열 1: %s\n", str1);
    printf("문자열 2: %s\n", str2);
    printf("LCS: %s (길이: %d)\n\n", result->sequence, result->length);

    // 문자열 1에서 LCS 문자 하이라이트
    printf("문자열 1에서 LCS: ");
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

    // 문자열 2에서 LCS 문자 하이라이트
    printf("문자열 2에서 LCS: ");
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
    printf("=== 최장 공통 부분수열(LCS) 계산 프로그램 ===\n");

    char str1[100], str2[100];
    bool print_steps;

    while (1) {
        printf("\n1. LCS 계산\n");
        printf("2. 종료\n");
        printf("선택: ");

        int choice;
        scanf("%d", &choice);
        getchar();  // 버퍼 비우기

        if (choice == 2) break;

        if (choice == 1) {
            printf("\n첫 번째 문자열 입력: ");
            fgets(str1, sizeof(str1), stdin);
            str1[strcspn(str1, "\n")] = '\0';

            printf("두 번째 문자열 입력: ");
            fgets(str2, sizeof(str2), stdin);
            str2[strcspn(str2, "\n")] = '\0';

            printf("계산 과정을 출력하시겠습니까? (1: 예, 0: 아니오): ");
            scanf("%d", &print_steps);

            // LCS 계산
            LCSTable* table = compute_lcs(str1, str2, print_steps);
            LCSResult* result = get_lcs_result(table, str1, str2);

            // 결과 출력
            visualize_lcs(str1, str2, result);

            // 메모리 해제
            destroy_lcs_table(table);
            destroy_lcs_result(result);
        }
    }

    return 0;
}

/*
LCS 알고리즘 분석
===============

1. 시간 복잡도
-----------
- O(mn): m, n은 각 문자열의 길이
- 2중 루프로 모든 부분 문제 해결
- 경로 추적은 O(m+n)

2. 공간 복잡도
-----------
- O(mn): 2차원 배열 사용
- 길이 테이블과 경로 테이블 필요
- 최적화 가능성 있음

3. 최적화 가능성
------------
공간 최적화:
- O(min(m,n)) 공간으로 개선 가능
- 한 행만 저장하는 방식
- 경로 추적 포기 시 가능

시간 최적화:
- 병렬화 가능
- 분할 정복 접근
- 특수 케이스 최적화

4. 응용 분야
---------
- 유전자 서열 분석
- 파일 비교 (diff)
- 표절 검사
- 자연어 처리
- 버전 관리 시스템

5. 구현 시 고려사항
--------------
메모리 관리:
- 동적 할당/해제 주의
- 큰 문자열 처리
- 메모리 누수 방지

예외 처리:
- 빈 문자열 처리
- 메모리 할당 실패
- 입력 크기 제한

시각화:
- 단계별 진행 표시
- 결과 하이라이트
- 교육용 출력

이 구현은 LCS의 기본 개념부터
실제 응용까지 포괄하며, 교육과
실무에서 모두 활용 가능합니다.
*/
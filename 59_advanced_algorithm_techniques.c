```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

/*
알고리즘 분류: 알고리즘 설계 기법
하위 분류: 동적 계획법 심화
난이도: 상
관련 알고리즘: 분할 정복, 그리디

고급 동적 계획법 예제:
1. 편집 거리 (Edit Distance)
2. 행렬 체인 곱셈 (Matrix Chain Multiplication)
3. 최적 이진 탐색 트리 (Optimal BST)
*/

// === 편집 거리 (Edit Distance) ===
typedef struct {
    int** dp;         // DP 테이블
    char** ops;       // 연산 추적 테이블
    int m, n;         // 문자열 길이
} EditDistance;

// 최소값 반환
int min3(int a, int b, int c) {
    int min = a;
    if (b < min) min = b;
    if (c < min) min = c;
    return min;
}

// 편집 거리 초기화
EditDistance* init_edit_distance(int m, int n) {
    EditDistance* ed = (EditDistance*)malloc(sizeof(EditDistance));
    ed->m = m;
    ed->n = n;

    ed->dp = (int**)malloc((m + 1) * sizeof(int*));
    ed->ops = (char**)malloc((m + 1) * sizeof(char*));

    for (int i = 0; i <= m; i++) {
        ed->dp[i] = (int*)malloc((n + 1) * sizeof(int));
        ed->ops[i] = (char*)malloc((n + 1) * sizeof(char));
    }

    return ed;
}

// 편집 거리 계산
int edit_distance(char* str1, char* str2, bool print_steps) {
    int m = strlen(str1);
    int n = strlen(str2);
    EditDistance* ed = init_edit_distance(m, n);

    // 기저 사례 초기화
    for (int i = 0; i <= m; i++) {
        ed->dp[i][0] = i;
        ed->ops[i][0] = 'D';  // Delete
    }

    for (int j = 0; j <= n; j++) {
        ed->dp[0][j] = j;
        ed->ops[0][j] = 'I';  // Insert
    }

    // DP 테이블 채우기
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (str1[i - 1] == str2[j - 1]) {
                ed->dp[i][j] = ed->dp[i - 1][j - 1];
                ed->ops[i][j] = 'M';  // Match
            }
            else {
                ed->dp[i][j] = 1 + min3(
                    ed->dp[i - 1][j],    // 삭제
                    ed->dp[i][j - 1],    // 삽입
                    ed->dp[i - 1][j - 1]   // 대체
                );

                if (ed->dp[i][j] == 1 + ed->dp[i - 1][j])
                    ed->ops[i][j] = 'D';
                else if (ed->dp[i][j] == 1 + ed->dp[i][j - 1])
                    ed->ops[i][j] = 'I';
                else
                    ed->ops[i][j] = 'R';  // Replace
            }

            if (print_steps) {
                printf("dp[%d][%d] = %d (%c)\n",
                    i, j, ed->dp[i][j], ed->ops[i][j]);
            }
        }
    }

    // 편집 순서 출력
    if (print_steps) {
        printf("\n편집 순서:\n");
        int i = m, j = n;
        while (i > 0 || j > 0) {
            switch (ed->ops[i][j]) {
            case 'M':
                printf("유지: %c\n", str1[i - 1]);
                i--; j--;
                break;
            case 'D':
                printf("삭제: %c\n", str1[i - 1]);
                i--;
                break;
            case 'I':
                printf("삽입: %c\n", str2[j - 1]);
                j--;
                break;
            case 'R':
                printf("대체: %c -> %c\n", str1[i - 1], str2[j - 1]);
                i--; j--;
                break;
            }
        }
    }

    int result = ed->dp[m][n];

    // 메모리 해제
    for (int i = 0; i <= m; i++) {
        free(ed->dp[i]);
        free(ed->ops[i]);
    }
    free(ed->dp);
    free(ed->ops);
    free(ed);

    return result;
}

// === 행렬 체인 곱셈 ===
typedef struct {
    int** dp;         // DP 테이블
    int** bracket;    // 괄호 위치 테이블
    int n;           // 행렬 개수
} MatrixChain;

// 행렬 체인 초기화
MatrixChain* init_matrix_chain(int n) {
    MatrixChain* mc = (MatrixChain*)malloc(sizeof(MatrixChain));
    mc->n = n;

    mc->dp = (int**)malloc(n * sizeof(int*));
    mc->bracket = (int**)malloc(n * sizeof(int*));

    for (int i = 0; i < n; i++) {
        mc->dp[i] = (int*)malloc(n * sizeof(int));
        mc->bracket[i] = (int*)malloc(n * sizeof(int));
        for (int j = 0; j < n; j++) {
            mc->dp[i][j] = 0;
            mc->bracket[i][j] = 0;
        }
    }

    return mc;
}

// 괄호 위치 출력
void print_parenthesis(MatrixChain* mc, int i, int j) {
    if (i == j) {
        printf("A%d", i);
        return;
    }

    printf("(");
    print_parenthesis(mc, i, mc->bracket[i][j]);
    print_parenthesis(mc, mc->bracket[i][j] + 1, j);
    printf(")");
}

// 행렬 체인 곱셈 최적화
int matrix_chain_mult(int* dimensions, int n, bool print_steps) {
    MatrixChain* mc = init_matrix_chain(n);

    // len은 체인의 길이
    for (int len = 2; len < n; len++) {
        for (int i = 1; i < n - len + 1; i++) {
            int j = i + len - 1;
            mc->dp[i][j] = INT_MAX;

            // k는 분할 위치
            for (int k = i; k < j; k++) {
                int cost = mc->dp[i][k] + mc->dp[k + 1][j] +
                    dimensions[i - 1] * dimensions[k] * dimensions[j];

                if (cost < mc->dp[i][j]) {
                    mc->dp[i][j] = cost;
                    mc->bracket[i][j] = k;
                }

                if (print_steps) {
                    printf("dp[%d][%d] = %d (k=%d)\n", i, j, cost, k);
                }
            }
        }
    }

    if (print_steps) {
        printf("\n최적의 괄호 배치:\n");
        print_parenthesis(mc, 1, n - 1);
        printf("\n");
    }

    int result = mc->dp[1][n - 1];

    // 메모리 해제
    for (int i = 0; i < n; i++) {
        free(mc->dp[i]);
        free(mc->bracket[i]);
    }
    free(mc->dp);
    free(mc->bracket);
    free(mc);

    return result;
}

// === 최적 이진 탐색 트리 ===
typedef struct {
    double** dp;      // DP 테이블
    int** root;       // 루트 선택 테이블
    int n;           // 키의 개수
} OptimalBST;

// 최적 BST 초기화
OptimalBST* init_optimal_bst(int n) {
    OptimalBST* obst = (OptimalBST*)malloc(sizeof(OptimalBST));
    obst->n = n;

    obst->dp = (double**)malloc((n + 1) * sizeof(double*));
    obst->root = (int**)malloc(n * sizeof(int*));

    for (int i = 0; i <= n; i++) {
        obst->dp[i] = (double*)calloc(n + 1, sizeof(double));
    }

    for (int i = 0; i < n; i++) {
        obst->root[i] = (int*)malloc(n * sizeof(int));
    }

    return obst;
}

// 최적 BST 계산
double optimal_bst(int* keys, double* freq, int n, bool print_steps) {
    OptimalBST* obst = init_optimal_bst(n);
    double* sum = (double*)calloc(n + 1, sizeof(double));

    // 누적 빈도수 계산
    for (int i = 0; i < n; i++) {
        sum[i + 1] = sum[i] + freq[i];
    }

    // 길이 1인 서브트리 초기화
    for (int i = 0; i < n; i++) {
        obst->dp[i][i + 1] = freq[i];
        obst->root[i][i] = i;
    }

    // 길이 2 이상의 서브트리
    for (int len = 2; len <= n; len++) {
        for (int i = 0; i <= n - len; i++) {
            int j = i + len;
            obst->dp[i][j] = INT_MAX;

            for (int r = i; r < j; r++) {
                double cost = obst->dp[i][r] + obst->dp[r + 1][j] +
                    sum[j] - sum[i];

                if (cost < obst->dp[i][j]) {
                    obst->dp[i][j] = cost;
                    obst->root[i][j - 1] = r;
                }

                if (print_steps) {
                    printf("dp[%d][%d] = %.2f (root=%d)\n",
                        i, j, cost, r);
                }
            }
        }
    }

    if (print_steps) {
        printf("\n최적 BST의 루트 선택:\n");
        for (int i = 0; i < n; i++) {
            for (int j = i; j < n; j++) {
                printf("서브트리 [%d-%d]의 루트: %d\n",
                    i, j, obst->root[i][j]);
            }
        }
    }

    double result = obst->dp[0][n];

    // 메모리 해제
    for (int i = 0; i <= n; i++) {
        free(obst->dp[i]);
    }
    for (int i = 0; i < n; i++) {
        free(obst->root[i]);
    }
    free(obst->dp);
    free(obst->root);
    free(obst);
    free(sum);

    return result;
}

int main(void) {
    int choice;

    while (1) {
        printf("\n=== 고급 동적 계획법 ===\n");
        printf("1. 편집 거리\n");
        printf("2. 행렬 체인 곱셈\n");
        printf("3. 최적 이진 탐색 트리\n");
        printf("0. 종료\n");
        printf("선택: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1: {
            char str1[100], str2[100];
            printf("\n첫 번째 문자열: ");
            scanf("%s", str1);
            printf("두 번째 문자열: ");
            scanf("%s", str2);

            int distance = edit_distance(str1, str2, true);
            printf("\n최소 편집 거리: %d\n", distance);
            break;
        }

        case 2: {
            int n;
            printf("\n행렬 개수: ");
            scanf("%d", &n);

            int* dimensions = (int*)malloc((n + 1) * sizeof(int));
            printf("각 행렬의 차원 입력:\n");
            for (int i = 0; i <= n; i++) {
                scanf("%d", &dimensions[i]);
            }

            int min_mult = matrix_chain_mult(dimensions, n + 1, true);
            printf("\n최소 곱셈 횟수: %d\n", min_mult);

            free(dimensions);
            break;
        }

        case 3: {
            int n;
            printf("\n키 개수: ");
            scanf("%d", &n);

            int* keys = (int*)malloc(n * sizeof(int));
            double* freq = (double*)malloc(n * sizeof(double));

            printf("키와 빈도수 입력:\n");
            for (int i = 0; i < n; i++) {
                scanf("%d %lf", &keys[i], &freq[i]);
            }

            double cost = optimal_bst(keys, freq, n, true);
            printf("\n최적 BST의 기대 검색 비용: %.2f\n", cost);

            free(keys);
            free(freq);
            break;
        }

        case 0:
            return 0;

        default:
            printf("잘못된 선택\n");
        }
    }

    return 0;
}

/*
고급 동적 계획법 분석
================

1. 편집 거리
---------
시간 복잡도:
- O(mn)

공간 복잡도:
- O(mn)

응용:
- 맞춤법 검사
- DNA 서열 비교
- 문자열 유사도

2. 행렬 체인 곱셈
---------
시간 복잡도:
- O(n³)

공간 복잡도:
-O(n²)

응용:
- 행렬 연산 최적화
- 연관 배열 계산
- 그래픽스 파이프라인

3. 최적 이진 탐색 트리
----------

시간 복잡도:
- O(n³)

공간 복잡도:
- O(n²)

응용:
- 검색 엔진
- 컴파일러 심볼 테이블
- 데이터베이스 인덱싱

일반적 교훈

1. 상태 설계의 중요성
- 상태 정의가 성능 결정
- 메모리 사용량 고려
- 갱신 순서 최적화


2. 부분 문제 중복
- 메모이제이션 활용
- 테이블 재사용
- 공간 복잡도 개선

3. 최적 부분 구조
- 재귀적 구조 활용
- 부분해 조합 방법
- 전역 최적해 보장

4. 구현 최적화
- 캐시 지역성
- 메모리 관리
- 반복문 최적화

5. 실제 응용
- 실시간 처리
- 대규모 데이터
- 정확성 vs 효율성

이 구현들은 동적 계획법의
고급 응용을 보여주며,
실제 시스템에서 사용되는
최적화 기법들을 포함합니다.
*/
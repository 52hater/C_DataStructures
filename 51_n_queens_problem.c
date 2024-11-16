#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
8-Queen 문제:
- 8x8 체스판에 8개의 퀸을 배치
- 서로 공격할 수 없는 위치 찾기
- 백트래킹의 대표적 예제
*/

typedef struct {
    int* board;       // 체스판 (각 열의 퀸 위치)
    int size;         // 보드 크기
    int solutions;    // 찾은 해답 수
} ChessBoard;

// 체스판 생성
ChessBoard* create_board(int size) {
    ChessBoard* board = (ChessBoard*)malloc(sizeof(ChessBoard));
    board->board = (int*)calloc(size, sizeof(int));
    board->size = size;
    board->solutions = 0;
    return board;
}

// 체스판 출력
void print_board(ChessBoard* board, int solution_number) {
    printf("\n=== 해답 #%d ===\n", solution_number);
    printf("   ");
    for (int i = 0; i < board->size; i++)
        printf(" %d", i);
    printf("\n");

    for (int i = 0; i < board->size; i++) {
        printf("%2d ", i);
        for (int j = 0; j < board->size; j++) {
            printf(" %c", board->board[i] == j ? 'Q' : '.');
        }
        printf("\n");
    }
}

// 유효성 검사
bool is_safe(ChessBoard* board, int row, int col) {
    // 같은 행 검사는 불필요 (각 행에 하나의 퀸만 배치)

    // 같은 열 검사
    for (int i = 0; i < row; i++) {
        if (board->board[i] == col)
            return false;
    }

    // 왼쪽 대각선 검사
    for (int i = row - 1, j = col - 1; i >= 0 && j >= 0; i--, j--) {
        if (board->board[i] == j)
            return false;
    }

    // 오른쪽 대각선 검사
    for (int i = row - 1, j = col + 1; i >= 0 && j < board->size; i--, j++) {
        if (board->board[i] == j)
            return false;
    }

    return true;
}

// 모든 해 찾기
void solve_n_queens_util(ChessBoard* board, int row, bool show_steps) {
    // 모든 행을 처리했으면 해답 출력
    if (row == board->size) {
        board->solutions++;
        print_board(board, board->solutions);
        return;
    }

    // 현재 행의 각 열에 퀸 배치 시도
    for (int col = 0; col < board->size; col++) {
        if (is_safe(board, row, col)) {
            board->board[row] = col;

            if (show_steps) {
                printf("\n현재 상태 (행 %d, 열 %d):\n", row, col);
                print_board(board, 0);
            }

            solve_n_queens_util(board, row + 1, show_steps);

            // 백트래킹
            board->board[row] = 0;

            if (show_steps) {
                printf("\n백트래킹 (행 %d에서):\n", row);
                print_board(board, 0);
            }
        }
    }
}

// N-Queen 문제 해결
void solve_n_queens(ChessBoard* board, bool show_steps) {
    solve_n_queens_util(board, 0, show_steps);
    printf("\n총 %d개의 해답을 찾았습니다.\n", board->solutions);
}

// 메모리 해제
void destroy_board(ChessBoard* board) {
    free(board->board);
    free(board);
}

// 대칭 해 생성
void generate_symmetric_solutions(ChessBoard* board) {
    printf("\n=== 대칭 해답 생성 ===\n");

    // 수평 대칭
    printf("\n수평 대칭:\n");
    for (int i = 0; i < board->size; i++) {
        int temp = board->board[i];
        board->board[i] = board->size - 1 - temp;
    }
    print_board(board, board->solutions + 1);

    // 수직 대칭
    printf("\n수직 대칭:\n");
    for (int i = 0; i < board->size / 2; i++) {
        int temp = board->board[i];
        board->board[i] = board->board[board->size - 1 - i];
        board->board[board->size - 1 - i] = temp;
    }
    print_board(board, board->solutions + 2);
}

int main(void) {
    int size;
    bool show_steps;

    printf("체스판 크기 입력 (N-Queen 문제): ");
    scanf("%d", &size);

    printf("풀이 과정을 보시겠습니까? (1: 예, 0: 아니오): ");
    scanf("%d", &show_steps);

    ChessBoard* board = create_board(size);

    printf("\n=== %d-Queen 문제 해결 시작 ===\n", size);
    solve_n_queens(board, show_steps);

    if (board->solutions > 0) {
        char generate_sym;
        printf("\n첫 번째 해답에 대한 대칭 해답을 생성하시겠습니까? (y/n): ");
        scanf(" %c", &generate_sym);

        if (generate_sym == 'y' || generate_sym == 'Y') {
            generate_symmetric_solutions(board);
        }
    }

    destroy_board(board);
    return 0;
}

/*
N-Queen 문제의 특성
===============

1. 문제의 특징
----------
- 제약 충족 문제
- 대칭성 존재
- 해의 개수: 8-Queen의 경우 92개
- 브루트 포스로는 불가능

2. 백트래킹 적용
------------
- 열 단위 배치
- 유망성 검사
- 즉각적인 가지치기
- 상태 복원

3. 시간 복잡도
----------
- 최악: O(N!)
- 가지치기로 실제로는 더 효율적
- 크기에 따라 기하급수적 증가

4. 최적화 기법
----------
- 대칭성 활용
- 첫 행 최적화
- 비트마스크 활용
- 병렬화 가능

이 구현은 백트래킹의 고급
응용을 보여주며, 제약 조건과
상태 공간 탐색의 효율성을
이해하는 데 도움이 됩니다.
*/
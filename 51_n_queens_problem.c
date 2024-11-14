#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
8-Queen ����:
- 8x8 ü���ǿ� 8���� ���� ��ġ
- ���� ������ �� ���� ��ġ ã��
- ��Ʈ��ŷ�� ��ǥ�� ����
*/

typedef struct {
    int* board;       // ü���� (�� ���� �� ��ġ)
    int size;         // ���� ũ��
    int solutions;    // ã�� �ش� ��
} ChessBoard;

// ü���� ����
ChessBoard* create_board(int size) {
    ChessBoard* board = (ChessBoard*)malloc(sizeof(ChessBoard));
    board->board = (int*)calloc(size, sizeof(int));
    board->size = size;
    board->solutions = 0;
    return board;
}

// ü���� ���
void print_board(ChessBoard* board, int solution_number) {
    printf("\n=== �ش� #%d ===\n", solution_number);
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

// ��ȿ�� �˻�
bool is_safe(ChessBoard* board, int row, int col) {
    // ���� �� �˻�� ���ʿ� (�� �࿡ �ϳ��� ���� ��ġ)

    // ���� �� �˻�
    for (int i = 0; i < row; i++) {
        if (board->board[i] == col)
            return false;
    }

    // ���� �밢�� �˻�
    for (int i = row - 1, j = col - 1; i >= 0 && j >= 0; i--, j--) {
        if (board->board[i] == j)
            return false;
    }

    // ������ �밢�� �˻�
    for (int i = row - 1, j = col + 1; i >= 0 && j < board->size; i--, j++) {
        if (board->board[i] == j)
            return false;
    }

    return true;
}

// ��� �� ã��
void solve_n_queens_util(ChessBoard* board, int row, bool show_steps) {
    // ��� ���� ó�������� �ش� ���
    if (row == board->size) {
        board->solutions++;
        print_board(board, board->solutions);
        return;
    }

    // ���� ���� �� ���� �� ��ġ �õ�
    for (int col = 0; col < board->size; col++) {
        if (is_safe(board, row, col)) {
            board->board[row] = col;

            if (show_steps) {
                printf("\n���� ���� (�� %d, �� %d):\n", row, col);
                print_board(board, 0);
            }

            solve_n_queens_util(board, row + 1, show_steps);

            // ��Ʈ��ŷ
            board->board[row] = 0;

            if (show_steps) {
                printf("\n��Ʈ��ŷ (�� %d����):\n", row);
                print_board(board, 0);
            }
        }
    }
}

// N-Queen ���� �ذ�
void solve_n_queens(ChessBoard* board, bool show_steps) {
    solve_n_queens_util(board, 0, show_steps);
    printf("\n�� %d���� �ش��� ã�ҽ��ϴ�.\n", board->solutions);
}

// �޸� ����
void destroy_board(ChessBoard* board) {
    free(board->board);
    free(board);
}

// ��Ī �� ����
void generate_symmetric_solutions(ChessBoard* board) {
    printf("\n=== ��Ī �ش� ���� ===\n");

    // ���� ��Ī
    printf("\n���� ��Ī:\n");
    for (int i = 0; i < board->size; i++) {
        int temp = board->board[i];
        board->board[i] = board->size - 1 - temp;
    }
    print_board(board, board->solutions + 1);

    // ���� ��Ī
    printf("\n���� ��Ī:\n");
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

    printf("ü���� ũ�� �Է� (N-Queen ����): ");
    scanf("%d", &size);

    printf("Ǯ�� ������ ���ðڽ��ϱ�? (1: ��, 0: �ƴϿ�): ");
    scanf("%d", &show_steps);

    ChessBoard* board = create_board(size);

    printf("\n=== %d-Queen ���� �ذ� ���� ===\n", size);
    solve_n_queens(board, show_steps);

    if (board->solutions > 0) {
        char generate_sym;
        printf("\nù ��° �ش信 ���� ��Ī �ش��� �����Ͻðڽ��ϱ�? (y/n): ");
        scanf(" %c", &generate_sym);

        if (generate_sym == 'y' || generate_sym == 'Y') {
            generate_symmetric_solutions(board);
        }
    }

    destroy_board(board);
    return 0;
}

/*
N-Queen ������ Ư��
===============

1. ������ Ư¡
----------
- ���� ���� ����
- ��Ī�� ����
- ���� ����: 8-Queen�� ��� 92��
- ���Ʈ �����δ� �Ұ���

2. ��Ʈ��ŷ ����
------------
- �� ���� ��ġ
- ������ �˻�
- �ﰢ���� ����ġ��
- ���� ����

3. �ð� ���⵵
----------
- �־�: O(N!)
- ����ġ��� �����δ� �� ȿ����
- ũ�⿡ ���� ���ϱ޼��� ����

4. ����ȭ ���
----------
- ��Ī�� Ȱ��
- ù �� ����ȭ
- ��Ʈ����ũ Ȱ��
- ����ȭ ����

�� ������ ��Ʈ��ŷ�� ���
������ �����ָ�, ���� ���ǰ�
���� ���� Ž���� ȿ������
�����ϴ� �� ������ �˴ϴ�.
*/
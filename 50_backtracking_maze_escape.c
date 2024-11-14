#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
�̷� Ż�� ���� (Maze Escape)
=========================

Ư¡:
1. ���� ��ġ���� ������ ��� ���� �õ�
2. ��ȿ���� ���� ��δ� �ǵ��ư�
3. ���� ������ ���޽� ����
4. ��� ���� ���н� ��Ʈ��ŷ
*/

#define MAX_SIZE 20

typedef struct {
    int x, y;
} Point;

typedef struct {
    int** maze;        // �̷� �迭
    int** solution;    // �ذ� ���
    int size;          // �̷� ũ��
    Point start;       // ������
    Point end;         // ������
} Maze;

// ���� ���� (��,��,��,��)
const int dx[] = { -1, 0, 1, 0 };
const int dy[] = { 0, 1, 0, -1 };

// �̷� ����
Maze* create_maze(int size) {
    Maze* maze = (Maze*)malloc(sizeof(Maze));
    maze->size = size;

    // �̷� �迭 �Ҵ�
    maze->maze = (int**)malloc(size * sizeof(int*));
    maze->solution = (int**)malloc(size * sizeof(int*));
    for (int i = 0; i < size; i++) {
        maze->maze[i] = (int*)calloc(size, sizeof(int));
        maze->solution[i] = (int*)calloc(size, sizeof(int));
    }

    return maze;
}

// �̷� ���
void print_maze_state(Maze* maze, bool show_solution) {
    printf("\n���� �̷� ����:\n");
    for (int i = 0; i < maze->size; i++) {
        for (int j = 0; j < maze->size; j++) {
            if (i == maze->start.x && j == maze->start.y)
                printf(" S ");
            else if (i == maze->end.x && j == maze->end.y)
                printf(" E ");
            else if (show_solution && maze->solution[i][j])
                printf(" * ");
            else if (maze->maze[i][j])
                printf(" # ");
            else
                printf(" . ");
        }
        printf("\n");
    }
    printf("\n");
}

// ��ȿ�� �˻�
bool is_valid_move(Maze* maze, int x, int y) {
    return (x >= 0 && x < maze->size &&
        y >= 0 && y < maze->size &&
        !maze->maze[x][y]);
}

// ��Ʈ��ŷ���� ��� ã��
bool solve_maze_util(Maze* maze, int x, int y, bool show_steps) {
    // ������ ����
    if (x == maze->end.x && y == maze->end.y) {
        maze->solution[x][y] = 1;
        return true;
    }

    // ���� ��ġ�� ��ȿ���� �˻�
    if (is_valid_move(maze, x, y)) {
        // ���� ��ġ �湮 ǥ��
        maze->solution[x][y] = 1;

        if (show_steps) {
            printf("\n���� ��ġ: (%d, %d)\n", x, y);
            print_maze_state(maze, true);
        }

        // �� ���� �õ�
        for (int i = 0; i < 4; i++) {
            int next_x = x + dx[i];
            int next_y = y + dy[i];

            if (show_steps) {
                printf("���� %d �õ� (%d, %d)\n", i, next_x, next_y);
            }

            if (solve_maze_util(maze, next_x, next_y, show_steps))
                return true;
        }

        // ��� ���� ���н� ��Ʈ��ŷ
        maze->solution[x][y] = 0;

        if (show_steps) {
            printf("\n��Ʈ��ŷ: (%d, %d)\n", x, y);
            print_maze_state(maze, true);
        }
    }

    return false;
}

// �̷� �ذ� �Լ�
bool solve_maze(Maze* maze, bool show_steps) {
    printf("\n=== �̷� Ż�� ���� ===\n");
    printf("������: (%d, %d)\n", maze->start.x, maze->start.y);
    printf("������: (%d, %d)\n", maze->end.x, maze->end.y);

    return solve_maze_util(maze, maze->start.x, maze->start.y, show_steps);
}

// �޸� ����
void destroy_maze(Maze* maze) {
    for (int i = 0; i < maze->size; i++) {
        free(maze->maze[i]);
        free(maze->solution[i]);
    }
    free(maze->maze);
    free(maze->solution);
    free(maze);
}

int main(void) {
    int size;
    printf("�̷��� ũ�� �Է�: ");
    scanf("%d", &size);

    Maze* maze = create_maze(size);

    printf("\n�̷� �Է� (0: ��, 1: ��)\n");
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            scanf("%d", &maze->maze[i][j]);
        }
    }

    printf("������ �Է� (x y): ");
    scanf("%d %d", &maze->start.x, &maze->start.y);

    printf("������ �Է� (x y): ");
    scanf("%d %d", &maze->end.x, &maze->end.y);

    print_maze_state(maze, false);

    bool show_steps;
    printf("Ǯ�� ������ ���ðڽ��ϱ�? (1: ��, 0: �ƴϿ�): ");
    scanf("%d", &show_steps);

    if (solve_maze(maze, show_steps)) {
        printf("\n=== �̷� Ż�� ����! ===\n");
        print_maze_state(maze, true);
    }
    else {
        printf("\n=== �̷� Ż�� ����! ===\n");
    }

    destroy_maze(maze);
    return 0;
}

/*
��Ʈ��ŷ�� �ٽ� ����
================

1. ���� ���� Ž��
--------------
- DFS ��� Ž��
- ������ �˻�
- ����ġ��
- ���� ����

2. �ð� ���⵵
-----------
- �־�: O(4^(N��))
- ����ġ��� �����δ� �� ȿ����
- �̷� ������ ���� ũ�� �޶���

3. ���� Ư¡
---------
- ����� ����
- �湮 ǥ��
- ��� ����
- ���� �ѹ�

4. ����ȭ ���
-----------
- �湮 üũ
- ���� �켱����
- �޸� ����
- ���� ����

�� ������ ��Ʈ��ŷ�� �⺻ ������
�̷� Ż�� ������ ���� �����ָ�,
���� ���� Ž���� �ٽ���
�����ϴ� �� ������ �˴ϴ�.
*/
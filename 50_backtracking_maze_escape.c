#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
미로 탈출 문제 (Maze Escape)
=========================

특징:
1. 현재 위치에서 가능한 모든 방향 시도
2. 유효하지 않은 경로는 되돌아감
3. 최종 목적지 도달시 성공
4. 모든 방향 실패시 백트래킹
*/

#define MAX_SIZE 20

typedef struct {
    int x, y;
} Point;

typedef struct {
    int** maze;        // 미로 배열
    int** solution;    // 해결 경로
    int size;          // 미로 크기
    Point start;       // 시작점
    Point end;         // 도착점
} Maze;

// 방향 벡터 (상,우,하,좌)
const int dx[] = { -1, 0, 1, 0 };
const int dy[] = { 0, 1, 0, -1 };

// 미로 생성
Maze* create_maze(int size) {
    Maze* maze = (Maze*)malloc(sizeof(Maze));
    maze->size = size;

    // 미로 배열 할당
    maze->maze = (int**)malloc(size * sizeof(int*));
    maze->solution = (int**)malloc(size * sizeof(int*));
    for (int i = 0; i < size; i++) {
        maze->maze[i] = (int*)calloc(size, sizeof(int));
        maze->solution[i] = (int*)calloc(size, sizeof(int));
    }

    return maze;
}

// 미로 출력
void print_maze_state(Maze* maze, bool show_solution) {
    printf("\n현재 미로 상태:\n");
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

// 유효성 검사
bool is_valid_move(Maze* maze, int x, int y) {
    return (x >= 0 && x < maze->size &&
        y >= 0 && y < maze->size &&
        !maze->maze[x][y]);
}

// 백트래킹으로 경로 찾기
bool solve_maze_util(Maze* maze, int x, int y, bool show_steps) {
    // 목적지 도달
    if (x == maze->end.x && y == maze->end.y) {
        maze->solution[x][y] = 1;
        return true;
    }

    // 현재 위치가 유효한지 검사
    if (is_valid_move(maze, x, y)) {
        // 현재 위치 방문 표시
        maze->solution[x][y] = 1;

        if (show_steps) {
            printf("\n현재 위치: (%d, %d)\n", x, y);
            print_maze_state(maze, true);
        }

        // 네 방향 시도
        for (int i = 0; i < 4; i++) {
            int next_x = x + dx[i];
            int next_y = y + dy[i];

            if (show_steps) {
                printf("방향 %d 시도 (%d, %d)\n", i, next_x, next_y);
            }

            if (solve_maze_util(maze, next_x, next_y, show_steps))
                return true;
        }

        // 모든 방향 실패시 백트래킹
        maze->solution[x][y] = 0;

        if (show_steps) {
            printf("\n백트래킹: (%d, %d)\n", x, y);
            print_maze_state(maze, true);
        }
    }

    return false;
}

// 미로 해결 함수
bool solve_maze(Maze* maze, bool show_steps) {
    printf("\n=== 미로 탈출 시작 ===\n");
    printf("시작점: (%d, %d)\n", maze->start.x, maze->start.y);
    printf("도착점: (%d, %d)\n", maze->end.x, maze->end.y);

    return solve_maze_util(maze, maze->start.x, maze->start.y, show_steps);
}

// 메모리 해제
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
    printf("미로의 크기 입력: ");
    scanf("%d", &size);

    Maze* maze = create_maze(size);

    printf("\n미로 입력 (0: 길, 1: 벽)\n");
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            scanf("%d", &maze->maze[i][j]);
        }
    }

    printf("시작점 입력 (x y): ");
    scanf("%d %d", &maze->start.x, &maze->start.y);

    printf("도착점 입력 (x y): ");
    scanf("%d %d", &maze->end.x, &maze->end.y);

    print_maze_state(maze, false);

    bool show_steps;
    printf("풀이 과정을 보시겠습니까? (1: 예, 0: 아니오): ");
    scanf("%d", &show_steps);

    if (solve_maze(maze, show_steps)) {
        printf("\n=== 미로 탈출 성공! ===\n");
        print_maze_state(maze, true);
    }
    else {
        printf("\n=== 미로 탈출 실패! ===\n");
    }

    destroy_maze(maze);
    return 0;
}

/*
백트래킹의 핵심 개념
================

1. 상태 공간 탐색
--------------
- DFS 기반 탐색
- 유망성 검사
- 가지치기
- 상태 복원

2. 시간 복잡도
-----------
- 최악: O(4^(N²))
- 가지치기로 실제로는 더 효율적
- 미로 구조에 따라 크게 달라짐

3. 구현 특징
---------
- 재귀적 접근
- 방문 표시
- 경로 추적
- 상태 롤백

4. 최적화 기법
-----------
- 방문 체크
- 방향 우선순위
- 메모리 재사용
- 조기 종료

이 구현은 백트래킹의 기본 원리를
미로 탈출 문제를 통해 보여주며,
상태 공간 탐색의 핵심을
이해하는 데 도움이 됩니다.
*/
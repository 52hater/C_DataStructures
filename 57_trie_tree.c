#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/*
알고리즘 분류: 트리 자료구조
하위 분류: 문자열 특화 트리
난이도: 중상
관련 자료구조: 해시 테이블, 이진 탐색 트리

트라이 트리 (Trie Tree / Prefix Tree):
- 문자열 집합을 저장하는 정렬된 트리
- 각 노드는 문자 하나를 표현
- 공통 접두어를 공유하여 공간 절약
- 문자열 검색, 자동 완성에 효율적
*/

#define ALPHABET_SIZE 26

typedef struct TrieNode {
    struct TrieNode* children[ALPHABET_SIZE];
    bool is_end_of_word;
    int word_count;       // 이 접두어로 시작하는 단어 수
} TrieNode;

typedef struct {
    TrieNode* root;
    int total_words;
} TrieTree;

// 새 노드 생성
TrieNode* create_node(void) {
    TrieNode* node = (TrieNode*)malloc(sizeof(TrieNode));
    node->is_end_of_word = false;
    node->word_count = 0;

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        node->children[i] = NULL;
    }

    return node;
}

// 트라이 트리 생성
TrieTree* create_trie(void) {
    TrieTree* trie = (TrieTree*)malloc(sizeof(TrieTree));
    trie->root = create_node();
    trie->total_words = 0;
    return trie;
}

// 단어 삽입
void insert(TrieTree* trie, const char* word) {
    TrieNode* current = trie->root;

    printf("\n단어 '%s' 삽입 과정:\n", word);

    for (int i = 0; word[i]; i++) {
        int index = word[i] - 'a';
        if (index < 0 || index >= ALPHABET_SIZE) {
            printf("경고: 알파벳 소문자만 허용됨\n");
            return;
        }

        if (current->children[index] == NULL) {
            current->children[index] = create_node();
            printf("새 노드 생성: '%c'\n", word[i]);
        }
        current = current->children[index];
        current->word_count++;
    }

    if (!current->is_end_of_word) {
        current->is_end_of_word = true;
        trie->total_words++;
        printf("단어 끝 표시 추가\n");
    }
}

// 단어 검색
bool search(TrieTree* trie, const char* word) {
    TrieNode* current = trie->root;

    printf("\n단어 '%s' 검색 과정:\n", word);

    for (int i = 0; word[i]; i++) {
        int index = word[i] - 'a';
        if (!current->children[index]) {
            printf("문자 '%c'에서 검색 실패\n", word[i]);
            return false;
        }
        current = current->children[index];
        printf("문자 '%c' 찾음\n", word[i]);
    }

    bool found = current->is_end_of_word;
    printf("검색 결과: %s\n", found ? "찾음" : "없음");
    return found;
}

// 접두어로 시작하는 단어 수 반환
int count_prefix(TrieTree* trie, const char* prefix) {
    TrieNode* current = trie->root;

    for (int i = 0; prefix[i]; i++) {
        int index = prefix[i] - 'a';
        if (!current->children[index]) {
            return 0;
        }
        current = current->children[index];
    }

    return current->word_count;
}

// 자동 완성 구현을 위한 재귀 함수
void find_words_with_prefix(TrieNode* node, char* prefix, int level) {
    if (node->is_end_of_word) {
        prefix[level] = '\0';
        printf("%s\n", prefix);
    }

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i]) {
            prefix[level] = i + 'a';
            find_words_with_prefix(node->children[i], prefix, level + 1);
        }
    }
}

// 자동 완성 기능
void autocomplete(TrieTree* trie, const char* prefix) {
    TrieNode* current = trie->root;
    char buffer[100];
    strcpy(buffer, prefix);

    printf("\n'%s'로 시작하는 단어들:\n", prefix);

    // 접두어까지 이동
    for (int i = 0; prefix[i]; i++) {
        int index = prefix[i] - 'a';
        if (!current->children[index]) {
            printf("해당 접두어로 시작하는 단어가 없습니다.\n");
            return;
        }
        current = current->children[index];
    }

    // 가능한 모든 단어 출력
    find_words_with_prefix(current, buffer, strlen(prefix));
}

// 트라이 노드 삭제 (재귀)
void free_node(TrieNode* node) {
    if (node == NULL) return;

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        free_node(node->children[i]);
    }

    free(node);
}

// 트라이 트리 삭제
void free_trie(TrieTree* trie) {
    free_node(trie->root);
    free(trie);
}

// 통계 정보 출력
void print_stats(TrieTree* trie) {
    printf("\n=== 트라이 트리 통계 ===\n");
    printf("총 단어 수: %d\n", trie->total_words);
}

int main(void) {
    TrieTree* trie = create_trie();
    char word[100];
    int choice;

    printf("=== 트라이 트리 테스트 ===\n");

    while (1) {
        printf("\n1. 단어 삽입\n");
        printf("2. 단어 검색\n");
        printf("3. 자동 완성\n");
        printf("4. 접두어 통계\n");
        printf("5. 트리 통계\n");
        printf("0. 종료\n");
        printf("선택: ");

        scanf("%d", &choice);
        getchar(); // 버퍼 비우기

        switch (choice) {
        case 1:
            printf("삽입할 단어 (소문자): ");
            scanf("%s", word);
            insert(trie, word);
            break;

        case 2:
            printf("검색할 단어: ");
            scanf("%s", word);
            search(trie, word);
            break;

        case 3:
            printf("접두어 입력: ");
            scanf("%s", word);
            autocomplete(trie, word);
            break;

        case 4:
            printf("접두어 입력: ");
            scanf("%s", word);
            printf("'%s'로 시작하는 단어 수: %d\n",
                word, count_prefix(trie, word));
            break;

        case 5:
            print_stats(trie);
            break;

        case 0:
            free_trie(trie);
            return 0;

        default:
            printf("잘못된 선택\n");
        }
    }

    return 0;
}

/*
트라이 트리 분석
============

1. 시간 복잡도
-----------
- 삽입: O(m), m은 문자열 길이
- 검색: O(m)
- 접두어 검색: O(p + k)
  p: 접두어 길이
  k: 결과 단어 수

2. 공간 복잡도
-----------
- 최악: O(ALPHABET_SIZE * m * n)
  m: 평균 문자열 길이
  n: 문자열 개수
- 실제로는 공통 접두어로 인해 더 적음

3. 장단점
-------
장점:
- 빠른 검색/삽입
- 접두어 기반 연산 효율적
- 자동 완성에 이상적

단점:
- 메모리 사용량 많음
- 공간 지역성 낮음
- 업데이트 비용 높음

4. 최적화 기법
-----------
- 압축 트라이
- 기수 트리
- 턴스틸 트리
- 메모리 풀링

5. 실제 응용
---------
- 자동 완성
- 맞춤법 검사
- IP 라우팅
- 전화번호부

이 구현은 문자열 처리에
특화된 트리 구조의
기본 원리를 보여줍니다.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 데이터 타입 추상화를 위한 type definition (필요시 int부분을 교체)
typedef int DataType;

// 노드 구조체 정의
typedef struct Node {
	DataType data;
	struct Node* next;
} Node;

// 연결리스트 구조체 정의
typedef struct {
	Node* head;
	size_t size;
} LinkedList;

/*  리스트 초기화 함수
	- 새로운 빈 연결 리스트를 생성하고 초기화
	- 반환값 : 초기화된 LinkedList 구조체 */
LinkedList* list_create(void) {
	LinkedList* list = (LinkedList*)malloc(sizeof(LinkedList));
	if (list == NULL) { // 이거 없으면 널포인터 역참조 일어남 (없는 구조체에 list->head = NULL; 이런걸 하려하니까 크래시남)
		puts("리스트 생성 실패");
		return NULL; // 포인터를 반환해야하니 1 안됨(타입불일치)
	}
	list->head = NULL;
	list->size = 0;
	return list; // 생성 성공시 포인터 반환
}

/*  새로운 노드 생성 함수
	- 데이터를 담은 새로운 노드를 동적 할당(힙영역)
	- 매개변수 : data >> 저장할 데이터 값
	- 반환값 : 생성된 새 노드의 포인터 */
Node* node_create(DataType data) {
	Node* new_node = (Node*)malloc(sizeof(Node));
	if (new_node == NULL) {
		return NULL;
	}
	new_node->data = data;
	new_node->next = NULL;
	return new_node;
}

/*  노드 삭제 함수
	- 개별 노드의 "메모리 자체" 를 해제
	- 매개 변수 : node - 삭제할 노드의 포인터 */
void node_destroy(Node* node) {
	free(node);
}

/*  리스트의 맨 앞에 노드 삽입
	- 새 노드를 생성하여 리스트의 헤드로 설정
	- 매개변수 : list - 대상 리스트, data - 저장할 데이터
	- 반환값 : 성공 시 1, 실패 시 0 */
int list_push_front(LinkedList* list, DataType data) {
	Node* new_node = node_create(data);
	if (new_node == NULL) {
		return 0;
	}
	new_node->next = list->head;
	list->head = new_node;
	list->size++;
	return 1;
}
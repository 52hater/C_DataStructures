#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 데이터 타입 추상화를 위한 type definition (필요시 int부분을 교체)
typedef int DataType;

// 노드 구조체 정의
typedef struct Node {
	DataType data; // 노드가 저장할 데이터
	struct Node* next; // 다음 노드를 가리키는 포인터
} Node;

// 연결리스트 구조체 정의
typedef struct LinkedList {
	Node* head; // 첫번째 노드(헤드)를 가리키는 포인터
	size_t size; // 리스트의 크기(노드 개수)
} LinkedList;

/*  리스트 생성 및 초기화 함수
	- 새로운 빈 연결 리스트를 생성하고 초기화
	- 반환값 : 초기화된 LinkedList 구조체 */
LinkedList* list_create(void) {
	LinkedList* list = (LinkedList*)malloc(sizeof(LinkedList)); //엠얼록함수는 void* 타입이므로 형변환 해줌.
	if (list == NULL) { // 이거 없으면 널포인터 역참조 일어남 (없는 구조체에 list->head = NULL; 이런걸 하려하니까 크래시남)
		puts("리스트 생성 실패");
		return NULL; // 포인터를 반환해야하니 1 안됨(타입불일치)
	}
	list->head = NULL; // = (*list).head 와 완벽하게 일치함(->연산자는 역참조와 멤버 접근을 동시에 수행)
	list->size = 0;
	return list; // 생성 성공시 포인터 반환
}

/*  새로운 노드 생성 함수
	- 데이터를 담은 새로운 노드를 동적 할당(힙영역)
	- 매개변수 : data >> 저장할 데이터 값
	- 반환값 : 생성된 새 노드의 포인터 */

// node_create(5); 이렇게 호출하면 파라미터에 5를 주고 노드의 data필드에 저장해서 데이터5를 가지는 새로운 노드가 생성되겠지.
Node* node_create(DataType data) { // 새로운 노드가 생성될 때 노드의 data 필드에 저장될 값을 받아오는 것.
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
	Node* new_node = node_create(data); // 새로운 노드를 생성하고(Node형 구조체의 메모리 주소 저장할 곳 : new_node) 내용물을 받아옴
	if (new_node == NULL) { // 뉴노드 삽입 실패시
		return 0;
	}
	new_node->next = list->head; // 새로운 노드의 next 포인터가 현재 리스트의 헤드를 가리키도록 설정
	list->head = new_node; // 이제 새로운노드가 연결리스트'의' 헤드가 되었고(->는 역참조 자동)
	list->size++; // 사이즈는(노드의 개수는) 하나 늘어났다
	return 1;
}
/*  
	Node* new_node = node_create(data);
	이 코드는 새로운 노드를 생성하고, 생성된 노드의 주소를 new_node 포인터에 저장합니다.
	즉, node_create 함수는 Node* 타입의 값을 반환합니다. 이 Node* 타입의 값은 새로 생성된 Node 구조체의 메모리 주소입니다.
	Node*를 반환한다는 것은 생성된 Node형 구조체의 주소를 반환하는 것입니다.
	Node 구조체 자체가 아니라, 그 구조체의 메모리 주소만 반환합니다.
	포인터형을 반환한다는 것은 메모리 주소를 반환한다는 뜻
*/

int main(void) {
	
	// 리스트 생성
	LinkedList* list = list_create();

	// 뒤에 노드 5개 추가 (0~4)
	for (int i = 0; i < 5; i++) {
		list_push_back(list, i);
	}

	// 앞에 노드 2개 추가 (-2, -1)
	list_push_front(list, -2);
	list_push_front(list, -1);

	// 리스트 출력
	printf("리스트1 :\n");
	for (size_t i = 0; i < list_size(list); i++) {
		Node* current = list_get_node(list, i);
		printf("list[%zu]: %d\n", i, current->data);
	}

	// 세 번째 노드 뒤에 새 노드 삽입
	printf("\n세번째 노드 뒤에 3000삽입\n");
	Node* third_node = list_get_node(list, 2);
	list_insert_after(third_node, 3000);

	// 수정된 리스트 출력
	printf("\n수정된 리스트 :\n");
	for (size_t i = 0; i < list_size(list); i++) {
		Node* current = list_get_node(list, i);
		printf("list[%zu]: %d\n", i, current->data);
	}

	// 리스트 메모리 해제
	printf("\n리스트 메모리 해제\n");
	list_destroy(list);

	return 0;

}

/*
==========================================
상세 설명 및 주요 개념
==========================================

1. 구조체 설계
-------------
- Node: 개별 노드를 표현하는 구조체
  * data: 실제 저장할 데이터
  * next: 다음 노드를 가리키는 포인터

- LinkedList: 전체 리스트를 관리하는 구조체
  * head: 첫 번째 노드를 가리키는 포인터
  * size: 현재 리스트의 크기

2. 함수 명명 규칙
----------------
현업에서 자주 사용되는 명명 규칙을 따름:
- list_*: 리스트 전체에 대한 작업
- node_*: 개별 노드에 대한 작업
- 동사_목적어 형태로 함수 이름 구성 (예: list_push_front)

3. 메모리 관리
-------------
- malloc()을 통한 동적 할당
- 각 할당 후 NULL 체크로 메모리 부족 상황 처리
- free()를 통한 적절한 메모리 해제
- 메모리 누수 방지를 위한 체계적인 해제 로직

4. 에러 처리
-----------
- 함수들은 작업 성공/실패를 나타내는 반환값 제공
- NULL 포인터 검사
- 잘못된 인덱스 접근 방지
- 메모리 할당 실패 처리

5. 주요 작업들
-------------
- 생성: list_create(), node_create()
- 삽입: list_push_front(), list_push_back(), list_insert_after()
- 탐색: list_get_node()
- 삭제: list_remove_node(), node_destroy()
- 해제: list_destroy()

6. 최적화 포인트
---------------
- size 필드를 통한 O(1) 크기 접근
- 포인터 연산을 통한 효율적인 노드 조작
- 불필요한 순회 최소화

7. 안전성 고려사항
----------------
- 모든 포인터 연산 전 NULL 체크
- 경계 조건(empty list, single node) 처리
- 메모리 누수 방지를 위한 체계적인 해제

8. 확장성
--------
- DataType typedef를 통한 데이터 타입 추상화
- 모듈화된 설계로 기능 추가 용이
- 일관된 인터페이스 제공

9. 코드 스타일
-------------
- 일관된 들여쓰기
- 명확한 함수 주석
- 의미 있는 변수명
- 논리적인 함수 구조화

10. 사용 예시
------------
- 기본적인 CRUD 작업 구현
- 리스트 순회 및 데이터 접근
- 메모리 관리 예시
- 에러 처리 예시

이 구현은 교육용으로도 적합하며, 실제 프로젝트에서도
활용할 수 있는 수준의 안정성과 확장성을 갖추고 있습니다.
*/
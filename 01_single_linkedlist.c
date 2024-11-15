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


// 리스트를 지운다고 되는게 아니고 노드들이 각각 다 따로 메모리지정이 되어있으니 노드부터 지워주고 남은 빈 리스트를 지워야 됨.
void list_destroy(LinkedList* list) {
	// 리무브노드에서 노드의 메모리해제까지 만들고
	// 이 함수에서는 리무브노드를 반복문으로 호출해서 노드를 싹 없애는 코드를 만들어주고
	// 마지막에 free(list);
}

/*  리스트의 맨 앞에 노드 삽입
	- 새 노드를 생성하여 리스트의 헤드로 설정
	- 매개변수 : list - 대상 리스트, data - 저장할 데이터
	- 반환값 : 성공 시 1, 실패 시 0 */
int list_push_front(LinkedList* list, DataType data) {
	Node* new_node = node_create(data); // 새로운 노드를 생성하고(Node형 구조체의 메모리 주소 저장할 곳 : new_node) 내용물을 받아옴
	if (new_node == NULL) { // 뉴노드 삽입 실패시
		return 0; // 노드삽입 실패시 0반환
	}
	new_node->next = list->head; // 새로운 노드의 next 포인터가 현재 리스트의 헤드를 가리키도록 설정
	list->head = new_node; // 이제 새로운노드가 연결리스트'의' 헤드가 되었고(->는 역참조 자동)
	list->size++; // 사이즈는(노드의 개수는) 하나 늘어났다
	return 1; // 맨앞에 노드삽입 성공시 1반환
}
/*  
	Node* new_node = node_create(data);
	이 코드는 새로운 노드를 생성하고, 생성된 노드의 주소를 new_node 포인터에 저장합니다.
	즉, node_create 함수는 Node* 타입의 값을 반환합니다. 이 Node* 타입의 값은 새로 생성된 Node 구조체의 메모리 주소입니다.
	Node*를 반환한다는 것은 생성된 Node형 구조체의 주소를 반환하는 것입니다.
	Node 구조체 자체가 아니라, 그 구조체의 메모리 주소만 반환합니다.
	포인터형을 반환한다는 것은 메모리 주소를 반환한다는 뜻
*/

/* 리스트의 맨 뒤에 노드 추가
  - 새 노드를 생성하여 리스트의 마지막에 추가
  - 매개변수: list - 대상 리스트, data - 저장할 데이터
  - 반환값: 성공 시 1, 실패 시 0
*/

int list_push_back(LinkedList* list, DataType data) {
	Node* new_node = node_create(data); // 노드구조체포인터타입 뉴노드는 노드만드는 함수에 data를 넣어서 만들어옴
	if (new_node == NULL) {
		return 0; // 못만들어왔으면 0반환
	}

	if (list->head == NULL) { // 리스트의 헤드가 없으면 (비어있으면)
		list->head == new_node; // 여기서 만든 뉴노드가 헤드가 되겠지
	}
	else { // 리스트가 비어있지 않으면
		Node* current = list->head; // 리스트의 원래 있던 헤드를 현재노드를 가리키는 포인터로 지정하고
		while (current->next != NULL) { // 현재노드주소의 다음 노드주소를 가리키는 넥스트 포인터가 없을때까지(제일 뒤로 올 때까지)
			current = current->next; // 조건문 한번 반복될때마다 이게 실행되는거지 > 한칸씩 이동
		}
		current->next = new_node; // 현재 노드의 넥스트가 새로 생성된 노드를 가리킴(push_back이 된 것)
		//current->next == NULL이 되면 while문 빠져나오고 current->next = new_node; 실행
	}
	list->size++; // 노드 추가되었으니 노드 개수 늘어났고
	return 1; // 노드추가 성공시 1반환
}

/* 특정 위치 뒤에 노드 삽입
  - 지정된 노드 뒤에 새 노드를 삽입
  - 매개변수: node - 삽입 위치 노드, data - 저장할 데이터
  - 반환값: 성공 시 1, 실패 시 0
 */
int list_insert_after(Node* node, DataType data) {
	
	if (node == NULL) { // 삽입할 위치(node) 유효성검사부터
		return 0;
	}

	Node* new_node = node_create(data);
	if (new_node == NULL) { // 노드생성 유효성검사
		return 0;
	}

	new_node->next = node->next; // 원래 노드의 넥스트주소에 새노드의 넥스트주소를 대입 (뒤에 끼어든거지)
	node->next = new_node; // 그래서 뒤에오게 된 것
	
	return 1;
}

/* 특정 위치의 노드 탐색
  - 인덱스로 노드를 찾아서 반환
  - 매개변수: list - 대상 리스트, index - 찾을 위치
  - 반환값: 찾은 노드의 포인터 또는 NULL
*/
Node* list_get_node(LinkedList* list, size_t index) { // search
	if (index >= list->size) { // index == list면 전체 size가 5라쳤을때 index 0,1,2,3,4 이므로 유효하지 않은 위치접근임
		return NULL;
	}

	Node* current = list->head; // 헤드부터 쭉 가보자고
	for (size_t i = 0; i < index; i++) { // 지정한 index(예를들어 3이면) 3크기의 012 나오고 2를 찾으니 3번째 인덱스를 찾은 셈)
		current = current->next;
		/* 주석처럼 리스트 크기 5이고 0, 1, 2, 3, 4 가 있다고 쳤을 때
		 Node* current = list->head; 여기서 current는 0을 가리킴
		 index에 3을 받아오면 0 > 1 까지 찾고(i < index; 이니까 2번째까지 순회한거지)
		 1노드 다음인 2노드를 가리키고 있다(current = current->next;)
		 그래서 결국 찾아야 하는 2노드가 나온 것. (받아온 값3은 결국 3번째 노드(2노드)니까)
		*/
	}
	return current;
}

/* 노드 삭제
 * - 리스트에서 특정 노드를 제거하고 메모리 해제
 * - 매개변수: list - 대상 리스트, target - 삭제할 노드
 * - 반환값: 성공 시 1, 실패 시 0
 */
int list_remove_target_node(LinkedList* list, Node* target) {
	
	if (target >= list->size) { // 타겟 유효성 검사 outOfRange면 return 0;
		return 0;
	}

	// 타겟 찾기
	Node* current = list->head; // 현재노드 포인터 하나 헤드에 지정해주고
	Node* prev = NULL; // 이전노드 포인터 하나 지정해주고 (previous)
	for (size_t i = 0; i < target; i++) { // target -1 크기만큼 순회 > 인덱스 찾음
		prev = current; // 순회하는동안 얘도 한칸씩 이동
		current = current->next; // 순회하는동안 포인터 한칸씩 이동 (타겟주소는 current포인터에 저장)
	} // 찾았으면 반복문 빠져나오고

	// 이전노드 링크해줘야지(노드 연결 관계 수정)
	if (prev == NULL) { // 첫번째 노드를 삭제할 때
		list->head = current->next; // 삭제할 타겟 다음 노드가 헤드가 되고
	}
	else { // 중간이나 마지막 노드를 삭제할 때(타겟 앞에 노드가 있다)
		prev->next = current->next; // 삭제할 타겟의 다음노드와 앞노드의 next 연결
	}

	free(current); // 타겟 노드 메모리 해제
	list->size--; // 하나 삭제하면 리스트의 크기--;
	return 1; // 삭제 성공
}

int list_remove_head(LinkedList* list) {

	

}

int list_remove_tail(LinkedList* list, Node* index) {



}

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
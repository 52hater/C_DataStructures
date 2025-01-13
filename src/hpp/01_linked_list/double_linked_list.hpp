#ifndef DOUBLE_LINKED_LIST_HPP
#define DOUBLE_LINKED_LIST_HPP

#include <memory>
#include <stdexcept>
#include <iostream>
#include "i_linked_list.hpp"

/**
 * @brief 이중 연결 리스트 구현 클래스
 * 각 노드가 이전 노드와 다음 노드를 모두 가리키는 양방향 연결 리스트
 */
template<typename T>
class DoubleLinkedList : public ILinkedList<T> {
private:
    // 이중 연결 리스트의 노드 구조체
    struct Node {
        T data;                      // 노드에 저장될 데이터
        std::unique_ptr<Node> next;  // 다음 노드를 가리키는 포인터
        Node* prev;                  // 이전 노드를 가리키는 포인터

        // 새로운 노드 생성 시 데이터 초기화
        explicit Node(const T& value)
            : data(value), next(nullptr), prev(nullptr) {}
    };

    std::unique_ptr<Node> head_;  // 리스트의 첫 번째 노드
    Node* tail_;                  // 리스트의 마지막 노드
    size_t size_;                // 리스트의 크기

public:
    // 빈 리스트 생성
    DoubleLinkedList() : head_(nullptr), tail_(nullptr), size_(0) {}

    // 가상 소멸자
    ~DoubleLinkedList() override = default;

    // 복사 생성자와 대입 연산자 삭제
    DoubleLinkedList(const DoubleLinkedList&) = delete;
    DoubleLinkedList& operator=(const DoubleLinkedList&) = delete;

    // 이동 생성자와 이동 대입 연산자 기본 구현 사용
    DoubleLinkedList(DoubleLinkedList&&) = default;
    DoubleLinkedList& operator=(DoubleLinkedList&&) = default;

    // 리스트가 비어있는지 확인
    [[nodiscard]] bool empty() const override {
        return size_ == 0;
    }

    // 리스트의 크기 반환
    [[nodiscard]] size_t size() const override {
        return size_;
    }

    // 리스트의 맨 앞에 노드 추가
    void push_front(const T& value) override {
        auto new_node = std::make_unique<Node>(value);
        Node* new_node_ptr = new_node.get();

        if (empty()) {
            tail_ = new_node_ptr;
        } else {
            new_node->next = std::move(head_);
            new_node->next->prev = new_node_ptr;
        }

        head_ = std::move(new_node);
        ++size_;
    }

    // 리스트의 맨 뒤에 노드 추가
    void push_back(const T& value) override {
        auto new_node = std::make_unique<Node>(value);
        Node* new_node_ptr = new_node.get();

        if (empty()) {
            head_ = std::move(new_node);
            tail_ = head_.get();
        } else {
            new_node->prev = tail_;
            tail_->next = std::move(new_node);
            tail_ = new_node_ptr;
        }

        ++size_;
    }

    // 지정된 위치 다음에 노드 삽입
    void insert_after(size_t index, const T& value) override {
        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }

        if (index == size_ - 1) {
            push_back(value);
            return;
        }

        Node* current = head_.get();
        for (size_t i = 0; i < index; ++i) {
            current = current->next.get();
        }

        auto new_node = std::make_unique<Node>(value);
        new_node->prev = current;
        new_node->next = std::move(current->next);
        new_node->next->prev = new_node.get();
        current->next = std::move(new_node);

        ++size_;
    }

    // 리스트의 맨 앞 노드 제거
    void pop_front() override {
        if (empty()) {
            throw std::runtime_error("List is empty");
        }

        head_ = std::move(head_->next);
        if (head_) {
            head_->prev = nullptr;
        } else {
            tail_ = nullptr;
        }

        --size_;
    }

    // 지정된 위치의 노드 제거
    void remove_at(size_t index) override {
        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }

        if (index == 0) {
            pop_front();
            return;
        }

        Node* current = head_.get();
        for (size_t i = 0; i < index; ++i) {
            current = current->next.get();
        }

        if (current == tail_) {
            tail_ = current->prev;
            tail_->next = nullptr;
        } else {
            current->next->prev = current->prev;
            current->prev->next = std::move(current->next);
        }

        --size_;
    }

    // 지정된 위치의 노드 데이터 반환
    T& at(size_t index) override {
        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }

        Node* current = head_.get();
        for (size_t i = 0; i < index; ++i) {
            current = current->next.get();
        }
        return current->data;
    }

    // 지정된 위치의 노드 데이터 반환 (상수 버전)
    const T& at(size_t index) const override {
        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }

        const Node* current = head_.get();
        for (size_t i = 0; i < index; ++i) {
            current = current->next.get();
        }
        return current->data;
    }

    // 리스트의 모든 요소 출력
    void print() const override {
        const Node* current = head_.get();
        std::cout << "List contents: ";
        while (current) {
            std::cout << current->data << " ";
            current = current->next.get();
        }
        std::cout << std::endl;
    }
};

#endif // DOUBLE_LINKED_LIST_HPP
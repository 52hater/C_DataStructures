#ifndef SINGLE_LINKED_LIST_HPP
#define SINGLE_LINKED_LIST_HPP

#include <memory>
#include <stdexcept>
#include <iostream>
#include "i_linked_list.hpp"

/**
 * @brief 단일 연결 리스트 구현 클래스
 * 각 노드가 다음 노드만을 가리키는 단방향 연결 리스트
 */
template<typename T>
class SingleLinkedList : public ILinkedList<T> {
private:
    // 단일 연결 리스트의 노드 구조체
    struct Node {
        T data;                      // 노드에 저장될 데이터
        std::unique_ptr<Node> next;  // 다음 노드를 가리키는 포인터

        // 새로운 노드 생성 시 데이터 초기화
        explicit Node(const T& value) : data(value), next(nullptr) {}
    };

    std::unique_ptr<Node> head_;  // 리스트의 첫 번째 노드
    size_t size_;                 // 리스트의 크기

public:
    // 빈 리스트 생성
    SingleLinkedList() : head_(nullptr), size_(0) {}

    // 가상 소멸자
    ~SingleLinkedList() override = default;

    // 복사 생성자와 대입 연산자 삭제
    SingleLinkedList(const SingleLinkedList&) = delete;
    SingleLinkedList& operator=(const SingleLinkedList&) = delete;

    // 이동 생성자와 이동 대입 연산자 기본 구현 사용
    SingleLinkedList(SingleLinkedList&&) = default;
    SingleLinkedList& operator=(SingleLinkedList&&) = default;

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
        new_node->next = std::move(head_);
        head_ = std::move(new_node);
        ++size_;
    }

    // 리스트의 맨 뒤에 노드 추가
    void push_back(const T& value) override {
        auto new_node = std::make_unique<Node>(value);

        if (!head_) {
            head_ = std::move(new_node);
        } else {
            Node* current = head_.get();
            while (current->next) {
                current = current->next.get();
            }
            current->next = std::move(new_node);
        }
        ++size_;
    }

    // 지정된 위치 다음에 노드 삽입
    void insert_after(size_t index, const T& value) override {
        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }

        Node* current = head_.get();
        for (size_t i = 0; i < index; ++i) {
            current = current->next.get();
        }

        auto new_node = std::make_unique<Node>(value);
        new_node->next = std::move(current->next);
        current->next = std::move(new_node);
        ++size_;
    }

    // 리스트의 맨 앞 노드 제거
    void pop_front() override {
        if (empty()) {
            throw std::runtime_error("List is empty");
        }
        head_ = std::move(head_->next);
        --size_;
    }

    // 지정된 위치의 노드 제거
    void remove_at(size_t index) override {
        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }

        if (index == 0) {
            head_ = std::move(head_->next);
        } else {
            Node* current = head_.get();
            for (size_t i = 0; i < index - 1; ++i) {
                current = current->next.get();
            }
            current->next = std::move(current->next->next);
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

    // 지정된 위치의 노드 데이터 반환 (const(상수) 버전)
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

#endif // SINGLE_LINKED_LIST_HPP
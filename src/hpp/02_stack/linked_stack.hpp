#ifndef LINKED_STACK_HPP
#define LINKED_STACK_HPP

#include <memory>
#include <stdexcept>
#include <iostream>
#include "i_stack.hpp"

template<typename T>
class LinkedStack : public IStack<T> {
private:
    // 노드 구조체 정의
    struct Node {
        T data;
        std::unique_ptr<Node> next;

        // 새로운 노드 생성시 데이터 초기화
        explicit Node(const T& value) : data(value), next(nullptr) {}
    };

    std::unique_ptr<Node> top_;   // 스택의 최상위 노드
    size_t size_;                // 현재 스택에 저장된 요소의 수

public:
    // 생성자
    LinkedStack() : top_(nullptr), size_(0) {}

    // 복사 금지
    LinkedStack(const LinkedStack&) = delete;
    LinkedStack& operator=(const LinkedStack&) = delete;

    // 이동 생성/대입 허용
    LinkedStack(LinkedStack&&) = default;
    LinkedStack& operator=(LinkedStack&&) = default;

    // 소멸자
    ~LinkedStack() override = default;

    // 스택이 비어있는지 확인
    [[nodiscard]] bool empty() const override {
        return size_ == 0;
    }

    // 현재 스택 크기 반환
    [[nodiscard]] size_t size() const override {
        return size_;
    }

    // 현재 스택 용량 반환 (연결 리스트는 이론적으로 무제한)
    [[nodiscard]] size_t capacity() const override {
        return size_t(-1);  // 최대값 반환
    }

    // 스택에 요소 추가
    void push(const T& value) override {
        auto new_node = std::make_unique<Node>(value);
        new_node->next = std::move(top_);
        top_ = std::move(new_node);
        ++size_;
    }

    // 스택에서 요소 제거
    void pop() override {
        if (empty()) {
            throw std::runtime_error("Stack is empty");
        }
        top_ = std::move(top_->next);
        --size_;
    }

    // 최상단 요소 확인 (const 버전)
    const T& peek() const override {
        if (empty()) {
            throw std::runtime_error("Stack is empty");
        }
        return top_->data;
    }

    // 최상단 요소 확인
    T& peek() override {
        if (empty()) {
            throw std::runtime_error("Stack is empty");
        }
        return top_->data;
    }

    // 스택 초기화
    void clear() override {
        while (!empty()) {
            pop();
        }
    }

    // 스택 내용 출력
    void print() const override {
        if (empty()) {
            std::cout << "Stack is empty" << std::endl;
            return;
        }

        std::cout << "Stack (size=" << size_ << "): TOP [ ";
        Node* current = top_.get();
        while (current != nullptr) {
            std::cout << current->data << " ";
            current = current->next.get();
        }
        std::cout << "] BOTTOM" << std::endl;
    }
};

#endif // LINKED_STACK_HPP
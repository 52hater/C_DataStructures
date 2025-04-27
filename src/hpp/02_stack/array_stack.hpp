#ifndef ARRAY_STACK_HPP
#define ARRAY_STACK_HPP

#include <memory>
#include <stdexcept>
#include <iostream>
#include "i_stack.hpp"

template<typename T>
class ArrayStack : public IStack<T> {
private:
    static const size_t INITIAL_CAPACITY = 8;     // 초기 스택 크기
    static const size_t GROWTH_FACTOR = 2;        // 확장 비율
    static constexpr float SHRINK_THRESHOLD = 0.25f;  // 축소 임계값

    std::unique_ptr<T[]> elements_;         // 요소를 저장할 동적 배열
    size_t size_;                          // 현재 저장된 요소의 수
    size_t capacity_;                      // 현재 할당된 배열의 크기

    // 스택 크기 조정
    void resize(size_t new_capacity) {
        auto new_elements = std::make_unique<T[]>(new_capacity);
        for (size_t i = 0; i < size_; ++i) {
            new_elements[i] = std::move(elements_[i]);
        }
        elements_ = std::move(new_elements);
        capacity_ = new_capacity;
    }

public:
    // 생성자
    ArrayStack() : elements_(std::make_unique<T[]>(INITIAL_CAPACITY)),
                  size_(0),
                  capacity_(INITIAL_CAPACITY) {}

    // 복사 금지
    ArrayStack(const ArrayStack&) = delete;
    ArrayStack& operator=(const ArrayStack&) = delete;

    // 이동 생성/대입 허용
    ArrayStack(ArrayStack&&) = default;
    ArrayStack& operator=(ArrayStack&&) = default;

    // 소멸자
    ~ArrayStack() override = default;

    // 스택이 비어있는지 확인
    [[nodiscard]] bool empty() const override {
        return size_ == 0;
    }

    // 현재 스택 크기 반환
    [[nodiscard]] size_t size() const override {
        return size_;
    }

    // 현재 스택 용량 반환
    [[nodiscard]] size_t capacity() const override {
        return capacity_;
    }

    // 스택에 요소 추가
    void push(const T& value) override {
        if (size_ == capacity_) {
            resize(capacity_ * GROWTH_FACTOR);
        }
        elements_[size_++] = value;
    }

    // 스택에서 요소 제거
    void pop() override {
        if (empty()) {
            throw std::runtime_error("Stack is empty");
        }

        --size_;

        // 스택 크기가 충분히 작아지면 배열 크기 축소
        if (size_ < capacity_ * SHRINK_THRESHOLD && capacity_ > INITIAL_CAPACITY) {
            size_t new_capacity = capacity_ / GROWTH_FACTOR;
            if (new_capacity < INITIAL_CAPACITY) {
                new_capacity = INITIAL_CAPACITY;
            }
            resize(new_capacity);
        }
    }

    // 최상단 요소 확인 (const 버전)
    const T& peek() const override {
        if (empty()) {
            throw std::runtime_error("Stack is empty");
        }
        return elements_[size_ - 1];
    }

    // 최상단 요소 확인
    T& peek() override {
        if (empty()) {
            throw std::runtime_error("Stack is empty");
        }
        return elements_[size_ - 1];
    }

    // 스택 초기화
    void clear() override {
        size_ = 0;
        if (capacity_ > INITIAL_CAPACITY) {
            resize(INITIAL_CAPACITY);
        }
    }

    // 스택 내용 출력
    void print() const override {
        if (empty()) {
            std::cout << "Stack is empty" << std::endl;
            return;
        }

        std::cout << "Stack (size=" << size_ << ", capacity=" << capacity_ << "): TOP [ ";
        for (size_t i = size_; i > 0; --i) {
            std::cout << elements_[i - 1] << " ";
        }
        std::cout << "] BOTTOM" << std::endl;
    }
};

#endif // ARRAY_STACK_HPP
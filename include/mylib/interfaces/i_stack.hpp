#ifndef I_STACK_HPP
#define I_STACK_HPP

#include <cstddef>

template<typename T>
class IStack {
public:
    // 가상 소멸자
    virtual ~IStack() = default;

    // 스택 상태 확인
    [[nodiscard]] virtual bool empty() const = 0;
    [[nodiscard]] virtual size_t size() const = 0;
    [[nodiscard]] virtual size_t capacity() const = 0;

    // 데이터 삽입 및 삭제
    virtual void push(const T& value) = 0;
    virtual void pop() = 0;

    // 최상단 데이터 확인
    virtual const T& peek() const = 0;
    virtual T& peek() = 0;

    // 스택 관리
    virtual void clear() = 0;
    virtual void print() const = 0;
};

#endif // I_STACK_HPP
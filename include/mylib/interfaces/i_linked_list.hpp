#ifndef I_LINKED_LIST_HPP
#define I_LINKED_LIST_HPP

#include <cstddef>  // for size_t

template<typename T>
class ILinkedList {
public:
    // 가상 소멸자
    virtual ~ILinkedList() = default;

    // 리스트 상태 확인
    [[nodiscard]] virtual bool empty() const = 0;
    [[nodiscard]] virtual size_t size() const = 0;

    // 데이터 삽입
    virtual void push_front(const T& value) = 0;
    virtual void push_back(const T& value) = 0;
    virtual void insert_after(size_t index, const T& value) = 0;

    // 데이터 삭제
    virtual void pop_front() = 0;
    virtual void remove_at(size_t index) = 0;

    // 데이터 접근
    virtual T& at(size_t index) = 0;
    [[nodiscard]] virtual const T& at(size_t index) const = 0;

    // 출력
    virtual void print() const = 0;
};

#endif // I_LINKED_LIST_HPP
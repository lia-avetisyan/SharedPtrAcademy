#include <iostream>
#include <memory>

template <typename U>
class SharedPtr {
private:
    U* data;
    size_t* count; //shared count, * need that the same instance has same count, not static because one object should have different count from another and the count should differ for different objs
public:
    SharedPtr(): data(nullptr), count(new size_t(0)){}
    explicit SharedPtr(U* new_data) : data(new_data), count(new size_t(1)) {}
    SharedPtr(const SharedPtr& other): data(other.data), count(other.count) {
       if (data) ++(*count);
    }
    SharedPtr(SharedPtr&& other): data(other.data), count(other.count) {
        if (data) ++(*count);
        other.release();
    }
    ~SharedPtr() {
        std::cout << "dtor" << std::endl;
        release();
    }

    template <typename M>
    SharedPtr(std::unique_ptr<M> &&other) noexcept {
        data = other.release();
        *count = 1;
    }

    SharedPtr&operator=(SharedPtr &other) noexcept{
        if (this  != &other) {
            data = other.data;
            count = other.count
            ++(*count);
            return *this;
        }
    }
    SharedPtr&operator=(SharedPtr&& other) noexcept {
        if (this != &other) {
            data = other.data;
            count = other.count;
            ++(*count);
            other.release();

        }
        return *this;
    }
    void reset(U* new_data) {
        release();
        data = new_data;
        count = new size_t(new_data ? 1 : 0);
    }
    void swap(SharedPtr&other) noexcept {
        std::swap(data, other.data);
        std::swap(count, other.count);
    }
    U* get() const {
        return data;
    }
    U &operator*() const {
        return *data;
    }
    U *operator->() const {
        return data;
    }

private:
    void release() {
        if (data) {
            if (--(*count) == 0) {
                std::cout <<"deleted" << std::endl;
                delete data;
                delete count;
            }
        }
        data = nullptr;
        count = nullptr;
    }
public:
    size_t use_count() const {
        return *count;
    }

    operator bool() const {
        return data != nullptr;
    }
    bool operator==(const SharedPtr&other) const {
        return data == other.data;
    }
};
int main() {

    SharedPtr<int> shared_ptr(new int(10));
    std::cout << *shared_ptr << std::endl;
    std::cout << "count: " << shared_ptr.use_count()<< std::endl;
    auto ptr2 = shared_ptr;
    std::cout << "count: " << shared_ptr.use_count()<< std::endl;
    if (shared_ptr == ptr2) {
        std::cout << "shared_ptr is equal to ptr2" << std::endl;
    }
    else {
        std::cout << "shared_ptr is not equal to ptr2" << std::endl;
    }
    SharedPtr<int> ptr3(new int(10));
    if (shared_ptr == ptr3) {
        std::cout << "shared_ptr is equal to ptr3" << std::endl;
    }
    else {
        std::cout << "shared_ptr is not equal to ptr3" << std::endl;
    }


}
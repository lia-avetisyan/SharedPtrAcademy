#include <iostream>
#include <memory>

template <typename U>
class SharedPtr {
private:
    U* data;
    size_t* count; //shared count, * need that the same instance has same count, not static because one object should have different count from another and the count should differ for different objs
public:
    SharedPtr(): data(nullptr), count(new size_t(0)){}
    SharedPtr(U* new_data) : data(new_data), count(nullptr) {}
    SharedPtr(const SharedPtr& other): data(other.data), count(other.count) {
        ++(*count);
    }
    SharedPtr(SharedPtr&& other): data(other.data), count(other.count) {
        ++(*count);
        other.release();
    }
    template <typename M>
    SharedPtr(std::unique_ptr<M> &&other) noexcept {
        data = other.release();
        *count = 1;
    }

    SharedPtr&operator=(const SharedPtr &other) noexcept{
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
        (*count) = 1;
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
                delete data;
                delete count;
            }
        }
        data = nullptr;
        count = nullptr;
    }
};
int main() {

    SharedPtr<int> shared_ptr(new int);
}
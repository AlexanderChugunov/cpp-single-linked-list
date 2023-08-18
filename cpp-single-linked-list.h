#include <cassert>
#include <cstddef>
#include <iterator>
#include <string>
#include <utility>

template <typename Type>
class SingleLinkedList {
    // Узел списка
    struct Node {
        Node() = default;
        Type value;
        Node* next_node = nullptr;
        Node(const Type& val, Node* next) : value(val), next_node(next) {}
    };
        template <typename ValueType>
    class BasicIterator {
        friend class SingleLinkedList;
        explicit BasicIterator(Node* node) : ptr_(node) {}

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = Type;
        using difference_type = std::ptrdiff_t;
        using pointer = ValueType*;
        using reference = ValueType&;

        BasicIterator() = default;
        BasicIterator(const BasicIterator<Type>& other) noexcept : ptr_(other.ptr_)
        {}

        BasicIterator& operator=(const BasicIterator& rhs) = default;
        [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
            return ptr_ == rhs.ptr_;
        }
        [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
            return !(*this == rhs);
        }
        [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {
            return ptr_ == rhs.ptr_;
        }
        [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {
            return !(*this == rhs);
        }


        BasicIterator& operator++() noexcept {
            assert(ptr_ != nullptr);
            ptr_ = ptr_->next_node;
            return *this;
        }
        BasicIterator operator++(int) noexcept {
            BasicIterator iterator = *this;
            ++(*this);
            return iterator;
        }
        [[nodiscard]] reference operator*() const noexcept {
            assert(ptr_ != nullptr);
            return ptr_->value;
        }


        [[nodiscard]] pointer operator->() const noexcept {
            assert(ptr_ != nullptr);
            return &ptr_->value;
        }

    private:
        Node* ptr_;
    };
public:
    SingleLinkedList<Type>()
        : size_(0)
    {}
    ~SingleLinkedList<Type>() {
        Clear();
    }
    
    [[nodiscard]] size_t GetSize() const noexcept {
        return size_;
    }

    [[nodiscard]] bool IsEmpty() const noexcept {
        return size_ == 0;
    }
    void PushFront(const Type& value) {
        head_.next_node = new Node(value, head_.next_node);
        ++size_;
    }
    void PopFront() {
        if (IsEmpty()) {
            return;
        }
        Node* tmp_head = head_.next_node->next_node;
        delete head_.next_node;
        head_.next_node = tmp_head;
        --size_;
    }
    void Clear() noexcept {
        while (head_.next_node != nullptr) {
            auto ptr_copy = head_.next_node;
            head_.next_node = ptr_copy->next_node;
            delete ptr_copy;
        }
        size_ = 0;
    }
    SingleLinkedList(std::initializer_list<Type> values) {
        Create(values.begin(), values.end());

    }
 
    SingleLinkedList(const SingleLinkedList& other) {
        Create(other.begin(), other.end());
    }
    template <typename InputIterator>
    void Create(InputIterator begin_, InputIterator end_) {
        SingleLinkedList tmp;
        auto it = begin_;
        
        Node** node_ptr = &tmp.head_.next_node;
        while(it != end_) {
        *node_ptr = new Node(*it, nullptr);// выделяем память и копируем
        node_ptr = &((*node_ptr)->next_node);// переходим к следующему узлу
        ++it;
        ++tmp.size_;
        }
        swap(tmp);
    }
    SingleLinkedList& operator=(const SingleLinkedList& rhs) {
        assert(head_.next_node != rhs.head_.next_node);
        SingleLinkedList temp(rhs);
        this->swap(temp);
        return *this;
    }

    void swap(SingleLinkedList& other) noexcept {
        std::swap(head_.next_node, other.head_.next_node);
        std::swap(size_, other.size_);
    }

    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;

    using Iterator = BasicIterator<Type>;
    using ConstIterator = BasicIterator<const Type>;

    [[nodiscard]] Iterator begin() noexcept {
        return Iterator(head_.next_node);
    }
    [[nodiscard]] Iterator end() noexcept {
        return Iterator(nullptr);
    }
    [[nodiscard]] ConstIterator begin() const noexcept {
        return ConstIterator(head_.next_node);
    }
    [[nodiscard]] ConstIterator end() const noexcept {
        return ConstIterator(nullptr);
    }
    [[nodiscard]] ConstIterator cbegin() const noexcept {
        return begin();
    }
    [[nodiscard]] ConstIterator cend() const noexcept {
        return end();
    }
    [[nodiscard]] Iterator before_begin() noexcept {
        // Реализуйте самостоятельно
        return Iterator(&head_);
    }
    [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
        // Реализуйте самостоятельно
        return ConstIterator (const_cast<Node*>(&head_) );
    }
    [[nodiscard]] ConstIterator before_begin() const noexcept {
        // Реализуйте самостоятельно
        return cbefore_begin();
    }


    Iterator InsertAfter(ConstIterator iter, const Type& value) {
        assert(iter.ptr_ != nullptr);
        iter.ptr_->next_node = new Node(value, iter.ptr_->next_node);
        ++size_;
        return Iterator{ iter.ptr_->next_node };
    }

    Iterator EraseAfter(ConstIterator iter) noexcept {
        assert(!IsEmpty());
        assert(iter.ptr_ != nullptr);
        assert(iter.ptr_->next_node != nullptr);
        Node* temp = iter.ptr_->next_node->next_node;
        delete iter.ptr_->next_node;
        iter.ptr_->next_node = temp;
        --size_;
 
        return Iterator{ iter.ptr_->next_node };
    }

private:
    // Фиктивный узел, используется для вставки "перед первым элементом"
    Node head_;
    size_t size_;
};

template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
    lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs == rhs);
}

template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(rhs < lhs);
}

template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return rhs < lhs;
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs < rhs);
}


#pragma once
#include <cassert>
#include <cstddef>
#include <iterator>
#include <string>
#include <utility>
#include <algorithm>
#include <iostream>

template <typename Type>
class SingleLinkedList {
    struct Node {
        Node() = default;
        Node(const Type& val, Node* next)
            : value(val)
            , next_node(next) {
        }
        Type value;
        Node* next_node = nullptr;
    };
 
    template <typename ValueType>
    class BasicIterator {
    
        friend class SingleLinkedList;
        
        explicit BasicIterator(Node* node) {
            node_ = node;
        }

        public:
      
           using iterator_category = std::forward_iterator_tag;
           using value_type = Type;
           using difference_type = std::ptrdiff_t;
           using pointer = ValueType*;
           using reference = ValueType&;

           BasicIterator() = default;

           BasicIterator(const BasicIterator<Type>& other) noexcept {
              node_ = other.node_;
           }

           BasicIterator& operator=(const BasicIterator& rhs) = default;

           [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
               return node_ == rhs.node_;
           }

           [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
               return node_ != rhs.node_;
           }

           [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {
               return node_ == rhs.node_;
           }

           [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {
               return node_ != rhs.node_;
           }

           BasicIterator& operator++() noexcept {
               assert(node_ != nullptr);
               node_ = node_->next_node;
               return *this;
           }

           BasicIterator operator++(int) noexcept {
               assert(node_ != nullptr);               
               auto last_node = node_;
               node_ = node_->next_node;
               return BasicIterator(last_node);
           }

           [[nodiscard]] reference operator*() const noexcept {
               assert(node_ != nullptr);
               return (*node_).value;
           }

           [[nodiscard]] pointer operator->() const noexcept {
               assert(node_ != nullptr);
               return &(*node_).value;
           }

       private:
           Node* node_ = nullptr;
    };
    
public:

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
        return ConstIterator(head_.next_node);
    }

    [[nodiscard]] ConstIterator cend() const noexcept {
        return ConstIterator(nullptr);
    }

    SingleLinkedList(){ 
        size_ = 0;
    }
    ~SingleLinkedList(){
        Clear();
    }
    
    [[nodiscard]] Iterator before_begin() noexcept {
        return Iterator(&head_);
    }

    [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
        return ConstIterator(const_cast<Node*>(&head_));
    }

    [[nodiscard]] ConstIterator before_begin() const noexcept {
        return ConstIterator(const_cast<Node*>(&head_));
    }

    Iterator InsertAfter(ConstIterator pos, const Type& value) {
        assert(pos.node_ != nullptr);
        Node* node = new Node(value, pos.node_->next_node);
        pos.node_->next_node = node;
        size_++;
        return Iterator(node);
    }

    void PopFront() noexcept {
       auto first = head_.next_node;
       head_.next_node = first->next_node;
       delete first;
    }

    Iterator EraseAfter(ConstIterator pos) noexcept {
       assert(pos.node_ != nullptr);
       auto toErase = pos.node_->next_node;
       pos.node_->next_node = toErase->next_node;
       delete toErase;
       return Iterator(pos.node_->next_node);
    }
    
    void PushFront(const Type& value) {
       head_.next_node = new Node(value, head_.next_node);
       ++size_;
    }
    
    void Clear() noexcept {
       while (head_.next_node != nullptr) {
          Node* buf = head_.next_node;
          head_.next_node = buf->next_node;
          delete buf;
       }
       size_ = 0;
    }
    
    [[nodiscard]] size_t GetSize() const noexcept {
        return size_;
    }

    [[nodiscard]] bool IsEmpty() const noexcept {
        return !(size_ > 0);
    }
    
    SingleLinkedList(std::initializer_list<Type> values) {
        SingleLinkedList List;
        for(auto iter = (values.end()-1);iter!=values.begin();iter--){
           List.PushFront(*iter);
        }
        List.PushFront(*values.begin());
        wap(List);
    }

    SingleLinkedList(const SingleLinkedList& other) {
        size_ = 0;
        if(!other.IsEmpty()) {
           SingleLinkedList List;
           Iterator iter = List.before_begin();
           try {
                for(auto a : other){
                   List.InsertAfter(iter, a);
                   iter++;
                }
           } catch(...) {
               Clear();
               throw;
           }
           swap(List);
           List.Clear();
        }  
    }
    SingleLinkedList& operator=(const SingleLinkedList& rhs) {
        SingleLinkedList List(rhs);
        swap(List);
        return *this;
    }

    // Обменивает содержимое списков за время O(1)
    void swap(SingleLinkedList& other) noexcept {
    if (!other.IsEmpty()) {
       std::swap( head_.next_node, other.head_.next_node);
       std::swap( size_, other.size_);}  
    }

private:
    Node head_;
    size_t size_;
};

template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
    lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::equal(lhs.begin(),lhs.end(),rhs.begin(),rhs.end());
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(std::equal(lhs.begin(),lhs.end(),rhs.begin(),rhs.end()));
}

template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::lexicographical_compare(lhs.begin(),lhs.end(),rhs.begin(),rhs.end());
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
   return !(std::lexicographical_compare(rhs.begin(),rhs.end(),lhs.begin(),lhs.end()));
}

template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::lexicographical_compare(rhs.begin(),rhs.end(),lhs.begin(),lhs.end());
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(std::lexicographical_compare(lhs.begin(),lhs.end(),rhs.begin(),rhs.end()));
} 


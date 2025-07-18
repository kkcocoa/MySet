#ifndef MY_SET_H
#define MY_SET_H

#include <functional>
#include <iostream>

template <typename T>
struct Node {
    T val;
    Node* next;
    Node(const T& value) : val(value), next(nullptr) {}  // 构造函数
};
    
template <typename T, typename Compare = std::less<T>>
class my_set {
private:
    Node<T>* head;
    size_t size_;
    Compare comp;
public: 
    class Iterator {
    public:
        // 使自定义迭代器兼容STL算法
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;
    private:
        Node<T>* current;
    public:
        Iterator(Node<T>* node) : current(node) {}  
        
        const T& operator*() {  
            return current->val;
        }

        const Node<T>* operator->() {
            return current;
        }

        Iterator& operator++() {
            if (current != nullptr)
                current = current->next;
            return *this;
        }
        // 如果返回引用，可能会返回一个已经被销毁的临时对象（因为tmp是局部变量）
        // 导致未定义行为。
        Iterator operator++(int) {  
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        bool operator!=(const Iterator& other) const {
            return current != other.current;
        }
    };

    my_set(Compare c = Compare()) : head(nullptr), size_(0), comp(c) {}

    // 拷贝构造函数
    my_set(const my_set& other) : head(nullptr), size_(other.size_), comp(other.comp) {
        Node<T>* cur = other.head;
        Node<T>* tail = nullptr;
        while (cur) {
            Node<T>* newNode = new Node<T>(cur->val);
            if (!head) {
                head = newNode;
                tail = head;
            } else {
                tail->next = newNode;
                tail = tail->next;
            }
            cur = cur->next;
        }
    }

    // 赋值运算符重载
    my_set& operator=(const my_set& other) {
        if (this == &other) return *this; // 自赋值保护
        comp = other.comp;
        size_ = other.size_;
        this->clear();
        head = nullptr;
        Node<T>* cur = other.head;
        Node<T>* tail = nullptr;
        while (cur) {
            Node<T>* newNode = new Node<T>(cur->val);
            // if head is null
            if (!head) {
                head = newNode;
                tail = head;
            } else {
                tail->next = newNode;
                tail = tail->next;
            }
            cur = cur->next;
        }
        return *this;
    }

    ~my_set() {
        Node<T>* current = head;
        while (current != nullptr) {
            Node<T>* next = current->next;
            delete current;
            current = next;
        }
    }

// 插入和删除需要找到待插位置的前一个位置，因此不可用find查找
    void insert(T val) {
        if (find(val)) return;
        Node<T>* node = new Node<T>(val);
        if (head == nullptr || comp(val, head->val)) {
            node->next = head;
            head = node;
            ++size_;
            return;
        }
        Node<T>* current = head;
        while (current->next && comp(current->next->val, val)) {
            current = current->next;
        }
        node->next = current->next;
        current->next = node;
        ++size_;
    }

    void erase(T val) {  
        if (!find(val)){
            return ;
        }
        if (head == nullptr) return;

        if (head->val == val) {
            Node<T>* tmp = head;
            head = head->next;
            delete tmp;
            --size_;
            return;
        }

        Node<T>* current = find_parent(val);
        
        if (current->next != nullptr) {
            Node<T>* temp = current->next;
            current->next = current->next->next;
            delete temp;
            --size_;
        }

       
    }

    size_t size() const{
        return size_;
    }
    bool empty() const{
        if (!size_) return true;
        return false;
    }
// 查找函数返回指向结点的指针 
    Node<T>* find(const T& val) const{
        Node<T>* cur = head;

        while (cur) {
            if (cur->val == val) return cur;
            cur = cur->next;
        }
        return nullptr;
    }

    my_set<T> union_set(const my_set<T>& other) const{
        my_set<T> result;
        for (auto it = this->begin(); it != this->end(); ++it){
            result.insert(*it);
        }
        for (auto it = other.begin(); it != other.end(); ++it){
            result.insert(*it);
        }

        return result;
    }

    my_set<T> diff_set(const my_set<T>& other) const{
        my_set<T> result;
        for (auto it = this->begin(); it != this->end(); ++it){
            if (!other.find(*it)) result.insert(*it);
        }

        return result;
    }
    
    my_set<T> intersect_set(const my_set<T>& other) const{
        my_set<T> result;
        for (auto it = this->begin(); it != this->end(); ++it){
            if (other.find(*it)) result.insert(*it);
        }

        return result;
    }

    bool is_subset_of(const my_set& other) const {
        for (auto it = this->begin(); it != this->end(); it ++)
            if (!other.find(*it)) return false;
        return true;
    }
    void clear(){
        Node<T>* current = head;
        while (current != nullptr) {
            Node<T>* next = current->next;
            delete current;
            current = next;
        }
        head = nullptr;
        size_ = 0;
    }
// 返回第一个大于等于val的迭代器
    Iterator lower_bound(const T& val) const{
        Node<T>* cur = head;
        while (cur && comp(cur->val, val))
            cur = cur->next;
        return Iterator(cur);
    }
// 返回第一个大于val的迭代器, 注意判断条件的不同!
    Iterator upper_bound(const T& val) const{
        Node<T>* cur = head;
        while (cur && !comp(val, cur->val))
            cur = cur->next;
        return Iterator(cur);
    }

    Iterator begin() const{
        return Iterator(head);
    }

    Iterator end() const {
        return Iterator(nullptr);
    }
private:
    Node<T>* find_parent(const T& val){
        Node<T>* current = head;
        while (current->next != nullptr && comp(current->next->val, val)) {
            current = current->next;
        }
        return current;
    }
};

// 在类外部重载 operator<<
template <typename T, typename Compare>
std::ostream& operator<<(std::ostream& os, const my_set<T, Compare>& s) {
    os << "{";
    bool first = true;
    for (auto it = s.begin(); it != s.end(); ++it) {
        if (!first) os << ", ";
        os << *it;
        first = false;
    }
    os << "}";
    return os;
}

#endif
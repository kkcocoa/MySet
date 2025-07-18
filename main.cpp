#include <iostream>
#include <string>
#include <vector>
#include "headers/my_set.h"
#include <set>
#include <chrono>
#include <algorithm>
using namespace std;

int main() {
    //  1. 测试插入和遍历
    my_set<string> st;
    st.insert("b");
    st.insert("a");
    st.insert("c");
    st.insert("b"); // 重复插入
    cout << "st: " << st << endl; // {a, b, c}

    // 2. 测试删除
    st.erase("b");
    cout << "after erase b: " << st << endl; // {a, c}
    st.erase("x"); // 删除不存在元素
    cout << "after erase x: " << st << endl; // {a, c}

    // 3. 测试查找
    cout << "find a: " << (st.find("a") ? "yes" : "no") << endl; // yes
    cout << "find b: " << (st.find("b") ? "yes" : "no") << endl; // no

    // 4. 测试 size 和 empty
    cout << "size: " << st.size() << endl; // 2
    cout << "empty: " << (st.empty() ? "yes" : "no") << endl; // no
    st.erase("a");
    st.erase("c");
    cout << "empty after erase all: " << (st.empty() ? "yes" : "no") << endl; // yes

    // 5. 测试拷贝构造和赋值
    st.insert("x");
    st.insert("y");
    my_set<string> st2 = st; // 拷贝构造
    cout << "st2 (copy): " << st2 << endl;
    my_set<string> st3;
    st3 = st; // 赋值
    cout << "st3 (assign): " << st3 << endl;

    // 6. 测试并集、交集、差集
    my_set<string> s1, s2;
    s1.insert("a"); s1.insert("b"); s1.insert("c");
    s2.insert("b"); s2.insert("c"); s2.insert("d");
    cout << "s1: " << s1 << endl;
    cout << "s2: " << s2 << endl;
    cout << "union: " << s1.union_set(s2) << endl;      // {a, b, c, d}
    cout << "intersect: " << s1.intersect_set(s2) << endl; // {b, c}
    cout << "diff: " << s1.diff_set(s2) << endl;        // {a}

    // 7. 测试子集
    my_set<string> s3;
    s3.insert("b");
    cout << "s3 is subset of s1: " << (s3.is_subset_of(s1) ? "yes" : "no") << endl; // yes
    cout << "s1 is subset of s3: " << (s1.is_subset_of(s3) ? "yes" : "no") << endl; // no

    // 8. 测试 clear
    s1.clear();
    cout << "s1 after clear: " << s1 << ", size: " << s1.size() << endl;

    // 9. 测试自定义排序
    my_set<int, std::greater<int>> s4;
    s4.insert(1); s4.insert(3); s4.insert(2);
    cout << "s4 (greater): " << s4 << endl; // {3, 2, 1}

    // 10.测试lower_bound和upper_bound
    my_set<int> s;
    s.insert(1); s.insert(3); s.insert(5);
    cout <<  "目前集合中的内容为" << s << endl;
    auto it1 = s.lower_bound(3); // 指向3
    auto it2 = s.upper_bound(3); // 指向5
    if (it1 != s.end()) cout << "lower_bound(3):" <<  *it1 << endl;
    if (it2 != s.end()) cout << "upper_bound(3):" << *it2 << endl;

    //11. 性能对比测试
    const int N1 = 1000, N2 = 10000, N3 = 30000;
    for (int n : {N1, N2, N3}) {
        vector<int> data;
        for (int i = 0; i < n; ++i) data.push_back(rand());
        // my_set
        my_set<int> myset;
        auto t1 = chrono::high_resolution_clock::now();
        for (int x : data) myset.insert(x);
        auto t2 = chrono::high_resolution_clock::now();
        auto myset_insert = chrono::duration_cast<chrono::milliseconds>(t2 - t1).count();
        t1 = chrono::high_resolution_clock::now();
        for (int x : data) myset.find(x);
        t2 = chrono::high_resolution_clock::now();
        auto myset_find = chrono::duration_cast<chrono::milliseconds>(t2 - t1).count();
        // std::set
        set<int> stdset;
        t1 = chrono::high_resolution_clock::now();
        for (int x : data) stdset.insert(x);
        t2 = chrono::high_resolution_clock::now();
        auto stdset_insert = chrono::duration_cast<chrono::milliseconds>(t2 - t1).count();
        t1 = chrono::high_resolution_clock::now();
        for (int x : data) stdset.find(x);
        t2 = chrono::high_resolution_clock::now();
        auto stdset_find = chrono::duration_cast<chrono::milliseconds>(t2 - t1).count();
        cout << "N=" << n << ": my_set insert " << myset_insert << "ms, find " << myset_find << "ms; std::set insert " << stdset_insert << "ms, find " << stdset_find << "ms\n";
    }
    // 12.测试兼容stl库中的函数
    auto it = find(st.begin(), st.end(), "a");
    if (it != st.end()) {
        cout << "\"a\" found in st" << endl;
    } else {
        cout << "\"a\" not found in st" << endl;
    }
    // 13.测试my_set的初始大小
    cout << "初始大小为: ";
    cout << sizeof(my_set<int>) << "字节" << endl;
    return 0;
}
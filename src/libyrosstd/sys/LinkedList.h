// SPDX-License-Identifier: MulanPSL-2.0

/*

    ����

    ������2023��2��5�� ����ʡ��������ɽ��

*/

#pragma once

#include <libyrosstd/sys/types.h>


/**
 * ����ڵ㡣
 * ���ýṹǶ����Ҫ���������ӵĶ���ṹ��ͨ��ƫ�Ʒ�ʽ�õ����ݽṹ���塣
 */
struct LinkedListNode {
    LinkedListNode* prev;
    LinkedListNode* next;
};


class LinkedListIterator {

    typedef LinkedListIterator Self;
    typedef LinkedListNode ElementType;

public:
    LinkedListIterator(ElementType* node) { this->node = node;}

    ElementType& operator * () const { return *node; };

    Self& operator ++ () {
        node = node->next;
        return *this;
    };

    Self operator ++ (int) {
        auto tmp = *this;
        node = node->next;
        return tmp;
    }

    Self& operator -- () {
        node = node->prev;
        return *this;
    }

    Self operator -- (int) {
        auto tmp = *this;
        node = node->prev;
        return tmp;
    }

    friend bool operator == (const Self& a, const Self& b) {
        return a.node == b.node;
    }

    friend bool operator != (const Self& a, const Self& b) {
        return a.node != b.node;
    }


protected:
    ElementType* node;
};

struct LinkedList {
    LinkedListNode* head = nullptr;
    LinkedListNode* tail = nullptr;

    int length = 0;

    LinkedList& append(LinkedListNode* node);
    LinkedList& insert(LinkedListNode* node, int index);

    /**
     * ��λ���ж��Ƿ�ɲ��롣
     * 
     * @param judge ���� 0 ��ʾ���Բ��롣����������ʾ�������ҡ����ظ�����ʾȡ��������
     */
    LinkedList& insertWhen(
        LinkedListNode& node,
        const int64_t cargo, 
        int (* judge) (const int64_t cargo, LinkedListNode* before, LinkedListNode* after)
    );

    LinkedList& insertWhen(
        LinkedListNode* node,
        const int64_t cargo, 
        int (* judge) (const int64_t cargo, LinkedListNode* before, LinkedListNode* after)
    );

    LinkedList& pushBack(LinkedListNode* node);

    LinkedList& remove(LinkedListNode* node);
    
    LinkedListNode* removeAt(int index);
    LinkedListNode* removeFirst();
    LinkedListNode* removeLast();
    LinkedListNode* popBack();

    bool contains(LinkedListNode* node);

    LinkedListNode* getFirst();
    LinkedListNode* getLast();
    LinkedListNode* get(int index);

    int indexOf(LinkedListNode* node);

    bool isEmpty();
    bool isNotEmpty();

    void clear();

    void forEach(
        void (* callable) (
            LinkedListNode* node
        )
    );

    // ------ ������� ------

    inline LinkedListIterator begin() const {
        return LinkedListIterator(this->head);
    }

    inline LinkedListIterator end() const {
        return LinkedListIterator(nullptr);
    }
};





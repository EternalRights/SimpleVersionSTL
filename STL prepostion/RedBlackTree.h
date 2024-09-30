#pragma once

#include<iostream>
#include<string>
#include<sstream>

enum class Color { RED, BLACK };

template<typename Key, typename Value>
class RedBlackTree {
private:

    //��Ա���ݣ��ڵ�����
    class Node {
    public:
        Key key;
        Value value;
        Color color;
        Node* left;
        Node* right;
        Node* parent;

        //��Ҷ�ӽڵ㹹�캯��
        Node(const Key& k, const Value& v, Color c, Node* p = nullptr) :key(k), value(v), color(c), left(nullptr), right(nullptr), parent(p) {};

        //Ҷ�ӽڵ㹹�캯��
        Node() :color(Color::BLACK), left(nullptr), right(nullptr), parent(nullptr) {};
    };

    //��Ա����
    Node* root;
    size_t size;
    Node* Nil;

    //��ѯ�ڵ��Ա����
    Node* lookUp(Key key) {
        Node* cmpNode = root;

        while (cmpNode) {
            if (key < cmpNode->key)
                cmpNode = cmpNode->left;
            else if (key > cmpNode->key)
                cmpNode = cmpNode->right;
            else
                return cmpNode;
        }
        return cmpNode;
    }

    //����
    void rightRotate(Node* node) {
        Node* l_son = node->left;
        node->left = l_son->right;

        if (l_son->right)
            l_son->right->parent = node;

        l_son->parent = node->parent;

        if (!node->parent)
            root = l_son;
        else if (node->parent->left == node)
            node->parent->left = l_son;
        else
            node->parent->right = l_son;

        l_son->right = node;
        node->parent = l_son;
    }

    //����
    void leftRotate(Node* node) {
        Node* r_son = node->right;
        node->right = r_son->left;

        if (r_son->left)
            r_son->left->parent = node;

        r_son->parent = node->parent;

        if (!node->parent)
            root = r_son;
        else if (node->parent->left == node)
            node->parent->left = r_son;
        else
            node->parent->right = r_son;

        r_son->left = node;
        node->parent = r_son;
    }

    //�����޸�
    void insertFixup(Node* target) {
        //���ڵ�����Ҹ��ڵ���ɫΪ��ɫ��Υ��������ɫ����
        while (target->parent && target->parent->color == Color::RED) {

            //A.���ڵ���ү�ڵ���ڵ�
            if (target->parent->parent->left == target->parent) {

                Node* uncle = target->parent->parent->right;

                //a.��ڵ��������ڵ���ɫΪ��ɫ:��ɫ
                if (uncle && uncle->color == Color::RED) {
                    target->parent->color = Color::BLACK;
                    target->parent->parent->color = Color::RED;
                    uncle->color = Color::BLACK;
                    target = target->parent->parent;
                }

                //b.��ڵ㲻���ڻ�����ڵ���ɫΪ��ɫ����ת
                else {

                    if (target->parent->right == target) {
                        target = target->parent;
                        leftRotate(target);
                    }

                    target->parent->color = Color::BLACK;
                    target->parent->parent->color = Color::RED;
                    rightRotate(target->parent->parent);
                }
            }

            //B.���ڵ���ү�ڵ��ҽڵ�
            else {

                Node* uncle = target->parent->parent->left;

                //a.��ڵ��������ڵ���ɫΪ��ɫ����ɫ
                if (uncle && uncle->color == Color::RED) {
                    target->parent->color = Color::BLACK;
                    target->parent->parent->color = Color::RED;
                    uncle->color = Color::BLACK;
                    target = target->parent->parent;
                }

                //b.��ڵ㲻���ڻ�����ڵ���ɫΪ��ɫ����ת
                else {

                    if (target->parent->left == target) {
                        target = target->parent;
                        rightRotate(target);
                    }

                    target->parent->color = Color::BLACK;
                    target->parent->parent->color = Color::RED;
                    leftRotate(target->parent->parent);
                }
            }
        }

        //ע�⣺һ��Ҫȷ�����ڵ�Ϊ��ɫ��
        root->color = Color::BLACK;
    }

    //����
    void insertNode(const Key& key, const Value& value) {

        //�����½ڵ�Ĭ��Ϊ��ɫ���������������Ϊ��ɫ��Ҫ�����������жϽ������
        Node* newnode = new Node(key, value, Color::RED);//��ζ�ſ���ֱ��ʹ��deleteɾ��
        Node* cmpNode = root;
        Node* parent = nullptr;

        //������������ҵ��½ڵ�λ��
        while (cmpNode) {

            //Ԥ��cmpNode�ĸ��ڵ�,�Ա�ƴ��
            parent = cmpNode;

            if (newnode->key < cmpNode->key)
                cmpNode = cmpNode->left;
            else if (newnode->key > cmpNode->key)
                cmpNode = cmpNode->right;

            //����key��ȣ�����ԭ������Ѿ�����Ŀ�����ֵ,��ֱ�ӷ��ز�������
            else {
                delete newnode;
                return;
            }
        }

        //������ڵ���+1
        size++;

        newnode->parent = parent;

        if (!parent)
            root = newnode;
        else if (parent->key < newnode->key)
            parent->right = newnode;
        else
            parent->left = newnode;

        //�����޸����
        insertFixup(newnode);
    }

    //�������
    void inorderTraversal(Node* node)const {
        if (node) {
            inorderTraversal(node->left);
            std::cout << node->key << " ";
            std::cout << node->value << " ";
            inorderTraversal(node->right);
        }
    }

    //�ڵ��滻
    void replaceNode(Node* targetNode, Node* newNode) {
        if (!targetNode->parent)
            root = newNode;
        else if (targetNode->parent->left == targetNode)
            targetNode->parent->left = newNode;
        else
            targetNode->parent->right = newNode;
        if (newNode)
            newNode->parent = targetNode->parent;
    }

    //Ѱ��ĳ����������С�ڵ�
    Node* findMinimumNode(Node* node) {
        while (node->left) {
            node = node->left;
        }
        return node;
    }

    //ɾ���޸�
    void removeFixup(Node* node) {

        //�սڵ㣬ֱ�ӷ���
        if (node == Nil && node->parent == nullptr)
            return;

        while (node != root) {

            //A.ɾ���ڵ����丸�ڵ�����ӽڵ�
            if (node->parent->left == node) {
                Node* sibling = node->parent->right;

                //a.�ֵܽڵ��Ǻ�ɫ
                if (getColor(sibling) == Color::RED) {
                    setColor(sibling, Color::BLACK);
                    setColor(node->parent, Color::RED);
                    leftRotate(node->parent);
                    sibling = node->parent->right;
                }

                //b.�ֵܽڵ�����ӽڵ��Ϊ��ɫ
                if (getColor(sibling->right) == Color::BLACK && getColor(sibling->left) == Color::BLACK) {
                    setColor(sibling, Color::RED);
                    node = node->parent;

                    //�������:�����ڵ�Ϊ��ɫ��ֱ�ӱ�ɫ����ֹ
                    if (node->color == Color::RED) {
                        setColor(node, Color::BLACK);
                        node = root;
                    }

                    else {

                        //c.�ֵܽڵ����Ӻ�
                        if (getColor(sibling->right) == Color::BLACK) {
                            setColor(sibling->left, Color::BLACK);
                            setColor(sibling, Color::RED);
                            rightRotate(sibling);
                            sibling = node->parent->right;
                        }

                        //d.�ֵܽڵ����Ӻ�
                        setColor(sibling, getColor(node->parent));
                        setColor(sibling->right, Color::BLACK);
                        setColor(node->parent, Color::BLACK);
                        leftRotate(node->parent);
                        node = root;
                    }
                }

                //B.ɾ���ڵ����丸�ڵ�����ӽڵ㣬���϶Գ�
                else {
                    Node* sibling = node->parent->left;

                    if (getColor(sibling) == Color::RED) {
                        setColor(sibling, Color::BLACK);
                        setColor(node->parent, Color::RED);
                        rightRotate(node->parent);
                        sibling = node->parent->left;
                    }

                    if (getColor(sibling->right) == Color::BLACK && getColor(sibling->left) == Color::BLACK) {
                        setColor(sibling, Color::RED);
                        node = node->parent;
                        if (node->color == Color::RED) {
                            setColor(node, Color::BLACK);
                            node = root;
                        }
                    }

                    else {
                        if (getColor(sibling->left) == Color::BLACK) {
                            setColor(sibling->right, Color::BLACK);
                            setColor(sibling, Color::RED);
                            leftRotate(sibling);
                            sibling = node->parent->left;
                        }

                        setColor(sibling->left, Color::BLACK);
                        setColor(sibling, getColor(node->parent));
                        setColor(node->parent, Color::BLACK);
                        rightRotate(node->parent);
                        node = root;
                    }
                }
            }

            //��֤���ڵ��Ǻ�ɫ
            setColor(node, Color::BLACK);
        }
    }

    //��ȡ��ɫ����ָ��Ϊ��ɫ
    Color getColor(Node* node) {
        if (node == nullptr)
            return Color::BLACK;

        return node->color;
    }

    //������ɫ����ָ��ֱ�ӷ���
    void setColor(Node* node, Color color) {
        if (node == nullptr)
            return;
        node->color = color;
    }

    //�Ͽ�Nil����������
    void dieConnectNil() {

        if (Nil == nullptr)
            return;

        if (Nil->parent != nullptr) {

            if (Nil->parent->right == Nil)
                Nil->parent->right = nullptr;

            else
                Nil->parent->left = nullptr;
        }
    }

    //ɾ������
    void deleteNode(Node* del) {
        Node* rep = del;
        Node* parentRP;
        Node* child = nullptr;
        Color origCol = rep->color;

        //A.ɾ���ڵ������ӽڵ�,parentRP��Ϊɾ���޸���ڲ���
        if (!del->right) {
            rep = del->left;
            parentRP = del->parent;
            origCol = getColor(rep);
            replaceNode(del, rep);
        }

        //B.ɾ���ڵ������ӽڵ㣬parentRP��Ϊɾ���޸���ڲ���
        else if (!del->left) {
            rep = del->right;
            parentRP = del->parent;
            origCol = getColor(rep);
            replaceNode(del, rep);
        }

        //C.ɾ���ڵ�ĸ��ڵ���˫�ӽڵ�
        else {
            rep = findMinimumNode(del->right);
            origCol = rep->color;

            //a.����ڵ��ɾ���ڵ��ֱ�ӽڵ㣬ɾ���޸���ڲ���ȡ�����Ƿ����ӽڵ�
            if (del->right != rep) {
                parentRP = rep->parent;

                //1.��������ڵ�ĺ��ӽڵ�
                child = rep->right;
                parentRP->left = child;

                if (child != nullptr)
                    child->parent = parentRP;

                //2.����ڵ���ɾ���ڵ���滻���ӽڵ㷽�棩
                rep->left = del->left;
                rep->right = del->right;
                del->left->parent = rep;
                del->right->parent = rep;

                //3.����ڵ�ĸ��ڵ�ĸı�
                replaceNode(del, rep);
            }

            //b.����ڵ���ɾ���ڵ��ֱ�ӽڵ㣬ɾ���޸���ڲ���ȡ�����Ƿ����ӽڵ�
            else {
                //1.����ڵ���ɾ���ڵ���滻���ӽڵ㷽�棩
                child = rep->right;
                rep->left = del->left;
                del->left->parent = rep;

                //2.����ڵ�ĸ��ڵ�ĸı�
                replaceNode(del, rep);

                parentRP = rep;//Ϊ��rep���ڵ�ָ��rep����  ԭ�򣺴�Ϊ�������������rep���Һ��ӣ�����ôrep�滻֮����ڲ��������Ϊ�޸ĺ��delλ�ô������ʱ����½ڵ��������ˣ�
            }
        }

        //������ɫ
        //����ڵ���ɫ��ɾ������¼��origCol
        if (rep != nullptr)
            rep->color = del->color;

        //ɾ���ڵ���ɫ��ɾ������¼��origCol
        else
            origCol = del->color;

        //ɾ����ɫ�Ǻ�ɫ�������޲�����ɫ����
        if (origCol == Color::BLACK) {

            //���ӽڵ���ڣ��Ӻ��ӽڵ��޲�
            if (child != nullptr)
                removeFixup(child);

            //���ӽڵ㲻���ڣ�����Nil��β�ڵ㣩
            else {
                Nil->parent = parentRP;

                if (parentRP != nullptr) {
                    if (parentRP->left == nullptr)
                        parentRP->left = Nil;
                    else
                        parentRP->right = Nil;
                }

                removeFixup(Nil);

                dieConnectNil();
            }
        }

        delete del;
    }

public:
    //���캯��
    RedBlackTree() :root(nullptr), size(0), Nil(new Node()) {
        Nil->color = Color::BLACK;
    }

    //����
    void insert(const Key& key, const Value& value) {
        insertNode(key, value);
    }

    //ɾ��
    void remove(const Key& key) {
        Node* nodeToBeRemoved = lookUp(key);

        if (nodeToBeRemoved != nullptr)
            deleteNode(nodeToBeRemoved);

        size--;
    }

    //��ȡ��ַ
    Value* at(const Key& key) {
        auto ans = lookUp(key);

        if (ans != nullptr)
            return &ans->value;

        return nullptr;
    }

    //��ӡ
    void print() {
        inorderTraversal(root);
        std::cout << std::endl;
    }

    //����
    size_t getSize() {
        return size;
    }

    //�ж��Ƿ�Ϊ��
    bool empty() {
        return size == 0;
    }

    //����
    void clear() {
        deleteTree(root);
        size = 0;
    }

    //��������
    ~RedBlackTree() {
        deleteTree(root);
    }

private:
    //����ɾ������
    void deleteTree(Node* node) {
        if (node) {
            deleteTree(node->left);
            deleteTree(node->right);
            delete node;
        }
    }
};
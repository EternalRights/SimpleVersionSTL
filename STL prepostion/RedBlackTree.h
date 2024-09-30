#pragma once

#include<iostream>
#include<string>
#include<sstream>

enum class Color { RED, BLACK };

template<typename Key, typename Value>
class RedBlackTree {
private:

    //成员数据：节点类型
    class Node {
    public:
        Key key;
        Value value;
        Color color;
        Node* left;
        Node* right;
        Node* parent;

        //非叶子节点构造函数
        Node(const Key& k, const Value& v, Color c, Node* p = nullptr) :key(k), value(v), color(c), left(nullptr), right(nullptr), parent(p) {};

        //叶子节点构造函数
        Node() :color(Color::BLACK), left(nullptr), right(nullptr), parent(nullptr) {};
    };

    //成员数据
    Node* root;
    size_t size;
    Node* Nil;

    //查询节点成员函数
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

    //右旋
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

    //左旋
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

    //插入修复
    void insertFixup(Node* target) {
        //父节点存在且父节点颜色为红色，违反连续红色规则
        while (target->parent && target->parent->color == Color::RED) {

            //A.父节点是爷节点左节点
            if (target->parent->parent->left == target->parent) {

                Node* uncle = target->parent->parent->right;

                //a.叔节点存在且叔节点颜色为红色:变色
                if (uncle && uncle->color == Color::RED) {
                    target->parent->color = Color::BLACK;
                    target->parent->parent->color = Color::RED;
                    uncle->color = Color::BLACK;
                    target = target->parent->parent;
                }

                //b.叔节点不存在或者叔节点颜色为黑色：旋转
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

            //B.父节点是爷节点右节点
            else {

                Node* uncle = target->parent->parent->left;

                //a.叔节点存在且叔节点颜色为红色：变色
                if (uncle && uncle->color == Color::RED) {
                    target->parent->color = Color::BLACK;
                    target->parent->parent->color = Color::RED;
                    uncle->color = Color::BLACK;
                    target = target->parent->parent;
                }

                //b.叔节点不存在或者叔节点颜色为黑色：旋转
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

        //注意：一定要确保根节点为黑色！
        root->color = Color::BLACK;
    }

    //插入
    void insertNode(const Key& key, const Value& value) {

        //设置新节点默认为红色，这样相较于设置为黑色需要处理的情况与判断将会减少
        Node* newnode = new Node(key, value, Color::RED);//意味着可以直接使用delete删除
        Node* cmpNode = root;
        Node* parent = nullptr;

        //遍历红黑树，找到新节点位置
        while (cmpNode) {

            //预留cmpNode的父节点,以便拼接
            parent = cmpNode;

            if (newnode->key < cmpNode->key)
                cmpNode = cmpNode->left;
            else if (newnode->key > cmpNode->key)
                cmpNode = cmpNode->right;

            //如若key相等，代表原红黑树已经存在目标插入值,则直接返回不做插入
            else {
                delete newnode;
                return;
            }
        }

        //红黑树节点数+1
        size++;

        newnode->parent = parent;

        if (!parent)
            root = newnode;
        else if (parent->key < newnode->key)
            parent->right = newnode;
        else
            parent->left = newnode;

        //插入修复入口
        insertFixup(newnode);
    }

    //中序遍历
    void inorderTraversal(Node* node)const {
        if (node) {
            inorderTraversal(node->left);
            std::cout << node->key << " ";
            std::cout << node->value << " ";
            inorderTraversal(node->right);
        }
    }

    //节点替换
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

    //寻找某二叉树中最小节点
    Node* findMinimumNode(Node* node) {
        while (node->left) {
            node = node->left;
        }
        return node;
    }

    //删除修复
    void removeFixup(Node* node) {

        //空节点，直接返回
        if (node == Nil && node->parent == nullptr)
            return;

        while (node != root) {

            //A.删除节点是其父节点的左子节点
            if (node->parent->left == node) {
                Node* sibling = node->parent->right;

                //a.兄弟节点是红色
                if (getColor(sibling) == Color::RED) {
                    setColor(sibling, Color::BLACK);
                    setColor(node->parent, Color::RED);
                    leftRotate(node->parent);
                    sibling = node->parent->right;
                }

                //b.兄弟节点的两子节点皆为黑色
                if (getColor(sibling->right) == Color::BLACK && getColor(sibling->left) == Color::BLACK) {
                    setColor(sibling, Color::RED);
                    node = node->parent;

                    //特殊情况:若父节点为红色，直接变色，终止
                    if (node->color == Color::RED) {
                        setColor(node, Color::BLACK);
                        node = root;
                    }

                    else {

                        //c.兄弟节点左子红
                        if (getColor(sibling->right) == Color::BLACK) {
                            setColor(sibling->left, Color::BLACK);
                            setColor(sibling, Color::RED);
                            rightRotate(sibling);
                            sibling = node->parent->right;
                        }

                        //d.兄弟节点右子红
                        setColor(sibling, getColor(node->parent));
                        setColor(sibling->right, Color::BLACK);
                        setColor(node->parent, Color::BLACK);
                        leftRotate(node->parent);
                        node = root;
                    }
                }

                //B.删除节点是其父节点的右子节点，与上对称
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

            //保证根节点是黑色
            setColor(node, Color::BLACK);
        }
    }

    //获取颜色，空指针为黑色
    Color getColor(Node* node) {
        if (node == nullptr)
            return Color::BLACK;

        return node->color;
    }

    //设置颜色，空指针直接返回
    void setColor(Node* node, Color color) {
        if (node == nullptr)
            return;
        node->color = color;
    }

    //断开Nil与树的链接
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

    //删除操作
    void deleteNode(Node* del) {
        Node* rep = del;
        Node* parentRP;
        Node* child = nullptr;
        Color origCol = rep->color;

        //A.删除节点无右子节点,parentRP作为删除修复入口参数
        if (!del->right) {
            rep = del->left;
            parentRP = del->parent;
            origCol = getColor(rep);
            replaceNode(del, rep);
        }

        //B.删除节点无左子节点，parentRP作为删除修复入口参数
        else if (!del->left) {
            rep = del->right;
            parentRP = del->parent;
            origCol = getColor(rep);
            replaceNode(del, rep);
        }

        //C.删除节点的父节点有双子节点
        else {
            rep = findMinimumNode(del->right);
            origCol = rep->color;

            //a.替代节点非删除节点的直接节点，删除修复入口参数取决于是否有子节点
            if (del->right != rep) {
                parentRP = rep->parent;

                //1.处理替代节点的孩子节点
                child = rep->right;
                parentRP->left = child;

                if (child != nullptr)
                    child->parent = parentRP;

                //2.替代节点与删除节点的替换（子节点方面）
                rep->left = del->left;
                rep->right = del->right;
                del->left->parent = rep;
                del->right->parent = rep;

                //3.替代节点的父节点的改变
                replaceNode(del, rep);
            }

            //b.替代节点是删除节点的直接节点，删除修复入口参数取决于是否有子节点
            else {
                //1.替代节点与删除节点的替换（子节点方面）
                child = rep->right;
                rep->left = del->left;
                del->left->parent = rep;

                //2.替代节点的父节点的改变
                replaceNode(del, rep);

                parentRP = rep;//为何rep父节点指向rep自身？  原因：此为特殊情况——若rep无右孩子，，那么rep替换之后，入口参数将会变为修改后的del位置处，这个时候更新节点必须满足此，
            }
        }

        //分配颜色
        //替代节点颜色被删除，记录于origCol
        if (rep != nullptr)
            rep->color = del->color;

        //删除节点颜色被删除，记录于origCol
        else
            origCol = del->color;

        //删除颜色是黑色，则需修补；红色不需
        if (origCol == Color::BLACK) {

            //孩子节点存在，从孩子节点修补
            if (child != nullptr)
                removeFixup(child);

            //孩子节点不存在，利用Nil（尾节点）
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
    //构造函数
    RedBlackTree() :root(nullptr), size(0), Nil(new Node()) {
        Nil->color = Color::BLACK;
    }

    //插入
    void insert(const Key& key, const Value& value) {
        insertNode(key, value);
    }

    //删除
    void remove(const Key& key) {
        Node* nodeToBeRemoved = lookUp(key);

        if (nodeToBeRemoved != nullptr)
            deleteNode(nodeToBeRemoved);

        size--;
    }

    //提取地址
    Value* at(const Key& key) {
        auto ans = lookUp(key);

        if (ans != nullptr)
            return &ans->value;

        return nullptr;
    }

    //打印
    void print() {
        inorderTraversal(root);
        std::cout << std::endl;
    }

    //数量
    size_t getSize() {
        return size;
    }

    //判断是否为空
    bool empty() {
        return size == 0;
    }

    //清理
    void clear() {
        deleteTree(root);
        size = 0;
    }

    //析构函数
    ~RedBlackTree() {
        deleteTree(root);
    }

private:
    //遍历删除数据
    void deleteTree(Node* node) {
        if (node) {
            deleteTree(node->left);
            deleteTree(node->right);
            delete node;
        }
    }
};
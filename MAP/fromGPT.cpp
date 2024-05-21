// 左旋（左-右旋）操作
void leftRotate(Node* x) {
    Node* y = x->right;
    x->right = y->left;
    if (y->left != nullptr) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == nullptr) {
        root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

// 右旋（右-左旋）操作
void rightRotate(Node* x) {
    Node* y = x->left;
    x->left = y->right;
    if (y->right != nullptr) {
        y->right->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == nullptr) {
        root = y;
    } else if (x == x->parent->right) {
        x->parent->right = y;
    } else {
        x->parent->left = y;
    }
    y->right = x;
    x->parent = y;
}

// 左旋（左-左旋）操作
void leftLeftRotate(Node* x) {
    Node* y = x->parent;
    Node* z = y->parent;
    y->parent = z->parent;
    if (z->parent == nullptr) {
        root = y;
    } else if (z == z->parent->left) {
        z->parent->left = y;
    } else {
        z->parent->right = y;
    }
    z->parent = y;
    z->left = y->right;
    if (y->right != nullptr) {
        y->right->parent = z;
    }
    y->right = z;
}

// 右旋（右-右旋）操作
void rightRightRotate(Node* x) {
    Node* y = x->parent;
    Node* z = y->parent;
    y->parent = z->parent;
    if (z->parent == nullptr) {
        root = y;
    } else if (z == z->parent->left) {
        z->parent->left = y;
    } else {
        z->parent->right = y;
    }
    z->parent = y;
    z->right = y->left;
    if (y->left != nullptr) {
        y->left->parent = z;
    }
    y->left = z;
}

/**
 * 当从红黑树中删除一个节点时，需要执行以下步骤：

找到要删除的节点，如果不存在则直接返回。
如果要删除的节点有两个非空子节点，则需要找到其后继节点（即大于该节点的最小节点），然后将后继节点的值复制到要删除的节点中，并删除后继节点。
如果要删除的节点只有一个或零个子节点，则直接删除该节点。
如果删除的节点是黑色节点，则需要执行 removeAdjust 操作来恢复红黑树的平衡性质。
下面是一个红黑树的 remove 函数示例代码：
*/

// 从红黑树中删除节点
void remove(int value) {
    Node* z = findNode(value);
    if (z == nullptr) {
        return; // 节点不存在，直接返回
    }
    Node* x = nullptr;
    Node* y = z;
    Color y_original_color = y->color;
    if (z->left == nullptr) {
        x = z->right;
        transplant(z, z->right);
    } else if (z->right == nullptr) {
        x = z->left;
        transplant(z, z->left);
    } else {
        y = minimum(z->right);
        y_original_color = y->color;
        x = y->right;
        if (y->parent == z) {
            x->parent = y;
        } else {
            transplant(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        transplant(z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    if (y_original_color == Color::BLACK) {
        removeAdjust(x);
    }
    delete z;
}

/*
其中，transplant 函数用于将一个节点移植到另一个节点的位置，并更新相应的父节点和子节点指针。minimum 函数用于查找以某个节点为根的子树中的最小节点。

removeAdjust 函数用于在删除一个黑色节点后，恢复红黑树的平衡性质。下面是一个红黑树的 removeAdjust 函数示例代码：
*/

// 删除节点后恢复红黑树的平衡性质
void removeAdjust(Node* x) {
    while (x != root && x->color == Color::BLACK) {
        if (x == x->parent->left) {
            Node* w = x->parent->right;
            if (w->color == Color::RED) {
                w->color = Color::BLACK;
                x->parent->color = Color::RED;
                leftRotate(x->parent);
                w = x->parent->right;
            }
            if (w->left->color == Color::BLACK && w->right->color == Color::BLACK) {
                w->color = Color::RED;
                x = x->parent;
            } else {
                if (w->right->color == Color::BLACK) {
                    w->left->color = Color::BLACK;
                    w->color = Color::RED;
                    rightRotate(w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = Color::BLACK;
                w->right->color = Color::BLACK;
                leftRotate(x->parent);
                x = root;
            }
        } else {
            Node* w = x->parent->left;
            if (w->color == Color::RED) {
                w->color = Color::BLACK;
                x->parent->color = Color::RED;
                rightRotate(x->parent);
                w = x->parent->left;
            }
            if (w->left->color == Color::BLACK && w->right->color == Color::BLACK) {
                w->color = Color::RED;
                x = x->parent;
            } else {
                if (w->left->color == Color::BLACK) {
                    w->right->color = Color::BLACK;
                    w->color = Color::RED;
                    leftRotate(w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = Color::BLACK;
                w->left->color = Color::BLACK;
                rightRotate(x->parent);
                x = root;
            }
        }
    }
    x->color = Color::BLACK;
}

/*
当前节点为红色节点，直接将其染成黑色即可，因为删除的是黑色节点，其父节点的黑色节点数目减一，而红色节点不会影响黑色节点数目。
当前节点为黑色节点，其兄弟节点为红色节点，此时将其兄弟节点染成黑色，其父节点染成红色，然后对其父节点进行左旋或右旋操作（具体左旋还是右旋取决于当前节点在其父节点的位置），使得当前节点的兄弟节点成为其父节点的新子节点。
当前节点为黑色节点，其兄弟节点为黑色节点，且其兄弟节点的子节点都为黑色节点，此时将其兄弟节点染成红色，然后将当前节点上移一层，继续进行操作。
当前节点为黑色节点，其兄弟节点为黑色节点，且其兄弟节点至少有一个红色子节点，此时需要对其兄弟节点的子节点进行染色和旋转操作，使得其兄弟节点的一个子节点变成其新的兄弟节点，然后继续按照上述情况进行处理。
*/
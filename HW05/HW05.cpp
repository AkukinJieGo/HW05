#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <iomanip>
using namespace std;

// AVL 樹節點結構
struct AVLNode {
    int data;               // 儲存節點資料
    AVLNode* left;          // 左子樹
    AVLNode* right;         // 右子樹
    int height;             // 標註節點高度
    AVLNode(int val) : data(val), left(nullptr), right(nullptr), height(1) {} // 建構函數初始化
};

// 2-3 樹節點結構
struct Tree23Node {
    int data[2];            // 最多存兩個鍵值
    Tree23Node* child[3];   // 最多存三個子節點
    int size;               // 鍵值數量
    bool isLeaf;            // 是否為葉節點
    Tree23Node() : size(0), isLeaf(true) {  // 建構函數初始化
        for (int i = 0; i < 3; i++) child[i] = nullptr;  // 初始化子節點為 null
    }
};

// 取得 AVL 樹節點的高度
int height(AVLNode* node) {
    return node ? node->height : 0;  // 如果節點存在，回傳高度，否則回傳 0
}

// 取得 AVL 樹節點的平衡因子
int getBalance(AVLNode* node) {
    return node ? height(node->left) - height(node->right) : 0;  // 計算左右子樹的高度差
}

// AVL 樹右旋
AVLNode* rightRotate(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = max(height(y->left), height(y->right)) + 1;  // 更新 y 節點的高度
    x->height = max(height(x->left), height(x->right)) + 1;  // 更新 x 節點的高度
    return x;  // 返回新的根節點 x
}

// AVL 樹左旋
AVLNode* leftRotate(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = max(height(x->left), height(x->right)) + 1;  // 更新 x 節點的高度
    y->height = max(height(y->left), height(y->right)) + 1;  // 更新 y 節點的高度
    return y;  // 返回新的根節點 y
}

// 插入到 AVL 樹
AVLNode* insertAVL(AVLNode* node, int key) {
    if (!node) return new AVLNode(key);  // 如果樹是空的，建立新節點
    if (key < node->data) node->left = insertAVL(node->left, key);  // 插入到左子樹
    else if (key > node->data) node->right = insertAVL(node->right, key);  // 插入到右子樹
    else return node;  // 如果鍵值已經存在，直接返回

    node->height = 1 + max(height(node->left), height(node->right));  // 更新當前節點的高度
    int balance = getBalance(node);  // 計算平衡因子

    // 進行旋轉以保持平衡
    if (balance > 1 && key < node->left->data)
        return rightRotate(node);  // 左左情況，進行右旋
    if (balance < -1 && key > node->right->data)
        return leftRotate(node);  // 右右情況，進行左旋
    if (balance > 1 && key > node->left->data) {
        node->left = leftRotate(node->left);  // 左右情況，先進行左旋再右旋
        return rightRotate(node);
    }
    if (balance < -1 && key < node->right->data) {
        node->right = rightRotate(node->right);  // 右左情況，先進行右旋再左旋
        return leftRotate(node);
    }
    return node;  // 返回不變的節點
}

// 自上而下打印 AVL 樹為樹狀圖
void printAVLTree(AVLNode* root, int space = 0, int level_gap = 10) {
    if (!root) return;  // 如果樹是空的，什麼都不做
    space += level_gap;
    printAVLTree(root->right, space);  // 先遞歸右子樹
    cout << endl;
    for (int i = level_gap; i < space; i++) cout << " ";  // 輸出空格來顯示樹的結構
    cout << root->data << endl;  // 輸出當前節點資料
    printAVLTree(root->left, space);  // 再遞歸左子樹
}

// 將節點分裂並推升到父節點
Tree23Node* split23(Tree23Node* parent, Tree23Node* node, int key, Tree23Node* child) {
    int temp[3] = { node->data[0], node->data[1], key };

    // 排序 temp 陣列中的元素，確保升序
    if (temp[0] > temp[1]) std::swap(temp[0], temp[1]);
    if (temp[1] > temp[2]) std::swap(temp[1], temp[2]);
    if (temp[0] > temp[1]) std::swap(temp[0], temp[1]);

    Tree23Node* newChild = new Tree23Node();  // 創建新節點
    // 初始化 newChild 節點的 data 和 child
    for (int i = 0; i < 3; i++) {
        newChild->data[i] = 0;  // 初始化為 0
        newChild->child[i] = nullptr;  // 初始化子節點為 null
    }

    // 根據 temp 來處理分裂
    if (key < temp[0]) {
        newChild->data[0] = temp[2];
        newChild->size = 1;
        node->size = 1;
    }
    else if (key < temp[1]) {
        newChild->data[0] = temp[2];
        newChild->size = 1;
        node->size = 1;
    }
    else {
        newChild->data[0] = temp[2];
        newChild->size = 1;
        node->size = 1;
    }

    // 如果沒有父節點，創建新的根節點
    if (!parent) {
        Tree23Node* newRoot = new Tree23Node();
        newRoot->data[0] = temp[1];  // 根節點資料
        newRoot->child[0] = node;    // 左子樹
        newRoot->child[1] = newChild; // 右子樹
        newRoot->size = 1;
        newRoot->isLeaf = false;  // 設置根節點不是葉節點
        return newRoot;  // 返回新創建的根節點
    }

    // 如果父節點存在，則將分裂結果傳遞給父節點
    return split23(nullptr, parent, temp[1], newChild);
}

// 插入到 2-3 樹
Tree23Node* insert23(Tree23Node* root, int key) {
    if (!root) {
        Tree23Node* node = new Tree23Node();
        node->data[0] = key;
        node->size = 1;
        return node;  // 如果樹是空的，創建新的根節點
    }

    Tree23Node* node = root;
    Tree23Node* parent = nullptr;
    while (!node->isLeaf) {  // 迴圈直到找到葉節點
        parent = node;
        if (key < node->data[0]) node = node->child[0];  // 插入到左子樹
        else if (node->size == 1 || key < node->data[1]) node = node->child[1];  // 插入到中間子樹
        else node = node->child[2];  // 插入到右子樹
    }

    // 如果節點只有一個鍵值
    if (node->size == 1) {
        if (key < node->data[0]) {
            node->data[1] = node->data[0];
            node->data[0] = key;  // 將新鍵值插入到左邊
        }
        else {
            node->data[1] = key;  // 將新鍵值插入到右邊
        }
        node->size = 2;  // 更新節點的鍵值數量
    }
    else {
        return split23(parent, node, key, nullptr);  // 如果節點已經有兩個鍵值，進行分裂
    }

    return root;  // 返回根節點
}

// 打印 2-3 樹為樹狀圖
void print23Tree(Tree23Node* root, int space = 0, int level_gap = 10) {
    if (!root) return;  // 如果樹是空的，什麼都不做
    space += level_gap;
    if (root->size == 2) {
        print23Tree(root->child[2], space);  // 遞歸右子樹
    }
    print23Tree(root->child[1], space);  // 然後是中間子樹
    cout << endl;
    for (int i = level_gap; i < space; i++) cout << " ";  // 輸出空格來顯示樹的結構
    cout << "[ ";
    for (int i = 0; i < root->size; i++) cout << root->data[i] << " ";  // 輸出當前節點資料
    cout << "]" << endl;
    print23Tree(root->child[0], space);  // 最後是左子樹
}

int main() {
    AVLNode* avlRoot = nullptr;          // AVL 樹的根節點
    Tree23Node* tree23Root = nullptr;    // 2-3 樹的根節點
    vector<int> numbers;                 // 儲存輸入的數字
    int n;

    // 輸入數字
    cout << "請輸入多個數字，以空格分隔，並以 -1 結束輸入：" << endl;
    while (cin >> n && n != -1) {
        numbers.push_back(n);  // 讀取數字並儲存到 vector
    }

    // 將數字插入到 AVL 樹和 2-3 樹
    for (int num : numbers) {
        avlRoot = insertAVL(avlRoot, num);    // 插入到 AVL 樹
        tree23Root = insert23(tree23Root, num);  // 插入到 2-3 樹
    }

    // 打印結果
    cout << "\nAVL 樹結構（樹狀圖）：" << endl;
    printAVLTree(avlRoot);  // 輸出 AVL 樹

    cout << "\n2-3 樹結構（樹狀圖）：" << endl;
    print23Tree(tree23Root);  // 輸出 2-3 樹

    return 0;
}

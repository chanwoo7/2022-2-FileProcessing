#include <iostream>
#include <stack>
using namespace std;

// 20213062 ������ - B-Tree ����Ʈ

// - ��� ���� ����
// ����: ����
// ����: ����

struct Node {
    int n;  // ������ �ִ� Ű�� ����
    int* K;  // Ű���� ��� �ִ� �迭
    Node** P;  // ������(�ڽ� ���)���� ��� �ִ� �迭

public:
    Node(int m) {
        n = 0;
        K = new int[m];  // Ű �迭 ����
        P = new Node * [m + 1];  // ������ �迭 ����

        for (int i = 0; i < m; i++) {  // Ű �迭 �ʱ�ȭ
            K[i] = 0;
        }
        for (int i = 0; i < m + 1; i++) {  // ������ �迭 �ʱ�ȭ
            P[i] = nullptr;
        }
    }
};

Node* getNode(int m) {
    Node* newNode = new Node(m);  // Node ����ü�� �����ڸ� �̿��� �� ��� ����
    return newNode;
}

bool searchPath(Node*& T, int m, int key, stack<Node*>& stack) {
    Node* x = T;

    int i = 0;

    do {
        i = 0;

        while (i < x->n && key > x->K[i])
            i++;

        // ������ Ű�� ���� �߰����� ����
        stack.push(x);

        // ������ Ű�� �߰���. ���� �Ұ�.
        if (i < x->n && key == x->K[i])
            return true;
    } while ((x = x->P[i]) != nullptr);

    return false;

}

void insertKey(Node*& T, int m, Node*& x, Node*& y, int newKey) {
    // newKey���� ū Ű���� ���������� �� ĭ�� �̵�
    int i = x->n - 1;
    
    while (i >= 0 && newKey < x->K[i]) {
        x->K[i + 1] = x->K[i];
        x->P[i + 1] = x->P[i];
        i--;
    }

    // newKey�� ����
    x->K[i + 1] = newKey;
    x->P[i + 1] = y;
    x->n++;
}

Node* splitNode(Node*& T, int m, Node*& x, Node*& y, int& newKey) {
    Node* tempNode = getNode(m);  // �����÷θ� ���� �ӽ� ��� (x�� newKey�� ����)

    // x�� ������ tempNode�� ����
    tempNode->n = x->n;
    for (int i = 0; i < m; i++) {
        tempNode->K[i] = x->K[i];
    }
    for (int i = 0; i < m + 1; i++) {
        tempNode->P[i] = x->P[i];
    }

    insertKey(T, m, tempNode, y, newKey);

    newKey = tempNode->K[T->n / 2];  // ���� ������ �߾Ӱ�

    x->n = 0;  // centerKey ���� ���� ��� x�� ����
    int i = 0;
    while (tempNode->K[i] < newKey) {
        x->K[i] = tempNode->K[i];
        x->P[i] = tempNode->P[i];
        i++;
        x->n++;
    }
    x->P[i] = tempNode->P[i];

    Node* newNode = getNode(m);  // centerKey ���� ���� ��� newNode�� ����
    i++;
    while (i <= tempNode->n) {
        newNode->K[newNode->n] = tempNode->K[i];
        newNode->P[newNode->n] = tempNode->P[i];
        i++;
        newNode->n++;
    }
    newNode->P[newNode->n] = tempNode->P[i];
    return newNode;
}

bool insertBT(Node*& T, int m, int newKey) {
    // ��Ʈ ��� ����
    if (T == nullptr) {
        T = getNode(m);
        T->K[0] = newKey;
        T->n = 1;
        return true;
    }

    // newKey�� ������ ����� ��θ� Ž���ϸ�, ���ÿ� ��� ����
    stack<Node*> stack;
    bool found = searchPath(T, m, newKey, stack);

    if (found == true)
        return false;

    // newKey�� �����Ƿ�, T�� ���� (���� x�� null)
    bool finished = false;
    Node* x = stack.top();
    stack.pop();
    Node* y = nullptr;

    do {
        // Overflow �߻� ���� �˻�
        if (x->n < m - 1) {  // Overflow �߻� ����. newKey�� ��� x�� �ùٸ� ��ġ�� ����
            insertKey(T, m, x, y, newKey);
            finished = true;
        }
        else {  // Overflow �߻�
            // x�� newKey�� �������� ����, ���ҵ� ��� ��ȯ
            y = splitNode(T, m, x, y, newKey);

            if (!stack.empty()) {
                x = stack.top();
                stack.pop();
            }
            else {
                T = getNode(m);
                T->K[0] = newKey;
                T->P[0] = x;
                T->P[1] = y;
                // T->n++;
                finished = true;
            }
        }
    } while (!finished);

    return true;
}

void inorderBT(Node* T) {
    // T�� leaf ����� �� Ű�� ���
    if (T->P[0] == nullptr) {
        for (int i = 0; i < T->n; i++) {
            cout << T->K[i] << " ";
        }
    }
    
    // T�� leaf ��尡 �ƴ� �� �ڽ� ���� ����� ȣ��
    int i = 0;
    while (T->P[0] != nullptr && i < T->n) {
        inorderBT(T->P[i++]);
    }
}

int main(void) {
    int m = 3;
    Node* tree = nullptr;  // Ʈ�� ����

    while (true) {
        if (cin.eof() == true)
            break;

        char command;
        int num;
        cin >> command >> num;

        if (command == 'i') {
            if (!insertBT(tree, m, num)) {
                cout << "i " << num << " : The key already exists" << endl;
            }
        }

        inorderBT(tree);
        cout << endl;

    }

    delete tree;

}
#include <iostream>
#include <stack>
using namespace std;

// 20213062 이찬우 - B-Tree 리포트

// - 기능 성공 여부
// 삽입: 실패
// 삭제: 실패

struct Node {
    int n;  // 가지고 있는 키의 개수
    int* K;  // 키들을 담고 있는 배열
    Node** P;  // 포인터(자식 노드)들을 담고 있는 배열

public:
    Node(int m) {
        n = 0;
        K = new int[m];  // 키 배열 생성
        P = new Node * [m + 1];  // 포인터 배열 생성

        for (int i = 0; i < m; i++) {  // 키 배열 초기화
            K[i] = 0;
        }
        for (int i = 0; i < m + 1; i++) {  // 포인터 배열 초기화
            P[i] = nullptr;
        }
    }
};

Node* getNode(int m) {
    Node* newNode = new Node(m);  // Node 구조체의 생성자를 이용해 새 노드 생성
    return newNode;
}

bool searchPath(Node*& T, int m, int key, stack<Node*>& stack) {
    Node* x = T;

    int i = 0;

    do {
        i = 0;

        while (i < x->n && key > x->K[i])
            i++;

        // 삽입할 키를 아직 발견하지 못함
        stack.push(x);

        // 삽입할 키를 발견함. 삽입 불가.
        if (i < x->n && key == x->K[i])
            return true;
    } while ((x = x->P[i]) != nullptr);

    return false;

}

void insertKey(Node*& T, int m, Node*& x, Node*& y, int newKey) {
    // newKey보다 큰 키들을 오른쪽으로 한 칸씩 이동
    int i = x->n - 1;
    
    while (i >= 0 && newKey < x->K[i]) {
        x->K[i + 1] = x->K[i];
        x->P[i + 1] = x->P[i];
        i--;
    }

    // newKey를 삽입
    x->K[i + 1] = newKey;
    x->P[i + 1] = y;
    x->n++;
}

Node* splitNode(Node*& T, int m, Node*& x, Node*& y, int& newKey) {
    Node* tempNode = getNode(m);  // 오버플로를 위한 임시 노드 (x와 newKey를 저장)

    // x의 값들을 tempNode에 복사
    tempNode->n = x->n;
    for (int i = 0; i < m; i++) {
        tempNode->K[i] = x->K[i];
    }
    for (int i = 0; i < m + 1; i++) {
        tempNode->P[i] = x->P[i];
    }

    insertKey(T, m, tempNode, y, newKey);

    newKey = tempNode->K[T->n / 2];  // 분할 기준인 중앙값

    x->n = 0;  // centerKey 이전 값을 노드 x로 복사
    int i = 0;
    while (tempNode->K[i] < newKey) {
        x->K[i] = tempNode->K[i];
        x->P[i] = tempNode->P[i];
        i++;
        x->n++;
    }
    x->P[i] = tempNode->P[i];

    Node* newNode = getNode(m);  // centerKey 이후 값을 노드 newNode로 복사
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
    // 루트 노드 생성
    if (T == nullptr) {
        T = getNode(m);
        T->K[0] = newKey;
        T->n = 1;
        return true;
    }

    // newKey를 삽입할 노드의 경로를 탐색하며, 스택에 경로 저장
    stack<Node*> stack;
    bool found = searchPath(T, m, newKey, stack);

    if (found == true)
        return false;

    // newKey가 없으므로, T에 삽입 (이제 x는 null)
    bool finished = false;
    Node* x = stack.top();
    stack.pop();
    Node* y = nullptr;

    do {
        // Overflow 발생 여부 검사
        if (x->n < m - 1) {  // Overflow 발생 안함. newKey를 노드 x의 올바른 위치에 삽입
            insertKey(T, m, x, y, newKey);
            finished = true;
        }
        else {  // Overflow 발생
            // x를 newKey를 기준으로 분할, 분할된 노드 반환
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
    // T가 leaf 노드일 때 키값 출력
    if (T->P[0] == nullptr) {
        for (int i = 0; i < T->n; i++) {
            cout << T->K[i] << " ";
        }
    }
    
    // T가 leaf 노드가 아닐 때 자식 노드들 재귀적 호출
    int i = 0;
    while (T->P[0] != nullptr && i < T->n) {
        inorderBT(T->P[i++]);
    }
}

int main(void) {
    int m = 3;
    Node* tree = nullptr;  // 트리 생성

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
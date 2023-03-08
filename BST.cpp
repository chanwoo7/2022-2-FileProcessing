#include <iostream>
#include <stack>
using namespace std;

// 20213062 이찬우 - BST 리포트

struct Node {
    int key;
    struct Node *left;
    struct Node *right;

    Node() : key(0), left(NULL), right(NULL) {};
};


Node* getNode(void) {
    Node* newNode = new Node();  // Node 구조체의 생성자를 이용해 새 노드 생성
    return newNode;
}


Node* searchBST(Node* T, int searchKey) {
    Node* p = T;
    if (p == NULL)  // 최종적으로 searchkey를 찾지 못하면 NULL 반환
        return NULL;
    if (searchKey == p->key)  // searchKey를 찾은 경우
        return p;
    if (searchKey > p->key)  // searchKey가 키값보다 큰 경우 -> 오른쪽 서브트리에서 재탐색
        return searchBST(p->right, searchKey);
    else  // searchKey가 키값보다 작은 경우 -> 왼쪽 서브트리에서 재탐색
        return searchBST(p->left, searchKey);
}


Node* searchParent(Node* T, int searchKey) {
    Node* q = NULL;  // p의 부모 노드, 초기값은 NULL (p가 NULL이거나 p가 루트이면 그대로 NULL 반환)
    Node* p = T;
    while (p != NULL) {
        if (searchKey == p->key)  // searchKey를 찾은 경우
            return q;
        q = p;
        if (searchKey < p->key) // searchKey가 키값보다 작은 경우 -> 왼쪽 서브트리에서 재탐색
            p = p->left;
        else  // searchKey가 키값보다 큰 경우 -> 오른쪽 서브트리에서 재탐색
            p = p->right;
    }
    return q;
}


int height(Node* T) {
    // 트리를 매개변수로 받아 트리의 높이 계산
    if (T == NULL)  // 트리가 비어 있다면 0 반환
        return 0;
    int l_height = height(T->left);
    int r_height = height(T->right);
    // 왼쪽 서브트리의 height와 오른쪽 서브트리의 height 중 더 큰 값에 1(루트 노드의 높이)을 더함
    return 1 + ((l_height > r_height) ? l_height : r_height);
}


Node* maxNode(Node* T) {
    // 트리의 최대 오른쪽으로 이동 -> 최댓값 도출
    if (T->right != NULL)
        maxNode(T->right);
    else return T;
}


Node* minNode(Node* T) {
    // 트리의 최대 왼쪽으로 이동 -> 최솟값 도출
    if (T->left != NULL)
        minNode(T->left);
    else return T;
}


int noNodes(Node* T) {
    if (T == NULL)  // 트리가 비어 있다면 0 반환
        return 0;
    // 양쪽 서브트리의 노드 수의 합에 1(자기 자신)을 더함
    return 1 + noNodes(T->left) + noNodes(T->right);
}

// insertBST
bool insertBST(Node*& T, int newKey) {
    Node *p = T;
    Node *q = NULL;

    if (p->key == 0) {  // 키 값이 0인 경우 (트리의 초기 상태로 사용함)
        p->key = newKey;  // 새 키값이 0을 대체함
        return true;
    }
    
    while (p != NULL) {  
        if (newKey == p->key)  // 삽입하고자 하는 값이 이미 있는 경우 -> 삽입 실패
            return false;

        q = p;

        if (newKey < p->key)
            p = p->left;
        else
            p = p->right;
    }

    Node* newNode = getNode();  // 새 노드 생성
    newNode->key = newKey;

    if (T == NULL)  // 트리가 비어 있는 경우
        T = newNode;
    else if (newKey < q->key)
        q->left = newNode;
    else
        q->right = newNode;

    return true;
}

// deleteBST
bool deleteBST(Node*& T, int deleteKey) {
    Node* p = searchBST(T, deleteKey);
    Node* q = searchParent(T, deleteKey);

    if (p == NULL)  // 삭제하고자 하는 값이 발견되지 않은 경우 -> 삭제 실패
        return false;

    if (p->left == NULL && p->right == NULL) {  // 삭제할 노드의 차수가 0인 경우
        if (q == NULL) {  // 루트 노드일 경우
            T = NULL;
            return true;
        }
        if (q->left == p)
            q->left = NULL;
        else
            q->right = NULL;
    }
    else if (p->left == NULL || p->right == NULL) {  // 삭제할 노드의 차수가 1인 경우
        if (p->left != NULL) {  // 그 중 왼쪽 자식 노드만 있는 경우
            if (q == NULL) {  // 루트 노드일 경우
                p->key = p->left->key;
                p->left = p->left->left;
                return true;
            }
            if (q->left == p)
                q->left = p->left;
            else 
                q->right = p->left;
        }
        else {  // 그 중 오른쪽 자식 노드만 있는 경우
            if (q == NULL) {  // 루트 노드일 경우
                p->key = p->right->key;
                p->right = p->right->right;
                return true;
            }
            if (q->left == p)
                q->left = p->right;
            else
                q->right = p->right;
        }
    }
    else {  // 삭제할 노드의 차수가 2인 경우
        Node* r;  // 삭제될 노드 자리에 키값이 복사될 노드
        char flag;  // r 키값이 삭제될 서브트리 방향

        if (height(p->left) > height(p->right)) {
            r = maxNode(p->left);
            flag = 'l';
        }
        else if (height(p->left) < height(p->right)) {
            r = minNode(p->right);
            flag = 'r';
        }
        else {  // 왼쪽 서브트리의 높이 = 오른쪽 서브트리의 높이
            if (noNodes(p->left) >= noNodes(p->right)) {
                r = maxNode(p->left);
                flag = 'l';
            }
            else {
                r = minNode(p->right);
                flag = 'r';
            }
        }
        
        p->key = r->key;  // 키값 복사
        
        if (flag == 'l')
            deleteBST(p->left, r->key);  // 왼쪽 서브트리에서 삭제
        else
            deleteBST(p->right, r->key);  // 오른쪽 서브트리에서 삭제
    }

    return true;
}


void inorderBST(Node* T) {
    // inorder 순회
    if (T != NULL) {
        inorderBST(T->left);
        cout << T->key << " ";
        inorderBST(T->right);
    }
}


int main(void) {
    Node* tree = new Node();  // 트리 생성

    while (true) {
        if (cin.eof() == true)  // 텍스트 파일의 끝을 감지하면 반복문 탈출
            break;
        
        char command;  // 명령어 i 또는 d
        int num;  // 숫자
        cin >> command >> num;

        // 삽입
        if (command == 'i') {
            if (!insertBST(tree, num))  // 삽입에 실패할 경우 (이미 키가 존재할 때)
                cout << 'i' << " " << num << " : The key already exists" << endl;
        }
        // 삭제
        else if (command == 'd') {
            if (!deleteBST(tree, num))  // 삭제에 실패할 경우 (해당 키가 존재하지 않을 때)
                cout << 'd' << " " << num << " : The key does not exist" << endl;
        }

        // inorder 출력
        inorderBST(tree);
        cout << endl;
    }

    delete tree;

}

#include <iostream>
#include <stack>
using namespace std;

// 20213062 이찬우 - AVL 리포트

struct Node {
    int key;
    int height;
    int bf;
    struct Node* left;
    struct Node* right;

    Node() : key(0), height(0), bf(0), left(NULL), right(NULL) {};
};


Node* getNode(void) {
    Node* newNode = new Node();  // Node 구조체의 생성자를 이용해 새 노드 생성
    return newNode;
}


Node* searchBST(Node*& T, int searchKey) {
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


Node* searchParent(Node*& T, int searchKey) {
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

void updateHeight(Node* T) {
    // 트리의 노드들 높이 업데이트
    if (T == NULL)
        return;
    T->height = height(T);
    updateHeight(T->left);
    updateHeight(T->right);
}

int bf(Node* T) {
    // 노드의 bf값 구함
    if (T == NULL)
        return 0;
    int l_height = (T->left == NULL) ? 0 : height(T->left);
    int r_height = (T->right == NULL) ? 0 : height(T->right);
    return l_height - r_height;
}

void updateBF(Node* T) {
    // 트리 노드들의 BF값 업데이트
    if (T == NULL)
        return;
    T->bf = bf(T);
    updateBF(T->left);
    updateBF(T->right);
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
    Node* p = T;
    Node* q = NULL;

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
Node* deleteBST(Node*& T, int deleteKey) {
    Node* p = searchBST(T, deleteKey);
    Node* q = searchParent(T, deleteKey);
    // Node* return_q = NULL;  // 삭제한 노드의 부모 노드 (리턴할 때 사용)

    if (p == NULL)  // 삭제하고자 하는 값이 발견되지 않은 경우 -> 삭제 실패
        return NULL;

    if (p->left == NULL && p->right == NULL) {  // 삭제할 노드의 차수가 0인 경우
        if (q == NULL) {  // 루트 노드일 경우
            T = NULL;
            return NULL;
        }
        if (q->left == p)
            q->left = NULL;
        else
            q->right = NULL;

        return q;
    }
    else if (p->left == NULL || p->right == NULL) {  // 삭제할 노드의 차수가 1인 경우
        if (p->left != NULL) {  // 그 중 왼쪽 자식 노드만 있는 경우
            if (q == NULL) {  // 루트 노드일 경우
                p->key = p->left->key;
                p->left = p->left->left;
                return NULL;
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
                return NULL;
            }
            if (q->left == p)
                q->left = p->right;
            else
                q->right = p->right;
        }

        return q;
    }
    else {  // 삭제할 노드의 차수가 2인 경우
        Node* r;  // 삭제될 노드 자리에 키값이 복사될 노드
        char flag;  // r 키값이 삭제될 서브트리 방향

        if (height(p->left) > height(p->right)) {
            r = maxNode(p->left);
            flag = 'l';
        }
        else {
            r = minNode(p->right);
            flag = 'r';
        }
        p->key = r->key;  // 키값 복사

        if (flag == 'l') {
            if (p->key == p->left->key) {
                p->left = p->left->left;
                return p;
            }
            deleteBST(p->left, r->key);  // 왼쪽 서브트리에서 삭제
        }
        else {
            if (p->key == p->right->key) {
                p->right = p->right->right;
                return p;
            }
            deleteBST(p->right, r->key);  // 오른쪽 서브트리에서 삭제
        }
        return q;
    }
}

// checkBalance 함수의 반환값을 담은 구조체
struct checkBalanceResult {
    std::string rotationType;  // 회전 형식
    Node* p;  // 불균형 노드
    Node* q;  // 불균형 노드의 부모 노드
    
    checkBalanceResult() : rotationType("NO"), p(NULL), q(NULL) {};
};

checkBalanceResult* checkBalance(Node*& T, int newKey) {
    checkBalanceResult* result = new checkBalanceResult();
    Node* f = NULL;  // 처음으로 불균형이 감지된 노드의 부모 노드
    Node* a = NULL;  // 처음으로 불균형이 감지된 노드
    Node* p = T;  // 탐색을 위한 포인터 변수
    Node* q = NULL;  // newKey의 위치

    // newKey 위치 탐색
    while (p != NULL) {
        p->bf = bf(p);
        if (p->bf < -1 || p->bf > 1) {  // 탐색 중 불균형 발견한 경우
            a = p;
            f = q;
        }

        q = p;

        if (newKey < p->key)
            p = p->left;
        else if (newKey > p->key)
            p = p->right;
        else
            break;
    }

    if (p == NULL)  // newKey를 찾지 못할 경우
        return result;

    if (a == NULL)  // 불균형(bf의 절댓값이 1 초과)인 노드를 찾지 못한 경우
        return result;

    // 최초로 균형이 깨진 노드부터 newKey까지 bf를 다시 계산
    Node* b = NULL;  // 최초의 불균형 노드의 newKey 방향쪽 자식 노드

    if (newKey < a->key) {
        p = a->left;
        b = p;
    }
    else {
        p = a->right;
        b = p;
    }
    while (p != q) {
        if (newKey < p->key) 
            p = p->left;
        else
            p = p->right;
    }

    // 회전 유형 결정
    string rotationType;

    if (a->bf > 1 && b->bf > 0)  // LL 회전 필요
        rotationType = "LL";
    else if (a->bf > 1 && b->bf < 0)  // LR 회전 필요
        rotationType = "LR";
    else if (a->bf < 1 && b->bf > 0)  // RL 회전 필요
        rotationType = "RL";
    else if (a->bf < 1 && b->bf < 0)  // RR 회전 필요
        rotationType = "RR";

    result->p = a;
    result->q = f;
    result->rotationType = rotationType;

    return result;
}

void rotateTree(Node*& T, string rotationType, Node* p, Node* q) {
    Node* a = p;
    Node* b = NULL;
    Node* c = NULL;

    if (rotationType == "LL") {  // LL 회전
        b = p->left;

        a->left = b->right;
        b->right = a;
    }
    else if (rotationType == "LR") {  // LR 회전
        b = p->left;
        c = b->right;

        b->right = c->left;
        a->left = c->right;
        c->left = b;
        c->right = a;
    }
    else if (rotationType == "RL") {  // RL 회전
        b = p->right;
        c = b->left;

        b->left = c->right;
        a->right = c->left;
        c->right = b;
        c->left = a;
    }
    else if (rotationType == "RR") {  // RR 회전
        b = p->right;

        a->right = b->left;
        b->left = a;
    }

    if (rotationType == "LL" || rotationType == "RR") {
        if (q == NULL)
            T = b;
        
        else if (q->left == a)
            q->left = b;
        else  // q->right == a
            q->right = b;
    }
    else {  // rotationType == "LR" || rotationType == "RL"
        if (q == NULL)
            T = c;
        else if (q->left == a)
            q->left = c;
        else  // q->right == a
            q->right = c;
    }

    // bf 갱신
    updateBF(T);
}
 
string insertAVL(Node*& T, int newKey) {
    if (T == NULL) {  // 트리가 비어 있을 
        T = getNode();
        T->key = newKey;
        return "NO";
    }
    
    if (!insertBST(T, newKey))  // 삽입 실패 시 (키가 없으면) FAIL 리턴
        return "FAIL";

    checkBalanceResult* checkBalanceResult = checkBalance(T, newKey);  // checkBalanceResult 변수에 값 3개 저장
    string rotationType = checkBalanceResult->rotationType;
    Node* p = checkBalanceResult->p;
    Node* q = checkBalanceResult->q;

    if (rotationType != "NO")  // 회전이 필요한 경우
        rotateTree(T, rotationType, p, q);

    updateHeight(T);

    return rotationType;  // 회전 형식 스트링으로 리턴
}

string deleteAVL(Node*& T, int deleteKey) {
    if (searchBST(T, deleteKey) == NULL)  // 삭제 실패 시 (키가 없으면) FAIL 리턴
        return "FAIL";

    Node* q = deleteBST(T, deleteKey);

    updateHeight(T);

    if (q == NULL)  // 불균형(bf의 절댓값이 1 초과)인 노드를 찾지 못한 경우
        return "NO";

    checkBalanceResult* checkBalanceResult = checkBalance(T, q->key);  // checkBalanceResult 변수에 값 3개 저장
    string rotationType = checkBalanceResult->rotationType;
    Node* p_ = checkBalanceResult->p;
    Node* q_ = checkBalanceResult->q;

    if (rotationType != "NO")  // 회전이 필요한 경우
        rotateTree(T, rotationType, p_, q_);

    updateHeight(T);
    updateBF(T);

    return rotationType;  // 회전 형식 스트링으로 리턴
}

void inorderBST(Node* T) {
    // inorder 순회
    if (T != NULL) {
        inorderBST(T->left);
        cout << "(" << T->key << ", " << T->bf << ") ";
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
            string rotationType = insertAVL(tree, num);
            if (rotationType == "FAIL")
                cout << "i " << num << " : The key already exists" << endl;
            else
                cout << rotationType << " ";
        }

        // 삭제
        if (command == 'd') {
            if (num == 33) {
                cout << "";
            }
            string rotationType = deleteAVL(tree, num);
            if (rotationType == "FAIL")
                cout << "i " << num << " : The key does not exist" << endl;
            else
                cout << rotationType << " ";
        }

        // inorder 출력
        inorderBST(tree);
        cout << endl;
        /*printLink(tree);
        cout << endl;*/
    }

    delete tree;

}

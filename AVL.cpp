#include <iostream>
#include <stack>
using namespace std;

// 20213062 ������ - AVL ����Ʈ

// - ��� ���� ����
// ����: ����
// ����: ����

struct Node {
    int key;
    int height;
    int bf;
    struct Node* left;
    struct Node* right;

    Node() : key(0), height(0), bf(0), left(NULL), right(NULL) {};
};


Node* getNode(void) {
    Node* newNode = new Node();  // Node ����ü�� �����ڸ� �̿��� �� ��� ����
    return newNode;
}


Node* searchBST(Node*& T, int searchKey) {
    Node* p = T;
    if (p == NULL)  // ���������� searchkey�� ã�� ���ϸ� NULL ��ȯ
        return NULL;
    if (searchKey == p->key)  // searchKey�� ã�� ���
        return p;
    if (searchKey > p->key)  // searchKey�� Ű������ ū ��� -> ������ ����Ʈ������ ��Ž��
        return searchBST(p->right, searchKey);
    else  // searchKey�� Ű������ ���� ��� -> ���� ����Ʈ������ ��Ž��
        return searchBST(p->left, searchKey);
}


Node* searchParent(Node*& T, int searchKey) {
    Node* q = NULL;  // p�� �θ� ���, �ʱⰪ�� NULL (p�� NULL�̰ų� p�� ��Ʈ�̸� �״�� NULL ��ȯ)
    Node* p = T;
    while (p != NULL) {
        if (searchKey == p->key)  // searchKey�� ã�� ���
            return q;
        q = p;
        if (searchKey < p->key) // searchKey�� Ű������ ���� ��� -> ���� ����Ʈ������ ��Ž��
            p = p->left;
        else  // searchKey�� Ű������ ū ��� -> ������ ����Ʈ������ ��Ž��
            p = p->right;
    }
    return q;
}


int height(Node* T) {
    // Ʈ���� �Ű������� �޾� Ʈ���� ���� ���
    if (T == NULL)  // Ʈ���� ��� �ִٸ� 0 ��ȯ
        return 0;
    int l_height = height(T->left);
    int r_height = height(T->right);
    // ���� ����Ʈ���� height�� ������ ����Ʈ���� height �� �� ū ���� 1(��Ʈ ����� ����)�� ����
    return 1 + ((l_height > r_height) ? l_height : r_height);
}

void updateHeight(Node* T) {
    // Ʈ���� ���� ���� ������Ʈ
    if (T == NULL)
        return;
    T->height = height(T);
    updateHeight(T->left);
    updateHeight(T->right);
}

int bf(Node* T) {
    // ����� bf�� ����
    if (T == NULL)
        return 0;
    int l_height = (T->left == NULL) ? 0 : height(T->left);
    int r_height = (T->right == NULL) ? 0 : height(T->right);
    return l_height - r_height;
}

void updateBF(Node* T) {
    // Ʈ�� ������ BF�� ������Ʈ
    if (T == NULL)
        return;
    T->bf = bf(T);
    updateBF(T->left);
    updateBF(T->right);
}

Node* maxNode(Node* T) {
    // Ʈ���� �ִ� ���������� �̵� -> �ִ� ����
    if (T->right != NULL)
        maxNode(T->right);
    else return T;
}


Node* minNode(Node* T) {
    // Ʈ���� �ִ� �������� �̵� -> �ּڰ� ����
    if (T->left != NULL)
        minNode(T->left);
    else return T;
}


int noNodes(Node* T) {
    if (T == NULL)  // Ʈ���� ��� �ִٸ� 0 ��ȯ
        return 0;
    // ���� ����Ʈ���� ��� ���� �տ� 1(�ڱ� �ڽ�)�� ����
    return 1 + noNodes(T->left) + noNodes(T->right);
}

// insertBST
bool insertBST(Node*& T, int newKey) {
    Node* p = T;
    Node* q = NULL;

    if (p->key == 0) {  // Ű ���� 0�� ��� (Ʈ���� �ʱ� ���·� �����)
        p->key = newKey;  // �� Ű���� 0�� ��ü��
        return true;
    }

    while (p != NULL) {
        if (newKey == p->key)  // �����ϰ��� �ϴ� ���� �̹� �ִ� ��� -> ���� ����
            return false;

        q = p;

        if (newKey < p->key)
            p = p->left;
        else
            p = p->right;
    }

    Node* newNode = getNode();  // �� ��� ����
    newNode->key = newKey;

    if (T == NULL)  // Ʈ���� ��� �ִ� ���
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
    // Node* return_q = NULL;  // ������ ����� �θ� ��� (������ �� ���)

    if (p == NULL)  // �����ϰ��� �ϴ� ���� �߰ߵ��� ���� ��� -> ���� ����
        return NULL;

    if (p->left == NULL && p->right == NULL) {  // ������ ����� ������ 0�� ���
        if (q == NULL) {  // ��Ʈ ����� ���
            T = NULL;
            return NULL;
        }
        if (q->left == p)
            q->left = NULL;
        else
            q->right = NULL;

        return q;
    }
    else if (p->left == NULL || p->right == NULL) {  // ������ ����� ������ 1�� ���
        if (p->left != NULL) {  // �� �� ���� �ڽ� ��常 �ִ� ���
            if (q == NULL) {  // ��Ʈ ����� ���
                p->key = p->left->key;
                p->left = p->left->left;
                return NULL;
            }
            if (q->left == p)
                q->left = p->left;
            else
                q->right = p->left;
        }
        else {  // �� �� ������ �ڽ� ��常 �ִ� ���
            if (q == NULL) {  // ��Ʈ ����� ���
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
    else {  // ������ ����� ������ 2�� ���
        Node* r;  // ������ ��� �ڸ��� Ű���� ����� ���
        char flag;  // r Ű���� ������ ����Ʈ�� ����

        if (height(p->left) > height(p->right)) {
            r = maxNode(p->left);
            flag = 'l';
        }
        else {
            r = minNode(p->right);
            flag = 'r';
        }
        p->key = r->key;  // Ű�� ����

        if (flag == 'l') {
            if (p->key == p->left->key) {
                p->left = p->left->left;
                return p;
            }
            deleteBST(p->left, r->key);  // ���� ����Ʈ������ ����
        }
        else {
            if (p->key == p->right->key) {
                p->right = p->right->right;
                return p;
            }
            deleteBST(p->right, r->key);  // ������ ����Ʈ������ ����
        }
        return q;
    }
}

// checkBalance �Լ��� ��ȯ���� ���� ����ü
struct checkBalanceResult {
    std::string rotationType;  // ȸ�� ����
    Node* p;  // �ұ��� ���
    Node* q;  // �ұ��� ����� �θ� ���
    
    checkBalanceResult() : rotationType("NO"), p(NULL), q(NULL) {};
};

checkBalanceResult* checkBalance(Node*& T, int newKey) {
    checkBalanceResult* result = new checkBalanceResult();
    Node* f = NULL;  // ó������ �ұ����� ������ ����� �θ� ���
    Node* a = NULL;  // ó������ �ұ����� ������ ���
    Node* p = T;  // Ž���� ���� ������ ����
    Node* q = NULL;  // newKey�� ��ġ

    // newKey ��ġ Ž��
    while (p != NULL) {
        p->bf = bf(p);
        if (p->bf < -1 || p->bf > 1) {  // Ž�� �� �ұ��� �߰��� ���
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

    if (p == NULL)  // newKey�� ã�� ���� ���
        return result;

    if (a == NULL)  // �ұ���(bf�� ������ 1 �ʰ�)�� ��带 ã�� ���� ���
        return result;

    // ���ʷ� ������ ���� ������ newKey���� bf�� �ٽ� ���
    Node* b = NULL;  // ������ �ұ��� ����� newKey ������ �ڽ� ���

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

    // ȸ�� ���� ����
    string rotationType;

    if (a->bf > 1 && b->bf > 0)  // LL ȸ�� �ʿ�
        rotationType = "LL";
    else if (a->bf > 1 && b->bf < 0)  // LR ȸ�� �ʿ�
        rotationType = "LR";
    else if (a->bf < 1 && b->bf > 0)  // RL ȸ�� �ʿ�
        rotationType = "RL";
    else if (a->bf < 1 && b->bf < 0)  // RR ȸ�� �ʿ�
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

    if (rotationType == "LL") {  // LL ȸ��
        b = p->left;

        a->left = b->right;
        b->right = a;
    }
    else if (rotationType == "LR") {  // LR ȸ��
        b = p->left;
        c = b->right;

        b->right = c->left;
        a->left = c->right;
        c->left = b;
        c->right = a;
    }
    else if (rotationType == "RL") {  // RL ȸ��
        b = p->right;
        c = b->left;

        b->left = c->right;
        a->right = c->left;
        c->right = b;
        c->left = a;
    }
    else if (rotationType == "RR") {  // RR ȸ��
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

    // bf ����
    updateBF(T);
}
 
string insertAVL(Node*& T, int newKey) {
    if (T == NULL) {  // Ʈ���� ��� ���� ��
        T = getNode();
        T->key = newKey;
        return "NO";
    }
    
    if (!insertBST(T, newKey))  // ���� ���� �� (Ű�� ������) FAIL ����
        return "FAIL";

    checkBalanceResult* checkBalanceResult = checkBalance(T, newKey);  // checkBalanceResult ������ �� 3�� ����
    string rotationType = checkBalanceResult->rotationType;
    Node* p = checkBalanceResult->p;
    Node* q = checkBalanceResult->q;

    if (rotationType != "NO")  // ȸ���� �ʿ��� ���
        rotateTree(T, rotationType, p, q);

    updateHeight(T);

    return rotationType;  // ȸ�� ���� ��Ʈ������ ����
}

string deleteAVL(Node*& T, int deleteKey) {
    if (searchBST(T, deleteKey) == NULL)  // ���� ���� �� (Ű�� ������) FAIL ����
        return "FAIL";

    Node* q = deleteBST(T, deleteKey);

    updateHeight(T);

    if (q == NULL)  // �ұ���(bf�� ������ 1 �ʰ�)�� ��带 ã�� ���� ���
        return "NO";

    checkBalanceResult* checkBalanceResult = checkBalance(T, q->key);  // checkBalanceResult ������ �� 3�� ����
    string rotationType = checkBalanceResult->rotationType;
    Node* p_ = checkBalanceResult->p;
    Node* q_ = checkBalanceResult->q;

    if (rotationType != "NO")  // ȸ���� �ʿ��� ���
        rotateTree(T, rotationType, p_, q_);

    updateHeight(T);
    updateBF(T);

    return rotationType;  // ȸ�� ���� ��Ʈ������ ����
}

void inorderBST(Node* T) {
    // inorder ��ȸ
    if (T != NULL) {
        inorderBST(T->left);
        cout << "(" << T->key << ", " << T->bf << ") ";
        inorderBST(T->right);
    }
}

int main(void) {
    Node* tree = new Node();  // Ʈ�� ����

    while (true) {
        if (cin.eof() == true)  // �ؽ�Ʈ ������ ���� �����ϸ� �ݺ��� Ż��
            break;

        char command;  // ��ɾ� i �Ǵ� d
        int num;  // ����
        cin >> command >> num;

        // ����
        if (command == 'i') {
            string rotationType = insertAVL(tree, num);
            if (rotationType == "FAIL")
                cout << "i " << num << " : The key already exists" << endl;
            else
                cout << rotationType << " ";
        }

        // ����
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

        // inorder ���
        inorderBST(tree);
        cout << endl;
        /*printLink(tree);
        cout << endl;*/
    }

    delete tree;

}
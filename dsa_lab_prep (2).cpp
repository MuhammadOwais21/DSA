#include <iostream>
#include <queue>
#include <stack>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
using namespace std;

/* ======================================================================
   SECTION 1: DOUBLY LINKED LIST — insert, print, tail pointer, index
   ====================================================================== */

struct DNode {
    float data;
    int index;
    DNode* prev;
    DNode* next;
};

class DoublyLinkedList {
public:
    DNode* head;
    DNode* tail;
    int size;

    DoublyLinkedList() : head(nullptr), tail(nullptr), size(0) {}

    void insert(float value) {
        DNode* newNode = new DNode{value, size, nullptr, nullptr};
        if (head == nullptr) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        size++;
    }
    /* LOGIC: We always append at the tail, so index = insertion order
       automatically. Keeping a tail pointer makes insertion O(1) instead
       of walking the whole list every time. */

    void print() {
        DNode* curr = head;
        while (curr != nullptr) {
            cout << curr->data << "(idx:" << curr->index << ") ";
            curr = curr->next;
        }
        cout << endl;
    }
    /* LOGIC: Standard forward traversal using next pointers. */
};


/* ======================================================================
   SECTION 2: QUICK SORT ON A DOUBLY LINKED LIST
   ====================================================================== */

void swapData(DNode* a, DNode* b) {
    float temp = a->data;
    a->data = b->data;
    b->data = temp;
}
/* LOGIC: Swap the DATA inside nodes instead of rewiring prev/next
   pointers — same sorted result, far less risk of breaking the list. */

DNode* partition(DNode* low, DNode* high) {
    float pivot = high->data;
    DNode* i = low->prev; // tracks the last element known to be <= pivot

    for (DNode* j = low; j != high; j = j->next) {
        if (j->data <= pivot) {
            i = (i == nullptr) ? low : i->next;
            swapData(i, j);
        }
    }
    i = (i == nullptr) ? low : i->next;
    swapData(i, high);
    return i;
}
/* LOGIC: Exact Lomuto partition scheme from array quicksort — i and j
   play their usual roles, we just move them with ->next instead of
   i++/j++, since list nodes can't be indexed directly. */

void quickSort(DNode* low, DNode* high) {
    if (high != nullptr && low != high && low != high->next) {
        DNode* p = partition(low, high);
        quickSort(low, p->prev);
        quickSort(p->next, high);
    }
}
/* LOGIC: Same divide-and-conquer as array quicksort. The base case
   "low != high && low != high->next" replaces "low < high" since node
   pointers can't be compared with <. Call as: quickSort(head, tail); */


/* ======================================================================
   SECTION 3: REVERSE A DOUBLY LINKED LIST
   ====================================================================== */

void reverseDLL(DoublyLinkedList& list) {
    DNode* curr = list.head;
    DNode* temp = nullptr;

    while (curr != nullptr) {
        temp = curr->prev;
        curr->prev = curr->next;
        curr->next = temp;
        curr = curr->prev; // this is the OLD curr->next, i.e. move forward
    }

    if (temp != nullptr) {
        list.head = temp->prev;
    }

    DNode* t = list.head;
    list.head = list.tail;
    list.tail = t;
}
/* LOGIC: For every node, swap its prev and next pointers. After the
   loop, the old tail is reachable first, so head/tail are swapped at
   the list level too. Matches "change pointers from previous to next
   and next to previous, start will point to last node." */


/* ======================================================================
   SECTION 4: MIN / MAX ON A DOUBLY LINKED LIST
   ====================================================================== */

float findMin(DoublyLinkedList& list) {
    DNode* curr = list.head;
    float minVal = curr->data;
    while (curr != nullptr) {
        if (curr->data < minVal) minVal = curr->data;
        curr = curr->next;
    }
    return minVal;
}

float findMax(DoublyLinkedList& list) {
    DNode* curr = list.head;
    float maxVal = curr->data;
    while (curr != nullptr) {
        if (curr->data > maxVal) maxVal = curr->data;
        curr = curr->next;
    }
    return maxVal;
}
/* LOGIC: Plain O(n) linear scans. Written as a general scan (not just
   head/tail) so they work correctly even if the list isn't sorted. */


/* ======================================================================
   SECTION 5: SINGLY LINKED LIST — insert at Nth position
   (n derived from a number, e.g. last digit of 767 -> position 7)
   ====================================================================== */

struct SNode {
    float data;
    SNode* next;
};

void insertAtNth(SNode*& head, float value, int n) {
    SNode* newNode = new SNode{value, nullptr};

    if (n == 1 || head == nullptr) {
        newNode->next = head;
        head = newNode;
        return;
    }

    SNode* curr = head;
    for (int i = 1; i < n - 1 && curr->next != nullptr; i++) {
        curr = curr->next;
    }
    // curr is now at the (n-1)th node, or the last node if list is shorter

    newNode->next = curr->next;
    curr->next = newNode;
}
/* LOGIC: "Insert at position n" means n-1 nodes come BEFORE the new
   node, so we walk n-1 steps to land on the node the new one goes
   after. The "curr->next != nullptr" check is a safety net — if the
   list is shorter than n, curr stops at the last node instead of
   crashing, and the value gets appended there.
   To derive n from a number like 767: use (number % 10) for "last
   digit," or (number % listSize) if she says "divide by list size" —
   confirm exact wording in the actual question. */


/* ======================================================================
   SECTION 6: BINARY TREE — heap-style level-order build (float data)
   ====================================================================== */

struct TNode {
    float data;
    TNode* left;
    TNode* right;
};

TNode* insertLevelOrder(TNode* root, float value) {
    TNode* newNode = new TNode{value, nullptr, nullptr};
    if (root == nullptr) return newNode;

    queue<TNode*> q;
    q.push(root);
    while (!q.empty()) {
        TNode* curr = q.front(); q.pop();
        if (curr->left == nullptr) { curr->left = newNode; return root; }
        else q.push(curr->left);

        if (curr->right == nullptr) { curr->right = newNode; return root; }
        else q.push(curr->right);
    }
    return root;
}
/* LOGIC: Exactly like inserting into an array-based heap left-to-right,
   but with real pointers. A queue holds nodes waiting to be checked;
   we pop the front, and if it has a free left or right slot, the new
   node goes there. If both slots are full, we push its children onto
   the queue and keep looking — this guarantees the tree fills level
   by level, left to right, which is what "same manner as heap
   insertion" means. */


/* ======================================================================
   SECTION 7: PREORDER TRAVERSAL — WITHOUT RECURSION (stack)
   ====================================================================== */

void preorderIterative(TNode* root) {
    if (root == nullptr) return;
    stack<TNode*> s;
    s.push(root);
    while (!s.empty()) {
        TNode* curr = s.top(); s.pop();
        cout << curr->data << " ";
        if (curr->right) s.push(curr->right);
        if (curr->left) s.push(curr->left);
    }
}
/* LOGIC: Preorder = root, left, right. A stack is LIFO, so to process
   left BEFORE right, we must push right FIRST, then left — that way
   left ends up on top and gets popped (processed) first. Each node is
   printed the moment it's popped. */


/* ======================================================================
   SECTION 8: POSTORDER TRAVERSAL — WITHOUT RECURSION (two stacks)
   ====================================================================== */

void postorderIterative(TNode* root) {
    if (root == nullptr) return;
    stack<TNode*> s1, s2;
    s1.push(root);
    while (!s1.empty()) {
        TNode* curr = s1.top(); s1.pop();
        s2.push(curr);
        if (curr->left) s1.push(curr->left);
        if (curr->right) s1.push(curr->right);
    }
    while (!s2.empty()) {
        cout << s2.top()->data << " ";
        s2.pop();
    }
}
/* LOGIC: Postorder = left, right, root — the trickiest non-recursive
   traversal to derive on the spot, so this fixed two-stack method is
   worth memorizing directly. s1 does a "root, right, left" traversal
   (mirror of preorder — note left is pushed before right here, the
   opposite of Section 7) while dumping every popped node into s2.
   Since s2 collects nodes in "root, right, left" order, popping s2
   entirely reverses that to "left, right, root" — exactly postorder. */


/* ======================================================================
   SECTION 9: BFS / LEVEL-ORDER PRINT
   ====================================================================== */

void bfsPrint(TNode* root) {
    if (root == nullptr) return;
    queue<TNode*> q;
    q.push(root);
    while (!q.empty()) {
        TNode* curr = q.front(); q.pop();
        cout << curr->data << " ";
        if (curr->left) q.push(curr->left);
        if (curr->right) q.push(curr->right);
    }
}
/* LOGIC: A queue is FIFO, so nodes come out in the same order they
   went in — level by level, left to right — which is exactly BFS.
   This is the same queue mechanism as Section 6's build, just used to
   print instead of insert. */


/* ======================================================================
   SECTION 10: MIRROR IMAGE OF A TREE
   ====================================================================== */

TNode* mirror(TNode* root) {
    if (root == nullptr) return nullptr;
    swap(root->left, root->right);
    mirror(root->left);
    mirror(root->right);
    return root;
}
/* LOGIC: At every node, swap its left and right children, then repeat
   the same swap on whatever is now in those two subtrees. Recursion
   guarantees this happens at every level, not just the root. */


/* ======================================================================
   SECTION 11: SEARCH + PREDECESSOR/SUCCESSOR (inorder-based, general tree)
   ====================================================================== */

bool search(TNode* root, float x) {
    if (root == nullptr) return false;
    if (root->data == x) return true;
    return search(root->left, x) || search(root->right, x);
}
/* LOGIC: This is NOT a BST, so we can't discard half the tree the way
   BST search does — we must check every node, left and right. */

void inorderPredSucc(TNode* root, float x, TNode*& prev, TNode*& pred, TNode*& succ) {
    if (root == nullptr) return;
    inorderPredSucc(root->left, x, prev, pred, succ);

    if (prev != nullptr && prev->data == x) succ = root;
    if (root->data == x) pred = prev;
    prev = root;

    inorderPredSucc(root->right, x, prev, pred, succ);
}
/* LOGIC: "Predecessor/successor of x" (for a general, non-BST tree)
   means the node immediately BEFORE and AFTER x in an inorder walk.
   We do a normal inorder traversal (left, root, right) while carrying
   a "prev" pointer of the last node visited. When we land ON x, prev
   is its predecessor. The next node visited after x becomes its
   successor — detected by checking "was the previous node x?" right
   before printing/visiting the current one.
   Call like this:
   TNode *prev=nullptr, *pred=nullptr, *succ=nullptr;
   inorderPredSucc(root, x, prev, pred, succ); */


/* ======================================================================
   SECTION 12: COUNT TOTAL NODES
   ====================================================================== */

int countNodes(TNode* root) {
    if (root == nullptr) return 0;
    return 1 + countNodes(root->left) + countNodes(root->right);
}
/* LOGIC: Every node counts itself as 1, plus whatever its left and
   right subtrees count. Empty tree contributes 0. Classic divide and
   conquer. */


/* ======================================================================
   SECTION 13: LEVEL OF A NODE / HEIGHT OF A NODE
   ====================================================================== */

int findLevel(TNode* root, float x, int level) {
    if (root == nullptr) return -1;
    if (root->data == x) return level;
    int left = findLevel(root->left, x, level + 1);
    if (left != -1) return left;
    return findLevel(root->right, x, level + 1);
}
/* LOGIC: We walk down carrying a running "level" counter (root starts
   at level 0, or 1 — confirm her convention). If we find x, that
   level is the answer. If not found on the left, we try the right
   subtree with the same incremented level. -1 means "not found here." */

TNode* findNode(TNode* root, float x) {
    if (root == nullptr) return nullptr;
    if (root->data == x) return root;
    TNode* left = findNode(root->left, x);
    if (left != nullptr) return left;
    return findNode(root->right, x);
}

int heightOfSubtree(TNode* root) {
    if (root == nullptr) return -1; // empty subtree has height -1 (0 for a single leaf)
    return 1 + max(heightOfSubtree(root->left), heightOfSubtree(root->right));
}
/* LOGIC: "Height of node x" = height of the subtree rooted at x. So
   first locate x with findNode, then run the standard height formula
   (1 + max of left/right subtree heights) starting from that node
   instead of from the real root:
   int h = heightOfSubtree(findNode(root, x)); */


/* ======================================================================
   SECTION 14: BST — build, non-recursive postorder, DESCENDING print
   ====================================================================== */

TNode* insertBST(TNode* root, float value) {
    if (root == nullptr) return new TNode{value, nullptr, nullptr};
    if (value < root->data) root->left = insertBST(root->left, value);
    else root->right = insertBST(root->right, value);
    return root;
}
/* LOGIC: Standard BST rule — smaller values go left, larger (or equal)
   go right, applied recursively until an empty spot is found. */

void printDescending(TNode* root) {
    if (root == nullptr) return;
    printDescending(root->right);
    cout << root->data << " ";
    printDescending(root->left);
}
/* LOGIC: Normal inorder (left, root, right) prints a BST in ASCENDING
   order because left is always smaller. Simply reversing the order to
   (right, root, left) visits biggest-first, giving DESCENDING order —
   no sorting needed, the BST structure already encodes the order.
   Note: Section 8's postorderIterative() works unchanged on a BST too
   — postorder doesn't care whether the tree is a BST or not, since it
   only depends on tree SHAPE, not on value ordering. */


/* ======================================================================
   SECTION 15: MAZE AS GRAPH — adjacency list + BFS shortest path
   ====================================================================== */

vector<vector<int>> buildAdjList(vector<vector<int>>& A, int n, int m) {
    vector<vector<int>> adj(n * m);
    int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (A[i][j] == 1) continue; // blocked cell, no outgoing edges
            int u = i * m + j;
            for (int d = 0; d < 8; d++) {
                int ni = i + dx[d], nj = j + dy[d];
                if (ni >= 0 && ni < n && nj >= 0 && nj < m && A[ni][nj] == 0) {
                    adj[u].push_back(ni * m + nj);
                }
            }
        }
    }
    return adj;
}
/* LOGIC: Every valid (0-valued) cell (i,j) is flattened into a single
   integer id = i*m + j, so the grid becomes a normal graph with n*m
   nodes. For each cell, we check all 8 neighboring offsets (dx,dy
   pairs cover N,S,E,W and 4 diagonals) and add an edge only if the
   neighbor is inside bounds AND is itself unblocked (0). This is
   "mapping the maze onto a graph" — adjacency list only, as required. */

vector<int> bfsPath(vector<vector<int>>& adj, int n, int m) {
    int start = 0, end = n * m - 1; // (0,0) and (n-1,m-1) flattened
    vector<int> parent(n * m, -1);
    vector<bool> visited(n * m, false);
    queue<int> q;

    q.push(start);
    visited[start] = true;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        if (u == end) break;
        for (int v : adj[u]) {
            if (!visited[v]) {
                visited[v] = true;
                parent[v] = u;
                q.push(v);
            }
        }
    }

    vector<int> path;
    if (!visited[end]) return path; // empty = no path exists
    for (int v = end; v != -1; v = parent[v]) path.push_back(v);
    reverse(path.begin(), path.end());
    return path;
}
/* LOGIC: Standard BFS guarantees the SHORTEST path in an unweighted
   graph, which is why it's specified here instead of DFS. Every time
   we visit a new node v from u, we record "v was reached FROM u" in
   parent[v]. Once we reach the end, we walk backwards through parent[]
   from end to start to reconstruct the actual path, then reverse it
   to get start-to-end order. To print as (i,j): row = id/m, col = id%m. */


/* ======================================================================
   SECTION 16: MEDIAN-SPLIT RECURSIVE TREE (4-ary tree)
   NOTE: exact split-range wording on the paper was partly unclear —
   verify boundaries (M/2, M, 3M/2) against her exact phrasing before
   the exam. Logic below follows the example worked in the picture.
   ====================================================================== */

struct MNode {
    float data;
    MNode* child[4] = {nullptr, nullptr, nullptr, nullptr};
};

MNode* buildMedianTree(vector<float> S, int level, int& maxHeight) {
    if (S.empty()) return nullptr;
    sort(S.begin(), S.end());
    float M = S[S.size() / 2]; // median = middle element of sorted S

    MNode* node = new MNode();
    node->data = M;
    cout << "Inserted " << M << " at level " << level << endl;
    if (level > maxHeight) maxHeight = level;

    vector<float> S1, S2, S3, S4;
    for (float val : S) {
        if (val == M) continue; // the median itself becomes this node
        if (val <= M / 2)        S1.push_back(val);
        else if (val < M)        S2.push_back(val);
        else if (val < 3 * M / 2) S3.push_back(val);
        else                      S4.push_back(val);
    }

    node->child[0] = buildMedianTree(S1, level + 1, maxHeight);
    node->child[1] = buildMedianTree(S2, level + 1, maxHeight);
    node->child[2] = buildMedianTree(S3, level + 1, maxHeight);
    node->child[3] = buildMedianTree(S4, level + 1, maxHeight);

    return node;
}
/* LOGIC: Each call sorts the current subset, picks the middle value as
   M (the node to insert), then buckets every OTHER value into 4 groups
   by comparing against M/2, M, and 3M/2 — matching the picture's S1/S2/
   S3/S4 boundary arrows. Each bucket recursively becomes one of the 4
   children (S1->child[0], S2->child[1], S3->child[2], S4->child[3]),
   built the exact same way. "level" is passed down and incremented
   each recursive call, giving you level-of-insertion for free; the
   height is just the deepest level reached (tracked via maxHeight). */


/* ======================================================================
   MAIN — manual test harness (remove/adjust for actual submission)
   ====================================================================== */

int main() {
    srand((unsigned)time(0));

    // --- Test DLL + quicksort + reverse + min/max ---
    DoublyLinkedList dll;
    int arr[] = {5, 2, 9, 1, 7};
    for (int v : arr) dll.insert(v);
    cout << "DLL before sort: "; dll.print();
    quickSort(dll.head, dll.tail);
    cout << "DLL after quicksort: "; dll.print();
    cout << "Min: " << findMin(dll) << ", Max: " << findMax(dll) << endl;
    reverseDLL(dll);
    cout << "DLL after reverse: "; dll.print();

    // --- Test SLL insert at nth ---
    SNode* shead = nullptr;
    for (int i = 8; i >= 1; i--) { SNode* n = new SNode{(float)i, shead}; shead = n; }
    insertAtNth(shead, 99, 7); // 767 % 10 = 7
    cout << "\nSLL after insertAtNth: ";
    for (SNode* c = shead; c != nullptr; c = c->next) cout << c->data << " ";
    cout << endl;

    // --- Test binary tree build (matches sample: 89,122,12,75,99,68) ---
    TNode* root = nullptr;
    float vals[] = {89, 122, 12, 75, 99, 68};
    for (float v : vals) root = insertLevelOrder(root, v);

    cout << "\nPreorder (iterative): "; preorderIterative(root); cout << endl;
    cout << "Postorder (iterative): "; postorderIterative(root); cout << endl;
    cout << "BFS / level-order: "; bfsPrint(root); cout << endl;
    cout << "Total nodes: " << countNodes(root) << endl;
    cout << "Level of 75: " << findLevel(root, 75, 0) << endl;
    cout << "Height of subtree at 122: " << heightOfSubtree(findNode(root, 122)) << endl;

    TNode *prev = nullptr, *pred = nullptr, *succ = nullptr;
    inorderPredSucc(root, 12, prev, pred, succ);
    cout << "Predecessor of 12: " << (pred ? to_string(pred->data) : "none")
         << ", Successor of 12: " << (succ ? to_string(succ->data) : "none") << endl;

    mirror(root);
    cout << "BFS after mirror: "; bfsPrint(root); cout << endl;

    // --- Test BST + descending print ---
    TNode* bst = nullptr;
    float bvals[] = {50, 30, 70, 20, 40, 60, 80};
    for (float v : bvals) bst = insertBST(bst, v);
    cout << "\nBST postorder (iterative): "; postorderIterative(bst); cout << endl;
    cout << "BST descending: "; printDescending(bst); cout << endl;

    // --- Test maze BFS ---
    int n = 4, m = 4;
    vector<vector<int>> A(n, vector<int>(m));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            A[i][j] = (rand() % 4 == 0) ? 1 : 0; // ~25% blocked, random
    A[0][0] = 0; A[n-1][m-1] = 0; // keep start/end open

    cout << "\nMaze grid:\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) cout << A[i][j] << " ";
        cout << endl;
    }

    vector<vector<int>> adj = buildAdjList(A, n, m);
    vector<int> path = bfsPath(adj, n, m);
    cout << "Path: ";
    if (path.empty()) cout << "no path found";
    else for (int id : path) cout << "(" << id / m << "," << id % m << ") ";
    cout << endl;

    // --- Test median-split tree (example array from the handout) ---
    vector<float> S = {22, 44, 75, 90, 92, 99, 110, 112, 125, 130, 131};
    int maxHeight = 0;
    cout << "\nMedian-split tree build order:\n";
    buildMedianTree(S, 0, maxHeight);
    cout << "Max height reached: " << maxHeight << endl;

    return 0;
}

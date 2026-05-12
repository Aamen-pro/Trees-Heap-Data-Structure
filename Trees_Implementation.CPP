// ============================================================
//  Smart Library Book Management System
//  Data Structure: AVL Tree (Self-Balancing BST)
//  Key: Book ID
// ============================================================

#include <iostream>
#include <string>
#include <queue>
#include <climits>
using namespace std;

// ── Book Data ────────────────────────────────────────────────
struct Book {
    int    id;
    string title;
    string author;
};

// ── AVL Node ─────────────────────────────────────────────────
struct Node {
    Book  book;
    Node* left;
    Node* right;
    int   height;

    Node(Book b) {
        book   = b;
        left   = right = nullptr;
        height = 1;
    }
};
// ============================================================
//  BST Tree Class
// ============================================================
class BST {
    public:
        Node* root;
    
        BST() {
            root = nullptr;
        }
    
        // Insert
        Node* insert(Node* node, Book book) {
            if (node == nullptr)
                return new Node(book);
    
            if (book.id < node->book.id)
                node->left = insert(node->left, book);
            else if (book.id > node->book.id)
                node->right = insert(node->right, book);
    
            return node;
        }
        //Search by ID
        Node* search(Node* node , int id , int& stepCount) {
            stepCount++;
            if (node == nullptr || node->book.id == id) {
                return node;
            }
            if (id < node->book.id) return search(node->left , id , stepCount);
    
            return search(node->right , id , stepCount);
        }
    
       //Find books in ID range
        void FindInRange(Node* node , int low , int high) {
            if (!node) return;
    
            if (low < node->book.id) {
                FindInRange(node->left , low , high);
            }
            if (low <= node->book.id && node->book.id <= high) {
                cout << "  [" << node->book.id << "] "
                    << node->book.title
                    << " - " << node->book.author << endl;
            }
            if (node->book.id < high) {
                FindInRange(node->right , low , high);
            }
        }
    
        // Find closest book ID
        Book* FindClosest(Node* node , int target) {
            if (node == nullptr) return nullptr;
    
            Book* closestID = nullptr;
            int   minDiff = INT_MAX;
            Node* current = node;
    
            while (current != nullptr) {
                int diff = abs(current->book.id - target);
                if (diff < minDiff) {
                    minDiff = diff;
                    closestID = &current->book;
                }
                if (diff == 0) break;
                if (target < current->book.id)
                    current = current->left;
                else
                    current = current->right;
            }
            return closestID;
        }
    
    
        // Find minimum value
        Node* minValue(Node* node) {
            while (node && node->left != nullptr)
                node = node->left;
    
            return node;
        }
    
        // Delete node
        Node* deleteNode(Node* node, int key) {
    
            if (node == nullptr)
                return node;
    
            // search for the node
            if (key < node->book.id)
                node->left = deleteNode(node->left, key);
    
            else if (key > node->book.id)
                node->right = deleteNode(node->right, key);
    
            else {
                // Case 1: no child
                if (node->left == nullptr && node->right == nullptr) {
                    delete node;
                    return nullptr;
                }
    
                // Case 2: one child
                else if (node->left == nullptr) {
                    Node* temp = node->right;
                    delete node;
                    return temp;
                }
                else if (node->right == nullptr) {
                    Node* temp = node->left;
                    delete node;
                    return temp;
                }
    
                // Case 3: two children
                Node* temp = minValue(node->right);
                node->book = temp->book;
                node->right = deleteNode(node->right, temp->book.id);
            }
    
            return node;
        }
    
        // Return height of BST tree (recursive)
        int treeHeight(Node* node) {
            if (node == nullptr) return 0;
            return 1 + max(treeHeight(node->left), treeHeight(node->right));
        }

        // DFS Traversals
        void inorder(Node* node) {
            if (node != nullptr) {
                inorder(node->left);
                cout << "  [" << node->book.id << "] "
                        << node->book.title
                        << " - " << node->book.author << endl;
                inorder(node->right);
            }
        }
    
        void preorder(Node* node) {
            if (node != nullptr) {
                cout << node->book.id << " ";
                preorder(node->left);
                preorder(node->right);
            }
        }
    
        void postorder(Node* node) {
            if (node != nullptr) {
                postorder(node->left);
                postorder(node->right);
                cout << node->book.id << " ";
            }
        }
    
        // BFS
        void bfs(Node* node) {
    
            if (node == nullptr)
                return;
    
            queue<Node*> q;
            q.push(node);
    
            while (!q.empty()) {
    
                Node* current = q.front();
                q.pop();
    
                cout << current->book.id << " ";
    
                if (current->left)
                    q.push(current->left);
    
                if (current->right)
                    q.push(current->right);
            }
        }
};

// ============================================================
//  AVL Tree Class
// ============================================================
class AVL {
public:
    Node* root;

    AVL() {
        root = nullptr;
    }

    // ── Utility ──────────────────────────────────────────────

    int getHeight(Node* node) {
        if (node == nullptr) return 0;
        return node->height;
    }

    // Return the balance factor = height(left) - height(right)
    // Positive  → right-heavy
    // Negative  → left-heavy
    // |factor| <= 1 → balanced
    int getBalance(Node* node) {
        if (node == nullptr) return 0;
        return getHeight(node->right) - getHeight(node->left);
    }

    void updateHeight(Node* node) {
        node->height = 1 + max(getHeight(node->left), getHeight(node->right));
    }

    // Return total height of the tree
    int treeHeight(Node* node) {
        return getHeight(node);
    }

    // ── AVL Rotations ─────────────────────────────────────────

    // Right rotation (LL case)
    Node* rightRotate(Node* y) {
        // x is the New sub root , and y is the node where the impalnce occured 
        Node* x  = y->left;
        // T2 is the right subtree of x , to but it in the left of y after rotation
        Node* T2 = x->right;
        // rotate left
        x->right = y;
        y->left  = T2;

        updateHeight(y);
        updateHeight(x);

        return x;
    }

    // Left rotation (RR case)
    Node* leftRotate(Node* x) {
        // x is the New sub root , and y is the node where the impalnce occured 
        Node* y  = x->right;
        // T2 is the left subtree of y , to but it in the right of x after rotation
        Node* T2 = y->left;

        y->left  = x;
        x->right = T2;

        updateHeight(x);
        updateHeight(y);

        return y;
    }

    // Left-Right rotation (LR case)
    Node* leftRightRotate(Node* node) {
        // first do left rotaiton for the left side of the old root then do right rotaiton for the old root
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Right-Left rotation (RL case)
    Node* rightLeftRotate(Node* node) {
        // first do right rotaiton for the right side of the old root then do left rotaiton for the old root
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    // ── Insert Operation ──────────────────────────────────────
    Node* insert(Node* node, Book book) {
        // Normal BST insert
        if (node == nullptr)
            return new Node(book);

        if (book.id < node->book.id)
            node->left = insert(node->left, book);
        else if (book.id > node->book.id)
            node->right = insert(node->right, book);
        else
            return node;   // Duplicate IDs not allowed

        // Update height
        updateHeight(node);

        // Get balance factor
        int balance = getBalance(node);

        // LL Case
        if (balance < -1 && book.id < node->left->book.id)
            return rightRotate(node);

        // RR Case
        if (balance > 1 && book.id > node->right->book.id)
            return leftRotate(node);

        // LR Case
        if (balance < -1 && book.id > node->left->book.id)
            return leftRightRotate(node);

        // RL Case
        if (balance > 1 && book.id < node->right->book.id)
            return rightLeftRotate(node);

        return node;
    }

    // ── Delete Operation ──────────────────────────────────────

    // Find the in-order successor (smallest in right subtree)
    Node* minValue(Node* node) {
        while (node->left != nullptr)
            node = node->left;
        return node;
    }

    Node* deleteNode(Node* node, int id) {
        if (node == nullptr)
            return node;

        // BST delete
        if (id < node->book.id)
            node->left = deleteNode(node->left, id);
        else if (id > node->book.id)
            node->right = deleteNode(node->right, id);
        else {
            // Node found — handle 0 or 1 child

            if (node->left == nullptr) {
                Node* temp = node->right;
                delete node;
                return temp;
            }

            else if (node->right == nullptr) {
                Node* temp = node->left;
                delete node;
                return temp;
            }
            //case 3
            else {
                // Two children: replace with in-order successor
                Node* temp   = minValue(node->right);
                node->book   = temp->book;
                node->right  = deleteNode(node->right, temp->book.id);
            }
        }

        if (node == nullptr)
            return node;

        // Update height
        updateHeight(node);

        int balance = getBalance(node);

        // RR: right-heavy, right child is right-heavy or balanced
        if (balance > 1 && getBalance(node->right) >= 0)
            return leftRotate(node);

        // RL: right-heavy, right child is left-heavy
        if (balance > 1 && getBalance(node->right) < 0)
            return rightLeftRotate(node);

        // LL: left-heavy, left child is left-heavy or balanced
        if (balance < -1 && getBalance(node->left) <= 0)
            return rightRotate(node);

        // LR: left-heavy, left child is right-heavy
        if (balance < -1 && getBalance(node->left) > 0)
            return leftRightRotate(node);

        return node;
    }

    // ── Search Utilities ──────────────────────────────────────

    // Search by ID — stepCount tracks nodes visited
    Node* search(Node* node, int id, int& stepCount) {
        stepCount++;
        if (node == nullptr || node->book.id == id)
            return node;

        if (id < node->book.id)
            return search(node->left, id, stepCount);

        return search(node->right, id, stepCount);
    }

    // ── Traversals ────────────────────────────────────────────

    // Inorder: prints books sorted by ID
    void inorder(Node* node) {
        if (node == nullptr) return;
        inorder(node->left);
        cout << "  [" << node->book.id << "] "
                << node->book.title
                << " - " << node->book.author << endl;
        inorder(node->right);
    }

    void preorder(Node* node) {
        if (node == nullptr) return;
        cout << "  [" << node->book.id << "] " << node->book.title << endl;
        preorder(node->left);
        preorder(node->right);
    }

    void postorder(Node* node) {
        if (node == nullptr) return;
        postorder(node->left);
        postorder(node->right);
        cout << "  [" << node->book.id << "] " << node->book.title << endl;
    }

    void bfs(Node* node) {
        if (node == nullptr) return;

        queue<Node*> q;
        q.push(node);
        while (!q.empty()) {
            int levelSize = q.size();
            for(int i=0; i < levelSize; i++){
                Node* current = q.front();
                q.pop();
                cout << "  [" << current->book.id << "] "
                    << current->book.title << endl;

                if (current->left)  q.push(current->left);
                if (current->right) q.push(current->right);
            }
            cout<<endl;
        }
    }

    // ── Extra Features ────────────────────────────────────────

    // Print all books with ID between low and high
    void findInRange(Node* node, int low, int high) {
        if (node == nullptr) return;

        if (node->book.id > low)
            findInRange(node->left, low, high);

        if (node->book.id >= low && node->book.id <= high)
            cout << "  [" << node->book.id << "] "
                << node->book.title
                << " - " << node->book.author << endl;

        if (node->book.id < high)
            findInRange(node->right, low, high);
    }

    // Find the book whose ID is closest to targetID
    Book* findClosest(Node* node, int targetID) {
        if (node == nullptr) return nullptr;

        Book* closest = nullptr;
        int   minDiff = INT_MAX;
        Node* current = node;

        while (current != nullptr) {
            int diff = abs(current->book.id - targetID);
            if (diff < minDiff) {
                minDiff = diff;
                closest = &current->book;
            }
            if (diff == 0) break;
            if (targetID < current->book.id)
                current = current->left;
            else
                current = current->right;
        }
        return closest;
    }

    // ── Memory cleanup ────────────────────────────────────────

    void destroyTree(Node* node) {
        if (node == nullptr) return;
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }

    ~AVL() {
        destroyTree(root);
    }
};

// ============================================================
//  Main
// ============================================================

int main() {

    // ============================================================
    //  PART 1 — BST Demo
    // ============================================================
    cout << "============================================" << endl;
    cout << "          BST Demo Section                  " << endl;
    cout << "============================================" << endl;

    BST bst;

    // ── Insert 6 books ────────────────────────────────────────
    cout << "\n--- BST: Inserting 6 Books ---" << endl;
    bst.root = bst.insert(bst.root, {300, "The Great Gatsby",       "F. Scott Fitzgerald"});
    bst.root = bst.insert(bst.root, {150, "To Kill a Mockingbird",  "Harper Lee"});
    bst.root = bst.insert(bst.root, {500, "1984",                   "George Orwell"});
    bst.root = bst.insert(bst.root, { 80, "Brave New World",        "Aldous Huxley"});
    bst.root = bst.insert(bst.root, {420, "The Alchemist",          "Paulo Coelho"});
    bst.root = bst.insert(bst.root, {650, "Crime and Punishment",   "Fyodor Dostoevsky"});
    cout << "  6 books inserted." << endl;

    // ── Inorder traversal ─────────────────────────────────────
    cout << "\n--- BST: Inorder Traversal (sorted by ID) ---" << endl;
    bst.inorder(bst.root);

    // ── Search ────────────────────────────────────────────────
    cout << "\n--- BST: Search ---" << endl;
    int steps = 0;
    Node* result = bst.search(bst.root, 420, steps);
    cout << "  Search ID 420: " << (result ? "FOUND" : "NOT FOUND")
        << " (" << steps << " steps)" << endl;

    // ── Delete ────────────────────────────────────────────────
    cout << "\n--- BST: Delete ---" << endl;
    cout << "  Deleting ID 300 (node with two children)..." << endl;
    bst.root = bst.deleteNode(bst.root, 300);
    cout << "  Deleting ID 80  (leaf node)..." << endl;
    bst.root = bst.deleteNode(bst.root, 80);

    cout << "\n--- BST: Inorder After Deletions ---" << endl;
    bst.inorder(bst.root);

    // ── Range search ──────────────────────────────────────────
    cout << "\n--- BST: Range Search [100 - 500] ---" << endl;
    bst.FindInRange(bst.root, 100, 500);

    // ── Closest ID ────────────────────────────────────────────
    cout << "\n--- BST: Closest ID Search ---" << endl;
    Book* closest = bst.FindClosest(bst.root, 400);
    if (closest)
        cout << "  Search ID 400 -> Closest: [" << closest->id << "] "
            << closest->title << endl;

    // ============================================================
    //  PART 2 — AVL Demo
    // ============================================================
    cout << "\n\n============================================" << endl;
    cout << "          AVL Demo Section                  " << endl;
    cout << "============================================" << endl;

    AVL avl;

    // ── Insert 6 books (same IDs as BST demo) ─────────────────
    cout << "\n--- AVL: Inserting 6 Books ---" << endl;
    avl.root = avl.insert(avl.root, {300, "The Great Gatsby",       "F. Scott Fitzgerald"});
    avl.root = avl.insert(avl.root, {150, "To Kill a Mockingbird",  "Harper Lee"});
    avl.root = avl.insert(avl.root, {500, "1984",                   "George Orwell"});
    avl.root = avl.insert(avl.root, { 80, "Brave New World",        "Aldous Huxley"});
    avl.root = avl.insert(avl.root, {420, "The Alchemist",          "Paulo Coelho"});
    avl.root = avl.insert(avl.root, {650, "Crime and Punishment",   "Fyodor Dostoevsky"});
    cout << "  6 books inserted. Tree height = "
         << avl.treeHeight(avl.root) << " (balanced)" << endl;

    // ── Inorder traversal ─────────────────────────────────────
    cout << "\n--- AVL: Inorder Traversal (sorted by ID) ---" << endl;
    avl.inorder(avl.root);

    // ── Search ────────────────────────────────────────────────
    cout << "\n--- AVL: Search ---" << endl;
    steps = 0;
    result = avl.search(avl.root, 420, steps);
    cout << "  Search ID 420: " << (result ? "FOUND" : "NOT FOUND")
        << " (" << steps << " steps)" << endl;

    // ── Delete ────────────────────────────────────────────────
    cout << "\n--- AVL: Delete ---" << endl;
    cout << "  Deleting ID 300 (node with two children)..." << endl;
    avl.root = avl.deleteNode(avl.root, 300);
    cout << "  Deleting ID 80  (leaf node)..." << endl;
    avl.root = avl.deleteNode(avl.root, 80);

    cout << "\n--- AVL: Inorder After Deletions ---" << endl;
    avl.inorder(avl.root);
    cout << "  Tree height after deletions = "
        << avl.treeHeight(avl.root) << " (still balanced)" << endl;

    // ── Range search ──────────────────────────────────────────
    cout << "\n--- AVL: Range Search [100 - 500] ---" << endl;
    avl.findInRange(avl.root, 100, 500);

    // ── Closest ID ────────────────────────────────────────────
    cout << "\n--- AVL: Closest ID Search ---" << endl;
    closest = avl.findClosest(avl.root, 400);
    if (closest)
        cout << "  Search ID 400 -> Closest: [" << closest->id << "] "
            << closest->title << endl;

    // ============================================================
    //  BST vs AVL Performance Comparison
    // ============================================================
    cout << "\n\n============================================" << endl;
    cout << "  BST vs AVL Tree - Performance Comparison  " << endl;
    cout << "============================================" << endl;

    BST bst_rand, bst_sort;
    AVL avl_rand, avl_sort;

    // Same 25 books in RANDOM order
    Book randomBooks[] = {
        {340, "The Great Gatsby",              "F. Scott Fitzgerald"},
        {120, "To Kill a Mockingbird",         "Harper Lee"},
        {560, "1984",                           "George Orwell"},
        { 75, "Brave New World",               "Aldous Huxley"},
        {890, "The Catcher in the Rye",        "J.D. Salinger"},
        {230, "Of Mice and Men",               "John Steinbeck"},
        {450, "The Alchemist",                 "Paulo Coelho"},
        { 15, "Don Quixote",                   "Miguel de Cervantes"},
        {670, "Crime and Punishment",          "Fyodor Dostoevsky"},
        {310, "The Odyssey",                   "Homer"},
        {780, "Moby Dick",                     "Herman Melville"},
        {180, "Pride and Prejudice",           "Jane Austen"},
        {500, "The Divine Comedy",             "Dante Alighieri"},
        { 50, "Hamlet",                        "William Shakespeare"},
        {720, "War and Peace",                 "Leo Tolstoy"},
        {801, "Anna Karenina",                 "Leo Tolstoy"},
        {820, "Les Miserables",                "Victor Hugo"},
        {835, "The Count of Monte Cristo",     "Alexandre Dumas"},
        {850, "Jane Eyre",                     "Charlotte Bronte"},
        {860, "Wuthering Heights",             "Emily Bronte"},
        {875, "Frankenstein",                  "Mary Shelley"},
        {888, "Dracula",                       "Bram Stoker"},
        {900, "The Picture of Dorian Gray",    "Oscar Wilde"},
        {915, "Heart of Darkness",             "Joseph Conrad"},
        {930, "The Old Man and the Sea",       "Ernest Hemingway"}
    };

    // Same 25 books in SORTED (ascending) order — worst case for BST
    Book sortedBooks[] = {
        { 15, "Don Quixote",                   "Miguel de Cervantes"},
        { 50, "Hamlet",                        "William Shakespeare"},
        { 75, "Brave New World",               "Aldous Huxley"},
        {120, "To Kill a Mockingbird",         "Harper Lee"},
        {180, "Pride and Prejudice",           "Jane Austen"},
        {230, "Of Mice and Men",               "John Steinbeck"},
        {310, "The Odyssey",                   "Homer"},
        {340, "The Great Gatsby",              "F. Scott Fitzgerald"},
        {450, "The Alchemist",                 "Paulo Coelho"},
        {500, "The Divine Comedy",             "Dante Alighieri"},
        {560, "1984",                           "George Orwell"},
        {670, "Crime and Punishment",          "Fyodor Dostoevsky"},
        {720, "War and Peace",                 "Leo Tolstoy"},
        {780, "Moby Dick",                     "Herman Melville"},
        {801, "Anna Karenina",                 "Leo Tolstoy"},
        {820, "Les Miserables",                "Victor Hugo"},
        {835, "The Count of Monte Cristo",     "Alexandre Dumas"},
        {850, "Jane Eyre",                     "Charlotte Bronte"},
        {860, "Wuthering Heights",             "Emily Bronte"},
        {875, "Frankenstein",                  "Mary Shelley"},
        {888, "Dracula",                       "Bram Stoker"},
        {890, "The Catcher in the Rye",        "J.D. Salinger"},
        {900, "The Picture of Dorian Gray",    "Oscar Wilde"},
        {915, "Heart of Darkness",             "Joseph Conrad"},
        {930, "The Old Man and the Sea",       "Ernest Hemingway"}
    };

    int n = 25;
    for (int i = 0; i < n; i++) {
        bst_rand.root = bst_rand.insert(bst_rand.root, randomBooks[i]);
        avl_rand.root = avl_rand.insert(avl_rand.root, randomBooks[i]);
        bst_sort.root = bst_sort.insert(bst_sort.root, sortedBooks[i]);
        avl_sort.root = avl_sort.insert(avl_sort.root, sortedBooks[i]);
    }

    // ── Height Comparison ─────────────────────────────────────
    cout << "\n--- Tree Height " << n << " books ---" << endl;
    cout << "  [Random Insertion]" << endl;
    cout << "    BST height = " << bst_rand.treeHeight(bst_rand.root) << endl;
    cout << "    AVL height = " << avl_rand.treeHeight(avl_rand.root) << endl;
    cout << "  [Sorted Insertion - worst case for BST]" << endl;
    cout << "    BST height = " << bst_sort.treeHeight(bst_sort.root)
        << "  <-- degenerates to a linked list!" << endl;
    cout << "    AVL height = " << avl_sort.treeHeight(avl_sort.root)
        << "  <-- stays balanced (log2 25 ~ 5)" << endl;

    // ── Search Steps Comparison ───────────────────────────────
    cout << "\n--- Search Steps Comparison (searching ID 930) ---" << endl;
    int bstSteps = 0, avlSteps = 0;

    bst_rand.search(bst_rand.root, 930, bstSteps);
    avl_rand.search(avl_rand.root, 930, avlSteps);
    cout << "  [Random Insertion]" << endl;
    cout << "    BST steps = " << bstSteps << endl;
    cout << "    AVL steps = " << avlSteps << endl;

    bstSteps = 0; avlSteps = 0;
    bst_sort.search(bst_sort.root, 930, bstSteps);
    avl_sort.search(avl_sort.root, 930, avlSteps);
    cout << "  [Sorted Insertion - worst case for BST]" << endl;
    cout << "    BST steps = " << bstSteps << "  <-- O(n) linear scan" << endl;
    cout << "    AVL steps = " << avlSteps << "  <-- O(log n)" << endl;

    cout << "\n============================================" << endl;
    cout << "  Conclusion: AVL always stays O(log n)." << endl;
    cout << "  BST degenerates to O(n) on sorted input." << endl;
    cout << "============================================" << endl;
    return 0;
}
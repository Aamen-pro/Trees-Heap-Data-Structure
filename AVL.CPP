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
    // Positive  → left-heavy
    // Negative  → right-heavy
    // |factor| <= 1 → balanced
    int getBalance(Node* node) {
        if (node == nullptr) return 0;
        return getHeight(node->left) - getHeight(node->right);
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
        Node* x  = y->left;
        Node* T2 = x->right;

        x->right = y;
        y->left  = T2;

        updateHeight(y);
        updateHeight(x);

        return x;
    }

    // Left rotation (RR case)
    Node* leftRotate(Node* x) {
        Node* y  = x->right;
        Node* T2 = y->left;

        y->left  = x;
        x->right = T2;

        updateHeight(x);
        updateHeight(y);

        return y;
    }

    // Left-Right rotation (LR case)
    Node* leftRightRotate(Node* node) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Right-Left rotation (RL case)
    Node* rightLeftRotate(Node* node) {
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
        if (balance > 1 && book.id < node->left->book.id)
            return rightRotate(node);

        // RR Case
        if (balance < -1 && book.id > node->right->book.id)
            return leftRotate(node);

        // LR Case
        if (balance > 1 && book.id > node->left->book.id)
            return leftRightRotate(node);

        // RL Case
        if (balance < -1 && book.id < node->right->book.id)
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
            if (node->left == nullptr || node->right == nullptr) {
                Node* temp = node->left ? node->left : node->right;

                if (temp == nullptr) {
                    temp = node;
                    node = nullptr;
                } else {
                    *node = *temp;
                }
                delete temp;
            } else {
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

        // LL
        if (balance > 1 && getBalance(node->left) >= 0)
            return rightRotate(node);

        // LR
        if (balance > 1 && getBalance(node->left) < 0)
            return leftRightRotate(node);

        // RR
        if (balance < -1 && getBalance(node->right) <= 0)
            return leftRotate(node);

        // RL
        if (balance < -1 && getBalance(node->right) > 0)
            return rightLeftRotate(node);

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
            cout << "  [" << node->book.id << "] " << node->book.title << endl;
            preorder(node->left);
            preorder(node->right);
        }
    }

    void postorder(Node* node) {
        if (node != nullptr) {
            postorder(node->left);
            postorder(node->right);
            cout << "  [" << node->book.id << "] " << node->book.title << endl;
        }
    }

    void bfs(Node* node) {
        if (node == nullptr) return;

        queue<Node*> q;
        q.push(node);

        while (!q.empty()) {
            Node* current = q.front();
            q.pop();
            cout << "  [" << current->book.id << "] "
                << current->book.title << endl;

            if (current->left)  q.push(current->left);
            if (current->right) q.push(current->right);
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

    AVL tree;

    // ── Insert books (random IDs) ─────────────────────────────
    cout << "--- Inserting Books (Random IDs) ---" << endl;

    tree.root = tree.insert(tree.root, {340, "The Great Gatsby",        "F. Scott Fitzgerald"});
    tree.root = tree.insert(tree.root, {120, "To Kill a Mockingbird",   "Harper Lee"});
    tree.root = tree.insert(tree.root, {560, "1984",                    "George Orwell"});
    tree.root = tree.insert(tree.root, { 75, "Brave New World",         "Aldous Huxley"});
    tree.root = tree.insert(tree.root, {890, "The Catcher in the Rye",  "J.D. Salinger"});
    tree.root = tree.insert(tree.root, {230, "Of Mice and Men",         "John Steinbeck"});
    tree.root = tree.insert(tree.root, {450, "The Alchemist",           "Paulo Coelho"});
    tree.root = tree.insert(tree.root, { 15, "Don Quixote",             "Miguel de Cervantes"});
    tree.root = tree.insert(tree.root, {670, "Crime and Punishment",    "Fyodor Dostoevsky"});
    tree.root = tree.insert(tree.root, {310, "The Odyssey",             "Homer"});
    tree.root = tree.insert(tree.root, {780, "Moby Dick",               "Herman Melville"});
    tree.root = tree.insert(tree.root, {180, "Pride and Prejudice",     "Jane Austen"});
    tree.root = tree.insert(tree.root, {500, "The Divine Comedy",       "Dante Alighieri"});
    tree.root = tree.insert(tree.root, { 50, "Hamlet",                  "William Shakespeare"});
    tree.root = tree.insert(tree.root, {720, "War and Peace",           "Leo Tolstoy"});

    // ── Insert books (sorted IDs — AVL handles this fine) ─────
    cout << "\n--- Inserting Books (Sorted IDs) ---" << endl;

    tree.root = tree.insert(tree.root, {801, "Anna Karenina",               "Leo Tolstoy"});
    tree.root = tree.insert(tree.root, {820, "Les Miserables",              "Victor Hugo"});
    tree.root = tree.insert(tree.root, {835, "The Count of Monte Cristo",   "Alexandre Dumas"});
    tree.root = tree.insert(tree.root, {850, "Jane Eyre",                   "Charlotte Bronte"});
    tree.root = tree.insert(tree.root, {860, "Wuthering Heights",           "Emily Bronte"});
    tree.root = tree.insert(tree.root, {875, "Frankenstein",                "Mary Shelley"});
    tree.root = tree.insert(tree.root, {888, "Dracula",                     "Bram Stoker"});
    tree.root = tree.insert(tree.root, {900, "The Picture of Dorian Gray",  "Oscar Wilde"});
    tree.root = tree.insert(tree.root, {915, "Heart of Darkness",           "Joseph Conrad"});
    tree.root = tree.insert(tree.root, {930, "The Old Man and the Sea",     "Ernest Hemingway"});

    // ── Tree height ───────────────────────────────────────────
    cout << "\n--- Tree Height ---" << endl;
    cout << "  Height = " << tree.treeHeight(tree.root) << endl;

    // ── Inorder traversal ─────────────────────────────────────
    cout << "\n--- Inorder Traversal (sorted by ID) ---" << endl;
    tree.inorder(tree.root);

    // ── Search ────────────────────────────────────────────────
    cout << "\n--- Search ---" << endl;
    int steps = 0;

    Node* result = tree.search(tree.root, 450, steps);
    cout << "  Search ID 450: " << (result ? "FOUND" : "NOT FOUND")
        << " (" << steps << " steps)" << endl;

    steps = 0;
    result = tree.search(tree.root, 999, steps);
    cout << "  Search ID 999: " << (result ? "FOUND" : "NOT FOUND")
        << " (" << steps << " steps)" << endl;

    // ── Range search ──────────────────────────────────────────
    cout << "\n--- Range Search [100 - 400] ---" << endl;
    tree.findInRange(tree.root, 100, 400);

    cout << "\n--- Range Search [800 - 900] ---" << endl;
    tree.findInRange(tree.root, 800, 900);

    // ── Closest ID ────────────────────────────────────────────
    cout << "\n--- Closest ID Search ---" << endl;

    Book* closest = tree.findClosest(tree.root, 130);
    if (closest) cout << "  Closest to 130: [" << closest->id << "] " << closest->title << endl;

    closest = tree.findClosest(tree.root, 555);
    if (closest) cout << "  Closest to 555: [" << closest->id << "] " << closest->title << endl;

    closest = tree.findClosest(tree.root, 450);
    if (closest) cout << "  Closest to 450: [" << closest->id << "] " << closest->title << " (exact)" << endl;

    // ── Delete ────────────────────────────────────────────────
    cout << "\n--- Delete ---" << endl;

    cout << "  Deleting ID 340..." << endl;
    tree.root = tree.deleteNode(tree.root, 340);

    cout << "  Deleting ID 15..."  << endl;
    tree.root = tree.deleteNode(tree.root, 15);

    cout << "  Deleting ID 930..." << endl;
    tree.root = tree.deleteNode(tree.root, 930);

    // ── Inorder after deletion ────────────────────────────────
    cout << "\n--- Inorder After Deletions ---" << endl;
    tree.inorder(tree.root);

    // ── Confirm ───────────────────────────────────────────────
    cout << "\n--- Confirm Deletions ---" << endl;

    steps = 0;
    result = tree.search(tree.root, 340, steps);
    cout << "  Search ID 340: " << (result ? "FOUND" : "NOT FOUND") << endl;

    steps = 0;
    result = tree.search(tree.root, 120, steps);
    cout << "  Search ID 120: " << (result ? "FOUND" : "NOT FOUND") << endl;

    return 0;
}
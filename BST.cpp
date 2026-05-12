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
// Node structure with constructor
struct Node {
    Book book;
    Node* left;
    Node* right;

    Node(Book b) {
        book = b;
        left = nullptr;
        right = nullptr;
    }
};


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

    // DFS Traversals
    /*
    void inorder(Node* node) {
        if (node != nullptr) {
            inorder(node->left);
            cout << node->book.id << " ";
            inorder(node->right);
        }
    }*/
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



int main() {

    BST tree;
    // ── Insert books (sorted IDs — AVL handles this fine) ─────
    cout << "\n--- Inserting Books (Sorted IDs) ---" << endl;


    tree.root = tree.insert(tree.root, {450, "Clean Code",     "Robert Martin"});
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

/*
    tree.root = tree.insert(tree.root, 50);
    tree.root = tree.insert(tree.root, 30);
    tree.root = tree.insert(tree.root, 20);
    tree.root = tree.insert(tree.root, 40);
    tree.root = tree.insert(tree.root, 70);
    tree.root = tree.insert(tree.root, 60);
    tree.root = tree.insert(tree.root, 80);
*/


    /*
    cout << "BFS: ";
    tree.bfs(tree.root);
    cout << endl;



    cout << "BFS: ";
    tree.bfs(tree.root);
    cout << endl;
*/

    cout << "\n--- Inorder Traversal (sorted by ID) ---" << endl;
    tree.inorder(tree.root);


    // ── Range search ──────────────────────────────────────────
    cout << "\n--- Range Search [100 - 400] ---" << endl;
    tree.FindInRange(tree.root, 100, 400);

    cout << "\n--- Range Search [800 - 900] ---" << endl;
    tree.FindInRange(tree.root, 800, 900);

    // ── Closest ID ────────────────────────────────────────────
    cout << "\n--- Closest ID Search ---" << endl;

    Book* closest = tree.FindClosest(tree.root, 130);
    if (closest) cout << "  Closest to 130: [" << closest->id << "] " << closest->title << endl;

    closest = tree.FindClosest(tree.root, 555);
    if (closest) cout << "  Closest to 555: [" << closest->id << "] " << closest->title << endl;

    closest = tree.FindClosest(tree.root, 450);
    if (closest) cout << "  Closest to 450: [" << closest->id << "] " << closest->title << " (exact)" << endl;

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



    // Delete node
    tree.root = tree.deleteNode(tree.root, 450);

    cout << "After deleting 450\n";

    cout << "Inorder: \n";
    tree.inorder(tree.root);
    cout << endl;

    return 0;
}
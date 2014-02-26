/*  Nick Sweeting 2014/02/13
    Heap
    MIT License
*/

#include <stdlib.h>
#include <ctype.h>      // isdigit
#include <string>
#include <vector>       // vector of tokenized input
#include <iostream>     // cout
#include <sstream>      // istringstream
#include <math.h>       // pow
using namespace std;


const string delims = ",; ";

int stoint(string input) {
    int i = 0;
    istringstream(string(input)) >> i;
    return i;
}

string itostring(int i) {
    std::stringstream out;
    out << i;
    return out.str();
}

vector<int> tokenize(string input) {
    vector<int> tokens;
    size_t prev = 0, pos;
    // find next occurence of delimiter after prev delimiter, then append everything between those two positions
    while ((pos = input.find_first_of(delims, prev)) != string::npos) {
        if (pos > prev)
            tokens.push_back(stoint(input.substr(prev, pos-prev)));
        //tokens.push_back(stoint(input.substr(pos, 1)));                   // this adds the delimiter as a token as well, useful for catching operators as tokens (e.g. +-()*/^)
        prev = pos+1;
    }
    // catch trailing info after the last delimiter
    if (prev < input.length())
        tokens.push_back(stoint(input.substr(prev, string::npos)));

    return tokens;
}

// Heap implemented with a binary tree
template <class Type> class THeap {
    // Binary Tree Nodes
    private:
    struct node {
        Type value;
        node *left;
        node *right;

        node(Type val=NULL, node *lefti=0, node *righti=0) {
            value = val; left = lefti;  right = righti;
        }
    };

    node *root;       // Fixed start of list
    node *tail;       // Fixed end of list
    node *conductor;  // points to each node while looping over the whole list

    void resort() {

    }

    public:
    THeap() {}
    ~THeap() {};
};

// Heap implemented with a flat array and index math (implicit data structure)
class FHeap {
    // Math for flat heap indexes:
    //  Left = (index*2)+1
    //  Right = (index+1)*2
    //  Parent = floor((index-1)/2)
    //  Depth = floor(√(index)+0.5)  (0 indexed depth where the root is at depth=0, next level down is depth=1, ...)

    private:
        int heap[80];
        int heapsize;

        void sortbyparent(int index) {
            // recursive function that checks if(parent>current) {swap parent with current; then recurse with parent}
            if (index !=0) {
                int node = heap[index];
                int parent = floor((index-1)/2);
                //cout << "parent: " << heap[parent] << " node: " << node;
                if (node > heap[parent]) {
                    //cout << " SWAPPING";
                    heap[index] = heap[parent];
                    heap[parent] = node;
                }
                //cout << endl;
                sortbyparent(parent);
            }
        }

        void sortbychildren(int index) {
            // recursive function that checks if(left or right < current) {swap left or right with current; then recurse with left or right}
            int node = heap[index];
            int left = (index*2)+1;
            int right = (index+1)*2;
            //cout << "parent: " << heap[parent] << " node: " << node;
            if (node < heap[left] || node < heap[right]) {
                if (heap[left] > heap[right]) {
                    heap[index] = heap[left];
                    heap[left] = node;
                    sortbychildren(left);
                }
                else {
                    heap[index] = heap[right];
                    heap[right] = node;
                    sortbychildren(right);
                }
            }
        }

        string offset(int idx) {
            string spaces = "";
            int offset = int(floor(sqrt(heapsize-1)+0.5))-int(floor(sqrt(idx)+0.5));
            for (int i=0;i<offset;i++) {
                spaces += "    ";
            }
            if (!(offset))
                spaces += " ";
            return spaces;
        }

    public:
        FHeap() {heapsize=0;}
        ~FHeap() {};

        void add(int item) {
            heap[heapsize] = item;
            heapsize++;
            sortbyparent(heapsize-1);
        }

        int pop() {
            int root;
            root = heap[0];
            heap[0] = heap[heapsize-1];
            heap[heapsize-1] = 0;
            heapsize--;
            sortbychildren(0);
            return root;
        }

        void remove(int value) {
            int idx = 0;
            while (heap[idx] != value && idx < heapsize) idx++;
            int node;
            node = heap[idx];
            heap[0] = heap[heapsize-1];  // swap node to remove and lower rightmost node
            heap[heapsize-1] = 0;
            heapsize--;
            sortbyparent(idx);
            sortbychildren(0);
        }

        string unsorted() {
            string unsortedlist = "";
            for (int i=0;i<heapsize;i++) {
                unsortedlist += itostring(heap[i]);
                unsortedlist += ",";
            }
            return unsortedlist;
        }

        string sorted() {
            string sortedlist = "";
            for (int i=0;i<heapsize;i++) {
                sortedlist += itostring(heap[i]);
                sortedlist += ",";
            }
            return sortedlist;
        }

        int size() {
            return heapsize;
        }

        void print() {
            int idx = 0;
            // for each level in the tree
            for (int l=1;l<heapsize+1;l=l*2) {
                // print the nodes on that level
                for (int i=0;i<l;i++) {
                    if (idx < heapsize) {
                        cout << offset(idx) << "(" << heap[idx] << ")";
                    }
                    idx++;
                }
                cout << endl;
            }
        }
};


int main () {
    FHeap heap;

    string toaddtoheap;
    cout << "[i] Input numbers you would like added to the heap, separated by commas semicolons or spaces: ";
    cin >> toaddtoheap;
    cin.clear();

    vector<int> tokens = tokenize(toaddtoheap);
    if (tokens.size() == 0)
        return 1;
    for (int i=0; i<tokens.size(); i++)
        heap.add(tokens[i]);

    cout << "[o] Heap Diagram:" << endl;
    heap.print();
    cout << "[o] Heap Array:" << endl;
    cout << heap.unsorted() << endl;
    cout << "[o] Sorted Output:" << endl;
    cout << heap.sorted() << endl;

    string toremovefromheap;
    cout << "[i] Input numbers you would like removed from the heap, separated by commas semicolons or spaces: ";
    cin >> toremovefromheap;
    cin.clear();

    tokens = tokenize(toremovefromheap);
    for (int i=0; i<tokens.size(); i++)
        heap.remove(tokens[i]);

    cout << "[o] Heap diagram:" << endl;
    heap.print();
    cout << "[o] Unsorted heap:" << endl;
    cout << heap.unsorted() << endl;
    cout << "[o] Sorted heap:" << endl;
    cout << heap.sorted() << endl;
}

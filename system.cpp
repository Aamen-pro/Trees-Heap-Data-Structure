#include <bits/stdc++.h>
#define endl "\n"
using namespace std;

class Patient {
public:
    int id;
    string name;
    int severity;  // from 1 to 10
    int arrivalTime;  // second preiority
    Patient(int id , string name , int severity , int time){
        this->id = id;
        this->name = name;
        this->arrivalTime = time;
        this->severity = severity;
    }
};

class MaxHeap{
    private:
    // attributes
        vector<Patient> heap;
        int size{};
        // functions
        int p(int i){return (i-1)/2 ;}  // return the index of parent node
        int l(int i){return i*2 +1 ;} // return the index of the left node it's like to multiply by 2
        int r(int i){return i*2 +2 ;}  // return the index of the right node it's like to multiply by 2 +1
        public:
        int check (int patientId);
        void updateSeverity(int patientId , int newSeverity);
        bool isEmpty() const {return (this->size == 0);}
        bool higherPriority(Patient& a, Patient& b) const;
        Patient viewNextPatient() const ;
        void insertPatient(Patient val);
        void shiftUp(int i);
        Patient treatNextPatient();
        void shiftDown(int i);
        void printByLevel()const;
        void printByPriority()const;
};

// implementation of the functions of MaxHeap

Patient MaxHeap::treatNextPatient(){
    if (isEmpty()) {
        throw std::out_of_range("Heap is empty");
    }
    
    Patient maxval = heap[0];
    heap[0] = heap.back();
    heap.pop_back(); size--;
    if(size == 0) return maxval;  // to handle one element in the heap
    shiftDown(0);  // start from root
    return maxval;
}

void MaxHeap::printByPriority() const {
    if (isEmpty()) {
        cout << "Heap is empty\n";
        return;
    }

    // copy heap
    MaxHeap temp = *this;

    cout << "Patients by priority:\n";

    while (!temp.isEmpty()) {
        Patient x = temp.treatNextPatient();

        cout << "[ Name: " << x.name  << " , ";
        cout << "ID: " << x.id << " , ";
        cout << "Arrival Time: " << x.arrivalTime << " , ";
        cout << "Severity: " << x.severity  << " ]\n";
    }
    cout<<endl << "--------------------------------------------------------------------------------";
}

void MaxHeap::printByLevel()const{
    for(Patient x : heap){
        cout << "[ Name: " << x.name  << " , ";
        cout << "ID: " << x.id << " , ";
        cout << "Arrival Time: " << x.arrivalTime << " O'clock , ";
        cout << "Degree of Severity: " << x.severity  << " ]" << endl;
    }
    cout<<endl << "--------------------------------------------------------------------------------"<<endl;
}

int MaxHeap::check(int patientId){
    for (int i = 0; i < size; i++){
        if (heap[i].id == patientId)
            return i;
    }
    return -1;
}

void MaxHeap::updateSeverity(int patientId , int newSeverity){
    int i = check(patientId);
    
    if(i == -1){
        cout << "patient not found\n";
        return;
    }

    heap[i].severity = newSeverity; // update the severity 

    // fix the heap
    shiftUp(i);  
    shiftDown(i);
}

bool MaxHeap::higherPriority(Patient& a, Patient& b) const{
    return (a.severity > b.severity) ||
            (a.severity == b.severity && a.arrivalTime < b.arrivalTime);
}

Patient MaxHeap::viewNextPatient() const {
if (isEmpty())
    throw std::out_of_range("Heap is empty");
return heap[0];
}

void MaxHeap::shiftUp(int i){
    if(i == 0 ) return;  // base case
    // heap[i].severity > heap[p(i)].severity || (heap[i].severity == heap[p(i)].severity && heap[i].arrivalTime < heap[p(i)].arrivalTime)
    if(higherPriority(heap[i],heap[p(i)])){
        swap(heap[i],heap[p(i)]);  // if the node greater than parent swap till it became heapified
        shiftUp(p(i)); // recursive
    }
}

void MaxHeap::insertPatient(Patient val){
    heap.push_back(val);
    size++;
    shiftUp(size - 1);
    return;
}

void MaxHeap::shiftDown(int i){
    if(i >= size) return;  // to ensure the security
    int id = i;
if (l(i) < size) {
    if (higherPriority(heap[l(i)],heap[i])) {
        id = l(i);
    }
}
if (r(i) < size) {
    if (higherPriority(heap[r(i)],heap[id])) {
        id = r(i);
    }
}
    if(id != i){
        swap(heap[i], heap[id]);  // swap between node and the greater
        shiftDown(id);  // recursive 
    }
    return;
}



int main() {

    MaxHeap h;

    vector<Patient> patients = {
        {1,"ayman",1,7}, {2,"esmail",9,8}, {3,"emam",9,8},
        {4,"hamed",7,4}, {5,"mohamed",5,2}, {6,"ali",10,1},
        {7,"youssef",6,3}, {8,"kareem",8,6}, {9,"hassan",4,5},
        {10,"omar",3,9}, {11,"nour",2,10}, {12,"salma",7,11},
        {13,"fatma",9,12}, {14,"tarek",6,13}, {15,"ahmed",5,14},
        {16,"sara",8,15}, {17,"mostafa",10,16}, {18,"layla",4,17},
        {19,"ziad",3,18}, {20,"jana",2,19}, {21,"hamza",1,20}
    };

    for (Patient p : patients) {
        h.insertPatient(p);
    }

    cout << "Initial Heap (Level Order):\n";
    h.printByLevel();

    cout << "\nNext patient to treat:\n";
    Patient next = h.viewNextPatient();

    cout << "[ Name: " << next.name
         << " , ID: " << next.id
         << " , Severity: " << next.severity
         << " , Arrival Time: " << next.arrivalTime
         << " ]\n";

    cout << "\n====================================================\n";

    cout << "\nUpdating severity...\n";

    h.updateSeverity(1, 10);   
    h.updateSeverity(11, 9);  
    h.updateSeverity(21, 8);   

    cout << "\nHeap After Updates:\n";
    h.printByLevel();

    cout << "\n====================================================\n";

    cout << "\nTreating 5 patients:\n";

    for (int i = 0; i < 5; i++) {
        Patient treated = h.treatNextPatient();

        cout << "[ Treated -> Name: " << treated.name
             << " , ID: " << treated.id
             << " , Severity: " << treated.severity
             << " , Arrival Time: " << treated.arrivalTime
             << " ]\n";
    }

    cout << "\n====================================================\n";

    cout << "\nRemaining Patients (Level Order):\n";
    h.printByLevel();

    h.printByPriority();

    return 0;
}
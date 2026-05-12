#include <bits/stdc++.h>
#define endl "\n"
using namespace std;

class Patient {
public:
    int id;
    string name;
    int severity;  
    int arrivalTime;  
    Patient(int id , string name , int severity , int time){
        this->id = id;
        this->name = name;
        this->arrivalTime = time;
        this->severity = severity;
    }
};

class ComparePatient {
public:
    bool operator()(const Patient& a, const Patient& b) const {
        return (a.severity < b.severity) ||
               (a.severity == b.severity && a.arrivalTime > b.arrivalTime);
    }
};


void print(priority_queue<Patient, vector<Patient>, ComparePatient> pq) {
    while (!pq.empty()) {
        Patient x = pq.top();
        pq.pop();
        cout << endl;
        cout << "[ Name: " << x.name   << " , ";
        cout << "ID: "     << x.id     << " , ";
        cout << "Arrival Time: " << x.arrivalTime << " O'clock , ";
        cout << "Degree of Severity: " << x.severity << " ]" << endl;
    }
    cout << "------------------------------------------------------" << endl;
}

int main() {
    priority_queue<Patient, vector<Patient>, ComparePatient> h;

    Patient p(1,"ayman",1,7);
    Patient c(2,"esmail",9,8);
    Patient d(3,"emam",9,8);
    Patient e(4,"hamed",7,4);
    h.push(p);
    h.push(c);
    h.push(d);
    print(h);
    
    h.pop();
    
    print(h);
    
    h.push(e);
    
        
    print(h);
    
    return 0;
}
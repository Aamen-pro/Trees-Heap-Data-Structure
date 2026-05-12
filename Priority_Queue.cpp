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

void updateSeverity(priority_queue<Patient, vector<Patient>, ComparePatient>& pq, int patientId, int newSeverity) {

    vector<Patient> temp;
    bool found = false;

    // extract all patients
    while (!pq.empty()) {
        Patient p = pq.top();
        pq.pop();

        if (p.id == patientId) {
            p.severity = newSeverity;
            found = true;
        }

        temp.push_back(p);
    }

    for (auto &p : temp) {
        pq.push(p);
    }

    if (!found) {
        cout << "Patient not found\n";
    }
}

void print(priority_queue<Patient, vector<Patient>, ComparePatient> pq) {
    while (!pq.empty()) {
        Patient x = pq.top();
        pq.pop();
        cout << "[ Name: " << x.name   << " , ";
        cout << "ID: "     << x.id     << " , ";
        cout << "Arrival Time: " << x.arrivalTime << " O'clock , ";
        cout << "Degree of Severity: " << x.severity << " ]" << endl;
    }
    cout << "------------------------------------------------------" << endl;
}

int main() {

    priority_queue<Patient, vector<Patient>, ComparePatient> h;

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
        h.push(p);
    }

    cout << "All Patients:\n";
    print(h);

    cout << "\n====================================================\n";

    if (!h.empty()) {
        Patient top = h.top();

        cout << "\nNext patient to treat:\n";

        cout << "[ Name: " << top.name
            << " , ID: " << top.id
            << " , Severity: " << top.severity
            << " , Arrival Time: " << top.arrivalTime
            << " ]\n";
    }

    cout << "\n====================================================\n";

    updateSeverity(h, 1, 10);   
    updateSeverity(h, 11, 9);   
    updateSeverity(h, 21, 8);   

    cout << "\nPatients After Updates:\n";
    print(h);

    cout << "\n====================================================\n";

    cout << "\nTreating 5 patients:\n";

    for (int i = 0; i < 5 && !h.empty(); i++) {

        Patient treated = h.top();
        h.pop();

        cout << "[ Treated -> Name: " << treated.name
            << " , ID: " << treated.id
            << " , Severity: " << treated.severity
            << " , Arrival Time: " << treated.arrivalTime
            << " ]\n";
    }

    cout << "\n====================================================\n";

    cout << "\nRemaining Patients:\n";
    print(h);

    return 0;
}
#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>
using namespace std;

struct Student {

    int roll;
    string name;
    float marks;
};

vector<Student> students;

void inputStudents() {

    int n;
    cout << "Enter number of students: ";
    cin >> n;

    for (int i = 0; i < n; ++i) {

        Student s;

        cout << "Enter details for student " << i + 1 << ":\n";
        cout << "Roll Number: ";
        cin >> s.roll;
        cin.ignore();  

        cout << "Name: ";
        getline(cin, s.name);
        cout << "Marks: ";
        cin >> s.marks;

        students.push_back(s);
    }
}

void displayAll() {

    if (students.empty()) {
        cout << "No records to display.\n";
        return;
    }

    cout << " Student Records \n";
    cout << left << setw(10) << "Roll" << setw(20) << "Name" << "Marks\n";

    for (int i = 0; i < students.size(); ++i) {
        cout << left << setw(10) << students[i].roll
             << setw(20) << students[i].name
             << students[i].marks << endl;
    }
}

void showStats() {

    if (students.empty()) {
        cout << "No data available.\n";
        return;
    }

    float total = 0;
    float highest = students[0].marks;
    float lowest = students[0].marks;
    Student top = students[0];
    Student bottom = students[0];

    for (int i = 0; i < students.size(); ++i) {
        total += students[i].marks;
        if (students[i].marks > highest) {
            highest = students[i].marks;
            top = students[i];
        }
        if (students[i].marks < lowest) {
            lowest = students[i].marks;
            bottom = students[i];
        }
    }

    float avg = total / students.size();

    cout << "Average Marks: " << avg << endl;
    cout << "Highest Scorer: " << top.name << " (" << top.marks << ")\n";
    cout << "Lowest Scorer: " << bottom.name << " (" << bottom.marks << ")\n";
}

void sortByMarks() {

    if (students.empty()) {
        cout << "No data to sort.\n";
        return;
    }

    sort(students.begin(), students.end(), [](Student a, Student b) {
        return a.marks > b.marks;
    });

    cout << " Students sorted by marks (high to low):\n";
    displayAll();
}

int main() {

    int choice;
    
do {
    cout << "Student Marks Analyzer\n";
    cout << "1. Input Student Data\n";
    cout << "2. Display All Students\n";
    cout << "3. Show Statistics\n";
    cout << "4. Sort by Marks\n";
    cout << "5. Exit\n";
    cout << "Enter your choice: \n";
    cin >> choice;

    switch (choice) {
        case 1:
            inputStudents();
            break;
        case 2:
            displayAll();
            break;
        case 3:
            showStats();
            break;
        case 4:
            sortByMarks();
            break;
        case 5:
            cout << "Exiting program \n";
            break;
        default:
            cout << "Invalid choice! Please try again.\n";
    }
} while (choice != 5);

    return 0;
}

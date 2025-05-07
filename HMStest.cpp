#include <iostream>

#include <vector>
#include <string>

#include <stdexcept>
using namespace std;

bool isValidDate(const string& date) {

    if (date.size() != 10 || date[2] != '/' || date[5] != '/')
        return false;

    try {

        int day = stoi(date.substr(0, 2));
        int month = stoi(date.substr(3, 2));
        int year = stoi(date.substr(6, 4));

        if (month < 1 || month > 12) return false;

        int daysInMonth[] = { 31, (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) ? 29 : 28,
                              31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

        return (day >= 1 && day <= daysInMonth[month - 1]);

    } catch (...) {
        return false;
    }
}

bool isValidName(const string& name) {

    for (char c : name) {

        if (isdigit(c)) return false;
    }
    return true;
}

bool isValidPhoneNumber(const string& phone) {
    if (phone.length() != 10) return false;
    for (char c : phone) {
        if (!isdigit(c)) return false;
    }
    return true;
}



class Person {

protected:
    string name;

public:

    Person(const string& n) : name(n) {}
    virtual ~Person() {}
    virtual void displayOptions() const = 0;
};


class Patient {

    string name, phoneNumber, appointmentDate;
    int previousAdmittances;
    double paymentDue;
    bool hasAppointment;

public:

    Patient(string n, int admit, double payment, bool appt, string date, string phone){
        name = n ;
        previousAdmittances = admit ;
        paymentDue = payment ;
        phoneNumber = phone ;
        appointmentDate = date ;
        hasAppointment = appt ;
    }

    void displayDetails() const {

        cout << "Name: " << name
             << "\nPrevious Admittances: " << previousAdmittances
             << "\nPayment Due: " << paymentDue
             << "\nAppointment Scheduled: " << (hasAppointment ? "Yes" : "No") << endl;

        if (hasAppointment) cout << "Appointment Date: " << appointmentDate << endl;
        cout << "Phone Number: " << phoneNumber << endl;
    }

    bool matchesName(const string& searchName) const {
        return name == searchName;
    }

    void displayDues() const {
        cout << "Payment Due: " << paymentDue << endl;
    }

    void displayAppointments() const {
        cout << "Appointment: " << (hasAppointment ? "Yes" : "No") << endl;
        if (hasAppointment) cout << "Date: " << appointmentDate << endl;
    }

    void updateAppointment(const string& date) {

        if (!isValidDate(date)) throw invalid_argument("Invalid appointment date. Use DD/MM/YYYY format.");
        hasAppointment = true;
        appointmentDate = date;
        cout << "Appointment updated to: " << appointmentDate << endl;
    }

    void cancelAppointment() {

        hasAppointment = false;
        appointmentDate = "";
        cout << "Appointment cancelled." << endl;
    }
};


template <typename RoleType>
class Staff : public Person {

public:
    Staff(const string& n) : Person(n) {}

    void displayOptions() const override {

        cout << RoleType::roleName() << " " << name << " Options:\n"
             << "1. Patient Details\n"
             << "2. Inventory\n"
             << "3. Earnings\n"
             << "4. Update Appointment\n"
             << "5. Cancel Appointment\n";
    }

    void displayPatientDetails(const vector<Patient>& patients) const {

        for (const auto& p : patients) {
            p.displayDetails();
            cout << endl;
        }
    }

    void displayInventory() const {

        cout << "Inventory:\n"
             << "B+ Blood bags: 12\n"
             << "A+ Blood bags: 0\n"
             << "O+ Blood bags: 40\n"
             << "Syringes: 3300\n"
             << "Crocin: 4500\n"
             << "Hydrochloroquine: Check with chemist\n";
    }

    void displayEarnings() const {

        cout << "Earnings: $45,000 salary due at month-end" << endl;
    }

    void updatePatientAppointment(vector<Patient>& patients) const {

        string pname ;
        string date ;

        cout << "Enter patient name to update appointment: ";

        cin.ignore();
        getline(cin, pname);

        for (auto& p : patients) {

            if (p.matchesName(pname)) {

                cout << "Enter new appointment date (DD/MM/YYYY): ";
                getline(cin, date);
                try {
                    p.updateAppointment(date);
                } catch (const exception& e) {
                    cerr << "Error: " << e.what() << endl;
                }
                return;
            }
        }
        cout << "Patient not found." << endl;
    }

    void cancelPatientAppointment(vector<Patient>& patients) const {

        string pname;

        cout << "Enter patient name to cancel appointment: ";
        cin.ignore();

        getline(cin, pname);

        for (auto& p : patients) {

            if (p.matchesName(pname)) {
                p.cancelAppointment();
                return;
            }
        }
        cout << "Patient not found." << endl;
    }
};

struct DoctorRole {
    
    static string roleName() { return "Doctor"; }
};

struct NurseRole {
    static string roleName() { return "Nurse"; }
};

using Doctor = Staff<DoctorRole>;
using Nurse = Staff<NurseRole>;

class PatientUser : public Person {
    vector<Patient>& patients;

public:
    PatientUser(string n, vector<Patient>& p) : Person(n), patients(p) {}

    void displayOptions() const override {
        try {
            for (const auto& p : patients) {
                if (p.matchesName(name)) {
                    p.displayDetails();
                    int choice;
                    cout << "\n1. Dues\n2. Appointments\nEnter your choice: ";
                    cin >> choice;
                    if (choice == 1) p.displayDues();
                    else if (choice == 2) p.displayAppointments();
                    else cout << "Invalid choice.\n";
                    return;
                }
            }
            throw runtime_error("No patient found with this name.");
        } catch (exception& e) {
            cerr << "Error: " << e.what() << endl;
        }
    }
};

void addNewPatient(vector<Patient>& patients) {

    string name ;
    string phone ;
    string date;
    int admit;
    double due;
    bool hasAppt;

    cin.ignore();

    cout << "Enter name: ";
    getline(cin, name);

    if (!isValidName(name)) {
        cout << "Error: Name cannot contain numeric values.\n";
        return;
    }

    cout << "Previous admittances: ";
    cin >> admit;
    cout << "Payment due: ";
    cin >> due;
    cout << "Has appointment? (1/0): ";
    cin >> hasAppt;
    cin.ignore();

    if (hasAppt) {
        cout << "Appointment date (DD/MM/YYYY): ";
        getline(cin, date);

        if (!isValidDate(date)) {
            cout << "Error: Invalid appointment date.\n";
            return;
        }
    } else date = "";

    cout << "Phone number: ";
    getline(cin, phone);

    if (!isValidPhoneNumber(phone)) {
        cout << "Error: Invalid phone number. Must be exactly 10 digits.\n";
        return;
    }
    


    for (const auto& p : patients) {
        if (p.matchesName(name)) {
            cout << "Patient already exists.\n";
            return;
        }
    }

    patients.emplace_back(name, admit, due, hasAppt, date, phone);
    cout << "Patient added.\n";
}

template <typename StaffType>

void handleStaff(StaffType& staff, vector<Patient>& patients) {

    staff.displayOptions();
    int opt;

    cout << "Enter choice: ";
    cin >> opt;

    switch (opt) {

        case 1: staff.displayPatientDetails(patients);
         break;

        case 2: staff.displayInventory();
         break;

        case 3: staff.displayEarnings();
         break;

        case 4: staff.updatePatientAppointment(patients);
         break;

        case 5: staff.cancelPatientAppointment(patients);
         break;

        default: cout << "Invalid option.\n";
    }
}

int main() {

    vector<Patient> patients;
    int choice;

    while (true) {

        cout << "\nHospital Management System\n"
             << "1. Patient\n2. Nurse\n3. Doctor\n4. Add New Patient\n5. Exit\n"
             << "Enter choice: ";
        cin >> choice;

        switch (choice) {

            case 1: {

                string pname;
                cout << "Enter your name: ";
                cin.ignore();
                getline(cin, pname);
                PatientUser(pname, patients).displayOptions();
                break;
            }

            case 2: {

                string nname;
                cout << "Enter Nurse's name: ";
                cin.ignore();
                getline(cin, nname);
                Nurse nurse(nname);
                handleStaff(nurse, patients);
                break;
            }

            case 3: {

                string dname;
                cout << "Enter Doctor's name: ";
                cin.ignore();
                getline(cin, dname);
                Doctor doc(dname);
                handleStaff(doc, patients);
                break;
            }
            
            case 4:

                addNewPatient(patients);
                break;

            case 5:

                cout << "Exiting...\n";
                return 0;

            default:

                cout << "Invalid selection.\n";
        }
    }

    return 0;
}

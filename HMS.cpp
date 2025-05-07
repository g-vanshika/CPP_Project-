#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <limits>
#include <stdexcept>
#include <algorithm>
#include <map>
#include <set>
#include <memory>

using namespace std;

class Doctor;
class Nurse;
class Receptionist;
class Administrator;
class Patient;
class Appointment;
class Inventory;
class Record;

class InvalidInputException : public runtime_error {
public:
    InvalidInputException(const string& message) : runtime_error(message) {}
};

class RecordNotFoundException : public runtime_error {
public:
    RecordNotFoundException(const string& message) : runtime_error(message) {}
};

class PatientNotFoundException : public runtime_error {
public:
    PatientNotFoundException(const string& message) : runtime_error(message) {}
};

class InsufficientInventoryException : public runtime_error {
public:
    InsufficientInventoryException(const string& message) : runtime_error(message) {}
};

template <typename T>
void displayDetails(const T& obj) {
    obj.display();
}

int getValidIntegerInput(const string& prompt) {
    int value;
    cout << prompt;
    cin >> value;
    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        throw InvalidInputException("Invalid integer input. Please try again.");
    }
    return value;
}

double getValidDoubleInput(const string& prompt) {
    double value;
    cout << prompt;
    cin >> value;
    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        throw InvalidInputException("Invalid floating-point input. Please try again.");
    }
    return value;
}

bool getYesNoInput(const string& prompt) {
    char response;
    cout << prompt << " (y/n): ";
    cin >> response;
    if (cin.fail() || (response != 'y' && response != 'n')) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        throw InvalidInputException("Invalid input. Please enter 'y' or 'n'.");
    }
    return response == 'y';
}

class Appointment {
private:
    string patientName;
    string dateAndTime;
    string doctorName;

public:
    Appointment(const string& patient, const string& dateTime, const string& doctor)
        : patientName(patient), dateAndTime(dateTime), doctorName(doctor) {}

    void display() const {
        cout << "Patient: " << patientName << endl;
        cout << "Date and Time: " << dateAndTime << endl;
        cout << "Doctor: " << doctorName << endl;
    }

    const string& getPatientName() const {
        return patientName;
    }

    const string& getDateAndTime() const {
        return dateAndTime;
    }

    const string& getDoctorName() const {
        return doctorName;
    }
};

class Patient {
private:
    string name;
    int previousAdmittances;
    double paymentDue;
    bool hasAppointment;
    string appointmentDate;
    string phoneNumber;

public:
    Patient(const string& n, int prevAdmit, double payment, bool appointment, const string& date, const string& phone)
        : name(n), previousAdmittances(prevAdmit), paymentDue(payment), hasAppointment(appointment), appointmentDate(date), phoneNumber(phone) {}

    void display() const {
        cout << "Name: " << name << endl;
        cout << "Previous Admittances: " << previousAdmittances << endl;
        cout << "Payment Due: " << paymentDue << endl;
        cout << "Appointment Scheduled: " << (hasAppointment ? "Yes" : "No") << endl;
        if (hasAppointment) {
            cout << "Appointment Date: " << appointmentDate << endl;
        }
        cout << "Phone Number: " << phoneNumber << endl;
    }

    bool matchesName(const string& searchName) const {
        return name == searchName;
    }

    void displayDues() const {
        cout << "Payment Due: " << paymentDue << endl;
    }

    void displayAppointments() const {
        cout << "Appointment Scheduled: " << (hasAppointment ? "Yes" : "No") << endl;
        if (hasAppointment) {
            cout << "Appointment Date: " << appointmentDate << endl;
        }
    }

    const string& getName() const {
        return name;
    }
};

class Inventory {
private:
    map<string, int> items;

public:
    Inventory() {
        items["B+ Blood bags"] = 12;
        items["A+ Blood bags"] = 0;
        items["O+ Blood bags"] = 40;
        items["Syringes"] = 3300;
        items["Crocin"] = 4500;
        items["Hydrochloroquine"] = 100;
    }

    void addItem(const string& itemName, int quantity) {
        items[itemName] += quantity;
    }

    void removeItem(const string& itemName, int quantity) {
        if (items.count(itemName) && items[itemName] >= quantity) {
            items[itemName] -= quantity;
        } else {
            throw InsufficientInventoryException("Insufficient quantity of " + itemName + " in inventory.");
        }
    }

    void display() const {
        cout << "Inventory Records:" << endl;
        for (const auto& pair : items) {
            cout << pair.first << ": " << pair.second << " in quantity" << endl;
        }
    }
};

class Staff {
protected:
    string name;
    double salary;

public:
    Staff(const string& n, double s) : name(n), salary(s) {}
    virtual void displayEarnings() const = 0;
    virtual void displayPatientDetails(const vector<unique_ptr<Patient>>& patients) const = 0;
    virtual void displayInventory(const Inventory& inventory) const = 0;
    virtual void manageAppointments(const vector<unique_ptr<Appointment>>& appointments, const vector<unique_ptr<Patient>>& patients, const vector<unique_ptr<Doctor>>& doctors) const {} // Default implementation for those who don't manage appointments
    virtual void manageInventorySystem(Inventory& inventory) const {} // Default implementation
    virtual ~Staff() {}

    const string& getName() const {
        return name;
    }
};

class Doctor : public Staff {
public:
    Doctor(const string& n) : Staff(n, 150000.0) {}

    void displayEarnings() const override {
        cout << "Doctor " << name << " Earnings: $" << salary / 12.0 << " salary due at end of month" << endl;
    }

    void displayPatientDetails(const vector<unique_ptr<Patient>>& patients) const override {
        cout << "Doctor " << name << ", here are the patient details:" << endl;
        for (const auto& patient : patients) {
            displayDetails(*patient);
            cout << endl;
        }
    }

    void displayInventory(const Inventory& inventory) const override {
        inventory.display();
    }
};

class Nurse : public Staff {
public:
    Nurse(const string& n) : Staff(n, 75000.0) {}

    void displayEarnings() const override {
        cout << "Nurse " << name << " Earnings: $" << salary / 12.0 << " salary due at end of month" << endl;
    }

    void displayPatientDetails(const vector<unique_ptr<Patient>>& patients) const override {
        cout << "Nurse " << name << ", here are the patient details:" << endl;
        for (const auto& patient : patients) {
            displayDetails(*patient);
            cout << endl;
        }
    }

    void displayInventory(const Inventory& inventory) const override {
        inventory.display();
    }
};

class Receptionist : public Staff {
public:
    Receptionist(const string& n) : Staff(n, 60000.0) {}

    void displayEarnings() const override {
        cout << "Receptionist " << name << " Earnings: $" << salary / 12.0 << " salary due at end of month" << endl;
    }

    void displayPatientDetails(const vector<unique_ptr<Patient>>& patients) const override {
        cout << "Receptionist " << name << ", here are the patient details:" << endl;
        for (const auto& patient : patients) {
            displayDetails(*patient);
            cout << endl;
        }
    }

    void displayInventory(const Inventory& inventory) const override {
        cout << "Receptionists do not typically view inventory." << endl;
    }

    void manageAppointments(const vector<unique_ptr<Appointment>>& appointments, const vector<unique_ptr<Patient>>& patients, const vector<unique_ptr<Doctor>>& doctors) const override {
        cout << "--- Appointment Management ---" << endl;
        int choice;
        do {
            cout << "\n1. Add New Appointment" << endl;
            cout << "2. Display All Appointments" << endl;
            cout << "0. Back to Main Menu" << endl;
            cout << "Enter your choice: ";
            try {
                choice = getValidIntegerInput("");
                switch (choice) {
                    case 1: {
                        string patientName, doctorName, dateTime;

                        cout << "Enter patient name for the appointment: ";
                        cin >> patientName;

                        auto patientIt = find_if(patients.begin(), patients.end(),
                                             [&](const unique_ptr<Patient>& p){ return p->getName() == patientName; });
                        if (patientIt == patients.end()) {
                            throw PatientNotFoundException("Patient not found!");
                        }

                        cout << "Enter doctor's name for the appointment: ";
                        cin >> doctorName;
                        // Ideally, you'd validate if the doctor exists here

                        cout << "Enter appointment date and time (e.g.,YYYY-MM-DD HH:MM): ";
                        cin.ignore();
                        getline(cin, dateTime);

                        // In a real system, you'd have a way to add this new appointment
                        cout << "New appointment scheduled for " << patientName << " with " << doctorName << " on " << dateTime << endl;
                        // appointments.push_back(make_unique<Appointment>(patientName, dateTime, doctorName));
                        break;
                    }
                    case 2: {
                        cout << "--- All Scheduled Appointments ---" << endl;
                        if (appointments.empty()) {
                            cout << "No appointments scheduled." << endl;
                        } else {
                            for (const auto& appointment : appointments) {
                                displayDetails(*appointment);
                                cout << "------------------------------" << endl;
                            }
                        }
                        break;
                    }
                    case 0:
                        cout << "Returning to main menu." << endl;
                        break;
                    default:
                        cout << "Invalid choice!" << endl;
                }
            } catch (const InvalidInputException& e) {
                cerr << "Error: " << e.what() << endl;
            } catch (const PatientNotFoundException& e) {
                cerr << "Error: " << e.what() << endl;
            }
        } while (choice != 0);
    }
};

class Administrator : public Staff {
public:
    Administrator(const string& n) : Staff(n, 90000.0) {}

    void displayEarnings() const override {
        cout << "Administrator " << name << " Earnings: $" << salary / 12.0 << " salary due at end of month" << endl;
    }

    void displayPatientDetails(const vector<unique_ptr<Patient>>& patients) const override {
        cout << "Administrator " << name << ", here are the patient details:" << endl;
        for (const auto& patient : patients) {
            displayDetails(*patient);
            cout << endl;
        }
    }

    void displayInventory(const Inventory& inventory) const override {
        inventory.display();
    }

    void manageInventorySystem(Inventory& inventory) const override {
        manageInventorySystem(inventory); // Reusing the existing inventory management function
    }
};

void handlePatient(const vector<unique_ptr<Patient>>& patients) {
    cout << "Welcome, Patient! You can check your details." << endl;
    string inputName;
    cout << "Enter your name: ";
    cin >> inputName;
    auto it = find_if(patients.begin(), patients.end(),
                        [&](const unique_ptr<Patient>& p){ return p->matchesName(inputName); });

    if (it != patients.end()) {
        cout << "Patient details found:" << endl;
        displayDetails(**it);
        int choice;
        cout << "What would you like to know?" << endl;
        cout << "1. Dues" << endl;
        cout << "2. Appointments" << endl;
        cout << "Enter your choice (1-2): ";
        try {
            choice = getValidIntegerInput("");
            switch (choice) {
                case 1:
                    (*it)->displayDues();
                    break;
                case 2:
                    (*it)->displayAppointments();
                    break;
                default:
                    cout << "Invalid choice!" << endl;
            }
        } catch (const InvalidInputException& e) {
            cerr << "Error: " << e.what() << endl;
        }
    } else {
        cout << "No records found for " << inputName << "!" << endl;
    }
}

void handleNurse(const vector<unique_ptr<Patient>>& patients, const Inventory& inventory) {
    cout << "Welcome, Nurse!" << endl;
    string nurseName;
    cout << "Enter Nurse's name: ";
    cin >> nurseName;
    Nurse nurse(nurseName);

    int option;
    cout << "What would you like to see?" << endl;
    cout << "1. Patient Details" << endl;
    cout << "2. Inventory Records" << endl;
    cout << "3. Earnings" << endl;
    cout << "Enter your choice (1-3): ";
    try {
        option = getValidIntegerInput("");
        switch (option) {
            case 1:
                nurse.displayPatientDetails(patients);
                break;
            case 2:
                nurse.displayInventory(inventory);
                break;
            case 3:
                nurse.displayEarnings();
                break;
            default:
                cout << "Invalid choice!" << endl;
        }
    } catch (const InvalidInputException& e) {
        cerr << "Error: " << e.what() << endl;
    }
}

void handleDoctor(const vector<unique_ptr<Patient>>& patients, const Inventory& inventory) {
    cout << "Welcome, Doctor!" << endl;
    string doctorName;
    cout << "Enter Doctor's name: ";
    cin >> doctorName;
    Doctor doctor(doctorName);

    int option;
    cout << "What would you like to see?" << endl;
    cout << "1. Patient Details" << endl;
    cout << "2. Inventory Records" << endl;
    cout << "3. Earnings" << endl;
    cout << "Enter your choice (1-3): ";
    try {
        option = getValidIntegerInput("");
        switch (option) {
            case 1:
                doctor.displayPatientDetails(patients);
                break;
            case 2:
                doctor.displayInventory(inventory);
                break;
            case 3:
                doctor.displayEarnings();
                break;
            default:
                cout << "Invalid choice!" << endl;
        }
    } catch (const InvalidInputException& e) {
        cerr << "Error: " << e.what() << endl;
    }
}

void handleReceptionist(const vector<unique_ptr<Appointment>>& appointments, const vector<unique_ptr<Patient>>& patients, const vector<unique_ptr<Doctor>>& doctors) {
    cout << "Welcome, Receptionist!" << endl;
    string receptionistName;
    cout << "Enter Receptionist's name: ";
    cin >> receptionistName;
    Receptionist receptionist(receptionistName);

    int option;
    cout << "What would you like to do?" << endl;
    cout << "1. Manage Appointments" << endl;
    cout << "2. View Patient Details" << endl;
    cout << "3. View Earnings" << endl;
    cout << "Enter your choice (1-3): ";
    try {
        option = getValidIntegerInput("");
        switch (option) {
            case 1:
                receptionist.manageAppointments(appointments, patients, doctors);
                break;
            case 2: {
                string patientName;
                cout << "Enter patient name to view details: ";
                cin >> patientName;
                auto it = find_if(patients.begin(), patients.end(),
                                     [&](const unique_ptr<Patient>& p){ return p->matchesName(patientName); });
                if (it != patients.end()) {
                    displayDetails(**it);
                } else {
                    cout << "Patient not found!" << endl;
                }
                break;
            }
            case 3:
                receptionist.displayEarnings();
                break;
            default:
                cout << "Invalid choice!" << endl;
        }
    } catch (const InvalidInputException& e) {
        cerr << "Error: " << e.what() << endl;
    }
}

void handleAdministrator(Inventory& inventory, const vector<unique_ptr<Patient>>& patients) {
    cout << "Welcome, Administrator!" << endl;
    string adminName;
    cout << "Enter Administrator's name: ";
    cin >> adminName;
    Administrator administrator(adminName);

    int choice;
    cout << "What would you like to do?" << endl;
    cout << "1. Manage Inventory" << endl;
    cout << "2. View Patient Details" << endl;
    cout << "3. View Earnings" << endl;
    cout << "Enter your choice (1-3): ";
    try {
        choice = getValidIntegerInput("");
        switch (choice) {
            case 1:
                administrator.manageInventorySystem(inventory);
                break;
            case 2:
                 administrator.displayPatientDetails(patients);
                 break;
            case 3:
                administrator.displayEarnings();
                break;
            default:
                cout << "Invalid choice!" << endl;
        }
    } catch (const InvalidInputException& e) {
        cerr << "Error: " << e.what() << endl;
    }
}

void addNewPatient(vector<unique_ptr<Patient>>& patients) {
    string name, phoneNumber, appointmentDate = "";
    int previousAdmittances = 0;
    double paymentDue = 0.0;
    bool hasAppointment = false;

    cout << "Enter patient name: ";
    cin >> name;

    try {
        previousAdmittances = getValidIntegerInput("Enter previous admittances: ");
        paymentDue = getValidDoubleInput("Enter payment due: ");
        hasAppointment = getYesNoInput("Does the patient have an appointment?");
        if (hasAppointment) {
            cout << "Enter appointment date and time: ";
            cin >> appointmentDate;
        }
        cout << "Enter phone number: ";
        cin >> phoneNumber;

        patients.push_back(make_unique<Patient>(name, previousAdmittances, paymentDue, hasAppointment, appointmentDate, phoneNumber));
        cout << "New patient added successfully!" << endl;
    } catch (const InvalidInputException& e) {
        cerr << "Error: " << e.what() << endl;
    }
}

void addNewAppointment(vector<unique_ptr<Appointment>>& appointments, const vector<unique_ptr<Patient>>& patients, const vector<unique_ptr<Doctor>>& doctors) {
    string patientName, doctorName, dateTime;

    cout << "Enter patient name for the appointment: ";
    cin >> patientName;

    auto patientIt = find_if(patients.begin(), patients.end(),
                                 [&](const unique_ptr<Patient>& p){ return p->getName() == patientName; });
    if (patientIt == patients.end()) {
        cout << "Patient not found!" << endl;
        return;
    }

    cout << "Enter doctor's name for the appointment: ";
    cin >> doctorName;

    cout << "Enter appointment date and time (e.g.,YYYY-MM-DD HH:MM): ";
    cin.ignore();
    getline(cin, dateTime);

    appointments.push_back(make_unique<Appointment>(patientName, dateTime, doctorName));
    cout << "New appointment scheduled successfully!" << endl;
}

void displayAllAppointments(const vector<unique_ptr<Appointment>>& appointments) {
    cout << "--- All Scheduled Appointments ---" << endl;
    if (appointments.empty()) {
        cout << "No appointments scheduled." << endl;
        return;
    }
    for (const auto& appointment : appointments) {
        displayDetails(*appointment);
        cout << "------------------------------" << endl;
    }
}

void searchPatientByName(const vector<unique_ptr<Patient>>& patients) {
    string searchName;
    cout << "Enter the name of the patient to search: ";
    cin >> searchName;

    auto it = find_if(patients.begin(), patients.end(),
                        [&](const unique_ptr<Patient>& p){ return p->matchesName(searchName); });

    if (it != patients.end()) {
        cout << "Patient found:" << endl;
        displayDetails(**it);
    } else {
        cout << "Patient with name '" << searchName << "' not found." << endl;
    }
}

void manageInventory(Inventory& inventory) {
    int choice;
    cout << "\n--- Inventory Management ---" << endl;
    cout << "1. Add Item" << endl;
    cout << "2. Remove Item" << endl;
    cout << "3. Display Inventory" << endl;
    cout << "4. Back to Main Menu" << endl;
    cout << "Enter your choice: ";

    try {
        choice = getValidIntegerInput("");
        switch (choice) {
            case 1: {
                string itemName;
                int quantity;
                cout << "Enter item name to add: ";
                cin >> itemName;
                quantity = getValidIntegerInput("Enter quantity to add: ");
                inventory.addItem(itemName, quantity);
                cout << itemName << " added to inventory." << endl;
                break;
            }
            case 2: {
                string itemName;
                int quantity;
                cout << "Enter item name to remove: ";
                cin >> itemName;
                quantity = getValidIntegerInput("Enter quantity to remove: ");
                try {
                    inventory.removeItem(itemName, quantity);
                    cout << quantity << " of " << itemName << " removed from inventory." << endl;
                } catch (const InsufficientInventoryException& e) {
                    cerr << "Error: " << e.what() << endl;
                }
                break;
            }
            case 3:
                inventory.display();
                break;
            case 4:
                cout << "Returning to main menu." << endl;
                break;
            default:
                cout << "Invalid choice!" << endl;
        }
    } catch (const InvalidInputException& e) {
        cerr << "Error: " << e.what() << endl;
    }
}

int main() {
    vector<unique_ptr<Patient>> patients;
    patients.push_back(make_unique<Patient>("Vanshika", 2, 30000.0, true, "Saturday, 11th May, 7:30 PM", "7838186547"));
    patients.push_back(make_unique<Patient>("Anant", 1, 10000.0, false, "", "9812343210"));
    patients.push_back(make_unique<Patient>("Kanishka", 3, 0.0, false, "", "7890343210"));
    patients.push_back(make_unique<Patient>("Naysha ", 0, 0.0, true, "Friday, 10th May, 3:00 PM", "88880343210"));

    vector<unique_ptr<Appointment>> appointments;
    appointments.push_back(make_unique<Appointment>(" Vanshika", "2025-05-11 19:30", "Dr. Smith"));
    appointments.push_back(make_unique<Appointment>("Kanishka", "2025-05-10 15:00", "Dr. Jones"));

    Inventory inventory;
    vector<unique_ptr<Doctor>> doctors;
    doctors.push_back(make_unique<Doctor>("Dr. Smith"));
    doctors.push_back(make_unique<Doctor>("Dr. Jones"));
    vector<unique_ptr<Nurse>> nurses;
    nurses.push_back(make_unique<Nurse>("Nurse Alice"));
    nurses.push_back(make_unique<Nurse>("Nurse Bob"));
    vector<unique_ptr<Receptionist>> receptionists;
    receptionists.push_back(make_unique<Receptionist>("Receptionist Carol"));
    vector<unique_ptr<Administrator>> administrators;
    administrators.push_back(make_unique<Administrator>("Admin Dave"));cout << "Welcome to the HospitalManagement System!" << endl;

    int choice;
    do {
        cout << "\nPlease select your role:" << endl;
        cout << "1. Patient" << endl;
        cout << "2. Nurse" << endl;
        cout << "3. Doctor" << endl;
        cout << "4. Receptionist" << endl;
        cout << "5. Administrator" << endl;
        cout << "6. Add New Patient" << endl;
        cout << "7. Add New Appointment" << endl;
        cout << "8. Display All Appointments" << endl;
        cout << "9. Search Patient by Name" << endl;
        cout << "10. Manage Inventory" << endl;
        cout << "0. Exit" << endl;
        cout << "Enter your choice: ";

        try {
            choice = getValidIntegerInput("");
            switch (choice) {
                case 1:
                    handlePatient(patients);
                    break;
                case 2:
                    handleNurse(patients, inventory);
                    break;
                case 3:
                    handleDoctor(patients, inventory);
                    break;
                case 4:
                    handleReceptionist(appointments, patients, doctors);
                    break;
                case 5:
                    handleAdministrator(inventory, patients);
                    break;
                case 6:
                    addNewPatient(patients);
                    break;
                case 7:
                    addNewAppointment(appointments, patients, doctors);
                    break;
                case 8:
                    displayAllAppointments(appointments);
                    break;
                case 9:
                    searchPatientByName(patients);
                    break;
                case 10:
                    manageInventory(inventory);
                    break;
                case 0:
                    cout << "Exiting the system. Goodbye!" << endl;
                    break;
                default:
                    cout << "Invalid choice! Please enter a valid option." << endl;
            }
        } catch (const InvalidInputException& e) {
            cerr << "Error: " << e.what() << endl;
        }

    } while (choice != 0);

    return 0;
}

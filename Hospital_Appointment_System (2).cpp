#include <iostream>
#include <string>
#include <unordered_map>
#include <queue>
#include <vector>
#include <algorithm>
#include <fstream>

using namespace std;

const int maxPatientsPerSlot = 2; // Max patients allowed per time slot

// Structure to hold patient details
struct Patient {
    string name;
    int age;
    string id;
    int severity; // >0 = emergency, 0 = normal
};

// Node for linked list of appointments
struct AppointmentNode {
    Patient patient;
    AppointmentNode* next;
    AppointmentNode(Patient p) : patient(p), next(nullptr) {}
};

// Comparator for emergency queue (higher severity first)
struct CompareSeverity {
    bool operator()(Patient a, Patient b) {
        return a.severity < b.severity;
    }
};

// Class handling appointment system
class HospitalAppointmentSystem {
private:
    vector<string> slots = {"9am", "10am", "11am", "12pm"}; // Fixed time slots
    unordered_map<string, AppointmentNode*> slotMap; // Maps slot to linked list of appointments
    priority_queue<Patient, vector<Patient>, CompareSeverity> emergencyQueue; // Emergency queue
    unordered_map<string, string> idToSlotMap; // Maps patient ID to slot for search/cancel

public:
    // Function to log any action to log.txt
    void logAction(const string& message) {
        ofstream log("log.txt", ios::app);
        log << message << endl;
        log.close();
    }

    // Count how many patients are booked in a given slot
    int countPatientsInSlot(const string& slot) {
        int count = 0;
        AppointmentNode* curr = slotMap[slot];
        while (curr) {
            count++;
            curr = curr->next;
        }
        return count;
    }

    // Add a patient appointment (either emergency or normal)
    void addAppointment() {
        Patient patient;
        cout << "\n--- Add Appointment ---\n";
        cout << "Enter patient name: ";
        getline(cin, patient.name);
        cout << "Enter age: ";
        cin >> patient.age;
        cin.ignore();
        cout << "Enter patient ID: ";
        getline(cin, patient.id);
        cout << "Enter severity (0 = normal, >0 = emergency): ";
        cin >> patient.severity;
        cin.ignore();

        if (patient.severity > 0) {
            // Emergency patients are added to emergency queue
            emergencyQueue.push(patient);
            logAction("Emergency patient " + patient.name + " added to queue.");
            cout << "Emergency patient added to priority queue.\n";
        } else {
            // Show slot availability for normal patients
            cout << "Available slots (max " << maxPatientsPerSlot << " patients per slot):\n";
            for (size_t i = 0; i < slots.size(); ++i) {
                int count = countPatientsInSlot(slots[i]);
                cout << i + 1 << ". " << slots[i] << " (" << count << "/" << maxPatientsPerSlot << " booked)\n";
            }

            int slotChoice;
            cout << "Choose slot (1-" << slots.size() << "): ";
            cin >> slotChoice;
            cin.ignore();

            if (slotChoice < 1 || slotChoice > slots.size()) {
                cout << "Invalid slot choice.\n";
                return;
            }

            string slot = slots[slotChoice - 1];
            int count = countPatientsInSlot(slot);

            if (count >= maxPatientsPerSlot) {
                cout << "Selected slot is full.\n";
                return;
            }

            // Add to linked list for that slot
            AppointmentNode* newNode = new AppointmentNode(patient);
            newNode->next = slotMap[slot];
            slotMap[slot] = newNode;
            idToSlotMap[patient.id] = slot;

            logAction("Scheduled patient " + patient.name + " at slot " + slot);
            cout << "Appointment scheduled at " << slot << ".\n";
        }
    }

    // Assign emergency patient to next available slot
    void assignEmergency() {
        cout << "\n--- Assign Emergency Patient ---\n";
        if (emergencyQueue.empty()) {
            cout << "No emergency patients in queue.\n";
            return;
        }

        for (string slot : slots) {
            int count = countPatientsInSlot(slot);
            if (count < maxPatientsPerSlot) {
                Patient p = emergencyQueue.top();
                emergencyQueue.pop();
                AppointmentNode* newNode = new AppointmentNode(p);
                newNode->next = slotMap[slot];
                slotMap[slot] = newNode;
                idToSlotMap[p.id] = slot;

                logAction("Emergency patient " + p.name + " assigned to slot " + slot);
                cout << "Emergency patient " << p.name << " assigned to slot " << slot << ".\n";
                return;
            }
        }

        cout << "No slots available to assign emergency patient.\n";
    }

    // Cancel an appointment using patient ID
    void cancelAppointment() {
        cout << "\n--- Cancel Appointment ---\n";
        string patientId;
        cout << "Enter patient ID to cancel: ";
        getline(cin, patientId);

        if (idToSlotMap.find(patientId) == idToSlotMap.end()) {
            cout << "Patient not found.\n";
            return;
        }

        string slot = idToSlotMap[patientId];
        AppointmentNode* curr = slotMap[slot];
        AppointmentNode* prev = nullptr;

        while (curr) {
            if (curr->patient.id == patientId) {
                if (prev)
                    prev->next = curr->next;
                else
                    slotMap[slot] = curr->next;

                delete curr;
                idToSlotMap.erase(patientId);

                logAction("Cancelled appointment for patient ID " + patientId);
                cout << "Appointment cancelled.\n";
                return;
            }
            prev = curr;
            curr = curr->next;
        }
    }

    // Search for a patient by ID
    void searchAppointment() {
        cout << "\n--- Search Appointment ---\n";
        string patientId;
        cout << "Enter patient ID: ";
        getline(cin, patientId);

        if (idToSlotMap.find(patientId) == idToSlotMap.end()) {
            cout << "Patient not found.\n";
            return;
        }

        string slot = idToSlotMap[patientId];
        AppointmentNode* curr = slotMap[slot];

        while (curr) {
            if (curr->patient.id == patientId) {
                cout << "Name: " << curr->patient.name
                     << ", Age: " << curr->patient.age
                     << ", Slot: " << slot;
                if (curr->patient.severity > 0) cout << " (Emergency)";
                cout << "\n";
                return;
            }
            curr = curr->next;
        }
    }

    // View all appointments, slot-wise
    void viewAppointments() {
        cout << "\n--- All Appointments ---\n";
        for (string slot : slots) {
            cout << "Slot: " << slot << "\n";
            AppointmentNode* curr = slotMap[slot];
            int count = 1;
            while (curr) {
                cout << "  " << count++ << ". Name: " << curr->patient.name;
                if (curr->patient.severity > 0) cout << " (Emergency)";
                cout << ", ID: " << curr->patient.id
                     << ", Age: " << curr->patient.age
                     << ", Severity: " << curr->patient.severity << "\n";
                curr = curr->next;
            }
        }
    }

    // View all patients above a certain age
    void viewPatientsAboveAge(int ageThreshold) {
        cout << "\n--- Patients Above Age " << ageThreshold << " ---\n";
        for (const auto& slot : slots) {
            AppointmentNode* curr = slotMap[slot];
            while (curr) {
                if (curr->patient.age > ageThreshold) {
                    cout << "Name: " << curr->patient.name
                         << ", ID: " << curr->patient.id
                         << ", Age: " << curr->patient.age
                         << ", Slot: " << slot;
                    if (curr->patient.severity > 0) cout << " (Emergency)";
                    cout << "\n";
                }
                curr = curr->next;
            }
        }
    }

    // Menu-driven interface
    void menu() {
        int choice;
        do {
            cout << "\n--- Hospital Appointment System ---\n";
            cout << "1. Add Appointment\n";
            cout << "2. Assign Emergency Patient\n";
            cout << "3. Cancel Appointment\n";
            cout << "4. Search Appointment\n";
            cout << "5. View All Appointments\n";
            cout << "6. View Patients Above Age\n";
            cout << "0. Exit\n";
            cout << "Enter your choice: ";
            cin >> choice;
            cin.ignore();

            if (choice == 1) addAppointment();
            else if (choice == 2) assignEmergency();
            else if (choice == 3) cancelAppointment();
            else if (choice == 4) searchAppointment();
            else if (choice == 5) viewAppointments();
            else if (choice == 6) {
                int ageLimit;
                cout << "Enter age threshold: ";
                cin >> ageLimit;
                cin.ignore();
                viewPatientsAboveAge(ageLimit);
            }
            else if (choice == 0) cout << "Exiting system...\n";
            else cout << "Invalid choice.\n";

        } while (choice != 0);
    }
};

// Main function to start the program
int main() {
    HospitalAppointmentSystem system;
    system.menu();
    return 0;
}

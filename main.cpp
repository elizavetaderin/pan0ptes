#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <limits>
#include <algorithm>

using namespace std;

struct Incident {
    string date;
    string platform;
    string username;
    string description;
};

const string FILE_NAME = "incidents.txt";

// Yeni olay kaydetme
void addIncident() {
    Incident incident;

    cout << "\n--- NEW INCIDENT ---\n";

    cout << "Date: ";
    getline(cin, incident.date);

    cout << "Platform: ";
    getline(cin, incident.platform);

    cout << "Username / Nickname: ";
    getline(cin, incident.username);

    cout << "Description: ";
    getline(cin, incident.description);

    ofstream file(FILE_NAME, ios::app);

    if (!file.is_open()) {
        cout << "Error: Could not save the incident.\n";
        return;
    }

    file << quoted(incident.date)
         << quoted(incident.platform)
         << quoted(incident.username)
         << quoted(incident.description) << '\n';

    file.close();

    cout << "\nIncident saved successfully!\n";
}

// Kayıtları listeleme
void listIncidents() {
    ifstream file(FILE_NAME);

    if (!file.is_open()) {
        cout << "\nNo incident records found.\n";
        return;
    }

    Incident incident;
    int count = 0;

    cout << "\n--- SAVED INCIDENTS ---\n";

    while (file >> quoted(incident.date)
                >> quoted(incident.platform)
                >> quoted(incident.username)
                >> quoted(incident.description)) {

        count++;

        cout << "\nIncident #" << count << '\n';
        cout << "Date: " << incident.date << '\n';
        cout << "Platform: " << incident.platform << '\n';
        cout << "Username: " << incident.username << '\n';
        cout << "Description: " << incident.description << '\n';
        cout << "-------------------------\n";
    }

    if (count == 0) {
        cout << "No records available.\n";
    }

    file.close();
}

// Kullanıcı adına göre arama
void searchIncident() {
    ifstream file(FILE_NAME);

    if (!file.is_open()) {
        cout << "\nNo records available.\n";
        return;
    }

    string searchName;
    Incident incident;
    bool found = false;

    cout << "\nEnter username to search: ";
    getline(cin, searchName);

    while (file >> quoted(incident.date)
                >> quoted(incident.platform)
                >> quoted(incident.username)
                >> quoted(incident.description)) {

        if (incident.username.find(searchName) != string::npos) {
            found = true;

            cout << "\n--- MATCH FOUND ---\n";
            cout << "Date: " << incident.date << '\n';
            cout << "Platform: " << incident.platform << '\n';
            cout << "Username: " << incident.username << '\n';
            cout << "Description: " << incident.description << '\n';
        }
    }

    if (!found) {
        cout << "\nNo matching records found.\n";
    }

    file.close();
}

// Basit URL kontrolü
void checkURL() {
    string url;
    bool suspicious = false;

    cout << "\nEnter URL to analyze: ";
    getline(cin, url);

    cout << "\n--- URL ANALYSIS ---\n";

    if (url.find("https://") != 0) {
        cout << "[WARNING] URL does not start with HTTPS.\n";
        suspicious = true;
    }

    if (url.find("@") != string::npos) {
        cout << "[WARNING] URL contains an @ symbol.\n";
        suspicious = true;
    }

    if (url.find("xn--") != string::npos) {
        cout << "[WARNING] URL contains an internationalized domain pattern.\n";
        suspicious = true;
    }

    string lowerURL = url;

    transform(lowerURL.begin(), lowerURL.end(),
              lowerURL.begin(), ::tolower);

    string keywords[] = {
        "login", "verify", "password", "account"
    };

    for (const string& keyword : keywords) {
        if (lowerURL.find(keyword) != string::npos) {
            cout << "[NOTICE] URL contains the keyword: "
                 << keyword << '\n';
            suspicious = true;
        }
    }

    if (!suspicious) {
        cout << "No basic warning signs detected.\n";
    }

    cout << "\nNote: This is a basic heuristic check, not a security guarantee.\n";
}

// Başarısız giriş sayısını değerlendirme
void analyzeLoginAttempts() {
    int attempts;

    cout << "\nEnter the number of failed login attempts: ";

    if (!(cin >> attempts) || attempts < 0) {
        cout << "Invalid number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "\n--- LOGIN ATTEMPT ANALYSIS ---\n";

    if (attempts >= 5) {
        cout << "[WARNING] High number of failed login attempts.\n";
    }
    else if (attempts >= 1) {
        cout << "[NOTICE] Failed login attempts detected.\n";
    }
    else {
        cout << "No failed login attempts reported.\n";
    }

    cout << "Analysis is based only on the number you entered.\n";
}

// Ana menü
int main() {
    int choice;

    cout << "====================================\n";
    cout << "          PANOPTES SYSTEM           \n";
    cout << "   Cybersecurity Incident Tracker   \n";
    cout << "====================================\n";

    do {
        cout << "\n1. Add New Incident\n";
        cout << "2. View All Incidents\n";
        cout << "3. Search Incident\n";
        cout << "4. Analyze Suspicious URL\n";
        cout << "5. Analyze Failed Login Attempts\n";
        cout << "0. Exit\n";

        cout << "\nSelect an option: ";

        if (!(cin >> choice)) {
            cout << "\nInvalid input. Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1:
                addIncident();
                break;

            case 2:
                listIncidents();
                break;

            case 3:
                searchIncident();
                break;

            case 4:
                checkURL();
                break;

            case 5:
                analyzeLoginAttempts();
                break;

            case 0:
                cout << "\nPANOPTES is shutting down. Goodbye!\n";
                break;

            default:
                cout << "\nInvalid choice. Try again.\n";
        }

    } while (choice != 0);

    return 0;
}

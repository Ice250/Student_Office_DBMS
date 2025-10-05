#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
#include <tuple>
#include <limits>
#include <ctime>
#include <mysql/mysql.h> // Include MySQL C API header
#include "DBManager.h"


// --- NEW QT HEADERS ---
#include <QApplication>
#include <QWidget>
#include <QObject>      // Added for QObject inheritance
#include <QMessageBox>  // Added for better error display
// ----------------------

using namespace std;

// ====================================================
// DATABASE CONFIGURATION (MUST BE EDITED FOR YOUR SYSTEM)
// ====================================================
#define HOST "localhost"
#define USER "root"
#define PASS "Rajal_mysql!"
#define DB "bvp_student_office"

#include <QApplication>
#include <QWidget>
#include <QDebug>
#include "DBManager.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    qDebug() << "Starting StudentOffice...";

    DBManager db;
    if (!db.initialize()) {
        qDebug() << "Database connection failed!";
        return -1;
    } else {
        qDebug() << "Database connected successfully!";
    }

    QWidget window;
    window.setWindowTitle("College Office System");
    window.resize(400, 300);
    window.show();

    return app.exec();
}

// Forward declarations
class DBManager;
class Student;
class Admin;

// Utility function to get current date/time string for records
string getCurrentDateTime() {
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    // Format: YYYY-MM-DD HH:MM:SS
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tstruct);
    return buf;
}

// Escape input to prevent SQL injection
string escapeString(MYSQL* conn, const string& str) {
    if (str.empty()) return "";
    // Allocate space for the escaped string (max 2*length + 1)
    char* escaped = new char[str.length() * 2 + 1];
    unsigned long escaped_len = mysql_real_escape_string(conn, escaped, str.c_str(), str.length());
    string result(escaped, escaped_len);
    delete[] escaped;
    return result;
}

// ====================================================
// Student Class: Data model and methods
// ====================================================
class Student {
public:
    string studentID, name, department, contact, feeStatus, academicRecord, password;
    string dateOfBirth, address, parentContact;
    int year;

    // These functions still contain the console output logic, 
    // but in the Qt app, they would be used to fetch data for display widgets.
    void viewProfile() { /* ... console output logic ... */ }
    void viewMarksheet(DBManager& db) { /* ... console output logic ... */ }
    void viewFeeReceipts(DBManager& db) { /* ... console output logic ... */ }
    void viewExamStatus(DBManager& db) { /* ... console output logic ... */ }
    void viewMiscRecords(DBManager& db) { /* ... console output logic ... */ }
};

// ====================================================
// Admin Class: Administrative functions
// ====================================================
class Admin {
public:
    string adminID, name;

    bool login(DBManager& db, const string& id, const string& pass) {
        string escapedID = escapeString(db.getConnection(), id);
        string escapedPass = escapeString(db.getConnection(), pass);
        
        string query = "SELECT AdminID, Name FROM Admins WHERE AdminID = '" + escapedID + "' AND Password = '" + escapedPass + "'";
        
        MYSQL_RES* res = db.executeQuery(query);
        if (!res) return false;

        MYSQL_ROW row = mysql_fetch_row(res);
        bool success = false;

        if (row) {
            adminID = row[0];
            name = row[1];
            success = true;
        }
        
        mysql_free_result(res);
        return success;
    }

    bool studentExists(DBManager& db, const string& studentID) { /* ... logic ... */ return false; }
    void addStudent(DBManager& db) { /* ... console input logic ... */ }
    void updateFeeStatus(DBManager& db, const string& studentID) { /* ... console input logic ... */ }
    void addMarks(DBManager& db, const string& studentID) { /* ... console input logic ... */ }
    void addFeeReceipt(DBManager& db, const string& studentID) { /* ... console input logic ... */ }
    void addExamStatus(DBManager& db, const string& studentID) { /* ... console input logic ... */ }
    void addMiscRecord(DBManager& db, const string& studentID) { /* ... console input logic ... */ }
    void viewAllStudents(DBManager& db) { /* ... console output logic ... */ }
};

// ====================================================
// Authentication Function (Placeholder - logic moves to Qt Widget)
// ====================================================
// This function will be replaced by a custom Qt class (e.g., LoginWidget)
bool authenticate(DBManager& db, Student& currentStudent, Admin& admin) {
    // In the Qt application, a widget will handle input and call Admin::login or query Students table directly.
    return false; 
}

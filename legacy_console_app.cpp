// #include <iostream>
// #include <string>
// #include <vector>
// #include <iomanip>
// #include <sstream>
// #include <tuple>
// #include <limits>
// #include <ctime>
// #include <mysql/mysql.h> // Include MySQL C API header

// using namespace std;

// // ====================================================
// // DATABASE CONFIGURATION (MUST BE EDITED FOR YOUR SYSTEM)
// // ====================================================
// #define HOST "localhost"
// #define USER "root"
// #define PASS "Rajal_mysql!" // <-- CHANGE THIS TO YOUR MySQL ROOT PASSWORD!
// #define DB "bvp_student_office"

// // Forward declarations
// class DBManager;
// class Student;
// class Admin;

// // Utility function to get current date/time string for records
// string getCurrentDateTime() {
//     time_t now = time(0);
//     struct tm tstruct;
//     char buf[80];
//     tstruct = *localtime(&now);
//     // Format: YYYY-MM-DD HH:MM:SS
//     strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tstruct);
//     return buf;
// }

// // Escape input to prevent SQL injection
// string escapeString(MYSQL* conn, const string& str) {
//     if (str.empty()) return "";
//     // Allocate space for the escaped string (max 2*length + 1)
//     char* escaped = new char[str.length() * 2 + 1];
//     unsigned long escaped_len = mysql_real_escape_string(conn, escaped, str.c_str(), str.length());
//     string result(escaped, escaped_len);
//     delete[] escaped;
//     return result;
// }

// // ====================================================
// // DBManager Class: Handles all database connection and queries
// // ====================================================
// class DBManager {
// private:
//     MYSQL* conn;

// public:
//     DBManager() : conn(nullptr) {}

//     // Initialize connection to MySQL
//     bool initialize() {
//         conn = mysql_init(0);
//         if (!conn) {
//             cerr << "MySQL Initialization failed." << endl;
//             return false;
//         }

//         // Connect to the database
//         conn = mysql_real_connect(conn, HOST, USER, PASS, DB, 3306, NULL, 0);

//         if (conn) {
//             cout << "Database connection successful." << endl;
//             return true;
//         } else {
//             cerr << "Connection failed: " << mysql_error(conn) << endl;
//             mysql_close(conn);
//             conn = nullptr;
//             return false;
//         }
//     }

//     // Executes a query and returns the result set
//     MYSQL_RES* executeQuery(const string& query) {
//         if (!conn) {
//             cerr << "Error: Not connected to database." << endl;
//             return nullptr;
//         }
        
//         if (mysql_query(conn, query.c_str())) {
//             cerr << "Query failed: " << mysql_error(conn) << endl;
//             cerr << "Query was: " << query << endl;
//             return nullptr;
//         }
        
//         return mysql_store_result(conn);
//     }
    
//     // Executes a non-SELECT query (INSERT, UPDATE, DELETE)
//     bool executeUpdate(const string& query) {
//         if (!conn) {
//             cerr << "Error: Not connected to database." << endl;
//             return false;
//         }
//         if (mysql_query(conn, query.c_str())) {
//             cerr << "Update failed: " << mysql_error(conn) << endl;
//             cerr << "Query was: " << query << endl;
//             return false;
//         }
//         return true;
//     }

//     // Get the underlying MySQL connection pointer (needed for escapeString)
//     MYSQL* getConnection() {
//         return conn;
//     }

//     // Close connection
//     ~DBManager() {
//         if (conn) {
//             mysql_close(conn);
//             cout << "Database connection closed." << endl;
//         }
//     }
// };

// // ====================================================
// // Student Class: Data model and view methods for a student
// // ====================================================
// class Student {
// public:
//     string studentID, name, department, contact, feeStatus, academicRecord, password;
//     string dateOfBirth, address, parentContact;
//     int year;

//     // View Profile (now includes personal data)
//     void viewProfile() {
//         cout << "\n==============================================" << endl;
//         cout << "            STUDENT PROFILE" << endl;
//         cout << "==============================================" << endl;
//         cout << setw(20) << left << "ID:" << studentID << endl;
//         cout << setw(20) << left << "Name:" << name << endl;
//         cout << setw(20) << left << "Department:" << department << endl;
//         cout << setw(20) << left << "Year:" << year << endl;
//         cout << setw(20) << left << "Contact:" << contact << endl;
//         cout << setw(20) << left << "Fee Status:" << feeStatus << endl;
//         cout << "\n--- Personal Data ---" << endl;
//         cout << setw(20) << left << "Date of Birth:" << (dateOfBirth.empty() ? "N/A" : dateOfBirth) << endl;
//         cout << setw(20) << left << "Parent Contact:" << (parentContact.empty() ? "N/A" : parentContact) << endl;
//         cout << setw(20) << left << "Address:" << (address.empty() ? "N/A" : address) << endl;
//         cout << "\nAcademic Record:\n" << academicRecord << endl;
//         cout << "==============================================" << endl;
//     }

//     // View Marksheet
//     void viewMarksheet(DBManager& db) {
//         string query = "SELECT Subject, Marks, Grade FROM Marksheets WHERE StudentID = '" + escapeString(db.getConnection(), studentID) + "'";
//         MYSQL_RES* res = db.executeQuery(query);
//         if (!res) return;

//         cout << "\n==============================================" << endl;
//         cout << "             STUDENT MARK SHEET" << endl;
//         cout << "==============================================" << endl;
//         cout << setw(20) << left << "SUBJECT" << setw(10) << left << "MARKS" << setw(10) << left << "GRADE" << endl;
//         cout << string(40, '-') << endl;

//         MYSQL_ROW row;
//         bool found = false;
//         while ((row = mysql_fetch_row(res))) {
//             cout << setw(20) << left << row[0] << setw(10) << left << row[1] << setw(10) << left << row[2] << endl;
//             found = true;
//         }
//         if (!found) cout << "No marksheet records found." << endl;
        
//         mysql_free_result(res);
//         cout << "==============================================" << endl;
//     }

//     // View Fee Receipts (updated to include FeeType)
//     void viewFeeReceipts(DBManager& db) {
//         string query = "SELECT ReceiptID, FeeType, Amount, PaidOn, Status FROM FeeReceipts WHERE StudentID = '" + escapeString(db.getConnection(), studentID) + "'";
//         MYSQL_RES* res = db.executeQuery(query);
//         if (!res) return;

//         cout << "\n==========================================================================" << endl;
//         cout << "                          STUDENT FEE RECEIPTS" << endl;
//         cout << "==========================================================================" << endl;
//         cout << setw(10) << left << "ID" << setw(20) << left << "TYPE" << setw(10) << left << "AMOUNT" << setw(15) << left << "PAID ON" << setw(10) << left << "STATUS" << endl;
//         cout << string(75, '-') << endl;

//         MYSQL_ROW row;
//         bool found = false;
//         while ((row = mysql_fetch_row(res))) {
//             cout << setw(10) << left << row[0] << setw(20) << left << row[1] << setw(10) << left << row[2] << setw(15) << left << row[3] << setw(10) << left << row[4] << endl;
//             found = true;
//         }
//         if (!found) cout << "No fee receipts found." << endl;

//         mysql_free_result(res);
//         cout << "==========================================================================" << endl;
//     }

//     // NEW: View Exam Status
//     void viewExamStatus(DBManager& db) {
//         string query = "SELECT Semester, GPA, ResultStatus, DateReleased FROM ExamStatus WHERE StudentID = '" + escapeString(db.getConnection(), studentID) + "' ORDER BY DateReleased DESC";
//         MYSQL_RES* res = db.executeQuery(query);
//         if (!res) return;

//         cout << "\n==========================================================" << endl;
//         cout << "                 STUDENT EXAM STATUS" << endl;
//         cout << "==========================================================" << endl;
//         cout << setw(15) << left << "SEMESTER" << setw(8) << left << "GPA" << setw(15) << left << "STATUS" << setw(15) << left << "RELEASE DATE" << endl;
//         cout << string(53, '-') << endl;

//         MYSQL_ROW row;
//         bool found = false;
//         while ((row = mysql_fetch_row(res))) {
//             cout << setw(15) << left << row[0] << setw(8) << left << row[1] << setw(15) << left << row[2] << setw(15) << left << row[3] << endl;
//             found = true;
//         }
//         if (!found) cout << "No exam status records found." << endl;

//         mysql_free_result(res);
//         cout << "==========================================================" << endl;
//     }
    
//     // NEW: View Miscellaneous Records
//     void viewMiscRecords(DBManager& db) {
//         string query = "SELECT RecordType, Details, RecordedBy, RecordedOn FROM MiscellaneousRecords WHERE StudentID = '" + escapeString(db.getConnection(), studentID) + "' ORDER BY RecordedOn DESC";
//         MYSQL_RES* res = db.executeQuery(query);
//         if (!res) return;

//         cout << "\n=======================================================================" << endl;
//         cout << "                        MISCELLANEOUS RECORDS" << endl;
//         cout << "=======================================================================" << endl;
        
//         MYSQL_ROW row;
//         bool found = false;
//         while ((row = mysql_fetch_row(res))) {
//             cout << "Type: " << row[0] << " | By: " << row[2] << " | On: " << row[3] << endl;
//             cout << "Details: " << row[1] << endl;
//             cout << string(70, '-') << endl;
//             found = true;
//         }
//         if (!found) cout << "No miscellaneous records found." << endl;

//         mysql_free_result(res);
//         cout << "=======================================================================" << endl;
//     }
// };

// // ====================================================
// // Admin Class: Administrative functions
// // ====================================================
// class Admin {
// public:
//     string adminID, name;

//     // Admin login utility function
//     bool login(DBManager& db, const string& id, const string& pass) {
//         string escapedID = escapeString(db.getConnection(), id);
//         string escapedPass = escapeString(db.getConnection(), pass);
        
//         string query = "SELECT AdminID, Name FROM Admins WHERE AdminID = '" + escapedID + "' AND Password = '" + escapedPass + "'";
        
//         MYSQL_RES* res = db.executeQuery(query);
//         if (!res) return false;

//         MYSQL_ROW row = mysql_fetch_row(res);
//         bool success = false;

//         if (row) {
//             adminID = row[0];
//             name = row[1];
//             success = true;
//         }
        
//         mysql_free_result(res);
//         return success;
//     }

//     // Helper to check if a student ID exists
//     bool studentExists(DBManager& db, const string& studentID) {
//         string escapedID = escapeString(db.getConnection(), studentID);
//         string query = "SELECT StudentID FROM Students WHERE StudentID = '" + escapedID + "'";
//         MYSQL_RES* res = db.executeQuery(query);
//         bool exists = false;
//         if (res && mysql_num_rows(res) > 0) {
//             exists = true;
//         }
//         if(res) mysql_free_result(res);
//         return exists;
//     }

//     // Add a new student record
//     void addStudent(DBManager& db) {
//         // Collect mandatory data
//         string id, name, dept, contact, pass;
//         int year;
//         cout << "\n--- Add New Student ---" << endl;
//         cout << "Enter Student ID: "; cin >> id;
//         cout << "Enter Name: "; cin.ignore(); getline(cin, name);
//         cout << "Enter Department: "; getline(cin, dept);
//         cout << "Enter Year (1-4): "; cin >> year;
//         cin.ignore(numeric_limits<streamsize>::max(), '\n');
//         cout << "Enter Contact: "; getline(cin, contact);
//         cout << "Enter Temporary Password: "; getline(cin, pass);

//         if (studentExists(db, id)) {
//             cout << "Error: Student ID already exists." << endl;
//             return;
//         }

//         // Collect new optional personal data
//         string dob, address, parentContact;
//         cout << "Enter Date of Birth (YYYY-MM-DD, or leave blank): "; getline(cin, dob);
//         cout << "Enter Address (or leave blank): "; getline(cin, address);
//         cout << "Enter Parent Contact (or leave blank): "; getline(cin, parentContact);

//         // Escape and format fields
//         string s_id = escapeString(db.getConnection(), id);
//         string s_name = escapeString(db.getConnection(), name);
//         string s_dept = escapeString(db.getConnection(), dept);
//         string s_contact = escapeString(db.getConnection(), contact);
//         string s_pass = escapeString(db.getConnection(), pass);
//         string s_dob = dob.empty() ? "NULL" : "'" + escapeString(db.getConnection(), dob) + "'";
//         string s_address = address.empty() ? "NULL" : "'" + escapeString(db.getConnection(), address) + "'";
//         string s_parentContact = parentContact.empty() ? "NULL" : "'" + escapeString(db.getConnection(), parentContact) + "'";

//         stringstream ss;
//         ss << "INSERT INTO Students (StudentID, Name, Department, Year, Contact, FeeStatus, Password, DateOfBirth, Address, ParentContact) VALUES ('"
//            << s_id << "', '" << s_name << "', '" << s_dept << "', " << year << ", '" << s_contact << "', 'Pending', '" 
//            << s_pass << "', " << s_dob << ", " << s_address << ", " << s_parentContact << ")";

//         if (db.executeUpdate(ss.str())) {
//             cout << "Student " << name << " added successfully." << endl;
//         } else {
//             cout << "Failed to add student." << endl;
//         }
//     }

//     // Update existing student's fee status
//     void updateFeeStatus(DBManager& db, const string& studentID) {
//         if (!studentExists(db, studentID)) {
//             cout << "Error: Student ID not found." << endl;
//             return;
//         }

//         string status;
//         cout << "Enter new Fee Status (Paid, Pending, Overdue): "; cin >> status;
//         string escapedStatus = escapeString(db.getConnection(), status);
//         string escapedID = escapeString(db.getConnection(), studentID);

//         string query = "UPDATE Students SET FeeStatus = '" + escapedStatus + "' WHERE StudentID = '" + escapedID + "'";
        
//         if (db.executeUpdate(query)) {
//             cout << "Fee status for student " << studentID << " updated to " << status << "." << endl;
//         } else {
//             cout << "Failed to update fee status." << endl;
//         }
//     }

//     // Add marks for a student
//     void addMarks(DBManager& db, const string& studentID) {
//         if (!studentExists(db, studentID)) {
//             cout << "Error: Student ID not found." << endl;
//             return;
//         }
        
//         string subject, grade;
//         int marks;
//         cout << "Enter Subject Name: "; cin.ignore(); getline(cin, subject);
//         cout << "Enter Marks: "; cin >> marks;
//         cout << "Enter Grade: "; cin >> grade;
//         cin.ignore(numeric_limits<streamsize>::max(), '\n');

//         string s_subject = escapeString(db.getConnection(), subject);
//         string s_grade = escapeString(db.getConnection(), grade);
//         string s_id = escapeString(db.getConnection(), studentID);

//         stringstream ss;
//         // Use REPLACE INTO to update if the subject/student pair already exists
//         ss << "REPLACE INTO Marksheets (StudentID, Subject, Marks, Grade) VALUES ('"
//            << s_id << "', '" << s_subject << "', " << marks << ", '" << s_grade << "')";

//         if (db.executeUpdate(ss.str())) {
//             cout << "Marks for " << subject << " added/updated successfully." << endl;
//         } else {
//             cout << "Failed to add/update marks." << endl;
//         }
//     }
    
//     // NEW: Add a Fee Receipt (includes FeeType)
//     void addFeeReceipt(DBManager& db, const string& studentID) {
//         if (!studentExists(db, studentID)) {
//             cout << "Error: Student ID not found." << endl;
//             return;
//         }

//         string receiptID, feeType, paidOn, details;
//         double amount;
        
//         cout << "\n--- Add New Fee Receipt ---" << endl;
//         cout << "Enter Receipt ID: "; cin >> receiptID;
//         cout << "Enter Fee Type (e.g., Tuition, Exam, Hostel): "; cin.ignore(); getline(cin, feeType);
//         cout << "Enter Amount: "; cin >> amount;
//         cin.ignore(numeric_limits<streamsize>::max(), '\n');
//         cout << "Enter Date Paid (YYYY-MM-DD): "; getline(cin, paidOn);
//         cout << "Enter Transaction Details: "; getline(cin, details);

//         string s_receiptID = escapeString(db.getConnection(), receiptID);
//         string s_feeType = escapeString(db.getConnection(), feeType);
//         string s_paidOn = escapeString(db.getConnection(), paidOn);
//         string s_details = escapeString(db.getConnection(), details);
//         string s_id = escapeString(db.getConnection(), studentID);

//         stringstream ss;
//         ss << "INSERT INTO FeeReceipts (ReceiptID, StudentID, FeeType, Amount, PaidOn, TransactionDetails, Status) VALUES ('"
//            << s_receiptID << "', '" << s_id << "', '" << s_feeType << "', " << fixed << setprecision(2) << amount << ", '"
//            << s_paidOn << "', '" << s_details << "', 'Paid')";

//         if (db.executeUpdate(ss.str())) {
//             cout << "Fee receipt " << receiptID << " recorded successfully." << endl;
//             // Optionally update overall student fee status
//             db.executeUpdate("UPDATE Students SET FeeStatus = 'Paid' WHERE StudentID = '" + s_id + "'");
//         } else {
//             cout << "Failed to add fee receipt. (ID might be duplicate)" << endl;
//         }
//     }

//     // NEW: Add Exam Status record
//     void addExamStatus(DBManager& db, const string& studentID) {
//         if (!studentExists(db, studentID)) {
//             cout << "Error: Student ID not found." << endl;
//             return;
//         }

//         string semester, resultStatus, dateReleased;
//         double gpa;
        
//         cout << "\n--- Add Exam Status ---" << endl;
//         cout << "Enter Semester (e.g., Sem 4): "; cin.ignore(); getline(cin, semester);
//         cout << "Enter GPA (0.00 - 10.00): "; cin >> gpa;
//         cin.ignore(numeric_limits<streamsize>::max(), '\n');
//         cout << "Enter Result Status (Pass, Fail, ATKT, Distinction): "; getline(cin, resultStatus);
//         cout << "Enter Date Released (YYYY-MM-DD, or leave blank for today): "; getline(cin, dateReleased);
//         if (dateReleased.empty()) {
//             // Placeholder: use current date (Note: date formatting needs care in real implementation)
//             // For simplicity, we'll let the user input this, or rely on a default.
//         }
        
//         string s_semester = escapeString(db.getConnection(), semester);
//         string s_resultStatus = escapeString(db.getConnection(), resultStatus);
//         string s_dateReleased = dateReleased.empty() ? getCurrentDateTime().substr(0, 10) : escapeString(db.getConnection(), dateReleased);
//         string s_id = escapeString(db.getConnection(), studentID);

//         stringstream ss;
//         // Check for duplicates before inserting
//         ss << "INSERT INTO ExamStatus (StudentID, Semester, GPA, ResultStatus, DateReleased) VALUES ('"
//            << s_id << "', '" << s_semester << "', " << fixed << setprecision(2) << gpa << ", '"
//            << s_resultStatus << "', '" << s_dateReleased << "')";

//         if (db.executeUpdate(ss.str())) {
//             cout << "Exam status for " << semester << " added successfully." << endl;
//         } else {
//             cout << "Failed to add exam status. (Semester might be a duplicate)" << endl;
//         }
//     }

//     // NEW: Add Miscellaneous Record
//     void addMiscRecord(DBManager& db, const string& studentID) {
//         if (!studentExists(db, studentID)) {
//             cout << "Error: Student ID not found." << endl;
//             return;
//         }

//         string recordType, details;
        
//         cout << "\n--- Add Miscellaneous Record ---" << endl;
//         cout << "Enter Record Type (Warning, Attendance, Leave, General): "; cin.ignore(); getline(cin, recordType);
//         cout << "Enter Details: "; getline(cin, details);
        
//         string s_recordType = escapeString(db.getConnection(), recordType);
//         string s_details = escapeString(db.getConnection(), details);
//         string s_id = escapeString(db.getConnection(), studentID);
//         string s_recordedOn = getCurrentDateTime();

//         stringstream ss;
//         ss << "INSERT INTO MiscellaneousRecords (StudentID, RecordType, Details, RecordedBy, RecordedOn) VALUES ('"
//            << s_id << "', '" << s_recordType << "', '" << s_details << "', '" << escapeString(db.getConnection(), adminID) << "', '"
//            << s_recordedOn << "')";

//         if (db.executeUpdate(ss.str())) {
//             cout << "Miscellaneous record added successfully." << endl;
//         } else {
//             cout << "Failed to add miscellaneous record." << endl;
//         }
//     }

//     // View all students
//     void viewAllStudents(DBManager& db) {
//         string query = "SELECT StudentID, Name, Department, Year, FeeStatus FROM Students";
//         MYSQL_RES* res = db.executeQuery(query);
//         if (!res) return;

//         cout << "\n==========================================================================" << endl;
//         cout << "                          ALL STUDENT RECORDS" << endl;
//         cout << "==========================================================================" << endl;
//         cout << setw(10) << left << "ID" << setw(30) << left << "NAME" << setw(15) << left << "DEPT" << setw(5) << left << "YR" << setw(15) << left << "FEE STATUS" << endl;
//         cout << string(75, '-') << endl;

//         MYSQL_ROW row;
//         bool found = false;
//         while ((row = mysql_fetch_row(res))) {
//             cout << setw(10) << left << row[0] << setw(30) << left << row[1] << setw(15) << left << row[2] << setw(5) << left << row[3] << setw(15) << left << row[4] << endl;
//             found = true;
//         }
//         if (!found) cout << "No students found." << endl;

//         mysql_free_result(res);
//         cout << "==========================================================================" << endl;
//     }
// };

// // ====================================================
// // Authentication Function
// // ====================================================
// bool authenticate(DBManager& db, Student& currentStudent, Admin& admin) {
//     string id, pass;
//     cout << "Enter ID: "; cin >> id;
//     cout << "Enter Password: "; cin >> pass;
//     cin.ignore(numeric_limits<streamsize>::max(), '\n');

//     // 1. Try Admin Login
//     if (admin.login(db, id, pass)) {
//         cout << "Admin login successful! Welcome, " << admin.name << "." << endl;
//         return true; // Returns true for Admin
//     }

//     // 2. Try Student Login
//     string escapedID = escapeString(db.getConnection(), id);
//     string escapedPass = escapeString(db.getConnection(), pass);
    
//     string query = "SELECT StudentID, Name, Department, Year, Contact, AcademicRecord, FeeStatus, DateOfBirth, Address, ParentContact FROM Students WHERE StudentID = '" + escapedID + "' AND Password = '" + escapedPass + "'";
    
//     MYSQL_RES* res = db.executeQuery(query);
//     if (!res) return false;

//     MYSQL_ROW row = mysql_fetch_row(res);
//     if (row) {
//         // Load student data
//         currentStudent.studentID = row[0];
//         currentStudent.name = row[1];
//         currentStudent.department = row[2];
//         currentStudent.year = (row[3] ? stoi(row[3]) : 0);
//         currentStudent.contact = row[4];
//         currentStudent.academicRecord = row[5];
//         currentStudent.feeStatus = row[6];
//         currentStudent.dateOfBirth = (row[7] ? row[7] : "");
//         currentStudent.address = (row[8] ? row[8] : "");
//         currentStudent.parentContact = (row[9] ? row[9] : "");
        
//         cout << "Student login successful! Welcome, " << currentStudent.name << "." << endl;
//         mysql_free_result(res);
//         return false; // Returns false for Student
//     }

//     mysql_free_result(res);
//     cout << "Login failed. Invalid ID or Password." << endl;
//     return false; // Returns false for failure
// }

// // ====================================================
// // Main Application Loop
// // ====================================================
// int main() {
//     DBManager db;
//     if (!db.initialize()) {
//         cerr << "Application cannot start without database connection." << endl;
//         return 1;
//     }

//     Student currentStudent;
//     Admin admin;
//     bool loggedIn = false;
//     bool isAdmin = false;

//     while (true) {
//         if (!loggedIn) {
//             cout << "\n=== Welcome to College Office System ===" << endl;
//             cout << "1. Login\n2. Exit\nChoice: ";
//             int choice;
//             if (!(cin >> choice)) {
//                 cin.clear();
//                 cin.ignore(numeric_limits<streamsize>::max(), '\n');
//                 continue;
//             }
//             cin.ignore(numeric_limits<streamsize>::max(), '\n');

//             if (choice == 2) break;

//             if (choice == 1) {
//                 isAdmin = authenticate(db, currentStudent, admin);
//                 if (admin.adminID.empty() && currentStudent.studentID.empty()) {
//                     // Login failed, isAdmin will be false, and IDs will be empty.
//                 } else {
//                     loggedIn = true;
//                 }
//             }
//             continue;
//         }
        
//         // ====================================================
//         // MENU HANDLING
//         // ====================================================
//         if (isAdmin) {
//             // Admin Menu (Updated to include new functions)
//             cout << "\n=== Admin Menu (" << admin.name << ") ===" << endl;
//             cout << "1. View All Students\n2. Add New Student\n3. Update Student Fee Status\n4. Add Student Marks\n5. Record Fee Receipt\n6. Record Exam Status\n7. Record Miscellaneous Note\n8. Logout\nChoice: ";
//             int adminChoice;
//             if (!(cin >> adminChoice)) {
//                 cin.clear();
//                 cin.ignore(numeric_limits<streamsize>::max(), '\n');
//                 continue;
//             }
//             cin.ignore(numeric_limits<streamsize>::max(), '\n');

//             string tempID;
//             switch (adminChoice) {
//                 case 1: admin.viewAllStudents(db); break;
//                 case 2: admin.addStudent(db); break;
//                 case 3: 
//                     cout << "Enter Student ID to update fee status: "; getline(cin, tempID); 
//                     admin.updateFeeStatus(db, tempID); 
//                     break;
//                 case 4: 
//                     cout << "Enter Student ID to add marks: "; getline(cin, tempID); 
//                     admin.addMarks(db, tempID); 
//                     break;
//                 case 5: 
//                     cout << "Enter Student ID to record fee receipt: "; getline(cin, tempID); 
//                     admin.addFeeReceipt(db, tempID); 
//                     break;
//                 case 6: 
//                     cout << "Enter Student ID to record exam status: "; getline(cin, tempID); 
//                     admin.addExamStatus(db, tempID); 
//                     break;
//                 case 7: 
//                     cout << "Enter Student ID to add miscellaneous record: "; getline(cin, tempID); 
//                     admin.addMiscRecord(db, tempID); 
//                     break;
//                 case 8: 
//                     loggedIn = false; 
//                     admin.adminID.clear(); 
//                     admin.name.clear();
//                     cout << "Logged out." << endl; 
//                     break;
//                 default: cout << "Invalid choice." << endl;
//             }
//         } else {
//             // Student Menu (Updated to include new views)
//             cout << "\n=== Student Menu (" << currentStudent.name << ") ===" << endl;
//             cout << "1. View Profile\n2. View Marksheet\n3. View Fee Receipts\n4. View Exam Status\n5. View Miscellaneous Records\n6. Logout\nChoice: ";
//             int studentChoice;
//             if (!(cin >> studentChoice)) {
//                 cin.clear();
//                 cin.ignore(numeric_limits<streamsize>::max(), '\n');
//                 continue;
//             }
//             cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
//             switch (studentChoice) {
//                 case 1: currentStudent.viewProfile(); break;
//                 case 2: currentStudent.viewMarksheet(db); break;
//                 case 3: currentStudent.viewFeeReceipts(db); break;
//                 case 4: currentStudent.viewExamStatus(db); break; // NEW
//                 case 5: currentStudent.viewMiscRecords(db); break; // NEW
//                 case 6: 
//                     loggedIn = false; 
//                     currentStudent.studentID.clear(); // Clear student session data
//                     cout << "Logged out." << endl; 
//                     break;
//                 default: cout << "Invalid choice." << endl;
//             }
//         }
//     }

//     return 0;
// }

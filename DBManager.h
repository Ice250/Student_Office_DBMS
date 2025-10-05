#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QObject>
#include <mysql/mysql.h>
#include <string>
#include <QMessageBox>
#include <iostream>

// ====================================================
// DATABASE CONFIGURATION 
// IMPORTANT: These credentials must match your MySQL setup.
// ====================================================
#define HOST "localhost"
#define USER "root"
#define PASS "Rajal_mysql!"
#define DB   "bvp_student_office"

/**
 * @brief Manages the MySQL database connection and transactions.
 * Inherits QObject for integration with the Qt framework.
 */
class DBManager : public QObject {
    Q_OBJECT
private:
    MYSQL* conn;

public:
    explicit DBManager(QObject* parent = nullptr);
    ~DBManager();

    /**
     * @brief Initializes and connects to the database. Displays a graphical error 
     * message if connection fails.
     * @return true if connection is successful, false otherwise.
     */
    bool initialize();
    
    /**
     * @brief Executes a SELECT query and returns the result set.
     */
    MYSQL_RES* executeQuery(const std::string& query);
    
    /**
     * @brief Executes a non-SELECT query (INSERT, UPDATE, DELETE).
     */
    bool executeUpdate(const std::string& query);
    
    /**
     * @brief Getter for the raw MySQL connection object (used for string escaping).
     */
    MYSQL* getConnection() const; 
};

#endif // DBMANAGER_H
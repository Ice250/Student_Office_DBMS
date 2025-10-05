#include "DBManager.h"
#include <QMessageBox>
#include <iostream>
#include <QDebug> // Include QDebug for qDebug()

// Implementation based on DBManager.h and user-provided code
DBManager::DBManager(QObject* parent) : QObject(parent), conn(nullptr) {
    // Attempt to initialize the MySQL client library environment
    if (mysql_library_init(0, nullptr, nullptr)) {
        QMessageBox::critical(nullptr, "Database Error", "Could not initialize MySQL client library.");
    }
}

bool DBManager::initialize() {
    conn = mysql_init(0);
    if (!conn) {
        QMessageBox::critical(nullptr, "Database Error", "MySQL Initialization failed.");
        return false;
    }

    // --- FIX FOR SSL ERROR ---
    // The previous flag (CLIENT_SSL_DONT_VERIFY_SERVER_CERT) is not supported 
    // by your MySQL Connector version. 
    // We use the universally supported CLIENT_SSL flag combined with the NOT operator 
    // to explicitly disable the SSL connection attempt.
    unsigned long clientFlags = 0; // Start with no flags
    clientFlags |= CLIENT_MULTI_STATEMENTS; // Useful flag for multiple statements
    
    // Explicitly exclude the SSL flag to force a non-encrypted connection
    clientFlags &= ~CLIENT_SSL; 

    conn = mysql_real_connect(conn, HOST, USER, PASS, DB, 3306, NULL, clientFlags);
    
    if (conn) {
        // Since we are using a GUI, use qDebug() for console output
        qDebug() << "Database connection successful.";
        return true;
    } else {
        std::string errorMsg = "Connection failed: " + std::string(mysql_error(conn));
        QMessageBox::critical(nullptr, "Database Error", errorMsg.c_str());
        mysql_close(conn);
        conn = nullptr;
        return false;
    }
}

MYSQL_RES* DBManager::executeQuery(const std::string& query) {
    if (!conn) return nullptr;
    if (mysql_query(conn, query.c_str())) {
        std::cerr << "Query failed: " << mysql_error(conn) << std::endl;
        return nullptr;
    }
    return mysql_store_result(conn);
}

bool DBManager::executeUpdate(const std::string& query) {
    if (!conn) return false;
    if (mysql_query(conn, query.c_str())) {
        std::cerr << "Update failed: " << mysql_error(conn) << std::endl;
        return false;
    }
    return true;
}

MYSQL* DBManager::getConnection() const { return conn; }

DBManager::~DBManager() {
    if (conn) mysql_close(conn);
    // Cleanup MySQL client library environment
    mysql_library_end();
}

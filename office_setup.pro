# ============================================
# Project: Student Office DBMS (Qt + MySQL)
# ============================================

# 1. Project type
TEMPLATE = app

# 2. Source files
SOURCES += main.cpp \
           DBManager.cpp

# 3. Headers
HEADERS += DBManager.h

# 4. Qt Modules required
QT += core widgets

# 5. Standard C++ version and config
CONFIG += c++17 console

# 6. MySQL Linking (CRITICAL - 64-bit for Qt mingw_64)
INCLUDEPATH += "C:/PROGRA~2/MySQL/mysql-connector-c-6.1.11-winx64/include"
LIBS += -L"C:/PROGRA~2/MySQL/mysql-connector-c-6.1.11-winx64/lib64" -lmysqlclient -lws2_32

# 7. Suppress warnings (for unused params in main.cpp)
QMAKE_CXXFLAGS += -Wno-unused-parameter

# 8. Optional: Output name and directory
TARGET = StudentOffice
DESTDIR = release  # Outputs to ./release/StudentOffice.exe

# 9. Windows-specific (ensure Winsock for networking)
win32 {
    LIBS += -lws2_32
}
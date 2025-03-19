#include <iostream>
#include <mysql.h>
#include <mysqld_error.h>
#include <Windows.h>
#include <string>

using namespace std;

// Database Class for CRUD Operations
class Database {
private:
    MYSQL* conn;
    MYSQL_ROW row;
    MYSQL_RES* res;

public:
    Database() : conn(nullptr), row(nullptr), res(nullptr) {} // Initialize to nullptr

    bool connect() {
        conn = mysql_real_connect(mysql_init(0), "localhost", "root", "Hamza@123", "hotel_db", 3306, NULL, 0);
        if (conn) {
            cout << "Database Connected Successfully!" << endl;
            return true;
        }
        else {
            cout << "Database Connection Failed: " << mysql_error(conn) << endl;
            return false;
        }
    }

    void executeQuery(string query) {
        if (mysql_query(conn, query.c_str()) == 0) {
            cout << "Query Executed Successfully!" << endl;
        }
        else {
            cout << "Query Failed: " << mysql_error(conn) << endl;
        }
    }

    void fetchData(string query) {
        if (mysql_query(conn, query.c_str()) == 0) {
            res = mysql_store_result(conn);
            if (!res) {
                cout << "Error storing result: " << mysql_error(conn) << endl;
                return;
            }

            while ((row = mysql_fetch_row(res))) {
                for (int i = 0; i < mysql_num_fields(res); i++) {
                    cout << (row[i] ? row[i] : "NULL") << " ";
                }
                cout << endl;
            }
            mysql_free_result(res); // Free memory
        }
        else {
            cout << "SELECT Query Failed: " << mysql_error(conn) << endl;
        }
    }

    ~Database() {
        if (conn) {
            mysql_close(conn);
            cout << "Database Connection Closed." << endl;
        }
    }
};

// Customer Class
class Customer {
public:
    string customerID;
    string firstName, lastName, address, mobile, gender, email, nationality, idType;

    void getCustomerDetails() {
        cout << "Enter Customer ID: "; cin >> customerID;
        cout << "Enter First Name: "; cin >> firstName;
        cout << "Enter Last Name: "; cin >> lastName;
        cout << "Enter Address: "; cin >> address;
        cout << "Enter Mobile: "; cin >> mobile;
        cout << "Enter Gender: "; cin >> gender;
        cout << "Enter Email: "; cin >> email;
        cout << "Enter Nationality: "; cin >> nationality;
        cout << "Enter ID Type: "; cin >> idType;
    }

    void insertData(Database& db) {
        string query = "INSERT INTO customers (customerID, firstName, lastName, address, mobile, gender, email, nationality, idType) VALUES ('" + customerID + "', '" + firstName + "', '" + lastName + "', '" + address + "', '" + mobile + "', '" + gender + "', '" + email + "', '" + nationality + "', '" + idType + "')";
        db.executeQuery(query);
    }
};

// RoomDetails Class
class RoomDetails {
public:
    long roomNo;
    string roomType;
    double price;
    bool availability;

    RoomDetails(long r, string t, double p, bool a) : roomNo(r), roomType(t), price(p), availability(a) {}

    void displayRoomDetails() {
        cout << "Room No: " << roomNo << endl;
        cout << "Room Type: " << roomType << endl;
        cout << "Price: $" << price << endl;
        cout << "Availability: " << (availability ? "Yes" : "No") << endl;
    }
};

// Booking Class
class Booking {
public:
    string customerID;
    string roomNo;
    string checkinDate, checkoutDate;

    void bookRoom(Database& db) {
        cout << "Enter Customer ID: "; cin >> customerID;
        cout << "Enter Room No: "; cin >> roomNo;
        cout << "Enter Check-in Date (YYYY-MM-DD): "; cin >> checkinDate;
        cout << "Enter Check-out Date (YYYY-MM-DD): "; cin >> checkoutDate;

        string query = "INSERT INTO bookings (customerID, roomNo, checkinDate, checkoutDate) VALUES ('" +
            customerID + "', '" + roomNo + "', '" + checkinDate + "', '" + checkoutDate + "')";
        db.executeQuery(query);
    }
};

// Main Function
int main() {
    Database db;
    if (!db.connect()) return 1;

    int choice;
    do {
        cout << "\nHotel Management System\n";
        cout << "1. Add Customer\n";
        cout << "2. Book Room\n";
        cout << "3. View Customers\n";
        cout << "4. View Bookings\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            Customer cust;
            cust.getCustomerDetails();
            cust.insertData(db);
            break;
        }
        case 2: {
            Booking book;
            book.bookRoom(db);
            break;
        }
        case 3:
            db.fetchData("SELECT * FROM customers");
            break;
        case 4:
            db.fetchData("SELECT * FROM bookings");
            break;
        case 5:
            cout << "Exiting program..." << endl;
            break;
        default:
            cout << "Invalid choice! Please try again." << endl;
        }
    } while (choice != 5);

    return 0;
}

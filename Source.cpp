#include <iostream>
#include <mysql.h>
#include <mysqld_error.h>
#include <Windows.h>
#include<string.h>
#include <sstream>

using namespace std;

// Database Class for CRUD Operations
class Database {
private:
	MYSQL* conn;
	MYSQL_ROW row;
	MYSQL_RES* res;

public:
	Database() : conn(nullptr), row(nullptr), res(nullptr) {} // Initialize to nullptr

	MYSQL* getConnection() {
		return conn;
	}

	bool connect() {
		// APNA PASSWORD OR DATABASE IDHR ADD KRNA .
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
			mysql_free_result(res);
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

// Hotel Management
class HotelManagement {
protected:
	string hotelName;
	double hotelID;

public:
	void setHotelName(string name) {
		hotelName = name;
	}
	string getHotelName() {
		return hotelName;
	}

	void setHotelID(double id) {
		hotelID = id;
	}
	double getHotelID() {
		return hotelID;
	}

	void displayHotelDetails() {
		cout << "Hotel Name: " << hotelName << endl;
		cout << "Hotel ID: " << hotelID << endl;
	}
};

// Login System
class Login {
protected:
	string username, password;

public:
	void setCredentials(string uname, string pass) {
		username = uname;
		password = pass;
	}

	void registerUser(Database& db) {
		string query = "INSERT INTO users (username, password) VALUES ('" + username + "', '" + password + "')";
		db.executeQuery(query);
	}

	bool loginUser(Database& db) {
		string query = "SELECT * FROM users WHERE username='" + username + "' AND password='" + password + "'";

		if (mysql_query(db.getConnection(), query.c_str()) == 0) {
			MYSQL_RES* res = mysql_store_result(db.getConnection());
			if (res) {
				MYSQL_ROW row = mysql_fetch_row(res);
				if (row) {
					cout << "Login Successful! Welcome, " << username << "!" << endl;
					mysql_free_result(res);
					return true;
				}
			}
			mysql_free_result(res);
		}

		cout << "Login Failed! Incorrect username or password." << endl;
		return false;
	}


	void forgotPassword() {
		cout << "Password reset link sent!" << endl;
	}
};

// Customer Class
class Customer {
protected:
	string customerID;
	string firstName, lastName, address, mobile, gender, email, nationality, idType;
public:
	Customer(string cID = "", string FN = "", string LN = "", string ADD = "",
		string MOBI = "", string GEN = "", string EML = "",
		string NATION = "", string IDT = "") {
		customerID = cID;
		firstName = FN;
		lastName = LN;
		address = ADD;
		mobile = MOBI;
		gender = GEN;
		email = EML;
		nationality = NATION;
		idType = IDT;
	}

	void inputCustomer() {
		cout << "Enter Customer ID: ";
		cin >> customerID;
		cin.ignore(50, '\n');
		cout << "Enter First Name: ";
		cin >> firstName;
		cout << "Enter Last Name: ";
		cin >> lastName;
		cout << "Enter Address: ";
		cin >> address;
		cin.ignore(100, '\n');
		bool isvalid;
		do {
			cout << "Enter Mobile Number (valid): ";
			cin >> mobile;

			isvalid = true;
			if (mobile.length() != 11) isvalid = false;
			for (int i = 0; i < 11; i++) {
				if (mobile[i] < '0' || mobile[i] > '9') isvalid = false;
			}

		} while (!isvalid);
		cout << "Enter Gender: ";
		cin >> gender;
		cout << "Enter Email: ";
		cin >> email;
		cout << "Enter Nationality: ";
		cin >> nationality;
		cout << "Enter ID Type (Passport/License/etc.): ";
		cin >> idType;
	}


	void insert_customers(Database& db) {
		string query = "INSERT INTO customers (customerID,firstName,lastName,address,mobile,gender,email,nationality,idType) VALUES('"
			+ customerID + "','" + firstName + "','" + lastName + "','" + address + "','" + mobile + "','" + gender + "','" + email + "','" + nationality + "','" + idType + "')";
		db.executeQuery(query);
	}
};

// RoomDetails Class
class RoomDetails :virtual public Customer {
protected:
	string roomNo;
	string roomType;
	double price;
	bool availability;

	const string roomTypeList[3] = { "Deluxe", "Standard", "Suite" };

public:
	RoomDetails(string rNo = "", string rType = "", double p = 0.0, bool avail = true) {
		roomNo = rNo;
		roomType = rType;
		price = p;
		availability = avail;
	}

	void setRoomPrice() {
		if (roomType == "Deluxe") {
			price = 150.0;
		}
		else if (roomType == "Standard") {
			price = 100.0;
		}
		else if (roomType == "Suite") {
			price = 200.0;
		}
		else {
			price = 0.0;
		}
	}


	void inputRoomDetails() {
		const string allowedRooms[8] = { "101", "102", "103", "104", "105", "106", "107", "108" };
		const string roomTypeList[3] = { "Deluxe", "Standard", "Suite" };
		cout << "\nDELUXE ------> 101 AND 102\nSUITE------> 103,104 AND 105\nSTANDARD------> 106,107 AND 108\n";
		bool validRoom = false;
		do {
			cout << "Enter Room Number: ";
			cin >> roomNo;
			for (int i = 0; i < 8; i++) {
				if (allowedRooms[i] == roomNo) {
					validRoom = true;
					break;
				}
			}
			if (!validRoom) {
				cout << "Error: Invalid room number! Choose from 101, 102, 103, 104, 105, 106, 107, 108.\n";
			}

		} while (!validRoom);

		if (roomNo == "101" || roomNo == "102") {
			roomType = "Deluxe";
		}
		else if (roomNo == "103" || roomNo == "104" || roomNo == "105") {
			roomType = "Suite";
		}
		else {
			roomType = "Standard";
		}

		setRoomPrice();

		cout << "Availing (1 for Yes, 0 for No): ";
		while (!(cin >> availability) || (availability != 0 && availability != 1)) {
			cout << "Error: Enter 1 for Yes or 0 for No.\n";
			cin.clear();
			cin.ignore(100, '\n');
		}

		if (availability == 0) {
			cout << "Returning to the main menu...\n";
			return;
		}
		cout << "Price: $" << price << endl;
	}

	int getavail() {
		return availability;
	}


};

class Booking : virtual public Customer, public RoomDetails {
private:
	string checkinDate, checkoutDate;
	long bookingID;

public:
	Booking() {
		bookingID = 0;
	}

	void inputBookingDetails() {
		bookingID = 10000 + rand() % 90000;
		Database db;

		cout << "\n--- Enter Booking Details ---\n";
		cout << "BOOKING_ID: " << bookingID << endl;
		inputCustomer();
		inputRoomDetails();  
		if (availability == 0) return;
		cout << "Enter Check-in Date (YYYY-MM-DD): ";
		cin >> checkinDate;
		cout << "Enter Check-out Date (YYYY-MM-DD): ";
		cin >> checkoutDate;

		cout << endl;
	}
	void insert_booking(Database& db) {
		stringstream book;
		book << bookingID;

		string query = "INSERT INTO bookings(bookingID,customerID,roomNo,checkindate,checkoutdate)VALUES(" + book.str() + ",'" + customerID + "','" + roomNo + "','" + checkinDate + "','" + checkoutDate + "')";
		string detailquery = "INSERT INTO details(bookingID,customerID,roomNo)VALUES(" + book.str() + ",'" + customerID + "','" + roomNo + "')";
		db.executeQuery(query);
		db.executeQuery(detailquery);
	}
};

// Checkout Class
class Checkout {
	double bill;
public:
	void processCheckout(Database& db, string customerID) {
		// Query to get stay details
		string query = "SELECT DATEDIFF(checkoutDate, checkinDate) + 1 AS daysStayed, r.price, b.roomNo "
			"FROM bookings b JOIN rooms r ON b.roomNo = r.roomNo "
			"WHERE b.customerID = '" + customerID + "'";

		if (mysql_query(db.getConnection(), query.c_str()) != 0) {
			cout << "Error executing query: " << mysql_error(db.getConnection()) << endl;
			return;
		}

		MYSQL_RES* res = mysql_store_result(db.getConnection());
		if (!res) {
			cout << "Error fetching bill details: " << mysql_error(db.getConnection()) << endl;
			return;
		}

		int num_rows = mysql_num_rows(res);
		cout << "Rows found: " << num_rows << endl;

		if (num_rows == 0) {
			cout << "No booking found for this customer!" << endl;
			mysql_free_result(res);
			return;
		}

		MYSQL_ROW row = mysql_fetch_row(res);
		int daysStayed = atoi(row[0]);
		double roomPrice = atof(row[1]);
		string roomNo = row[2]; // Store the room number
		mysql_free_result(res);

		bill = daysStayed * roomPrice;

		// Display Checkout Summary
		cout << "--------------------------\n";
		cout << "Checkout Summary\n";
		cout << "Customer ID: " << customerID << endl;
		cout << "Days Stayed: " << daysStayed << endl;
		cout << "Room Price per Night: $" << roomPrice << endl;
		cout << "Total Bill: $" << bill << endl;
		cout << "--------------------------\n";

		// Delete from bookings table
		string deleteBookings = "DELETE FROM bookings WHERE customerID = '" + customerID + "'";
		db.executeQuery(deleteBookings);

		// Delete from customers table
		string deleteCustomer = "DELETE FROM customers WHERE customerID = '" + customerID + "'";
		db.executeQuery(deleteCustomer);

		// Update room availability
		string updateRoom = "UPDATE rooms SET availability = 1 WHERE roomNo = '" + roomNo + "'";
		db.executeQuery(updateRoom);
	}
};


// Room Availability
class Availability {
public:
	void checkRoomAvailability(Database& db) {
		cout << "Available Rooms: " << endl;
		db.fetchData("SELECT * FROM rooms WHERE availability = 1");
	}
};

// View Details
class ViewDetails {
public:
	int choice;
	void showDetails(Database& db) {
		db.fetchData("SELECT * FROM details");
	}
};

int main() {
	Database db;
	if (!db.connect()) return 1;

	int choice;
	do {
		cout << "\nEnter the choice\n1.) Register User\n2.) Login User\n0.) Exit" << endl;
		cout << "\nChoice:";
		cin >> choice;

		switch (choice) {
		case 1: {
			string user_name, password;
			Login user;
			cout << "Enter the User Name: ";
			cin >> user_name;
			cout << "Enter the Password: ";
			cin >> password;

			user.setCredentials(user_name, password);
			user.registerUser(db);
			break;
		}
		case 2: {
			string user_name, password;
			Login user;
			cout << "Enter the User Name: ";
			cin >> user_name;
			cout << "Enter the Password: ";
			cin >> password;

			// Set credentials before attempting login
			user.setCredentials(user_name, password);

			if (user.loginUser(db)) {
				cout << "\t------ WELCOME ------" << endl;

				int CHOICE;
				do {
					cout << "\n1.) Book Room\n2.) Checkout\n3.) Check Room Availability\n4.) View Details\n0.) Logout\n";
					cout << "Enter your choice: ";
					cin >> CHOICE;

					switch (CHOICE) {
					case 1: {

						Booking booking1;
						booking1.inputBookingDetails();
						if (booking1.getavail() == 0) break;
						booking1.insert_customers(db);
						booking1.insert_booking(db);
						break;
					}
					case 2: {
						string customerID;
						cout << "Enter Customer ID to Checkout: ";
						cin >> customerID;
						Checkout checkout;
						checkout.processCheckout(db, customerID);
						break;
					}
					case 3: {
						Availability avail;
						avail.checkRoomAvailability(db);
						break;
					}
					case 4: {
						ViewDetails view;
						view.showDetails(db);
						break;
					}
					case 0:
						cout << "Logging out...\n";
						break;
					default:
						cout << "Invalid choice! Try again.\n";
					}
				} while (CHOICE != 0);
			}
			break;
		}
		case 0:
			cout << "Exiting program...\n";
			break;
		default:
			cout << "Invalid choice! Try again.\n";
		}
	} while (choice != 0);

	return 0;
}

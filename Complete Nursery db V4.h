// PlantCompany.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
//#include "pch.h"     You may or may not need this file check your precompiled headers option under properties
#include "pch.h"
#include <iostream>
#include <mysql.h>
#include <string>
#include <cstring>
#include <stdlib.h> // for clear screen system("CLS");
#include <vector>
using namespace std;
int qstate;
unsigned int accnum = 0;// for customer 
MYSQL* conn;
MYSQL_ROW row;
MYSQL_RES* res;


void identityMessage()
{
	cout << "Welcome to Plant Nursery database" << endl
		<< "*****************************************************" << endl
		<< "************************Log in***********************" << endl
		<< "Enter 1 to login as manager" << endl
		<< "Enter 2 to login as staff" << endl
		<< "Enter 3 to login as gardner" << endl
		<< "Enter 4 to login as customer" << endl
		<< "Enter 5 to change to another database" << endl;
}
void manager()
{
	puts("Starting up");
	//MYSQL* conn_M;
	//conn_M = mysql_init(0);
	MYSQL_ROW row_M;
	MYSQL_RES* res_M;


	long num_results = 0;
	int menu1 = 0;
	int menu2 = 0;
	int menu3 = 0;
	int menu4 = 0;

	int managerID = 0;
	string location_tag;
	string managerPass;

	string sQuery;
	string s_choice;
	int s_choice_update = 16;
	bool add_s = false;
	int staff_count = 0;
	int s_reserve = 15;
	int sl_start = 0;

	string gQuery;
	string g_choice;
	int g_choice_update = 5;
	bool add_g = false;
	int gardener_count = 0;
	int g_reserve = 4;
	int g_commision = 0;

	string rQuery;
	int id = 0;
	string gender;
	string sdate;
	string fname;
	string lname;
	int salary = 2000; //Starting salary
	string bday;
	string phone;
	string password;




	cout << "***************Manager log in***************" << endl;
	cout << "Please enter your ID: ";
	cin >> managerID;	//get ID
	cout << endl;
	cout << "Please enter your password: ";
	cin.ignore();
	getline(cin, managerPass);	//get Password



	string manaQuery = "SELECT e_ID,e_fname,lead_lo_id  FROM employee, leads WHERE e_id = " + to_string(managerID) + " and e_password = md5('" + managerPass + "')";
	const char* qM = manaQuery.c_str();
	qstate = mysql_query(conn, qM);

	if (!qstate)
	{
		res_M = mysql_store_result(conn);
		num_results = mysql_num_rows(res_M);

		if (num_results == 0)
		{
			printf("The ID or password you have entered is not valid please try again\n");
			mysql_free_result(res_M);
			return;
		}
		while (row_M = mysql_fetch_row(res_M))
		{

			cout << "Match found, Welcome to the manager console, " << row_M[1] << endl;
			location_tag = row_M[2];
			mysql_free_result(res_M);
			num_results = 0;
			//cout << row_M[2];
		}
	}
	while (1)
	{
		manaQuery = "SELECT concat(e_fname, ' ', e_lname) as employee_name, e_id FROM employee WHERE e_id in (SELECT distinct gwork_gardener_id as employee FROM employee, leads left join g_work on lead_lo_id = gwork_lo_id WHERE lead_manager_id = " + to_string(managerID) + " union SELECT distinct swork_staff_id FROM leads left join s_work on lead_lo_id = swork_lo_id WHERE lead_manager_id = " + to_string(managerID) + " ) ORDER BY employee_name";
		//cout << manaQuery;
		//select concat(e_fname, ' ', e_lname) as employee_name, e_id
		//	from employee
		//	where
		//	e_id in
		//	(Select distinct gwork_gardener_id as employee
		//		from employee, leads
		//		left join g_work on lead_lo_id = g_work.gwork_lo_id
		//where lead_manager_id = managerID
		//	union
		//	Select distinct swork_staff_id
		//	from leads
		//	left join s_work on lead_lo_id = s_work.swork_lo_id
		//where lead_manager_id = managerID
		//	)
		//	order by
		//	employee_name
		qM = manaQuery.c_str();
		qstate = mysql_query(conn, qM);
		if (!qstate)
		{
			res_M = mysql_store_result(conn);
			num_results = mysql_num_rows(res_M);
			if (num_results == 0)
			{
				cout << "You have no one under you\n" << endl;
			}
			else
			{
				printf("These are your employees at store #%s\n", location_tag);
				while (row_M = mysql_fetch_row(res_M))
				{
					printf("Employee: %s    ID: %s\n", row_M[0], row_M[1]);
				}
			}
			mysql_free_result(res_M);
		}
		else
		{
			cout << "Query failed: " << mysql_error(conn) << endl;
		}
		cout << "Would you like to add, remove, or register some workers?" << endl
			<< "Enter 1 to add" << endl
			<< "Enter 2 to remove" << endl
			<< "Enter 3 to register" << endl
			<< "Enter 4 to go back to Log In" << endl;
		cin >> menu2;
		cin.ignore();
		switch (menu2)
		{
		case 1:
			cout << "Would you like to add gardners or staff?" << endl
				<< "Enter 1 for gardners" << endl
				<< "Enter 2 for staff" << endl
				<< "Enter 3 to go back to main manager console" << endl;
			cin >> menu3;
			cin.ignore();
			switch (menu3)
			{
			case 1:
				gQuery = "select count(gwork_num_per_lo) as gardener_count from g_work where gwork_lo_id = " + location_tag;
				qM = gQuery.c_str();
				qstate = mysql_query(conn, qM);
				if (!qstate)
				{
					res_M = mysql_store_result(conn);
					while (row_M = mysql_fetch_row(res_M))
					{
						gardener_count = stoi(row_M[0]);

					}
					mysql_free_result(res_M);
				}
				else
				{
					cout << "Query failed: " << mysql_error(conn) << endl;
				}
				cout << gardener_count << endl;
				if (gardener_count >= 0 && gardener_count < 4)
				{
					cout << "You can add " << (g_reserve = 4 - gardener_count) << " more gardeners" << endl;
				}
				else
				{
					cout << "You cannot add anymore gardeners, please remove some first" << endl;
					break;
				}
				gQuery = "select concat(e_fname, ' ', e_lname) as employee_name, e_id from employee where e_id not in (select gardener_id from gardener union select staff_id from staff union select manager_id from manager) order by employee_name";
				qM = gQuery.c_str();
				qstate = mysql_query(conn, qM);
				if (!qstate)
				{
					res_M = mysql_store_result(conn);
					num_results = mysql_num_rows(res_M);
					if (num_results == 0)
					{
						cout << "There is no one to add\n" << endl;
						mysql_free_result(res_M);
						break;
					}
					printf("Please Pick an employee by Id from the list below\n");
					while (row_M = mysql_fetch_row(res_M))
					{
						printf("Employee: %s    ID: %s\n", row_M[0], row_M[1]);
					}
					mysql_free_result(res_M);
					cin >> g_choice;
					cin.ignore();
				}
				else
				{
					cout << "Query failed: " << mysql_error(conn) << endl;
				}

				qstate = mysql_query(conn, qM);
				if (!qstate)
				{
					res_M = mysql_store_result(conn);

					while (row_M = mysql_fetch_row(res_M))
					{
						if (g_choice == row_M[1])
						{
							add_g = true;
						}

					}
					mysql_free_result(res_M);
				}
				if (add_g)
				{
					gardener_count++;
					cout << "Adding employee: " << g_choice << " Please give them a starting commission" << endl;
					cin >> g_commision;
					cin.ignore();
					gQuery = "insert into gardener (gardener_id, gardener_bonus) values(" + g_choice + ", " + to_string(g_commision) + ")";
					qM = gQuery.c_str();
					qstate = mysql_query(conn, qM);
					if (!qstate)
					{
						gQuery = "insert into g_work (gwork_gardener_id, gwork_lo_id, gwork_num_per_lo) values(" + g_choice + "," + location_tag + "," + to_string(gardener_count) + ")";
						qM = gQuery.c_str();
						qstate = mysql_query(conn, qM);
						if (!qstate)
						{
							cout << "Gardener added" << endl;
						}
						else
						{
							gQuery = "delete from gardener where gardener_id = " + g_choice + "";
							qM = gQuery.c_str();
							qstate = mysql_query(conn, qM);
							cout << "Query failed: " << mysql_error(conn) << endl;

						}
					}
					else
					{
						cout << "Query failed: " << mysql_error(conn) << endl;
					}
					add_g = false;
				}
				else
				{
					cout << "You selected an employee that does not exist or is assigned elsewhere" << endl;
				}
				break;
			case 2: ////////////////////////////////////////////////////////////////
				sQuery = "select count(swork_num_per_lo) as staff_count from s_work where swork_lo_id = " + location_tag;
				qM = sQuery.c_str();
				qstate = mysql_query(conn, qM);
				if (!qstate)
				{
					res_M = mysql_store_result(conn);
					while (row_M = mysql_fetch_row(res_M))
					{
						staff_count = stoi(row_M[0]);
					}

					mysql_free_result(res_M);
				}
				else
				{
					cout << "Query failed: " << mysql_error(conn) << endl;
				}

				if (staff_count >= 0 && staff_count < 15)
				{
					cout << "You can add " << (s_reserve = 15 - staff_count) << " more staff memebers" << endl;
				}
				else
				{
					cout << "You cannot add anymore staff, please remove some first" << endl;
					break;
				}
				sQuery = "select concat(e_fname, ' ', e_lname) as employee_name, e_id from employee where e_id not in (select gardener_id from gardener union select staff_id from staff union select manager_id from manager) order by employee_name";
				qM = sQuery.c_str();
				qstate = mysql_query(conn, qM);
				if (!qstate)
				{
					res_M = mysql_store_result(conn);
					num_results = mysql_num_rows(res_M);
					if (num_results == 0)
					{
						cout << "There is no one to add\n" << endl;
						break;
					}
					printf("Please Pick an employee by Id from the list below\n");
					while (row_M = mysql_fetch_row(res_M))
					{
						printf("Employee: %s    ID: %s\n", row_M[0], row_M[1]);
					}
					mysql_free_result(res_M);
					cin >> s_choice;
					cin.ignore();
				}
				else
				{
					cout << "Query failed: " << mysql_error(conn) << endl;
				}

				qstate = mysql_query(conn, qM);
				if (!qstate)
				{
					res_M = mysql_store_result(conn);

					while (row_M = mysql_fetch_row(res_M))
					{
						if (s_choice == row_M[1])
						{
							add_s = true;
						}
					}
				}
				if (add_s)
				{
					staff_count++;
					cout << "Adding employee: " << g_choice << " Please give them a starting staff level" << endl;
					cin >> sl_start;
					cin.ignore();
					sQuery = "insert into staff (staff_id, staff_level) values(" + s_choice + ", " + to_string(sl_start) + ")";
					qM = sQuery.c_str();
					qstate = mysql_query(conn, qM);
					if (!qstate)
					{
						sQuery = "insert into s_work (swork_staff_id, swork_lo_id, swork_num_per_lo) values(" + s_choice + "," + location_tag + "," + to_string(staff_count) + ")";
						qM = sQuery.c_str();
						qstate = mysql_query(conn, qM);
						if (!qstate)
						{
							cout << "Staff added" << endl;
						}
						else
						{
							sQuery = "delete from staff where staff_id = " + s_choice + "";
							qM = sQuery.c_str();
							qstate = mysql_query(conn, qM);
							cout << "Query failed: " << mysql_error(conn) << endl;

						}
					}
					else
					{
						cout << "Query failed: " << mysql_error(conn) << endl;
					}
					add_s = false;
				}
				else
				{
					cout << "You selected an employee that does not exist or is assigned elsewhere" << endl;
				}
				break;
			case 3:
				break;
			default:
				cout << "invalid menu option" << endl;
				continue;
			}
			menu3 = 0;
			break; //leave it
		case 2:
			cout << "Would you like to remove gardners or staff?" << endl
				<< "Enter 1 for gardners" << endl
				<< "Enter 2 for staff" << endl
				<< "Enter 3 to go back to main manager console" << endl;
			cin >> menu4;
			cin.ignore();
			switch (menu4)
			{
			case 1:
				gQuery = "select concat(e_fname, ' ', e_lname) as employee_name, e_id , gwork_num_per_lo from employee, g_work where e_id = gwork_gardener_id and gwork_lo_id = " + location_tag;
				qM = gQuery.c_str();
				qstate = mysql_query(conn, qM);
				if (!qstate)
				{
					res_M = mysql_store_result(conn);
					num_results = mysql_num_rows(res_M);
					if (num_results == 0)
					{
						cout << "There is no one to remove\n" << endl;
						mysql_free_result(res_M);
						break;
					}

					printf("Please Pick an employee by Id from the list below to remove\n");
					while (row_M = mysql_fetch_row(res_M))
					{
						printf("Employee: %s    ID: %s\n", row_M[0], row_M[1]);
					}
					mysql_free_result(res_M);
					cin >> g_choice;
					cin.ignore();
				}
				else
				{
					cout << "Query failed: " << mysql_error(conn) << endl;
				}
				qstate = mysql_query(conn, qM);
				if (!qstate)
				{
					res_M = mysql_store_result(conn);

					while (row_M = mysql_fetch_row(res_M))
					{
						if (g_choice == row_M[1])
						{
							add_g = true;
							g_choice_update = stoi(row_M[2]); //keep their place in the store line 
						}
					}
					mysql_free_result(res_M);
				}
				if (add_g)
				{
					gQuery = "delete from gardener where gardener_id = " + g_choice;
					qM = gQuery.c_str();
					qstate = mysql_query(conn, qM);
					if (!qstate)
					{
						cout << "Gardener removed" << endl;
					}
					else
					{
						cout << "Query failed: " << mysql_error(conn) << endl;
					}
					add_g = false;
					gQuery = "update g_work set gwork_num_per_lo = gwork_num_per_lo - 1 where gwork_num_per_lo > " + to_string(g_choice_update);
					qM = gQuery.c_str();
					qstate = mysql_query(conn, qM);
					/*if (!qstate)
					{
						cout << "Size rebalanced" << endl;
					}
					else
					{
						cout << "Query failed: " << mysql_error(conn) << endl;
					}*/
					g_choice_update = 5;
				}
				else if (!add_g)
				{
					cout << "That employee does not exist or does not work for you" << endl;
				}
				break;
			case 2:
				sQuery = "select concat(e_fname, ' ', e_lname) as employee_name, e_id, swork_num_per_lo from employee, s_work where e_id = swork_staff_id and swork_lo_id = " + location_tag;
				qM = sQuery.c_str();
				qstate = mysql_query(conn, qM);
				if (!qstate)
				{
					res_M = mysql_store_result(conn);
					num_results = mysql_num_rows(res_M);
					if (num_results == 0)
					{
						cout << "There is no one to remove\n" << endl;
						mysql_free_result(res_M);
						break;
					}
					printf("Please Pick an employee by Id from the list below to remove\n");
					while (row_M = mysql_fetch_row(res_M))
					{
						printf("Employee: %s    ID: %s\n", row_M[0], row_M[1]);
					}
					mysql_free_result(res_M);
					cin >> s_choice;
					cin.ignore();
				}
				else
				{
					cout << "Query failed: " << mysql_error(conn) << endl;
				}
				qstate = mysql_query(conn, qM);
				if (!qstate)
				{
					res_M = mysql_store_result(conn);

					while (row_M = mysql_fetch_row(res_M))
					{
						if (s_choice == row_M[1])
						{
							add_s = true;
							s_choice_update = stoi(row_M[2]);
						}
					}
					mysql_free_result(res_M);
				}
				if (add_s)
				{
					sQuery = "delete from staff where staff_id = " + s_choice;
					qM = sQuery.c_str();
					qstate = mysql_query(conn, qM);
					if (!qstate)
					{
						cout << "Employee removed" << endl;
					}
					else
					{
						cout << "Query failed: " << mysql_error(conn) << endl;
					}
					add_s = false;
					sQuery = "update s_work set swork_num_per_lo = swork_num_per_lo - 1 where swork_num_per_lo > " + to_string(s_choice_update);
					qM = sQuery.c_str();
					qstate = mysql_query(conn, qM);
					/*if (!qstate)
					{
						cout << "Size rebalanced" << endl;
					}
					else
					{
						cout << "Query failed: " << mysql_error(conn) << endl;
					}*/
					s_choice_update = 16;
				}
				else if (!add_s)
				{
					cout << "That employee does not exist or does not work for you" << endl;
				}
				break;
			case 3:
				break;
			default:
				continue;
			}
			menu4 = 0;
			break;
		case 3:
			rQuery = "select max(e_id) from employee";
			qM = rQuery.c_str();
			qstate = mysql_query(conn, qM);
			if (!qstate)
			{
				res_M = mysql_store_result(conn);
				while (row_M = mysql_fetch_row(res_M))
				{
					if (row_M[0] == NULL)     //You have no employees
					{
						int id = 1;
					}
					else
					{
						id = (stoi(row_M[0]) + 1);
					}
				}
			}
			else
			{
				cout << "Query failed: " << mysql_error(conn) << endl;
			}

			cout << "What gender is the employee? male - female - other" << endl;
			cin >> gender;
			cin.ignore();
			cout << "Please enter the date in this Format: YYYY-MM-DD" << endl;
			cin >> sdate;
			cin.ignore();
			cout << "What is their first name?" << endl;
			cin >> fname;
			cin.ignore();
			cout << "What is their last name?" << endl;
			cin >> lname;
			cin.ignore();
			cout << "Please enter their date of birth in this format: YYYY-MM-DD" << endl;
			cin >> bday;
			cin.ignore();
			cout << "Please enter their phone number" << endl;
			cin >> phone;
			cin.ignore();
			cout << "Please enter their password" << endl;
			cin >> password;
			cin.ignore();

			rQuery = "Insert into employee (e_id,e_gender,e_startdate,e_fname,e_lname,e_salary,e_birthday,e_phone,e_password) value ( " + to_string(id) + ",'" + gender + "','" + sdate + "','" + fname + "','" + lname + "'," + to_string(salary) + ",'" + bday + "','" + phone + "',md5('" + password + "'))";
			qM = rQuery.c_str();
			qstate = mysql_query(conn, qM);
			if (!qstate)
			{
				cout << "Employee Registered" << endl;
			}
			else
			{
				cout << "Query failed: " << mysql_error(conn) << endl;
			}
			break;
		case 4:
			return;
		default:
			cout << "invalid menu option" << endl;
			continue;
		}
		menu2 = 0;
	}
}
void staff()
{
	string staffID;
	string staffPass;
	int menu_d0 = 0;
	int menu_d1 = 0;
	string staffQuery;
	MYSQL_RES* staffRes;
	MYSQL_ROW staffRow;



	//login
	cout << "***************Staff log in***************" << endl;
	cout << "Please enter your ID: ";
	getline(cin, staffID);	//get ID	
	cout << endl;
	cout << "Please enter your password: ";
	getline(cin, staffPass);	//get Password
	cout << endl;

	//password verification
	staffQuery = "SELECT e_id FROM employee, staff WHERE e_id = staff_id AND e_password = md5(\'" + staffPass + "\') AND e_id = " + staffID;
	if (!mysql_query(conn, staffQuery.c_str()))
	{
		bool authenticated = false;
		staffRes = mysql_store_result(conn);
		staffRow = mysql_fetch_row(staffRes);
		if (staffRow == nullptr)
		{
			cout << "Incorrect Password or ID" << endl;
			return;
		}
	}
	else
	{
		cout << "Query failed: " << mysql_error(conn) << endl;
		cout << "Unable to authenticate..." << endl;
		return;
	}

	//staff console
	cout << "Welcome to the staff console" << endl
		<< "*****************************************************" << endl;
	while (1)
	{
		cout << "Enter 1 to view available plants" << endl
			<< "Enter 2 to process orders" << endl
			<< "Enter 3 to exit the console" << endl;

		cin >> menu_d0;
		cin.ignore();
		switch (menu_d0)
		{
			//Searching plants
		case 1:
			//Building query
			staffQuery = "SELECT p_name, p_stock FROM plant";
			while (1)
			{
				cout << "Which plants?" << endl
					<< "Enter 1 to search trees" << endl
					<< "Enter 2 to search fruits" << endl
					<< "Enter 3 to search vegetables" << endl
					<< "Enter 4 to search flowers" << endl
					<< "Enter 5 to search all plants" << endl;
				cin >> menu_d1;
				cin.ignore();
				switch (menu_d1)
				{
				case 1:
					staffQuery += " WHERE (plant.p_id IN (SELECT tr_id FROM tree))";
					break;
				case 2:
					staffQuery += " WHERE (plant.p_id IN (SELECT fr_id FROM fruit))";
					break;
				case 3:
					staffQuery += " WHERE (plant.p_id IN (SELECT v_id FROM vegetable))";
					break;
				case 4:
					staffQuery += " WHERE (plant.p_id IN (SELECT fl_id FROM flower))";
					break;
				case 5:
					//dont need to filter with WHERE
					break;
				default:
					cout << "invalid menu option" << endl;
					continue;
				}
				menu_d1 = 0;
				staffQuery += " ORDER BY p_name";

				//executing query
				if (!mysql_query(conn, staffQuery.c_str()))
				{
					cout << "Query success!" << endl;
					cout << "Name\tStock" << endl;
					staffRes = mysql_store_result(conn);
					while (staffRow = mysql_fetch_row(staffRes))
					{
						cout << staffRow[0] << "\t" << staffRow[1] << endl;
					}
				}
				else
				{
					cout << "Query failed: " << mysql_error(conn) << endl;
				}

				cout << "enter any value to continue ...";
				cin.get();
				cin.ignore();
				break;
			}

			break;
		case 2:
			//finding and displaying all unprocessed orders
			if (!mysql_query(conn, "SELECT * FROM orders WHERE(orders.order_estimate_deliver_date IS NULL)"))
			{
				cout << "Query success!" << endl;
				staffRes = mysql_store_result(conn);
				while (staffRow = mysql_fetch_row(staffRes))
				{
					cout << staffRow[0] << endl;
				}
			}
			else
			{
				cout << "Query failed: " << mysql_error(conn) << endl;
				break;
			}

			menu_d1 = 0;
			for (bool loop = true; loop; )
			{
				string hold;
				cout << "Enter 1 to process an order" << endl
					<< "Enter 2 to quit" << endl;
				cin >> menu_d1;
				cin.ignore();
				switch (menu_d1)
				{
				case 1:
					cout << "Enter the order to process in the format \"Order_ID,YYYY-MM-DD\"" << endl;
					getline(cin, hold);
					staffQuery = "UPDATE orders SET order_estimate_deliver_date = '" + hold.substr(hold.find_first_of(",") + 1);
					staffQuery += "' WHERE order_id = '";
					staffQuery += hold.substr(0, hold.find_first_of(","));
					staffQuery += "'";
					if (!mysql_query(conn, staffQuery.c_str()))
					{
						cout << "Query success!" << endl;
					}
					else
					{
						cout << "Query failed: " << mysql_error(conn) << endl;
					}
					break;
				case 2:
					loop = false;
					break;
				default:
					cout << "invalid menu option" << endl;
					continue;
				}
				menu_d1 = 0;
			}

			break;
		case 3:
			return;
		default:
			cout << "invalid menu option" << endl;
		}
		menu_d0 = 0;

	}
}
void gardener()
{
	bool passwordGood = 0;
	int gardenerOption;
	int modifyOption;
	int gardenerSearchMethod;
	int pId_int;
	string pId_string;
	char yesNO;
	string modifyString;
	string plantID;
	int pType;
	string plantName;
	string gardenerID;
	string gardenerPass;
	string orderID;
	string shipper;
	string estDeliver;
	string pId, pName, pStock, pLifespan, pStartdate, pNumOfGard, pSold; // for insertion
	string tr_grade; //Tree catagory
	string fr_season; //Fruit
	string v_season;
	string v_color; // Vegetable
	string v_color2 = "NULL"; // vegtable
	string v_strength;
	string fl_season;//flower
	string fl_color;
	string fl_color2;
	string fl_duration;


startGardener:
	cout << "\n*********************************************" << endl;
	cout << "***************Gardener log in***************" << endl;
	cout << "Please enter your ID: ";
	cin >> gardenerID;
	cout << "Please enter your password: ";
	cin >> gardenerPass;
	cout << endl;
	string query = "SELECT e_id FROM employee join gardener on e_id = gardener_id where e_password = md5('" + gardenerPass + "')"; //pass is 12345 for 9957389
	const char* q = query.c_str();
	qstate = mysql_query(conn, q);
	if (!qstate)
	{
		res = mysql_store_result(conn);
		while (row = mysql_fetch_row(res))
		{

			if (gardenerID == (string)row[0])
			{
				cout << "Authentication confirmed!\n\n";
				passwordGood = 1;
			}


		}
		if (!passwordGood)
		{
			cout << "Authentication Failed!\n\n";
			goto startGardener;
		}
	gardenerMenuSelect:
		cout << "\n************************************************" << endl;
		cout << "***************Gardener Main Menu***************\n" << endl;
		cout << "What would you like to do?" << endl;
		cout << "Enter the corresponding number: " << endl;
		cout << "1. Add Plant" << endl;
		cout << "2. Update Plant Information" << endl;
		cout << "3. Complete Customer Order(s)" << endl;
		cout << "4. Check work location" << endl;
		cout << "5. Sign out" << endl;

		cin >> gardenerOption;

		switch (gardenerOption)
		{
		case 1:
		gardenerAdd:
			query = "SELECT p_id FROM plant order by p_id";
			q = query.c_str();
			qstate = mysql_query(conn, q);
			if (!qstate)
			{
				res = mysql_store_result(conn);
				while (row = mysql_fetch_row(res))
				{
					pId_string = (string)row[0];

				}
			}
			pId_int = stoi(pId_string) + 1;
			pId = to_string(pId_int);
			cout << "\n********************************************" << endl;
			cout << "***************Add New Plant ***************\n" << endl;

			cout << "Please enter the following information about the plant." << endl;
			cout << "ID: '" + pId + "'" << endl; //4

			cout << "Enter plant type\n1. Tree\n2. Fruit\n3. Vegetable\n4. Flower\n ";
			cin >> pType;
			cout << "Name: "; //2
			cin >> pName;
			cout << "Stock: "; //3
			cin >> pStock;
			cout << "Lifespan: ";
			cin >> pLifespan; //1
			cout << "Startdate (YYYY-MM-DD): "; //5
			cin >> pStartdate;
			cout << "Number of Gardeners: "; //6
			cin >> pNumOfGard;
			cout << "Amount of Sold: "; //7
			cin >> pSold;
			if (pType == 1) //Tree
			{
				cout << "Tree Grade: ";
				cin >> tr_grade;

				cout << endl;

				query = "INSERT INTO PLANT VALUES ('" + pLifespan + "', '" + pName + "', '" + pStock + "', '" + pId + "', '" + pStartdate + "', '" + pNumOfGard + "', '" + pSold + "');";
				q = query.c_str();
				qstate = mysql_query(conn, q);

				query = "INSERT INTO TREE VALUES ('" + tr_grade + "', '" + pId + "');";
				q = query.c_str();
				qstate = mysql_query(conn, q);

				cout << "Do you want to add another plant? (Y/N): ";
				cin >> yesNO;
				if (tolower(yesNO) == 'y')
					goto gardenerAdd;
				goto gardenerMenuSelect;
			}
			else if (pType == 2) //Fruit
			{
				cout << "Season: ";
				cin >> fr_season;

				cout << endl;

				query = "INSERT INTO PLANT VALUES ('" + pLifespan + "', '" + pName + "', '" + pStock + "', '" + pId + "', '" + pStartdate + "', '" + pNumOfGard + "', '" + pSold + "');";
				q = query.c_str();
				qstate = mysql_query(conn, q);

				query = "INSERT INTO FRUIT VALUES ('" + fr_season + "', '" + pId + "');";
				q = query.c_str();
				qstate = mysql_query(conn, q);

				cout << "Do you want to add another plant? (Y/N): ";
				cin >> yesNO;
				if (tolower(yesNO) == 'y')
					goto gardenerAdd;
				goto gardenerMenuSelect;

			}
			else if (pType == 3) //Vegetable
			{
				cout << "Season: ";
				cin >> v_season;
				cout << "Color: ";
				cin >> v_color;
				cout << "Strength (1-5): ";
				cin >> v_strength;

				cout << endl;

				query = "INSERT INTO PLANT VALUES ('" + pLifespan + "', '" + pName + "', '" + pStock + "', '" + pId + "', '" + pStartdate + "', '" + pNumOfGard + "', '" + pSold + "');";
				q = query.c_str();
				qstate = mysql_query(conn, q);

				query = "INSERT INTO VEGETABLE VALUES ('" + v_season + "', '" + v_color + "', '" + pId + "', '" + v_strength + "');";
				q = query.c_str();
				qstate = mysql_query(conn, q);

				cout << "Do you want to add another plant? (Y/N): ";
				cin >> yesNO;
				if (tolower(yesNO) == 'y')
					goto gardenerAdd;
				goto gardenerMenuSelect;
			}
			else if (pType == 4) //Flower
			{
				cout << "Season: ";
				cin >> fl_season;
				cout << "Color: ";
				cin >> fl_color;
				cout << "Duration (Days): ";
				cin >> fl_duration;

				cout << endl;

				query = "INSERT INTO PLANT VALUES ('" + pLifespan + "', '" + pName + "', '" + pStock + "', '" + pId + "', '" + pStartdate + "', '" + pNumOfGard + "', '" + pSold + "');";
				q = query.c_str();
				qstate = mysql_query(conn, q);

				query = "INSERT INTO FLOWER VALUES ('" + fl_season + "', '" + fl_color + "', '" + pId + "', " + fl_duration + ");";
				q = query.c_str();
				qstate = mysql_query(conn, q);

				cout << "Do you want to add another plant? (Y/N): ";
				cin >> yesNO;
				if (tolower(yesNO) == 'y')
					goto gardenerAdd;
				goto gardenerMenuSelect;
			}


			break;

		case 2:
		gardenerUpdate:
			cout << "\n******************************************************" << endl;
			cout << "***************Update Plant Information***************\n" << endl;
			cout << "\nIf you want to sort by filter, please enter the following option number: " << endl;
			cout << "1. All\n2. Tree\n3. Fruit\n4. Vegetable\n5. Flower\n";
			cin >> gardenerSearchMethod;

			//Sort by ALL
			if (gardenerSearchMethod == 1)
			{
				cout << "-------------------------------------------------------------------------------------------------------------------------" << endl;
				query = "SELECT * FROM plant";
				q = query.c_str();
				qstate = mysql_query(conn, q);
				if (!qstate)
				{
					res = mysql_store_result(conn);
					cout << "|ID\t| NAME          \t\t\t| STOCK\t| STARTDATE\t| LIFESPAN\t| # OF GARDENERS\t| SOLD\t|" << endl;
					while (row = mysql_fetch_row(res))
					{
						cout << "-------------------------------------------------------------------------------------------------------------------------" << endl;

						cout << "|" << row[3] << "\t| " << row[1] << "          \t\t\t| " << row[2] << "\t|" << row[4] << "\t|" << row[0] << "\t\t|" << row[5] << "\t\t\t|" << row[6] << "\t|" << endl;

					}
				}
				cout << "-------------------------------------------------------------------------------------------------------------------------" << endl;

				cout << "\nPlease enter plant ID:" << endl;
				cin >> plantID;
				cout << "\n-------------------------------------------------------------------------------------------------------------------------" << endl;
				query = "SELECT * FROM PLANT WHERE p_id =" + plantID;
				q = query.c_str();
				qstate = mysql_query(conn, q);
				if (!qstate)
				{
					res = mysql_store_result(conn);
					cout << "|ID\t| NAME          \t\t\t| STOCK\t| STARTDATE\t| LIFESPAN\t| # OF GARDENERS\t| SOLD\t|" << endl;
					while (row = mysql_fetch_row(res))
					{
						cout << "-------------------------------------------------------------------------------------------------------------------------" << endl;

						cout << "|" << row[3] << "\t| " << row[1] << "          \t\t\t| " << row[2] << "\t|" << row[4] << "\t|" << row[0] << "\t\t|" << row[5] << "\t\t\t|" << row[6] << "\t|" << endl;

					}
				}
				cout << "-------------------------------------------------------------------------------------------------------------------------" << endl;
			}
			//Sort by TREE
			else if (gardenerSearchMethod == 2)
			{
				cout << "-----------------------------------------------------------------------------------------------------------------------------------------" << endl;
				query = "SELECT * FROM plant join tree on p_id = tr_id";
				q = query.c_str();
				qstate = mysql_query(conn, q);
				if (!qstate)
				{
					res = mysql_store_result(conn);
					cout << "|ID\t| NAME          \t\t\t| STOCK\t| STARTDATE\t| LIFESPAN\t| # OF GARDENERS\t| SOLD\t| Tr Grade\t|" << endl;
					while (row = mysql_fetch_row(res))
					{
						cout << "-----------------------------------------------------------------------------------------------------------------------------------------" << endl;

						cout << "|" << row[3] << "\t| " << row[1] << "          \t\t\t| " << row[2] << "\t|" << row[4] << "\t|" << row[0] << "\t\t|" << row[5] << "\t\t\t|" << row[6] << "\t|" << row[7] << "\t\t|" << endl;

					}
				}
				cout << "-----------------------------------------------------------------------------------------------------------------------------------------" << endl;


				cout << "\nPlease enter plant ID:" << endl;
				cin >> plantID;
				cout << "-----------------------------------------------------------------------------------------------------------------------------------------" << endl;
				query = "SELECT * FROM plant join tree on p_id =" + plantID;
				q = query.c_str();
				qstate = mysql_query(conn, q);
				if (!qstate)
				{
					res = mysql_store_result(conn);
					cout << "|ID\t| NAME          \t\t\t| STOCK\t| STARTDATE\t| LIFESPAN\t| # OF GARDENERS\t| SOLD\t| Tr Grade\t|" << endl;
					while (row = mysql_fetch_row(res))
					{
						cout << "-----------------------------------------------------------------------------------------------------------------------------------------" << endl;

						cout << "|" << row[3] << "\t| " << row[1] << "          \t\t\t| " << row[2] << "\t|" << row[4] << "\t|" << row[0] << "\t\t|" << row[5] << "\t\t\t|" << row[6] << "\t|" << row[7] << "\t\t|" << endl;

					}
				}
				cout << "-----------------------------------------------------------------------------------------------------------------------------------------" << endl;
			}

			else if (gardenerSearchMethod == 3)
			{
				cout << "-----------------------------------------------------------------------------------------------------------------------------------------" << endl;
				query = "SELECT * FROM plant join fruit on p_id = fr_id";
				q = query.c_str();
				qstate = mysql_query(conn, q);
				if (!qstate)
				{
					res = mysql_store_result(conn);
					cout << "|ID\t| NAME          \t\t\t| STOCK\t| STARTDATE\t| LIFESPAN\t| # OF GARDENERS\t| SOLD\t| FR SEASON\t|" << endl;
					while (row = mysql_fetch_row(res))
					{
						cout << "-----------------------------------------------------------------------------------------------------------------------------------------" << endl;

						cout << "|" << row[3] << "\t| " << row[1] << "          \t\t\t| " << row[2] << "\t|" << row[4] << "\t|" << row[0] << "\t\t|" << row[5] << "\t\t\t|" << row[6] << "\t|" << row[7] << "\t\t|" << endl;

					}
				}
				cout << "-----------------------------------------------------------------------------------------------------------------------------------------" << endl;


				cout << "\nPlease enter plant ID:" << endl;
				cin >> plantID;
				cout << "-----------------------------------------------------------------------------------------------------------------------------------------" << endl;
				query = "SELECT * FROM plant join fruit on p_id = fr_id WHERE p_id =" + plantID + ";";
				q = query.c_str();
				qstate = mysql_query(conn, q);
				if (!qstate)
				{
					res = mysql_store_result(conn);
					cout << "|ID\t| NAME          \t\t\t| STOCK\t| STARTDATE\t| LIFESPAN\t| # OF GARDENERS\t| SOLD\t| FR SEASON\t|" << endl;
					while (row = mysql_fetch_row(res))
					{
						cout << "-----------------------------------------------------------------------------------------------------------------------------------------" << endl;

						cout << "|" << row[3] << "\t| " << row[1] << "          \t\t\t| " << row[2] << "\t|" << row[4] << "\t|" << row[0] << "\t\t|" << row[5] << "\t\t\t|" << row[6] << "\t|" << row[7] << "\t\t|" << endl;

					}
				}
				cout << "-----------------------------------------------------------------------------------------------------------------------------------------" << endl;
			}

			else if (gardenerSearchMethod == 4) // vegetable
			{
				cout << "-------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
				query = "SELECT * FROM plant join vegetable on p_id = v_id";
				q = query.c_str();
				qstate = mysql_query(conn, q);
				if (!qstate)
				{
					res = mysql_store_result(conn);
					cout << "|ID\t| NAME          \t\t\t| STOCK\t| STARTDATE\t| LIFESPAN\t| # OF GARDENERS\t| SOLD\t| V SEASON\t| V COLOR\t| V STRENGTH\t|" << endl;
					while (row = mysql_fetch_row(res))
					{
						cout << "------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;

						cout << "|" << row[3] << "\t| " << row[1] << "          \t\t\t| " << row[2] << "\t|" << row[4] << "\t|" << row[0] << "\t\t|" << row[5] << "\t\t\t|" << row[6] << "\t|" << row[7] << "\t\t|" << row[8] << "\t\t|" << row[10] << "\t\t|" << endl;

					}
				}
				cout << "-------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;


				cout << "\nPlease enter plant ID:" << endl;
				cin >> plantID;
				cout << "---------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
				query = "SELECT * FROM plant join vegetable on p_id = v_id WHERE p_id =" + plantID + ";";
				q = query.c_str();
				qstate = mysql_query(conn, q);
				if (!qstate)
				{
					res = mysql_store_result(conn);
					cout << "|ID\t| NAME          \t\t\t| STOCK\t| STARTDATE\t| LIFESPAN\t| # OF GARDENERS\t| SOLD\t| V SEASON\t| V COLOR\t| V STRENGTH\t|" << endl;
					while (row = mysql_fetch_row(res))
					{
						cout << "------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;

						cout << "|" << row[3] << "\t| " << row[1] << "          \t\t\t| " << row[2] << "\t|" << row[4] << "\t|" << row[0] << "\t\t|" << row[5] << "\t\t\t|" << row[6] << "\t|" << row[7] << "\t\t|" << row[8] << "\t\t|" << row[10] << "\t\t|" << endl;

					}
				}
				cout << "------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
			}
			else if (gardenerSearchMethod == 5) // flower
			{
				cout << "-------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
				query = "SELECT * FROM plant join flower on p_id = fl_id";
				q = query.c_str();
				qstate = mysql_query(conn, q);
				if (!qstate)
				{
					res = mysql_store_result(conn);
					cout << "|ID\t| NAME          \t\t\t| STOCK\t| STARTDATE\t| LIFESPAN\t| # OF GARDENERS\t| SOLD\t| FL SEASON\t| FL COLOR\t| FL DURATION\t|" << endl;
					while (row = mysql_fetch_row(res))
					{
						cout << "------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;

						cout << "|" << row[3] << "\t| " << row[1] << "          \t\t\t| " << row[2] << "\t|" << row[4] << "\t|" << row[0] << "\t\t|" << row[5] << "\t\t\t|" << row[6] << "\t|" << row[7] << "\t\t|" << row[8] << "\t\t|" << row[10] << "\t\t|" << endl;

					}
				}
				cout << "-------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;


				cout << "\nPlease enter plant ID:" << endl;
				cin >> plantID;
				cout << "-------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
				query = "SELECT * FROM plant join flower on p_id = fl_id WHERE p_id =" + plantID + ";";
				q = query.c_str();
				qstate = mysql_query(conn, q);
				if (!qstate)
				{
					res = mysql_store_result(conn);
					cout << "|ID\t| NAME          \t\t\t| STOCK\t| STARTDATE\t| LIFESPAN\t| # OF GARDENERS\t| SOLD\t| FL SEASON\t| FL COLOR\t| FL DURATION\t|" << endl;
					while (row = mysql_fetch_row(res))
					{
						cout << "------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;

						cout << "|" << row[3] << "\t| " << row[1] << "          \t\t\t| " << row[2] << "\t|" << row[4] << "\t|" << row[0] << "\t\t|" << row[5] << "\t\t\t|" << row[6] << "\t|" << row[7] << "\t\t|" << row[8] << "\t\t|" << row[10] << "\t\t|" << endl;

					}
				}
				cout << "------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
			}

		startGardenerOption:
			cout << "\n****************************************************" << endl;
			cout << "***************Edit Plant Information***************" << endl;
			cout << "Which category would you like to change?" << endl;
			cout << "2. Name" << endl;
			cout << "3. Stock" << endl;
			cout << "4. Start Date" << endl;
			cout << "5. Life Span" << endl;
			cout << "6. Number of Gardeners" << endl;
			cout << "7. Amount Sold" << endl;
			cout << "8. Delete Plant" << endl;
			if (gardenerSearchMethod == 2)
				cout << "9. Tree Grade" << endl;
			else if (gardenerSearchMethod == 3)
				cout << "9. Fruit Season" << endl;
			else if (gardenerSearchMethod == 4)
			{
				cout << "9. Vegetable Season" << endl;
				cout << "10. Vegetable Color" << endl;
				cout << "11. Vegetable Strength" << endl;
			}
			else if (gardenerSearchMethod == 5)
			{
				cout << "9. Flower Season" << endl;
				cout << "10. Flower Color" << endl;
				cout << "11. Fower Duration" << endl;
			}
			cin >> modifyOption;
			if (modifyOption == 1)
			{
				cout << "\n***************Edit Plant ID***************" << endl;
				cout << "Input New ID: ";
				cin >> modifyString;
				query = "UPDATE PLANT SET p_id =" + modifyString + " WHERE p_id=" + plantID + ";";
				q = query.c_str();
				qstate = mysql_query(conn, q);
			}
			else if (modifyOption == 2)
			{
				cout << "\n***************Edit Plant Name***************" << endl;
				cout << "Input New Name: ";
				cin >> modifyString;
				query = "UPDATE PLANT SET p_name ='" + modifyString + "' WHERE p_id=" + plantID + ";";
				q = query.c_str();
				qstate = mysql_query(conn, q);
			}
			else if (modifyOption == 3)
			{
				cout << "\n***************Edit Plant Stock***************" << endl;
				cout << "Input Stock: ";
				cin >> modifyString;
				query = "UPDATE PLANT SET p_stock =" + modifyString + " WHERE p_id=" + plantID + ";";
				q = query.c_str();
				qstate = mysql_query(conn, q);
			}
			else if (modifyOption == 4)
			{
				cout << "\n***************Edit Plant Start Date***************" << endl;
				cout << "Input New Startdate (ex. YYYY-MM-DD): ";
				cin >> modifyString;
				query = "UPDATE PLANT SET p_startdate ='" + modifyString + "' WHERE p_id=" + plantID + ";";
				q = query.c_str();
				qstate = mysql_query(conn, q);
			}
			else if (modifyOption == 5)
			{
				cout << "\n***************Edit Plant Lifespan***************" << endl;
				cout << "Input New lifespan (in years): ";
				cin >> modifyString;
				query = "UPDATE PLANT SET p_lifespan =" + modifyString + " WHERE p_id=" + plantID + ";";
				q = query.c_str();
				qstate = mysql_query(conn, q);
			}
			else if (modifyOption == 6)
			{
				cout << "\n*************Edit Plant Number of Gardeners*************" << endl;
				cout << "Input New Number of Gardeners: ";
				cin >> modifyString;
				query = "UPDATE PLANT SET p_numbergardener =" + modifyString + " WHERE p_id=" + plantID + ";";
				q = query.c_str();
				qstate = mysql_query(conn, q);
			}
			else if (modifyOption == 7)
			{
				cout << "\n***************Edit Plant Amount Sold***************" << endl;
				cout << "Input New Amount Sold: ";
				cin >> modifyString;
				query = "UPDATE PLANT SET p_sold =" + modifyString + " WHERE p_id=" + plantID + ";";
				q = query.c_str();
				qstate = mysql_query(conn, q);
			}
			else if (modifyOption == 8)
			{
				cout << "\n***************Delete Plant***************" << endl;
				cout << "Are you sure you want to delete this plant (Y/N): ";
				cin >> yesNO;
				if (tolower(yesNO) == 'y')
				{
					query = "DELETE FROM PLANT WHERE p_id = '" + plantID + "';";
					q = query.c_str();
					qstate = mysql_query(conn, q);
				}
				else
					cout << "Delete Canceled" << endl;
			}
			else if (gardenerSearchMethod == 2 && modifyOption == 9)
			{
				cout << "\n***************Edit Tree Grade***************" << endl;
				cout << "Input New Tree Grade: ";
				cin >> modifyString;
				query = "UPDATE Tree SET tr_grade =" + modifyString + " WHERE tr_id=" + plantID + ";";
				q = query.c_str();
				qstate = mysql_query(conn, q);
			}
			else if (gardenerSearchMethod == 3 && modifyOption == 9)
			{
				cout << "\n***************Edit Fruit Season***************" << endl;
				cout << "Input New Fruit Season: ";
				cin >> modifyString;
				query = "UPDATE Fruit SET fr_season ='" + modifyString + "' WHERE fr_id=" + plantID + ";";
				q = query.c_str();
				qstate = mysql_query(conn, q);
			}
			else if (gardenerSearchMethod == 4 && modifyOption == 9)
			{
				cout << "\n***************Edit Vegetable Season***************" << endl;
				cout << "Input New Vegetable Season: ";
				cin >> modifyString;
				query = "UPDATE vegetable SET v_season ='" + modifyString + "' WHERE v_id=" + plantID + ";";
				q = query.c_str();
				qstate = mysql_query(conn, q);
			}
			else if (gardenerSearchMethod == 4 && modifyOption == 10)
			{
				cout << "\n***************Edit Vegetable Color***************" << endl;
				cout << "Input New Vegetable Color: ";
				cin >> modifyString;
				query = "UPDATE vegetable SET v_color ='" + modifyString + "' WHERE v_id=" + plantID + ";";
				q = query.c_str();
				qstate = mysql_query(conn, q);
			}
			else if (gardenerSearchMethod == 4 && modifyOption == 11)
			{
				cout << "\n***************Edit Vegetable Strength***************" << endl;
				cout << "Input New Vegetable Strength: ";
				cin >> modifyString;
				query = "UPDATE vegetable SET v_strength ='" + modifyString + "' WHERE v_id=" + plantID + ";";
				q = query.c_str();
				qstate = mysql_query(conn, q);
			}
			else if (gardenerSearchMethod == 5 && modifyOption == 9)
			{
				cout << "\n***************Edit Flower Season***************" << endl;
				cout << "Input New Flower Season: ";
				cin >> modifyString;
				query = "UPDATE flower SET fl_season ='" + modifyString + "' WHERE fl_id=" + plantID + ";";
				q = query.c_str();
				qstate = mysql_query(conn, q);
			}
			else if (gardenerSearchMethod == 5 && modifyOption == 10)
			{
				cout << "\n***************Edit Flower Color***************" << endl;
				cout << "Input New Flower Color: ";
				cin >> modifyString;
				query = "UPDATE flower SET fl_color ='" + modifyString + "' WHERE fl_id=" + plantID + ";";
				q = query.c_str();
				qstate = mysql_query(conn, q);
			}
			else if (gardenerSearchMethod == 5 && modifyOption == 11)
			{
				cout << "\n***************Edit Flower Duration***************" << endl;
				cout << "Input New Flower Duration: ";
				cin >> modifyString;
				query = "UPDATE flower SET fl_duration =" + modifyString + " WHERE fl_id=" + plantID + ";";
				q = query.c_str();
				qstate = mysql_query(conn, q);
			}

			else
			{
				cout << "Invalid Option.\n" << endl;
				goto startGardenerOption;
			}
			//Show plant data after data change
			if (gardenerSearchMethod == 1)
			{
				cout << "\n-------------------------------------------------------------------------------------------------------------------------" << endl;
				query = "SELECT * FROM PLANT WHERE p_id =" + plantID;
				q = query.c_str();
				qstate = mysql_query(conn, q);
				if (!qstate)
				{
					res = mysql_store_result(conn);
					cout << "|ID\t| NAME          \t\t\t| STOCK\t| STARTDATE\t| LIFESPAN\t| # OF GARDENERS\t| SOLD\t|" << endl;
					while (row = mysql_fetch_row(res))
					{
						cout << "-------------------------------------------------------------------------------------------------------------------------" << endl;

						cout << "|" << row[3] << "\t| " << row[1] << "          \t\t\t| " << row[2] << "\t|" << row[4] << "\t|" << row[0] << "\t\t|" << row[5] << "\t\t\t|" << row[6] << "\t|" << endl;

					}
				}
				cout << "-------------------------------------------------------------------------------------------------------------------------" << endl;
			}
			else if (gardenerSearchMethod == 2)
			{
				cout << "\n-----------------------------------------------------------------------------------------------------------------------------------------" << endl;
				query = "SELECT * FROM plant join tree on p_id = " + plantID;
				q = query.c_str();
				qstate = mysql_query(conn, q);
				if (!qstate)
				{
					res = mysql_store_result(conn);
					cout << "|ID\t| NAME          \t\t\t| STOCK\t| STARTDATE\t| LIFESPAN\t| # OF GARDENERS\t| SOLD\t| Tr Grade\t|" << endl;
					while (row = mysql_fetch_row(res))
					{
						cout << "---------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;

						cout << "|" << row[3] << "\t| " << row[1] << "          \t\t\t| " << row[2] << "\t|" << row[4] << "\t|" << row[0] << "\t\t|" << row[5] << "\t\t\t|" << row[6] << "\t|" << row[7] << "\t\t|" << endl;

					}
				}
				cout << "---------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
			}
			else if (gardenerSearchMethod == 3)
			{
				cout << "\n-----------------------------------------------------------------------------------------------------------------------------------------" << endl;
				query = "SELECT * FROM plant join fruit on p_id = fr_id WHERE p_id =" + plantID + ";";
				q = query.c_str();
				qstate = mysql_query(conn, q);
				if (!qstate)
				{
					res = mysql_store_result(conn);
					cout << "|ID\t| NAME          \t\t\t| STOCK\t| STARTDATE\t| LIFESPAN\t| # OF GARDENERS\t| SOLD\t| FR SEASON\t|" << endl;
					while (row = mysql_fetch_row(res))
					{
						cout << "-----------------------------------------------------------------------------------------------------------------------------------------" << endl;

						cout << "|" << row[3] << "\t| " << row[1] << "          \t\t\t| " << row[2] << "\t|" << row[4] << "\t|" << row[0] << "\t\t|" << row[5] << "\t\t\t|" << row[6] << "\t|" << row[7] << "\t\t|" << endl;

					}
				}
				cout << "-----------------------------------------------------------------------------------------------------------------------------------------" << endl;
			}
			else if (gardenerSearchMethod == 4)
			{
				cout << "\n-------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
				query = "SELECT * FROM plant join vegetable on p_id = v_id WHERE p_id =" + plantID + ";";
				q = query.c_str();
				qstate = mysql_query(conn, q);
				if (!qstate)
				{
					res = mysql_store_result(conn);
					cout << "|ID\t| NAME          \t\t\t| STOCK\t| STARTDATE\t| LIFESPAN\t| # OF GARDENERS\t| SOLD\t| V SEASON\t| V COLOR\t| V STRENGTH\t|" << endl;
					while (row = mysql_fetch_row(res))
					{
						cout << "-------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;

						cout << "|" << row[3] << "\t| " << row[1] << "          \t\t\t| " << row[2] << "\t|" << row[4] << "\t|" << row[0] << "\t\t|" << row[5] << "\t\t\t|" << row[6] << "\t|" << row[7] << "\t\t|" << row[8] << "\t\t|" << row[10] << "\t\t|" << endl;

					}
				}
				cout << "-------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
			}
			else if (gardenerSearchMethod == 5)
			{
				cout << "\n-------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
				query = "SELECT * FROM plant join flower on p_id = fl_id WHERE p_id =" + plantID + ";";
				q = query.c_str();
				qstate = mysql_query(conn, q);
				if (!qstate)
				{
					res = mysql_store_result(conn);
					cout << "|ID\t| NAME          \t\t\t| STOCK\t| STARTDATE\t| LIFESPAN\t| # OF GARDENERS\t| SOLD\t| FL SEASON\t| FL COLOR\t| FL DURATION\t|" << endl;
					while (row = mysql_fetch_row(res))
					{
						cout << "------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;

						cout << "|" << row[3] << "\t| " << row[1] << "          \t\t\t| " << row[2] << "\t|" << row[4] << "\t|" << row[0] << "\t\t|" << row[5] << "\t\t\t|" << row[6] << "\t|" << row[7] << "\t\t|" << row[8] << "\t\t|" << row[10] << "\t\t|" << endl;

					}
				}
				cout << "------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
			}




			cout << "Would you like to update another plant?: ";
			cin >> yesNO;
			if (tolower(yesNO) == 'y')
				goto gardenerUpdate;
			goto gardenerMenuSelect;




			break;
		case 3:
		gardenerFulfill:
			cout << "\n********************************************" << endl;
			cout << "***************Fullfil Orders***************" << endl;
			cout << "These are the current orders that have yet to be filled." << endl;
			query = "SELECT * FROM orders WHERE order_estimate_deliver_date is NULL";
			q = query.c_str();
			qstate = mysql_query(conn, q);
			if (!qstate)
			{
				res = mysql_store_result(conn);
				cout << "\n-------------------------------------------------" << endl;
				cout << "|ID\t| DATE\t\t\t| QUANTITY\t|" << endl;
				while (row = mysql_fetch_row(res))
				{
					cout << "-------------------------------------------------" << endl;
					cout << "|" << row[0] << "\t| " << row[1] << "\t| " << row[2] << "\t\t|" << endl;
				}
			}
			cout << "-------------------------------------------------\n" << endl;
			cout << "Input the ID of the order you want to fulfill:";
			cin >> orderID;
			cout << "Input the estimated time of delivery (yyyy/mm/dd):";
			cin >> estDeliver;
			query = "UPDATE ORDERS SET  order_estimate_deliver_date = '" + estDeliver + "' WHERE order_id = " + orderID + ";";
			q = query.c_str();
			qstate = mysql_query(conn, q);
			if (!qstate)
			{
				cout << "Order has been filled!" << endl;
			}
			cout << " Would you like to fulfill another order? (Y/N):";
			cin >> yesNO;
			cout << endl;
			if (tolower(yesNO) == 'y')
				goto gardenerFulfill;
			goto gardenerMenuSelect;
			break;
		case 4:
			cout << "\n************************************************" << endl;
			cout << "***************Find Work Location***************" << endl;
			cout << "You are scheduled to work at ";
			query = "SELECT lo_cityname FROM g_work join location on gwork_lo_id = lo_id WHERE gwork_gardener_id = " + gardenerID;
			q = query.c_str();
			qstate = mysql_query(conn, q);
			if (!qstate)
			{
				res = mysql_store_result(conn);
				while (row = mysql_fetch_row(res))
				{
					cout << row[0] << endl << endl;

				}
			}

			goto gardenerMenuSelect;
			break;
		case 5:
			break;
		}



	}

}

string leftpadding()
{
	string str;
	str = "\"";
	return str;
}
void customer()
{
	//customer_start:

	string fname, lname, address, city, zipcode, email, password;
	string login_accnumber, login_password;
	string choice;
	string query;
	const char* q;
	cout << "Enter 1 to register" << endl
		<< "Enter any number to login with existing account" << endl;
	cin >> choice;

	if (choice == "1")
	{
		cout << "***************Customer Register****************" << endl
			<< "First name: ";
		cin.ignore(100, '\n');
		getline(cin, fname);
		cout << endl << "Last name: ";
		getline(cin, lname);
		cout << endl << "Address: ";
		getline(cin, address);
		cout << endl << "City: ";
		getline(cin, city);
		cout << endl << "Zip Code: ";
		cin >> zipcode;
		cout << endl << "Email: ";
		cin >> email;
		cout << endl << "Password: ";
		cin >> password;
		cin.ignore(100, '\n');

	Setaccnumber:
		accnum = rand() % UINT_MAX + 1;
		query = "select c_accnumber from customer where c_accnumber=\"" + to_string(accnum) + "\";";
		q = query.c_str();
		qstate = mysql_query(conn, q);

		res = mysql_store_result(conn);
		if (mysql_num_rows(res) == 0)
		{
			//cout << "New acc number is accepted" << endl;
		}
		else
		{
			//cout << "New acc number already exists" << endl;
			goto Setaccnumber;
		}
		query = "insert customer values (md5(\"" + password + "\"),\"" + address + "\",\"" + city + "\",\"" + zipcode + "\",\"" + to_string(accnum) + "\",\"" + email + "\",\"" + fname + "\",\"" + lname + "\");";
		q = query.c_str();
		qstate = mysql_query(conn, q);
		if (!qstate)
		{
			cout << "Register new Account successful" << endl;
			query = "select c_accnumber from customer where c_email = \"" + email + "\";";
			q = query.c_str();
			qstate = mysql_query(conn, q);
			if (!qstate)
			{
				res = mysql_store_result(conn);
				row = mysql_fetch_row(res);
				cout << "Your account number is: " << row[0] << endl;
				cout << "**********************************************" << endl;
			}
			else
			{
				cout << "Fail to show newly created customer accnumber" << endl;
				cout << "**********************************************" << endl;
			}

		}
		else
		{
			cout << mysql_error(conn) << endl;
			cout << "Fail to register Account" << endl;
			cout << "**********************************************" << endl;
		}

	}// go back to main screen
	else// Customer log in
	{
	customer_login:
		cin.ignore(100, '\n');
		cout << "***************Customer Login***************" << endl;
		cout << "Account number: ";
		getline(cin, login_accnumber);
		cout << "Password: ";
		getline(cin, login_password);
		cout << "**********************************************" << endl;

		query = "select c_accnumber from customer where c_accnumber=\"" + login_accnumber + "\";";
		q = query.c_str();
		qstate = mysql_query(conn, q);
		res = mysql_store_result(conn);
		if (mysql_num_rows(res) == 0)
		{
			cout << "Cant find matched customer account number" << endl;// cant find matched customer account number
			cout << "**********************************************" << endl;
			string a;
			cout << "Enter 1 to try again" << endl
				<< "Enter 2 to get back to main menu" << endl;
			getline(cin, a);
			if (a == "1")
			{
				goto customer_login;
			}
			else
			{
				// back to the main (main menu) 
			}
		}
		else
		{
			cout << "Valid customer acc number" << endl; // match customer acc number
			// check for password match or not
			query = "select c_accnumber from customer where c_accnumber = \"" + login_accnumber + "\" and c_password= md5(\"" + login_password + "\");";
			q = query.c_str();
			qstate = mysql_query(conn, q);
			res = mysql_store_result(conn);
			if (mysql_num_rows(res) == 0)
			{
				cout << "Wrong password" << endl;// entered password does not match
				cout << "**********************************************" << endl;
				string a;
				cout << "Enter 1 to try again" << endl
					<< "Enter 2 to get back to main menu" << endl;
				getline(cin, a);
				if (a == "1")
				{
					goto customer_login;
				}
				else
				{
					// back to the main (main menu) 
				}
			}
			else
			{
				// successful login with entered accnumber and password 
				cout << "***************************************************" << endl;
				cout << "***************Successful login********************" << endl;
				cout << "***************************************************" << endl;
				// customer option
			customer_option:
				int customer_action;
				cout << "***************Customer Action Panel***************" << endl
					<< "Enter 1 to View your account infomation" << endl
					<< "Enter 2 to Change your personal information" << endl
					<< "Enter 3 to Check plant availability " << endl
					<< "Enter 4 to rate and make comment for your recent order " << endl
					<< "Enter 5 to Place an order" << endl
					<< "Enter 6 to View order detail and status" << endl
					<< "Enter 7 to view rating and comment for a desired plant " << endl
					<< "Enter other to log out" << endl;
				cin >> customer_action;
				cin.ignore(100, '\n');
				cout << "***************************************************" << endl;
				switch (customer_action)
				{
				case 1://View your account infomation
				{
					query = "select * from customer where c_accnumber = \"" + login_accnumber + "\";";
					q = query.c_str();
					qstate = mysql_query(conn, q);
					if (!qstate)
					{
						res = mysql_store_result(conn);
						while (row = mysql_fetch_row(res))
						{

							/*printf("First name: %5s, Last name: %5s, Address: %5s, City: %s, Zipcode: %s, Email: %s\n", row[6], row[7], row[1], row[2], row[3], row[5]);*/
							//printf("First name %5s, Last name: %5s, Address: %5s, City: %s, Zipcode: %s, Email: %s\n")
							printf("First name: %10s, Last name: %10s, Address: %10s, City: %10s, Zipcode: %10s, Email: %10s\n", row[6], row[7], row[1], row[2], row[3], row[5]);
							/*printf("First name\t Last Name\t Address\t City\t Zipcode\t Email\t\n");
							printf("%10s %10s %10s %10s %10s %10s\n", row[6], row[7], row[1], row[2], row[3], row[5]);*/
						}
					}
					else
					{
						cout << mysql_error(conn) << endl;
						cout << "Fail to retrieve customer info" << endl;
					}
					cout << "***************************************************" << endl;
					goto customer_option;
					break;
				}
				case 2://Change your personal information
				{
				info_change:
					string info_change;
					cout << "Enter fname, lname, address, city, zipcode, email, or password" << endl;
					getline(cin, info_change);
					if (info_change == "fname")
					{
						cout << "Enter new first name: ";
						getline(cin, fname);
						query = "update customer set c_fname = \"" + fname + "\" where c_accnumber = \"" + login_accnumber + "\";";
						q = query.c_str();
						qstate = mysql_query(conn, q);

						if (!qstate)
						{
							cout << "Update first name successful" << endl;
						}
						else
						{
							cout << mysql_error(conn) << endl;
							cout << "fail to Update first name" << endl;
						}
					}
					else if (info_change == "lname")
					{
						cout << "Enter new last name: ";
						getline(cin, lname);
						query = "update customer set c_lname = \"" + lname + "\" where c_accnumber = \"" + login_accnumber + "\";";
						q = query.c_str();
						qstate = mysql_query(conn, q);
						if (!qstate)
						{
							cout << "Update last name successful" << endl;
						}
						else
						{
							cout << mysql_error(conn) << endl;
							cout << "fail to Update last name" << endl;
						}
					}
					else if (info_change == "address")
					{
						cout << "Enter new address : ";
						getline(cin, address);
						query = "update customer set c_streetaddress = \"" + address + "\" where c_accnumber = \"" + login_accnumber + "\";";
						q = query.c_str();
						qstate = mysql_query(conn, q);

						if (!qstate)
						{
							cout << "Update address successful" << endl;
						}
						else
						{
							cout << mysql_error(conn) << endl;
							cout << "fail to Update address" << endl;
						}
					}
					else if (info_change == "city")
					{
						cout << "Enter new city: ";
						getline(cin, city);
						query = "update customer set c_city = \"" + city + "\" where c_accnumber = \"" + login_accnumber + "\";";
						q = query.c_str();
						qstate = mysql_query(conn, q);

						if (!qstate)
						{
							cout << "Update city successful" << endl;
						}
						else
						{
							cout << mysql_error(conn) << endl;
							cout << "Fail to Update city" << endl;
						}
					}
					else if (info_change == "zipcode")
					{
						cout << "Enter new zipcode: ";
						getline(cin, zipcode);
						query = "update customer set c_zipcode = \"" + zipcode + "\" where c_accnumber = \"" + login_accnumber + "\";";
						q = query.c_str();
						qstate = mysql_query(conn, q);

						if (!qstate)
						{
							cout << "Update zipcode successful" << endl;
						}
						else
						{
							cout << mysql_error(conn) << endl;
							cout << "Fail to Update zipcode" << endl;
						}
					}
					else if (info_change == "email")
					{
						cout << "Enter new email : ";
						getline(cin, email);
						query = "update customer set c_fname = \"" + email + "\" where c_accnumber = \"" + login_accnumber + "\";";
						q = query.c_str();
						qstate = mysql_query(conn, q);
						if (!qstate)
						{
							cout << "Update email address successful" << endl;
						}
						else
						{
							cout << mysql_error(conn) << endl;
							cout << "Fail to Update email address" << endl;
						}
					}
					else if (info_change == "password")
					{
						cout << "Enter new password: ";
						getline(cin, password);
						query = "update customer set c_password = md5(\"" + password + "\") where c_accnumber = \"" + login_accnumber + "\";";
						q = query.c_str();
						qstate = mysql_query(conn, q);
						if (!qstate)
						{
							cout << "Update password successful" << endl;
						}
						else
						{
							cout << mysql_error(conn) << endl;
							cout << "Fail to Update password" << endl;
						}
					}
					else
					{
						string retry;
						cout << "Wrong input" << endl
							<< "retry? (1 for yes/ other for no) : ";
						getline(cin, retry);
						cout << endl;
						if (retry == "1")
						{
							goto info_change;
						}

					}
					cout << "***************************************************" << endl;
					goto customer_option;
					break;
				}
				case 3://Check plant availability
				{
					// check specific plant
				checkplant:
					string plant, option, location;

					cout << "Using plant name or location?" << endl;
					getline(cin, option);
					cout << "***************************************************" << endl;
					if (option == "plant")
					{
						vector<string> vect3;
						//mysql_free_result(res);
						query = "select p_name from plant order by p_name asc;";
						q = query.c_str();
						qstate = mysql_query(conn, q);

						if (!qstate)
						{
							cout << "***************Plant list***************" << endl;
							res = mysql_store_result(conn);
							while (row = mysql_fetch_row(res))
							{
								printf("%s\n", row[0]);
								vect3.push_back(row[0]);
							}
							cout << "***************END Plant list***************" << endl;
						}
						else
						{
							cout << mysql_error(conn) << endl;
							cout << "Fail to display plant list" << endl;
							cout << "**********************************************" << endl;
						}
						cout << "What plant are you looking for? " << endl;
						getline(cin, plant);
						cout << "***************************************************" << endl;
						bool check3 = false;
						for (int i = 0; i < vect3.size(); i++)
						{
							if (vect3[i] == plant)
							{
								check3 = true;
								break;// out of the for loop
							}
						}
						if (check3)
						{
							query = "select location.* from location where exists (select * from foundat where (location.lo_id = foundat.foundat_lo_id) and exists (select * from plant where (plant.p_id = foundat_p_id) and plant.p_name = \"" + plant + "\")) ";
							q = query.c_str();
							qstate = mysql_query(conn, q);

							if (!qstate)
							{
								cout << "Found location successful" << endl;
								cout << "***************************************************" << endl;
								res = mysql_store_result(conn);
								while (row = mysql_fetch_row(res))
								{
									printf("City Name: %s, Area code: %s, Temperature: %s, Location ID: %s \n", row[0], row[1], row[2], row[3]);
								}
							}
							else
							{
								cout << mysql_error(conn) << endl;
								cout << "Fail to found location" << endl;
								cout << "***************************************************" << endl;
							}
							goto customer_option;
						}
						else
						{
							cout << mysql_error(conn) << endl;
							cout << "Wrong input" << endl;
							cout << "***************************************************" << endl;
							goto customer_option;
						}
					}
					else if (option == "location")
					{
						vector<string> vect3;
						// listing all available location
						query = "select * from location;";
						q = query.c_str();
						qstate = mysql_query(conn, q);
						if (!qstate)
						{
							cout << "Access all location info successful" << endl;
							res = mysql_store_result(conn);
							while (row = mysql_fetch_row(res))
							{
								printf("City Name: %s, Area code: %s, Temperature: %s, Location ID: %s \n", row[0], row[1], row[2], row[3]);
								vect3.push_back(row[3]);
							}
							cout << "***************************************************" << endl;
							//mysql_free_result(conn); /************FREE result**************/
							string loID;
							cout << "Enter your desired location ID: " << endl;
							getline(cin, loID);
							cout << "***************************************************" << endl;

							bool check3 = false;
							for (int i = 0; i < vect3.size(); i++)
							{
								if (vect3[i] == loID)
								{
									check3 = true;
									break;// out of the for loop
								}
							}
							if (check3)
							{
								query = "select * from plant where exists (select * from foundat where foundat.foundat_p_id = plant.p_id and exists (select * from location where location.lo_id = foundat.foundat_lo_id and location.lo_id = \"" + loID + "\"))";
								q = query.c_str();
								qstate = mysql_query(conn, q);
								if (!qstate)
								{
									cout << "Plant list at location \"" << loID << "\"" << endl;
									res = mysql_store_result(conn);
									while (row = mysql_fetch_row(res))
									{
										printf("Plant name: %s, Available stock: %s \n", row[1], row[2]);
									}
									cout << "***************************************************" << endl;
									//mysql_free_result(conn); /************FREE result**************/
								}
								else
								{
									cout << mysql_error(conn) << endl;
									cout << "Fail to access plant at the desired location" << endl;
									cout << "***************************************************" << endl;
									//mysql_free_result(conn); /************FREE result**************/
								}
							}
							else
							{
								cout << "Wrong input location ID" << endl;
								cout << "***************************************************" << endl;
							}
						}
						else
						{
							cout << mysql_error(conn) << endl;
							cout << "Fail to access location info" << endl;
							cout << "***************************************************" << endl;
						}
						goto customer_option;
					}
					else
					{
						cout << "Wrong input" << endl;
						cout << "***************************************************" << endl;
						goto customer_option;
					}
					break;
				}
				case 4:// make rating and review on recent order
				{
					// display recent order 
					query = "select make_order_id from make where make_c_accnumber = \"" + login_accnumber + "\";";
					q = query.c_str();
					qstate = mysql_query(conn, q);
					if (!qstate)
					{
						res = mysql_store_result(conn);
						if (mysql_num_rows(res) == 0) // no recent order
						{
							cout << "No recent order from your account" << endl;// No order for this account
							goto customer_option;
						}
						else // has recent order
						{
							vector<string> vect;
							bool check = false;
							cout << "***************Customer order history***************" << endl;
							while (row = mysql_fetch_row(res))
							{
								printf("Order ID: %s\n", row[0]);
								vect.push_back(row[0]);
							}
							cout << "***************************************************" << endl;
							//get input order ID
							string make_order_id, rating, comment;
							cout << "Choose your desired order ID: " << endl;
							getline(cin, make_order_id);
							cout << "Enter rating: " << endl;
							getline(cin, rating);
							cout << "Enter comment: " << endl;
							getline(cin, comment);

							// check input order_id
							for (int i = 0; i < vect.size(); i++)
							{
								if (make_order_id == vect[i])
								{
									check = true;
									break;
								}
							}
							if (check)
							{
								query = "SET SQL_SAFE_UPDATES = 0;";
								q = query.c_str();
								mysql_query(conn, q);


								query = "update make set rating = \"" + rating + "\", comments = \"" + comment + "\", datetimes = default where make_c_accnumber = \"" + login_accnumber + "\" and make_order_id = \"" + make_order_id + "\" ;";
								q = query.c_str();
								qstate = mysql_query(conn, q);
								if (!qstate)
								{
									cout << "Successful adding Rating" << endl;
								}
								else
								{
									cout << mysql_error(conn) << endl;
									cout << "Fail update Rating" << endl;
									cout << "***************************************************" << endl;

								}
								query = "SET SQL_SAFE_UPDATES = 1;";
								q = query.c_str();
								mysql_query(conn, q);

							}
							else // no matched order_id
							{
								cout << "Order ID does not exists in your account" << endl;
							}
						}
					}
					else
					{
						cout << mysql_error(conn) << endl;
						cout << "Fail to retrieve order list of customer" << endl;
					}
					cout << "**********************************************" << endl;
					goto customer_option;
					break;
				}// end case 4
				case 5://Place an order (have to get the date for each order)
				{
					string order_plant, quantity;
					vector<string> vect5;
					//mysql_free_result(res);

					int order_id;

					query = "select p_name from plant;";
					q = query.c_str();
					qstate = mysql_query(conn, q);

					if (!qstate)
					{
						cout << "***************Plant list***************" << endl;
						res = mysql_store_result(conn);
						while (row = mysql_fetch_row(res))
						{
							printf("%s\n", row[0]);
							vect5.push_back(row[0]);
						}
						cout << "***************END Plant list***************" << endl;
					}
					else
					{
						cout << "Fail to display plant list" << endl;
						cout << "**********************************************" << endl;
					}

					cout << "What plant do you want to order? " << endl;
					getline(cin, order_plant);
					cout << "quatity: " << endl;
					getline(cin, quantity);
					bool check5 = false;
					for (int i = 0; i < vect5.size(); i++)
					{
						if (vect5[i] == order_plant)
						{
							check5 = true;
							break;// out of the for loop
						}
					}
					if (check5)
					{
					setorderid:
						order_id = rand() % UINT_MAX + 1;
						query = "select order_id from orders where order_id=\"" + to_string(order_id) + "\";";
						q = query.c_str();
						qstate = mysql_query(conn, q);

						res = mysql_store_result(conn);
						if (mysql_num_rows(res) == 0)
						{
							cout << "New order ID number is accepted" << endl;
							query = "insert into orders values (" + to_string(order_id) + ", default," + quantity + ",default, default);";
							q = query.c_str();
							qstate = mysql_query(conn, q);
							if (!qstate)
							{
								cout << "New order is placed succesfully" << endl;
								// update p_stock and p_sold 
								query = "update plant set p_stock = p_stock - \"" + quantity + "\", p_sold = p_sold + \"" + quantity + "\"where p_name = \"" + order_plant + "\";";
								//query_order = "update plant set p_stock = p_stock - 1, p_sold = p_sold + 1 where p_name = \"" + order_plant + "\";";
								q = query.c_str();
								qstate = mysql_query(conn, q);
								if (!qstate)
								{
									cout << "Update p_stock and p_sold sucessfully" << endl;

									// get the plant ID from plant name
									query = "select p_id from plant where p_name = \"" + order_plant + "\"";
									q = query.c_str();
									qstate = mysql_query(conn, q);
									if (!qstate)
									{
										cout << "Successful get Plant ID from p_name" << endl;

										res = mysql_store_result(conn);
										row = mysql_fetch_row(res); // row[0] contain the ID
										string plantID;
										plantID = row[0];
										// insert to include table
										query = "insert into include values (" + plantID + "," + to_string(order_id) + ");";
										q = query.c_str();
										qstate = mysql_query(conn, q);
										if (!qstate)
										{
											cout << "Successful update the include table" << endl;
										}
										else
										{
											cout << "fail to insert the PlantID and orderID into INCLUDE table" << endl;
										}
									}
									else
									{
										cout << "Fail to find plant ID" << endl;
									}

								}
								else
								{
									cout << mysql_error(conn) << endl;
									cout << "Fail to update p_stock and p_sold  " << endl;
								}
							}
							else
							{
								cout << mysql_error(conn) << endl;
								cout << "Fail to make new order" << endl;
							}

							//update the make for this new order
							query = "insert into make values ( " + login_accnumber + "," + to_string(order_id) + ",default, default, default);";
							q = query.c_str();
							qstate = mysql_query(conn, q);

							if (!qstate)
							{
								//cout << "New order is added to MAKE table" << endl;
							}
							else
							{
								cout << mysql_error(conn) << endl;
								cout << "Fail to add new order to MAKE table" << endl;
							}
						}
						else
						{
							//cout << "New acc number already exists" << endl;
							//	cout << mysql_num_rows(res) << endl;
							goto setorderid;
						}
					}
					else
					{
						cout << "Wrong plant name input" << endl;
					}
					cout << "**********************************************" << endl;
					goto customer_option;
					break;
				}//end case 5

				case 6:// view detail order and status
				{
					cout << "***************Order detail***************" << endl;
					string str;
					query = "select * from orders inner join make on(orders.order_id = make.make_order_id) inner join customer on(customer.c_accnumber = make.make_c_accnumber)	where customer.c_accnumber = \"" + login_accnumber + "\" order by order_date desc;";
					//query = "select * from customer";
					q = query.c_str();
					qstate = mysql_query(conn, q);
					if (!qstate)
					{
						res = mysql_store_result(conn);
						while (row = mysql_fetch_row(res))
						{
							printf("Order ID: %s, Order Date: %s, Quantity: %s, Shipper: %s, Estimated Deliver date: %s, Rating: %s, Address: %s, City: %s, Zipcode: %s\n", row[0], row[1], row[2], row[3], row[4], row[7], row[11], row[12], row[13]);
						}
					}
					else
					{
						cout << mysql_error(conn) << endl;
						cout << "Fail to display plant list" << endl;
					}
					goto customer_option;
					break;
				}
				case 7:// view rating of a desired plant
				{
					string view_plant, view_all;
					vector<string> vect6;
					query = "select p_name from plant;";
					q = query.c_str();
					qstate = mysql_query(conn, q);

					if (!qstate)
					{
						cout << "***************Plant list***************" << endl;
						res = mysql_store_result(conn);
						while (row = mysql_fetch_row(res))
						{
							printf("%s\n", row[0]);
							vect6.push_back(row[0]);
						}
						cout << "***************END Plant list***************" << endl;
					}
					else
					{
						cout << mysql_error(conn) << endl;
						cout << "Fail to display plant list" << endl;
					}

					cout << "View all plant (y/n)?" << endl;
					getline(cin, view_all);
					if (view_all == "y")
					{
						query = "select p_name as Plant,rating, comments from make inner join orders on(make.make_order_id = orders.order_id) inner join include on(include.include_order_id = orders.order_id) join plant on(plant.p_id = include.include_p_id) order by p_name asc";
						q = query.c_str();
						qstate = mysql_query(conn, q);

						if (!qstate)
						{
							cout << "*****************ALL plant rating********************" << endl;
							res = mysql_store_result(conn);
							while (row = mysql_fetch_row(res))
							{
								printf("Plant: %s, Rating: %s, Comments: %s \n", row[0], row[1], row[2]);
							}
						}
						else
						{
							cout << mysql_error(conn) << endl;
							cout << "Fail to list ALL plant rating" << endl;
						}
					}
					else
					{
						cout << "What plant do you want to view? " << endl;
						getline(cin, view_plant);
						bool check6 = false;
						for (int i = 0; i < vect6.size(); i++)
						{
							if (vect6[i] == view_plant)
							{
								check6 = true;
								break;// out of the for loop
							}
						}
						if (check6)
						{
							query = "select p_name as Plant,rating, comments from make inner join orders on(make.make_order_id = orders.order_id) inner join include on(include.include_order_id = orders.order_id) join plant on(plant.p_id = include.include_p_id) where p_name = \"" + view_plant + "\"order by p_name asc";
							q = query.c_str();
							qstate = mysql_query(conn, q);

							if (!qstate)
							{
								cout << "***************************************************" << endl;
								res = mysql_store_result(conn);
								while (row = mysql_fetch_row(res))
								{
									printf("Plant: %s, Rating: %s, Comments: %s \n", row[0], row[1], row[2]);
								}
							}
							else
							{
								cout << mysql_error(conn) << endl;
								cout << "Fail to list plant rating" << endl;
							}

						}
						else
						{
							cout << mysql_error(conn) << endl;
							cout << "Wrong input plant to view rating" << endl;
						}
					}
					cout << "***************************************************" << endl;
					goto customer_option;
					break;
				}//end case 7
				default:
				{
					cout << "***************Logging out***************" << endl;
					break;
				}// end default

				}//end switch
				cout << endl;

			}
		}
	}
}
int main()
{
	/**************************************************************
	  Initialize and connect to demand database at root directory
	***************************************************************/
start:
	conn = mysql_init(0);

	string password;
	string databasename;

	cout << "Enter your database name: ";
	getline(cin, databasename);
	cout << endl;

	cout << "Enter your database password: ";
	getline(cin, password);
	cout << endl;

	const char* pass = password.c_str();
	const char* name = databasename.c_str();

	conn = mysql_real_connect(conn, "localhost", "root", pass, name, 3306, NULL, 0);

	if (conn)
	{
		cout << "Successful connection to database!" << endl;
	}
	else
	{
		cout << mysql_error(conn) << endl;
		cout << "Connection to database has failed!" << endl;

		cout << "**********************************************" << endl;
		goto start;
	}

	/**************************************************************
	  Stage 1: Identity
	***************************************************************/
	while (1)
	{
		string identity;

		identityMessage();

		getline(cin, identity);	// taking identity option
		cout << "**********************************************" << endl;
		/***************
			manager
		****************/
		if (identity == "1")
		{
			manager();
		}
		/***************
			staff
		***************/
		else if (identity == "2")
		{
			staff();
		}
		/***************
			gardener
		***************/
		else if (identity == "3")
		{
			gardener();
		}
		/***************
			customer
		***************/
		else if (identity == "4")
		{
			customer();
		}
		/***************
			log out and change to another database
		***************/
		else
		{
			mysql_close(conn);
			system("cls");
			cout << "**********************************************" << endl;
			goto start;
		}
	}
	return 0;
}
#SJSU CMPE 138 Spring 2019 TEAM 12
#project
drop database if exists plant_database;
create database plant_database;
use plant_database;
########################big table#######################
#to explicitly make disjoint
#create table employeetype
#(
#	employee_type	varchar(1) not null check (employee_type in (1,2,3)),
#    primary key (employee_type)
#    emp_name			varchar(20),		
#    foreign key (emp_name) references employee(name)
#    # 1: manager, 2: staff, 3: gardener
#);
create table employee
(	
    e_id				int check (e_id >0),#	e_id				varchar(8)	not null,
	e_gender			varchar(8)	not null check (e_gender = "male" or e_gender = "female" or e_gender = "other"),
	e_startdate			date,
	e_fname				varchar(10),
	e_lname				varchar(10),	
	e_salary			numeric(8,2) not null check (salary > 29000),
	e_birthday			date,
	e_phone				varchar(10),
	e_password			longblob not null,
    primary key (e_id),
	index e_name (e_fname, e_lname)
);
create table customer
(
	c_password			longblob	not null,	
	c_streetaddress		varchar(50)	not null,
	c_city				varchar(50)	not null,
	c_zipcode			varchar(20)	not null,
	c_accnumber			int check (c_accnumber >0),#	c_accnumber			varchar(8)	not null,
	c_email				varchar(100)	not null,
	c_fname				varchar(20)	not null,
	c_lname				varchar(20)	not null,
	primary key (c_accnumber),
	index c_deliveryaddresssd(c_streetaddress, c_city, c_zipcode),
	unique (c_email)
 );
create table location
(
	lo_cityname		varchar(20)	not null,##	lo_cityname		varchar(20)	not null default "huyhuyhuyhuy",
	lo_areacode		numeric(5,0)	not null,
	lo_temperature		numeric(3,0)	not null,
	lo_id				int not null check (lo_id>=0),  
	primary key (lo_id)
);
/*create table shipper
(
	shipper_id			varchar(8),
	shipper_name		varchar(20),
	primary key (shipper_id)
);*/
create table plant
(
	p_lifespan			numeric(3,0),
	p_name				varchar(20),#	p_stock				int check (p_stock >=0),
	p_stock				int,
    p_id				int check (p_id >0),#	p_id				varchar(8),
	p_startdate			date default null,#this does not make sense here
	p_numbergardener	numeric(2,0) check (p_numergardener>=0 and p_numbergardener<=99),
	p_sold 				int check (p_sold >= 0),
	primary key (p_id), 
    unique (p_name)
 );
create table orders
(
#	order_id						varchar(8),
	order_id						int check (order_id >0),
	order_date						datetime default current_timestamp,
    order_quantity					int,
    order_shipper_name				varchar(40) default "UPS",
    order_estimate_deliver_date		date default null,
	primary key (order_id)
 );

#####################small table####################################################################################################################

create table gardener
(
	gardener_id			int check (gardener_id >0),
    gardener_bonus		numeric(8,2),
    primary key (gardener_id),
	foreign key (gardener_id) references employee(e_id) on delete cascade
);
create table staff
(
	staff_id			int check (staff_id >0),
	staff_level			numeric(1,0) check (staff_level>=0),
    primary key (staff_id),
    foreign key (staff_id) references employee(e_id) on delete cascade
);
create table manager
(
	manager_id			int check (manager_id >0),
    manager_commission 	int,
    primary key (manager_id),
    foreign key (manager_id) references employee(e_id) on delete cascade
);
############################################################################################################################################################
create table  tree
(
	tr_grade		varchar(1) not null check (tr_grade<10 and tr_grade>=0),
    tr_id			int check (tr_id >0),
    primary key (tr_id),
    foreign key (tr_id) references plant(p_id) on delete cascade   
);
create table fruit
(
	fr_season	varchar(6) not null check (fr_season in ("Spring", "Fall", "Summer", "Winter")),
    fr_id		int check (fr_id >0),
    primary key (fr_id),
    foreign key (fr_id) references plant(p_id) on delete cascade
);
create table vegetable
(
	v_season	varchar(8) not null check (f_season in ("Spring", "Fall", "Summer", "Winter")),
    v_color		varchar(20) not null,
    v_id		int check (v_id >0),
    v_strength	varchar(6) check (v_strength in ("low","medium","high")),
    primary key (v_id),
    foreign key (v_id) references plant(p_id) on delete cascade
);
create table flower
(
	fl_season	varchar(8) not null check (f_season in ("Spring", "Fall", "Summer", "Winter")),
    fl_color	varchar(20) not null,
    fl_id		int check (fl_id >0),
    fl_duration	int check (fl_duration>0),
    primary key (fl_id),
    foreign key (fl_id) references plant(p_id) on delete cascade
);
############################################################################################################################################################
##########################relationship table##########################
create table g_work ##1:N relationship where N on gardener
(
	gwork_gardener_id			int check (gwork_gardener_id >0),
    gwork_lo_id					int not null check (gwork_lo_id>=0),  
    gwork_num_per_lo			int check (gwork_num_per_lo>=1 and gwork_num_per_lo<=4),
    primary key (gwork_gardener_id),	
    foreign key (gwork_gardener_id) references gardener(gardener_id) on delete cascade,
    foreign key (gwork_lo_id) references location(lo_id) on delete cascade    
);	
create table s_work			##1:N relationship where N on staff	
(
	swork_staff_id 			int check (swork_staff_id >0),
    swork_lo_id				int not null check (swork_lo_id>=0),
    swork_num_per_lo		int check (swork_num_per_lo>=1 and swork_num_per_lo<=15),
    primary key (swork_staff_id),
    foreign key (swork_staff_id) references staff(staff_id) on delete cascade,
    foreign key (swork_lo_id) references location(lo_id) on delete cascade
);
create table leads		##1:1 relationship total participation	
(
	lead_manager_id		int check (lead_manager_id >0),
    lead_lo_id			int not null check (lead_lo_id>=0),
    lead_num_per_lo		int default 0 check (lead_num_per_lo>=0 and lead_num_per_lo<=1),
    primary key (lead_lo_id),
    unique (lead_manager_id),
    foreign key (lead_manager_id) references manager(manager_id) on delete cascade,
    foreign key (lead_lo_id) references location(lo_id) on delete cascade
);
create table foundat	##M:N relationship total participation	need composite primary key
(
	foundat_p_id 		int check (foundat_p_id >0),
    foundat_lo_id 		int not null check (foundat_lo_id>=0),
	qty					int,
    primary key (foundat_p_id, foundat_lo_id),
    foreign key (foundat_p_id) references plant(p_id) on delete cascade on update cascade,
    foreign key (foundat_lo_id) references location(lo_id) on delete cascade on update cascade
);

create table specialized		##1:N relationship where N on gardener 
(
	specialized_p_id 			int check (specialized_p_id >0),
    specialized_gardener_id		int check (specialized_gardener_id >0),
    primary key (specialized_gardener_id), 
    foreign key (specialized_p_id) references plant(p_id) on delete cascade,
    foreign key (specialized_gardener_id) references gardener(gardener_id) on delete cascade
);

create table make		##1:N relationship where N is on Order
(
#	make_c_accnumber		varchar(8)	not null,
    make_c_accnumber 		int check (make_c_accnumber >0),
    make_order_id			int check (make_order_id >0),
	rating			numeric(1,0) default 1 check (rating in(1,2,3,4,5)) ,
    comments		varchar(200) default "unknown",
    datetimes		timestamp default current_timestamp,
    primary key (make_order_id),
    foreign key (make_order_id) references orders(order_id) on delete cascade,
    foreign key (make_c_accnumber) references customer(c_accnumber) on delete cascade
);

create table include		##M:N relationship btw plant and order ,need composite primary key change to 1:N where N on the order side
(
	include_p_id			int check (include_p_id >0),
    include_order_id		int check (include_order_id >0),
    #primary key (include_p_id, include_order_id),
    primary key (include_order_id),
    foreign key (include_p_id) references plant(p_id) on delete cascade on update cascade,
    foreign key (include_order_id) references orders(order_id) on delete cascade on update cascade
);
/*create table receive		##M:N relationship btw orders and shipper ,need composite primary key
(
	receive_order_id		varchar(8),
    receive_shipper_id		varchar(8),
    primary key(receive_order_id, receive_shipper_id),
    foreign key (receive_order_id) references orders(order_id) on delete cascade on update cascade,
    foreign key (receive_shipper_id) references shipper(shipper_id) on delete cascade on update cascade
);*/
/*create table deliver		##M:N relationship btw shipper and customer ,need composite primary key
(
	deliver_shipper_id		varchar(8),
    deliver_c_accnumber		varchar(8) not null,
    deliver_time			datetime	default null,
    primary key (deliver_shipper_id, deliver_c_accnumber),
    foreign key (deliver_shipper_id) references shipper(shipper_id) on delete cascade on update cascade,
    foreign key (deliver_c_accnumber) references customer(c_accnumber) on delete cascade on update cascade
);*/

use plant_database;

#INSERT INTO employee VALUES (e_id, e_gender, e_startdate, e_fname, e_lname, e_salary, e_birthdate, e_phone, md5(e_password));
INSERT INTO employee VALUES (1000, 'male', '2010-07-17', 'David', 'Reed', 19350, '1969-04-16', e_phone, md5('dqjff58q'));
INSERT INTO employee VALUES (1001, 'male', '2011-03-29', 'Martin', 'Turner', 18581, '1969-07-09', e_phone, md5('dwj8yr47'));
INSERT INTO employee VALUES (1002, 'male', '2009-11-23', 'Samuel', 'Ramirez', 19732, '1970-11-27', e_phone, md5('deq949fp'));
INSERT INTO employee VALUES (1003, 'male', '2019-12-18', 'Joshua', 'Collins', 21881, '1973-01-06', e_phone, md5('thb3a7e9'));
INSERT INTO employee VALUES (1004, 'male', '2006-03-17', 'Ronald', 'Bailey', 21267, '1974-03-15', e_phone, md5('kt39ncem'));
INSERT INTO employee VALUES (1005, 'male', '2011-04-21', 'Stephen', 'Adams', 18309, '1976-10-21', e_phone, md5('99a2m8vv'));
INSERT INTO employee VALUES (1006, 'male', '2014-12-26', 'Todd', 'Lee', 18311, '1978-10-10', e_phone, md5('c6cg6spm'));
INSERT INTO employee VALUES (1007, 'male', '2018-09-21', 'Nicholas', 'Patterson', 20455, '1982-11-17', e_phone, md5('u93fzz3b'));
INSERT INTO employee VALUES (1008, 'male', '2006-08-18', 'Larry', 'Miller', 21289, '1983-08-21', e_phone, md5('292qf8ks'));
INSERT INTO employee VALUES (1009, 'male', '2001-11-26', 'Brian', 'Carter', 20931, '1984-08-11', e_phone, md5('axe8e9sf'));
INSERT INTO employee VALUES (1010, 'female', '2005-02-13', 'Angela', 'Ross', 19071, '1986-11-05', e_phone, md5('t3gp8x5p'));
INSERT INTO employee VALUES (1011, 'female', '2001-01-22', 'Mary', 'Wood', 18230, '1987-04-03', e_phone, md5('fug2vm4v'));
INSERT INTO employee VALUES (1012, 'female', '2011-08-18', 'Diane', 'Powell', 18716, '1990-02-10', e_phone, md5('7vanbyuf'));
INSERT INTO employee VALUES (1013, 'female', '2013-04-12', 'Theresa', 'Coleman', 18607, '1990-11-09', e_phone, md5('8dd73fay'));
INSERT INTO employee VALUES (1014, 'female', '2002-07-27', 'Anna', 'Edwards', 18495, '1990-12-04', e_phone, md5('r88eff6f'));
INSERT INTO employee VALUES (1015, 'female', '2014-10-24', 'Janice', 'Butler', 19124, '1991-05-09', e_phone, md5('vyjk9dgx'));
INSERT INTO employee VALUES (1016, 'female', '2017-07-12', 'Frances', 'Hernandez', 18712, '1992-07-05', e_phone, md5('qhrtt5x6'));
INSERT INTO employee VALUES (1017, 'female', '2002-03-18', 'Rebecca', 'Garcia', 20855, '1993-04-27', e_phone, md5('qud6k596'));
INSERT INTO employee VALUES (1018, 'female', '2006-10-14', 'Ruby', 'Nelson', 18901, '1994-01-21', e_phone, md5('8btqcbmk'));
INSERT INTO employee VALUES (1019, 'female', '2016-11-08', 'Katherine', 'Diaz', 21748, '1996-01-06', e_phone, md5('qpe48vv5'));

#INSERT INTO gardener VALUES(gardener_id, gardener_bonus);
INSERT INTO gardener VALUES(1015, 1353);
INSERT INTO gardener VALUES(1008, 1179);
INSERT INTO gardener VALUES(1004, 1372);
INSERT INTO gardener VALUES(1002, 1166);
INSERT INTO gardener VALUES(1018, 1171);
INSERT INTO gardener VALUES(1014, 1377);
INSERT INTO gardener VALUES(1019, 1431);
INSERT INTO gardener VALUES(1001, 1334);

#INSERT INTO staff VALUES(staff_id, staff_level);
INSERT INTO staff VALUES(1007, 0);
INSERT INTO staff VALUES(1009, 0);
INSERT INTO staff VALUES(1017, 1);
INSERT INTO staff VALUES(1012, 1);
INSERT INTO staff VALUES(1003, 2);
INSERT INTO staff VALUES(1013, 2);
INSERT INTO staff VALUES(1016, 3);
INSERT INTO staff VALUES(1005, 3);
INSERT INTO staff VALUES(1011, 4);
INSERT INTO staff VALUES(1000, 4);

#INSERT INTO manager VALUES(manager_id, manager_commission);
-- INSERT INTO manager VALUES(1010, 15);
INSERT INTO manager VALUES(1006, 10);

#INSERT INTO customer VALUES (c_password, c_streetaddress, c_city, c_zipcode,  c_accnumber, c_email, c_fname, c_lname);
INSERT INTO customer VALUES (md5('4j73wmrt'), '8 School St.', 'Homestead', '33030',  1000, 'Kevin.Wood@email.com', 'Kevin', 'Wood');
INSERT INTO customer VALUES (md5('e92s98hy'), '811 Hawthorne Lan.', 'Baldwinsville', '13027',  1001, 'Cathy.C@email.com', 'Cathy', 'Clark');
INSERT INTO customer VALUES (md5('x26ka5p6'), '9338 Prince Cour.', 'Newport', '23601',  1002, 'Joseph.W@email.com', 'Joseph', 'Ward');
INSERT INTO customer VALUES (md5('gvcm7729'), '8881 Pin Oak St.', 'Allison', '15101',  1003, 'Larry.B@email.com', 'Larry', 'Barnes');
INSERT INTO customer VALUES (md5('rzee8ndh'), '210 Cherry Hill Cour.', 'Indiana', '15701',  1004, 'Norma.B@email.com', 'Norma', 'Bryant');
INSERT INTO customer VALUES (md5('2dyb2yy2'), '69 Hartford St.', 'Muskego', '53150',  1005, 'George.M@email.com', 'George', 'Miller');
INSERT INTO customer VALUES (md5('sah7t5ag'), '898 Princess Stree.', 'Elkridge', '21075',  1006, 'Tina.S@email.com', 'Tina', 'Simmons');
INSERT INTO customer VALUES (md5('ax6krcn7'), '53 Shirley Rd.', 'Cocoa', '32927',  1007, 'Justin.H@email.com', 'Justin', 'Howard');
INSERT INTO customer VALUES (md5('n8n3z85s'), '82 S. Morris Ave.', 'Clarksburg', '26301',  1008, 'Irene.D@email.com', 'Irene', 'Davis');
INSERT INTO customer VALUES (md5('854kbnuy'), '893 Mill Ave.', 'Mechanicsville', '23111',  1009, 'Jean.A@email.com', 'Jean', 'Anderson');

#INSERT INTO location VALUES ('lo_cityname', lo_areacode, lo_temperature, lo_id);
INSERT INTO location VALUES ('Gilroy', 669, 85, 0);
INSERT INTO location VALUES ('San Jose', 408, 85, 1);

#INSERT INTO plant VALUES(p_lifespan, 'p_name', p_stock, p_id, 'p_startdate', p_numbergardener, p_sold);
INSERT INTO plant VALUES(90, 'Aubrieta', 17, 1000, '2000-05-28', 1, 20);
INSERT INTO plant VALUES(150, 'Cal Poppy', 20, 1001, '2003-05-23', 1, 22);
INSERT INTO plant VALUES(180, 'Bld Orange', 25, 1002, '2004-10-29', 1, 24);
INSERT INTO plant VALUES(180, 'Meyer Lemon', 20, 1003, '2010-12-02', 1, 29);
INSERT INTO plant VALUES(365, 'Cedar', 25, 1004, '2014-11-19', 1, 22);
INSERT INTO plant VALUES(365, 'Cypress', 21, 1005, '2017-10-27', 1, 24);
INSERT INTO plant VALUES(100, 'Cauliflower', 22, 1006, '2018-05-12', 1, 22);
INSERT INTO plant VALUES(135, 'Zucchini', 18, 1007, '2017-06-26', 1, 24);

#INSERT INTO  tree VALUES(tr_grade, tr_id);
INSERT INTO  tree VALUES(8, 1004);
INSERT INTO  tree VALUES(3, 1005);

#INSERT INTO fruit VALUES(fr_season, fr_id);
INSERT INTO fruit VALUES('Spring', 1002);
INSERT INTO fruit VALUES('Spring', 1003);

#INSERT INTO vegetable VALUES(v_season, v_color, v_id, v_strength);
INSERT INTO vegetable VALUES('Fall', 'white', 1006, 'high');
INSERT INTO vegetable VALUES('Summer', 'green', 1007, 'low');

#INSERT INTO flower VALUES(fl_season, fl_color, fl_id, fl_duration);
INSERT INTO flower VALUES('Spring', 'purple', 1000, 90);
INSERT INTO flower VALUES('Winter', 'orange', 1001, 120);

#INSERT INTO g_work VALUES(gwork_gardener_id, gwork_lo_id, gwork_num_per_lo);
INSERT INTO g_work VALUES(1015, 0, 4);
INSERT INTO g_work VALUES(1008, 0, 4);
INSERT INTO g_work VALUES(1004, 0, 4);
INSERT INTO g_work VALUES(1002, 0, 4);
INSERT INTO g_work VALUES(1018, 1, 4);
INSERT INTO g_work VALUES(1014, 1, 4);
INSERT INTO g_work VALUES(1019, 1, 4);
INSERT INTO g_work VALUES(1001, 1, 4);
   
#INSERT INTO s_work VALUES(swork_staff_id, swork_lo_id, swork_num_per_lo);
INSERT INTO s_work VALUES(1007, 0, 5);
INSERT INTO s_work VALUES(1009, 0, 5);
INSERT INTO s_work VALUES(1017, 0, 5);
INSERT INTO s_work VALUES(1012, 0, 5);
INSERT INTO s_work VALUES(1003, 0, 5);
INSERT INTO s_work VALUES(1013, 1, 5);
INSERT INTO s_work VALUES(1016, 1, 5);
INSERT INTO s_work VALUES(1005, 1, 5);
INSERT INTO s_work VALUES(1011, 1, 5);
INSERT INTO s_work VALUES(1000, 1, 5);

#INSERT INTO leads VALUES(lead_manager_id, lead_lo_id, lead_num_per_lo);
-- INSERT INTO leads VALUES(1010, 0, 1);
INSERT INTO leads VALUES(1006, 1, 1);

#INSERT INTO foundat VALUES(foundat_p_id, foundat_lo_id, qty);
INSERT INTO foundat VALUES(1000, 0, 17);
INSERT INTO foundat VALUES(1001, 1, 20);
INSERT INTO foundat VALUES(1002, 0, 25);
INSERT INTO foundat VALUES(1003, 1, 20);
INSERT INTO foundat VALUES(1004, 0, 25);
INSERT INTO foundat VALUES(1005, 1, 21);
INSERT INTO foundat VALUES(1006, 0, 22);
INSERT INTO foundat VALUES(1007, 1, 18);

#INSERT INTO specialized VALUES(specialized_p_id, specialized_gardener_id);
INSERT INTO specialized VALUES(1000, 1015);
INSERT INTO specialized VALUES(1001, 1018);
INSERT INTO specialized VALUES(1002, 1008);
INSERT INTO specialized VALUES(1003, 1014);
INSERT INTO specialized VALUES(1004, 1004);
INSERT INTO specialized VALUES(1005, 1019);
INSERT INTO specialized VALUES(1006, 1002);
INSERT INTO specialized VALUES(1007, 1001);

#INSERT INTO make VALUES(make_c_accnumber, make_order_id, rating, comments, datetimes);

#INSERT INTO include VALUES(include_p_id, include_order_id);


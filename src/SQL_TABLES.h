#ifndef SQL_TABLES_H
#define SQL_TABLES_H
#include <QList>
#include "Table.h"

QList<TableElement> SQLTable_Human{
	TableElement("id",				Int, NULL_T, true),
	TableElement("Name",			String),
	TableElement("Surname",			String),
	TableElement("Patronimic",		String),
	TableElement("Gender",			Char),
	TableElement("Passport",		String),
	TableElement("Phone Number",	String)
};

QList<TableElement> SQLTable_Patient{
	TableElement("id",				Int, Human),
	TableElement("MedCard_Number",	Int),
	TableElement("Place of work",	String)
};

QList<TableElement> SQLTable_Doctor{
	TableElement("id",				Int, Human),
	TableElement("Office_number",	Int),
	TableElement("on_vac",			Bool),
	TableElement("on_work",			Bool)
};

QList<TableElement> SQLTable_Service{
	TableElement("id", 				Int, NULL_T, true),
	TableElement("Name", 			String),
	TableElement("Doctor_ID", 		Int, Doctor),
	TableElement("Price", 			Int),
	TableElement("Time_begin", 		Time),
	TableElement("Time_end", 		Time)
};

QList<TableElement> SQLTable_Contract{
	TableElement("id",				Int, NULL_T, true),
	TableElement("Customer",		Int, Patient),
	TableElement("Executor",		Int, Doctor),
	TableElement("Date",			Date)
};

QList<TableElement> SQLTable_MedDocs{
	TableElement("id",				Int, NULL_T, true),
	TableElement("Title",			String),
	TableElement("Doctor_ID",		Int, Doctor),
	TableElement("Recivier",		Int, Patient),
	TableElement("Date of ready",	Date),
	TableElement("Valif until",		Date),
	TableElement("Date of give",	Date),
};

QList<TableElement> SQLTable_Schedules{
	TableElement("id",				Int, NULL_T, true),
	TableElement("Service_id",		Int, Service, false),
	TableElement("Patient_id",		Int, Patient, false),
	TableElement("Date",			Date),
	TableElement("Time",			Time),
	TableElement("Status",			Bool)
};

#endif
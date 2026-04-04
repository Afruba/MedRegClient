#ifndef SQL_TABLES_HEADER
#define SQL_TABLES_HEADER
#include <QList>
#include "Table.h"
//									0			1			2		3			4		5			6			7
const QStringList TABLE_NAME = {"patient", "schedules", "meddoc", "service", "doctor", "contract", "human", "passport"}; 
const QStringList HUMAN_TABLE_NAME = {"Пациенты", "Расписание", "Мед Документы", "Услуги", "Доктора", "Контракты", "Люди", "Пасспорта"};
extern QList<TableElement> SQLTable_Patient;
extern QList<TableElement> SQLTable_Doctor;
extern QList<TableElement> SQLTable_Service;
extern QList<TableElement> SQLTable_Contract;
extern QList<TableElement> SQLTable_MedDocs;
extern QList<TableElement> SQLTable_Schedules;
extern QList<TableElement> SQLTable_Human;
extern QList<TableElement> SQLTable_Passport;

QList<TableElement>& get_sql_table(short t);
QList<TableElement>& get_sql_table(SQLTable t);
#endif
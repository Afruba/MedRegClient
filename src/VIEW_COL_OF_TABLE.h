#ifndef VIEW_COL_OF_TABLE_H
#define VIEW_COL_OF_TABLE_H
#include "Table.h"
#include <QList>
extern	QList<TableElement> HTable_Patient;
extern	QList<TableElement> HTable_Schedules;
extern	QList<TableElement> HTable_MedDocs;
extern	QList<TableElement> HTable_Service;
extern	QList<TableElement> HTable_Doctor;
extern	QList<TableElement> HTable_Contract;
extern	QList<TableElement> HTable_Human;
extern	QList<TableElement> HTable_Passport;

QList<TableElement>& get_h_table(short t);
QList<TableElement>& get_h_table(SQLTable t);
#endif
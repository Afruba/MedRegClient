#ifndef H_TABLE
#define H_TABLE
#include <QString>

enum DataType {Int = 0, String, Char, Bool, Telephone, Date, Time};
enum SQLTable {NULL_T, Patient, Doctor, Service, Contract, Human, MedDoc, Schedules, Pasports};

class TableElement{
public:
	TableElement():
		column_name("NONE"),
		column_type(Int),
		link_to_table(NULL_T),
		primaly_key(false){};

	TableElement(QString name, DataType type, SQLTable link_to_table=NULL_T, bool primaly=false):
		column_name(name),
		column_type(type),
		link_to_table(link_to_table),
		primaly_key(primaly_key){};
	QString		column_name;
	DataType	column_type;
	SQLTable	link_to_table;
	bool		primaly_key;
};
#endif
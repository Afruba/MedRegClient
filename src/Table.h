#ifndef H_TABLE
#define H_TABLE
#include <QString>

enum DataType {Int = 0, String, Char, Bool, Telephone, Date, Time, Timestamp, Text, StrInt};
enum SQLTable {NULL_T = -1, Patient, Schedules, MedDoc, Service, Doctor, Contract, Human, Passport};

struct SQLFilter
{
	QString method;
	QString cln_name;
	QString param;
	QString value;
};
const SQLFilter NULL_FILTER{"NULL", "", "", ""};
class TableElement{
public:
	TableElement():
		column_name("NONE"),
		column_type(Int),
		link_to_table(NULL_T),
		primaly_key(false){};

	TableElement(QString column_name, QString human_column_name, DataType type, SQLTable link_to_table=NULL_T, bool primaly_key=false, short max_char_count=1, bool is_nullable=false):
		column_name(column_name),
		human_column_name(human_column_name),
		column_type(type),
		link_to_table(link_to_table),
		primaly_key(primaly_key),
		max_char_count(max_char_count),
		is_nullable(is_nullable){};
	TableElement(QString name, DataType type, SQLTable link_to_table=NULL_T, bool primaly_key=false, short max_char_count=1, bool is_nullable=false):
		human_column_name(name),
		column_type(type),
		link_to_table(link_to_table),
		primaly_key(primaly_key),
		max_char_count(max_char_count),
		is_nullable(is_nullable){};

	QString		column_name;
	QString		human_column_name;
	DataType	column_type;
	SQLTable	link_to_table;
	bool		primaly_key;
	short		max_char_count;
	bool		is_nullable;
};
#endif

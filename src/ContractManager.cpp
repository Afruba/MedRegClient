#include "ContractManager.h"
#include "SQL_Manager.h"
#include "SQL_TABLES.h"
#include "Printer.h"
#include <QDate>
void add_new_contract(QString patient_id, pqxx::connection *sql_cn){
	QStringList data_for_contract{"default", "'Договор на оказание платных медицинских услуг'", patient_id, "1", '\''+QDate::currentDate().toString("yyyy-MM-dd")+'\''};
	inster_data_in_table(data_for_contract, TABLE_NAME[Contract], get_sql_table(Contract)[0].column_name, sql_cn);
}

void print_contract(QString contr_id, pqxx::connection *sql_cn){
	QStringList dataH; QStringList data; QStringList hum_data; QStringList pass_data;
	//Human-Patient Data
	get_row_data_from_tbl(data,		TABLE_NAME[Contract],	get_sql_table(Contract)[0].column_name, contr_id,	sql_cn);
	get_row_data_from_tbl(hum_data, TABLE_NAME[Patient],	get_sql_table(Patient)[0].column_name,	data[2], 	sql_cn);
	get_row_data_from_tbl(hum_data, TABLE_NAME[Human],		get_sql_table(Human)[0].column_name,	hum_data[3],sql_cn);
	get_row_data_from_tbl(pass_data,TABLE_NAME[Passport],	get_sql_table(Passport)[0].column_name, '\''+hum_data[5]+'\'',sql_cn);
	get_row_data_from_Htbl(dataH,	Contract,				get_sql_table(Contract)[0].column_name, contr_id,	sql_cn);
	//	FIO 	date 		date 		FIO 	life_addres number series issue_by phone doctor
	QStringList data_for_print{
		dataH[2], dataH[4], dataH[4], dataH[2], hum_data[7], pass_data[0], pass_data[1], pass_data[2], hum_data[6], dataH[3]
	};
	print_doc("service_agreement", data_for_print);
}
#ifndef SQL_MANAGER_H
#define SQL_MANAGER_H
#include <pqxx/pqxx>
#include <QList>
#include <QString>
void get_patient_data	(QList<QStringList>& buf, pqxx::connection *cx);
void get_schedules_data	(QList<QStringList>& buf, pqxx::connection *cx);
void get_meddoc_data	(QList<QStringList>& buf, pqxx::connection *cx);
void get_service_data	(QList<QStringList>& buf, pqxx::connection *cx);
void get_doctor_data	(QList<QStringList>& buf, pqxx::connection *cx);
void get_contract_data	(QList<QStringList>& buf, pqxx::connection *cx);
void get_data_from_sql_tbl	(QList<QStringList>& buf, QString table_name, QStringList columns, pqxx::connection *cx);
void get_row_data_from_tbl	(QStringList& buf, QString table_name, QString column_name, unsigned int object_id, pqxx::connection *cx);
bool inster_data_in_table	(QStringList data, QString table_name, pqxx::connection *cx);
void update_data_in_table	(QStringList data, QStringList columns, QString table_name, unsigned int object_id, pqxx::connection *cx);
void delte_row_from_table	(QString column_name, QString table_name, unsigned int object_id, pqxx::connection *cx);
bool check_user_account		(QString u_name, QString u_pass, pqxx::connection *cx);
void find_elements			(QStringList& buf, QString table_name, QString column_name, QString finder, pqxx::connection *cx);
int get_last_elenent_id_in_tbl(QString table_name, pqxx::connection *cx);
int get_last_elenent_id_in_tbl2(QString table_name, pqxx::connection *cx);
#endif
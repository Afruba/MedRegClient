#ifndef SQL_MANAGER_H
#define SQL_MANAGER_H
#include <pqxx/pqxx>
#include "Table.h"
#include <QList>
#include <QString>
void get_data_from_sql_tbl	(QList<QStringList>& buf, short tbl_id,pqxx::connection *cx, const QList<SQLFilter> &filters = {});
void get_data_from_sql_tbl	(QList<QStringList>& buf, QString table_name, QStringList columns, pqxx::connection *cx);
void get_row_data_from_tbl	(QStringList& buf, QString table_name, QString column_name, QString object_id, pqxx::connection *cx);
unsigned int inster_data_in_table	(QStringList data, QString table_name, QString column_id_name, pqxx::connection *cx);
void update_data_in_table	(QStringList data, QStringList columns, QString table_name, QString object_id, pqxx::connection *cx);
void delte_row_from_table	(QString column_name, QString table_name, QString object_id, pqxx::connection *cx);
bool check_user_account		(QString u_name, QString u_pass, pqxx::connection *cx);
void find_elements			(QStringList& buf, QString table_name, QString column_name, QString finder, pqxx::connection *cx);
int get_last_elenent_id_in_tbl(QString table_name, pqxx::connection *cx);
int get_last_elenent_id_in_tbl2(QString table_name, pqxx::connection *cx);
bool SQL_check_doctor_in_service(QString object_id, pqxx::connection *cx);
QString get_FIO_human(QString object_id, pqxx::connection *cx);
void get_all_date_schedules(QStringList &buf, pqxx::connection *cx);
#endif

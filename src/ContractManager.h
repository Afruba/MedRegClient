#ifndef CONTRACT_M_H
#define CONTRACT_M_H
#include <pqxx/pqxx>
#include <QString>
void add_new_contract(QString patient_id, pqxx::connection *sql_cn);
void print_contract(QString id, pqxx::connection *sql_cn);
#endif
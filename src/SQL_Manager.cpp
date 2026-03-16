#include "SQL_Manager.h"
#include <string>
#include <QDebug>
#include <QErrorMessage>
void fill_bufer(QList<QStringList>& buf, pqxx::result &r);

void get_patient_data(QList<QStringList>& buf, pqxx::connection *cx){
    pqxx::work tx(*cx);
    /*!
    select human.human_id, concat_ws(' ',human_name, human_surname, human_patronymic) as FIO, human.posport_number, passport.date_of_birth,patient.medcard_number, human.phone_number FROM patient
    inner JOIN human
        ON human.human_id = patient.patient_id
        inner JOIN passport
        ON human.posport_number = passport.passport_number
    */
    pqxx::result r = tx.exec("select human.human_id, concat_ws(' ',human_name, human_surname, human_patronymic) as FIO, human.posport_number, passport.date_of_birth,patient.medcard_number, human.phone_number FROM patient inner JOIN human ON human.human_id = patient.patient_id  inner JOIN passport ON human.posport_number = passport.passport_number ORDER BY human.human_id;");
    fill_bufer(buf, r);
}
void get_schedules_data(QList<QStringList>& buf, pqxx::connection *cx){
    pqxx::work tx(*cx);
    const char *sql_request =
    "select schedules.action_id,"
    "service.service_name,"
    "concat_ws(' ',human_name, human_surname, human_patronymic) as FIO_P,"
    "(select concat_ws(' ',human_name, human_surname, human_patronymic) as FIO_D from schedules"
    " inner join human on human.human_id = (select service.doctor_id from service where schedules.service_id = service.service_id )limit 1) as FIO_D, "
    "doctor.office,"
    "schedules.action_date, "
    "schedules.status FROM schedules "
        "inner JOIN human ON human.human_id = schedules.patient_id "
        "inner JOIN service ON service.service_id = schedules.service_id "
        "inner JOIN doctor ON doctor.doctor_id = service.doctor_id"
    " ORDER BY schedules.action_id;";
    pqxx::result r = tx.exec(sql_request);
    fill_bufer(buf, r);
}

void get_meddoc_data(QList<QStringList>& buf, pqxx::connection *cx){
    pqxx::work tx(*cx);
    const char *sql_request =
    "select meddoc.meddoc_id,"
    "meddoc.title,"
    "concat_ws(' ',human_name, human_surname, human_patronymic) as Doc,"
    "(select concat_ws(' ',human_name, human_surname, human_patronymic) FROM meddoc inner JOIN human ON human.human_id = meddoc.receiver) as Pat,"
    "meddoc.date_of_ready,"
    "meddoc.vlid_util,"
    "meddoc.date_of_give FROM meddoc inner JOIN human ON human.human_id = meddoc.doctor_id"
    " ORDER BY meddoc.meddoc_id;";
    pqxx::result r = tx.exec(sql_request);
    fill_bufer(buf, r);
}

void get_service_data(QList<QStringList>& buf, pqxx::connection *cx){
    pqxx::work tx(*cx);
    const char *sql_request =
    "select service.service_id,"
    "service.service_name,"
    "concat_ws(' ',human_name, human_surname, human_patronymic) as FIO_D,"
    "doctor.office,"
    "service.price,"
    "service.time_begin,"
    "service.time_end FROM service "
        "inner JOIN human ON human.human_id = service.doctor_id "
        "inner JOIN doctor ON doctor.doctor_id = service.doctor_id"
    " ORDER BY service.service_id;";
    pqxx::result r = tx.exec(sql_request);
    fill_bufer(buf, r);
}

void get_doctor_data(QList<QStringList>& buf, pqxx::connection *cx){
    pqxx::work tx(*cx);
    const char *sql_request =
    "select doctor.doctor_id,"
    "concat_ws(' ',human_name, human_surname, human_patronymic) as FIO_D,"
    "doctor.office,"
    "human.phone_number,"
    "doctor.on_vacation,"
    "doctor.on_work "
    "FROM doctor inner JOIN human ON human.human_id = doctor.doctor_id"
    " ORDER BY doctor.doctor_id;";
    pqxx::result r = tx.exec(sql_request);
    fill_bufer(buf, r);
}

void get_contract_data(QList<QStringList>& buf, pqxx::connection *cx){
    pqxx::work tx(*cx);
    const char *sql_request =
    "select contract.contract_id,"
    "contract.contract_title,"
    "concat_ws(' ',human_name, human_surname, human_patronymic) as Customer,"
    "(select concat_ws(' ',human_name, human_surname, human_patronymic) FROM contract inner JOIN human ON human.human_id = contract.executor) as Executor,"
    "contract.executor,"
    "contract.date_of_create "
    "FROM contract inner JOIN human ON human.human_id = contract.customer"
    " ORDER BY contract.contract_id;";
    pqxx::result r = tx.exec(sql_request);
    fill_bufer(buf, r);
}

void get_data_from_sql_tbl(QList<QStringList>& buf, QString table_name, QStringList columns, pqxx::connection *cx){
    pqxx::work tx(*cx);
    std::string sql_request = "select ";
    for(int i=0; i<columns.size(); i++){
        sql_request += columns[i].toStdString();
        if(i!=columns.size()-1) sql_request+=',';
    }
    sql_request += " from " + table_name.toStdString();
    sql_request += " ORDER BY " +columns[0].toStdString()+ ";";
    pqxx::result r = tx.exec(sql_request);
    fill_bufer(buf, r);
}

bool inster_data_in_table(QStringList data, QString table_name, pqxx::connection *cx){
    try{
        pqxx::work tx(*cx);
        std::string sql_request = "insert into " + table_name.toStdString() + " values (";
        short total = data.size();
        for(short i=0; i<total; i++){
            if(data[i]=="default") sql_request +="default";
            else sql_request += '\''+data[i].toStdString()+'\'';
            if(i!=total-1) sql_request += ", ";
        }
        sql_request+=");";
        qDebug()<<data;
        tx.exec(sql_request);
        tx.commit();
        return true;
    } catch(std::exception const &e){
        QErrorMessage em;
        em.showMessage(e.what());
        qDebug() << e.what();
        return false;
    }
}

void update_data_in_table(QStringList data, QStringList columns, QString table_name, unsigned int object_id, pqxx::connection *cx){
    try{
        qDebug()<<data;
        std::string sql_request = "UPDATE " + table_name.toStdString() + " SET ";
        for(short i=0; i<columns.size(); i++){
            if(data[i]!="default") {
                sql_request += columns[i].toStdString() +" = "+ '\''+data[i].toStdString()+'\'';
                if(i!=columns.size()-1) sql_request += ", ";
            }
        }
        sql_request += " WHERE " +columns[0].toStdString()+ " = " +std::to_string(object_id);

        pqxx::work tx(*cx);
        tx.exec(sql_request);
        tx.commit();
    } catch(std::exception const &e){
        QErrorMessage em;
        em.showMessage(e.what());
        qDebug() << e.what();
    }
}

void delte_row_from_table (QString column_name, QString table_name, unsigned int object_id, pqxx::connection *cx){
    try{
        std::string sql_request = "DELETE FROM " +table_name.toStdString()+ " WHERE " +column_name.toStdString()+ " = \'" +std::to_string(object_id)+ "\';";
        pqxx::work tx(*cx);
        tx.exec(sql_request);
        tx.commit();
    } catch(std::exception const &e){
        QErrorMessage em;
        em.showMessage(e.what());
        qDebug() << e.what();
    }
}

bool check_user_account(QString u_name, QString u_pass, pqxx::connection *cx){
    try{
        std::string sql_request = "SELECT * FROM users WHERE user_name = \'" +u_name.toStdString()+ "\' AND user_password = \'" +u_pass.toStdString()+"\';";
        pqxx::work tx(*cx);
        pqxx::result r = tx.exec(sql_request);
        return r.size()!=0;
    } catch(std::exception const &e){
        QErrorMessage em;
        em.showMessage(e.what());
        qDebug() << e.what();
    }
}

int get_last_elenent_id_in_tbl(QString table_name, pqxx::connection *cx){
    try{
        pqxx::work tx(*cx);
        std::string id_name = table_name.toStdString()+"_id";
        std::string sql_request = "select " +id_name+ " from " +table_name.toStdString()+ " order by " +id_name+ " desc limit 1";

        pqxx::field f = tx.exec(sql_request).one_field();
        int id=0; id = f.as(id);
        return id;
        // auto id = r.as<int>();
        // return std::get<0>(id); 
    } catch(std::exception const &e){
        qDebug() << "[E_SQL]: " << e.what();
        return -1;
    }
}

void get_row_data_from_tbl  (QStringList& buf, QString table_name, QString column_name, unsigned int object_id, pqxx::connection *cx){
    try{
        pqxx::work tx(*cx);
        const std::string sql_request = "select * from " +table_name.toStdString()+ " where " +column_name.toStdString()+ " = \'" +std::to_string(object_id)+ "\';";
        pqxx::result r = tx.exec(sql_request);

        buf.clear();
        for (auto const &row_ref: r)
        {
            for (auto const &field_ref: row_ref) buf.push_back(QString(field_ref.c_str()));
        }
    } catch(std::exception const &e){
        qDebug() << "[E_SQL]: " << e.what();
    }
}

void find_elements(QStringList& buf, QString table_name, QString column_name, QString finder, pqxx::connection *cx){
    try{
        pqxx::work tx(*cx);
        const std::string sql_request = "select * from " +table_name.toStdString()+ " where " +column_name.toStdString()+ " = \'" +finder.toStdString()+ "\';";
        pqxx::result r = tx.exec(sql_request);

        buf.clear();
        for (auto const &row_ref: r)
        {   QString s = "";
            for (auto const &field_ref: row_ref) s += (QString(field_ref.c_str()))+" ";
            buf.push_back(s);
        }
    } catch(std::exception const &e){
        qDebug() << "[E_SQL]: " << e.what();
    }
}

int get_last_elenent_id_in_tbl2(QString table_name, pqxx::connection *cx){
    try{
        pqxx::work tx(*cx);
        std::string pkey_name = table_name.toStdString()+"_pkey";

        cx->prepare(pkey_name, "insert into human values(default, '','','','',) select * from human;"); 
        pqxx::result r = tx.exec_prepared(pkey_name);
        // pqxx::field f = r.at(0,0).as<int>; 
        // int id=0; id = f.as(id);
        for(int i=0; i< r.size();i++){
            qDebug() << "0:0" <<r[i][0].c_str();
        }
        
        // int id = r.at(0,0).as<int>; 
        return 0;
        // auto id = r.as<int>();
        // return std::get<0>(id); 
    } catch(std::exception const &e){
        qDebug() << "[E_SQL]: " << e.what();
        return -1;
    }
}

void fill_bufer(QList<QStringList>& buf, pqxx::result &r){
    buf.clear();
    for (auto const &row_ref: r)
    {
        QStringList data_in_row = {};
        for (auto const &field_ref: row_ref) data_in_row.push_back(QString(field_ref.c_str()));
        buf.push_back(data_in_row);
    }
}
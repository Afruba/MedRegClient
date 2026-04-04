#include "SQL_Manager.h"
#include <fstream>
#include <string>
#include <QDebug>
#include <QMessageBox>
void fill_bufer(QList<QStringList>& buf, pqxx::result &r);
void show_error_msg(const char* msg);
std::string load_request(std::string file_name);

void get_data_from_sql_tbl(QList<QStringList>& buf, short tbl_id,pqxx::connection *cx, const QList<SQLFilter> &filters){
    try{
        std::string file_name;
        switch(tbl_id){
        case 0: file_name="patient.sreq";   break;
        case 1: file_name="schedules.sreq"; break;
        case 2: file_name="meddoc.sreq";    break;
        case 3: file_name="service.sreq";   break;
        case 4: file_name="doctor.sreq";    break;
        case 5: file_name="contract.sreq";  break;
        case 6: file_name="human.sreq";     break;
        case 7: file_name="passport.sreq";  break;
        }
        std::string sql_request = load_request(file_name);
        if(filters.size()!=0){
            for(auto f: filters){
                f.method = QChar(' ') + f.method + QChar(' ');
                QString inserter = f.method + f.cln_name + f.param + f.value;
                sql_request.insert(sql_request.size()-1, inserter.toStdString());
                qDebug()<<QString::fromStdString(sql_request);
            }
        }
        pqxx::work tx(*cx);
        pqxx::result r = tx.exec(sql_request);
        fill_bufer(buf, r);
        qDebug()<<"[*] Data is load";
    } catch(std::exception const &e){
        show_error_msg(e.what()); 
    }
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

unsigned int inster_data_in_table(QStringList data, QString table_name, QString id_column_name, pqxx::connection *cx){
    try{
        pqxx::work tx(*cx);
        std::string sql_request = "insert into " + table_name.toStdString() + " values (";
        short total = data.size();
        for(short i=0; i<total; i++){
            if(data[i]=="default") sql_request +="default";
            else sql_request += data[i].toStdString();
            if(i!=total-1) sql_request += ", ";
        }
        sql_request += (") RETURNING " + id_column_name.toStdString() + ";");

        pqxx::field f = tx.exec(sql_request).one_field();
        tx.commit();
        int last_id=0; last_id = f.as(last_id);

        return last_id;
    } catch(std::exception const &e){
        show_error_msg(e.what());
        return 0;
    }
}

void update_data_in_table(QStringList data, QStringList columns, QString table_name, QString object_id, pqxx::connection *cx){
    try{
        std::string sql_request = "UPDATE " + table_name.toStdString() + " SET ";
        for(short i=0; i<columns.size(); i++){
            if(data[i]!="default") {
                sql_request += columns[i].toStdString() +" = "+ data[i].toStdString();
                if(i!=columns.size()-1) sql_request += ", ";
            }
        }
        sql_request += " WHERE " +columns[0].toStdString()+ " = " +object_id.toStdString();

        pqxx::work tx(*cx);
        tx.exec(sql_request);
        tx.commit();
    } catch(std::exception const &e){
        show_error_msg(e.what()); 
    }
}

void delte_row_from_table (QString column_name, QString table_name, QString object_id, pqxx::connection *cx){
    try{
        std::string sql_request = "DELETE FROM " +table_name.toStdString()+ " WHERE " +column_name.toStdString()+ " = " +object_id.toStdString()+ ";";
        pqxx::work tx(*cx);
        tx.exec(sql_request);
        tx.commit();
    } catch(std::exception const &e){
        show_error_msg(e.what()); 
    }
}

bool check_user_account(QString u_name, QString u_pass, pqxx::connection *cx){
    try{
        std::string sql_request = "SELECT * FROM users WHERE user_name = \'" +u_name.toStdString()+ "\' AND user_password = " +u_pass.toStdString()+";";
        pqxx::work tx(*cx);
        pqxx::result r = tx.exec(sql_request);
        return r.size()!=0;
    } catch(std::exception const &e){
        show_error_msg(e.what()); 
    }
}

void get_row_data_from_tbl  (QStringList& buf, QString table_name, QString column_name, QString object_id, pqxx::connection *cx){
    try{
        pqxx::work tx(*cx);
        const std::string sql_request = "select * from " +table_name.toStdString()+ " where " +column_name.toStdString()+ "=" +object_id.toStdString()+ ";";
        pqxx::result r = tx.exec(sql_request);

        buf.clear();
        for (auto const &row_ref: r)
        {
            for (auto const &field_ref: row_ref) buf.push_back(QString(field_ref.c_str()));
        }
    } catch(std::exception const &e){
        show_error_msg(e.what()); 
    }
}

void find_elements(QStringList& buf, QString table_name, QString column_name, QString finder, pqxx::connection *cx){
    try{
        pqxx::work tx(*cx);
        const std::string sql_request = "select * from " +table_name.toStdString()+ " where " +column_name.toStdString()+ "=" +finder.toStdString()+ ";";
        pqxx::result r = tx.exec(sql_request);

        buf.clear();
        for (auto const &row_ref: r)
        {   QString s = "";
            for (auto const &field_ref: row_ref) s += (QString(field_ref.c_str()))+" ";
            buf.push_back(s);
        }
    } catch(std::exception const &e){
        show_error_msg(e.what()); 
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

std::string load_request(std::string file_name){
    std::ifstream file("request/"+file_name);
    if(!file.is_open()){
        qDebug()<<"[E] File dont find";
        return ""; 
    }
    std::string sql_request = "";
    std::string buf="";
    while(!file.eof()){
        file>>buf;
        sql_request += " " + buf;
    }
    file.close();
    return sql_request;
}

bool SQL_check_doctor_in_service(QString object_id, pqxx::connection *cx){
   try{
        pqxx::work tx(*cx);
        std::string sql_request = load_request("doctor.onVac_service.sreq");
        sql_request.insert(sql_request.size()-1, object_id.toStdString());
        pqxx::field f  = tx.exec(sql_request).one_field();
        tx.commit();
        std::string on_vacation = f.c_str();
        qDebug()<<QString::fromStdString(on_vacation);
        return on_vacation[0]=='t';
    }catch(std::exception const &e){
        show_error_msg(e.what()); 
        return true;
    }

}
QString get_FIO_human(QString object_id, pqxx::connection *cx){
    pqxx::work tx(*cx);
    std::string sql_request = load_request("human.FIO.sreq");
    sql_request.insert(sql_request.size()-1, object_id.toStdString());
    pqxx::field f  = tx.exec(sql_request).one_field();
    tx.commit();
    return QString(f.c_str());
}
void get_all_date_schedules(QStringList &buf, pqxx::connection *cx){
    pqxx::work tx(*cx);
    pqxx::result r = tx.exec(load_request("schedules_all_day.sreq"));

    buf.clear();
    for (auto const &row_ref: r)
    {
        for (auto const &field_ref: row_ref){
            QString text = QString(field_ref.c_str());
            QString Mon = text.mid(5,3);//text[3]+text[4]+text[5];
            if     (QString::compare(Mon,"Jan")) Mon="Янв";
            else if(QString::compare(Mon,"Feb")) Mon="Фев";
            else if(QString::compare(Mon,"Mar")) Mon="Мар";
            else if(QString::compare(Mon,"Apr")) Mon="Апр";
            else if(QString::compare(Mon,"May")) Mon="Май";
            else if(QString::compare(Mon,"Jun")) Mon="Июн";
            else if(QString::compare(Mon,"Jul")) Mon="Июл";
            else if(QString::compare(Mon,"Фгп")) Mon="Авг";
            else if(QString::compare(Mon,"Sep")) Mon="Сен";
            else if(QString::compare(Mon,"Okt")) Mon="Окт";
            else if(QString::compare(Mon,"Nov")) Mon="Ноя";
            else if(QString::compare(Mon,"Dec")) Mon="Дек";
            text = QString(' ')+text.right(2)+QString(' ')+Mon+QString(' ')+text.left(4);
            buf.push_back(text);
        }
    }
}

void show_error_msg(const char* msg){
    QMessageBox messageBox;
    //messageBox.critical(nullptr, qmsg);
    messageBox.setText(msg);
    messageBox.setFixedSize(500,200);
    messageBox.setIcon(QMessageBox::Critical);
    messageBox.exec();
}

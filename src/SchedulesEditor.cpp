#include "SchedulesEditor.h"
#include "SQL_Manager.h"
#include "SQL_TABLES.h"
#include "SearchWindow.h"
#include "Printer.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QLabel>
#include <QMessageBox>
#include <QDebug>
#include <QLocale>
#include <QTime>



SchedulesEditor::SchedulesEditor(bool add_mode, pqxx::connection *sql_cn, QString default_date, QString id):
add_mode(add_mode), sql_cn(sql_cn), object_id(id){
	QList<TableElement> sql_tbl_obj = get_sql_table(Schedules);
	QVBoxLayout *layout = new QVBoxLayout(this);
	if (add_mode)	setWindowTitle("Добавить запись");
	else			setWindowTitle("Редактирование");
	
	QStringList data{};
	if(!add_mode){
		get_row_data_from_tbl(data, TABLE_NAME[Schedules], sql_tbl_obj[0].column_name, id, sql_cn);
		qDebug()<<"UPDATE DATA: "<<data;
	}
	EditField *field;
	field = new EditField(TableElement("Дата",	Date), sql_cn);
	layout -> addWidget(field);
	all_fields.push_back(field);
	QLocale locale(QLocale::English, QLocale::UnitedStates);
	if(add_mode) field->set_value(locale.toDate(default_date,"yyyy MMM dd"));
	else field->set_value(locale.toDate(data[3].left(10),"yyyy-MM-dd"));
	
	field = new EditField(sql_tbl_obj[2], sql_cn); //Пациент
	if(!add_mode) field->set_value(data[2]);
	layout -> addWidget(field);
	all_fields.push_back(field);

	field = new EditField(sql_tbl_obj[1], sql_cn); //Услуга
	connect(field, &InputField::edit_finished, this, &SchedulesEditor::check_service);
	layout -> addWidget(field);
	all_fields.push_back(field);

	l_doctor = new QLabel("Доктор: ");
	l_time = new QLabel("Начало Приёма: --:--\nКонец Приёма: --:--");
	l_stime = new  QLabel("Выбранное время: --:--");
	QWidget *wdg_bt_times = new QWidget();
	gl_times = new QGridLayout(wdg_bt_times); 



	if (add_mode) bt_apply = new QPushButton("Добавить");
	else bt_apply = new QPushButton("Изменить");
	connect(bt_apply, &QPushButton::clicked, this, &SchedulesEditor::CheckAllFields);
	bt_apply -> setEnabled(false);

	

	layout -> addWidget(l_doctor);
	layout -> addWidget(l_time);
	layout -> addWidget(l_stime);
	if(!add_mode){
		field = new EditField(sql_tbl_obj[4], sql_cn);
		field->set_value(data[4]);
		layout -> addWidget(field);
		all_fields.push_back(field);
	}
	layout -> addWidget(wdg_bt_times);
	layout -> addWidget(bt_apply);

	if(!add_mode){
		select_time(data[3].mid(11,5));
		all_fields[2]->set_value(data[1]); //UPDATE service_field
		
		QPushButton *del_button	 = new QPushButton("Удалить");
		connect(del_button, &QPushButton::clicked, this, &SchedulesEditor::delete_data);

		QPushButton *bt_print = new QPushButton("Напечатать толон");
		connect(bt_print, &QPushButton::clicked, this, &SchedulesEditor::print_ticket);
		layout -> addWidget(del_button);
		layout -> addWidget(bt_print);
	} else {
		QCheckBox *cb_print = new QCheckBox("Напечатать толон");
		connect(cb_print, &QCheckBox::clicked, this, &SchedulesEditor::print_ticket);
		layout -> addWidget(cb_print);
	}

	show();
}

SchedulesEditor::~SchedulesEditor(){
	QLayoutItem *li = 0;
	while ((li = layout()->takeAt(0)) != nullptr){
		delete li->widget();
		delete li;
	}
	delete layout();
}

void SchedulesEditor::closeEvent (QCloseEvent *event){
	event->accept();
	delete this;
}

void SchedulesEditor::check_service(QString id){
	bt_apply -> setEnabled(false);
	if(gl_times->count()!=0){ //Clear gl_times
		QLayoutItem *li = 0;
		while( (li=gl_times->takeAt(0)) != nullptr){
			delete li->widget();
			delete li;
		}
	}
	QStringList data{};
	get_row_data_from_tbl(data, TABLE_NAME[Service], get_sql_table(Service)[0].column_name, id, sql_cn);
	if(data.size()!=0){
		QLocale locale(QLocale::English, QLocale::UnitedStates);
		QString date = locale.toString(all_fields[0]->get_date(), "yyyy MMM dd");
		QTime buf = QTime::fromString(data[6], "HH:mm:ss");
		unsigned int sec_delay = buf.hour()*3600 + buf.minute()*60 + buf.second();

		QStringList doctor_data{};
		get_row_data_from_Htbl(doctor_data, Doctor, get_sql_table(Doctor)[0].column_name, data[2], sql_cn);
		QString text = QString("Доктор: ") +doctor_data[1];
		if(doctor_data[4]=="t"){
			doctor_no_vac = false;
			text += "\n[!] ДОКТОР В ОТПУСКЕ [!]";
		}
		l_doctor -> setText(text);

		get_row_data_from_Htbl(data, Service, get_sql_table(Service)[0].column_name, id, sql_cn);
		text = QString("Начало Приёма: ") +data[5]+ QString("\nКонец Приёма: ") +data[6];
		l_time -> setText(text);
		QTime t_begin = QTime::fromString(data[5], "HH:mm:ss");
		QTime t_end = QTime::fromString(data[6], "HH:mm:ss");

		get_all_action_time_for_day(data, id, '\''+date+'\'', sql_cn);

		short gl_x = 0;
		short gl_y = 0;
		while(t_begin<t_end){
			bool flag = false;
			QString t = t_begin.toString("HH:mm:ss").left(5);
			t_begin = t_begin.addSecs(sec_delay);
			
			for(int i=0; i<data.size(); i++){
				if(data[i]==t){flag=(t!=selected_time); break;}
			}
			QPushButton *btn = new QPushButton(t);
			if(flag) btn->setEnabled(false);
			else connect( btn, &QPushButton::clicked, this , [=](){select_time(t);} );
			gl_times -> addWidget(btn, gl_y, gl_x);
			if(++gl_x==5) {gl_x=0; gl_y++;}
			
		}

	}
	else{
		l_doctor -> setText("Нет такой услуги ");
		l_time -> setText("Начало Приёма: --:--\nКонец Приёма: --:--");
		l_stime -> setText("Выбранное время: --:--");
	}
}

void SchedulesEditor::select_time(QString t){
	selected_time = t;
	qDebug()<<selected_time;
	l_stime -> setText(QString("Выбранное время: ")+t);
	bt_apply -> setEnabled(true);
}

void SchedulesEditor::CheckAllFields(){
	bool result = true;
	result *= all_fields.at(1) -> CheckInput();
	result *= all_fields.at(2) -> CheckInput();
	result *= doctor_no_vac;


	if (result){
		QMessageBox::StandardButton reply;
			QString text = "Подтвердить?";
			reply = QMessageBox::question(this, "Подтверджение", text,  QMessageBox::Yes|QMessageBox::No);
		if (reply == QMessageBox::Yes) {
			if(add_mode) AddToDB();
			else ApplyChange();
		}
	}

}

void SchedulesEditor::ApplyChange(){
	QStringList data{};
	QStringList columns{};

	data.push_back("default"); //action_id
	data.push_back(all_fields[2] -> get_value()); //service_id
	data.push_back(all_fields[1] -> get_value()); //patient_id
	data.push_back('\''+ all_fields[0] -> get_value(false) +' '+ selected_time +'\''); //action_date
	data.push_back(all_fields[3] -> get_value()); //status
	QList<TableElement> tbl = get_sql_table(Schedules);
	for(int i=0; i<tbl.size(); i++){
		columns.push_back(tbl[i].column_name);
	}
	update_data_in_table(data, columns, TABLE_NAME[Schedules], object_id, sql_cn);
	emit data_is_changed();
	close();
	
}

void SchedulesEditor::AddToDB(){
	QStringList data{};
	data.push_back("default"); //action_id
	data.push_back(all_fields[2] -> get_value()); //service_id
	data.push_back(all_fields[1] -> get_value()); //patient_id
	data.push_back('\''+ all_fields[0] -> get_value(false) +' '+ selected_time +'\''); //action_date
	data.push_back("false"); //status

	int last_id = inster_data_in_table(data, TABLE_NAME[Schedules], get_sql_table(Schedules)[0].column_name, sql_cn);
	if(print_late_ticket && add_mode){
		add_mode=false;
		object_id = QString("%1").arg(last_id);
		print_ticket();
	}
	emit data_is_changed();
	close();
}	

void SchedulesEditor::delete_data(){
	QMessageBox::StandardButton reply;
		QString text = "Подтвердить?";
		reply = QMessageBox::question(this, "Подтверджение", text,  QMessageBox::Yes|QMessageBox::No);
	if (reply == QMessageBox::Yes) {
		delte_row_from_table(get_sql_table(Schedules)[0].column_name, TABLE_NAME[Schedules], object_id, sql_cn);
		emit data_is_changed();
		close();
	}
}

void SchedulesEditor::print_ticket(bool v){
	if(add_mode){
		print_late_ticket = v;
	} else {
		QStringList data{};
		QStringList buf;
		get_row_data_from_Htbl(buf, Patient, get_sql_table(Patient)[0].column_name, all_fields[1] -> get_value(), sql_cn);
		data.push_back(buf[1]); //Patient
		data.push_back(buf[4]); //Med Card Number

		get_row_data_from_Htbl(buf, Schedules, get_sql_table(Schedules)[0].column_name, object_id, sql_cn);
		data.push_back(buf[5]); //Cabinet
		data.push_back(buf[6]);	//Data
		data.push_back(buf[7]); //Time
		data.push_back(buf[4]); //Doctor
		print_doc("admission_ticket", data);
	}
}

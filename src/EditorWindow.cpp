#include "EditorWindow.h"
#include "SQL_Manager.h"
#include "SQL_TABLES.h"
#include "SearchWindow.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QDebug>





EditorWindow::EditorWindow(SQLTable t, bool add_mode, pqxx::connection *sql_cn, QString id):
add_mode(add_mode), sql_cn(sql_cn), object_id(id), sql_tbl(t){
	QVBoxLayout *layout = new QVBoxLayout(this);
	if (add_mode)	setWindowTitle("Добавление элемента");
	else			setWindowTitle("Редактирование");
	
	QStringList data{};
	if(!add_mode) get_row_data_from_tbl(data, TABLE_NAME[t], get_sql_table(t)[0].column_name, id, sql_cn);

	QList<TableElement> sql_tbl_obj = get_sql_table(t);
	for(short i=0; i<sql_tbl_obj.size(); i++){
		EditField *field = new EditField(sql_tbl_obj[i], sql_cn);
		all_fields.push_back(field);
		if(!add_mode) field -> set_value(data[i]);
		layout -> addWidget(field);
	}
	QPushButton *apply_button;
	if (add_mode) apply_button = new QPushButton("Добавить");
	else apply_button = new QPushButton("Изменить");
	connect(apply_button, &QPushButton::pressed, this, &EditorWindow::CheckAllFields);
	layout -> addWidget(apply_button);

	if(!add_mode){
		QPushButton *del_button	 = new QPushButton("Удалить");
		connect(del_button, &QPushButton::pressed, this, &EditorWindow::delete_data);
		layout -> addWidget(del_button);
	}

	show();
}

EditorWindow::~EditorWindow(){
	QLayoutItem *li = 0;
	while ((li = layout()->takeAt(0)) != nullptr){
		delete li->widget();
		delete li;
	}
	delete layout();
}

void EditorWindow::closeEvent (QCloseEvent *event){
	event->accept();
	delete this;
}

void EditorWindow::CheckAllFields(){
	bool result = true;
	for(short i=0; i<all_fields.size(); i++){
		bool b = all_fields.at(i) -> CheckInput();
		all_fields.at(i) -> SetIndicator(b);
		result *= b;
	}
	if (result && sql_tbl==Schedules){
		result = !SQL_check_doctor_in_service(all_fields[1]->get_value(), sql_cn);
		if(!result){
			QStringList data;
			get_row_data_from_tbl(data, "service", "service_id", all_fields[1]->get_value(), sql_cn);
			QString buf = data[2];
			get_row_data_from_tbl(data, "doctor", "doctor_id", buf, sql_cn);
			buf = data[4];
			buf = tr("Врач ") +get_FIO_human(buf, sql_cn)+ tr(" (ID:") +buf+ tr(") находиться в отпуске");

			QMessageBox MB(QMessageBox::Warning, "Ошиибка", buf);
			MB.exec();
		}
	}

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

void EditorWindow::ApplyChange(){
	QStringList data{};
	QStringList columns{};
	for(int i=0; i<all_fields.size(); i++){
		qDebug()<<i;
		data.push_back(all_fields[i]->get_value());
		qDebug()<<i;
		columns.push_back(get_sql_table(sql_tbl)[i].column_name);
	}
	update_data_in_table(data, columns, TABLE_NAME[sql_tbl], object_id, sql_cn);
	emit data_is_changed();
	close();
	
}

void EditorWindow::AddToDB(){
	QStringList data{};
	for(int i=0; i<all_fields.size(); i++){
		data.push_back(all_fields[i] -> get_value());
	}
	unsigned int last_id = 0;
	last_id = inster_data_in_table(data, TABLE_NAME[sql_tbl], get_sql_table(sql_tbl)[0].column_name, sql_cn);
	qDebug()<<"LAST_ID: " <<last_id;
	emit data_is_changed();
	close();
}	

void EditorWindow::delete_data(){
	QMessageBox::StandardButton reply;
		QString text = "Подтвердить?";
		reply = QMessageBox::question(this, "Подтверджение", text,  QMessageBox::Yes|QMessageBox::No);
	if (reply == QMessageBox::Yes) {
		delte_row_from_table(get_sql_table(sql_tbl)[0].column_name, TABLE_NAME[sql_tbl], object_id, sql_cn);
		emit data_is_changed();
		close();
	}
}

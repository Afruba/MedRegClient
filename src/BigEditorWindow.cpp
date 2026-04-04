#include "BigEditorWindow.h"
#include "SQL_Manager.h"
#include "SQL_TABLES.h"
#include "SearchWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QCheckBox>
#include <QDebug>
#include <QLineEdit>



const short PASSPORT_ELEMENT_CNT = SQLTable_Passport.size();

BigEditorWindow::BigEditorWindow(SQLTable t, bool add_mode, pqxx::connection *sql_cn, QString id):
add_mode(add_mode), sql_cn(sql_cn), sql_tbl(t), use_passport(true){
	QHBoxLayout *layout = new QHBoxLayout(this);

	if (add_mode){
		setWindowTitle("Добавление элемента");
		add_edit_column(Passport);
		add_edit_column(Human);
	}
	else{
		setWindowTitle("Редактирование");
		QStringList data;
		QList<TableElement> sql_tbl_obj = get_sql_table(t);
		get_row_data_from_tbl(data, TABLE_NAME[t], get_sql_table(t)[0].column_name, id, sql_cn);
		get_row_data_from_tbl(data, TABLE_NAME[Human], "human_id", data[data.size()-1], sql_cn);
		add_edit_column(Passport, data[5]);
		add_edit_column(Human, data[0]);
		object_id.push_back(data[5]);
		object_id.push_back(data[0]);
		object_id.push_back(id);
	}
	add_edit_column(t, id);


	QPushButton *apply_button;
	if (add_mode){
		apply_button = new QPushButton("Добавить");
		QCheckBox *cb = new QCheckBox("Есть паспорт");
		connect(cb, &QCheckBox::clicked, this, &BigEditorWindow::disable_passport);
		layout -> addWidget(cb);

		QLineEdit *pass_num_le = all_fields[0] -> get_line_edit();
		QLineEdit *h_pass_num_le = all_fields[12] -> get_line_edit();
		connect(pass_num_le, &QLineEdit::textChanged, h_pass_num_le, &QLineEdit::setText);
	}
	else apply_button = new QPushButton("Изменить");
	connect(apply_button, &QPushButton::pressed, this, &BigEditorWindow::CheckAllFields);
	layout -> addWidget(apply_button);


	show();
}

BigEditorWindow::~BigEditorWindow(){
	QLayoutItem *li = 0;
	while ((li = layout()->takeAt(0)) != nullptr){
		QLayoutItem *li_c = 0;
		while ((li_c = li->layout()->takeAt(0)) != nullptr){
			delete li_c->widget();
			delete li_c;
		}

		delete li->widget();
		delete li;
	}
	delete layout();
}

void BigEditorWindow::closeEvent (QCloseEvent *event){
	event->accept();
	delete this;
}

void BigEditorWindow::add_edit_column(SQLTable t, QString id){
	QWidget *holder = new QWidget();
	QVBoxLayout *h_layout = new QVBoxLayout(holder);

	QStringList data{};
	if(!add_mode) get_row_data_from_tbl(data, TABLE_NAME[t], get_sql_table(t)[0].column_name, id, sql_cn);
	QList<TableElement> t_obj = get_sql_table(t);

	for(int i=0; i<t_obj.size(); i++){
		InputField *w = 0;
		if(add_mode) w = new InputField(t_obj[i]);
		else {
			w = new EditField(t_obj[i], sql_cn);
			w -> set_value(data[i]);
		}
		all_fields.push_back(w);
		h_layout->addWidget(w);
	}
	layout()->addWidget(holder);
}

void BigEditorWindow::CheckAllFields(){
	bool result = true;
	for(short i=0; i<all_fields.size(); i++){
		bool b = all_fields.at(i) -> CheckInput();
		all_fields.at(i) -> SetIndicator(b);
		result *= b;
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

void BigEditorWindow::ApplyChange(){
	QStringList data{};
	QStringList columns{};
	QList<TableElement> sql_tbl_obj = get_sql_table(Passport);
	short offset = 0;
	if(use_passport){

		for(int i=0; i<sql_tbl_obj.size(); i++){
			data.push_back(all_fields[i]->get_value());
			columns.push_back(sql_tbl_obj[i].column_name);
		}
		update_data_in_table(data, columns, TABLE_NAME[Passport], object_id[2], sql_cn);
		data.clear();
	}
	offset = sql_tbl_obj.size();

	sql_tbl_obj = get_sql_table(Passport);
	data.clear(); columns.clear();
	for(int i=0; i<sql_tbl_obj.size(); i++){
		data.push_back(all_fields[offset+i]->get_value());
		columns.push_back(sql_tbl_obj[offset+i].column_name);
	}
	update_data_in_table(data, columns, TABLE_NAME[Human], object_id[1], sql_cn);
	data.clear();
	offset += sql_tbl_obj.size();

	sql_tbl_obj = get_sql_table(sql_tbl);
	data.clear(); columns.clear();
	for(int i=0; i<sql_tbl_obj.size(); i++){
		data.push_back(all_fields[offset+i]->get_value());
		columns.push_back(sql_tbl_obj[offset+i].column_name);
	}
	update_data_in_table(data, columns, TABLE_NAME[sql_tbl], object_id[0], sql_cn);
	emit data_is_changed();
	close();

}

void BigEditorWindow::AddToDB(){
	QStringList data{};
	QList<TableElement> sql_tbl_obj = get_sql_table(Passport);
	short offset = 0;
	unsigned int last_id = 0;

	if(use_passport){
		for(int i=0; i<sql_tbl_obj.size(); i++){
			data.push_back(all_fields[i]->get_value());
		}
		inster_data_in_table(data, TABLE_NAME[Passport], sql_tbl_obj[0].column_name, sql_cn);
		data.clear();
	}
	offset = sql_tbl_obj.size();

	sql_tbl_obj = get_sql_table(Human);
	for(int i=0; i<sql_tbl_obj.size(); i++){
		data.push_back(all_fields[i+offset]->get_value());
	}
	last_id = inster_data_in_table(data, TABLE_NAME[Human], sql_tbl_obj[0].column_name, sql_cn);
	qDebug()<<last_id;
	data.clear();
	offset += sql_tbl_obj.size();

	sql_tbl_obj = get_sql_table(sql_tbl);
	for(int i=0; i<sql_tbl_obj.size(); i++){
		data.push_back(all_fields[i+offset] -> get_value());
	}
	data[data.size()-1] = QString::number(last_id);
	inster_data_in_table(data, TABLE_NAME[sql_tbl], sql_tbl_obj[0].column_name, sql_cn);
	qDebug()<<data;
	emit data_is_changed();
	close();
}

void BigEditorWindow::disable_passport(bool state){
	use_passport = state;
	for(short i=0; i<PASSPORT_ELEMENT_CNT; i++){
		all_fields.at(i) -> SetEnabled(state);
	}
}

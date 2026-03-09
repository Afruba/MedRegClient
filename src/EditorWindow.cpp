#include "EditorWindow.h"
#include "MainWindow.h"
#include <QLineEdit>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QDebug>

const QStringList TABLE_NAME = {"PATIENT"}; 



EditField::EditField(QString name, DataType dt, SQLTable link_t, bool is_editable):
InputField(name, dt, link_t, is_editable){}

EditField::~EditField(){}

QString EditField::getValue(){
	if(getLineEdit()!=0) return getLineEdit()->text();
	else if (getCheckBox()!=0){
		if(getCheckBox()->isChecked()) return "true";
		else return "";
	}
}
void EditField::setValue(QString v){
	if(getLineEdit()!=0){
	    getLineEdit()->setText(v);
	}
	else if (getCheckBox()!=0){
		if(!v.isEmpty()) getCheckBox()->setChecked(true);
		else getCheckBox()->setChecked(false);
	}
}


EditorWindow::EditorWindow(QList<TableElement> t ,MainWindow *mw, bool add_mode, QStringList data, int row_pos):
mw(mw),
row_pos(row_pos),
add_mode(add_mode){
	QVBoxLayout *layout = new QVBoxLayout(this);
	if (add_mode)	layout -> addWidget(new QLabel("Добавление элемента"));
	else			layout -> addWidget(new QLabel("Редактирование"));
	qDebug()<<"Add data";
	
	for(short i=0; i<t.size(); i++){
		EditField *field = new EditField(t.at(i).column_name, t.at(i).column_type, t.at(i).link_to_table, !(t.at(i).primaly_key));
		all_fields.push_back(field);
		if(!data.isEmpty()) field -> setValue(data[i]);
		layout -> addWidget(field);
	}
	qDebug()<<"End";
	QPushButton *search_button;
	if (add_mode) search_button = new QPushButton("Добавить");
	else search_button = new QPushButton("Изменить");
	connect(search_button, &QPushButton::pressed, this, &EditorWindow::CheckAllFields);
	layout -> addWidget(search_button);
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
	if (result) ApplyChange();
}

void EditorWindow::ApplyChange(){
	QMessageBox::StandardButton reply;
		QString text = "Подтвердить?";
		reply = QMessageBox::question(this, "Подтверджение", text,  QMessageBox::Yes|QMessageBox::No);
	if (reply == QMessageBox::Yes) {
		qDebug()<<"Apply Change";
		if(mw!=nullptr){
			QStringList data = {};
			for(int i=0; i<all_fields.size(); i++){
				data.push_back(all_fields[i]->getValue());
			}
			if(!add_mode) mw->change_data_in_cur_table(row_pos, data);
			else		  mw->add_data_in_table(row_pos, data);//row_pos - номер таблицы
		}
		close();
	}
	else qDebug()<<"Ignore Change";
	
}

INSERT into Human (Name, Surname, Patronymic, Gender, Passport_number, Phone_number)
values ("Антон", "Бурлысенко", "Романович", 'м', "234233", "+7(909)874-58-55")
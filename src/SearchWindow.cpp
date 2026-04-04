#include "SearchWindow.h"
#include "SQLTableView.h"
#include "SQL_TABLES.h"
#include "SQL_Manager.h"
#include "VIEW_COL_OF_TABLE.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QDebug>
#include <QComboBox>
#include <QPushButton>
#include <QLineEdit>
#include <QCheckBox>

// #include <QRegularExpressionValidator>
SearchWindow::SearchWindow(pqxx::connection *sql_cn, QLineEdit *le, short tbl_id, short cln_id):
sql_cn(sql_cn), le(le){
	QVBoxLayout *layout = new QVBoxLayout(this);

	QWidget *button_holder = new QWidget();
	QHBoxLayout *layout_b = new QHBoxLayout(button_holder);
	setWindowTitle("Поиск");

	cb_table = new QComboBox();
	for(short t=0; t<TABLE_NAME.size(); t++){
		cb_table -> addItem(HUMAN_TABLE_NAME[t]);
	}
	cb_table -> setCurrentIndex(tbl_id);
	connect(cb_table, SIGNAL(currentIndexChanged(int)), this, SLOT(change_table(int)));

	cb_column = new QComboBox();
	QList<TableElement> tbl = get_h_table(tbl_id);
	for(short c=0; c<tbl.size(); c++){
		cb_column -> addItem(tbl[c].human_column_name);
	}
	cb_column -> setCurrentIndex(cln_id);
	connect(cb_column, SIGNAL(currentIndexChanged(int)), this, SLOT(change_column(int)));
	
	input_field = new InputField(tbl[cln_id], true);
	//lv_result = new QListView();
	tv = new SQLTableView(tbl_id, sql_cn, false);
	if(le!=nullptr) connect(tv, &SQLTableView::itemIdChanged, this, &SearchWindow::set_value_to_le);

	chb_inverted = new QCheckBox("Инвертировать");

	QPushButton *search_button = new QPushButton("ПОИСК");
	connect(search_button, &QPushButton::pressed, this, &SearchWindow::CheckAllFields);
		
	layout_b -> addWidget(cb_table);
	layout_b -> addWidget(cb_column);
	layout_b -> addWidget(input_field);
	layout_b -> addWidget(chb_inverted);
	layout_b -> addWidget(search_button);

	layout -> addWidget(button_holder);
	layout -> addWidget(tv);
	show();
}

SearchWindow::~SearchWindow(){
	QLayoutItem *li = layout()->takeAt(0);
	QLayoutItem *li_ch = 0;
	QLayout *l = li->widget()->layout();
	while ((li_ch = l->takeAt(0)) != nullptr){
		delete li_ch->widget();
		delete li_ch;
	}
	delete li;
	li = layout()->takeAt(0);
	delete li->widget();
	delete li;
	delete layout();
}

void SearchWindow::closeEvent (QCloseEvent *event){
	event->accept();
	delete this;
}

void SearchWindow::CheckAllFields(){
	//if (input_field->CheckInput())
	StartSearch();
}

void SearchWindow::StartSearch(){
	qDebug()<<"Satrt Search";
	QList<TableElement> tbl_obj = get_h_table(cb_table->currentIndex());
	QString v = input_field -> get_value();
	qDebug()<<v;
	QList<SQLFilter> filters = {};
	if(!v.isEmpty()){
		filters.push_back({
			"WHERE",
			tbl_obj[cb_column->currentIndex()].column_name,
			"=",
			input_field -> get_value()});
	}
	filters.push_back({"ORDER BY", tbl_obj[0].column_name, " ",(chb_inverted -> isChecked())? "DESC":"ASC"});
	tv->change_model(cb_table->currentIndex(), filters);
	
}

void SearchWindow::change_table(int index){
	qDebug()<<"change_table: "<<index;
	cb_column -> clear();
	QList<TableElement> tbl = get_h_table(index);
	for(short c=0; c<tbl.size(); c++){
		cb_column -> addItem(tbl[c].human_column_name);
	}
	cb_column -> setCurrentIndex(0);
	tv->change_model(index);
}

void SearchWindow::change_column(int index){
	qDebug()<<"change_column: "<<index;
	if(index==-1) return;
	QList<TableElement> tbl = get_h_table(cb_table->currentIndex());
	input_field -> change_table_element(tbl[index], true);
}

void SearchWindow::set_value_to_le(QString v){
	qDebug()<<v;
	le -> setText(v);
	qDebug()<<"CHANGE";
	//close();
}
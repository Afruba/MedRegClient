#include "SQLTableHolder.h"
#include "SQLTableView.h"
#include "InputField.h"
#include "SQL_TABLES.h"
#include "VIEW_COL_OF_TABLE.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>
SQLTableHolder::SQLTableHolder(short tbl_id, pqxx::connection *sql_cn, bool editable, const QList<SQLFilter> &filters){
	QVBoxLayout *layout = new QVBoxLayout(this);

	QWidget *button_holder = new QWidget();
	QHBoxLayout *layout_b = new QHBoxLayout(button_holder);

	cb_column = new QComboBox();
	QList<TableElement> tbl = get_h_table(tbl_id);
	for(short c=0; c<tbl.size(); c++){
		cb_column -> addItem(tbl[c].human_column_name);
	}
	cb_column -> setCurrentIndex(0);
	connect(cb_column, SIGNAL(currentIndexChanged(int)), this, SLOT(change_column(int)));
	
	input_field = new InputField(tbl[0], true);

	QCheckBox *chb_exactly = new QCheckBox("Точно");
	chb_exactly->setChecked(true);
	connect(chb_exactly, &QCheckBox::clicked, this, &SQLTableHolder::set_exactly);
	QCheckBox *chb_inverted = new QCheckBox("Инвертировать");
	connect(chb_inverted, &QCheckBox::clicked, this, &SQLTableHolder::set_inverted);

	QPushButton *search_button = new QPushButton("Фильтровать");
	connect(search_button, &QPushButton::pressed, this, &SQLTableHolder::setup_filter);

	tv = new SQLTableView(tbl_id, sql_cn, editable);
		
	layout_b -> addWidget(input_field);
	layout_b -> addWidget(cb_column);
	layout_b -> addWidget(chb_exactly);
	layout_b -> addWidget(chb_inverted);
	layout_b -> addWidget(search_button);

	layout -> addWidget(button_holder);
	layout -> addWidget(tv);
	show();
}
SQLTableHolder::~SQLTableHolder(){
	QLayoutItem *li;
	while( (li=layout()->itemAt(0)->widget()->layout()->takeAt(0))!=nullptr ){
		delete li->widget();
		delete li;
	}
	li = layout()->takeAt(0);
	delete li->widget();
	delete li;
	delete tv;
}

void SQLTableHolder::set_exactly(bool v) {exaclty=v;}
void SQLTableHolder::set_inverted(bool v) {inverted=v;}

void SQLTableHolder::change_column(int index){
	if(index==-1) return;
	QList<TableElement> tbl = get_h_table(tv->get_sql_tbl_id());
	if(tbl[index].column_type!=String){
		exaclty = true;
		//Добавить chb_exactly
	}
	input_field -> change_table_element(tbl[index], true);
}

void SQLTableHolder::setup_filter(){
	QList<SQLFilter> filters = {};

	QList<TableElement> tbl = get_h_table(tv->get_sql_tbl_id());

	if(!(input_field->get_value()).isEmpty() && !(input_field->get_value()=="''")){
		QString param = exaclty? "=" : " LIKE ";
		filters.push_back( {"where", tbl[cb_column->currentIndex()].column_name, param, input_field->get_value()} );
	}
	QString param = inverted? "DESC" : "ASC";
	filters.push_back( {"ORDER BY", tbl[0].column_name, " ", param} );
	tv->update_f(filters);
}

void SQLTableHolder::update(){
	tv->update();
}
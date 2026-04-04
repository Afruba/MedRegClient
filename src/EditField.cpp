#include "EditField.h"
#include "SearchWindow.h"
#include <QPushButton>
#include <QLayout>
EditField::EditField(TableElement te, pqxx::connection *sql_cn):
InputField(te), sql_cn(sql_cn), tbl(te.link_to_table){
	if(te.link_to_table != NULL_T){
		QPushButton *bt = new QPushButton("ИСКАТЬ");
		connect(bt, &QPushButton::pressed, this, &EditField::open_search_wnd);
		layout()->addWidget(bt);
	}
}

EditField::~EditField(){
	if (search_wnd_p!=nullptr) delete search_wnd_p;
}

void EditField::open_search_wnd(){
	if (search_wnd_p!=nullptr) search_wnd_p->activateWindow();
	else{
		search_wnd_p = new SearchWindow(sql_cn, get_line_edit(),tbl, 0);
		connect(search_wnd_p, &QObject::destroyed, this, &EditField::remove_search_wnd);
	}
}

void EditField::remove_search_wnd(){
	search_wnd_p = nullptr;
}

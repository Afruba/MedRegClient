#include "SearchWindow.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDebug>
// #include <QRegularExpressionValidator>

SearchWindow::SearchWindow(QList<TableElement> t, QString target_name){
	QVBoxLayout *layout = new QVBoxLayout(this);
	layout -> addWidget(new QLabel("Поиск в"+target_name));
	for(short i=0; i<t.size(); i++){
		InputField *field = new InputField(t.at(i).column_name, t.at(i).column_type);
		all_fields.push_back(field);
		layout -> addWidget(field);
	}
	QPushButton *search_button = new QPushButton("ПОИСК");
	connect(search_button, &QPushButton::pressed, this, &SearchWindow::CheckAllFields);
	layout -> addWidget(search_button);
	show();
}

SearchWindow::~SearchWindow(){
	QLayoutItem *li = 0;
	while ((li = layout()->takeAt(0)) != nullptr){
		delete li->widget();
		delete li;
	}
	delete layout();
}

void SearchWindow::closeEvent (QCloseEvent *event){
	event->accept();
	delete this;
}

void SearchWindow::CheckAllFields(){
	bool result = true;
	for(short i=0; i<all_fields.size(); i++){
		bool b = all_fields.at(i) -> CheckInput();
		all_fields.at(i) -> SetIndicator(b);
		result *= b;
	}
	if (result) StartSearch();
}

void SearchWindow::StartSearch(){
	qDebug()<<"Satrt Search";
}


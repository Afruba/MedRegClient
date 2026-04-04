#include "SchedulesView.h"
#include "Table.h"
#include "SQLTableView.h"
#include "SQL_Manager.h"
#include <QLabel>
#include <QListView>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStringListModel>
#include <QFont>

SchedulesView::SchedulesView(pqxx::connection *sql_cn):
sql_cn(sql_cn){
	QVBoxLayout *l = new QVBoxLayout(this);

	QWidget *tool_bar = new QWidget();
	QHBoxLayout *l_tool = new QHBoxLayout(tool_bar);
	label_date = new QLabel("");
	b_back = new QPushButton("Назад");
	b_back->setEnabled(false);
	connect(b_back, &QPushButton::clicked, this, &SchedulesView::open_date_list);

	l_tool -> addWidget(b_back);
	l_tool -> addWidget(label_date);

	l->addWidget(tool_bar);
	open_date_list();
}

SchedulesView::~SchedulesView(){
	if(p_tble_view!=0) delete p_tble_view;
	if(p_list_view!=0) delete p_list_view;
	delete b_back;
	delete label_date;
	delete layout()->itemAt(0)->widget();
	delete layout()->itemAt(0);
	delete layout();
}

void SchedulesView::open_date_list(){
	label_date -> setText("");
	b_back -> setEnabled(false);
	if(p_list_view!=0){
		delete p_tble_view;
		p_tble_view=0;
		p_list_view->show();
		return;
	}
	QListView *lv = new QListView();
	p_list_view = lv;
	lv->setFont(QFont("Adwaita", 12, QFont::Bold));
	QStringList data; get_all_date_schedules(data, sql_cn);
	QStringListModel *model = new QStringListModel();
	model -> setStringList(data);
	lv->setModel(model);

	connect(lv, &QListView::doubleClicked,this, [=](){ 
		open_schedules((lv->currentIndex().data()).toString());
	});

	layout() -> addWidget(lv);
}

void SchedulesView::open_schedules(QString object){
	label_date -> setText(object);
            QString Mon = object.mid(3,3);//text[3]+text[4]+text[5];
            if     (QString::compare(Mon,"Янв")) Mon="Jan";
            else if(QString::compare(Mon,"Фев")) Mon="Feb";
            else if(QString::compare(Mon,"Мар")) Mon="Mar";
            else if(QString::compare(Mon,"Апр")) Mon="Apr";
            else if(QString::compare(Mon,"Май")) Mon="May";
            else if(QString::compare(Mon,"Июн")) Mon="Jun";
            else if(QString::compare(Mon,"Июл")) Mon="Jul";
            else if(QString::compare(Mon,"Авг")) Mon="Фгп";
            else if(QString::compare(Mon,"Сен")) Mon="Sep";
            else if(QString::compare(Mon,"Окт")) Mon="Okt";
            else if(QString::compare(Mon,"Ноя")) Mon="Nov";
            else if(QString::compare(Mon,"Дек")) Mon="Dec";
            object = object.right(4)+QString(' ')+Mon+QString(' ')+object.mid(1,2);
	
	b_back -> setEnabled(true);
	if(p_tble_view!=0){
		delete p_tble_view;
		p_tble_view = 0;
	}

	p_list_view->hide();
	QList<SQLFilter> filters{ 
		{"WHERE", "to_char(schedules.action_date, 'yyyy Mon dd')", "=", '\''+object+'\''},
		{"ORDER BY", "schedules.action_date", " ", "DESC"}
	};
	p_tble_view = new SQLTableView(1, sql_cn, true, filters);
	layout() -> addWidget(p_tble_view);

}
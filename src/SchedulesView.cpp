#include "SchedulesView.h"
#include "Table.h"
#include "SQLTableView.h"
#include "SQL_Manager.h"
#include "SchedulesEditor.h"
#include <QLabel>
#include <QListView>
#include <QPushButton>
#include <QCheckBox>
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

	QPushButton *b_new = new QPushButton("Добавить");
	connect(b_new, &QPushButton::clicked, this, &SchedulesView::add_new_action);
	QCheckBox *cb_check_status = new QCheckBox("Убрать завершённые");
	connect(cb_check_status, &QCheckBox::clicked, this, &SchedulesView::set_check_status);
	l_tool -> addWidget(b_back);
	l_tool -> addWidget(label_date);
	l_tool -> addWidget(b_new);
	l_tool -> addWidget(cb_check_status);
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
	currnet_date = "";
	label_date -> setText("");
	b_back -> setEnabled(false);
	if(p_list_view!=0){
		delete p_tble_view;
		p_tble_view=0;
		p_list_view->show();
		return;
	}
	QListView *lv = new QListView();
	lv -> setEditTriggers(QAbstractItemView::NoEditTriggers);
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
    QString Mon = object.mid(4,3);
            if     (QString::compare(Mon,"Янв")==0) Mon="Jan";
            else if(QString::compare(Mon,"Фев")==0) Mon="Feb";
            else if(QString::compare(Mon,"Мар")==0) Mon="Mar";
            else if(QString::compare(Mon,"Апр")==0) Mon="Apr";
            else if(QString::compare(Mon,"Май")==0) Mon="May";
            else if(QString::compare(Mon,"Июн")==0) Mon="Jun";
            else if(QString::compare(Mon,"Июл")==0) Mon="Jul";
            else if(QString::compare(Mon,"Авг")==0) Mon="Aug";
            else if(QString::compare(Mon,"Сен")==0) Mon="Sep";
            else if(QString::compare(Mon,"Окт")==0) Mon="Okt";
            else if(QString::compare(Mon,"Ноя")==0) Mon="Nov";
            else if(QString::compare(Mon,"Дек")==0) Mon="Dec";
	object = object.right(4)+QString(' ')+Mon+QString(' ')+object.mid(1,2);
	currnet_date = object;
	b_back -> setEnabled(true);
	if(p_tble_view!=0){
		delete p_tble_view;
		p_tble_view = 0;
	}

	p_list_view->hide();

	QList<SQLFilter> filters{{"WHERE", "to_char(schedules.action_date, 'yyyy Mon dd')", "=", '\''+object+'\''}};
	if(check_status) filters.push_back({"AND", "schedules.status", "=", "false"});
	filters.push_back({"ORDER BY", "schedules.action_date", " ", "DESC"});

	p_tble_view = new SQLTableView(1, sql_cn, true, filters);
	layout() -> addWidget(p_tble_view);
}

void SchedulesView::update_page(){
	if(p_tble_view!=0){
		QList<SQLFilter> filters{{"WHERE", "to_char(schedules.action_date, 'yyyy Mon dd')", "=", '\''+currnet_date+'\''}};
		if(check_status) filters.push_back({"AND", "schedules.status", "=", "false"});
		filters.push_back({"ORDER BY", "schedules.action_date", " ", "DESC"});
		dynamic_cast<SQLTableView*>(p_tble_view) -> update_f(filters);
	}else{
		QListView *lv = dynamic_cast<QListView*>(p_list_view);
		QStringList data; get_all_date_schedules(data, sql_cn);
		QStringListModel *model = new QStringListModel();
		model -> setStringList(data);
		lv->setModel(model);
	}
}

void SchedulesView::set_check_status(bool v){
	check_status = v;
	update_page();
}
void SchedulesView::add_new_action(){
	SchedulesEditor *se = new SchedulesEditor(true, sql_cn, currnet_date);
	connect(se, &SchedulesEditor::data_is_changed, this, &SchedulesView::update_page);
	//connect(ew, SIGNAL(destroyed(QObject*)), this, SLOT(remove_closed_ew(QObject*)));
}
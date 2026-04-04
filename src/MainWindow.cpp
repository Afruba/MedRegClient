#include "MainWindow.h"
#include "SearchWindow.h"
#include "PasswordDialog.h"
#include "EditorWindow.h"
#include "BigEditorWindow.h"
#include "SQLTableView.h"
#include "SchedulesView.h"

#include <QTabWidget>
#include <QMenuBar>
#include <QDebug>
#include <QTableView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

#include <QStyle>
#include <QToolBar>
//! При смене виджета, прошлый уничтожается
// ТАЛОН НА ПРИЁМ
// МИС
MainWindow::MainWindow(pqxx::connection *sql_cn):
sql_cn(sql_cn){
	setWindowTitle("Стойка регистрации");
	//PasswordDialog *pd = new PasswordDialog(sql_cn);
	//connect(pd, &PasswordDialog::user_is_enter, this, &MainWindow::CreateUI);
	CreateUI();
}

MainWindow::~MainWindow(){
	delete sql_cn;
	if (centrall_widget!=0)		delete centrall_widget;
	if(search_wnd != 0) delete search_wnd;
	while(all_ew_points.size()){
		delete all_ew_points[0];
	}
}

void MainWindow::delete_CW(){
	if (centrall_widget!=0)	delete centrall_widget;
	centrall_widget = 0;
}

void MainWindow::closeEvent (QCloseEvent *event){
	event->accept();
	delete this;
}

void MainWindow::CreateUI(){
	setMinimumSize(800,500);

	QWidget *w = new QWidget();
	QHBoxLayout *hl = new QHBoxLayout(w);

	QWidget *button_holder = new QWidget();
	QVBoxLayout *vl = new QVBoxLayout(button_holder);
	vl -> setContentsMargins(QMargins(2,2,2,2));
	vl -> setSpacing(0);
	vl -> setAlignment(Qt::AlignCenter);
	QStringList tbl_name = {"Пациенты", "Расписание", "Мед Док", "Услуги", "Доктора", "Контракт","Люди", "Пасспорта"};
	QPushButton *button = new QPushButton("Главные Таблицы");
	connect(button, &QPushButton::pressed, this, &MainWindow::open_main_menu);
	vl -> addWidget(button);
	button = new QPushButton("Расписание");
	connect(button, &QPushButton::pressed, this, &MainWindow::open_schedules);
	vl -> addWidget(button);
	for(short i=3; i<tbl_name.size(); i++){
		button = new QPushButton(tbl_name[i]);
		connect(button, &QPushButton::pressed, this, [=]() {open_table(i);});
		vl -> addWidget(button);
	}
	vl->addStretch(1);

	centrall_widget = new QWidget();
	hl -> addWidget(button_holder);
	setCentralWidget(w);
	open_main_menu();
	CreateMenu();
	show();
}

void MainWindow::CreateMenu(){
	qDebug()<<"Create Menu";
	QToolBar *editToolBar = addToolBar(tr("Edit"));
	if (menu_is_create) return;
	QStyle::StandardPixmap(73);
	QIcon icon = this->style()->standardIcon(QStyle::SP_BrowserReload);
	QAction *a = new QAction(icon, "Обновить");
	//a -> setIcon(icon);
	connect(a, &QAction::triggered, this, &MainWindow::update_page);
	

	editToolBar -> addAction(a);
	icon = QIcon(":/res/system-search.png");
	a = new QAction(icon, "Поиск");
	connect(a, &QAction::triggered, this, &MainWindow::search);
	a->setShortcut(QKeySequence::Find);
	editToolBar -> addAction(a);

	a = new QAction("Добавить");
	connect(a, &QAction::triggered, this, &MainWindow::add_to_cur_table);
	editToolBar -> addAction(a);

	a = new QAction("Новый пациент");
	connect(a, &QAction::triggered, this, &MainWindow::add_new_patient);
	editToolBar -> addAction(a);

	a = new QAction("Новый приём");
	connect(a, &QAction::triggered, this, [=]() {add_to_oth_table(1);});
	editToolBar -> addAction(a);

	QStringList tbl_name = {"Пациенты", "Расписание", "Мед Док", "Услуги", "Доктора", "Контракт","Люди", "Пасспорта"};
	QMenu *m = menuBar() -> addMenu("Создать в");
	for(short i=0; i<tbl_name.size(); i++){
		a = new QAction(tbl_name[i]);
		connect(a, &QAction::triggered, this, [=]() {add_to_oth_table(i);});
		m -> addAction(a);
	}


	m = menuBar() -> addMenu("Пользователь");
	a = new QAction("Выйти");
	connect(a, &QAction::triggered, this, &MainWindow::exit_from_acc);
	m -> addAction(a);
	a = new QAction("Закрыть");
	connect(a, &QAction::triggered, this, &MainWindow::quit_from_app);
	m -> addAction(a);
	menu_is_create = true;
}

void MainWindow::update_page(){
	get_current_table_view() -> update();
}

void MainWindow::search(){
	qDebug() << "Serach";
	if(search_wnd!=0) search_wnd -> activateWindow();
	else{
		search_wnd = new SearchWindow(sql_cn);
		connect(search_wnd, &SearchWindow::destroyed, this, &MainWindow::remove_search_window);
	}	
}

void MainWindow::add_to_cur_table(){
	qDebug() << "Add to current table";
	add_to_oth_table(current_table);
}
void MainWindow::add_to_oth_table(short i){
	qDebug() << "Add to other table";
	EditorWindow *ew = 0;
	ew = new EditorWindow(static_cast<SQLTable>(i), true, sql_cn);
	all_ew_points.push_back(ew);
	connect(ew, &EditorWindow::data_is_changed, this, &MainWindow::update_page);
	connect(ew, SIGNAL(destroyed(QObject*)), this, SLOT(remove_closed_ew(QObject*)));
}
void MainWindow::add_new_patient(){
	BigEditorWindow *bew = 0;
	bew = new BigEditorWindow(Patient, true, sql_cn);
	all_ew_points.push_back(bew);
	connect(bew, &BigEditorWindow::data_is_changed, this, &MainWindow::update_page);
	connect(bew, SIGNAL(destroyed(QObject*)), this, SLOT(remove_closed_ew(QObject*)));
}

void MainWindow::open_main_menu(){
	delete_CW();
	QTabWidget *t_widget = new QTabWidget();
	t_widget -> addTab(new SQLTableView(0, sql_cn), "Пациенты");
	t_widget -> addTab(new SQLTableView(1, sql_cn), "Все записи");
	t_widget -> addTab(new SQLTableView(2, sql_cn), "Мед Документы");
	centrall_widget = t_widget;
	centralWidget()->layout() -> addWidget(t_widget);
	connect(t_widget, &QTabWidget::currentChanged,this, &MainWindow::change_in_main_widget);
	
	current_table = 0;
	//setCentralWidget(centrall_widget);
}
void MainWindow::open_table(short id){
	delete_CW();
	centrall_widget = new SQLTableView(id, sql_cn);
	current_table = id;
	centralWidget()->layout() -> addWidget(centrall_widget);
}
void MainWindow::open_schedules(){
	delete_CW();
	centrall_widget = new SchedulesView(sql_cn);
	current_table = 1;
	centralWidget()->layout() -> addWidget(centrall_widget);
}


void MainWindow::exit_from_acc(){
	qDebug() << "Quit";
	close();
	PasswordDialog *pd = new PasswordDialog(sql_cn);
	connect(pd, &PasswordDialog::user_is_enter, this, &MainWindow::CreateUI);
}
void MainWindow::quit_from_app(){
	qDebug() << "Exit";
	delete this;
}

void MainWindow::get_answer_from_pd(bool status){
	if(status) qDebug()<< "LOGIN +";
	else qDebug()<< "LOGIN -";
	password_widget = 0;
}

void MainWindow::change_in_main_widget(int index){
	qDebug()<<index;
	current_table = index;
}




// QList<TableElement>& MainWindow::get_current_table(){
//     return get_human_table(current_table);
// }

SQLTableView* MainWindow::get_current_table_view(){
	if(current_table>2) return dynamic_cast<SQLTableView*>(centrall_widget);

	QTabWidget *tb = dynamic_cast<QTabWidget*>(centrall_widget);
	return dynamic_cast<SQLTableView*>(tb->currentWidget());	
}


void MainWindow::remove_closed_ew(QObject *object){
	all_ew_points.removeOne(object);
}
void MainWindow::remove_search_window(){
	search_wnd = 0;
}

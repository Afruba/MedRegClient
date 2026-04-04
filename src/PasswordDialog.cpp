#include "PasswordDialog.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QCloseEvent>
#include "SQL_Manager.h"

PasswordDialog::PasswordDialog(pqxx::connection* sql_cn): sql_cn(sql_cn)
{
	setWindowTitle("Авторизация");
	le_n = new QLineEdit();
	le_p = new QLineEdit();
	QGridLayout *l = new QGridLayout(this);
	l -> addWidget(new QLabel("Логин"), 0, 0);
	l -> addWidget(le_n, 0, 1);
	l -> addWidget(new QLabel("Пароль"), 1, 0);
	l -> addWidget(le_p, 1, 1);
	QPushButton *button = new QPushButton("Войти");
	connect(button, &QPushButton::pressed , this, &PasswordDialog::check_input);
	l -> addWidget(button, 2, 0);
	show();
}

PasswordDialog::~PasswordDialog(){
	QLayoutItem *li = 0;
	while ((li=layout()->takeAt(0)) != nullptr){
		delete li -> widget();
		delete li;
	}
	delete layout();
}

void PasswordDialog::check_input(){
	if (check_user_account(le_n->text(), le_p->text(), sql_cn)){
		ret_status = true;
		close();
	}
	else if(!label_is_added){
		label_is_added = true;
		layout()-> addWidget(new QLabel("Неверный логин или пароль!"));
	}
}

void PasswordDialog::closeEvent(QCloseEvent *event){
	emit user_is_enter(ret_status);
	event->accept();
	delete this;
}
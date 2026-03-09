#include "PasswordDialog.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QCloseEvent>

PasswordDialog::PasswordDialog(QString un, QString up):
up(up),
un(un){
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

// void PasswordDialog::user_is_enter(){

// }

void PasswordDialog::check_input(){
	if (un == le_n->text() && up == le_p->text()){
		ret_status = true;
		close();
	}
}

void PasswordDialog::closeEvent(QCloseEvent *event){
	emit user_is_enter(ret_status);
	event->accept();
	delete this;
}
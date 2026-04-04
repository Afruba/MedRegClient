#include <QApplication>
#include "MainWindow.h"
#include <pqxx/pqxx>
int main(int argc, char **argv){
	pqxx::connection *cx = new pqxx::connection{"postgres://postgres@localhost:5432/MedRegDB"};
	QApplication app = QApplication(argc, argv);
	
	MainWindow *wnd = new MainWindow(cx);
	
	return app.exec();
}
#include "Printer.h"
#include <fstream>
#include <string>
#include <QMessageBox>
void print_doc(QString file_name, QStringList &args){
	QMessageBox messageBox;
	std::string file_path = "source_doc/"+file_name.toStdString()+".txt";
	std::ifstream source_file(file_path);
	if(!source_file.is_open()){
		messageBox.setText(QString::fromStdString("Can't open "+ file_path));
    	messageBox.setIcon(QMessageBox::Critical);
    	messageBox.exec();
		return;
	}

	std::ofstream out_file("source_doc/out.file");
	unsigned int arg_id=0;

	std::string buf;
	while (std::getline(source_file, buf))
    {
    	size_t pos = buf.npos;
    	std::string finder = '['+std::to_string(arg_id)+']';
		while( (pos=buf.find(finder))!=buf.npos){;//std::npos;
			buf.replace(pos, 3, args[arg_id].toStdString());
			arg_id++;
			finder = '['+std::to_string(arg_id)+']';
		}
		out_file<<buf<<'\n';
    }
	out_file.close();
	source_file.close();
}
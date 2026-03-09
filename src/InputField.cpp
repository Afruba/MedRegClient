#include "InputField.h"
#include <QHBoxLayout>
#include <QLineEdit>
#include <QCheckBox>
#include <QValidator>
#include <QLabel>
#include <QPushButton>

InputField::InputField(QString name, DataType dt, SQLTable link_t, bool is_editable):
dt(dt){
	QHBoxLayout *layout = new QHBoxLayout(this);
	layout -> addWidget(new QLabel(name));

	if(dt==Bool){
		cb = new QCheckBox();
		layout -> addWidget(cb);
	}
	else{
		le = new QLineEdit();
		le->setEnabled(is_editable);
		if(dt==String) layout -> addWidget(le);
		else{
			QValidator *validator = 0;
			if (dt==Int){
				validator = new QIntValidator(0, 2147483647, this);
				le -> setValidator(validator);
				layout -> addWidget(le);
			}
			else if (dt==Date){
				QRegularExpression rx("^(?<date>\\d{0,2})\\.(?<month>\\d{0,2})\\.(?<year>\\d{0,4})$");
				validator = new QRegularExpressionValidator(rx, this);
				le -> setValidator(validator);
				layout -> addWidget(le);
				connect(le, &QLineEdit::textEdited, this, &InputField::date_is_change);
			}
			else if (dt==Time){
				QRegularExpression rx("^\\d{0,2}?\\:\\d{0,2}?$");
				validator = new QRegularExpressionValidator(rx, this);
				le -> setValidator(validator);
				layout -> addWidget(le);
				connect(le, &QLineEdit::textEdited, this, &InputField::time_is_change);
			}
			else if (dt==Telephone){// \\+? - символ + может быть, а может не быть
				QRegularExpression rx("^\\+?7?\\(?\\d{0,3}?\\)?\\d{0,3}?\\-?\\d{0,2}\\-?\\d{0,2}$");
				validator = new QRegularExpressionValidator(rx, this);
				le -> setValidator(validator);
				layout -> addWidget(le);
				connect(le, &QLineEdit::textEdited, this, &InputField::telephone_is_change);
			}
			
		}
	}

	if(link_t != NULL_T){
		QPushButton *bt = new QPushButton("ИСКАТЬ");
		layout->addWidget(bt);
	}
}

InputField::~InputField(){
	QLayoutItem *li = 0;
	while ((li = layout()->takeAt(0)) != nullptr){
		delete li->widget();
		delete li;
	}
	delete layout();
}

QLineEdit* InputField::getLineEdit(){
	return le;
}
QCheckBox* InputField::getCheckBox(){
	return cb;
}
DataType InputField::getDataType(){
	return dt;
}

void InputField::telephone_is_change(QString new_text){
	bool is_change = false;
	int old_curs_pos = le->cursorPosition();
	QRegularExpression re1("\\+7\\(\\d{3}");
	QRegularExpression re2("\\+7\\(\\d{3}\\)\\d{3}");
	QRegularExpression re3("\\+7\\(\\d{3}\\)\\d{3}\\-\\d{2}");
	// QRegularExpressionMatch match = re.match("abc123 def");
	// bool hasMatch = match.hasMatch(); // true
	if(new_text=="+" || new_text=="("){
		new_text = "+7(";
		is_change = true;
	}
	else if (new_text=="7"){
		new_text = "+7(7";
		is_change = true;
	}
	else if (new_text!="7" && new_text.size()==1){
		new_text = "+7(" + new_text;
		is_change = true;
	}
	else{
		if(re1.match(new_text).hasMatch() && new_text.size() > 6 && new_text[6]!=')'){
			new_text.insert(6,")");
			is_change = true;
		}
		else if(re2.match(new_text).hasMatch() && new_text.size() > 10 && new_text[10]!='-'){
			new_text.insert(10,"-");
			is_change = true;
		}
		else if(re3.match(new_text).hasMatch() && new_text.size() > 13 && new_text[13]!='-'){
			new_text.insert(13,"-");
			is_change = true;
		}
	}
	// else if(re.match(new_text).hasMatch() && new_text.size() == 6) new_text += ")";
	
	
	if (is_change) le->setText(new_text);
}

void InputField::time_is_change(QString new_text){
	bool is_change = false;
	if (new_text.size()==2){
		new_text+=':';
		is_change = true;
	}
	else if(new_text.size()>2 && new_text[2]!=':'){
		new_text.insert(2,':');
	}
	if (is_change) le->setText(new_text);
}

void InputField::date_is_change(QString new_text){
	bool is_change = false;
	if (new_text.size()==2 || new_text.size()==5){
		new_text+='.';
		is_change = true;
	}
	if (is_change) le->setText(new_text);
}

bool InputField::CheckInput(){
	if(le==0){
		return true;
	}
	
	QString my_text = le->text();
	if (my_text.isEmpty()) return true;
	
	if (dt==Telephone){
		QRegularExpression re("^\\+7\\(\\d{3}\\)\\d{3}\\-\\d{2}\\-\\d{2}$");
		return re.match(my_text).hasMatch();
	}
	else if (dt==Time){
		QRegularExpression re("^\\d{2}\\:\\d{2}$");
		if (re.match(my_text).hasMatch()){
			return (my_text[0]<='2' && my_text[1]<='4' && my_text[3]<='6');
		}
		return false;
	}
	else if (dt==Date){
		QRegularExpression re("^\\d{2}\\.\\d{2}\\.(\\d{4}|\\d{2})$");
		return re.match(my_text).hasMatch();
	}
	else return true;
}

void InputField::SetIndicator(bool v){
	if(le!=0){
		if(v)	le->setStyleSheet("color: white");
		else	le->setStyleSheet("color: red");
	}	
}
#include "VIEW_COL_OF_TABLE.h"
QList<TableElement> HTable_Patient{ 
	TableElement("patient_id",				"ID",					Int,	NULL_T, 	true),
	TableElement("concat_ws(' ',human_name, human_surname, human_patronymic)",						"ФИО",					String, Human,		false, 74, true),
	TableElement("human.posport_number",	"Номер Паспорта",		StrInt,	Passport,	false, 6, true),
	TableElement("passport.date_of_birth",	"Дата рож.",			Date,	Passport),
	TableElement("medcard_number",			"№ Мед Карты",			Int),
	TableElement("place_of_work",			"Место работы",			String, NULL_T,		false, 80, true),
	TableElement("human.phone_number",		"Номер Телефона",		Telephone, NULL_T,	false, 16)};
	//1
QList<TableElement> HTable_Schedules{
	TableElement("action_id",				"Номер",	Int,	NULL_T,		true),
	TableElement("service.service_name",	"Услуга",	String, NULL_T,		false, 64),
	TableElement("concat_ws(' ',human_name, human_surname, human_patronymic)", "Пациент",	String, Human,		false, 74, true),
	TableElement("Врач",								String, Doctor),
	TableElement("doctor.office",			"Кабинет",	String, NULL_T,		false, 4, true),
	TableElement("schedules.action_date",	"Дата",		Timestamp),
	TableElement("schedules.action_date",	"Вермя",		Timestamp),
	TableElement("schedules.status",		"Статус",	Bool)};
	//2
QList<TableElement> HTable_MedDocs{
	TableElement("meddoc.meddoc_id", 	"ID",					Int,	NULL_T, true),
	TableElement("meddoc.title",		"Название",				String),
	TableElement("Получатель",		String, Patient),
	TableElement("concat_ws(' ',human_name, human_surname, human_patronymic)", "Врач",			String, Doctor),
	TableElement("meddoc.date_of_ready","Дата Создания",		Date),
	TableElement("meddoc.vlid_util",	"Срок",					Date),
	TableElement("meddoc.date_of_give",	"Дата Выдачи",			Date)};
	//3
QList<TableElement> HTable_Service{
	TableElement("service.service_id",	"ID", 					Int,	NULL_T, true),
	TableElement("service.service_name","Название", 			String),
	TableElement("concat_ws(' ',human_name, human_surname, human_patronymic)", "Доктор", 			String, Doctor),
	TableElement("doctor.office",		"Кабинет", 				Int),
	TableElement("service.price",		"Цена", 				Int),
	TableElement("service.time_begin",	"Начало Приёма",		Time),
	TableElement("service.time_end",	"Конец Приёма",			Time)
};
	//4
QList<TableElement> HTable_Doctor{
	TableElement("doctor_id",			"ID",					Int,	NULL_T, 	true),
	TableElement("concat_ws(' ',human_name, human_surname, human_patronymic)",						"ФИО",					String, Human,		false, 74, true),
	TableElement("human.phone_number",	"Номер Телефона",		Telephone, NULL_T,	false, 16),
	TableElement("office",				"Кабинет",				String, NULL_T,		false, 4, true),
	TableElement("doctor.on_vacation",	"В отпуске",			Bool),
	TableElement("doctor.on_work",		"На работе",			Bool)
};
	//5
QList<TableElement> HTable_Contract{
	TableElement("contract.contract_id", 	"ID",				Int,	NULL_T,		true),
	TableElement("contract.contract_title", "Контракт",			String,	NULL_T,		false, 80, true),
	TableElement("concat_ws(' ',human_name, human_surname, human_patronymic)", "Заказчик",		String, Human,		false, 74, true),
	TableElement("Исполнитель",									String),
	TableElement("contract.date_of_create", "Дата создания",	Date)
};
	//6
QList<TableElement> HTable_Human{
	TableElement("human_id",			"Номер",				Int,	NULL_T, 	true),
	TableElement("human_name",			"Имя",					String, NULL_T, 	false, 20),
	TableElement("human_surname",		"Фамилия",				String, NULL_T, 	false, 24),
	TableElement("human_patronymic",	"Отчество",				String, NULL_T, 	false, 30),
	TableElement("human_gender",		"Пол",					Char),
	TableElement("posport_number",		"Номер Паспорта",		StrInt,	Passport,	false, 6, true),
	TableElement("phone_number",		"Номер Телефона",		Telephone, NULL_T,	false, 16)
};
	//6
QList<TableElement> HTable_Passport{
	TableElement("passport_number",		 "Номер Паспорта",		StrInt, NULL_T,		false, 6),
	TableElement("passport_seriec",		 "Серия Паспорта",		StrInt, NULL_T,		false, 4),
	TableElement("issue_by",			 "Кем Выдан",			Text),
	TableElement("date_of_issue",		 "Дата Выдачи",			Date),
	TableElement("place_of_regestration","Место рег. ",			Text),
	TableElement("place_of_birth",		 "Место рож.",			Text),
	TableElement("date_of_birth",		 "Дата рож.",			Date)
};

QList<TableElement>& get_h_table(short id){
	switch(id){
	case 0:	return HTable_Patient;
	case 1:	return HTable_Doctor;
	case 2:	return HTable_Service;
	case 3:	return HTable_Contract;
	case 4:	return HTable_MedDocs;
	case 5:	return HTable_Schedules;
	case 6:	return HTable_Human;
	case 7:	return HTable_Passport;
	}
}
QList<TableElement>& get_h_table(SQLTable t){
	switch(t){
	case Passport:	return HTable_Passport;
	case Human:		return HTable_Human;
	case Patient:	return HTable_Patient;
	case Doctor:	return HTable_Doctor;
	case Service:	return HTable_Service;
	case Contract:	return HTable_Contract;
	case MedDoc:	return HTable_MedDocs;
	case Schedules:	return HTable_Schedules;
	}
}
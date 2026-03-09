#ifndef VIEW_COL_OF_TABLE_H
#define VIEW_COL_OF_TABLE_H
	//0
	QList<TableElement> TablePatient{ 
		TableElement("ID",				Int, NULL_T, true),//Human
		TableElement("ФИО",				String),
		TableElement("Паспорт",			String, Pasports),
		TableElement("Д/Р",				Date),
		TableElement("№ Мед Карты",		Int),
		TableElement("Телефон",			Telephone)};
	//1
	QList<TableElement> TableSchedules{
		TableElement("ID",				Int, NULL_T, true),
		TableElement("Услуга",			String, Service),
		TableElement("Пациент",			String, Patient),
		TableElement("Врач",			String, Doctor),
		TableElement("Кабинет",			Int),
		TableElement("Дата",			Date)};
	//2
	QList<TableElement> TableMedDocs{
		TableElement("ID",				Int, NULL_T, true),
		TableElement("Название",		String),
		TableElement("Получатель",		String, Patient),
		TableElement("Врач",			String, Doctor),
		TableElement("Дата Создания",	Date),
		TableElement("Срок",			Date),
		TableElement("Дата Выдачи",		Date)};
	//3
	QList<TableElement> TableService{
		TableElement("ID", 				Int, NULL_T, true),
		TableElement("Название", 		String),
		TableElement("Доктор", 			String, Doctor),
		TableElement("Кабинет", 		Int),
		TableElement("Цена", 			Int),
		TableElement("Начало Приёма",	Time),
		TableElement("Конец Приёма",	Time)};
	//4
	QList<TableElement> TableDoctor{
		TableElement("ID",				Int, NULL_T, true),//Human
		TableElement("ФИО",				String),
		TableElement("Доктор",			String),
		TableElement("Телефон",			Telephone),
		TableElement("В отпуске",		Bool),
		TableElement("На работе",		Bool)};
#endif
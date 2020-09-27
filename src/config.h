#ifndef CONFIG_H
#define CONFIG_H

#include <QString>

namespace config
{
    static const QString COPYRIGHT = "PhoneBook. UnitedTimur (c) 2020";
    static const QString MAIN_TITLE = "PhoneBook";
    static const QString APPLICATION = MAIN_TITLE;
    static const QString ORGANIZATION = "UnitedTimurCompany (c) 2020";

    static const QString ER_TITLE = "Ошибка";
    static const QString ER_EXIST = "Такой контакт уже содержится в таблице!";
    static const QString ER_IMPORT_FILE = "Файл не содержит полей!";

    static const QString DATE_FRMT = "dd.MM.yyyy";

}

#endif // CONFIG_H

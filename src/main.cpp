#include <QApplication>

#include "phonebookmanager.h"
#include "config.h"

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);

    QApplication::setApplicationName(config::COPYRIGHT);
    QApplication::setOrganizationName(config::ORGANIZATION);

    PhoneBookManager phonebook;
    phonebook.show();

	QApplication::exec();

	return 0;
}

#ifndef CONTACT_H
#define CONTACT_H

#include <QString>
#include <QDate>

struct Contact
{
    QString fullname;
    QString email;
    QDate birthday;
    QDate dateEntry;

    bool operator==(const Contact& contact)
    {
        return fullname == contact.fullname
                && email == contact.email
                && birthday == contact.birthday;
    }

};

#endif // CONTACT_H

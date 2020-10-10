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

inline QDataStream &operator<<(QDataStream&stream, const Contact& contact)
{
    return stream << contact.fullname << contact.email << contact.birthday << contact.dateEntry;
}

inline QDataStream &operator>>(QDataStream& stream, Contact& contact)
{
    return stream >> contact.fullname >> contact.email >> contact.birthday >> contact.dateEntry;
}


#endif // CONTACT_H

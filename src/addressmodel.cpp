#include "addressmodel.h"

AddressModel::AddressModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

int AddressModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return contacts.count();
}

int AddressModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return 5;
}

QVariant AddressModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= contacts.count() || index.row() < 0)
            return QVariant();

    if (role == Qt::DisplayRole)
    {
        const auto& contact = contacts.at(index.row());

        switch (index.column())
        {
        case 0:
            return contacts.indexOf(contact);
        case 1:
            return contact.fullname;
        case 2:
            return contact.email;
        case 3:
            return contact.birthday;
        case 4:
            return contact.dateEntry;
        default:
            break;
        }
    }

    return QVariant();
}

QVariant AddressModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
            return QVariant();

    if (orientation == Qt::Horizontal)
        {
            switch (section)
            {
                case 0:
                    return QString::fromUtf8(u8"Номер");
                case 1:
                    return QString::fromUtf8(u8"ФИО");
                case 2:
                    return QString::fromUtf8(u8"Email");
                case 3:
                    return QString::fromUtf8(u8"Дата рождения");
                case 4:
                    return QString::fromUtf8(u8"Дата внесения");
                default:
                    break;
            }
        }

    return QVariant();
}

Qt::ItemFlags AddressModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
            return Qt::ItemIsEnabled;

    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

bool AddressModel::removeRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);

    beginRemoveRows(QModelIndex(), position, position + rows - 1);

    for (int row = 0; row < rows; ++row)
        contacts.removeAt(position);

    endRemoveRows();

    return true;
}

bool AddressModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole)
    {
            const int row = index.row();
            Contact contact = contacts.value(row);

            switch (index.column())
            {
                case 1:
                    contact.fullname = value.toString();
                    break;
                case 2:
                    contact.email = value.toString();
                    break;
                case 3:
                    contact.birthday = value.toDate();
                    break;
                case 4:
                    contact.dateEntry = value.toDate();
                    break;
                default:
                    return false;
            }

            contacts.replace(row, contact);

            emit dataChanged(index, index, {Qt::DisplayRole, Qt::EditRole});

            return true;
        }

        return false;
}

bool AddressModel::appendContact(const QString &fullname, const QString &email, const QDate &birthday, const QDate& dateentry)
{
    Contact contact;
    {
        contact.fullname = fullname;
        contact.email = email;
        contact.birthday = birthday;
        contact.dateEntry = dateentry;
    }

    // If exist contact
    if (qFind(contacts.begin(), contacts.end(), contact) != contacts.end())
        return false;

    const int row = contacts.count();

    beginInsertRows(QModelIndex(), row, row);

    contacts.append(contact);

    endInsertRows();

    return true;
}

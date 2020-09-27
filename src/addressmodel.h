#ifndef ADDRESSMODEL_H
#define ADDRESSMODEL_H

#include <QAbstractTableModel>
#include <QVector>

#include "contact.h"

class AddressModel : public QAbstractTableModel
{
    Q_OBJECT

protected:
    typedef QVector<Contact> Data;

public:
    explicit AddressModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    bool appendContact(const QString& fullname, const QString& email, const QDate& birthday, const QDate& dateentry);

protected:
    Data contacts;
};

#endif // ADDRESSMODEL_H

#ifndef PHONEBOOKMANAGER_H
#define PHONEBOOKMANAGER_H

#include <QMainWindow>

class AddressModel;

namespace Ui
{
    class PhoneBookManager;
}

class PhoneBookManager : public QMainWindow
{
    Q_OBJECT

public:
    explicit PhoneBookManager(QWidget *parent = nullptr);
    ~PhoneBookManager();

protected:
    void init();
    void setup();
    void initConnections();

    Q_SLOT void addEntry();
    Q_SLOT void remEntry();
    Q_SLOT void editEntry();
    Q_SLOT void searchEntry(const QString& text);

private:
    void writeSettings();
    void readSettings();

private:
    Ui::PhoneBookManager *ui;
    AddressModel* addressModel;
};

#endif // PHONEBOOKMANAGER_H

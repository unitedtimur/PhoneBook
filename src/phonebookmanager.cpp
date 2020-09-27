#include "phonebookmanager.h"
#include "ui_phonebookmanager.h"
#include "config.h"
#include "addressmodel.h"
#include "editdialog.h"

#include <QDate>
#include <QMessageBox>
#include <QSortFilterProxyModel>
#include <QSettings>
#include <QFile>

PhoneBookManager::PhoneBookManager(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::PhoneBookManager),
    addressModel(new AddressModel())
{
    ui->setupUi(this);

    // Setup geometry the form
    this->readSettings();

    // Initialize the main form
    this->init();

    // Setup model with tableView
    this->setup();

    // Initialize connections
    this->initConnections();
}

PhoneBookManager::~PhoneBookManager()
{
    // Save geometry
    this->writeSettings();

    delete ui;
}

void PhoneBookManager::init()
{
    this->setWindowTitle(config::MAIN_TITLE);
    this->setMinimumSize(QSize(600, 600));

    QRegExp regExp("(0[1-9]|[12][0-9]|3[01]{1,2}).(0[1-9]|[12]{1,2}).(19[0-9][0-9]|20[0-9][0-9])");
    QRegExpValidator *validator = new QRegExpValidator(regExp, this);
    this->ui->birthday->setValidator(validator);
}

void PhoneBookManager::setup()
{
    QSortFilterProxyModel* proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(addressModel);
    proxyModel->setFilterKeyColumn(0);

    this->ui->tableView->setModel(proxyModel);
    this->ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->ui->tableView->horizontalHeader()->setStretchLastSection(true);
    this->ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    this->ui->tableView->setSortingEnabled(true);
}

void PhoneBookManager::initConnections()
{
    connect(this->ui->addButton, &QPushButton::clicked, this, &PhoneBookManager::addEntry);
    connect(this->ui->remButton, &QPushButton::clicked, this, &PhoneBookManager::remEntry);
    connect(this->ui->editButton, &QPushButton::clicked, this, &PhoneBookManager::editEntry);
    connect(this->ui->searchLineEdit, &QLineEdit::textChanged, this, &PhoneBookManager::searchEntry);
}

void PhoneBookManager::addEntry()
{
    if (!this->addressModel->appendContact(ui->fullname->text(),
                                      ui->email->text(),
                                      QDate::fromString(ui->birthday->text(), config::DATE_FRMT),
                                      QDate::currentDate()))
    {
        QMessageBox::critical(this, config::ER_TITLE, config::ER_EXIST);
    }
}

void PhoneBookManager::remEntry()
{
    QSortFilterProxyModel* proxy = static_cast<QSortFilterProxyModel*>(this->ui->tableView->model());
    QItemSelectionModel* selectionModel = this->ui->tableView->selectionModel();

    const QModelIndexList indexes = selectionModel->selectedRows();

    for (QModelIndex index : indexes)
    {
        int row = proxy->mapToSource(index).row();
        this->addressModel->removeRows(row, 1, QModelIndex());
    }
}

void PhoneBookManager::editEntry()
{
    QSortFilterProxyModel* proxy = static_cast<QSortFilterProxyModel*>(this->ui->tableView->model());
    QItemSelectionModel* selectionModel = this->ui->tableView->selectionModel();

    const QModelIndexList indexes = selectionModel->selectedRows();

    QString fullname;
    QString email;
    QDate birthday;
    QDate dateentry;

    int row = -1;

    if (indexes.empty())
        return;

    for (const auto& index : indexes)
    {
        row = proxy->mapToSource(index).row();

        QModelIndex fullnameIndex = this->addressModel->index(row, 1, QModelIndex());
        QVariant varFullname = this->addressModel->data(fullnameIndex, Qt::DisplayRole);
        fullname = varFullname.toString();

        QModelIndex emailIndex = this->addressModel->index(row, 2, QModelIndex());
        QVariant varEmail = this->addressModel->data(emailIndex, Qt::DisplayRole);
        email = varEmail.toString();

        QModelIndex birthdayIndex = this->addressModel->index(row, 3, QModelIndex());
        QVariant varBirthday = this->addressModel->data(birthdayIndex, Qt::DisplayRole);
        birthday = varBirthday.toDate();

        QModelIndex dateentryIndex = this->addressModel->index(row, 4, QModelIndex());
        QVariant varDateentry = this->addressModel->data(dateentryIndex, Qt::DisplayRole);
        dateentry = varDateentry.toDate();
    }

    EditDialog editDialog(this, fullname, email, birthday);

    if (editDialog.exec())
    {
        const QString newFullname = editDialog.fullname();
        const QString newEmail = editDialog.email();
        const QDate newBirthday = editDialog.birthday();

        if (newFullname != fullname || newEmail != email || newBirthday != birthday)
        {
            const QModelIndex newFullnameIndex = this->addressModel->index(row, 1, QModelIndex());
            this->addressModel->setData(newFullnameIndex, newFullname, Qt::EditRole);

            const QModelIndex newEmailIndex = this->addressModel->index(row, 2, QModelIndex());
            this->addressModel->setData(newEmailIndex, newEmail, Qt::EditRole);

            const QModelIndex newBirthdayIndex = this->addressModel->index(row, 3, QModelIndex());
            this->addressModel->setData(newBirthdayIndex, newBirthday, Qt::EditRole);

            const QModelIndex newDateEntry = this->addressModel->index(row, 4, QModelIndex());
            this->addressModel->setData(newDateEntry, QDate::currentDate(), Qt::EditRole);
        }
    }
}

void PhoneBookManager::searchEntry(const QString& text)
{
    QSortFilterProxyModel* proxy = static_cast<QSortFilterProxyModel*>(this->ui->tableView->model());
    proxy->setFilterRegExp(QRegExp(text, Qt::CaseInsensitive, QRegExp::FixedString));
    proxy->setFilterKeyColumn(1);
}

void PhoneBookManager::writeSettings()
{
    QSettings settings(config::ORGANIZATION, config::APPLICATION);

    settings.beginGroup("PhoneBook");
    settings.setValue("size", this->size());
    settings.setValue("pos", this->pos());
    settings.endGroup();
}

void PhoneBookManager::readSettings()
{
    QSettings settings(config::ORGANIZATION, config::APPLICATION);

    settings.beginGroup("PhoneBook");
    this->resize(settings.value("size", QSize(600, 600)).toSize());
    this->move(settings.value("pos", QPoint(600, 600)).toPoint());
    settings.endGroup();
}

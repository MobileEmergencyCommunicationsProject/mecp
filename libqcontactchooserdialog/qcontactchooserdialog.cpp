#include "qcontactchooserdialog.h"
#include "ui_qcontactchooserdialog.h"
#include <QKeyEvent>
#include <QRegExp>
#include <QtDebug>

QContactChooserDialog::QContactChooserDialog(QWidget *parent) :
    QDialog(parent),
    itemModel(new QStandardItemModel()),
    proxyModel(new QSortFilterProxyModel()),
    ui(new Ui::QContactChooserDialog)
{
    ui->setupUi(this);
    proxyModel->setSourceModel(itemModel);
    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    proxyModel->setFilterKeyColumn(0);
    proxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);
    proxyModel->setDynamicSortFilter(true);
    ui->listView->setModel(proxyModel);
    ui->listView->installEventFilter(this);
}

QContactChooserDialog::~QContactChooserDialog()
{
    delete ui;
}

void QContactChooserDialog::clear()
{
    itemModel->clear();
    proxyModel->setFilterRegExp("");
}

int QContactChooserDialog::count() const
{
    return itemModel->rowCount();
}

void QContactChooserDialog::insertItem( QStandardItem * item, bool isSelected )
{
   itemModel->appendRow(item);
   if (isSelected) {
       ui->listView->selectionModel()->select(proxyModel->mapFromSource(item->index()), QItemSelectionModel::Select);
   }
}

QList<QStandardItem *> QContactChooserDialog::selectedItems() const
{
    QModelIndexList indices = proxyModel->mapSelectionToSource(ui->listView->selectionModel()->selection()).indexes();
    QList<QStandardItem *> answer;
    for (int i=0; i < indices.count(); i++) {
	QStandardItem *item = itemModel->itemFromIndex(indices.at(i));
	if (item)
	    answer.append(item);
    }

    return answer;
}

//void QContactChooserDialog::show() {
void QContactChooserDialog::setLayoutDirection(Qt::LayoutDirection direction)
{
    ui->listView->setLayoutDirection(direction);

}

bool QContactChooserDialog::eventFilter(QObject *target, QEvent *event)
{
    bool answer = false;

    if (target == ui->listView) {
	if (event->type() == QEvent::KeyPress) {
	    QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
	    QString text = keyEvent->text();
	    QString filterText = proxyModel->filterRegExp().pattern();
	    if (keyEvent->key() == Qt::Key_Backspace) {
		filterText.chop(1);
		proxyModel->setFilterRegExp(filterText);
	    }
	    else if (!text.isEmpty()) {
		filterText.append(text);
		proxyModel->setFilterRegExp(filterText);
	    }
	    qDebug() << "filterText:" << filterText << " length:" << filterText.length();
	    qDebug() << "proxyModel rowCount " << proxyModel->rowCount();
	    answer = true;
	} else if (event->type() == QEvent::Show) {
	    proxyModel->sort(0);
	    // standard event processing
	    answer = QObject::eventFilter(target, event);
	} else {
	    // standard event processing
	    answer = QObject::eventFilter(target, event);
	}
    }
    else {
	answer = QObject::eventFilter(target, event);
    }
    return answer;
}

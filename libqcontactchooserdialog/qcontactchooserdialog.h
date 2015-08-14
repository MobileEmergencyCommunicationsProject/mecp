#ifndef QCONTACTCHOOSERDIALOG_H
#define QCONTACTCHOOSERDIALOG_H

#include <qcontactchooserdialog_global.h>

#include <QDialog>
#include <QEvent>
#include <QList>
#include <QListWidgetItem>
#include <QSortFilterProxyModel>
#include <QStandardItemModel>

namespace Ui {
    class QContactChooserDialog;
}

class QCONTACTCHOOSERDIALOGSHARED_EXPORT QContactChooserDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QContactChooserDialog(QWidget *parent = 0);
    ~QContactChooserDialog();
    void clear();
    int count() const;
    void insertItem(QStandardItem *item, bool isSelected);
//    QStandardItem *item(int row) const;
    QList<QStandardItem *> selectedItems() const;
    void setLayoutDirection(Qt::LayoutDirection direction);

protected:
    bool eventFilter(QObject *target, QEvent *event);

private:
    QStandardItemModel *itemModel;
    QSortFilterProxyModel *proxyModel;
    Ui::QContactChooserDialog *ui;
};

#endif // QCONTACTCHOOSERDIALOG_H

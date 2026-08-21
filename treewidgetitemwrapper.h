#ifndef TREEWIDGETITEMWRAPPER_H
#define TREEWIDGETITEMWRAPPER_H

#include <QObject>
#include<QTreeWidgetItem>


class TreeWidgetItemWrapper : public QObject
{
    Q_OBJECT
public:
    explicit TreeWidgetItemWrapper(QTreeWidgetItem *item, QObject *parent = nullptr) : QObject(parent), m_item(item) {}

signals:
    void itemClicked(QTreeWidgetItem *item, int column);

public slots:
    void handleItemClicked(int column)
    {
        emit itemClicked(m_item, column);
    }

private:
    QTreeWidgetItem *m_item;
};


#endif // TREEWIDGETITEMWRAPPER_H

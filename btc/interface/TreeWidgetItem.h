#pragma once

#include <QTreeWidgetItem>
#include <QObject>

namespace interface{
    class TreeWidgetItem : public QObject, public QTreeWidgetItem{
        Q_OBJECT

        public:
            TreeWidgetItem(int type = QTreeWidgetItem::Type) : QTreeWidgetItem(type){}
            TreeWidgetItem (const QTreeWidgetItem & other) : QTreeWidgetItem (other){}

            ~TreeWidgetItem(){}

        public slots:
            void setStatus(QString status){
                this->setText(4, status);
            }
    };
}

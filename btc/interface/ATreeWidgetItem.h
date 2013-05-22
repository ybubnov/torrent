#pragma once

#include <QTreeWidgetItem>
#include <QObject>

namespace interface{
    class ATreeWidgetItem : public QObject, public QTreeWidgetItem{
        Q_OBJECT

        public:
            ATreeWidgetItem(int type = QTreeWidgetItem::Type) : QTreeWidgetItem(type){}
            ATreeWidgetItem (const QTreeWidgetItem & other) : QTreeWidgetItem (other){}

            ~ATreeWidgetItem(){}

        public slots:
            void setStatus(QString status){
                this->setText(4, status);
            }
    };
}

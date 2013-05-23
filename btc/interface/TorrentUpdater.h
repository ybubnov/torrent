#pragma once

#include <QObject>
#include <QProgressBar>
#include <QString>

#include "network.h"
#include "TreeWidgetItem.h"

namespace interface{
    typedef class TorrentUpdater : public QObject, public network::bittorrent::io::notifiable{
        Q_OBJECT

        private:
            QProgressBar* _bar;
            TreeWidgetItem* _item;

        public:
            TorrentUpdater(QProgressBar* bar, TreeWidgetItem* item){
                _bar = bar;
                _item = item;

                connect(this, SIGNAL(update_bar(int)), _bar, SLOT(setValue(int)));
                connect(this, SIGNAL(update_status(QString)), _item, SLOT(setStatus(QString)));
            }

            void notify(double percents){
                emit update_bar((int)percents);
                emit update_status(QString::number(percents, 'f', 2) + " %");
            }

        signals:
            void update_bar(int);
            void update_status(QString);
        }torrent_updater;
}

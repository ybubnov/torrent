#pragma once

#include <QMainWindow>
#include <QProgressBar>
#include <QFileDialog>
#include <QMessageBox>
#include <list>

#include "network.h"
#include "TorrentRow.h"
#include "DownloadDialog.h"

namespace Ui {
    class TorrentWindow;
}

namespace interface{

    typedef class TorrentWindow : public QMainWindow{
            Q_OBJECT

        public:
            explicit TorrentWindow(QWidget *parent = 0);
            ~TorrentWindow();

            void add_row(utils::row row);

        private slots:
            void add_handle();
            void start_new_handle();

        private:
            interface::dialog* torrent_dialog;
            std::list<QProgressBar*> bar_list;
            std::list<utils::row> file_list;

            Ui::TorrentWindow *ui;
            QMessageBox* message_box;
    }window;

}

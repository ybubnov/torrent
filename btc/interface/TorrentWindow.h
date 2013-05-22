#pragma once

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <list>

#include "DownloadDialog.h"
#include "ATreeWidgetItem.h"
#include "TorrentUpdater.h"

namespace Ui {
    class TorrentWindow;
}

namespace interface{
    typedef class TorrentWindow : public QMainWindow{
            Q_OBJECT
        private:

        public:
            explicit TorrentWindow(QWidget *parent = 0);
            ~TorrentWindow();

            void add_row(std::wstring file_name, std::string file_size);

        private slots:
            void add_handle();
            void start_handle();
            void stop_handle();
            void delete_handle();

            void start_new_handle();
            void cancel_handle();

        private:
            std::vector<network::bittorrent::protocol*> protocol_list;
            std::vector<network::bittorrent::file_parser*> parser_list;
            std::vector<torrent_updater*> updater_list;
            std::vector<QProgressBar*> bar_list;

            interface::dialog* torrent_dialog;
            Ui::TorrentWindow *ui;
            QMessageBox* message_box;
    }window;
}

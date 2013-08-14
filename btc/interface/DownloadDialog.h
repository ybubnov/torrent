#pragma once

#include <QDialog>
#include <QFileDialog>

#include "FileIconProvider.h"
#include "network.h"

namespace Ui{
    class DownloadDialog;
}

namespace interface{
    typedef class DownloadDialog : public QDialog{
            Q_OBJECT

        public:
            explicit DownloadDialog(QWidget *parent = 0);
            ~DownloadDialog();

            static QString size(int64_t length);                                    //format file size in human readable format
            static QString size(std::string& length);

            void name(std::wstring value);                                          //window attributes
            void comment(std::wstring value);
            void date(std::string value);

            void download_list(std::list<network::bittorrent::file_stat> list); //list of downloadable files
            std::wstring get_folder();                                              //load folder

        private slots:
            void browse_handle();

        signals:
            void start_signal();
            void cancel_signal();

        private:

            QString directory;
            Ui::DownloadDialog *ui;
    }dialog;
}

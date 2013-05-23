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

            static QString size(int64_t length);
            static QString size(std::string& length);

            void name(std::wstring value);
            void comment(std::wstring value);
            void date(std::string value);

            void download_list(std::list<network::bittorrent::download_file> list);
            std::wstring get_folder();

        public slots:
            void browse_handle();

        signals:
            void start_signal();
            void cancel_signal();

        private:
            std::vector<std::wstring> split(std::wstring& str, wchar_t delimeter);

            QString directory;
            Ui::DownloadDialog *ui;
    }dialog;
}

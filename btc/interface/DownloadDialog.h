#pragma once

#include <QDialog>
#include <QFileIconProvider>
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

            void name(std::wstring value);
            void comment(std::wstring value);
            void date(std::string value);

            void download_list(std::list<network::bittorrent::download_file> list);

        signals:
            void start_signal();

        private slots:
            void cancel_handle();

        private:
            std::vector<std::wstring> split(std::wstring& str, wchar_t delimeter);

            Ui::DownloadDialog *ui;
    }dialog;
}

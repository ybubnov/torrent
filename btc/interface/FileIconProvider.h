#pragma once

#include <QFileIconProvider>
#include <QFile>

class FileIconProvider{
    private:
        QFileIconProvider provider;

    public:
        FileIconProvider();
        ~FileIconProvider();

        QIcon icon(QString path);
        QIcon icon(QFileIconProvider::IconType type);
};

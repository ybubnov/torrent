#include "DownloadDialog.h"
#include "ui_DownloadDialog.h"

using namespace interface;

DownloadDialog::DownloadDialog(QWidget *parent) : QDialog(parent), ui(new Ui::DownloadDialog){
    ui->setupUi(this);
    ui->downloadListTreeWidget->setColumnWidth(0, 350);


    connect(ui->cancelPushButton, SIGNAL(clicked()), this, SIGNAL(cancel_signal()));
    connect(ui->okPushButton, SIGNAL(clicked()), this, SIGNAL(start_signal()));
    connect(ui->browsePushButton, SIGNAL(clicked()), this, SLOT(browse_handle()));
}

void DownloadDialog::comment(std::wstring value){
    ui->torrentCommentLabel->setText(QString::fromStdWString(value).left(50));
}

void DownloadDialog::date(std::string value){
    ui->torrentDateLabel->setText(QString::fromStdString(value).left(50));
}

void DownloadDialog::name(std::wstring value){
    setWindowTitle(QString::fromStdWString(value) + tr(" - Add New Torrent"));

    ui->torrentNameLabel->setText(QString::fromStdWString(value).left(50));
}

QString DownloadDialog::size(int64_t length){
    double num = (double)length;
    QStringList list;
    list << tr("kB") << tr("MB") << tr("GB") << tr("TB");

    QStringListIterator i(list);
    QString unit(tr("bytes"));

    while(num >= 1024.0 && i.hasNext()){
        unit = i.next();
        num /= 1024.0;
    }

    return QString().setNum(num, 'f', 2) + " " + unit;
}

QString DownloadDialog::size(std::string& length){
    int64_t ilength;
    std::stringstream flow;
    flow << length;
    flow >> ilength;

    return size(ilength);
}

std::wstring DownloadDialog::get_folder(){
    if(!directory.size()){
        directory = ui->downloadFodlerLineEdit->text();
    }

    directory += "/";
    std::cout << directory.toStdString() << std::endl;

    return directory.toStdWString();
}

void DownloadDialog::browse_handle(){
    directory = QFileDialog::getExistingDirectory(
                this, tr("Open Directory"), QString(),
                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if(directory.size()){
        ui->downloadFodlerLineEdit->setText(directory);
    }
}


void DownloadDialog::download_list(std::list<network::bittorrent::file_stat> list){
    int64_t total_length = 0;

    QTreeWidgetItem* top_item = 0;
    FileIconProvider provider;

    for(std::list<network::bittorrent::file_stat>::iterator it = list.begin(); it != list.end(); it++){
        network::bittorrent::file_stat df = *it;

        total_length += df.length;

        QStringList path = QString::fromStdWString(df.path).split("\\");

        if(ui->downloadListTreeWidget->findItems(path[0], Qt::MatchFixedString).isEmpty()){
            top_item = new QTreeWidgetItem();
            top_item->setText(0, path[0]);

            if(path.size() == 1){
                top_item->setIcon(0, provider.icon(path[0]));
                top_item->setText(1, size(df.length));
                ui->downloadListTreeWidget->addTopLevelItem(top_item);
                continue;
            }else{
                top_item->setIcon(0, provider.icon(QFileIconProvider::Folder));
            }

            ui->downloadListTreeWidget->addTopLevelItem(top_item);
        }

        QTreeWidgetItem* parent_item = top_item;

        for(int i = 1; i < path.size() - 1; i++){
            bool is_exists = false;

            for(int j = 0; j < parent_item->childCount(); j++){
                if(path[i] == parent_item->child(j)->text(0)){
                    is_exists = true;
                    parent_item = parent_item->child(j);
                    break;
                }
            }

            if(!is_exists){
                parent_item = new QTreeWidgetItem(parent_item);
                parent_item->setText(0, path[i]);
                parent_item->setIcon(0, provider.icon(QFileIconProvider::Folder));
            }
        }

        QTreeWidgetItem* children_item = new QTreeWidgetItem(parent_item);
        children_item->setText(0, path.last());
        children_item->setIcon(0, provider.icon(path.last()));
        children_item->setText(1, size(df.length));
    }

    if(list.size() == 1){
        ui->torrentSizeLabel->setText(tr("1 file, ") + size(total_length));
    }else{
        ui->torrentSizeLabel->setText(QString::number(list.size()) + tr(" files, ") + size(total_length));
    }
}

DownloadDialog::~DownloadDialog(){
    delete ui;
}

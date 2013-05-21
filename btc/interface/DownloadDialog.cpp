#include "DownloadDialog.h"
#include "ui_DownloadDialog.h"

using namespace interface;

DownloadDialog::DownloadDialog(QWidget *parent) : QDialog(parent), ui(new Ui::DownloadDialog){
    ui->setupUi(this);
    ui->downloadListTreeWidget->setColumnWidth(0, 350);

    connect(ui->cancelPushButton, SIGNAL(clicked()), this, SLOT(cancel_handle()));
    connect(ui->okPushButton, SIGNAL(clicked()), this, SIGNAL(start_signal()));
}

void DownloadDialog::comment(std::wstring value){
    ui->torrentCommentLabel->setText(QString::fromStdWString(value));
}

void DownloadDialog::date(std::string value){
    ui->torrentDateLabel->setText(QString::fromStdString(value));
}

void DownloadDialog::name(std::wstring value){
    ui->torrentNameLabel->setText(QString::fromStdWString(value));
}

void DownloadDialog::cancel_handle(){
    close();
}

std::vector<std::wstring> DownloadDialog::split(std::wstring &wstring, wchar_t delimeter){
    std::wstringstream flow(wstring);
    std::vector<std::wstring> elements;
    std::wstring item;

    while(std::getline(flow, item, delimeter)) {
        elements.push_back(item);
    }

    return elements;
}

void DownloadDialog::download_list(std::list<network::bittorrent::download_file> list){

    for(std::list<network::bittorrent::download_file>::iterator it = list.begin(); it != list.end(); it++){
        network::bittorrent::download_file df = *it;
        std::stringstream flow;
        flow << df.length;

        QTreeWidgetItem* item = new QTreeWidgetItem(QTreeWidgetItem::UserType);

        std::vector<std::wstring> path = split(df.path, '\\');

        /*if(path.size() == 1){
            item->setText(0, QString::fromStdWString(df.path));
        }else{
            for(unsigned int i = 1; i < path.size(); i++){
                QList<QTreeWidgetItem*> list = ui->downloadListTreeWidget->findItems(QString::fromStdWString(path[i]));

                if(list.size() != 0){
                    list[0]->
                }else{

                }
            }
        }*/


        item->setText(0, QString::fromStdWString(path[path.size() - 1]));
        item->setText(1, QString::fromStdString(flow.str()));

        ui->downloadListTreeWidget->addTopLevelItem(item);
    }

}

DownloadDialog::~DownloadDialog(){
    delete ui;
}

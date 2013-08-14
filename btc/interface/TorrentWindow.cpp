#include "TorrentWindow.h"
#include "ui_TorrentWindow.h"

#include <cstdlib>
#include <sstream>


using namespace interface;

TorrentWindow::TorrentWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::TorrentWindow){
    setWindowIcon(QIcon(":/icons/icons/window.png"));

    ui->setupUi(this);
    ui->torrentListTreeWidget->setRootIsDecorated(false);
    ui->torrentListTreeWidget->resizeColumnToContents(0);
    ui->torrentListTreeWidget->resizeColumnToContents(4);
    ui->torrentListTreeWidget->resizeColumnToContents(5);

    ui->torrentListTreeWidget->setColumnWidth(1, 200);
    ui->torrentListTreeWidget->setColumnWidth(2, 70);
    ui->torrentListTreeWidget->setColumnWidth(3, 200);

    connect(ui->actionAdd, SIGNAL(triggered()), this, SLOT(add_handle()));
    connect(ui->actionStopTorrent, SIGNAL(triggered()), this, SLOT(stop_handle()));
    connect(ui->actionStartTorrent, SIGNAL(triggered()), this, SLOT(start_handle()));
    connect(ui->actionRemoveTorrent, SIGNAL(triggered()), this, SLOT(delete_handle()));

    connect(ui->torrentListTreeWidget, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(open_folder(QModelIndex)));
}

TorrentWindow::~TorrentWindow(){
    delete ui;
}

void TorrentWindow::closeEvent(QCloseEvent *event){
    this->hide();
    while(!garbage_collect()){
        boost::this_thread::sleep(boost::posix_time::milliseconds(700));
        std::cout << "TICK" << std::endl;
    }

    std::cout << "GARBAGE COLLECTED!!!" << std::endl;

}

bool TorrentWindow::garbage_collect(){
    bool collected = true;

    for(std::vector<network::bittorrent::protocol*>::iterator protocol_ptr = waste_list.begin();
        protocol_ptr != waste_list.end(); protocol_ptr++){

        if(!(*protocol_ptr)->alive()){
            delete (*protocol_ptr);
            collected = true;
            *protocol_ptr = 0;

            std::cout << "DELETED" << std::endl;
        }else{
            collected = false;
        }
    }

    std::vector<network::bittorrent::protocol*>::iterator ptr
            = std::remove(waste_list.begin(), waste_list.end(), (network::bittorrent::protocol*)0);

    waste_list.erase(ptr, waste_list.end());

    std::cout << "LAST = " << waste_list.size() << std::endl;

    return collected;
}

void TorrentWindow::open_folder(QModelIndex index){
   network::bittorrent::protocol* protocol = protocol_list[index.row()];

    QDesktopServices::openUrl(QUrl(QString::fromStdWString(protocol->folder())));
}

void TorrentWindow::start_new_handle(){
    network::bittorrent::file_parser* torrent_file = parser_list[parser_list.size() - 1];

    add_row(torrent_file->name(), torrent_file->size());
    torrent_dialog->close();

    torrent_updater* subscriber = updater_list[updater_list.size() - 1];
    network::bittorrent::file_parser* parser = parser_list[parser_list.size() - 1];

    network::bittorrent::protocol* protocol =
            new network::bittorrent::protocol(subscriber, *parser, torrent_dialog->get_folder());
    protocol_list.push_back(protocol);

    protocol->yeild();
}

void TorrentWindow::cancel_handle(){
    torrent_dialog->close();

    if(parser_list.size()){
        parser_list.pop_back();
    }
}

void TorrentWindow::delete_handle(){
    QList<QTreeWidgetItem*> selected = ui->torrentListTreeWidget->selectedItems();
    QString index_str;
    unsigned long index;

    for(QList<QTreeWidgetItem*>::Iterator item_ptr = selected.begin(); item_ptr != selected.end(); item_ptr++){
        index_str = (*item_ptr)->text(0);
        index = index_str.toLong();

        protocol_list[index - 1]->interrupt();

        network::bittorrent::protocol* ptr = protocol_list[index - 1];
        waste_list.push_back(ptr);

        std::cout << "1: " << (long)ptr << std::endl;

        delete parser_list[index - 1];
        delete updater_list[index - 1];
        delete bar_list[index - 1];

        std::cout << "2: " << (long)ptr << std::endl;

        protocol_list[index - 1] = 0;
        parser_list[index - 1] = 0;
        updater_list[index - 1] = 0;
        bar_list[index - 1] = 0;

        std::cout << "3: " << (long)ptr << std::endl;

        std::vector<network::bittorrent::protocol*>::iterator protocol_ptr
                = std::remove(protocol_list.begin(), protocol_list.end(), (network::bittorrent::protocol*)0);

        std::vector<network::bittorrent::file_parser*>::iterator parser_ptr
                = std::remove(parser_list.begin(), parser_list.end(), (network::bittorrent::file_parser*)0);

        std::vector<torrent_updater*>::iterator updated_ptr
                = std::remove(updater_list.begin(), updater_list.end(), (torrent_updater*)0);

        std::vector<QProgressBar*>::iterator bar_ptr
                = std::remove(bar_list.begin(), bar_list.end(), (QProgressBar*)0);

        std::cout << "4: " << (long)ptr << std::endl;

        protocol_list.erase(protocol_ptr, protocol_list.end());
        parser_list.erase(parser_ptr, parser_list.end());
        updater_list.erase(updated_ptr, updater_list.end());
        bar_list.erase(bar_ptr, bar_list.end());

        std::cout << "5: " << (long)ptr << std::endl;

        delete ui->torrentListTreeWidget->topLevelItem(index - 1);

        for(unsigned int i = 0; i < protocol_list.size(); i++){
            ui->torrentListTreeWidget->topLevelItem(i)->setText(0, QString::number(i + 1));
        }
    }

    bencode::element::gc();
}

void TorrentWindow::stop_handle(){
    //garbage_collect();

    QList<QTreeWidgetItem*> selected = ui->torrentListTreeWidget->selectedItems();
    QString index_str;
    unsigned long index;

    for(QList<QTreeWidgetItem*>::Iterator item_ptr = selected.begin(); item_ptr != selected.end(); item_ptr++){
        index_str = (*item_ptr)->text(0);
        index = index_str.toLong();

        //if(index - 1 < protocol_list.size()){
            protocol_list[index - 1]->interrupt();
            (*item_ptr)->setText(5, tr("Stopped"));
        //}
    }
}

void TorrentWindow::start_handle(){
    garbage_collect();

    QList<QTreeWidgetItem*> selected = ui->torrentListTreeWidget->selectedItems();
    QString index_str;
    unsigned long index;

    for(QList<QTreeWidgetItem*>::Iterator item_ptr = selected.begin(); item_ptr != selected.end(); item_ptr++){
        index_str = (*item_ptr)->text(0);
        index = index_str.toLong();

        if(index - 1 < protocol_list.size()){
            if(!protocol_list[index - 1]->alive()){
                protocol_list[index - 1]->restart();
                (*item_ptr)->setText(5, tr("Started"));
            }
        }
    }
}

void TorrentWindow::add_handle(){
    //garbage_collect();

    QString file_name = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Files (*.torrent)"));

    if(file_name.size()){
        try{
            bencode::provider::stream provider(file_name.toStdWString());
            bencode::parser parser(provider);
            bencode::element* node = parser.node();
            network::bittorrent::file_parser* torrent_file = new network::bittorrent::file_parser(node);

            bencode::element::gc();

            parser_list.push_back(torrent_file);

            torrent_dialog = new interface::dialog(this);
            torrent_dialog->comment(torrent_file->comment());
            torrent_dialog->date(torrent_file->creation_date());
            torrent_dialog->name(torrent_file->name());
            torrent_dialog->download_list(torrent_file->files());

            torrent_dialog->show();

            connect(torrent_dialog, SIGNAL(start_signal()), this, SLOT(start_new_handle()));
            connect(torrent_dialog, SIGNAL(cancel_signal()), this, SLOT(cancel_handle()));
        }catch(std::exception e){
            QString message = tr("Unable to load \n'") + file_name + tr("': torrent is not valid bencoding");

            message_box = new QMessageBox(this);
            message_box->setText(message);
            message_box->show();
        }
    }
}


void TorrentWindow::add_row(std::wstring file_name, std::string file_size){
    QProgressBar* bar = new QProgressBar();

    bar->setValue(0);
    bar->setStyleSheet("QProgressBar{color: transparent; height:16px; margin:2px;}");

    bar_list.push_back(bar);

    std::stringstream position_flow;
    position_flow << bar_list.size();
    std::string position(position_flow.str());


    TreeWidgetItem* item = new TreeWidgetItem(QTreeWidgetItem::UserType);
    item->setText(0, QString::fromStdString(position));
    item->setText(1, QString::fromStdWString(file_name));
    item->setText(2, DownloadDialog::size(file_size));
    item->setText(4, QString("0.00 %"));
    item->setText(5, tr("Started"));

    updater_list.push_back(new torrent_updater(bar, item));

    ui->torrentListTreeWidget->addTopLevelItem(item);

    ui->torrentListTreeWidget->setItemWidget(
                ui->torrentListTreeWidget->topLevelItem(bar_list.size() - 1),
                3,
                bar);
}

#include "managerdialog.h"
#include "ui_managerdialog.h"
#include "data.h"
#include <QtWidgets>

managerDialog::managerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::managerDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("经理");
    setBox1();
    setBox2();
    setBox3();
    connect(ui->dishTable->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(sortDishes()));

    ui->stackedWidget->addWidget(ui->page1);
    ui->stackedWidget->addWidget(ui->page2);
    connect(ui->button, SIGNAL(clicked(bool)), this, SLOT(switchPage()));
}

managerDialog::~managerDialog()
{
    delete ui;
}

void managerDialog::setBox1(){
    //chefTable
    ui->chefTable->verticalHeader()->setVisible(false);
    ui->chefTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->chefTable->setColumnCount(2);
    for(int i=0;Data::chef[i].id;i++){
        ui->chefTable->setRowCount(ui->chefTable->rowCount()+1);
        ui->chefTable->setItem(i, 0, new QTableWidgetItem(QString("%1").arg(Data::chef[i].id)));
        ui->chefTable->setItem(i, 1, new QTableWidgetItem(QString("%1").arg(Data::chef[i].history)));
    }
    ui->chefTable->setHorizontalHeaderLabels(chefHeader);
    ui->chefTable->horizontalHeader()->setStyleSheet("QHeaderView::section{background:bisque;}");
    ui->chefTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //waiterTable
    ui->waiterTable->verticalHeader()->setVisible(false);
    ui->waiterTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->waiterTable->setColumnCount(3);
    for(int i=0;Data::waiter[i].id;i++){
        ui->waiterTable->setRowCount(ui->waiterTable->rowCount()+1);
        ui->waiterTable->setItem(i, 0, new QTableWidgetItem(QString("%1").arg(Data::waiter[i].id)));
        ui->waiterTable->setItem(i, 1, new QTableWidgetItem(QString("%1").arg(Data::waiter[i].history)));
        ui->waiterTable->setItem(i, 1, new QTableWidgetItem(QString("%1").arg(Data::waiter[i].rating)));
    }
    ui->waiterTable->setHorizontalHeaderLabels(waiterHeader);
    ui->waiterTable->horizontalHeader()->setStyleSheet("QHeaderView::section{background:bisque;}");
    ui->waiterTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //layout
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(ui->chefTable);
    layout->addWidget(ui->waiterTable);
    ui->box1->setLayout(layout);
}

void managerDialog::setBox2(){
    ui->dishTable->verticalHeader()->setVisible(false);
    ui->dishTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->dishTable->setColumnCount(5);
    int k = 0;
    ui->dishTable->setRowCount(Dish::count);
    QHashIterator<int, Dish*> i(Data::hash1);
    while(i.hasNext()){
        i.next();
            ui->dishTable->setItem(k, 0, new QTableWidgetItem(QString("%1").arg(i.value()->id)));
            ui->dishTable->setItem(k, 1, new QTableWidgetItem(i.value()->name));
            if(i.value()->special)
                ui->dishTable->setItem(k, 2, new QTableWidgetItem(QIcon(":/buttons/star.png"), "推荐"));
            else
                ui->dishTable->setItem(k, 2, new QTableWidgetItem(QIcon(":/buttons/graystar.png"), "普通"));
            ui->dishTable->setItem(k, 3, new QTableWidgetItem(i.value()->history));
            ui->dishTable->setItem(k, 4, new QTableWidgetItem(QString("%1").arg(i.value()->rating)));
       k++;
    }
    sortDishes(0);
    sortDishes(0);
    ui->dishTable->setHorizontalHeaderLabels(dishHeader);
    ui->dishTable->horizontalHeader()->setStyleSheet("QHeaderView::section{background:bisque;}");
    ui->dishTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void managerDialog::setBox3(){
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setColumnCount(6);
    int i=0;
    for(orderInfo* info : Data::list){
        i++;
        ui->tableWidget->setRowCount(i);
        int id = info->dishid;
        int status = info->status;
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QString("%1").arg(info->id)));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(QString("%1").arg(info->tableid)));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(Data::hash1[id]->name));
        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(QString("%1").arg(info->count)));
        ui->tableWidget->setItem(i, 4, new QTableWidgetItem(Data::hash1[id]->price));
        if(status == -1){
            ui->tableWidget->setItem(i, 5, new QTableWidgetItem("等待厨师"));
        }
        else if(status == 0){
            ui->tableWidget->setItem(i, 5, new QTableWidgetItem("正在制作"));
        }
        else{
            ui->tableWidget->setItem(i, 5, new QTableWidgetItem("已完成"));
        }
    }
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(ui->tableWidget);
    ui->box3->setLayout(layout);
    ui->dishTable->setHorizontalHeaderLabels(streamHeader);
    ui->dishTable->horizontalHeader()->setStyleSheet("QHeaderView::section{background:bisque;}");
    ui->dishTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void managerDialog::sortDishes(int column)
{
    static bool f = true;
    ui->dishTable->sortByColumn(column, f ? Qt::AscendingOrder : Qt::DescendingOrder);
    f = !f;
}

void managerDialog::switchPage(){
    int index = ui->stackedWidget->currentIndex();
    if(index == 1)
        index = 0;
    else if(index == 0)
        index = 1;
    ui->stackedWidget->setCurrentIndex(index);
}
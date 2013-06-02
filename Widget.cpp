#include "Widget.hpp"
#include "ui_Widget.h"

#include <QGraphicsScene>
#include <qDebug>

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);

    // How to use the customized slider.
    ui->slider->setFixedHeight(40);
    ui->slider->setMarkHandleSize(QSize(10, 15));
    ui->slider->setRoundHandleSize(QSize(30, 30));
    ui->slider->setMargins(QMargins(20, 10, 20, 10));

    connect(ui->hideButton,  SIGNAL(clicked()), this, SLOT(hideSelectedHandles()));
    connect(ui->showButton,  SIGNAL(clicked()), this, SLOT(showAllHandles()));
    connect(ui->printButton, SIGNAL(clicked()), this, SLOT(printSliderValues()));
}

Widget::~Widget() {
    delete ui;
}

void Widget::printSliderValues() {
    qDebug() << ui->slider->getValues();
}

void Widget::hideSelectedHandles() {
    ui->slider->hideSelectedHandles();
}

void Widget::showAllHandles() {
    ui->slider->showAllHandles();
}

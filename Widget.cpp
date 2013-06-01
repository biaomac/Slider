#include "Widget.hpp"
#include "ui_Widget.h"

#include <QGraphicsScene>
#include <qDebug>

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);

    // How to use the customized slider.
    ui->slider->setFixedHeight(40);
    ui->slider->setHandleSize(QSize(30, 30));
    ui->slider->setMargins(QMargins(20, 10, 20, 10));

    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(printSliderValues()));
}

Widget::~Widget() {
    delete ui;
}

void Widget::printSliderValues() {
    qDebug() << ui->slider->getValues();
}

#include "dialog.h"
#include "ui_dialog.h"
#include <QFileDialog>

Dialog::Dialog(QWidget *parent) : QDialog(parent), ui(new Ui::Dialog) {
    ui->setupUi(this);
    
    connect(ui->browseButton, SIGNAL(released()), this, SLOT(chooseFile()));
}

Dialog::~Dialog() {
    delete ui;
}

void Dialog::chooseFile() {
    ui->filenameTextEdit->setPlainText(QFileDialog::getOpenFileName(this, "Choose a ROM", "", "Image Files (*.nds)"));
}

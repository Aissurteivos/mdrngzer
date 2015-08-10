#include "dialog.h"
#include "ui_dialog.h"
#include <QFileDialog>
#include "rom.h"

Dialog::Dialog(QWidget *parent) : QDialog(parent), ui(new Ui::Dialog) {
    ui->setupUi(this);
    
    connect(ui->browseButton, SIGNAL(released()), this, SLOT(chooseFile()));
}

Dialog::~Dialog() {
    delete ui;
}

void Dialog::chooseFile() {
    QString temp = QFileDialog::getOpenFileName(this, "Choose a ROM", "", "Image Files (*.nds)");
    filename = temp.toUtf8().constData();
    std::cout << "blah " << filename << std::endl;
    ui->filenameTextEdit->setPlainText(temp);
}

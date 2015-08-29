#include "dialog.h"
#include "ui_dialog.h"
#include <QFileDialog>
#include <QMessageBox>
#include "rom.h"

Dialog::Dialog(QWidget *parent) : QDialog(parent), ui(new Ui::Dialog) {
    ui->setupUi(this);
    
    connect(ui->browseButton, SIGNAL(released()), this, SLOT(chooseFile()));
    connect(ui->randomizeButton, SIGNAL(released()), this, SLOT(randomize()));
}

Dialog::~Dialog() {
    delete ui;
}

void Dialog::chooseFile() {
    QString temp = QFileDialog::getOpenFileName(this, "Choose a ROM", "", "Image Files (*.nds)");
    ui->filenameTextEdit->setPlainText(temp);
}

void Dialog::randomize() {
    try {
        ROM rom(314159);
        rom.open(ui->filenameTextEdit->toPlainText().toUtf8().constData());

        rom.randItems();

        if (ui->PokemonSpawnEnable->isChecked())
            rom.randPokemon();

        if (ui->AbilitiesEnable->isChecked())
            rom.randAbilities();

        if (ui->TypeEnable->isChecked())
            rom.randTypes();

        if (ui->IQGroupEnable->isChecked())
            rom.randIQs();

        if (ui->MusicEnable->isChecked())
            rom.randMusic();

        if (ui->TerrainEnable->isChecked())
            rom.randTerrain();

        if (ui->MovesetEnable->isChecked())
            rom.randMoveset();

        rom.save(QFileDialog::getSaveFileName(this, "Choose a Destination", "", "Image FIles (*.nds)")
                 .toUtf8().constData());

    } catch (const std::string &error) {
        QMessageBox::information(this, "Error", tr(error.c_str()));
    }
}

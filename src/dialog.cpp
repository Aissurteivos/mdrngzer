#include "dialog.h"
#include "ui_dialog.h"
#include <QFileDialog>
#include <QMessageBox>
#include <ctime>
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
        unsigned seed;
        if (!(ui->SeedEnable->isChecked()))
            seed = ui->SeedBox->value();
        else
            seed = std::time(NULL);

        ROM rom(seed);

        rom.open(ui->filenameTextEdit->toPlainText().toUtf8().constData());

        ui->progressBar->setValue(0);

        if(!rom.loadFile("rom/data/BALANCE/mappa_s.bin")) {
            throw std::string("ROM: Failed to open mappa_s.bin");
        }

        if (ui->TerrainEnable->isChecked())
            rom.randTerrain();
        ui->progressBar->setValue(10);

        if (ui->MusicEnable->isChecked())
            rom.randMusic();
        ui->progressBar->setValue(20);

        if (ui->PokemonSpawnEnable->isChecked())
            rom.randPokemon();
        ui->progressBar->setValue(30);

        if (ui->ItemSpawnEnable->isChecked())
            rom.randItems();
        ui->progressBar->setValue(40);

        rom.saveFile("rom/data/BALANCE/mappa_s.bin");

        if (ui->TypeEnable->isChecked())
            rom.randTypes(ui->TypePercentBox->value());
        ui->progressBar->setValue(50);

        if (ui->IQGroupEnable->isChecked())
            rom.randIQs();
        ui->progressBar->setValue(60);

        if (ui->AbilitiesEnable->isChecked())
            rom.randAbilities(ui->AbilityPercentBox->value());
        ui->progressBar->setValue(70);



        if (ui->MovesetEnable->isChecked())
            rom.randMoveset();
        ui->progressBar->setValue(80);



        if (ui->TextEnable->isChecked())
            rom.randText();
        ui->progressBar->setValue(90);



        ui->progressBar->setValue(100);

        rom.save(QFileDialog::getSaveFileName(this, "Choose a Destination", "", "Image FIles (*.nds)")
                 .toUtf8().constData());

        QMessageBox::information(this, "Complete", "Randomization complete!");


    } catch (const std::string &error) {
        QMessageBox::information(this, "Error", tr(error.c_str()));
    }
}

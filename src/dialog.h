#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <iostream>
#include <QDebug>

namespace Ui {
    class Dialog;
}

class Dialog : public QDialog {
    Q_OBJECT
    
public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    
public slots:
    void chooseFile();
    void randomize();

private slots:

private:
    Ui::Dialog *ui;
    std::string filename;
};

#endif // DIALOG_H

#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <iostream>

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
    
private:
    Ui::Dialog *ui;
    std::string filename;
};

#endif // DIALOG_H

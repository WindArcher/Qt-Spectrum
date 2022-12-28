#pragma once

#include <QDialog>
#include "dfttest.h"

namespace Ui {
    class Dialog;
}

class TestDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TestDialog( const QString& pathToFile, QWidget *parent = nullptr );
    ~TestDialog();

private:
    Ui::Dialog *ui;
    DFTTest test;
    void FFTtest();
    void FFThctest();
    void FHTtest();
};


#include "test_dialog.h"
#include "ui_test_dialog.h"

TestDialog::TestDialog( const QString& filename, QWidget *parent) : QDialog(parent), ui(new Ui::Dialog)

{
    ui->setupUi(this);
    test.loadFile( filename );
    QStringList horzHeaders;
    horzHeaders << "Sample Size(bytes)" << "Used Memory(bytes)" << "FowardTransform(s)" << "BackwardTransform(s)" << "TotalTime(s)";
    QStringList verticalHeader;
    verticalHeader << "FFT" << "FFThc" << "FHT";
    ui->tableWidget->setRowCount(3);
    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setHorizontalHeaderLabels( horzHeaders );
    ui->tableWidget->setVerticalHeaderLabels( verticalHeader );
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    FFTtest();
    FFThctest();
    FHTtest();
}

TestDialog::~TestDialog()
{
    delete ui;
}

void TestDialog::FFTtest()
{
    TestResult res = test.runFTTtest();
    ui->tableWidget->setItem(0, 0, new QTableWidgetItem( QString::number( res.sampleSize ) ) );
    ui->tableWidget->setItem(0, 1, new QTableWidgetItem( QString::number( res.memoryUsage) ) );
    ui->tableWidget->setItem(0, 2, new QTableWidgetItem( QString::number( res.timeOfTransform) ) );
    ui->tableWidget->setItem(0, 3, new QTableWidgetItem( QString::number( res.timeOfReverseTransfrom ) ) );
    ui->tableWidget->setItem(0, 4, new QTableWidgetItem( QString::number( res.fullTime ) ) );
}

void TestDialog::FFThctest()
{
    TestResult res = test.runFFThctest();
    ui->tableWidget->setItem(1, 0, new QTableWidgetItem( QString::number( res.sampleSize ) ) );
    ui->tableWidget->setItem(1, 1, new QTableWidgetItem( QString::number( res.memoryUsage) ) );
    ui->tableWidget->setItem(1, 2, new QTableWidgetItem( QString::number( res.timeOfTransform) ) );
    ui->tableWidget->setItem(1, 3, new QTableWidgetItem( QString::number( res.timeOfReverseTransfrom ) ) );
    ui->tableWidget->setItem(1, 4, new QTableWidgetItem( QString::number( res.fullTime ) ) );
}

void TestDialog::FHTtest()
{
    TestResult res = test.runFHTtest();
    ui->tableWidget->setItem(2, 0, new QTableWidgetItem( QString::number( res.sampleSize ) ) );
    ui->tableWidget->setItem(2, 1, new QTableWidgetItem( QString::number( res.memoryUsage) ) );
    ui->tableWidget->setItem(2, 2, new QTableWidgetItem( QString::number( res.timeOfTransform) ) );
    ui->tableWidget->setItem(2, 3, new QTableWidgetItem( QString::number( res.timeOfReverseTransfrom ) ) );
    ui->tableWidget->setItem(2, 4, new QTableWidgetItem( QString::number( res.fullTime ) ) );
}

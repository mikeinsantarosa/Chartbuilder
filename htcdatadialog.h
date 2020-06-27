#ifndef HTCDATADIALOG_H
#define HTCDATADIALOG_H

#include <QDialog>
#include <QTextStream>
#include <QStringList>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QStandardItemModel>
#include <QRegExp>
#include <algorithm>


namespace Ui {
class HtcDataDialog;
}

class HtcDataDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HtcDataDialog(QWidget *parent = 0);
    ~HtcDataDialog();

    QStringList * getData();
    QStringList * getHeader();

    bool Init(QString filename);
    QString GetDataFileName();

    int GetNumberOfDataRows();
    int GetNumberOfHeaderRows();

    QVector <int> getSelectedColumnsList();

signals:
    void ColumnSelected();
    void ColumnSelectorClosed();


private slots:
    void on_btnClose_clicked();
   // void on_tvData_sectionClicked(int Value);
    void ColumnHeaderClicked(int Value);

private:
    Ui::HtcDataDialog *ui;

    QStandardItemModel *model;

    QString _dataFileName;
    QString _delimiter;
    QStringList * _rawList;
    QStringList * _listToView;
    QStringList * _FileHeaderList;

    QVector <int> _selectedColumnsList;

    int _firstDataRow;
    int _numberOfDataRows;
    int _numberOfHeaderRows;

    bool _busy;

    int _chartFontSize = 9;
    // table formatting
    QFont _tableFont = QFont("Times New Roman",_chartFontSize, QFont::Normal );

    QString setDelimiter(QString fName);
    int findFirstDataRow(QString delimiter);

    bool getFileData(QStringList * list);
    int setListToView(QStringList * list);
    int setHeaderList(QStringList * list);


    QVector <int> setSelectedColumnsList();
    QVector <int>  StringToList(QString value, QString del);

    // test functions:
    void listColumnList();

    bool isValueInList(QString list, QString value, QString del);

    void loadModel();

};

#endif // HTCDATADIALOG_H

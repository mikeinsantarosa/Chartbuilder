// ++++++++++++++++++++++++++++++++++++++++++++
// Use this file as a source to figure out
// Which column to build charts from
//
//
//
// ++++++++++++++++++++++++++++++++++++++++++++

#include "htcdatadialog.h"
#include "ui_htcdatadialog.h"

HtcDataDialog::HtcDataDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HtcDataDialog)
{
    ui->setupUi(this);

    QString chartTitle = "";
    chartTitle.append("HTC ChartBuilder Data Column Selector");
    this->setWindowTitle(chartTitle);

    QObject::connect(ui->tvData->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(ColumnHeaderClicked(int)));
    ui->tvData->setFont(_tableFont);

    _rawList = new QStringList;
    _listToView = new QStringList;
    _FileHeaderList = new QStringList;
}

HtcDataDialog::~HtcDataDialog()
{
    delete ui;
}

QStringList *HtcDataDialog::getData()
{
    return _listToView;
}

QStringList *HtcDataDialog::getHeader()
{
    return _FileHeaderList;
}

bool HtcDataDialog::Init(QString filename)
{

    _busy = true;
    bool result;

    _dataFileName = filename;

    // figure out what char to use as a delimiter.
    _delimiter = setDelimiter(_dataFileName);

    // fill up a local list with data from the file
    result = getFileData(_rawList);


    if (result == true)
    {
        _firstDataRow = findFirstDataRow(_delimiter);
        _numberOfDataRows = setListToView(_listToView);
        _numberOfHeaderRows = setHeaderList(_FileHeaderList);
        loadModel();

    }

    _busy = false;

    return result;
}


QString HtcDataDialog::GetDataFileName()
{
    return _dataFileName;
}

int HtcDataDialog::GetNumberOfDataRows()
{
    return _numberOfDataRows;
}

int HtcDataDialog::GetNumberOfHeaderRows()
{
    return _numberOfHeaderRows;
}

QVector<int> HtcDataDialog::getSelectedColumnsList()
{

    return _selectedColumnsList;
}

void HtcDataDialog::on_btnClose_clicked()
{
    emit ColumnSelectorClosed();
    close();
}

void HtcDataDialog::ColumnHeaderClicked(int Value)
{

    _selectedColumnsList = setSelectedColumnsList();

   emit ColumnSelected();
}

QString HtcDataDialog::setDelimiter(QString fName)
{
    QString result = "$";

    if (fName.contains(".csv"))
    {
        result = ",";
    }
    else if (fName.contains(".txt"))
    {
        result = "\t";
    }

    return result;

}

int HtcDataDialog::findFirstDataRow(QString delimiter)
{

    QStringList group;
    QString current;
    QString dataItem;
    int numFinds;
    //bool isNumber;
    int result = -1;
    bool found = false;
    QRegExp re("^-?\\d*\\.?\\d+");
    QRegExp re2("(([-+]?[0-9]+)?\\.)?\\b[0-9]+([eE][-+]?[0-9]+)?");



    for (int listRow = 0; listRow < _rawList->count(); listRow++)
    {

        if (!found)
        {
            numFinds = 0;
            current = _rawList->at(listRow);
            group = current.split(delimiter);

            for (int i = 0; i < group.count(); i++)
            {
                dataItem = group[i];

                dataItem = dataItem.trimmed();



                //isNumber = false;


                if (!dataItem.isEmpty())
                {

                    //qDebug() << "Non empty item -> " << dataItem;

                    if(re.exactMatch(dataItem) || re2.exactMatch(dataItem))
                    {


                        numFinds = numFinds + 1;

                        if (numFinds > 1)
                        {
                            result = listRow;
                            found = true;
                            break;

                        }
                    }
                    else {
                        break;
                    }


                }
                else {
                    //qDebug() << "item was empty" << dataItem;
                }
            }
        }
        else
        {
            break;
        }


    }

    return result;

}



bool HtcDataDialog::getFileData(QStringList * list)
{
    bool result = false;

    if (!_dataFileName.isEmpty())
    {
        QFile file(_dataFileName);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                QTextStream in(&file);
                while (!in.atEnd())
                {
                    *list += in.readLine();
                }
                    file.close();
            }
        if (list->count() > 0)
        {
            result = true;
        }
    }

    return result;
}

int HtcDataDialog::setListToView(QStringList *list)
{
    int numRows = 0;
    list->clear();
    int first = _firstDataRow - 1;
    int last = _rawList->count();

    for (int i = first; i < last; i++)
    {
        list->append(_rawList->at(i));
        numRows += 1;
    }

    return numRows;
}

int HtcDataDialog::setHeaderList(QStringList *list)
{
    int numRows = 0;
    list->clear();
    int first = 0;
    int last = _firstDataRow -1 ;

    for (int i = first; i < last; i++)
    {
        list->append(_rawList->at(i));

        numRows += 1;
    }

    return numRows;
}


QVector<int> HtcDataDialog::setSelectedColumnsList()
{
    QString del = ";";
    QString values = "";
    QString value;
    QVector <int> result;
    QRegExp re("^-?\\d*\\.?\\d+");
    QRegExp re2("(([-+]?[0-9]+)?\\.)?\\b[0-9]+([eE][-+]?[0-9]+)?");

    int col;
    int row;

    const auto selectedIndexes = ui->tvData->selectionModel()->selectedIndexes();
    for(auto&& singleIndex : selectedIndexes)
    {

        row = singleIndex.row();

        if (row < 1)
        {
            col = singleIndex.column();
            value = QString::number(col);

            if (!values.isEmpty())
            {
                if (!isValueInList(values, value, del))
                {

                    QModelIndex index = model->index(row, col, QModelIndex());
                    QString checkValue = ui->tvData->model()->data(index).toString();

                    // make sure the data is plotable
                    // ------------------------------
                    if(re.exactMatch(checkValue) || re2.exactMatch(checkValue))
                    {
                        values.append(value + del);
                    }
                }
            }
            else
            {

                QModelIndex index = model->index(row, col, QModelIndex());
                QString checkValue = ui->tvData->model()->data(index).toString();

                // make sure the data is plotable
                // ------------------------------
                if(re.exactMatch(checkValue) || re2.exactMatch(checkValue))
                {
                    values.append(value + del);
                }


            }

            result = StringToList(values, del);
        }


    }

    /*
     * make sure 0 is in the list
    */

    if (!result.contains(0))
    {
        result.append(0);
    }

    std::sort(result.begin(),result.end());


    return result;
}

QVector<int> HtcDataDialog::StringToList(QString value, QString del)
{
    QStringList temp;
    QVector<int> result;
    int newNum;

    int len = value.length();

    if (value.at(len-1) == del)
    {
        value.chop(1);
    }

    temp = value.split(del);

    for (int i = 0; i < temp.size(); i++)
    {
        newNum = QString(temp.at(i)).toInt();
        result.append(newNum);
    }

    return result;
}


void HtcDataDialog::listColumnList()
{


    for (int i = 0; i < _selectedColumnsList.size();i++)
    {
        qDebug() << "column-" << _selectedColumnsList.at(i);
    }
}

bool HtcDataDialog::isValueInList(QString list, QString value, QString del)
{
    QStringList temp;
    QString val1;
    QString val2;

    bool result = false;

    int len = list.length();

    if (list.at(len-1) == del)
    {
        list.chop(1);
    }

    temp = list.split(del);
    for (int i = 0; i < temp.size(); i++)
    {
        val1 = value;
        val2 = temp.at(i);

        if ( val1 == val2)
        {
            result = true;
            break;

        }
    }
    return result;
}

void HtcDataDialog::loadModel()
{
    int pos;

    model = new QStandardItemModel;

    // add load header here
    // --------------------
    QString headerRow = _listToView->at(0);
    QStringList headerRowList = headerRow.split(_delimiter, QString::SkipEmptyParts);

    for (int h = 0; h < headerRowList.size(); h++)
    {
        QString value = headerRowList.at(h);
        model->setHorizontalHeaderItem(h, new QStandardItem(value));
    }


    for (int i = 1; i < _listToView->count(); i++)
    {

        // read one line from textstream(separated by "\n")
        QString listLine = _listToView->at(i);

        // parse the read line into separate pieces(tokens) with "," as the delimiter
        QStringList lineToken = listLine.split(_delimiter, QString::SkipEmptyParts);

        // load parsed data to model accordingly
        for (int j = 0; j < lineToken.size(); j++)
        {
            QString value = lineToken.at(j);
            QStandardItem *item = new QStandardItem(value);
            pos = i - 1;
            model->setItem(pos, j, item);
        }


    }

    ui->tvData->setModel(model);

}


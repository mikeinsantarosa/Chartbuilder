// ++++++++++++++++++++++++++++++++++++++++++++ //
// File: htcchartdataset.cpp
// Description: a unit that can be passed to the chart
//
//
// Date: 2019-03-07
//
//
// TODO:
//
//
//
// ++++++++++++++++++++++++++++++++++++++++++++ //

#include "htcchartdataset.h"



HTCChartDataSet::HTCChartDataSet(QObject *parent)
{
    if (!_numFilesperRangeIsGood.isEmpty())
    {
        _numFilesperRangeIsGood.clear();
    }
    for (int i = 0; i < 3; i++)
    {
        _numFilesperRangeIsGood.append(QString::number(0));
    }

    // --------------------------------- //
    //
    // --------------------------------- //
    _isCommCheckData = false;
    _testValues.append(0);
    _testValues.append(1);
    _testValues.append(2);
}

void HTCChartDataSet::SetData(QStringList data)
{
    _data = data;


}

void HTCChartDataSet::SetChartTitle(QString title)
{
    _chartTitle = title;

//    qDebug() << "listing dataset called " << _chartTitle;
//    listThisList(_data);

}

void HTCChartDataSet::SetModel(QString model)
{
    _model = model;
}

void HTCChartDataSet::SetSerial(QString serial)
{
    _serial = serial;
}

void HTCChartDataSet::SetXAxisTitle(QString title)
{
    _xAxisTitle = title;
}

void HTCChartDataSet::SetYAxisTitle(QString title)
{
    _yAxisTitle = title;
}

void HTCChartDataSet::SetXAxisScale(QString scale)
{
    _xAxisScaling = scale;
}

void HTCChartDataSet::SetYAxisScale(QString scale)
{
    _yAxisTitle = scale;
}

void HTCChartDataSet::SetSampleFileName(QString file)
{
    _sampleFileName = file;
}

void HTCChartDataSet::SetDataSetIndex(int value)
{
    _datasetIDX = value;
}

int HTCChartDataSet::getDataType()
{
    return _dataType;
}

void HTCChartDataSet::setDataType(int dataType)
{
    _dataType = dataType;
}

void HTCChartDataSet::setBaseFolder(QString baseFolder)
{
    _baseFolder = baseFolder;
}


QStringList HTCChartDataSet::GetData()
{
    return _data;
}

QString HTCChartDataSet::GetChartTitle()
{
    return _chartTitle;
}

QString HTCChartDataSet::GetModel()
{
    return _model;
}

QString HTCChartDataSet::GetSerial()
{
    return _serial;
}

QString HTCChartDataSet::GetXAxisTitle()
{
    return _xAxisTitle;
}

QString HTCChartDataSet::GetYAxisTitle()
{
    return _yAxisTitle;
}

QString HTCChartDataSet::GetXAxisScale()
{
    return _xAxisScaling;
}

QString HTCChartDataSet::GetYAxisScale()
{
    return _yAxisScaling;
}

QString HTCChartDataSet::GetSampleFileName()
{
    return _sampleFileName;
}

int HTCChartDataSet::GetDataSetIndex()
{
    return _datasetIDX;
}

bool HTCChartDataSet::GetInitializedOK()
{
    return _initializedOK;
}

QStringList HTCChartDataSet::GetFilesperRangeAreGood()
{
    return _numFilesperRangeIsGood;
}

int HTCChartDataSet::GetFilesPerRangeIsGood(int rangeID)
{
    int result = _numFilesperRangeIsGood[rangeID].toInt();

    return result;
}

void HTCChartDataSet::SetFilesPerRangeIsGood(int RangeID, int value)
{
    _numFilesperRangeIsGood[RangeID] = QString::number(value);
}

QString HTCChartDataSet::GetBaseFolder()
{
    return _baseFolder;
}

bool HTCChartDataSet::GetIsCommCheckData()
{
    QString del = getFileDelim();
    QStringList dataList = getMasterList();
    _isCommCheckData = IsCommCheckData(dataList);

    return _isCommCheckData;
}




void HTCChartDataSet::setInitializedOKState()
{
    //Assume success
    _initializedOK = true;

    if (_data.isEmpty())
    {
        _initializedOK = false;
    }
    else if (_chartTitle.isEmpty())
    {
        _initializedOK = false;
    }
    else if (_model.isEmpty())
    {
        _initializedOK = false;
    }
    else if (_serial.isEmpty())
    {
        _initializedOK = false;
    }
    else if (_xAxisTitle.isEmpty())
    {
        _initializedOK = false;
    }
    else if (_yAxisTitle.isEmpty())
    {
        _initializedOK = false;
    }
    else if (_xAxisScaling.isEmpty())
    {
        _initializedOK = false;
    }
    else if (_yAxisScaling.isEmpty())
    {
        _initializedOK = false;
    }
}

void HTCChartDataSet::listThisList(QStringList list)
{
    int numItems = list.count();

    for (int i = 0; i < numItems; i++)
    {
        qDebug() << "item " << i << " : " << list[i];
    }
}


int HTCChartDataSet::getMin(QList<int> values)
{
    int yMin = std::numeric_limits<int>::max();

    foreach (int i, values)
    {
        yMin = qMin(yMin, i);
    }

    return yMin;
}

int HTCChartDataSet::getMax(QList<int> values)
{

    int yMax = std::numeric_limits<int>::min();
    foreach (int i, values)
    {
        yMax = qMax(yMax, i);
    }

    return yMax;
}

int HTCChartDataSet::getMean(QList<int> values)
{
    int numValues = values.count();
    int result = -1;
    if (numValues > 0)
    {
        result = values[numValues/2];
    }

    return result;
}

QStringList HTCChartDataSet::getMasterList()
{
    QStringList result;

   QString del = getFileDelim();



   // if(info.)
    // strip off the first line
    int numlines = _data.count();
    QStringList parts;
    QString target;

    if (numlines > 0)
    {
        for (int i = 0; i < numlines; i++)
        {
//

            parts =
//            result.append(_data[numlines +1]);
//            qDebug() << "adding line " << i;


        }


    }






    return result;

}

QString HTCChartDataSet::getShortenedParts(QString target, QString delim)
{
    QStringList result;
    QStringList temp = target.split(delim);
    int numLines = temp.count();
    for (int i = 1; i < numLines; i++)
    {
        result.append(temp[i]);
    }

    return result;



}

bool HTCChartDataSet::IsCommCheckData(QStringList commCkData)
{
    bool result = true;
    QString delim = ",";
    QString target;
    int numLines = commCkData.count();

    for (int i = 0; i < numLines; i++)
    {
        target = commCkData[i];

        if (ThisLineIsCommCk(target,delim) == false)
        {

            result = false;
            qDebug() << "that line is " << result;
            break;
        }
        else
        {
            qDebug() << "that line is " << result;
        }

    }

    return result;
}

QString HTCChartDataSet::getFileDelim()
{
    QFileInfo info = QFileInfo(_sampleFileName);
    QString extension = info.suffix();
    QString result;

    if (extension == "txt")
    {
        result = "\t";
    }
    else
    {
        result = ",";
    }

    return result;

}

bool HTCChartDataSet::ThisLineIsCommCk(QString target, QString del)
{
    bool result = false;
    QList<int> myValues = ConvertToIntList(target,del);
    int min = getMin(myValues);
    int max = getMax(myValues);
    int mean = getMean(myValues);

    if(_testValues.contains(min) && _testValues.contains(max) && _testValues.contains(mean))
    {
        result = true;
    }

    return result;


}

QList<int> HTCChartDataSet::ConvertToIntList(QString target, QString delim)
{
    QStringList list = target.split(delim);

    int numItems = list.count();
    QList<int> l;

    std::sort(list.begin(),list.end());

    for (int i = 0; i < numItems; i++)
    {
        l.append(list[i].toInt());
    }

    return l;
}

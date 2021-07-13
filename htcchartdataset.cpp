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
//  Added comm check auto detect Aug 2020
//
// ++++++++++++++++++++++++++++++++++++++++++++ //

#include "htcchartdataset.h"



HTCChartDataSet::HTCChartDataSet(QObject *parent)
{

    _data.clear();

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

    loadSettings();
}


// this is the data including header
// -------------------------------------------
void HTCChartDataSet::SetData(QStringList data)
{

    _data = data;

}

void HTCChartDataSet::SetChartTitle(QString title)
{
    // ------------------------------- //
    // this is the last dataset
    // item that gets set
    // ------------------------------- //
    SetCommCheckAutoDetect();

    // if autodetect is enabled
    // see if the data is binary
    // _isCommCheckData instead of _commCheckAutoDetect

    // qDebug() << "_commCheckAutoDetect " << _commCheckAutoDetect;
    if (_commCheckAutoDetect == 1)
    {
        SetCommCheckAutoDetect();
    }
    else
    {
        // since we can't autoDetect
        // we won't be setting
        // binary min/max values
        _isCommCheckData = false;
    }


    // if the data is binary
    // set the default binary
    // min/max values

    if (_isCommCheckData == true)
    {
        SetBinaryMinMax();
    }
    else
    {
        // set min/max for all date
        // if not commCheck data

        SetAnalogMinMax();
    }

    // always set the Title, header & freq list
    // ------------------------------- //
    //
    _chartTitle = title;
    setHeader();
    SetFreqValues();
    //
    // ------------------------------- //
    // new Re-ranger for
    // number < E-12
    _dataSetIsUnderRange = setDataIsUnderRange(_lowestAmplitudePlotable, _dataSetYMinValue);

    if (_dataSetIsUnderRange == true && _isCommCheckData == false)
    {

        _dataHasReReRangedSet = reRangeData();

        if ( _dataHasReReRangedSet == false)
        {
            // Might need to add an error dialog here
            // If Qt fixes the bug, we can kill all
            // this anyway.
            qDebug() << "unable to rerange data";
        }
    }
    else
    {
        // add else for debugging
        // qDebug() << "This data does not need to be reranged";
    }
    //
    // ------------------------------- //

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
    // qDebug() << "void HTCChartDataSet::setBaseFolder(QString baseFolder) " << _baseFolder;
}


QStringList HTCChartDataSet::GetData()
{

    return _data;

}

QStringList HTCChartDataSet::GetReRangedData()
{
    return _reRangedData;
}

bool HTCChartDataSet::GetDatahasReRangedSet()
{
    return _dataHasReReRangedSet;
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

QStringList HTCChartDataSet::GetHeaderList()
{
    return _headerList;
}

bool HTCChartDataSet::GetIsCommCheckData()
{
    return _isCommCheckData;
}

bool HTCChartDataSet::GetAnalogMaxMinValuesSet()
{
    return _analogMinMaxValuesSet;
}

double HTCChartDataSet::GetAnalogYMaxValue()
{
    double result;

    if(_isCommCheckData == false)
    {
        result = _dataSetYMaxValue;
    }
    else
    {
        result = _binaryYMax;
    }

    return result;

}

double HTCChartDataSet::GetAnalogYMinValue()
{

    double result;

    if(_isCommCheckData == false)
    {
        result = _dataSetYMinValue;
    }
    else
    {
        result = _binaryYMin;
    }

    return result;


}

double HTCChartDataSet::GetAnalogXMinValue()
{

    return _dataSetXMinValue;
}

double HTCChartDataSet::GetAnalogXMaxValue()
{

    return _dataSetXMaxValue;
}

bool HTCChartDataSet::GetDataIsUnderRange()
{
    return _dataSetIsUnderRange;
}

double HTCChartDataSet::GetRangeMultiplier()
{
    return _lowestAmplitudeMultiplier;
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

void HTCChartDataSet::listThisStringList(QStringList list)
{
    int numItems = list.count();

    for (int i = 0; i < numItems; i++)
    {
        qDebug() << "item " << i << " : " << list[i];
    }
}




void HTCChartDataSet::listThisList(QList<double> list)
{
    int numItems = list.count();

    for (int i = 0; i < numItems; i++)
    {
        qDebug() << "item " << i << " : " << list[i];
    }
}

void HTCChartDataSet::listMasterList()
{
    QStringList list = getMasterList();
    qDebug() << "void HTCChartDataSet::listMasterList() result";
    listThisStringList(list);
}

void HTCChartDataSet::SetCommCheckAutoDetect()
{
    QString del = getFileDelim();
    _ProcessedDataList = getMasterList();

    // qDebug() << "_ProcessedDataList = getMasterList(); " << _ProcessedDataList.count();

    // listMasterList();;
    _isCommCheckData = IsCommCheckData(_ProcessedDataList);
}

void HTCChartDataSet::loadSettings()
{
    QString positionValue = "";
    QString penValue = "";

    QSettings setting("Keysight","ChartBuilder");

    setting.beginGroup("ProgramFolders");

    // new comm check autodetect
    _commCheckAutoDetect = setting.value("CommCheckAutoDetectEnabled").toInt();

    setting.endGroup();
}

void HTCChartDataSet::SetAnalogMinMax()
{


    FillMinMaxLists();

    _analogMinMaxValuesSet = true;

}

void HTCChartDataSet::SetBinaryMinMax()
{
     _binaryYMin = _defaultBinaryYMin;
     _binaryYMax = _defaultBinaryYMax;

}

void HTCChartDataSet::FillMinMaxLists()
{
    double myMin, myMax;
    QString target;
    QString delim = getFileDelim();
    int numLines = _ProcessedDataList.count();
    QList<double> testValues;

    //qDebug() << "_ProcessedDataList.count() " << _ProcessedDataList.count();

    FlushMinMaxLists();

    for (int i = 0; i < numLines; i++)
    {
        testValues.clear();
        target = _ProcessedDataList[i];

        testValues = ConvertToDoubleList(target, delim);
        myMin = getMin(testValues);
        myMax   = getMax(testValues);

        _minValuesList << myMin;
        _maxValuesList << myMax;



    }


    _dataSetYMinValue = getMin(_minValuesList);
    _dataSetYMaxValue = getMax(_maxValuesList);

    qDebug() << "Min/max in FillMinMaxLists() " << _dataSetYMinValue << "/" << _dataSetYMaxValue;

}

void HTCChartDataSet::FlushMinMaxLists()
{
    if (_minValuesList.count() > 0)
    {
        _minValuesList.clear();
    }
    if (_maxValuesList.count() > 0)
    {
        _maxValuesList.clear();
    }
    if (_freqList.count() > 0)
    {
        _freqList.clear();
    }

}


void HTCChartDataSet::SetFreqValues()
{
    QString target = "";
    QString delim = getFileDelim();
    int numLines = _data.count();
    int pos = 0;
    double thisFreq = 0;

    for (int i = 1; i < numLines; i++)
    {
        target = _data[i];

        thisFreq = getSingleValueDelimString(target, delim, pos);

        _freqList.append(thisFreq);

    }

    _dataSetXMinValue = getMin(_freqList);
    _dataSetXMaxValue = getMax(_freqList);

}

double HTCChartDataSet::getSingleValueDelimString(QString target, QString delim, int position)
{
    double result;

    QString hitTarget;
    QStringList values;
    QString value = "";

    values = target.split(delim);
    value = values[position];
    result = value.toDouble();

    return result;
}

bool HTCChartDataSet::setDataIsUnderRange(double limit, double testValue)
{
    bool result = false;
    QString prefix = "1.0";
    double YMinValue;
    double d = 1;
    YMinValue = testValue;
    double factorValue = YMinValue/limit;

    QString scientificNumber = (QString().setNum(factorValue, 'e', 10));
    int len = scientificNumber.length();
    int pos = scientificNumber.indexOf('e',Qt::CaseInsensitive);
    QString exp = scientificNumber.mid(pos,len-1);

    // -------------------------------------- //
    // see if the exponent is positive
    // -------------------------------------- //
    int sign = scientificNumber.indexOf("+");

    // -------------------------------------- //
    // construct a number from the exponent
    // -------------------------------------- //
    QString final = prefix.append(exp);

    // -------------------------------------- //
    // convert the exponent into a
    // positive multiplier
    // -------------------------------------- //

    if(sign < 0)
    {
        result = true;
        d = 1/final.toDouble();

    }
    // -------------------------------------- //
    // assign the multiplier to a local variable
    // to be used as a get
    // -------------------------------------- //

    _lowestAmplitudeMultiplier = d;

    return result;
}

bool HTCChartDataSet::reRangeData()
{
    bool result = true;
    QString target;
    QString delim = getFileDelim();
    //QString newValue;

    if (!_reRangedData.isEmpty())
    {
        _reRangedData.clear();
    }
    if(_data.isEmpty())
    {
        // you sent me an empty list!
        // -------------------------- //
        result = false;

    }
    else
    {
        QString header = _data[0];
        int numLines = _data.count();

        for (int i = 1; i < numLines; i++)
        {
            _reRangedData.append(getReRangedLine(_data[i], delim, _lowestAmplitudeMultiplier));
        }
    }

    return result;
}

QString HTCChartDataSet::getReRangedLine(QString target, QString delim, double factor)
{
    QString result = "";
    double newValue, oldValue;
    QStringList values = target.split(delim);
    int numValues = values.count();



    for (int i = 0; i < numValues -1; i++)
    {
        if (i > 0)
        {
            oldValue = values[i].toDouble();
            newValue = oldValue * factor;
            result.append(QString::number(newValue));
            result.append(delim);
        }
        else
        {
            result.append(values[i]);
            result.append(delim);
        }

    }

    // do last
    // ----------- //
    oldValue = values[numValues -1].toDouble();
    newValue = oldValue * factor;
    result.append(QString::number(newValue));

    return result;
}

void HTCChartDataSet::setHeader()
{
    //_header

    _header = _data[0];
    QString delim = getFileDelim();
    setHeaderList(_header, delim);

}

void HTCChartDataSet::setHeaderList(QString target, QString delim)
{
   _headerList = target.split(delim);
}


double HTCChartDataSet::getMin(QList<double> values)
{

    std::sort(values.begin(), values.end());
    double yMin = *std::min_element(values.begin(), values.end());

    return yMin;
}

double HTCChartDataSet::getMax(QList<double> values)
{

    std::sort(values.begin(), values.end());
    double yMax = *std::max_element(values.begin(), values.end());

    return yMax;
}

double HTCChartDataSet::getMean(QList<double> values)
{
    int numValues = values.count();
    double result = -1;
    std::sort(values.begin(), values.end());
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
    // qDebug() << " delim is " << del;

    int numlines = _data.count();

    // qDebug() << "data count is " << numlines;

    QStringList parts;
    QString target;
    QString newTarget;

    if (numlines > 0)
    {
        // strip off the first line
        // but do the rest
        for (int i = 1; i < numlines; i++)
        {
            target = getShortenedParts(_data[i], del);
            // qDebug() << "adding line " << target;
            result.append(target);
        }

    }

    return result;

}

QString HTCChartDataSet::getShortenedParts(QString target, QString delim)
{
    QString result;
    QStringList temp = target.split(delim);
    int numLines = temp.count();
    for (int i = 1; i < numLines; i++)
    {
        result.append(temp[i]);

        if (i < numLines -1)
        {
            result.append(delim);
        }

    }

    return result;

}

bool HTCChartDataSet::IsCommCheckData(QStringList commCkData)
{
    bool result = true;
    QString delim = getFileDelim();
    QString target;
    int numLines = commCkData.count();

    for (int i = 0; i < numLines; i++)
    {
        target = commCkData[i];

        if (ThisLineIsCommCk(target,delim) == false)
        {
            result = false;
            break;
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
    // check this method - not working correctly
    QList<double> myValues = ConvertToDoubleList(target,del);


    double min = getMin(myValues);
    double max = getMax(myValues);
    double mean = getMean(myValues);

    if(_testValues.contains(min) && _testValues.contains(max) && _testValues.contains(mean))
    {
        result = true;
    }

    return result;
}

QList<double> HTCChartDataSet::ConvertToDoubleList(QString target, QString delim)
{
    QStringList list = target.split(delim);

    int numItems = list.count();
    QList<double> l;

    std::sort(list.begin(),list.end());

    for (int i = 0; i < numItems; i++)
    {
        l.append(list[i].toDouble());
    }

    return l;
}

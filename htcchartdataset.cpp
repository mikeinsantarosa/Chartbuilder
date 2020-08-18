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
}

void HTCChartDataSet::SetData(QStringList data)
{
    _data = data;

}

void HTCChartDataSet::SetChartTitle(QString title)
{
    _chartTitle = title;
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

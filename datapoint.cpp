/* File     : datapoint.cpp
 * Author   : Mike Purtell
 * Date     : Nove 18, 2017
 *
 * Purpose  :
 *
 * Maintains a frequency
 * vs level data point.
 *
 *
 * */

#include "datapoint.h"



DataPoint::DataPoint()
{
   _freq = 0;
   _yValues.clear();
   _DoublePositions.clear();
}

DataPoint::DataPoint(double freq)
{
    _freq = freq;
    _yValues.clear();
    _DoublePositions.clear();
}

void DataPoint::setFreq(double value)
{
    _freq = value;
}

void DataPoint::addValue(QString value)
{
    int isDouble = 0;
    if (QString(value).toDouble())
    {
        isDouble = 1;
    }
    _yValues.append(value);
    _DoublePositions.append(isDouble);

}


QString DataPoint::getValueByPosition(int Position)
{
    QString result = "VALUE_SET_AT_POSITION";
    if (Position < _yValues.count())
    {
        result = _yValues.at(Position);
    }

    return result;
}

int DataPoint::getMemberCount()
{
    return _yValues.count();
}

QVector<QString> DataPoint::getYValues()
{
    return _yValues;
}

QVector<int> DataPoint::getDoublePositions()
{
    return _DoublePositions;
}

double DataPoint::getFrequency()
{
    return _freq;
}

/* ******************************************************************
 * Class: HTCChartDataFile.cpp
 * Purpose: Represent the data from a .csv or .txt file
 * Rev Date: Feb-14-2018
 *
 * TODO:
 * 1st parse the file name to hopefully help with
 * sorting
 * 2st - Load the data into memory for each file.
 *
 *
 *
 *
 *
 *
 *
 *
 ********************************************************************/

#include "htcchartdatafile.h"


HTCChartDataFile::HTCChartDataFile(QString dataFileName, int dType)
{

    _riMatchingTestCodes << "q241" << "q242" << "q243" << "q244" << "q245" << "q248" << "q249";
    _ciMatchingTestCodes  << "q420" << "q421" << "q422" << "q423" << "q424" << "q425";
    _orderCounter = 1;

    setDataFileDelim(dataFileName);
    _fileDelimCount = getFileDelimCount(dataFileName);
    setDataType(dType);

    initProperties();
    _dataSuccessfullyLoaded = false;
    _basedOnThisFile = dataFileName;
    _isStandardTestType = 0;


    init();
    setKey();
}

HTCChartDataFile::HTCChartDataFile()
{

}

QVector<DataPoint> HTCChartDataFile::getAllPoints()
{
    return _allPoints;
}

QStringList HTCChartDataFile::getColumnHeaderList()
{
    return _ColumnHeaderlist;

}

QString HTCChartDataFile::GetTableDataByRow(int rowNumber)
{

    QString result = _tableData.at(rowNumber);

    return result;

}

bool HTCChartDataFile::getDataSuccessfullyLoaded()
{
    return _dataSuccessfullyLoaded;

}

int HTCChartDataFile::getFirstDataRowNumber()
{
    return _firstDataRow;
}

int HTCChartDataFile::getlastDataRowNumber()
{
    return _lastDataRow;
}

double HTCChartDataFile::getFirstFreq()
{
    return _firstFreq;
}

double HTCChartDataFile::getLastFreq()
{
    return _lastFreq;
}

int HTCChartDataFile::getColumnHeadersRowNumber()
{
    return _firstDataRow -1;
}


int HTCChartDataFile::getTotalNumberOfRows()
{
    return _rawDataList.count();
}

void HTCChartDataFile::init()
{
    // make sure we stat with a clean set
    // ----------------------------------------
    _allPoints.clear();

    // assume failure
    _dataSuccessfullyLoaded = false;

    _totalNumberOfRows = loadFileIntoList();

    if (_totalNumberOfRows > 0)
    {
       _firstDataRow = findFirstDataRow(_rawDataList, _dataFileDelim);

       // if there is no numerical data, the firstDataRow
       // will be -1. Let's check that here and do something about it
       // before continuing.

       if(_firstDataRow > -1)
       {
           _lastDataRow = setLastDataRow();

           _numberOfDataColumns = setColumnHeadersList(_dataFileDelim);
           _fileInfo = QFileInfo(_basedOnThisFile);

           parseFileProperties(_fileInfo.fileName());


           setFirstFreq();
           setLastFreq();

           loadDataIntoMemory();


       }
       if (_numberOfDataColumns >= 3)
       {
           _dataSuccessfullyLoaded = true;
       }
       else
       {
           _dataSuccessfullyLoaded = false;
       }



    }


}

int HTCChartDataFile::findFirstDataRow(QStringList list, QString delimiter)
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



    for (int listRow = 0; listRow < list.count(); listRow++)
    {

        if (!found)
        {
            numFinds = 0;
            current = list[listRow];

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

int HTCChartDataFile::setLastDataRow()
{
    int result = -1;
    if (_rawDataList.count() > 0)
    {
        result = _rawDataList.count() -1;
    }
    return result;
}

int HTCChartDataFile::loadFileIntoList()
{
    _rawDataList.clear();
    //int i = 0;
    QFile file(_basedOnThisFile);
    QString msg;
    int result = -1;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream in(&file);
            while (!in.atEnd())
            {

                _rawDataList += in.readLine();

            }
                file.close();
        }

    if(_rawDataList.count() > 0)
    {
        result = _rawDataList.count();
    }

    return result;

}

void HTCChartDataFile::setRangeOrderMult()
{

    if (_fRange.contains("80M-1G"))
    {
        _rangeOrderMult = 0;
    }
    else if (_fRange.contains("1G-2G"))
    {
        _rangeOrderMult = 10;
    }
    else if (_fRange.contains("1G-6G"))
    {
        _rangeOrderMult = 20;
    }
    else if (_fRange.contains("2G-27G"))
    {
        _rangeOrderMult = 20;
    }

}

int HTCChartDataFile::setColumnHeadersList(QString delim)
{
    QStringList Headers;
    if (_rawDataList.count() > 0)
    {
        QString strHeader = _rawDataList.at(_firstDataRow - 1);
        Headers = strHeader.split(delim);
    }
    _ColumnHeaderlist = Headers;

    return Headers.count();
}

void HTCChartDataFile::parseFileProperties(QString fileName)
{

    QStringList parts = getFileParts(fileName,_fileNamePartsDelim);

    QString thisTestCode = parts.at(0);

    if (_riMatchingTestCodes.contains(thisTestCode))
    {
        solvePropertiesForRadiated(parts);
    }
    else if (_ciMatchingTestCodes.contains(thisTestCode))
    {
        solvePropertiesForConducted(parts);
    }
    else
    {
        qDebug() << "I got no idea what data type this is";
        showBadFileDataMessage(fileName);
        _dataSuccessfullyLoaded = false;
    }


}

void HTCChartDataFile::setDataFileDelim(QString fileName)
{
    QFileInfo info = QFileInfo(fileName);
    QString extension = info.suffix();

    if (extension == "txt")
    {
        _dataFileDelim = "\t";
    }
    else
    {
        _dataFileDelim = ",";
    }


}

int HTCChartDataFile::solveRangeIDX(QString rangeString)
{
    int result = -1;

    // return values for all known standard ranges
    // this does not solve for the custom ranges like:
    // 800M-880M ...
    // ----------------------------------------------



    if (rangeString.contains("80M-1G"))
    {
        result = 0;
    }
    else if (rangeString.contains("1G-2G"))
    {
        result = 1;
    }
    else if (rangeString.contains("1G-6G"))
    {
        result = 1;
    }
    else if (rangeString.contains("2G-27G"))
    {
        result = 2;
    }


    return result;
}

int HTCChartDataFile::solveOrientationIDX(QString polarity, QString rotation)
{
    int result = -1;

    if (polarity == "BL" && rotation == "0")
    {
        result = 0;
    }
    else if (rotation == "0" && polarity == "H")
    {
        result = 1;
    }
    else if (rotation == "0" && polarity == "V")
    {
        result = 2;
    }
    else if (rotation == "90" && polarity == "H")
    {
        result = 3;
    }
    else if (rotation == "90" && polarity == "V")
    {
        result = 4;
    }
    else if (rotation == "180" && polarity == "H")
    {
        result = 5;
    }
    else if (rotation == "180" && polarity == "V")
    {
        result = 6;
    }
    else if (rotation == "270" && polarity == "H")
    {
        result = 7;
    }
    else if (rotation == "270" && polarity == "V")
    {
        result = 8;
    }
    else
    {
        //default 0 added for custom file names
        result = 1;
    }

    return result;
}

QString HTCChartDataFile::GetPartFromPair(QString target, QString delim, int whichOne)
{
    QStringList pair;
    int needed = 2;
    QString answer = "";

    if (!pair.isEmpty())
    {
        pair.clear();
    }

    pair = target.split(delim);

    int numItems = pair.count();

    if (numItems == needed)
    {
        answer = pair.at(whichOne);
    }

    return answer;

}

QStringList HTCChartDataFile::getFileParts(QString target, QString delim)
{
    QStringList answer;
    QString lastPart = "";

    int last;
    answer = target.split(delim);

    last = answer.count() -1;
    if (answer.at(last).contains("."))
    {
        lastPart = GetPartFromPair(answer.at(last),".",0);
        answer[last] = lastPart;

    }

    return answer;

}

int HTCChartDataFile::setSortOrderIndex()
{
    int result =  -1;

    if (_rangeIDX == 0)
    {
        if (_orientationOrderIDX == 0)
        {
            result = 0 + _rangeOrderMult;
        }
        else if (_orientationOrderIDX == 1)
        {
            result = 1 + _rangeOrderMult;
        }
        else if (_orientationOrderIDX == 2)
        {
            result = 2 + _rangeOrderMult;
        }
        else if (_orientationOrderIDX == 3)
        {
            result = 3 + _rangeOrderMult;
        }
        else if (_orientationOrderIDX == 4)
        {
            result = 4 + _rangeOrderMult;
        }
        else if (_orientationOrderIDX == 5)
        {
            result = 5 + _rangeOrderMult;
        }else if (_orientationOrderIDX == 6)
        {
            result = 6 + _rangeOrderMult;
        }else if (_orientationOrderIDX == 7)
        {
            result = 7 + _rangeOrderMult;
        }else if (_orientationOrderIDX == 8)
        {
            result = 8 + _rangeOrderMult;
        }
    }
    else if (_rangeIDX == 1)
    {
        if (_orientationOrderIDX == 0)
        {
            result = 9;
        }
        else if (_orientationOrderIDX == 1)
        {
            result = 10;
        }
        else if (_orientationOrderIDX == 2)
        {
            result = 11;
        }
        else if (_orientationOrderIDX == 3)
        {
            result = 12;
        }
        else if (_orientationOrderIDX == 4)
        {
            result = 13;
        }
        else if (_orientationOrderIDX == 5)
        {
            result = 14;
        }else if (_orientationOrderIDX == 6)
        {
            result = 15;
        }else if (_orientationOrderIDX == 7)
        {
            result = 16;
        }else if (_orientationOrderIDX == 8)
        {
            result = 17;
        }
    }

        else if (_rangeIDX == 2)
        {
            if (_orientationOrderIDX == 0)
            {
                result = 18;
            }
            else if (_orientationOrderIDX == 1)
            {
                result = 19;
            }
            else if (_orientationOrderIDX == 2)
            {
                result = 20;
            }
            else if (_orientationOrderIDX == 3)
            {
                result = 21;
            }
            else if (_orientationOrderIDX == 4)
            {
                result = 22;
            }
            else if (_orientationOrderIDX == 5)
            {
                result = 23;
            }else if (_orientationOrderIDX == 6)
            {
                result = 24;
            }else if (_orientationOrderIDX == 7)
            {
                result = 25;
            }else if (_orientationOrderIDX == 8)
            {
                result = 26;
            }
    }
    //qDebug() << "orientaionIndex - " << _orientationOrderIDX;

    return result;

}

void HTCChartDataFile::initProperties()
{
    _standardTestRanges.append("80M-1G");
    _standardTestRanges.append("1G-2G");
    _standardTestRanges.append("2G-27G");
    _standardTestRanges.append("1G-6G");
}




void HTCChartDataFile::loadDataIntoMemory()
{
    QString ref;
    QStringList target;
    double freq;
    QString YValue;

    //QString newValue = "";
    int thisLastRow = _lastDataRow + 1;

    // clear the lists
    if(!_tableData.isEmpty())
    {
        _tableData.clear();
    }

    if(!_allPoints.isEmpty())
    {
        _allPoints.clear();
    }



    for (int i = _firstDataRow; i < thisLastRow; i++)
    {
        DataPoint * point = new DataPoint;
        ref = _rawDataList.at(i);

        // fill the table data list
        _tableData.append(ref);

        target = ref.split(_dataFileDelim);
        freq = QString(target.at(0)).toDouble();

        point->setFreq(freq);

        for (int i = 1; i < target.count(); i++)
        {
            YValue = target.at(i);
            point->addValue(YValue);
        }

        _allPoints.append(*point);

    }


}



void HTCChartDataFile::setFirstFreq()
{
    _firstFreq = -100000;
    if (_rawDataList.count() > 0)
   {
       QString rowData = _rawDataList.at(_firstDataRow);
       QStringList values = rowData.split(_dataFileDelim);
       _firstFreq = QString(values.at(0)).toDouble();
   }
}

void HTCChartDataFile::setLastFreq()
{
    _lastFreq = -1;
     if (_rawDataList.count() > 0)
    {
        QString rowData = _rawDataList.at(_lastDataRow);
        QStringList values = rowData.split(_dataFileDelim);
       _lastFreq = QString(values.at(0)).toDouble();
     }
}

void HTCChartDataFile::setStandardTestType(QString rangeString)
{
    _isStandardTestType = 0;


    if(_standardTestRanges.indexOf(rangeString) > -1)
    {
        _isStandardTestType = 1;
    }

}

void HTCChartDataFile::setKey()
{
    if(_firstDataRow > -1)
    {
        _SetKey.clear();
        _SetKey.append(_eutModel);
        _SetKey.append(_fileNamePartsDelim);
        _SetKey.append(_eutSerial);
        _SetKey.append(_fileNamePartsDelim);
        _SetKey.append(QString::number(_isStandardTestType));
    }
    else
    {
        _SetKey.clear();
        _SetKey.append(BAD_FILE_DATA);
    }

}

int HTCChartDataFile::getFileDelimCount(QString fileName)
{
    //_fileNamePartsDelim
    QStringList list = fileName.split(_fileNamePartsDelim);
    int result = list.count();

    return result;


}

void HTCChartDataFile::setDataType(int dataType)
{
    _dataType = dataType;
}

void HTCChartDataFile::showBadFileDataMessage(QString fileName)
{
    QMessageBox msgBox;
    QString badFileName = fileName;

    QString message = "";

        //find the bad range
        // ------------------------------------------


        message.append("Found a file with bad file name or incomplete data : ");
        message.append("Check for testCode_Model_Serial_param parts then correct it");
        message.append(" -- in Filename: ");
        message.append(badFileName);

        msgBox.setText(message);
        msgBox.setInformativeText("Please correct the problem and try again");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);


    int ret = msgBox.exec();

}

void HTCChartDataFile::solvePropertiesForRadiated(QStringList parts)
{

   //listThisList(parts);

    _testCode = parts.at(0);
    _eutModel = parts.at(1);
    _eutSerial = parts.at(2);
    _tLevel = parts.at(3);
    _fRange = parts.at(4);
    _ttRotation = parts.at(5);
    _polarity = parts.at(6);

    setRangeOrderMult();

    _rangeIDX = solveRangeIDX(_fRange);
    _orientationOrderIDX = solveOrientationIDX(_polarity, _ttRotation);
    _sortOrderIDX = setSortOrderIndex();

    // this looks backwards!
    SortOrderIndex = _sortOrderIDX;

    setStandardTestType(_fRange);

//    qDebug() << "RI parts";
//    qDebug() << _sortOrderIDX  <<  "/" << _testCode << "/" << _eutModel  << "/" <<  _eutSerial  << "/" <<  _tLevel  << "/" << _fRange  << "/" << _ttRotation  << "/" << _polarity;

}

void HTCChartDataFile::solvePropertiesForConducted(QStringList parts)
{
    int numParts = parts.count();

//    listThisList(parts);

    if (numParts > 5)
    {
        //qDebug() << "2-many-parts for " << assembleList(parts,"_");
    }
    else if (numParts == 5)
    {

        _testCode = parts.at(0);
        _tLevel = parts.at(1);
        _eutModel = parts.at(2);
        _eutSerial = parts.at(3);
        _polarity = parts.at(4);
        _ttRotation = "";
        _sortOrderIDX = getCISortingValue(_polarity);
        // SortOrderIndex = _sortOrderIDX;


        // qDebug() << "5 part CI Data";

    }
    else if (numParts == 4)
    {
        _testCode = parts.at(0);
        _eutModel = parts.at(1);
        _eutSerial = parts.at(2);
        _polarity = parts.at(3);
        _ttRotation = "";
        _sortOrderIDX = getCISortingValue(_polarity);
        // SortOrderIndex = _sortOrderIDX;


        qDebug() << "4 part CI Data";
    }
    else
    {

    }

       // qDebug() << "CI parts" << SortOrderIndex  <<  "/" << _testCode << "/" << _eutModel  << "/" <<  _eutSerial  << "/" <<  _tLevel  << "/" << _fRange  << "/" << _ttRotation  << "/" << _polarity;

}

QString HTCChartDataFile::assembleList(QStringList parts, QString delim)
{
    int count = parts.count();
    QString result = "";
    for (int i = 0; i < count - 1; i++)
    {
        result.append(parts.at(i));
        result.append(delim);

    }
    result.append(parts.at(count - 1));

    return result;

}

int HTCChartDataFile::getCISortingValue(QString polarity)
{
    int result;
    // qDebug() << "testing polarity " << polarity;
    QString target = polarity.toUpper();
    QString matchBase = "BASE";
    QString matchCDN = "CDN";
    QString matchClamp = "CLAMP";

    if (target.contains(matchBase))
    {
        result = 0;
    }
    else if (target.contains(matchCDN))
    {
        result = 1;
    }
    else if (target.contains(matchClamp))
    {
        result = 2;
    }
    else
    {
        result = 3;
    }

    return result;
}

void HTCChartDataFile::listThisList(QStringList list)
{
    for (int i = 0; i < list.count(); i++)
        qDebug() << "list #" << i << " is " << list.at(i);
}

QFileInfo HTCChartDataFile::getDataFileInfo()
{
    return _fileInfo;
}
;
QString HTCChartDataFile::getOrientationPolarity()
{
    return _polarity;
}

QString HTCChartDataFile::getOrientationTTRotation()
{
    return _ttRotation;
}

QString HTCChartDataFile::getOrientationFRange()
{
    return _fRange;
}

QString HTCChartDataFile::getOrientationTLevel()
{
    return _tLevel;
}

QString HTCChartDataFile::getOrientationEUTSerial()
{
    return _eutSerial;
}

QString HTCChartDataFile::getOrientationEUTModel()
{
    return _eutModel;
}

QString HTCChartDataFile::getOrientationTestCode()
{
    return _testCode;
}

int HTCChartDataFile::getOrientationRangeIndex()
{
    return _rangeIDX;
}

int HTCChartDataFile::getOrientationOrderIndex()
{
    return _orientationOrderIDX;
}

int HTCChartDataFile::GetSortOrderIndex()
{
    return _sortOrderIDX;
}

QString HTCChartDataFile::getKey()
{
    return _SetKey;
}

QString HTCChartDataFile::getFileDelim()
{
    return _dataFileDelim;
}

void HTCChartDataFile::setSortOrder(int order)
{
    _sortOrderIDX = order;
    SortOrderIndex = _sortOrderIDX;
}


int HTCChartDataFile::getDataType()
{
    return _dataType;
}

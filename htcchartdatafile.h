#ifndef HTCCHARTDATAFILE_H
#define HTCCHARTDATAFILE_H

//#include <QObject>
#include <QFileInfo>
#include <QDebug>
#include <QRegExp>
#include <QMessageBox>


#include "datapoint.h"


class HTCChartDataFile
{

public:
    explicit HTCChartDataFile(QString dataFileName, int dType );
    HTCChartDataFile();

    QVector <DataPoint> getAllPoints();

    QStringList getColumnHeaderList();
    QString GetTableDataByRow(int rowNumber);
    bool getDataSuccessfullyLoaded();
    int getFirstDataRowNumber();
    int getlastDataRowNumber();
    double getFirstFreq();
    double getLastFreq();
    int getColumnHeadersRowNumber();
    int getTotalNumberOfRows();
    QFileInfo getDataFileInfo();
    QString getOrientationPolarity();
    QString getOrientationTTRotation();
    QString getOrientationFRange();
    QString getOrientationTLevel();
    QString getOrientationEUTSerial();
    QString getOrientationEUTModel();
    QString getOrientationTestCode();
    int getOrientationRangeIndex();
    int getOrientationOrderIndex();
    int GetSortOrderIndex();
    QString getKey();
    QString getFileDelim();

    int SortOrderIndex;

    // new CI code
    int getDataType();




signals:

public slots:

private:




    QFileInfo _fileInfo;
    QString _basedOnThisFile;
    QString _dataFileDelim;

    // ---------------------------------
    QStringList _riMatchingTestCodes;
    QStringList _ciMatchingTestCodes;

    bool _dataSuccessfullyLoaded;
    int _totalNumberOfRows;
    int _firstDataRow;
    int _lastDataRow;
    int _headerDataRow;
    int _numberOfDataColumns;
    QStringList _rawDataList;
    QStringList _ColumnHeaderlist;
    int _isStandardTestType;
    QString _fileNamePartsDelim = "_";
    QStringList _standardTestRanges;//{"80M-1G","1G-2G","2G-27G","1G-6G"};
    QString _SetKey = "";

    // file sort properties
    QString _polarity;
    QString _ttRotation;
    QString _fRange;
    QString _tLevel;
    QString _eutSerial;
    QString _eutModel;
    QString _testCode;

    int _rangeIDX;
    int _orientationOrderIDX;
    int _sortOrderIDX;
    int _rangeOrderMult;

    // data values
    double _firstFreq;
    double _lastFreq;


    void init();

    int findFirstDataRow(QStringList list, QString delimiter);
    int setLastDataRow();
    int loadFileIntoList();
    void setRangeOrderMult();
    int setColumnHeadersList(QString delim);
    void parseFileProperties(QString fileName);
    void setDataFileDelim(QString fileName);

    int solveRangeIDX(QString rangeString);
    int solveOrientationIDX(QString polarity, QString rotation);

    // ----------------------------------------------------------------------- //
    // new datafile parts
    //
    //QString const _NoDataToParse_ = "missing";
    QString GetPartFromPair(QString target, QString delim, int whichOne);

    QStringList getFileParts(QString target, QString delim);
    //
    //
    // ----------------------------------------------------------------------- //
    int setSortOrderIndex();

    void initProperties();

    // file data
    QVector <DataPoint> _allPoints;
    QStringList _tableData;
    void loadDataIntoMemory();

//    void setFilenameProperties(QString fName);
    void setFirstFreq();
    void setLastFreq();
    void setStandardTestType(QString rangeString);
    void setKey();

    int getFileDelimCount(QString fileName);

    int _fileDelimCount;

    // new CI code
    void setDataType(int dataType);
    int RIdataType = 0;
    int CIdataType = 1;

    int _dataType = -1; // not initialized yet

    QString BAD_FILE_DATA = "UNKNOWN-NO-DATA";

    void showBadFileDataMessage(QString fileName);

    // debug functions
    void listThisList(QStringList list);

};

#endif // HTCCHARTDATAFILE_H

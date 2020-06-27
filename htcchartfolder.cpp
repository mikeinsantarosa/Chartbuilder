// ++++++++++++++++++++++++++++++++++++++++++++ //
// File: htcchartfolder.cpp
// Description: representation of a data folder
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

#include "htcchartfolder.h"
#include <QDebug>



HTCChartFolder::HTCChartFolder(QObject *parent) : QObject(parent)
{
   // _MatchingTestCodes << "q241" << "q242" << "q243" << "q244" << "q245" << "q248" << "q249";
    _riMatchingTestCodes << "q241" << "q242" << "q243" << "q244" << "q245" << "q248" << "q249";
    _ciMatchingTestCodes  << "q420" << "q421" << "q422" << "q423" << "q424" << "q425";
}

int HTCChartFolder::init(QString folder, QString extension, int dType)
{

    // ++++++++++++++++++++++++++++++++++++++++++++++++
    // check for file count first
    // we might need to protect against too many/few files
    // ++++++++++++++++++++++++++++++++++++++++++++++++
    int fileCount = CountFiles(folder);
    qDebug() << "found " << fileCount << " files.";

    setDataType(dType);


    if(!folder.isEmpty())
    {

        HTCChartDataFile filObj;

        QString itemName = "";
        int result = 0;
        QString model = "";
        QString serial = "";
        QString temp = "";
        QString fNameDelim = "_";
        QString thisSet = "";
        QString thisTag = "";
        QString nameToTest = "";
        QString rowCount = "";
        int numFRows = -1;

        QString msg;


        bool found = false;

        QDir recordDir(folder);
        recordDir.setSorting(QDir::Name);

        if(! _folderList.isEmpty())
        {
            _folderList.clear();
        }

        if(!_TaggedList.isEmpty())
        {
            _TaggedList.clear();
        }

        if(!_sets.isEmpty())
        {
            _sets.clear();
        }
        if(!_fileCountList.isEmpty())
        {
            _fileCountList.clear();
        }

        QDirIterator it(recordDir, QDirIterator::Subdirectories);

        while (it.hasNext())
        {
            if (!found)
            {
                found = true;

            }
//            qDebug() << "dataType = " << dType;
            itemName = it.next();
//            qDebug() << "Loading this file -> " << itemName;

            QFileInfo info = QFileInfo(itemName);

            nameToTest = info.fileName();
            QStringList fileParts = nameToTest.split("_");

            QString code = fileParts.at(0);

            msg.clear();
            msg.append("Addinging file: ");
            msg.append(info.fileName());
            msg.append(" to the list...");

            emit messageToStatusBar(msg);

            if (nameToTest.contains(extension))
            {

                // check for proper test codes contained in the
                // ones pointed to.
                //
                // return value of 1 == proper test codes exist.
                // dType == 1 == RI Data
                // ---------------------------------------------
                if (doesTypeMatch(code, dType) == 1)
                {


                    thisSet.clear();
                    HTCChartDataFile *  filObj = new HTCChartDataFile(itemName, _dataType);

                    // need to guard against there being a file that
                    // passes inspection but doesn't have any numerical
                    // data in it.

                    if(filObj->getDataSuccessfullyLoaded())
                    {
                        model = filObj->getOrientationEUTModel();
                        serial = filObj->getOrientationEUTSerial();
                        numFRows = filObj->getlastDataRowNumber() - filObj->getFirstDataRowNumber();

                        thisSet.append(model);
                        thisSet.append(fNameDelim);
                        thisSet.append(serial);

                        thisTag.append(filObj->getKey());
                        thisTag.append(",");
                        thisTag.append(itemName);

                        // protect the list from non-data datafiles
                        // ---------------------------------------- //
                        // ---------------------------------------- //
                        // ---------------------------------------- //
                        if (thisTag != BAD_FILE_DATA)
                        {
                            //add it to the set if it doesn't exist
                            if(!_sets.contains(thisSet))
                            {

                                _sets.append(thisSet);
                            }

                            _folderList.append(itemName);
                            _TaggedList.append(thisTag);
                            _fileCountList.append(QString::number(result));


                           // qDebug() << "Added file number " << result + 1;

                            result += 1;


                            thisTag.clear();
                            thisSet.clear();
                        }
                        else
                        {
                            showBadFileDataMessage(itemName);
                            qDebug() << "skipped adding file " << itemName;
                        }



                    }
                    else
                    {
                        showBadFileDataMessage(itemName);
                        qDebug() << "skipped adding file " << itemName;
                    }


                    delete filObj;
                }

            }

        }

        return result;
    }
}

QStringList HTCChartFolder::GetFolderList()
{
    return _folderList;
}

QStringList HTCChartFolder::GetDataSetNames()
{
    return _sets;
}

QStringList HTCChartFolder::GetTaggedList()
{
    return _TaggedList;
}

int HTCChartFolder::getDataType()
{
    return _dataType;
}

void HTCChartFolder::setDataType(int dataType)
{
    _dataType = dataType;
}

int HTCChartFolder::doesTypeMatch(QString testCode, int dType)
{
    int result = 0;

    //qDebug() << "Checking test code " << testCode << " to be a " << dType << " data type";

    if (dType == RIdataType)
    {
        if(_riMatchingTestCodes.contains(testCode))
        {
            result = 1;
        }
    }
    else
    {
        if(_ciMatchingTestCodes.contains(testCode))
        {
            result = 1;
        }
    }

    return result;
}


int HTCChartFolder::CountFiles(QString path)
{
    int suma = 0;
    QDir dir(path);
    dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    if(!dir.exists()) {
    return 1;
    }
    QFileInfoList sList = dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);


    for(QFileInfo ruta: qAsConst(sList))
    {
        if(ruta.isDir())
        {
            suma += CountFiles(ruta.path() + "/" + ruta.completeBaseName()+"/");
        }
        suma++;
    }

    return suma;

}

void HTCChartFolder::listThisList(QStringList target, QString delim)
{
    int numElements = target.count();

    for (int i = 0; i < numElements; i++)
    {
        qDebug() << "item " << i << " is " << target.at(i);
    }

    qDebug() << "done";
}

void HTCChartFolder::showBadFileDataMessage(QString fileName)
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

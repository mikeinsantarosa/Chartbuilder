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

    // set data type
    setDataType(dType);

   // qDebug() << "dataType in htcChartFolder is " << dType;

    // new files list

    QStringList fileList;
    fileList.clear();

    // ++++++++++++++++++++++++++++++++++++++++++++++++
    // CI parts counts: 5 == current, 4 == older
    // 5 pattern: testCode_testLevel_model_serial_what
    // 4 pattern: testCode__model_serial_what
    //
    // RI parts counts: 7 == current,
    // 7 pattern: testCode_model_serial_level_range_rotation_polarity
    //
    // ++++++++++++++++++++++++++++++++++++++++++++++++

    // ++++++++++++++++++++++++++++++++++++ //
    // initial file filter
    // dump unusable files.
    // ++++++++++++++++++++++++++++++++++++ //
    fileList = CountFiles(folder);

    // qDebug() << "num files in list " << fileList.count();
    int result = -1;

    // dTypes 0/RI, 1/CI





    if(!folder.isEmpty())
    {

        HTCChartDataFile filObj;

        QString itemName = "";
        result = 0;
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

        fileList.sort();

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

        //QDirIterator it(recordDir, QDirIterator::Subdirectories);
        for (int i = 0; i < fileList.count(); i++)
        {
        // --------------------------------
        // Convert this to a filtered list
        // -------------------------------
            if (!found) // what's this for?
            {
                found = true;

            }
            itemName = fileList.at(i);

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


    }

    return result;
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


QStringList HTCChartFolder::CountFiles(QString path)
{
    QStringList thisFileList;
    QString file;
    QString line;
    int partsCount;

    thisFileList.clear();

    QDir dir(path);
    dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);

    if(!dir.exists())
    {
        // leave the list alone/empty
        //return thisFileList;
    }
    else

    {
        QFileInfoList sList = dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);

        for (int i = 0; i < sList.count(); i++)
        {
            line = "";
            file = sList.at(i).fileName();
            path = sList.at(i).absoluteFilePath();

            line.append(path);

            partsCount = file.split("_").count();

            //qDebug() << "file/count " << file << "/" << partsCount;

           if(_dataType == 1) // CI data
           {
               if (partsCount >= 4 && partsCount <= 5)
               {

                   if(file.contains(".csv"))
                   {
                       thisFileList.append(path);
                       // qDebug() << "type = 1 list  count is now " << thisFileList.count();
                   }

               }
               else
               {
                   //qDebug() << "item " << line << " is being tossed";
               }
           }
           else // dataType == 0 (RI)
           {
               if (partsCount == 7)
               {
                   if(file.contains(".csv"))
                   {
                       thisFileList.append(path);
                       // qDebug() << "type = 1 list  count is now " << thisFileList.count();
                   }

               }
               else
               {
                   //qDebug() << "count " << partsCount << " - " << line << " is being tossed";
               }
           }
        }



    }

    return thisFileList;
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

void HTCChartFolder::listThisFileList(QFileInfoList fList)
{
    qDebug() << "...Begin PrintingfileInfoList";
    QString file = "";
    QString path = "";
    QString pathMarker = "";
    QString line = "";
    int partsCount;

    QStringList target;
    for (int i = 0; i < fList.count(); i++)
    {
       line = "";
      file = fList.at(i).fileName();
       path = fList.at(i).absoluteFilePath();
       //all = fList.at(i).completeBaseName();

       //line.append(path);
       line.append(path);
       partsCount = file.split("_").count();
       //qDebug() << "file " << file << " count is " << partsCount;
       if(_dataType == 1)
       {
           if (partsCount >= 4 && partsCount <= 5)
           {
               //qDebug() << "item " << i << " is " << line;
           }
           else
           {
               qDebug() << "item " << line << " is being tossed";
           }
       }
       else
       {
           if (partsCount != 7)
           {
               qDebug() << "count " << partsCount << " - " << line << " is being tossed";
           }
       }


    }

    qDebug() << "...Completed PrintingfileInfoList";
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

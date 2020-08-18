#include "htcchartfolder.h"
#include <QDebug>



HTCChartFolder::HTCChartFolder(QObject *parent) : QObject(parent)
{
    _MatchingTestCodes << "q241" << "q242" << "q243" << "q244";
}

int HTCChartFolder::init(QString folder, QString extension)
{

    // ++++++++++++++++++++++++++++++++++++++++++++++++
    // check for file count first
    // we might need to protect against too many files
    // ++++++++++++++++++++++++++++++++++++++++++++++++
    int fileCount = CountFiles(folder);


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

            itemName = it.next();

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


                //if(nameToTest.contains("q241") || nameToTest.contains("q242") || nameToTest.contains("q243") == true)
                if(_MatchingTestCodes.contains(code) == true)
                {
                    thisSet.clear();
                    HTCChartDataFile *  filObj = new HTCChartDataFile(itemName);

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

                        //add it to the set if it doesn't exist
                        if(!_sets.contains(thisSet))
                        {

                            _sets.append(thisSet);
                        }

                        _folderList.append(itemName);
                        _TaggedList.append(thisTag);
                        _fileCountList.append(rowCount);

                        result += 1;

                        thisTag.clear();
                        thisSet.clear();

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

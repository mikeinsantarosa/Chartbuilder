#ifndef HTCCHARTFOLDER_H
#define HTCCHARTFOLDER_H

#include <QObject>
#include <QDir>
#include <QDebug>
#include <QDirIterator>
#include <QFile>
#include <QFileInfo>
#include "htcchartdatafile.h"



class HTCChartFolder : public QObject
{
    Q_OBJECT
public:
    explicit HTCChartFolder(QObject *parent = nullptr);

    int init(QString folder, QString extension);
    QStringList GetFolderList();
    QStringList GetDataSetNames();
    QStringList GetTaggedList();


signals:
    void messageToStatusBar(QString msg);

public slots:

private:

    int _numberOfFiles;
    QStringList _folderList;
    QStringList _fileCountList;
    QStringList _sets;
    QStringList _MatchingTestCodes;

    QStringList _TaggedList;

    void fillTaggedList();

    int CountFiles(QString path);



};

#endif // HTCCHARTFOLDER_H

// ++++++++++++++++++++++++++++++++++++++++++++ //
// File: htcdataselector.cpp
// Description: Provides a method for users
// to select a folder to collect data to analyze
// then group up
//
// Date: 2019-03-07
//
//
// TODO:
//  1-Add callout capability to charts
//
//  2-Add rescaling object for datasets
//  with values < e-12
//
//
// ++++++++++++++++++++++++++++++++++++++++++++ /

#include "htcdataselector.h"
#include "ui_htcdataselector.h"

HTCDataSelector::HTCDataSelector(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::HTCDataSelector)
{

    ui->setupUi(this);
    ui->btnSelectColumns->setEnabled(false);
    ui->treeDatasets->setHeaderLabel("Click either Get RI or CI Data then select a folder to import data from.");
    this->statusBar()->showMessage("Ready...");

    QString ver = QString("%1.%2.%3").arg(MY_MAJOR_VERSION).arg(MY_MEAN_VERSION).arg(MY_MINOR_VERSION);

    QString chartTitle = "";
    chartTitle.append("Hardware Test Center - RI ChartBuilder version: ");
    chartTitle.append(ver);
    this->setWindowTitle(chartTitle);



    _currentSearchFilter = ".csv";

    loadSettings();
    connect(ui->treeDatasets, SIGNAL(itemSelectionChanged()), this, SLOT(TreeviewItemSelected()));
}

HTCDataSelector::~HTCDataSelector()
{
    delete ui;
}

void HTCDataSelector::SetFolderInService(QString folder, QString filter, int dType, QString baseFolder)
{
    int members = 0;
    if(!folder.isEmpty())
     {
        //QString dataType = RIdataType;
        _baseFolder = baseFolder;
        _folderInService = folder;
         _filterInService = filter;
         cdf = new HTCChartFolder;
         connect(cdf,SIGNAL(messageToStatusBar(QString)),this,SLOT(messageForStatusBar(QString)));
         members = cdf->init(_folderInService, _filterInService, dType);

         qDebug() << "Discovered " << members << " to be listed";

         FillListFromPath();

     }
}



void HTCDataSelector::ColumnsHaveBeenSelected()
{
    //do an emit here with a list;
    _selectedColumnsList = dd->getSelectedColumnsList();

}

void HTCDataSelector::ColumnSelectionCompleted()
{


    if (_selectedColumnsList.count() > 1)
    {
        dm = new HTCChartDataMangler();

        this->statusBar()->showMessage("Column selection and Chart Construction Complete...");
        this->repaint();
        dm->Init(_taggedList, _selectedColumnsList, _dataType, _baseFolder);
    }
    else
    {
        this->statusBar()->showMessage("No Columns were selected...");
        this->repaint();
    }

}

void HTCDataSelector::TreeviewItemSelected()
{
    int numberOfkids = -1;
    QString selectedSet = "";
    QTreeWidgetItem * item = ui->treeDatasets->currentItem();

    if (!item->parent())
    {
        numberOfkids = item->childCount();
        selectedSet.append("Selected Data set : ");
        selectedSet.append(item->text(0));
        selectedSet.append(" - Number of files = ");
        selectedSet.append(QString::number(numberOfkids));
        ui->labelSelectedDataSet->setText(selectedSet);

        FillTaggedList(item);

        ui->btnSelectColumns->setEnabled(true);

    }
}


void HTCDataSelector::FillListFromPath()
{
    QStringList s;

    if (!_folderList.isEmpty())
    {
        _folderList.clear();
    }

    this->repaint();
    statusBar()->showMessage("Retrieving the data you pointed me to...");

    _folderList = cdf->GetTaggedList();
     _folderList.sort();
    if(_folderList.count() > 0)
    {
        fillTree();
    }
}

void HTCDataSelector::fillTree()
{
    int count = 0;
    QFileInfo info;
    QString fName;
    QString tName;
    QString msg = "";

    showCursorBusy();

    //listThisList(_folderList);

    QTreeWidgetItem *topLevelItem = NULL;

    for(const QString &fileName: qAsConst(_folderList))
    {
        QStringList splitFileName = fileName.split(",");
        info = splitFileName[1];
        tName = splitFileName[0];
        fName = info.fileName();
        //qDebug() << "info/tName/fName ->" << info << "/" << tName << "/" << fName;

        msg.clear();
        msg.append("Loading file: [");
        msg.append(fName);
        msg.append("]");
        this->statusBar()->showMessage(msg);
        this->statusBar()->update();
        //qDebug() << "message in fillTree - " << msg;

        count++;

        // add keyed string as top level item if treeWidget doesn't already have it
        if (ui->treeDatasets->findItems(splitFileName[0], Qt::MatchFixedString).isEmpty())
        {

            topLevelItem = new QTreeWidgetItem;
            topLevelItem->setText(0, splitFileName[0]);

            ui->treeDatasets->addTopLevelItem(topLevelItem);

        }

        QTreeWidgetItem *parentItem = topLevelItem;

        // iterate through non-root directories (file name comes after)
        for (int i = 1; i < splitFileName.size() - 1; ++i)
        {

            // iterate through children of parentItem to see if this directory exists
            bool thisDirectoryExists = false;
            for (int j = 0; j < parentItem->childCount(); ++j)
            {

                if (splitFileName[i] == parentItem->child(j)->text(0))
                {
                    thisDirectoryExists = true;
                    parentItem = parentItem->child(j);

                    break;
                }
            }

            if (!thisDirectoryExists)
            {
                parentItem = new QTreeWidgetItem(parentItem);
                parentItem->setText(0, splitFileName[i]);
            }
        }

        QTreeWidgetItem *childItem = new QTreeWidgetItem(parentItem);
        childItem->setText(0, splitFileName.last());

    }

    msg.clear();
    msg.append("Completed loading files from the selected path");
    this->statusBar()->showMessage(msg);
    this->statusBar()->update();

    showCursorNormal();
}

void HTCDataSelector::FillTaggedList(QTreeWidgetItem *item)
{
    if (!_taggedList.isEmpty())
    {
        _taggedList.clear();
    }

    for (int i = 0; i < item->childCount(); i++)
    {
        _taggedList.append(item->child(i)->text(0));
    }
}

void HTCDataSelector::setupTaskbar()
{
    this->statusBar()->update();
}

void HTCDataSelector::showCursorBusy()
{
    this->setCursor(QCursor(Qt::BusyCursor));
}

void HTCDataSelector::showCursorNormal()
{
    this->setCursor(QCursor(Qt::ArrowCursor));
}

void HTCDataSelector::saveSettings()
{
    // store settings
    QSettings setting("Keysight","ChartBuilder");

    setting.beginGroup("ProgramFolders");
    setting.setValue("SearchPath",_currentSearchPath);
    setting.setValue("StartupPosition",this->geometry());

    setting.endGroup();
}


void HTCDataSelector::loadSettings()
{
    // load setting

    //linux
    QString path = "/home/mandbx/Desktop/misc-docs/q241/";

    //windows
    //QString path = "";

    // make sure it's set to something
    _currentSearchPath = "";

    QSettings setting("Keysight","ChartBuilder");

    setting.beginGroup("ProgramFolders");

    _currentSearchPath = setting.value("SearchPath").toString();


    if(_currentSearchPath == "")
    {
        _currentSearchPath = path;
    }


    QRect rect = setting.value("StartupPosition").toRect();
    setGeometry(rect);

    setting.endGroup();
}




void HTCDataSelector::showNoAccessFileForLoadingMessage(QString file)
{
    QMessageBox msgBox;
    QString message = "";

    message.append("I can't open the file you selected [");
    message.append(file);
    message.append("] Maybe someone else already has it open ");
    message.append(" or you've lost access to the path.");


    msgBox.setText(message);
    msgBox.setInformativeText("Can't access the selected file & path.");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);

    int ret = msgBox.exec();
}

void HTCDataSelector::listThisList(QStringList list)
{
    for (int i = 0; i < list.count(); i++)
        qDebug() << "list #" << i << " is " << list.at(i);
}

void HTCDataSelector::on_btnClose_clicked()
{

    saveSettings();

    close();
}


void HTCDataSelector::on_btnSelectColumns_clicked()
{
    QString file = _taggedList.at(0);

    if(_taggedList.count() > 0)
    {
        dd = new HtcDataDialog();
        connect(dd,SIGNAL(ColumnSelected()),this,SLOT(ColumnsHaveBeenSelected()));
        connect(dd,SIGNAL(ColumnSelectorClosed()),this,SLOT(ColumnSelectionCompleted()));

        //dd->move(6,6);

        dd->Init(_taggedList.at(0));
        dd->show();


    }
}


void HTCDataSelector::on_actionAbout_triggered()
{
    About * a = new About;
    //a->move(50,50);
    a->show();

}

void HTCDataSelector::on_actionSettings_triggered()
{
    HTCSettings * s = new HTCSettings;
    QPoint offset = QPoint(100,20);

    QPoint startPos = QPoint(this->window()->frameGeometry().topLeft() +
                           this->window()->rect().center() -
                           rect().center());
    startPos = startPos + offset;
    s->move(startPos);
    s->show();
}

void HTCDataSelector::messageForStatusBar(QString msg)
{

    this->repaint();
    this->statusBar()->showMessage(msg);
}

// -------------------------------------------------- //
// Load Chart from File
// -------------------------------------------------- //
void HTCDataSelector::on_actionLoad_triggered()
{

    QString msg;
    QString fileName = QFileDialog::getOpenFileName(this, tr("Save Chart File"),
                               _currentSearchPath,
                               "Charts (*.chart)");

    if(!fileName.isEmpty())
    {
        HtcChart * chart = new HtcChart(this);

        QFileInfo info(fileName);
        _currentSearchPath = info.path();
        _baseFolder = _currentSearchPath;

        qDebug() << "Search path = " << _currentSearchPath;
        qDebug() << "loading file = " << fileName;

        chart->setFileToOpen(fileName, true, _baseFolder);

        chart->show();
        msg.append("Chart data has been loaded...");

        messageForStatusBar(msg);

     }

}

void HTCDataSelector::on_btnSetCIFolders_clicked()
{
    if (CIDataisEnabled)
    {
        getCIData();
    }

}

void HTCDataSelector::getCIData()
{
    QString folderToOpen = QFileDialog::getExistingDirectory(this, tr("Select a folder to Load files from..."), _currentSearchPath, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    _currentSearchPath = folderToOpen;


    if (!folderToOpen.isEmpty())
    {
        _baseFolder = folderToOpen;
        // set dataType
        setDataType(CIdataType);
        ui->treeDatasets->clear();
        ui->labelSelectedDataSet->clear();
        ui->treeDatasets->setHeaderLabel("Click on a CI Set of data in the list below then Click Select Columns");
        this->repaint();
        statusBar()->showMessage("Retrieving the data you pointed me to...");
        SetFolderInService(folderToOpen, _currentSearchFilter, CIdataType, _baseFolder);

        qDebug() << "folder opened " << folderToOpen;
    }
}

void HTCDataSelector::on_btnSetRIFolders_clicked()
{
    if (RIDataisEnabled)
    {
        getRIData();
    }

}

void HTCDataSelector::getRIData()
{
    QString folderToOpen = QFileDialog::getExistingDirectory(this, tr("Select a folder to Load files from..."), _currentSearchPath, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    _currentSearchPath = folderToOpen;


    if (!folderToOpen.isEmpty())
    {
        //set dataType
        _baseFolder = folderToOpen;
        setDataType(RIdataType);
        ui->treeDatasets->clear();
        ui->labelSelectedDataSet->clear();
        ui->treeDatasets->setHeaderLabel("Click on an RI Set of data in the list below then Click Select Columns");
        this->repaint();
        statusBar()->showMessage("Retrieving the data you pointed me to...");
        SetFolderInService(folderToOpen, _currentSearchFilter, RIdataType, _baseFolder);

        qDebug() << "folder opened " << folderToOpen;

    }

}


void HTCDataSelector::setDataType(int dataType)
{
    _dataType = dataType;
}

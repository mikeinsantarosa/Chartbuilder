#ifndef HTCDATASELECTOR_H
#define HTCDATASELECTOR_H

#include <QMainWindow>
#include <QFileDialog>
#include <QDebug>
#include <QFileInfoList>
#include <QFileInfo>
#include <QDir>
#include <QTreeWidget>
#include <QRect>
#include <QSettings>
#include <QMessageBox>

#include "htcchartfolder.h"
#include "htcchartdatafile.h"
#include "htcdatadialog.h"
#include "htcchartdatamangler.h"
#include "htcchartfolder.h"
#include "datapoint.h"
#include "about.h"
#include "htcsettings.h"
#include "version.h"


namespace Ui {
class HTCDataSelector;
}

class HTCDataSelector : public QMainWindow
{
    Q_OBJECT

public:
    explicit HTCDataSelector(QWidget *parent = nullptr);
    ~HTCDataSelector();

    void SetFolderInService(QString folder, QString filter, int dType, QString baseFolder);


private slots:
    void on_btnSetRIFolders_clicked();

    void ColumnsHaveBeenSelected();

    void ColumnSelectionCompleted();

    void TreeviewItemSelected();

    void on_btnClose_clicked();

    void on_btnSelectColumns_clicked();

    void on_actionAbout_triggered();

    void on_actionSettings_triggered();

    void messageForStatusBar(QString msg);

    void on_actionLoad_triggered();

    void on_btnSetCIFolders_clicked();

private:

    // new CI Charts parts
    int RIdataType = 0;
    int CIdataType = 1;
    int _dataType = -1; // not initialized yet
    void setDataType(int dataType);
    QString _baseFolder;


    // end CI Chart parts
    QString _currentSearchFilter;
    QString _currentSearchPath;

    Ui::HTCDataSelector *ui;

    QString _folderInService;
    QString _filterInService;


    QStringList _folderList;

    QStringList _taggedList;
    QVector <int> _selectedColumnsList;

    HTCChartFolder * cdf;
    HtcDataDialog * dd;
    HTCChartDataMangler * dm;

    void FillListFromPath();
    void fillTree();

    void FillTaggedList(QTreeWidgetItem * item);

    void setupTaskbar();

    // window position setting methods
    void writePositionSettings();
    void readPositionSettings();
    void showCursorBusy();
    void showCursorNormal();

    void saveSettings();
    void loadSettings();

    // main launchers
    void getRIData();
    void getCIData();
    //-------------------------------------//
    // Data mode enablers
    // true/false controls get data mode
    // turn CI off for publishing
    // turn CI on for developing
    // -------------------------------------//
    //
    bool RIDataisEnabled = true;
    bool CIDataisEnabled = true;
    //
    //-------------------------------------//
    //
    //
    //
    //-------------------------------------//

    void showNoAccessFileForLoadingMessage(QString file);


    //debug functions
    void listThisList(QStringList list);


};

#endif // HTCDATASELECTOR_H

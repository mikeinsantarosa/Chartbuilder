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
#include "globals.h"


namespace Ui {
class HTCDataSelector;
}

class HTCDataSelector : public QMainWindow
{
    Q_OBJECT

public:
    explicit HTCDataSelector(QWidget *parent = nullptr);
    ~HTCDataSelector();

    void SetFolderInService(QString folder, QString filter);


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

private:

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

    void showNoAccessFileForLoadingMessage(QString file);

    // new Aug-16-2019
    void setupControls();
    void setControlsFont();
    QFont _formFont;


    //debug functions
    void listThisList(QStringList list);


};

#endif // HTCDATASELECTOR_H

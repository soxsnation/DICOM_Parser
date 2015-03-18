#ifndef __MAINWINDOW__
#define __MAINWINDOW__

#include <QMainWindow>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QMap>
#include <QCheckBox>

#include <QXmlStreamReader>

// Dicom Toolkit
#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dctk.h>
#include <dcmtk/dcmdata/dcfilefo.h>
#include <dcmtk/dcmdata/dcitem.h>
#include <dcmtk/dcmdata/dcistrmb.h>
#include <dcmtk/dcmdata/dcostrmb.h>

#include "xmlItem.h"

//namespace Ui {
//class MainWindow;
//}

//namespace Decimal {
//namespace DICOM_Parser {

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:

    void createLayout();

    void createWidgets();

    void init();

    QMap<QString, QPair<QString, QString> > loadDataDictionary();

    xmlItem parse(QXmlStreamReader &xml);

    bool writeDICOMXML(xmlItem item);

    void write_xml_item(QXmlStreamWriter &writer, xmlItem item);

private slots:

    void formatXML();

    void parseDICOM();

    void selectFile();

    void selectOutputDirectory();

private:
//    Ui::MainWindow *ui;

    QFileDialog *myFileDialog;

    QLineEdit *myFilePath;
    QLineEdit *myOutputFilePath;
    QPushButton *mySelectFileButton;
    QPushButton *mySelectOutDirectoryButton;
    QPushButton *myParseButton;
    QPushButton *myFormatXMLButton;

    QCheckBox *myMinDataCheckBox;
    QCheckBox *myOpenFileCheckBox;

    char* fileName;
    char* outfileName;


    QMap<QString, QPair<QString, QString> > myDataDictionary;
};
//}
//}

#endif // __MAINWINDOW__



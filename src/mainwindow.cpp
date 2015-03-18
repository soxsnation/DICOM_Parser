#include "mainwindow.h"
#include "xmlItem.h"
#include "xmlAttribute.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <QDebug>

#include <QDesktopServices>
#include <QUrl>
#include <QSettings>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    init();
    createWidgets();
    createLayout();

}

MainWindow::~MainWindow()
{
}

void MainWindow::createLayout()
{
    QVBoxLayout *mainLayout = new QVBoxLayout();

    QHBoxLayout *l = new QHBoxLayout();
    l->addWidget(myFilePath);
    l->addWidget(mySelectFileButton);

    QHBoxLayout *l2 = new QHBoxLayout();
    l2->addWidget(myOutputFilePath);
    l2->addWidget(mySelectOutDirectoryButton);

    mainLayout->addWidget(new QLabel("Select DICOM File:"));
    mainLayout->addLayout(l);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(new QLabel("Select Output Directory:"));
    mainLayout->addLayout(l2);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(myOpenFileCheckBox);
    mainLayout->addWidget(myMinDataCheckBox);
    mainLayout->addWidget(myParseButton);
//    mainLayout->addWidget(myFormatXMLButton);

    QWidget *mainWidget = new QWidget();
    mainWidget->setMinimumWidth(750);
    mainWidget->setMinimumHeight(275);
    mainWidget->setLayout(mainLayout);


    setCentralWidget(mainWidget);
}

void MainWindow::createWidgets()
{
    QSettings* settings = new QSettings("DICOM_Parser.ini", QSettings::IniFormat);
    myFileDialog = new QFileDialog();

    myFilePath = new QLineEdit(settings->value("open_dir").toString());
    mySelectFileButton = new QPushButton("...");

    myOutputFilePath = new QLineEdit(settings->value("export_dir").toString());
    mySelectOutDirectoryButton = new QPushButton("...");

    myParseButton = new QPushButton("Parse to XML");
    myFormatXMLButton = new QPushButton("Format");

    myMinDataCheckBox = new QCheckBox("Minify Data");
    myMinDataCheckBox->setToolTip("Data longer then 55 characters will not be written out(UIDs are 53 characters long)");

    myOpenFileCheckBox = new QCheckBox("Open file after parse");
    myOpenFileCheckBox->setChecked(settings->value("open_file").toBool());


    connect(mySelectFileButton, SIGNAL(clicked()), this, SLOT(selectFile()));
    connect(mySelectOutDirectoryButton, SIGNAL(clicked()), this, SLOT(selectOutputDirectory()));
    connect(myParseButton, SIGNAL(clicked()), this, SLOT(parseDICOM()));
    connect(myFormatXMLButton, SIGNAL(clicked()), this, SLOT(formatXML()));

}

void MainWindow::init()
{
    QSettings* settings = new QSettings("DICOM_Parser.ini", QSettings::IniFormat);
    if (!settings->allKeys().contains("open_dir"))
    {
        settings->setValue(QString("open_dir"), QDir::currentPath());
    }
    if (!settings->allKeys().contains("export_dir"))
    {
        settings->setValue(QString("export_dir"), QDir::currentPath());
    }
    if (!settings->allKeys().contains("data_dictionary"))
    {
        settings->setValue(QString("data_dictionary"), QString("%1/%2").arg(QDir::currentPath()).arg("dataDictionary.in"));
    }
    if (!settings->allKeys().contains("open_file"))
    {
        settings->setValue(QString("open_file"), true);
    }
    if (!settings->allKeys().contains("minify_length"))
    {
        settings->setValue(QString("minify_length"), 60);
    }

}

QMap<QString, QPair<QString, QString> > MainWindow::loadDataDictionary()
{
    QSettings* settings = new QSettings("DICOM_Parser.ini", QSettings::IniFormat);
    QMap<QString, QPair<QString, QString> > dd;
    QFile inFile (settings->value("data_dictionary").toString());
    if (!inFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Can not open data dictionary";
    }

    QTextStream in(&inFile);
    QString temp;
    QStringList temp2;

    while (!in.atEnd()) {

        temp = in.readLine();
        temp2 = temp.split(";");
        if (temp.length() > 2) {
            dd.insert(temp2[0], QPair<QString, QString>(temp2[1], temp2[2]));
        }
    }
    inFile.close();
    return dd;
}

void MainWindow::parseDICOM()
{

    std::ifstream is (myFilePath->text().toStdString(), std::ifstream::binary);
        if (is) {
            // get length of file:
            is.seekg (0, is.end);
//            dicomBlob.size = is.tellg();

            int bufSize = is.tellg();
            is.seekg (0, is.beg);

            char * buffer = new char [bufSize];
            // read data as a block:
            is.read (buffer, bufSize);

            if (is)
            {
                void *buf = buffer;
                //std::cout << "all characters read successfully: " << std::endl;
                is.close();

                DcmInputBufferStream dcmIS;
                dcmIS.setBuffer(buf, bufSize);
                dcmIS.setEos();
                dcmIS.putback();

                DcmFileFormat dcmFile;
                OFCondition status = dcmFile.read(dcmIS);
                if (status.good())
                {
                    //std::cout << "good"  << std::endl;
                }
                else
                {
                    std::cout << "bad: " << status.text()  << std::endl;
                }

                dcmIS.releaseBuffer();
                std::ofstream myfile(fileName);
                std::stringstream ss;
                dcmFile.writeXML(ss, bufSize);

                myDataDictionary = loadDataDictionary();
                xmlItem rootItem;
                QXmlStreamReader xml(QString::fromStdString(ss.str()));

                QXmlStreamReader::TokenType token = xml.readNext();
                while (!xml.atEnd() && token != QXmlStreamReader::StartElement)
                {
                    token = xml.readNext();
                }
                rootItem = parse(xml);
                writeDICOMXML(rootItem);
            }
            else
            {
                is.close();
                std::cout << "error: only " << is.gcount() << " could be read";
            }
        }

}

void MainWindow::formatXML()
{
    qDebug() << "MainWindow::formatXML()";

    myDataDictionary = loadDataDictionary();
    xmlItem rootItem;
    rootItem.setTag(QString("Root"), QString(""));
//    QMap<QString, QPair<QString, QString> > dd = loadDataDictionary();
    QFile* file = new QFile(fileName);
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Couldn't open file";
    }
    QXmlStreamReader xml(file);


    QXmlStreamReader::TokenType token = xml.readNext();
    while (!xml.atEnd() && token != QXmlStreamReader::StartElement)
    {
        token = xml.readNext();
    }
//    qDebug() << "token: " << token << xml.name().toString();

    rootItem = parse(xml);

    writeDICOMXML(rootItem);
}

xmlItem MainWindow::parse(QXmlStreamReader &xml)
{
//    qDebug() << "MainWindow::parse: " << xml.name() << xml.tokenType() << myDataDictionary.count();
    QXmlStreamReader::TokenType token = xml.tokenType();
    bool inElement = true;
    xmlItem xi;

    xi.setTag(QString(xml.name().toString()), QString(xml.text().toString()));

    for (int i = 0; i < xml.attributes().count(); ++i)
    {
        if (xml.attributes().at(i).name() == "tag")
        {
            xmlItemAttribute ia;
            ia.setAttribute(xml.attributes().at(i).name().toString(), xml.attributes().at(i).value().toString());
            xi.addAttribute(ia);

            QString tagCode(xml.attributes().at(i).value().toString());
            tagCode = tagCode.remove(",").toUpper();
            for (QMap<QString, QPair<QString, QString> >::iterator i = myDataDictionary.begin(); i != myDataDictionary.end(); ++i) {
                if (i.key() == tagCode) {
//                    qDebug() << "FOUND TAGCODE: " << tagCode << i.value();
                    xmlItemAttribute ia2;
                    ia2.setAttribute(QString("name"), i.value().first);
                    xi.addAttribute(ia2);
                    break;
                }
            }
        }
    }

    while(!xml.atEnd() && !xml.hasError() && inElement)
    {
        token = xml.readNext();
        if(token == QXmlStreamReader::EndDocument) { inElement = false; }
        else if(token == QXmlStreamReader::EndElement) { inElement = false; }
        else if(token == QXmlStreamReader::StartElement)
        {
            xmlItem cxi = parse(xml);
            xi.addItem(cxi);
        }
        else if(token == QXmlStreamReader::Characters && xi.tag() == "element")
        {
//            qDebug() << "xml.text()" << xml.text();
            xi.setValue(xml.text().toString());
        }
    }

//    qDebug() << "Returning xi" << xi.tag() << xi.itemCount();
    return xi;
}

bool MainWindow::writeDICOMXML(xmlItem item)
{
    QSettings* settings = new QSettings("DICOM_Parser.ini", QSettings::IniFormat);
    qDebug() << "MainWindow::writeDICOMXML" << item.itemCount();
    QFileInfo fileInfo(myFilePath->text());
    QString outPath = QString("%1/%2.xml").arg(myOutputFilePath->text()).arg(fileInfo.fileName());
    QFile* file = new QFile(outPath);
    if (!file->open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Couldn't open file";
    }
    QXmlStreamWriter xml_out(file);
    xml_out.setAutoFormatting(true);
    xml_out.writeStartDocument();

    write_xml_item(xml_out, item);
    xml_out.writeEndDocument();
    file->close();

    QDir f(myFilePath->text());
//    qDebug() << "outfileName" << QString("file:///%1").arg(outPath);

    if (settings->value("open_file").toBool())
    {
        QDesktopServices::openUrl(QUrl(QString("file:///%1").arg(outPath)));
    }

    return true;
}

void MainWindow::write_xml_item(QXmlStreamWriter &writer, xmlItem item)
{
    QSettings* settings = new QSettings("DICOM_Parser.ini", QSettings::IniFormat);
    if (item.itemCount() == 0)
    {
        writer.writeStartElement(item.tag());
        for (int i = 0; i < item.attributeCount(); ++i)
        {
            writer.writeAttribute(QString(), item.attribute(i).tag(), item.attribute(i).value());
        }
        if (myMinDataCheckBox->isChecked() && item.value().length() > settings->value("minify_length").toInt())
        {
            writer.writeCharacters(" ");
        }
        else
        {
            writer.writeCharacters(item.value());
        }
        writer.writeEndElement();
    }
    else
    {
         writer.writeStartElement(item.tag());
         for (int i = 0; i < item.attributeCount(); ++i)
         {
             writer.writeAttribute(QString(), item.attribute(i).tag(), item.attribute(i).value());
         }
         for (int i = 0; i < item.itemCount(); ++i)
         {
             write_xml_item(writer, item.item(i));
         }
         writer.writeEndElement();
    }
}

void MainWindow::selectFile()
{
    QSettings* settings = new QSettings("DICOM_Parser.ini", QSettings::IniFormat);
    QString path = QFileDialog::getOpenFileName(this, tr("DICOM File"), myFilePath->text());
    if ( path.isNull() == false )
    {
        settings->setValue(QString("open_dir"), path);
        QFileInfo fi (path);
        myFilePath->setText(path);
        myOutputFilePath->setText(fi.absolutePath());
        settings->setValue(QString("export_dir"), fi.absolutePath());
    }
}

void MainWindow::selectOutputDirectory()
{
    QSettings* settings = new QSettings("DICOM_Parser.ini", QSettings::IniFormat);
    QString path = QFileDialog::getExistingDirectory(this, tr("Output Directory"), myOutputFilePath->text());
    if ( path.isNull() == false )
    {
        settings->setValue(QString("export_dir"), path);
        myOutputFilePath->setText(path);
    }
}


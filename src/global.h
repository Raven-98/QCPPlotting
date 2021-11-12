#ifndef GLOBAL_H
#define GLOBAL_H

#include <QCoreApplication>
#include <QSettings>
#include <QString>
#include <QDialog>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QTabWidget>
#include <QWidget>
#include <QDialogButtonBox>
#include <QObject>
#include <QStandardItemModel>
#include <QFile>
#include <QTextStream>
#include <QGroupBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QSpacerItem>
#include <QDoubleSpinBox>
#include <QFont>
#include <QFontComboBox>
#include <QScrollArea>
#include <QMenu>
#include <QAction>
#include <QBoxLayout>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QFileDialog>
#include <QMainWindow>
#include <QMdiArea>
#include <QMenuBar>
#include <QToolBar>
#include <QMdiSubWindow>
#include <QMessageBox>
#include <QThread>
#include <QApplication>
#include <QClipboard>
#include <QTableView>
#include <QHeaderView>
#include <QKeyEvent>
#include <QPainter>
#include <QMouseEvent>
#include <QStyledItemDelegate>

#include "qcustomplot.h"

#include <QDebug>

namespace GBS {

    // ChartSettings
    namespace CS {
        enum csDialodType {
            Global,
            Axis,
            Graph,
        };
        enum Grid {
            Horizontal,
            Vertical,
        };
    }

    // FileDialogSetDetails
    namespace FDSD
    {
        enum Delimiter
        {
            Comma,
            Tab_step,
            Semicolon,
            Space,
        };

        struct Data
        {
            bool readZeroRow;
            bool readZeroColumn;
            short delimiter;
        };

        enum Type
        {
            Open,
            Save,
        };
    }

    // DialogSavePlot
    namespace DSP
    {
        struct Data
        {
            int Width;
            int Height;
            double Scale;
            short Quality;
            int Resolution;
            short ResolutionUnit;
        };
    }

    // DialogAnalyzeDiffractionDataDRON2
    namespace DADDDRON2
    {
        struct Data
        {
            QString fileName;
            float twoThetaStart;
            float twoThetaEnd;
            short delimiter;
        };
    }

    // ChartWidget
    namespace CW{
        enum ChartStyle{
            Graph,
            Bars,
            Curve,
            Financial,
            StatisticalBox,
        };
    }


    struct Settings
    {
        void loadSettings(QString *fileName)
        {
            QSettings *settings = new QSettings(QCoreApplication::applicationDirPath() + *fileName, QSettings::IniFormat);

            MainWindow.width = settings->value("MainWindow/width").toInt();
            MainWindow.heidht = settings->value("MainWindow/heidht").toInt();

            FileDialogSetDetails.filePaht = settings->value("FileDialogSetDetails/filePaht").toString();
            FileDialogSetDetails.readZeroRow = settings->value("FileDialogSetDetails/readZeroRow").toBool();
            FileDialogSetDetails.readZeroColumn = settings->value("FileDialogSetDetails/readZeroColumn").toBool();
            FileDialogSetDetails.delimiter = settings->value("FileDialogSetDetails/delimiter").toInt();

            DialogSavePlot.filePaht = settings->value("DialogSavePlot/filePaht").toString();

            DialogAnalyzeDiffractionDataDRON2.fileName = settings->value("DialogAnalyzeDiffractionDataDRON2/file").toString();
            DialogAnalyzeDiffractionDataDRON2.twoThetaStart = settings->value("DialogAnalyzeDiffractionDataDRON2/twoThetaStart").toInt();
            DialogAnalyzeDiffractionDataDRON2.twoThetaEnd = settings->value("DialogAnalyzeDiffractionDataDRON2/twoThetaEnd").toInt();
            DialogAnalyzeDiffractionDataDRON2.delimiter = settings->value("DialogAnalyzeDiffractionDataDRON2/delimiter").toInt();

            delete settings;
        }

        void saveSettings(QString *fileName)
        {
            QSettings *settings = new QSettings(QCoreApplication::applicationDirPath() + *fileName, QSettings::IniFormat);

            settings->setValue("MainWindow/width", MainWindow.width);
            settings->setValue("MainWindow/heidht", MainWindow.heidht);

            settings->setValue("FileDialogSetDetails/filePaht", FileDialogSetDetails.filePaht);
            settings->setValue("FileDialogSetDetails/readZeroRow", FileDialogSetDetails.readZeroRow);
            settings->setValue("FileDialogSetDetails/readZeroColumn", FileDialogSetDetails.readZeroColumn);
            settings->setValue("FileDialogSetDetails/delimiter", FileDialogSetDetails.delimiter);

            settings->setValue("DialogSavePlot/filePaht", DialogSavePlot.filePaht);

            settings->setValue("DialogAnalyzeDiffractionDataDRON2/file", DialogAnalyzeDiffractionDataDRON2.fileName);
            settings->setValue("DialogAnalyzeDiffractionDataDRON2/twoThetaStart", DialogAnalyzeDiffractionDataDRON2.twoThetaStart);
            settings->setValue("DialogAnalyzeDiffractionDataDRON2/twoThetaEnd", DialogAnalyzeDiffractionDataDRON2.twoThetaEnd);
            settings->setValue("DialogAnalyzeDiffractionDataDRON2/delimiter", DialogAnalyzeDiffractionDataDRON2.delimiter);

            delete settings;
        }

        struct MainWindow
        {
            int width;
            int heidht;
        };

        struct FileDialogSetDetails : FDSD::Data
        {
            FileDialogSetDetails& operator<<(const FDSD::Data &data)
            {
                if (this == &data)
                    return *this;

                readZeroRow = data.readZeroRow;
                readZeroColumn = data.readZeroColumn;
                delimiter = data.delimiter;

                return *this;
            }

            QString filePaht;
//            bool readZeroRow;
//            bool readZeroColumn;
//            short delimiter;
        };

        struct DialogSavePlot
        {
            QString filePaht;
        };

        struct DialogAnalyzeDiffractionDataDRON2 : DADDDRON2::Data
        {
            DialogAnalyzeDiffractionDataDRON2& operator<<(const DADDDRON2::Data &data)
            {
                if (this == &data)
                    return *this;

                fileName = data.fileName;
                twoThetaStart = data.twoThetaStart;
                twoThetaEnd = data.twoThetaEnd;
                delimiter = data.delimiter;

                return *this;
            };

//            QString file;
//            float twoThetaStart;
//            float twoThetaEnd;
//            short delimiter;
        };

        MainWindow MainWindow;
        FileDialogSetDetails FileDialogSetDetails;
        DialogSavePlot DialogSavePlot;
        DialogAnalyzeDiffractionDataDRON2 DialogAnalyzeDiffractionDataDRON2;

    };
}


#endif // GLOBAL_H

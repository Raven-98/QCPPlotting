#ifndef STRUCTS_H
#define STRUCTS_H

#include <QSettings>
#include <QMap>
#include <QStringList>
#include <QAbstractItemModel>

namespace QCPPlotting {
  struct Settings;
  struct SpreadSheet;
  struct Sheet;
  struct Row;
  struct NumSheet;
  struct NumColumn;

  enum CSVParameters {
    READ_ZERO_ROW = 0x01,
    READ_ZERO_COLUMN = 0x02,
    USE_ZERO_ROW = 0x04,
  };

  enum CSVDelimiter {
    COMMA = 0,
    TAB_STEP,
    SEMICOLON,
    SPACE,
  };

  enum ChartType {
    GRAPH,
    BARS,
    CURVE,
    FINANCIAl,
    STATISTICALBOX,
  };

  namespace FileDialogSetDetails {
    struct Data
    {
      bool readZeroRow{false};
      bool readZeroColumn{false};
      bool useZeroRow{false};
      bool writeHHeader{false};
      short delimiter{0};
    };

    enum Type
    {
      Open,
      Save,
    };
  }

  struct Settings : public QSettings {
    struct MainWindow {
      int width;
      int heidht;
    } MainWindow;

    struct FileDialogSetDetails : QCPPlotting::FileDialogSetDetails::Data
    {
      FileDialogSetDetails& operator<<(const QCPPlotting::FileDialogSetDetails::Data &data)
      {
        if (this != &data) {
            readZeroRow = data.readZeroRow;
            readZeroColumn = data.readZeroColumn;
            useZeroRow = data.useZeroRow;
            writeHHeader = data.writeHHeader;
            delimiter = data.delimiter;
          }
        return *this;
      }

      QString filePaht;
    } FileDialogSetDetails;

    void loadSettings(){
      MainWindow.width = value("MainWindow/width").toInt();
      MainWindow.heidht = value("MainWindow/heidht").toInt();

      FileDialogSetDetails.readZeroRow = value("FileDialogSetDetails/readZeroRow").toBool();
      FileDialogSetDetails.readZeroColumn = value("FileDialogSetDetails/readZeroColumn").toBool();
      FileDialogSetDetails.useZeroRow = value("FileDialogSetDetails/useZeroRow").toBool();
      FileDialogSetDetails.writeHHeader = value("FileDialogSetDetails/writeHHeader").toBool();
      FileDialogSetDetails.delimiter = value("FileDialogSetDetails/delimiter").toInt();
    };

    void saveSettings() {
      setValue("MainWindow/width", MainWindow.width);
      setValue("MainWindow/heidht", MainWindow.heidht);

      setValue("FileDialogSetDetails/readZeroRow", FileDialogSetDetails.readZeroRow);
      setValue("FileDialogSetDetails/readZeroColumn", FileDialogSetDetails.readZeroColumn);
      setValue("FileDialogSetDetails/useZeroRow", FileDialogSetDetails.useZeroRow);
      setValue("FileDialogSetDetails/writeHHeader", FileDialogSetDetails.writeHHeader);
      setValue("FileDialogSetDetails/delimiter", FileDialogSetDetails.delimiter);
    };
  };

  struct SpreadSheet : public QMap<QString, Sheet> {
    using QMap<QString, Sheet>::QMap;
  };

  struct Sheet : public QList<Row> {
    using QList<Row>::QList;
    QStringList HorizontalHeader;

    static Sheet fromModel(QAbstractItemModel *model) {
      Sheet sheet;

      return sheet;
    }
  };

  struct Row : public QStringList {
    using QStringList::QStringList;
  };

  struct NumSheet : public QList<NumColumn> {
    using QList<NumColumn>::QList;
    QStringList HorizontalHeader;
  };

  struct NumColumn : public QList<double> {
    using QList<double>::QList;
  };
}

#endif // STRUCTS_H

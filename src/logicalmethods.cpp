#include "logicalmethods.h"

#include <QFile>
#include <QTextStream>

bool LogicalMethods::readCSV(QCPPlotting::SpreadSheet &spreadSheet, const QString &file, const char &delimiter, const unsigned char &parameters)
{
  if (file.data()->isNull()) {
      emit error(tr("No file name specified"));
      return false;
    }

  QFile csv_doc(file);
  if (!csv_doc.open(QFile::ReadOnly | QFile::Text)) {
      emit error(tr("'%1' is not open").arg(file));
      return false;
    }

  QTextStream in(&csv_doc);

  QCPPlotting::Sheet sheet;
  QString line;

  line = in.readLine();

  auto readLine = [parameters, delimiter](QString &line){
      QCPPlotting::Row row;
      bool rc;
      if (parameters & QCPPlotting::READ_ZERO_COLUMN)
        rc = true;
      else
        rc = false;
      for (auto &&item : line.split(delimiter)) {
          if (rc)
              row.append(item);
          else
            rc = true;
        }
      return row;
  };

  if (parameters & QCPPlotting::READ_ZERO_ROW) {
      if (parameters & QCPPlotting::USE_ZERO_ROW)
          sheet.HorizontalHeader = readLine(line);
      else
          sheet.append(readLine(line));
    }

  while (!in.atEnd()) {
      line = in.readLine();
      sheet.append(readLine(line));
    }

  csv_doc.close();

  spreadSheet.insert(file.split("/").takeLast().split(".").first(), sheet);

  return true;
}

bool LogicalMethods::writeCSV(const QCPPlotting::Sheet &sheet, const QString &file, const char &delimiter, const unsigned char &parameters)
{
  QFile csv_doc(file);
  if (!csv_doc.open(QFile::WriteOnly | QIODevice::Truncate)) {
      emit error(tr("'%1' is not open").arg(file));
      return false;
    }

  QTextStream in(&csv_doc);

  if (parameters & QCPPlotting::USE_ZERO_ROW) {
      auto hHeader{&sheet.HorizontalHeader};
      for (int c = 0; c < hHeader->count() - 1; ++c) {
          in << hHeader->at(c) << delimiter;
        }
      in << hHeader->at(hHeader->count() - 1) << "\n";
    }

  for (int r = 0; r < sheet.count(); ++r) {
      auto row{sheet.at(r)};
      for (int c = 0; c < row.count() - 1; ++c) {
          in << row.at(c) << delimiter;
        }
      in << row.at(row.count() - 1) << "\n";
    }

  csv_doc.close();

  return true;
}

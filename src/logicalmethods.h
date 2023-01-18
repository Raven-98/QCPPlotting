#ifndef LOGICALMETHODS_H
#define LOGICALMETHODS_H

#include <QObject>
#include <QAbstractItemModel>

#include "structs.h"

class LogicalMethods : public QObject
{
  Q_OBJECT

public:
  using QObject::QObject;

  bool readCSV(QCPPlotting::SpreadSheet &spreadSheet, const QString &file, const char &delimiter, const unsigned char &parameters);
  bool writeCSV(const QCPPlotting::Sheet &sheet, const QString &file, const char &delimiter, const unsigned char &parameters);

signals:
  void error(QString);
  void warning(QString);
  void message(QString);

private:

};

#endif // LOGICALMETHODS_H

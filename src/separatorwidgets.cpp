#include "separatorwidgets.h"


Spacer::Spacer(QSizePolicy::Policy hPolicy, QSizePolicy::Policy vPolicy)
  : QSpacerItem(0, 0, hPolicy, vPolicy)
{}

VSpacer::VSpacer()
  : Spacer(QSizePolicy::Minimum, QSizePolicy::Expanding)
{}


HSpacer::HSpacer()
  : Spacer(QSizePolicy::Expanding, QSizePolicy::Minimum)
{}

VLine::VLine(int margin, QWidget *parent)
  : QFrame(parent)
{
  setStyleSheet(QString("margin: %1px;").arg(margin));
  setFrameShape(QFrame::VLine);
}

HLine::HLine(int margin, QWidget *parent)
  : QFrame(parent)
{
  setStyleSheet(QString("margin: %1px;").arg(margin));
  setFrameShape(QFrame::HLine);
}

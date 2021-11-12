#include "aboutprogram.h"

#include <QDebug>

AboutProgram::AboutProgram(QWidget *parent)
    : QDialog(parent)
{
    resize(500, 400);
    setWindowTitle(tr("About program"));

    iconLabel = new QLabel;
    iconLabel->setPixmap(QPixmap(":/img/plot.png"));
    iconLabel->setFixedSize(64,64);

    nameLabel = new QLabel;
    nameLabel->setText("<html><head/><body><p><span style=\" font-size:18pt; font-weight:600;\">"
                       + QCoreApplication::applicationName()
                       + "</span></p></body></html>");

    versionLabel = new QLabel;
    versionLabel->setText(tr("Version ") + QCoreApplication::applicationVersion());

    tabsWidget = new QTabWidget;

    tabAbout = new QWidget;
    labelAbout = new QLabel;
    labelAbout->setText(tr("Graphing software"));
    labelAbout->setWordWrap(true);
    labelAboutOwner = new QLabel;
    labelAboutOwner->setText("<html><head/><body><p>"
                                "© Raven-98, 2021"
                                "</p></body></html>");
    labelAboutWEB = new QLabel;
    labelAboutWEB->setText("<html><head/><body><p>"
                            "<a href=\"https://github.com/Raven-98/graphics_building_software\">"
                            "<span style=\" text-decoration: underline; color:#2980b9;\">"
                            "https://github.com/Raven-98/graphics_building_software"
                            "</span></a></p></body></html>");
    labelAboutLicense = new QLabel;
    labelAboutLicense->setText(tr("<html><head/><body><p>"
                                    "License: GNU General Public License v3.0"
                                    "</p><p><a href=\"http://www.gnu.org/licenses/gpl-3.0.html\">"
                                    "<span style=\" text-decoration: underline; color:#2980b9;\">"
                                    "http://www.gnu.org/licenses/gpl-3.0.html"
                                    "</span></a></p></body></html>"));
    boxLayoutAbout = new QVBoxLayout;
    boxLayoutAbout->addWidget(labelAbout);
    boxLayoutAbout->addWidget(labelAboutOwner);
    boxLayoutAbout->addWidget(labelAboutWEB);
    boxLayoutAbout->addWidget(labelAboutLicense);
    tabAbout->setLayout(boxLayoutAbout);
    tabsWidget->addTab(tabAbout, tr("About"));

    tabLibraries = new QWidget;
    labelLibraries_Qt = new QLabel;
    labelLibraries_Qt->setText(QString("Qt ") + QT_VERSION_STR);
    labelLibraries_QCP = new QLabel;
    labelLibraries_QCP->setText(QString("QCustomPlot ") + QCUSTOMPLOT_VERSION_STR);
    boxLayoutLibraries = new QVBoxLayout;
    boxLayoutLibraries->addWidget(labelLibraries_Qt);
    boxLayoutLibraries->addWidget(labelLibraries_QCP);
    tabLibraries->setLayout(boxLayoutLibraries);
    tabsWidget->addTab(tabLibraries, tr("Platforms and libraries"));

    tabAuthors = new QWidget;
    labelAuthors_Raven98 = new QLabel;
    labelAuthors_Raven98->setText(tr("<html><head/><body><p><span style=\" font-weight:600;\">"
                                     "Raven-98"
                                     "</span></p><p><span style=\" font-style:italic;\">"
                                     "Developer"
                                     "</span></p><p>E-mail: <a href=\"andriy1898k@hotmail.com\">"
                                     "<span style=\" text-decoration: underline; color:#2980b9;\">"
                                     "andriy1898k@hotmail.com"
                                     "</span></a></p></body></html>"));
    boxLayoutAuthors = new QVBoxLayout;
    boxLayoutAuthors->addWidget(labelAuthors_Raven98);
    tabAuthors->setLayout(boxLayoutAuthors);
    tabsWidget->addTab(tabAuthors, tr("Authors"));

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Close);
    connect(buttonBox,&QDialogButtonBox::rejected,this,&AboutProgram::close);

    gridLayout = new QGridLayout;

    gridLayout->addWidget(iconLabel,0,0,3,1);
    gridLayout->addWidget(nameLabel,0,1);
    gridLayout->addWidget(versionLabel,1,1);
    gridLayout->addWidget(tabsWidget,3,0,1,2);
    gridLayout->addWidget(buttonBox,4,0,1,2);

    setLayout(gridLayout);
}

AboutProgram::~AboutProgram()
{
    delete iconLabel;
    delete nameLabel;
    delete versionLabel;
    delete labelAbout;
    delete labelAboutOwner;
    delete labelAboutWEB;
    delete labelAboutLicense;
    delete labelLibraries_Qt;
    delete labelAuthors_Raven98;
    delete boxLayoutAbout;
    delete boxLayoutLibraries;
    delete boxLayoutAuthors;
    delete tabAbout;
    delete tabLibraries;
    delete tabAuthors;
    delete tabsWidget;
    delete gridLayout;
}

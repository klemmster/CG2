/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Sun May 6 17:47:25 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>
#include <QtGui/QLabel>
#include "GLWidget.hpp"

#include <float.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    GLWidget *glWidget;
    QGridLayout *gridLayout;
    QPushButton *findKNearestButton;
    QCheckBox *checkBoxKdTree;
    QSpacerItem *verticalSpacer;
    QSpinBox *selectKValueBox;
    QDoubleSpinBox *selectRadiusValueBox;
    QPushButton *findInRadiusButton;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(703, 609);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        glWidget = new GLWidget(centralwidget);
        glWidget->setObjectName(QString::fromUtf8("glWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(glWidget->sizePolicy().hasHeightForWidth());
        glWidget->setSizePolicy(sizePolicy);
        glWidget->setAutoFillBackground(false);

        horizontalLayout->addWidget(glWidget);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        findKNearestButton = new QPushButton(centralwidget);
        findKNearestButton->setObjectName(QString::fromUtf8("findKNearestButton"));

        gridLayout->addWidget(findKNearestButton, 4, 1, 1, 2);

        checkBoxKdTree = new QCheckBox(centralwidget);
        checkBoxKdTree->setObjectName(QString::fromUtf8("checkBoxKdTree"));

        gridLayout->addWidget(checkBoxKdTree, 0, 2);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);


        QLabel *selectKValueLabel = new QLabel("KNearest:");
        selectKValueBox = new QSpinBox(centralwidget);
        selectKValueBox->setObjectName(QString::fromUtf8("selectKValueBox"));
        selectKValueBox->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        selectKValueBox->setButtonSymbols(QAbstractSpinBox::PlusMinus);
        selectKValueBox->setMaximum(99999999);

        gridLayout->addWidget(selectKValueLabel, 1, 1);
        gridLayout->addWidget(selectKValueBox, 1, 2);

        QLabel *selectRadiusLabel = new QLabel("Radius:");
        selectRadiusValueBox = new QDoubleSpinBox(centralwidget);
        selectRadiusValueBox->setObjectName(QString::fromUtf8("selectRadiusBox"));
        selectRadiusValueBox->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        selectRadiusValueBox->setButtonSymbols(QAbstractSpinBox::PlusMinus);
        selectRadiusValueBox->setMinimum(0.0);
        selectRadiusValueBox->setMaximum(FLT_MAX);

        gridLayout->addWidget(selectRadiusLabel, 5, 1);
        gridLayout->addWidget(selectRadiusValueBox, 5, 2);

        findInRadiusButton = new QPushButton(centralwidget);
        findInRadiusButton->setObjectName(QString::fromUtf8("findInRadiusButton"));

        gridLayout->addWidget(findInRadiusButton, 6, 1 , 1, 2);

        gridLayout->addItem(verticalSpacer, 7, 2);
        horizontalLayout->addLayout(gridLayout);

        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);
        QObject::connect(checkBoxKdTree, SIGNAL(toggled(bool)), glWidget, SLOT(sigShowKDTree(bool)));
        QObject::connect(findKNearestButton, SIGNAL(clicked()), glWidget, SLOT(sigFindKNearest()));
        QObject::connect(selectKValueBox, SIGNAL(valueChanged(int)), glWidget, SLOT(sigSetKNearest(int)));
        QObject::connect(selectRadiusValueBox, SIGNAL(valueChanged(double)), glWidget, SLOT(sigSetRadius(double)));
        QObject::connect(findInRadiusButton, SIGNAL(clicked()), glWidget, SLOT(sigFindInRadius()));


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "CG2 - \303\234bung 1", 0, QApplication::UnicodeUTF8));
        findKNearestButton->setText(QApplication::translate("MainWindow", "Find K-Nearest Neigbours", 0, QApplication::UnicodeUTF8));
        findInRadiusButton->setText(QApplication::translate("MainWindow", "Find in Radius", 0, QApplication::UnicodeUTF8));
        checkBoxKdTree->setText(QApplication::translate("MainWindow", "kd-tree", 0, QApplication::UnicodeUTF8));
        selectKValueBox->setSpecialValueText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

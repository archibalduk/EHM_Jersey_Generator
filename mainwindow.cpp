#include "mainwindow.h"

// Application headers
#include "dimensions.h"
#include "ui/preview_widget.h"

// Qt headers
#include <QCoreApplication>
#include <QHBoxLayout>

// --- Constructor --- //
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Window
    setWindowTitle(QString("%1 v%2").arg(QCoreApplication::applicationName(),
                                         QCoreApplication::applicationVersion()));
    setMinimumSize(640, 480);

    // Central widget
    auto central_widget{new QWidget(this)};
    setCentralWidget(central_widget);

    // Widget
    preview_widget_ = new PreviewWidget(this);

    // Layout
    auto layout{new QHBoxLayout(central_widget)};
    layout->addWidget(preview_widget_);
}

// --- Destructor --- //
MainWindow::~MainWindow() {}

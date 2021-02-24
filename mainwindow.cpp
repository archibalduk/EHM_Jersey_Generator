#include "mainwindow.h"

// Application headers
#include "dimensions.h"
#include "ui/preview_widget.h"
#include "ui/settings_widget.h"

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
    setMinimumSize(Dimensions::WindowWidth, Dimensions::WindowHeight);

    // Central widget
    auto central_widget{new QWidget(this)};
    setCentralWidget(central_widget);

    // Widgets
    preview_widget_ = new PreviewWidget(this);
    settings_widget_ = new SettingsWidget(this);

    // Layout
    auto layout{new QHBoxLayout(central_widget)};
    layout->addWidget(settings_widget_);
    layout->addWidget(preview_widget_);
}

// --- Destructor --- //
MainWindow::~MainWindow() {}

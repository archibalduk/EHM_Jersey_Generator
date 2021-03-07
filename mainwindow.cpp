#include "mainwindow.h"

// Application headers
#include "common/dimensions.h"
#include "ui/batch_generator_widget.h"
#include "ui/preview_widget.h"
#include "ui/settings_widget.h"

// Qt headers
#include <QCoreApplication>
#include <QHBoxLayout>
#include <QTabWidget>

/* ===================== */
/*      Main Window      */
/* ===================== */

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
    batch_generator_widget_ = new BatchGeneratorWidget(this);
    preview_widget_ = new PreviewWidget(this);
    settings_widget_ = new SettingsWidget(this);

    // Tab widget
    auto tab_widget{new QTabWidget(this)};
    tab_widget->addTab(preview_widget_, tr("Preview"));
    tab_widget->addTab(batch_generator_widget_, tr("Batch"));

    // Layout
    auto layout{new QHBoxLayout(central_widget)};
    layout->addWidget(settings_widget_);
    layout->addWidget(tab_widget);
}

// --- Destructor --- //
MainWindow::~MainWindow() {}

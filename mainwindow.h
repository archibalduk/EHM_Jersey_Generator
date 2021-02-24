#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

// Application headers
class PreviewWidget;
class SettingsWidget;

// Qt headers
#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    PreviewWidget *preview_widget_;
    SettingsWidget *settings_widget_;
};
#endif // MAIN_WINDOW_H

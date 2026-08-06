#include <QApplication>
#include <QMessageBox>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QSettings>
#include <QDateTime>
#include <stdexcept>
#include "MainWindow.hpp"
#include "TrustChainCore.hpp"
#include "TrustChainQt.hpp"

static bool isLoggingEnabled() {
    QString configPath = QCoreApplication::applicationDirPath() + "/config.ini";
    if (!QFile::exists(configPath)) {
        // Create default config.ini with logging enabled
        QSettings settings(configPath, QSettings::IniFormat);
        settings.beginGroup("Logging");
        settings.setValue("EnableLogging", true);
        settings.endGroup();
        return true;
    }
    QSettings settings(configPath, QSettings::IniFormat);
    return settings.value("Logging/EnableLogging", true).toBool();
}

static void logError(const QString& msg) {
    if (!isLoggingEnabled()) return;

    QString logPath = QCoreApplication::applicationDirPath() + "/ROLC_StatusSimulator_error.log";
    QFile file(logPath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << "[" << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "] " << msg << "\n";
    }
}

int main(int argc, char* argv[]) {
    try {
        QApplication app(argc, argv);

        // Ensure Qt finds plugins deployed alongside the executable
        QString appDir = QCoreApplication::applicationDirPath();
        QCoreApplication::addLibraryPath(appDir);
        QCoreApplication::addLibraryPath(appDir + "/platforms");
        QCoreApplication::addLibraryPath(appDir + "/styles");

        // TrustChain Provenance Check & BMM Watermark
        TrustChain::Core guard;
        TrustChain::AuthStatus status = guard.verifyToken();

        rolc::MainWindow mainWindow;
        TrustChain::QtHelper::applyWatermark(&mainWindow, status);

        mainWindow.show();

        return app.exec();
    } catch (const std::exception& e) {
        QString err = QString("Unhandled C++ Exception: ") + e.what();
        logError(err);
        QMessageBox::critical(nullptr, "エラー (ROLC Status Simulator)", err);
        return -1;
    } catch (...) {
        QString err = "不明なエラーが発生したため起動を中止しました。";
        logError(err);
        QMessageBox::critical(nullptr, "エラー (ROLC Status Simulator)", err);
        return -1;
    }
}

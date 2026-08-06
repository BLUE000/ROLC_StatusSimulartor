#include "MainWindow.hpp"
#include "SimulatorPresenter.hpp"
#include "CharacterWidget.hpp"
#include "StatusWidget.hpp"
#include "EquipmentWidget.hpp"
#include "OutputWidget.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>

namespace rolc {

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle("ROLC ステータスシミュレーター (Qt6 C++20)");
    resize(1240, 750);
    setMinimumSize(1150, 680);

    // 1. Create Presenter first
    m_presenter = new SimulatorPresenter(this, this);

    // 2. Create child widgets in proper order
    m_characterWidget = new CharacterWidget(m_presenter, this);
    m_statusWidget = new StatusWidget(m_presenter, this);
    m_equipmentWidget = new EquipmentWidget(m_presenter, this);
    m_outputWidget = new OutputWidget(this);

    // 3. Setup Layouts
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);

    QHBoxLayout* topLayout = new QHBoxLayout();
    topLayout->addWidget(m_characterWidget, 5);
    topLayout->addWidget(m_statusWidget, 4);

    QHBoxLayout* bottomLayout = new QHBoxLayout();
    bottomLayout->addWidget(m_equipmentWidget, 5);
    bottomLayout->addWidget(m_outputWidget, 4);

    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(bottomLayout);

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidget(centralWidget);
    scrollArea->setWidgetResizable(true);
    setCentralWidget(scrollArea);

    // 4. Initial calculation now that all views are initialized
    m_presenter->recalculate();
}

MainWindow::~MainWindow() = default;

void MainWindow::updateDerivedStats(const DerivedStatsResult& result) {
    if (m_statusWidget) {
        m_statusWidget->updateStats(result);
    }
    if (m_outputWidget) {
        m_outputWidget->updateOutput(result);
    }
}

} // namespace rolc

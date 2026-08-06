#ifndef ROLC_MAIN_WINDOW_HPP
#define ROLC_MAIN_WINDOW_HPP

#include <QMainWindow>
#include "ISimulatorView.hpp"

namespace rolc {

class SimulatorPresenter;
class CharacterWidget;
class StatusWidget;
class EquipmentWidget;
class OutputWidget;

class MainWindow : public QMainWindow, public ISimulatorView {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

    // ISimulatorView interface
    void updateDerivedStats(const DerivedStatsResult& result) override;

    SimulatorPresenter* getPresenter() const { return m_presenter; }

private:
    SimulatorPresenter* m_presenter{nullptr};

    CharacterWidget* m_characterWidget{nullptr};
    StatusWidget* m_statusWidget{nullptr};
    EquipmentWidget* m_equipmentWidget{nullptr};
    OutputWidget* m_outputWidget{nullptr};
};

} // namespace rolc

#endif // ROLC_MAIN_WINDOW_HPP

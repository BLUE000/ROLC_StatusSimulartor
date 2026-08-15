#ifndef ROLC_STATUS_WIDGET_HPP
#define ROLC_STATUS_WIDGET_HPP

#include <QWidget>
#include <QSlider>
#include <QSpinBox>
#include <QLabel>
#include <QCheckBox>
#include <array>
#include "Types.hpp"

namespace rolc {

class SimulatorPresenter;

class StatusWidget : public QWidget {
    Q_OBJECT
public:
    explicit StatusWidget(SimulatorPresenter* presenter, QWidget* parent = nullptr);

    void updateStats(const DerivedStatsResult& result);

private:
    SimulatorPresenter* m_presenter;

    QCheckBox* m_nonShareCheck;
    QLabel* m_remainingPointLabel;

    std::array<QSlider*, 6> m_sliders;
    std::array<QSpinBox*, 6> m_spinBoxes;
    std::array<QLabel*, 6> m_statValLabels;
    std::array<QLabel*, 6> m_bonusPercentLabels;
};

} // namespace rolc

#endif // ROLC_STATUS_WIDGET_HPP

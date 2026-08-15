#ifndef ROLC_EQUIPMENT_WIDGET_HPP
#define ROLC_EQUIPMENT_WIDGET_HPP

#include <QWidget>
#include <QComboBox>
#include <QCheckBox>
#include "Types.hpp"

namespace rolc {

class SimulatorPresenter;

class EquipmentWidget : public QWidget {
    Q_OBJECT
public:
    explicit EquipmentWidget(SimulatorPresenter* presenter, QWidget* parent = nullptr);

private:
    SimulatorPresenter* m_presenter;

    QComboBox* m_rightHandTitleCombo;
    QComboBox* m_leftHandTitleCombo;
    QComboBox* m_bodyTitleCombo;
    QComboBox* m_handTitleCombo;
    QComboBox* m_legTitleCombo;

    QComboBox* m_amplifierCombo;

    QCheckBox* m_desperateAttackCheck;
    QCheckBox* m_desperateMagicCheck;
    QCheckBox* m_desperateAssaultCheck;
};

} // namespace rolc

#endif // ROLC_EQUIPMENT_WIDGET_HPP

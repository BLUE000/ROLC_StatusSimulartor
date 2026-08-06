#ifndef ROLC_CHARACTER_WIDGET_HPP
#define ROLC_CHARACTER_WIDGET_HPP

#include <QWidget>
#include <QComboBox>
#include <QSpinBox>
#include <QLabel>
#include <QRadioButton>
#include <QButtonGroup>
#include <QCheckBox>
#include "Types.hpp"

namespace rolc {

class SimulatorPresenter;

class CharacterWidget : public QWidget {
    Q_OBJECT
public:
    explicit CharacterWidget(SimulatorPresenter* presenter, QWidget* parent = nullptr);

private slots:
    void populateCharacters();
    void populateClasses();
    void updateDerivedClassCombos();
    void updateLevelLimits();

private:
    SimulatorPresenter* m_presenter;

    QComboBox* m_editionCombo;
    QComboBox* m_shareCombo;
    QLabel* m_levelLimitNoticeLabel;
    QSpinBox* m_restrictedLevelSpin;
    QCheckBox* m_restrictedAutoCheck;
    QComboBox* m_characterCombo;
    QSpinBox* m_moralSpin;
    QSpinBox* m_equipAtkSpin;
    QSpinBox* m_equipMatkSpin;
    QSpinBox* m_equipCritSpin;

    QComboBox* m_firstClassCombo;
    QComboBox* m_secondClassCombo;
    QComboBox* m_thirdClassCombo;
    QComboBox* m_fourthClassCombo;
    QComboBox* m_exClassCombo;

    QSpinBox* m_firstClassLevelSpin;
    QSpinBox* m_secondClassLevelSpin;
    QSpinBox* m_thirdClassLevelSpin;
    QSpinBox* m_fourthClassLevelSpin;
    QSpinBox* m_exClassLevelSpin;

    QButtonGroup* m_currentClassGroup;
    QRadioButton* m_radioFirst;
    QRadioButton* m_radioSecond;
    QRadioButton* m_radioThird;
    QRadioButton* m_radioFourth;
    QRadioButton* m_radioEx;
};

} // namespace rolc

#endif // ROLC_CHARACTER_WIDGET_HPP

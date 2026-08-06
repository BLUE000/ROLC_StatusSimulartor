#ifndef ROLC_SIMULATOR_PRESENTER_HPP
#define ROLC_SIMULATOR_PRESENTER_HPP

#include <QObject>
#include "ISimulatorView.hpp"
#include "Types.hpp"
#include "CalculatorEngine.hpp"

namespace rolc {

class SimulatorPresenter : public QObject {
    Q_OBJECT
public:
    explicit SimulatorPresenter(ISimulatorView* view, QObject* parent = nullptr);

    const UserBuildState& getState() const { return m_state; }
    DerivedStatsResult getDerivedStats() const { return m_lastResult; }

public slots:
    // Slot handlers for user interface events (Testable via GTest / QTest)
    void onShareCategorySelected(int categoryIndex);
    void onCharacterSelected(int characterId);
    void onLevelChanged(int level);
    void onMoralChanged(int moral);
    void onEquipAtkChanged(int equipAtk);
    void onEquipMatkChanged(int equipMatk);
    void onEquipCritChanged(int equipCrit);
    void onNonShareToggled(bool enabled);

    void onClassSelected(ClassStage stage, int classId);
    void onCurrentClassStageChanged(ClassStage stage);
    void onHistoryClassLevelChanged(ClassStage stage, int level);

    void onMakingPointChanged(StatType stat, int pointValue);

    void onRightHandTitleChanged(int titleId);
    void onLeftHandTitleChanged(int titleId);
    void onBodyTitleChanged(int titleId);
    void onHandTitleChanged(int titleId);
    void onLegTitleChanged(int titleId);

    void onAmplifierChanged(int ampId);

    void onDesperateAttackToggled(bool enabled);
    void onDesperateMagicToggled(bool enabled);
    void onDesperateAssaultToggled(bool enabled);

    void recalculate();

private:
    ISimulatorView* m_view;
    UserBuildState m_state;
    DerivedStatsResult m_lastResult;
};

} // namespace rolc

#endif // ROLC_SIMULATOR_PRESENTER_HPP

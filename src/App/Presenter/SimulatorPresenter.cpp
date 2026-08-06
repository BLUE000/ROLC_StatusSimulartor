#include "SimulatorPresenter.hpp"

namespace rolc {

SimulatorPresenter::SimulatorPresenter(ISimulatorView* view, QObject* parent)
    : QObject(parent)
    , m_view(view)
{
    // Initialize default state with 1st class level (100) as current level
    m_state.level = m_state.historyClassLevels[0];
}

void SimulatorPresenter::onEditionSelected(Edition edition) {
    m_state.edition = edition;
    recalculate();
}

void SimulatorPresenter::onShareCategorySelected(int categoryIndex) {
    m_state.shareCategoryId = categoryIndex;
    recalculate();
}

void SimulatorPresenter::onCharacterSelected(int characterId) {
    m_state.characterId = characterId;
    recalculate();
}

void SimulatorPresenter::onLevelChanged(int level) {
    m_state.level = level;
    recalculate();
}

void SimulatorPresenter::onMoralChanged(int moral) {
    m_state.moral = moral;
    recalculate();
}

void SimulatorPresenter::onEquipAtkChanged(int equipAtk) {
    m_state.equipAtk = equipAtk;
    recalculate();
}

void SimulatorPresenter::onEquipMatkChanged(int equipMatk) {
    m_state.equipMatk = equipMatk;
    recalculate();
}

void SimulatorPresenter::onEquipCritChanged(int equipCrit) {
    m_state.equipCrit = equipCrit;
    recalculate();
}

void SimulatorPresenter::onNonShareToggled(bool enabled) {
    m_state.isNonShare = enabled;
    recalculate();
}

void SimulatorPresenter::onClassSelected(ClassStage stage, int classId) {
    switch (stage) {
        case ClassStage::First: m_state.firstClassId = classId; break;
        case ClassStage::Second: m_state.secondClassId = classId; break;
        case ClassStage::Third: m_state.thirdClassId = classId; break;
        case ClassStage::Fourth: m_state.fourthClassId = classId; break;
        case ClassStage::Ex: m_state.exClassId = classId; break;
    }
    recalculate();
}

void SimulatorPresenter::onCurrentClassStageChanged(ClassStage stage) {
    m_state.currentClassStage = stage;
    int idx = static_cast<int>(stage) - 1;
    if (idx >= 0 && idx < 5) {
        m_state.level = m_state.historyClassLevels[idx];
    }
    recalculate();
}

void SimulatorPresenter::onHistoryClassLevelChanged(ClassStage stage, int level) {
    int idx = static_cast<int>(stage) - 1;
    if (idx >= 0 && idx < 5) {
        m_state.historyClassLevels[idx] = level;
        if (stage == m_state.currentClassStage) {
            m_state.level = level;
        }
        recalculate();
    }
}

void SimulatorPresenter::onMakingPointChanged(StatType stat, int pointValue) {
    int idx = static_cast<int>(stat);
    if (idx >= 0 && idx < 6) {
        m_state.makingPoints[idx] = pointValue;
        recalculate();
    }
}

void SimulatorPresenter::onRightHandTitleChanged(int titleId) {
    m_state.rightHandTitleId = titleId;
    recalculate();
}

void SimulatorPresenter::onLeftHandTitleChanged(int titleId) {
    m_state.leftHandTitleId = titleId;
    recalculate();
}

void SimulatorPresenter::onBodyTitleChanged(int titleId) {
    m_state.bodyTitleId = titleId;
    recalculate();
}

void SimulatorPresenter::onHandTitleChanged(int titleId) {
    m_state.handTitleId = titleId;
    recalculate();
}

void SimulatorPresenter::onLegTitleChanged(int titleId) {
    m_state.legTitleId = titleId;
    recalculate();
}

void SimulatorPresenter::onAmplifierChanged(int ampId) {
    m_state.amplifierId = ampId;
    recalculate();
}

void SimulatorPresenter::onDesperateAttackToggled(bool enabled) {
    m_state.desperateAttack = enabled;
    recalculate();
}

void SimulatorPresenter::onDesperateMagicToggled(bool enabled) {
    m_state.desperateMagic = enabled;
    recalculate();
}

void SimulatorPresenter::onDesperateAssaultToggled(bool enabled) {
    m_state.desperateAssault = enabled;
    recalculate();
}

void SimulatorPresenter::recalculate() {
    m_lastResult = CalculatorEngine::calculate(m_state);
    if (m_view) {
        m_view->updateDerivedStats(m_lastResult);
    }
}

} // namespace rolc

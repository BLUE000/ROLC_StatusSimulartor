#include "EquipmentWidget.hpp"
#include "SimulatorPresenter.hpp"
#include "MasterData.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>

namespace rolc {

EquipmentWidget::EquipmentWidget(SimulatorPresenter* presenter, QWidget* parent)
    : QWidget(parent)
    , m_presenter(presenter)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // Section 1: Equipment Titles
    QGroupBox* equipGroup = new QGroupBox("装備称号", this);
    QGridLayout* grid = new QGridLayout(equipGroup);

    grid->addWidget(new QLabel("右手称号:"), 0, 0);
    m_rightHandTitleCombo = new QComboBox(this);
    grid->addWidget(m_rightHandTitleCombo, 0, 1);

    grid->addWidget(new QLabel("左手称号:"), 1, 0);
    m_leftHandTitleCombo = new QComboBox(this);
    grid->addWidget(m_leftHandTitleCombo, 1, 1);

    grid->addWidget(new QLabel("胴称号:"), 2, 0);
    m_bodyTitleCombo = new QComboBox(this);
    grid->addWidget(m_bodyTitleCombo, 2, 1);

    grid->addWidget(new QLabel("手称号:"), 3, 0);
    m_handTitleCombo = new QComboBox(this);
    grid->addWidget(m_handTitleCombo, 3, 1);

    grid->addWidget(new QLabel("足称号:"), 4, 0);
    m_legTitleCombo = new QComboBox(this);
    grid->addWidget(m_legTitleCombo, 4, 1);

    // Populate Title Combos
    auto populateTitles = [](QComboBox* combo) {
        combo->clear();
        for (const auto& t : MasterData::getTitleBonuses()) {
            combo->addItem(QString::fromStdString(t.name), t.id);
        }
    };
    populateTitles(m_rightHandTitleCombo);
    populateTitles(m_leftHandTitleCombo);
    populateTitles(m_bodyTitleCombo);
    populateTitles(m_handTitleCombo);
    populateTitles(m_legTitleCombo);

    mainLayout->addWidget(equipGroup);

    // Section 2: Skill Buffs & Special Settings (Compact 2-row layout)
    QGroupBox* buffGroup = new QGroupBox("スキルバフ・特殊設定", this);
    QVBoxLayout* buffMainLayout = new QVBoxLayout(buffGroup);

    QHBoxLayout* ampRow = new QHBoxLayout();
    ampRow->addWidget(new QLabel("アンプ:"));
    m_amplifierCombo = new QComboBox(this);
    m_amplifierCombo->clear();
    for (const auto& a : MasterData::getAmplifierBonuses()) {
        m_amplifierCombo->addItem(QString::fromStdString(a.name), a.id);
    }
    ampRow->addWidget(m_amplifierCombo);
    ampRow->addStretch();
    buffMainLayout->addLayout(ampRow);

    QHBoxLayout* checkRow = new QHBoxLayout();
    m_desperateAttackCheck = new QCheckBox("デスペレイトアタック", this);
    m_desperateMagicCheck = new QCheckBox("デスペレイトマジック", this);
    m_desperateAssaultCheck = new QCheckBox("デスペレイトアサルト", this);

    checkRow->addWidget(m_desperateAttackCheck);
    checkRow->addWidget(m_desperateMagicCheck);
    checkRow->addWidget(m_desperateAssaultCheck);
    checkRow->addStretch();
    buffMainLayout->addLayout(checkRow);

    mainLayout->addWidget(buffGroup);

    // Connect signals to Presenter slots
    connect(m_rightHandTitleCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int idx) {
        m_presenter->onRightHandTitleChanged(m_rightHandTitleCombo->itemData(idx).toInt());
    });
    connect(m_leftHandTitleCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int idx) {
        m_presenter->onLeftHandTitleChanged(m_leftHandTitleCombo->itemData(idx).toInt());
    });
    connect(m_bodyTitleCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int idx) {
        m_presenter->onBodyTitleChanged(m_bodyTitleCombo->itemData(idx).toInt());
    });
    connect(m_handTitleCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int idx) {
        m_presenter->onHandTitleChanged(m_handTitleCombo->itemData(idx).toInt());
    });
    connect(m_legTitleCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int idx) {
        m_presenter->onLegTitleChanged(m_legTitleCombo->itemData(idx).toInt());
    });
    connect(m_amplifierCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int idx) {
        m_presenter->onAmplifierChanged(m_amplifierCombo->itemData(idx).toInt());
    });

    connect(m_desperateAttackCheck, &QCheckBox::toggled, m_presenter, &SimulatorPresenter::onDesperateAttackToggled);
    connect(m_desperateMagicCheck, &QCheckBox::toggled, m_presenter, &SimulatorPresenter::onDesperateMagicToggled);
    connect(m_desperateAssaultCheck, &QCheckBox::toggled, m_presenter, &SimulatorPresenter::onDesperateAssaultToggled);
}

} // namespace rolc

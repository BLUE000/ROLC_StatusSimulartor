#include "OutputWidget.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>

namespace rolc {

OutputWidget::OutputWidget(QWidget* parent)
    : QWidget(parent)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // Section 1: Combat ATK Table
    QGroupBox* atkGroup = new QGroupBox("攻撃力（会心率）・期待値", this);
    QGridLayout* atkGrid = new QGridLayout(atkGroup);

    atkGrid->addWidget(new QLabel("種別", this), 0, 0);
    atkGrid->addWidget(new QLabel("攻撃力 (Min - Max)", this), 0, 1);
    atkGrid->addWidget(new QLabel("会心率 (%)", this), 0, 2);
    atkGrid->addWidget(new QLabel("期待値", this), 0, 3);

    // Physical
    atkGrid->addWidget(new QLabel("物理", this), 1, 0);
    m_physAtkLabel = new QLabel("0 - 0", this);
    m_physCritLabel = new QLabel("0 %", this);
    m_physExpLabel = new QLabel("0", this);
    m_physExpLabel->setStyleSheet("font-weight: bold; color: #2e7d32;");

    atkGrid->addWidget(m_physAtkLabel, 1, 1);
    atkGrid->addWidget(m_physCritLabel, 1, 2);
    atkGrid->addWidget(m_physExpLabel, 1, 3);

    // Magical
    atkGrid->addWidget(new QLabel("魔法", this), 2, 0);
    m_magMatkLabel = new QLabel("0 - 0", this);
    m_magCritLabel = new QLabel("0 %", this);
    m_magExpLabel = new QLabel("0", this);
    m_magExpLabel->setStyleSheet("font-weight: bold; color: #2e7d32;");

    atkGrid->addWidget(m_magMatkLabel, 2, 1);
    atkGrid->addWidget(m_magCritLabel, 2, 2);
    atkGrid->addWidget(m_magExpLabel, 2, 3);

    // Combined
    atkGrid->addWidget(new QLabel("物魔", this), 3, 0);
    m_combinedAtkLabel = new QLabel("0 - 0", this);
    m_combinedCritLabel = new QLabel("0 %", this);
    m_combinedExpLabel = new QLabel("0", this);
    m_combinedExpLabel->setStyleSheet("font-weight: bold; color: #d84315;");

    atkGrid->addWidget(m_combinedAtkLabel, 3, 1);
    atkGrid->addWidget(m_combinedCritLabel, 3, 2);
    atkGrid->addWidget(m_combinedExpLabel, 3, 3);

    mainLayout->addWidget(atkGroup);

    // Section 2: HP, MP, Charge
    QGroupBox* statsGroup = new QGroupBox("HP / MP / チャージ時間", this);
    QGridLayout* statsGrid = new QGridLayout(statsGroup);

    statsGrid->addWidget(new QLabel("HP:", this), 0, 0);
    m_hpLabel = new QLabel("0", this);
    m_hpLabel->setStyleSheet("font-weight: bold; font-size: 13px;");
    statsGrid->addWidget(m_hpLabel, 0, 1);

    statsGrid->addWidget(new QLabel("MP:", this), 0, 2);
    m_mpLabel = new QLabel("0", this);
    m_mpLabel->setStyleSheet("font-weight: bold; font-size: 13px;");
    statsGrid->addWidget(m_mpLabel, 0, 3);

    statsGrid->addWidget(new QLabel("右手チャージ:", this), 1, 0);
    m_rightChargeLabel = new QLabel("0f", this);
    statsGrid->addWidget(m_rightChargeLabel, 1, 1);

    statsGrid->addWidget(new QLabel("左手チャージ:", this), 1, 2);
    m_leftChargeLabel = new QLabel("0f", this);
    statsGrid->addWidget(m_leftChargeLabel, 1, 3);

    mainLayout->addWidget(statsGroup);
}

void OutputWidget::updateOutput(const DerivedStatsResult& res) {
    m_physAtkLabel->setText(QString("%1 - %2").arg(res.minAtk).arg(res.maxAtk));
    m_physCritLabel->setText(QString("%1 %").arg(res.atkCriticalRate, 0, 'f', 1));
    m_physExpLabel->setText(QString::number(res.atkExpectation, 'f', 1));

    m_magMatkLabel->setText(QString("%1 - %2").arg(res.minMatk).arg(res.maxMatk));
    m_magCritLabel->setText(QString("%1 %").arg(res.matkCriticalRate, 0, 'f', 1));
    m_magExpLabel->setText(QString::number(res.matkExpectation, 'f', 1));

    m_combinedAtkLabel->setText(QString("%1 - %2").arg(res.minAtkMatk).arg(res.maxAtkMatk));
    m_combinedCritLabel->setText(QString("%1 %").arg(res.atkMatkCriticalRate, 0, 'f', 1));
    m_combinedExpLabel->setText(QString::number(res.atkMatkExpectation, 'f', 1));

    if (res.hp == res.maxTheoreticalHp) {
        m_hpLabel->setText(QString::number(res.hp));
    } else {
        m_hpLabel->setText(QString("%1 [最大: %2]").arg(res.hp).arg(res.maxTheoreticalHp));
    }

    if (res.mp == res.maxTheoreticalMp) {
        m_mpLabel->setText(QString::number(res.mp));
    } else {
        m_mpLabel->setText(QString("%1 [最大: %2]").arg(res.mp).arg(res.maxTheoreticalMp));
    }

    m_rightChargeLabel->setText(QString("%1f").arg(res.rightChargeFrames));
    m_leftChargeLabel->setText(QString("%1f").arg(res.leftChargeFrames));
}

} // namespace rolc

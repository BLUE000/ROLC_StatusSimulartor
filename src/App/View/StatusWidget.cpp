#include "StatusWidget.hpp"
#include "SimulatorPresenter.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>

namespace rolc {

StatusWidget::StatusWidget(SimulatorPresenter* presenter, QWidget* parent)
    : QWidget(parent)
    , m_presenter(presenter)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QGroupBox* statusGroup = new QGroupBox("基本ステータス振り分け & 計算結果", this);
    QGridLayout* grid = new QGridLayout(statusGroup);

    QHBoxLayout* topRow = new QHBoxLayout();
    m_nonShareCheck = new QCheckBox("ノンシェア", this);
    topRow->addWidget(m_nonShareCheck);
    topRow->addSpacing(15);

    m_remainingPointLabel = new QLabel("残りポイント: 150 / 150", this);
    m_remainingPointLabel->setStyleSheet("font-weight: bold; font-size: 14px; color: #1e88e5;");
    topRow->addWidget(m_remainingPointLabel);
    topRow->addStretch();

    grid->addLayout(topRow, 0, 0, 1, 6);

    connect(m_nonShareCheck, &QCheckBox::toggled, m_presenter, &SimulatorPresenter::onNonShareToggled);

    const char* statNames[6] = {"STR", "DEX", "VIT", "INT", "CON", "MEN"};

    for (int i = 0; i < 6; ++i) {
        int row = i + 1;
        grid->addWidget(new QLabel(statNames[i], this), row, 0);

        m_statValLabels[i] = new QLabel("0", this);
        m_statValLabels[i]->setStyleSheet("font-weight: bold; font-size: 13px;");
        grid->addWidget(m_statValLabels[i], row, 1);

        m_bonusPercentLabels[i] = new QLabel("( +0% )", this);
        grid->addWidget(m_bonusPercentLabels[i], row, 2);

        m_sliders[i] = new QSlider(Qt::Horizontal, this);
        m_sliders[i]->setRange(0, 20);
        grid->addWidget(m_sliders[i], row, 3);

        m_spinBoxes[i] = new QSpinBox(this);
        m_spinBoxes[i]->setRange(0, 20);
        grid->addWidget(m_spinBoxes[i], row, 4);

        // Sync slider and spinbox
        connect(m_sliders[i], &QSlider::valueChanged, m_spinBoxes[i], &QSpinBox::setValue);
        connect(m_spinBoxes[i], QOverload<int>::of(&QSpinBox::valueChanged), m_sliders[i], &QSlider::setValue);

        StatType st = static_cast<StatType>(i);
        connect(m_spinBoxes[i], QOverload<int>::of(&QSpinBox::valueChanged), [this, st](int val) {
            m_presenter->onMakingPointChanged(st, val);
        });
    }

    mainLayout->addWidget(statusGroup);
}

void StatusWidget::updateStats(const DerivedStatsResult& result) {
    m_remainingPointLabel->setText(QString("残りポイント: %1 / %2")
        .arg(result.remainingMakingPoints)
        .arg(result.maxMakingPoints));

    if (result.remainingMakingPoints < 0) {
        m_remainingPointLabel->setStyleSheet("font-weight: bold; font-size: 14px; color: red;");
    } else {
        m_remainingPointLabel->setStyleSheet("font-weight: bold; font-size: 14px; color: #1e88e5;");
    }

    for (int i = 0; i < 6; ++i) {
        int curStat = result.finalStats[i];
        int baseStat = result.baseStats[i];
        int bonus = result.statPercentBonuses[i];

        m_statValLabels[i]->setText(QString::number(curStat));
        if (curStat > 0) {
            m_statValLabels[i]->setStyleSheet("font-weight: bold; font-size: 13px; color: #2e7d32;");
        } else if (curStat < 0) {
            m_statValLabels[i]->setStyleSheet("font-weight: bold; font-size: 13px; color: #c62828;");
        } else {
            m_statValLabels[i]->setStyleSheet("font-weight: bold; font-size: 13px; color: black;");
        }

        QString sign = (bonus >= 0) ? "+" : "-";
        int absBonus = std::abs(bonus);

        m_bonusPercentLabels[i]->setText(QString("(  %1  %2  %3 % )")
            .arg(baseStat)
            .arg(sign)
            .arg(absBonus));
    }
}

} // namespace rolc

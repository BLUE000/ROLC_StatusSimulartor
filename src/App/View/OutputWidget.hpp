#ifndef ROLC_OUTPUT_WIDGET_HPP
#define ROLC_OUTPUT_WIDGET_HPP

#include <QWidget>
#include <QLabel>
#include "Types.hpp"

namespace rolc {

class OutputWidget : public QWidget {
    Q_OBJECT
public:
    explicit OutputWidget(QWidget* parent = nullptr);

    void updateOutput(const DerivedStatsResult& result);

private:
    QLabel* m_physAtkLabel;
    QLabel* m_physCritLabel;
    QLabel* m_physExpLabel;

    QLabel* m_magMatkLabel;
    QLabel* m_magCritLabel;
    QLabel* m_magExpLabel;

    QLabel* m_combinedAtkLabel;
    QLabel* m_combinedCritLabel;
    QLabel* m_combinedExpLabel;

    QLabel* m_hpLabel;
    QLabel* m_mpLabel;
    QLabel* m_rightChargeLabel;
    QLabel* m_leftChargeLabel;
};

} // namespace rolc

#endif // ROLC_OUTPUT_WIDGET_HPP

#ifndef ROLC_CALCULATOR_ENGINE_HPP
#define ROLC_CALCULATOR_ENGINE_HPP

#include "Types.hpp"

namespace rolc {

class CalculatorEngine {
public:
    // Calculates derived stats using optional custom formula config
    static DerivedStatsResult calculate(const UserBuildState& state,
                                        const FormulaConfig& config = FormulaConfig::defaultConfig());

    static int calculateMakingPointCost(int pt);
    static int calculateMaxMakingPoints(const UserBuildState& state,
                                        const FormulaConfig& config = FormulaConfig::defaultConfig());
};

} // namespace rolc

#endif // ROLC_CALCULATOR_ENGINE_HPP

#ifndef ROLC_ISIMULATOR_VIEW_HPP
#define ROLC_ISIMULATOR_VIEW_HPP

#include "Types.hpp"

namespace rolc {

class ISimulatorView {
public:
    virtual ~ISimulatorView() = default;
    virtual void updateDerivedStats(const DerivedStatsResult& result) = 0;
};

} // namespace rolc

#endif // ROLC_ISIMULATOR_VIEW_HPP

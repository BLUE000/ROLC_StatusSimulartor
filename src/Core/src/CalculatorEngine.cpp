#include "CalculatorEngine.hpp"
#include "MasterData.hpp"
#include <cmath>
#include <algorithm>

namespace rolc {

FormulaConfig FormulaConfig::defaultConfig() {
    FormulaConfig cfg;

    cfg.baseMakingPoints = 100;
    cfg.classUnlockMakingPointBonus = 10;

    cfg.hpFormula = [](int lvl, int vit) {
        int lvlDiv10 = lvl / 10;
        double hpPart1 = (lvl + 20) * ((lvl + 20) * (vit * 3 + lvlDiv10 + 40) + 5000) / 5000.0;
        int hpPart2 = (lvlDiv10 + 2) * (lvlDiv10 + 2);
        int val = static_cast<int>(std::floor(hpPart1)) - hpPart2;
        return std::max(0, val);
    };

    cfg.mpFormula = [](int lvl, int intStat) {
        int lvlDiv10 = lvl / 10;
        double mpPart1 = (lvl + 20) * ((lvl + 20) * (intStat * 3 + lvlDiv10 + 40) + 5000) / 5000.0;
        int mpPart2 = (lvlDiv10 + 2) * (lvlDiv10 + 2);
        int val = static_cast<int>(std::floor(mpPart1)) - mpPart2;
        return std::max(0, val);
    };

    cfg.maxAtkFormula = [](int lvl, int stat) {
        double rawMax = (lvl + 20) * ((lvl + 20) * (stat * 3 + lvl / 2.0 + 40) + 10000.0) / 10000.0;
        return static_cast<int>(std::floor(rawMax) * 10);
    };

    return cfg;
}

int CalculatorEngine::calculateMakingPointCost(int pt) {
    if (pt <= 0) return 0;
    if (pt > 20) pt = 20;

    if (pt <= 5) {
        return pt;
    } else if (pt <= 10) {
        return pt + (pt - 5);
    } else if (pt <= 15) {
        return pt + (pt - 5) + (pt - 10);
    } else {
        return pt + (pt - 5) + (pt - 10) + (pt - 15);
    }
}

int CalculatorEngine::calculateMaxMakingPoints(const UserBuildState& state, const FormulaConfig& config) {
    int maxPts = config.baseMakingPoints;
    if (state.secondClassId != 0) maxPts += config.classUnlockMakingPointBonus;
    if (state.thirdClassId != 0) maxPts += config.classUnlockMakingPointBonus;
    if (state.fourthClassId != 0) maxPts += config.classUnlockMakingPointBonus;
    if (state.exClassId != 0) maxPts += config.classUnlockMakingPointBonus;
    return maxPts;
}

DerivedStatsResult CalculatorEngine::calculate(const UserBuildState& state, const FormulaConfig& config) {
    DerivedStatsResult res;

    // 1. Calculate Making Points
    res.maxMakingPoints = calculateMaxMakingPoints(state, config);
    int totalCost = 0;
    for (int i = 0; i < 6; ++i) {
        totalCost += calculateMakingPointCost(state.makingPoints[i]);
    }
    res.remainingMakingPoints = res.maxMakingPoints - totalCost;

    // 2. Fetch Character Base Stats
    const CharacterData* charData = MasterData::getCharacterById(state.characterId);
    std::array<int, 6> charBase = {0, 0, 0, 0, 0, 0};
    if (charData) {
        charBase = charData->baseStats;
    }

    // Add Making Points to character base
    std::array<int, 6> allocatedStats;
    for (int i = 0; i < 6; ++i) {
        allocatedStats[i] = charBase[i] + state.makingPoints[i];
    }

    // 3. Aggregate Percent Bonuses & Flat Bonuses
    std::array<int, 6> percentBonuses = {0, 0, 0, 0, 0, 0};
    std::array<int, 6> maxTheoreticalPercentBonuses = {0, 0, 0, 0, 0, 0};
    std::array<int, 6> flatBonuses = {0, 0, 0, 0, 0, 0};

    // Active Class Stage Bonus
    int activeClassId = 0;
    switch (state.currentClassStage) {
        case ClassStage::First: activeClassId = state.firstClassId; break;
        case ClassStage::Second: activeClassId = state.secondClassId; break;
        case ClassStage::Third: activeClassId = state.thirdClassId; break;
        case ClassStage::Fourth: activeClassId = state.fourthClassId; break;
        case ClassStage::Ex: activeClassId = state.exClassId; break;
    }
    const ClassData* activeClass = MasterData::getClassById(state.currentClassStage, activeClassId);
    if (activeClass) {
        for (int i = 0; i < 6; ++i) {
            percentBonuses[i] += activeClass->statBonuses[i];
            maxTheoreticalPercentBonuses[i] += activeClass->statBonuses[i];
        }
    }

    // Title Bonuses
    const int titleIds[5] = {
        state.rightHandTitleId, state.leftHandTitleId,
        state.bodyTitleId, state.handTitleId, state.legTitleId
    };
    for (int tid : titleIds) {
        const TitleBonus* tb = MasterData::getTitleBonusById(tid);
        if (tb) {
            for (int i = 0; i < 6; ++i) {
                percentBonuses[i] += tb->percentBonuses[i];
                maxTheoreticalPercentBonuses[i] += tb->percentBonuses[i];
            }
        }
    }

    // Amplifier Bonus
    const AmplifierBonus* amp = MasterData::getAmplifierBonusById(state.amplifierId);
    if (amp) {
        for (int i = 0; i < 6; ++i) {
            flatBonuses[i] += amp->flatBonuses[i];
        }
    }

    // Skill Buffs
    if (state.desperateAttack) {
        percentBonuses[static_cast<int>(StatType::STR)] += 15;
        maxTheoreticalPercentBonuses[static_cast<int>(StatType::STR)] += 15;
    }
    if (state.desperateMagic) {
        percentBonuses[static_cast<int>(StatType::INT)] += 15;
        maxTheoreticalPercentBonuses[static_cast<int>(StatType::INT)] += 15;
    }
    if (state.desperateAssault) {
        percentBonuses[static_cast<int>(StatType::STR)] += 15;
        percentBonuses[static_cast<int>(StatType::INT)] += 15;
        maxTheoreticalPercentBonuses[static_cast<int>(StatType::STR)] += 15;
        maxTheoreticalPercentBonuses[static_cast<int>(StatType::INT)] += 15;
    }

    // History Class Level Scaling Bonuses
    int currentStageIdx = static_cast<int>(state.currentClassStage) - 1;
    int currentClassLvl = state.historyClassLevels[currentStageIdx];
    if (currentClassLvl <= 0) currentClassLvl = 1;

    int effectiveLvl = (state.restrictedLevel > 0) ? std::min(currentClassLvl, state.restrictedLevel) : currentClassLvl;

    const int classIds[5] = {
        state.firstClassId, state.secondClassId, state.thirdClassId,
        state.fourthClassId, state.exClassId
    };
    const ClassStage stages[5] = {
        ClassStage::First, ClassStage::Second, ClassStage::Third,
        ClassStage::Fourth, ClassStage::Ex
    };

    for (int s = 0; s < 5; ++s) {
        if (s == currentStageIdx) continue;
        if (classIds[s] == 0) continue;

        const ClassData* histClass = MasterData::getClassById(stages[s], classIds[s]);
        if (histClass) {
            int histLvl = state.historyClassLevels[s];
            if (histLvl < 0) histLvl = 0;
            int effectiveHistLvl = std::min(histLvl, effectiveLvl);

            double ratio = (static_cast<double>(effectiveHistLvl) / effectiveLvl) / 2.0;
            double maxRatio = (100.0 / effectiveLvl) / 2.0;

            for (int i = 0; i < 6; ++i) {
                double baseHist = 10.0 * ratio;
                double classHist = (histClass->statBonuses[i] / 2.0) * ratio;

                double maxBaseHist = 10.0 * maxRatio;
                double maxClassHist = (histClass->statBonuses[i] / 2.0) * maxRatio;

                percentBonuses[i] += static_cast<int>(std::round(baseHist + classHist));
                maxTheoreticalPercentBonuses[i] += static_cast<int>(std::round(maxBaseHist + maxClassHist));
            }
        }
    }

    res.statPercentBonuses = percentBonuses;
    res.maxTheoreticalPercentBonuses = maxTheoreticalPercentBonuses;

    // Calculate Final 6 Base Stats & Theoretical Max Stats
    for (int i = 0; i < 6; ++i) {
        double bonusMult = 1.0 + (percentBonuses[i] / 100.0);
        res.finalStats[i] = static_cast<int>(std::floor(allocatedStats[i] * bonusMult)) + flatBonuses[i];
        if (res.finalStats[i] < 0) res.finalStats[i] = 0;

        double maxBonusMult = 1.0 + (maxTheoreticalPercentBonuses[i] / 100.0);
        res.maxTheoreticalStats[i] = static_cast<int>(std::floor(allocatedStats[i] * maxBonusMult)) + flatBonuses[i];
        if (res.maxTheoreticalStats[i] < 0) res.maxTheoreticalStats[i] = 0;
    }

    int str = res.finalStats[static_cast<int>(StatType::STR)];
    int dex = res.finalStats[static_cast<int>(StatType::DEX)];
    int vit = res.finalStats[static_cast<int>(StatType::VIT)];
    int intStat = res.finalStats[static_cast<int>(StatType::INT)];
    int conStat = res.finalStats[static_cast<int>(StatType::CON)];

    int maxVit = res.maxTheoreticalStats[static_cast<int>(StatType::VIT)];
    int maxIntStat = res.maxTheoreticalStats[static_cast<int>(StatType::INT)];

    int lvl = effectiveLvl;

    // 4. Calculate HP & MP
    res.hp = config.hpFormula ? config.hpFormula(lvl, vit) : 0;
    res.maxTheoreticalHp = config.hpFormula ? config.hpFormula(lvl, maxVit) : 0;

    res.mp = config.mpFormula ? config.mpFormula(lvl, intStat) : 0;
    res.maxTheoreticalMp = config.mpFormula ? config.mpFormula(lvl, maxIntStat) : 0;

    // 5. Calculate Physical ATK
    res.maxAtk = config.maxAtkFormula ? config.maxAtkFormula(lvl, str) : (state.equipAtk + static_cast<int>(std::floor(str * 1.8)));
    res.minAtk = state.equipAtk / 2 + str + dex / 2;
    if (res.minAtk > res.maxAtk) res.minAtk = res.maxAtk;

    // Critical Rate %
    if (res.minAtk > 0 && res.maxAtk > res.minAtk) {
        double ratio = (static_cast<double>(res.maxAtk) / res.minAtk) - 1.0;
        res.atkCriticalRate = ratio * 75.0 + state.equipCrit;
    } else {
        res.atkCriticalRate = state.equipCrit;
    }
    if (res.atkCriticalRate < 0) res.atkCriticalRate = 0;
    if (res.atkCriticalRate > 100) res.atkCriticalRate = 100;

    // ATK Expectation
    double avgAtk = (res.minAtk + res.maxAtk) / 2.0;
    double expAtk = avgAtk * (1.0 + (res.atkCriticalRate / 100.0) * 0.8);
    res.atkExpectation = std::round(expAtk * 1000.0) / 1000.0;

    // 6. Calculate Magical MATK
    res.maxMatk = config.maxAtkFormula ? config.maxAtkFormula(lvl, intStat) : (state.equipMatk + static_cast<int>(std::floor(intStat * 1.8 + conStat * 0.8)));
    res.minMatk = state.equipMatk / 2 + conStat;
    if (res.minMatk > res.maxMatk) res.minMatk = res.maxMatk;

    if (res.minMatk > 0 && res.maxMatk > res.minMatk) {
        double ratio = (static_cast<double>(res.maxMatk) / res.minMatk) - 1.0;
        res.matkCriticalRate = ratio * 75.0 + state.equipCrit;
    } else {
        res.matkCriticalRate = state.equipCrit;
    }
    if (res.matkCriticalRate < 0) res.matkCriticalRate = 0;
    if (res.matkCriticalRate > 100) res.matkCriticalRate = 100;

    double avgMatk = (res.minMatk + res.maxMatk) / 2.0;
    double expMatk = avgMatk * (1.0 + (res.matkCriticalRate / 100.0) * 0.8);
    res.matkExpectation = std::round(expMatk * 1000.0) / 1000.0;

    // 7. Calculate Combined AtkMatk (物魔)
    res.minAtkMatk = static_cast<int>(std::floor((res.minAtk + res.minMatk) * 0.575));
    res.maxAtkMatk = static_cast<int>(std::floor((res.maxAtk + res.maxMatk) * 0.575));
    res.atkMatkCriticalRate = (res.atkCriticalRate + res.matkCriticalRate) / 2.0;
    double avgAtkMatk = (res.minAtkMatk + res.maxAtkMatk) / 2.0;
    res.atkMatkExpectation = std::round(avgAtkMatk * (1.0 + (res.atkMatkCriticalRate / 100.0) * 0.8) * 1000.0) / 1000.0;

    // 8. Charge frames (下限 5f)
    res.rightChargeFrames = std::max(5, 12 - dex / 10);
    res.leftChargeFrames = std::max(5, 16 - dex / 10);

    return res;
}

} // namespace rolc

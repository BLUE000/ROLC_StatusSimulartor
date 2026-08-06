#include "CalculatorEngine.hpp"
#include "MasterData.hpp"
#include <cmath>
#include <algorithm>

namespace rolc {

FormulaConfig FormulaConfig::defaultConfig() {
    FormulaConfig cfg;

    cfg.baseMakingPoints = 100;
    cfg.classUnlockMakingPointBonus = 10;

    cfg.hpFormula = nullptr;
    cfg.mpFormula = nullptr;

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

    // Determine Effective Level
    int currentStageIdx = static_cast<int>(state.currentClassStage) - 1;
    int currentClassLvl = state.historyClassLevels[currentStageIdx];
    if (currentClassLvl <= 0) currentClassLvl = 100;
    int effectiveLvl = (state.restrictedLevel > 0) ? std::min(currentClassLvl, state.restrictedLevel) : currentClassLvl;

    // 2. Fetch Character Base Stats (Dynamic calculation based on effectiveLvl)
    const CharacterData* charData = MasterData::getCharacterById(state.characterId);
    std::array<int, 6> charBase = {0, 0, 0, 0, 0, 0};
    if (charData) {
        if (state.characterId >= 53) {
            charBase = charData->baseStats;
        } else {
            for (int i = 0; i < 6; ++i) {
                double bPlus20 = effectiveLvl + 20.0;
                double bDiv10 = effectiveLvl / 10.0;
                double val1 = (bPlus20 * ((bPlus20 * (charData->baseStatCoeffs[i] * 3.0 + bDiv10 + 40.0) + 1000.0) / 1000.0));
                double val2 = (bDiv10 + 2.0) * (bDiv10 + 2.0);
                charBase[i] = static_cast<int>(std::floor(val1 - val2));
            }
        }
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
            for (int i = 0; i < 6; ++i) {
                int halfBonus = histClass->statBonuses[i] / 2;
                percentBonuses[i] += halfBonus;
                maxTheoreticalPercentBonuses[i] += halfBonus;
            }
        }
    }

    // Compute max_status_bonus_point cap (statusbonus.js)
    int additionalClassCount = 0;
    if (state.secondClassId != 0) additionalClassCount++;
    if (state.thirdClassId != 0) additionalClassCount++;
    if (state.fourthClassId != 0) additionalClassCount++;
    if (state.exClassId != 0) additionalClassCount++;
    int maxBonusCap = 100 + 10 * additionalClassCount;

    for (int i = 0; i < 6; ++i) {
        if (percentBonuses[i] > maxBonusCap) {
            percentBonuses[i] = maxBonusCap;
        }
        if (maxTheoreticalPercentBonuses[i] > maxBonusCap) {
            maxTheoreticalPercentBonuses[i] = maxBonusCap;
        }
    }

    res.baseStats = allocatedStats;
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
    int menStat = res.finalStats[static_cast<int>(StatType::MEN)];

    int maxVit = res.maxTheoreticalStats[static_cast<int>(StatType::VIT)];
    int maxIntStat = res.maxTheoreticalStats[static_cast<int>(StatType::INT)];

    int lvl = effectiveLvl;

    // 4. Desperate Skill Modifiers (statusbonus.js: desperate_atack_magic)
    int vitMenAvg = (vit + menStat) / 2;
    if (state.desperateAttack) {
        double factor = state.berserkArmor ? 0.90 : 0.75;
        str = static_cast<int>(std::floor((str + vitMenAvg) * factor));
        dex = static_cast<int>(std::floor((dex + vitMenAvg) * factor));
        vit = 0;
        menStat = 0;
    } else if (state.desperateMagic) {
        double factor = state.wizardClothes ? 0.90 : 0.75;
        intStat = static_cast<int>(std::floor((intStat + vitMenAvg) * factor));
        conStat = static_cast<int>(std::floor((conStat + vitMenAvg) * factor));
        vit = 0;
        menStat = 0;
    } else if (state.desperateAssault) {
        double factor = state.conquerorArmor ? 0.85 : 0.70;
        str = static_cast<int>(std::floor((str + vitMenAvg) * factor));
        dex = static_cast<int>(std::floor((dex + vitMenAvg) * factor));
        intStat = static_cast<int>(std::floor((intStat + vitMenAvg) * factor));
        conStat = static_cast<int>(std::floor((conStat + vitMenAvg) * factor));
        vit = 0;
        menStat = 0;
    }

    // 5. Calculate HP & MP (statusbonus.js: calc_character_HP)
    if (config.hpFormula) {
        res.hp = config.hpFormula(lvl, vit);
        res.maxTheoreticalHp = config.hpFormula(lvl, maxVit);
    } else {
        int charHpCoeff = charData ? charData->hpCoeff : 6;
        double aPlus20 = effectiveLvl + 20.0;
        double hpVal = aPlus20 * ((aPlus20 * (charHpCoeff * 60.0 + effectiveLvl / 2.0 + 600.0) + 5000.0) / 5000.0);
        res.hp = static_cast<int>(std::floor(hpVal)) * 10;
        res.maxTheoreticalHp = res.hp;
    }

    if (config.mpFormula) {
        res.mp = config.mpFormula(lvl, intStat);
        res.maxTheoreticalMp = config.mpFormula(lvl, maxIntStat);
    } else {
        res.mp = 1000;
        res.maxTheoreticalMp = 1000;
    }

    // 6. Calculate Physical ATK (statusbonus.js: store_atk_matk_val)
    double rawMaxAtk = str / 2.0 + state.equipAtk;
    double rawMinAtk = std::min(dex + state.equipAtk / 2.0, std::floor(str / 2.0) + state.equipAtk);

    res.maxAtk = static_cast<int>(std::floor(rawMaxAtk));
    res.minAtk = static_cast<int>(std::floor(rawMinAtk));

    // Physical Critical Rate (statusbonus.js: calc_critical_rate)
    double uncappedMinAtk = dex + state.equipAtk / 2.0;
    if (rawMaxAtk < uncappedMinAtk && rawMaxAtk > 0) {
        res.atkCriticalRate = ((uncappedMinAtk / rawMaxAtk) - 1.0) * 50.0 + state.equipCrit;
    } else {
        res.atkCriticalRate = state.equipCrit;
    }
    if (res.atkCriticalRate > 100.0) res.atkCriticalRate = 100.0;
    if (res.atkCriticalRate < 0.0) res.atkCriticalRate = 0.0;

    // Physical Expectation (statusbonus.js: calc_expectation)
    double avgAtk = (rawMaxAtk + rawMinAtk) / 2.0;
    double expAtk = avgAtk * (1.0 + (res.atkCriticalRate / 100.0) * 0.8);
    res.atkExpectation = std::round(expAtk * 10000.0) / 10000.0;

    // 7. Calculate Magical MATK (statusbonus.js: store_atk_matk_val)
    double rawMaxMatk = intStat / 2.0 + state.equipMatk;
    double rawMinMatk = std::min(conStat + state.equipMatk / 2.0, std::floor(intStat / 2.0) + state.equipMatk);

    res.maxMatk = static_cast<int>(std::floor(rawMaxMatk));
    res.minMatk = static_cast<int>(std::floor(rawMinMatk));

    // Magical Critical Rate
    double uncappedMinMatk = conStat + state.equipMatk / 2.0;
    if (rawMaxMatk < uncappedMinMatk && rawMaxMatk > 0) {
        res.matkCriticalRate = ((uncappedMinMatk / rawMaxMatk) - 1.0) * 50.0 + state.equipCrit;
    } else {
        res.matkCriticalRate = state.equipCrit;
    }
    if (res.matkCriticalRate > 100.0) res.matkCriticalRate = 100.0;
    if (res.matkCriticalRate < 0.0) res.matkCriticalRate = 0.0;

    double avgMatk = (rawMaxMatk + rawMinMatk) / 2.0;
    double expMatk = avgMatk * (1.0 + (res.matkCriticalRate / 100.0) * 0.8);
    res.matkExpectation = std::round(expMatk * 10000.0) / 10000.0;

    // 8. Calculate Combined AtkMatk (物魔) (statusbonus.js: store_atk_matk_val)
    double hybridFactor = state.desperateAssault ? 1.20 : 1.15;
    double rawMaxAtkMatk = ((std::floor(rawMaxAtk) + std::floor(rawMaxMatk)) / 2.0) * hybridFactor;
    double rawMinAtkMatk = ((std::floor(rawMinAtk) + std::floor(rawMinMatk)) / 2.0) * hybridFactor;

    res.maxAtkMatk = static_cast<int>(std::floor(rawMaxAtkMatk));
    res.minAtkMatk = static_cast<int>(std::floor(rawMinAtkMatk));
    res.atkMatkCriticalRate = std::min(100.0, (res.atkCriticalRate + res.matkCriticalRate) / 2.0);

    double avgAtkMatk = (rawMaxAtkMatk + rawMinAtkMatk) / 2.0;
    double expAtkMatk = avgAtkMatk * (1.0 + (res.atkMatkCriticalRate / 100.0) * 0.8);
    res.atkMatkExpectation = std::round(expAtkMatk * 10000.0) / 10000.0;

    // 9. Charge frames (下限 5f)
    res.rightChargeFrames = std::max(5, 12 - dex / 10);
    res.leftChargeFrames = std::max(5, 16 - dex / 10);

    return res;
}

} // namespace rolc

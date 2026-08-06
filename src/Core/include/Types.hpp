#ifndef ROLC_TYPES_HPP
#define ROLC_TYPES_HPP

#include <string>
#include <vector>
#include <array>
#include <cstdint>
#include <functional>

namespace rolc {

enum class StatType {
    STR = 0,
    DEX = 1,
    VIT = 2,
    INT = 3,
    CON = 4,
    MEN = 5,
    Count = 6
};

enum class ShareCategory {
    Free = 1,
    Kingdom = 2,
    Making = 3,
    CursedBlood = 4,
    TombOfKing = 5,
    SnowCastle = 6,
    Heritage = 7,
    ThunderTower = 8,
    IsolatedCastle = 9,
    DeepPalace = 10,
    OldRuins = 11,
    AncientCapital = 12,
    Fortress = 13,
    SealedArea = 14
};

enum class ClassStage {
    First = 1,
    Second = 2,
    Third = 3,
    Fourth = 4,
    Ex = 5
};

struct CharacterData {
    int id;
    std::string name;
    ShareCategory shareCategory;
    int firstClassId;
    std::array<int, 6> baseStats; // STR, DEX, VIT, INT, CON, MEN
    std::array<int, 24> weaponWearLevels; // 12 weapon R/L levels + wear levels
};

struct ClassData {
    int id;
    std::string name;
    std::string code;
    ClassStage stage;
    std::array<int, 6> statBonuses; // STR, DEX, VIT, INT, CON, MEN %
    std::array<std::string, 24> proficiencies;
};

struct TitleBonus {
    int id;
    std::string name;
    bool isLimited;
    std::array<int, 6> percentBonuses; // STR, DEX, VIT, INT, CON, MEN (%)
};

struct AmplifierBonus {
    int id;
    std::string name;
    bool isLimited;
    std::array<int, 6> flatBonuses; // STR, DEX, VIT, INT, CON, MEN
};

struct UserBuildState {
    int shareCategoryId{1};
    int characterId{1};
    int level{1};
    int moral{100};
    int equipAtk{0};
    int equipMatk{0};
    int equipCrit{0};
    bool isNonShare{false};

    int firstClassId{1};
    int secondClassId{0};
    int thirdClassId{0};
    int fourthClassId{0};
    int exClassId{0};
    ClassStage currentClassStage{ClassStage::First};

    std::array<int, 5> historyClassLevels{100, 100, 100, 100, 100}; // Lv 1~100 for 1st, 2nd, 3rd, 4th, Ex

    std::array<int, 6> makingPoints{0, 0, 0, 0, 0, 0}; // 0~20 for each stat

    int rightHandTitleId{0};
    int leftHandTitleId{0};
    int bodyTitleId{0};
    int handTitleId{0};
    int legTitleId{0};

    int amplifierId{0};

    bool desperateAttack{false};
    bool desperateMagic{false};
    bool desperateAssault{false};
    bool berserkArmor{false};
    bool wizardClothes{false};
    bool conquerorArmor{false};
    bool crdBodyAmplifier{false};
};

struct DerivedStatsResult {
    // Calculated Base Stats (Final Values)
    std::array<int, 6> finalStats{0, 0, 0, 0, 0, 0};
    std::array<int, 6> statPercentBonuses{0, 0, 0, 0, 0, 0};

    // Theoretical Maximum Stats (All history classes maxed out)
    std::array<int, 6> maxTheoreticalStats{0, 0, 0, 0, 0, 0};
    std::array<int, 6> maxTheoreticalPercentBonuses{0, 0, 0, 0, 0, 0};

    // HP & MP
    int hp{0};
    int mp{0};
    int maxTheoreticalHp{0};
    int maxTheoreticalMp{0};

    // Charge frames
    int rightChargeFrames{0};
    int leftChargeFrames{0};

    // Physical ATK
    int minAtk{0};
    int maxAtk{0};
    double atkCriticalRate{0.0};
    double atkExpectation{0.0};

    // Magical MATK
    int minMatk{0};
    int maxMatk{0};
    double matkCriticalRate{0.0};
    double matkExpectation{0.0};

    // Physical + Magical ATK
    int minAtkMatk{0};
    int maxAtkMatk{0};
    double atkMatkCriticalRate{0.0};
    double atkMatkExpectation{0.0};

    // Points
    int maxMakingPoints{100};
    int remainingMakingPoints{100};
};

// Configurable calculation parameters & formulas for future customization
struct FormulaConfig {
    int baseMakingPoints{100};
    int classUnlockMakingPointBonus{10};

    // Custom HP/MP calculation strategy
    std::function<int(int level, int vit)> hpFormula;
    std::function<int(int level, int intStat)> mpFormula;

    // Custom Max ATK/MATK calculation strategy
    std::function<int(int level, int stat)> maxAtkFormula;

    // Default configuration factory
    static FormulaConfig defaultConfig();
};

} // namespace rolc

#endif // ROLC_TYPES_HPP

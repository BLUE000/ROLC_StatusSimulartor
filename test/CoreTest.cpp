#include "CalculatorEngine.hpp"
#include "MasterData.hpp"
#include <QTest>
#include <QObject>
#include <iostream>

class CoreTest : public QObject {
    Q_OBJECT

private slots:
    void testMakingPointCosts() {
        QCOMPARE(rolc::CalculatorEngine::calculateMakingPointCost(0), 0);
        QCOMPARE(rolc::CalculatorEngine::calculateMakingPointCost(5), 5);
        QCOMPARE(rolc::CalculatorEngine::calculateMakingPointCost(10), 15);
        QCOMPARE(rolc::CalculatorEngine::calculateMakingPointCost(15), 30);
        QCOMPARE(rolc::CalculatorEngine::calculateMakingPointCost(20), 50);
    }

    void testMaxMakingPoints() {
        rolc::UserBuildState state;
        QCOMPARE(rolc::CalculatorEngine::calculateMaxMakingPoints(state), 100);

        state.secondClassId = 1;
        QCOMPARE(rolc::CalculatorEngine::calculateMaxMakingPoints(state), 110);

        state.thirdClassId = 1;
        state.fourthClassId = 1;
        state.exClassId = 1;
        QCOMPARE(rolc::CalculatorEngine::calculateMaxMakingPoints(state), 140);
    }

    void testFeltesBaseCalculation() {
        rolc::UserBuildState state;
        state.characterId = 1; // フェルテス
        state.level = 100;
        state.makingPoints = {10, 0, 0, 0, 0, 0}; // STR +10

        auto result = rolc::CalculatorEngine::calculate(state);

        QCOMPARE(result.finalStats[0], 1346);

        // Remaining making points: 100 - cost(10) = 100 - 15 = 85
        QCOMPARE(result.remainingMakingPoints, 85);
        QVERIFY(result.hp > 0);
        QVERIFY(result.mp > 0);
        QVERIFY(result.maxAtk >= result.minAtk);
    }

    void testHistoryClassLevelRatioAndMaxTheoretical() {
        rolc::UserBuildState state;
        state.characterId = 1; // フェルテス
        state.level = 100;
        state.firstClassId = 1; // SOL
        state.secondClassId = 1; // BRV
        state.currentClassStage = rolc::ClassStage::Second;

        // When 1st Class (SOL) is at Lv 50 while current 2nd Class (BRV) is at Lv 100
        state.historyClassLevels[0] = 50;  // 1st class level
        state.historyClassLevels[1] = 100; // 2nd class level

        auto result = rolc::CalculatorEngine::calculate(state);

        // Max theoretical stats should be >= current stats when history level < 100
        QVERIFY(result.maxTheoreticalStats[0] >= result.finalStats[0]);
        QVERIFY(result.maxTheoreticalPercentBonuses[0] >= result.statPercentBonuses[0]);

        // When all history class levels are set to 100, current stat equals max theoretical stat
        state.historyClassLevels[0] = 100;
        auto maxResult = rolc::CalculatorEngine::calculate(state);
        QCOMPARE(maxResult.finalStats[0], maxResult.maxTheoreticalStats[0]);
        QCOMPARE(maxResult.statPercentBonuses[0], maxResult.maxTheoreticalPercentBonuses[0]);
    }

    void testClassDerivationFiltering() {
        // UT-MASTER-001: 1st Class SOL (1) -> 2nd Class BRV (1), CEN (2)
        auto solDerived = rolc::MasterData::getValidDerivedClasses(rolc::ClassStage::Second, 1);
        QCOMPARE(solDerived.size(), static_cast<size_t>(2));
        QCOMPARE(solDerived[0].id, 1); // BRV
        QCOMPARE(solDerived[1].id, 2); // CEN

        // UT-MASTER-002: 2nd Class BRV (1) -> 3rd Class LOR (1), CNQ (2)
        auto brvDerived = rolc::MasterData::getValidDerivedClasses(rolc::ClassStage::Third, 1);
        QCOMPARE(brvDerived.size(), static_cast<size_t>(2));
        QCOMPARE(brvDerived[0].id, 1); // LOR
        QCOMPARE(brvDerived[1].id, 2); // CNQ

        // UT-MASTER-003: 3rd Class LOR (1) -> 4th Class HRO (1)
        auto lorDerived = rolc::MasterData::getValidDerivedClasses(rolc::ClassStage::Fourth, 1);
        QCOMPARE(lorDerived.size(), static_cast<size_t>(1));
        QCOMPARE(lorDerived[0].id, 1); // HRO

        // UT-MASTER-004: 3rd Class ELM (8) -> 4th Class SPM (7), NIN (2)
        auto elmDerived = rolc::MasterData::getValidDerivedClasses(rolc::ClassStage::Fourth, 8);
        QCOMPARE(elmDerived.size(), static_cast<size_t>(2));
        QCOMPARE(elmDerived[0].id, 7); // SPM
        QCOMPARE(elmDerived[1].id, 2); // NIN
    }

    void testShareCategoryLevelLimits() {
        // UT-MASTER-005: Kingdom Share (2) -> Max level is 50
        QCOMPARE(rolc::MasterData::getMaxLevelForShareCategory(2), 50);

        // UT-MASTER-006: Free / Other Share (1) -> Max level is 100
        QCOMPARE(rolc::MasterData::getMaxLevelForShareCategory(1), 100);

        // UT-MASTER-008: Kingdom Edition -> Max level is 50
        QCOMPARE(rolc::MasterData::getMaxLevelForEdition(rolc::Edition::Kingdom), 50);

        // UT-MASTER-009: Earth Edition -> Max level is 100
        QCOMPARE(rolc::MasterData::getMaxLevelForEdition(rolc::Edition::Earth), 100);
    }

    void testStatusTitleBonus() {
        // UT-MASTER-007: Title "剛力" (ID: 4) gives STR +60%
        auto title = rolc::MasterData::getTitleBonusById(4);
        QVERIFY(title != nullptr);
        QCOMPARE(title->name, std::string("剛力 (STR+60%)"));
        QCOMPARE(title->percentBonuses[0], 60);

        rolc::UserBuildState state;
        state.rightHandTitleId = 4; // 剛力
        auto result = rolc::CalculatorEngine::calculate(state);
        // Default 1st Class SOL (+10% STR) + Title 剛力 (+60% STR) = 70%
        QCOMPARE(result.statPercentBonuses[0], 70);

        // UT-MASTER-010: 蛮勇 (ID: 28) -> STR/VIT +20%
        auto title28 = rolc::MasterData::getTitleBonusById(28);
        QVERIFY(title28 != nullptr);
        QCOMPARE(title28->name, std::string("蛮勇 (STR/VIT+20%)"));
        QCOMPARE(title28->percentBonuses[0], 20);
        QCOMPARE(title28->percentBonuses[2], 20);

        // UT-MASTER-011: 闘気 (ID: 47) -> STR/DEX/VIT +20%
        auto title47 = rolc::MasterData::getTitleBonusById(47);
        QVERIFY(title47 != nullptr);
        QCOMPARE(title47->name, std::string("闘気 (STR/DEX/VIT+20%)"));
        QCOMPARE(title47->percentBonuses[0], 20);
        QCOMPARE(title47->percentBonuses[1], 20);
        QCOMPARE(title47->percentBonuses[2], 20);

        // UT-MASTER-012: 万能 (ID: 53) -> All +15%
        auto title53 = rolc::MasterData::getTitleBonusById(53);
        QVERIFY(title53 != nullptr);
        QCOMPARE(title53->name, std::string("万能 (全+15%)"));
        QCOMPARE(title53->percentBonuses[0], 15);
    }

    void testRestrictedLevel() {
        // UT-CALC-005: Restricted level scaling (e.g. Current Level 100, Restricted Level 5)
        rolc::UserBuildState state;
        state.level = 100;
        state.restrictedLevel = 5;
        auto result = rolc::CalculatorEngine::calculate(state);
        QVERIFY(result.minAtk > 0);
        QVERIFY(result.maxAtk > 0);
    }

    void testHybridAtkMatk() {
        // UT-CALC-006: Hybrid AtkMatk (物魔) formula check (0.575 / 1.15 factor)
        rolc::UserBuildState state;
        state.equipAtk = 0;
        state.equipMatk = 0;
        auto result = rolc::CalculatorEngine::calculate(state);
        int expectedMinAtkMatk = static_cast<int>(std::floor((result.minAtk + result.minMatk) / 2.0 * 1.15));
        int expectedMaxAtkMatk = static_cast<int>(std::floor((result.maxAtk + result.maxMatk) / 2.0 * 1.15));
        QCOMPARE(result.minAtkMatk, expectedMinAtkMatk);
        QCOMPARE(result.maxAtkMatk, expectedMaxAtkMatk);
    }

    void testScreenshotVerification() {
        // Exact verification against user screenshot for Michal Lv5 / Restricted Lv5
        rolc::UserBuildState state;
        state.characterId = 7; // ミヒャル
        state.level = 100;
        state.restrictedLevel = 5;
        state.moral = 100;
        state.equipAtk = 234;
        state.equipMatk = 152;
        state.equipCrit = 5;

        state.firstClassId = 4; // KNT
        state.secondClassId = 5; // GLD
        state.thirdClassId = 4; // BSK
        state.fourthClassId = 5; // SAM (active)
        state.exClassId = 5; // MNK
        state.currentClassStage = rolc::ClassStage::Fourth; // SAM

        state.historyClassLevels = {25, 53, 91, 100, 65};
        state.makingPoints = {0, 0, 0, 0, 0, 0};

        state.rightHandTitleId = 54; // 全能 (+20%)
        state.leftHandTitleId = 54;  // 全能 (+20%)
        state.bodyTitleId = 45;       // 不倒 (VM+40%)
        state.handTitleId = 48;       // 覇気 (SDV+30%)
        state.legTitleId = 54;        // 全能 (+20%)

        auto res = rolc::CalculatorEngine::calculate(state);

        qDebug() << "FINAL STR:" << res.finalStats[0] << "DEX:" << res.finalStats[1] << "VIT:" << res.finalStats[2]
                 << "INT:" << res.finalStats[3] << "CON:" << res.finalStats[4] << "MEN:" << res.finalStats[5];
        qDebug() << "BONUS STR %:" << res.statPercentBonuses[0] << "DEX %:" << res.statPercentBonuses[1]
                 << "VIT %:" << res.statPercentBonuses[2] << "INT %:" << res.statPercentBonuses[3]
                 << "CON %:" << res.statPercentBonuses[4] << "MEN %:" << res.statPercentBonuses[5];
        qDebug() << "HP:" << res.hp << "minAtk:" << res.minAtk << "maxAtk:" << res.maxAtk;

        // Final stats check against Qt App Screenshot titles
        QCOMPARE(res.finalStats[0], 180); // STR
        QCOMPARE(res.finalStats[1], 133); // DEX
        QCOMPARE(res.finalStats[2], 148); // VIT
        QCOMPARE(res.finalStats[3], 70);  // INT
        QCOMPARE(res.finalStats[4], 60);  // CON
        QCOMPARE(res.finalStats[5], 116); // MEN

        // HP & MP check
        QCOMPARE(res.hp, 1450);

        // Physical ATK check
        QCOMPARE(res.minAtk, 250);
        QCOMPARE(res.maxAtk, 324);
        QCOMPARE(res.atkCriticalRate, 5.0);
        QCOMPARE(res.atkExpectation, 298.48);

        // Magical MATK check
        QCOMPARE(res.minMatk, 136);
        QCOMPARE(res.maxMatk, 187);
        QCOMPARE(res.matkCriticalRate, 5.0);
        QCOMPARE(res.matkExpectation, 167.96);

        // Hybrid AtkMatk check
        QCOMPARE(res.minAtkMatk, 221);
        QCOMPARE(res.maxAtkMatk, 293);
        QCOMPARE(res.atkMatkCriticalRate, 5.0);
        QCOMPARE(res.atkMatkExpectation, 267.28);
    }
};

QTEST_MAIN(CoreTest)
#include "CoreTest.moc"

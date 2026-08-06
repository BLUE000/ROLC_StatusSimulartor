#include "CalculatorEngine.hpp"
#include "MasterData.hpp"
#include <QTest>
#include <QObject>

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

        // Character 1 (フェルテス) base STR is 8. With +10 making points = 18.
        // 1st Class Solider (SOL) gives STR +10%.
        // 18 * 1.10 = 19.8 -> floor = 19.
        QCOMPARE(result.finalStats[0], 19);

        // Remaining making points: 100 - cost(10) = 100 - 15 = 85
        QCOMPARE(result.remainingMakingPoints, 85);
        QVERIFY(result.hp > 0);
        QVERIFY(result.mp > 0);
        QVERIFY(result.maxAtk > result.minAtk);
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
    }
};

QTEST_MAIN(CoreTest)
#include "CoreTest.moc"

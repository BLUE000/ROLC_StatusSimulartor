#include "SimulatorPresenter.hpp"
#include "ISimulatorView.hpp"
#include <QTest>
#include <QObject>

class MockSimulatorView : public rolc::ISimulatorView {
public:
    rolc::DerivedStatsResult lastResult;
    int updateCallCount{0};

    void updateDerivedStats(const rolc::DerivedStatsResult& result) override {
        lastResult = result;
        updateCallCount++;
    }
};

class PresenterTest : public QObject {
    Q_OBJECT

private slots:
    void testPresenterSlotInvocation() {
        MockSimulatorView mockView;
        rolc::SimulatorPresenter presenter(&mockView);

        // Initial recalculation when view is ready
        presenter.recalculate();
        QCOMPARE(mockView.updateCallCount, 1);

        // Simulate user selecting character
        presenter.onCharacterSelected(2); // ガンツェ
        QCOMPARE(presenter.getState().characterId, 2);
        QCOMPARE(mockView.updateCallCount, 2);

        // Simulate user changing level
        presenter.onLevelChanged(50);
        QCOMPARE(presenter.getState().level, 50);
        QCOMPARE(mockView.updateCallCount, 3);

        // Simulate user toggling Desperate Attack checkbox
        presenter.onDesperateAttackToggled(true);
        QVERIFY(presenter.getState().desperateAttack);
        QCOMPARE(mockView.updateCallCount, 4);

        // Simulate user selecting Edition (Kingdom)
        presenter.onEditionSelected(rolc::Edition::Kingdom);
        QCOMPARE(presenter.getState().edition, rolc::Edition::Kingdom);
        QCOMPARE(mockView.updateCallCount, 5);

        // Verify that stats were recalculated correctly
        QVERIFY(mockView.lastResult.finalStats[0] > 0);
    }
};

QTEST_MAIN(PresenterTest)
#include "PresenterTest.moc"

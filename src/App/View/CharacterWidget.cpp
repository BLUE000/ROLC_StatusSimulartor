#include "CharacterWidget.hpp"
#include "SimulatorPresenter.hpp"
#include "MasterData.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>

namespace rolc {

CharacterWidget::CharacterWidget(SimulatorPresenter* presenter, QWidget* parent)
    : QWidget(parent)
    , m_presenter(presenter)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // Section 1: Character & Share
    QGroupBox* charGroup = new QGroupBox("キャラクター・基本情報", this);
    QVBoxLayout* charGroupLayout = new QVBoxLayout(charGroup);

    QHBoxLayout* row1Layout = new QHBoxLayout();
    row1Layout->setContentsMargins(0, 0, 0, 0);
    row1Layout->addWidget(new QLabel("編区分:"));
    m_editionCombo = new QComboBox(this);
    for (const auto& ed : MasterData::getEditions()) {
        m_editionCombo->addItem(QString::fromStdString(ed));
    }
    // Default to Earth Edition (index 1)
    m_editionCombo->setCurrentIndex(1);
    row1Layout->addWidget(m_editionCombo);

    row1Layout->addWidget(new QLabel("シェア:"));
    m_shareCombo = new QComboBox(this);
    for (const auto& share : MasterData::getShareCategories()) {
        m_shareCombo->addItem(QString::fromStdString(share));
    }
    row1Layout->addWidget(m_shareCombo);

    row1Layout->addWidget(new QLabel("制限Lv:"));
    m_restrictedLevelSpin = new QSpinBox(this);
    m_restrictedLevelSpin->setRange(1, 100);
    m_restrictedLevelSpin->setValue(100);
    m_restrictedLevelSpin->setMaximumWidth(60);
    row1Layout->addWidget(m_restrictedLevelSpin);

    row1Layout->addWidget(new QLabel("キャラ:"));
    m_characterCombo = new QComboBox(this);
    row1Layout->addWidget(m_characterCombo);
    row1Layout->addStretch();

    QHBoxLayout* row2Layout = new QHBoxLayout();
    row2Layout->setContentsMargins(0, 0, 0, 0);

    auto makeSpin = [this](int minVal, int maxVal, int defaultVal) {
        QSpinBox* spin = new QSpinBox(this);
        spin->setRange(minVal, maxVal);
        spin->setValue(defaultVal);
        spin->setMaximumWidth(80);
        return spin;
    };

    row2Layout->addWidget(new QLabel("モラル:"));
    m_moralSpin = makeSpin(0, 100, 100);
    row2Layout->addWidget(m_moralSpin);

    row2Layout->addSpacing(15);
    row2Layout->addWidget(new QLabel("装備ATK:"));
    m_equipAtkSpin = makeSpin(0, 9999, 0);
    row2Layout->addWidget(m_equipAtkSpin);

    row2Layout->addSpacing(15);
    row2Layout->addWidget(new QLabel("装備MATK:"));
    m_equipMatkSpin = makeSpin(0, 9999, 0);
    row2Layout->addWidget(m_equipMatkSpin);

    row2Layout->addSpacing(15);
    row2Layout->addWidget(new QLabel("装備会心:"));
    m_equipCritSpin = makeSpin(0, 100, 0);
    row2Layout->addWidget(m_equipCritSpin);

    row2Layout->addStretch();

    charGroupLayout->addLayout(row1Layout);
    charGroupLayout->addLayout(row2Layout);

    mainLayout->addWidget(charGroup);

    // Section 2: Class Selection & Class Level Setup
    QGroupBox* classGroup = new QGroupBox("クラス構成・到達Lv（１次〜４次 / Ex）", this);
    QGridLayout* classGrid = new QGridLayout(classGroup);

    classGrid->addWidget(new QLabel("１次クラス"), 0, 0);
    classGrid->addWidget(new QLabel("２次クラス"), 0, 1);
    classGrid->addWidget(new QLabel("３次クラス"), 0, 2);
    classGrid->addWidget(new QLabel("４次クラス"), 0, 3);
    classGrid->addWidget(new QLabel("Exクラス"), 0, 4);

    m_firstClassCombo = new QComboBox(this);
    m_secondClassCombo = new QComboBox(this);
    m_thirdClassCombo = new QComboBox(this);
    m_fourthClassCombo = new QComboBox(this);
    m_exClassCombo = new QComboBox(this);

    classGrid->addWidget(m_firstClassCombo, 1, 0);
    classGrid->addWidget(m_secondClassCombo, 1, 1);
    classGrid->addWidget(m_thirdClassCombo, 1, 2);
    classGrid->addWidget(m_fourthClassCombo, 1, 3);
    classGrid->addWidget(m_exClassCombo, 1, 4);

    // Level inputs per class stage
    auto makeLvlSpin = [this]() {
        QSpinBox* spin = new QSpinBox(this);
        spin->setRange(1, 100);
        spin->setValue(100);
        spin->setPrefix("Lv ");
        return spin;
    };

    m_firstClassLevelSpin = makeLvlSpin();
    m_secondClassLevelSpin = makeLvlSpin();
    m_thirdClassLevelSpin = makeLvlSpin();
    m_fourthClassLevelSpin = makeLvlSpin();
    m_exClassLevelSpin = makeLvlSpin();

    classGrid->addWidget(m_firstClassLevelSpin, 2, 0);
    classGrid->addWidget(m_secondClassLevelSpin, 2, 1);
    classGrid->addWidget(m_thirdClassLevelSpin, 2, 2);
    classGrid->addWidget(m_fourthClassLevelSpin, 2, 3);
    classGrid->addWidget(m_exClassLevelSpin, 2, 4);

    // Radio buttons for Current Class
    m_currentClassGroup = new QButtonGroup(this);
    m_radioFirst = new QRadioButton("現在", this);
    m_radioSecond = new QRadioButton("現在", this);
    m_radioThird = new QRadioButton("現在", this);
    m_radioFourth = new QRadioButton("現在", this);
    m_radioEx = new QRadioButton("現在", this);

    m_currentClassGroup->addButton(m_radioFirst, 1);
    m_currentClassGroup->addButton(m_radioSecond, 2);
    m_currentClassGroup->addButton(m_radioThird, 3);
    m_currentClassGroup->addButton(m_radioFourth, 4);
    m_currentClassGroup->addButton(m_radioEx, 5);
    m_radioFirst->setChecked(true);

    classGrid->addWidget(m_radioFirst, 3, 0);
    classGrid->addWidget(m_radioSecond, 3, 1);
    classGrid->addWidget(m_radioThird, 3, 2);
    classGrid->addWidget(m_radioFourth, 3, 3);
    classGrid->addWidget(m_radioEx, 3, 4);

    mainLayout->addWidget(classGroup);

    populateCharacters();
    populateClasses();
    updateLevelLimits();

    // Connect signals to Presenter slots
    connect(m_editionCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int idx) {
        Edition edition = static_cast<Edition>(idx);
        updateLevelLimits();
        m_presenter->onEditionSelected(edition);
    });
    connect(m_shareCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int idx) {
        m_presenter->onShareCategorySelected(idx);
    });
    connect(m_characterCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int idx) {
        int charId = m_characterCombo->itemData(idx).toInt();
        m_presenter->onCharacterSelected(charId);
    });
    connect(m_moralSpin, QOverload<int>::of(&QSpinBox::valueChanged), m_presenter, &SimulatorPresenter::onMoralChanged);
    connect(m_equipAtkSpin, QOverload<int>::of(&QSpinBox::valueChanged), m_presenter, &SimulatorPresenter::onEquipAtkChanged);
    connect(m_equipMatkSpin, QOverload<int>::of(&QSpinBox::valueChanged), m_presenter, &SimulatorPresenter::onEquipMatkChanged);
    connect(m_equipCritSpin, QOverload<int>::of(&QSpinBox::valueChanged), m_presenter, &SimulatorPresenter::onEquipCritChanged);

    connect(m_restrictedLevelSpin, QOverload<int>::of(&QSpinBox::valueChanged), [this](int val) {
        m_presenter->onRestrictedLevelChanged(val);
    });

    connect(m_firstClassCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int idx) {
        int classId = m_firstClassCombo->itemData(idx).toInt();
        m_presenter->onClassSelected(ClassStage::First, classId);
        updateDerivedClassCombos();
    });
    connect(m_secondClassCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int idx) {
        int classId = m_secondClassCombo->itemData(idx).toInt();
        m_presenter->onClassSelected(ClassStage::Second, classId);
        updateDerivedClassCombos();
    });
    connect(m_thirdClassCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int idx) {
        int classId = m_thirdClassCombo->itemData(idx).toInt();
        m_presenter->onClassSelected(ClassStage::Third, classId);
        updateDerivedClassCombos();
    });
    connect(m_fourthClassCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int idx) {
        int classId = m_fourthClassCombo->itemData(idx).toInt();
        m_presenter->onClassSelected(ClassStage::Fourth, classId);
    });
    connect(m_exClassCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int idx) {
        int classId = m_exClassCombo->itemData(idx).toInt();
        m_presenter->onClassSelected(ClassStage::Ex, classId);
    });

    connect(m_firstClassLevelSpin, QOverload<int>::of(&QSpinBox::valueChanged), [this](int val) {
        m_presenter->onHistoryClassLevelChanged(ClassStage::First, val);
    });
    connect(m_secondClassLevelSpin, QOverload<int>::of(&QSpinBox::valueChanged), [this](int val) {
        m_presenter->onHistoryClassLevelChanged(ClassStage::Second, val);
    });
    connect(m_thirdClassLevelSpin, QOverload<int>::of(&QSpinBox::valueChanged), [this](int val) {
        m_presenter->onHistoryClassLevelChanged(ClassStage::Third, val);
    });
    connect(m_fourthClassLevelSpin, QOverload<int>::of(&QSpinBox::valueChanged), [this](int val) {
        m_presenter->onHistoryClassLevelChanged(ClassStage::Fourth, val);
    });
    connect(m_exClassLevelSpin, QOverload<int>::of(&QSpinBox::valueChanged), [this](int val) {
        m_presenter->onHistoryClassLevelChanged(ClassStage::Ex, val);
    });

    connect(m_currentClassGroup, &QButtonGroup::idToggled, [this](int id, bool checked) {
        if (checked) {
            m_presenter->onCurrentClassStageChanged(static_cast<ClassStage>(id));
        }
    });
}

void CharacterWidget::populateCharacters() {
    m_characterCombo->clear();
    for (const auto& c : MasterData::getCharacters()) {
        m_characterCombo->addItem(QString::fromStdString(c.name), c.id);
    }
}

void CharacterWidget::populateClasses() {
    auto addItems = [](QComboBox* combo, ClassStage stage) {
        combo->clear();
        combo->addItem("なし", 0);
        for (const auto& cl : MasterData::getClassesByStage(stage)) {
            combo->addItem(QString::fromStdString(cl.name + " (" + cl.code + ")"), cl.id);
        }
    };

    addItems(m_firstClassCombo, ClassStage::First);
    addItems(m_secondClassCombo, ClassStage::Second);
    addItems(m_thirdClassCombo, ClassStage::Third);
    addItems(m_fourthClassCombo, ClassStage::Fourth);
    addItems(m_exClassCombo, ClassStage::Ex);

    updateDerivedClassCombos();
}

void CharacterWidget::updateDerivedClassCombos() {
    int firstId = m_firstClassCombo->itemData(m_firstClassCombo->currentIndex()).toInt();

    int curSecondId = m_secondClassCombo->itemData(m_secondClassCombo->currentIndex()).toInt();
    int curThirdId = m_thirdClassCombo->itemData(m_thirdClassCombo->currentIndex()).toInt();
    int curFourthId = m_fourthClassCombo->itemData(m_fourthClassCombo->currentIndex()).toInt();

    // 1. Update 2nd Class ComboBox
    m_secondClassCombo->blockSignals(true);
    m_secondClassCombo->clear();
    m_secondClassCombo->addItem("なし", 0);
    auto secondClasses = MasterData::getValidDerivedClasses(ClassStage::Second, firstId);
    for (const auto& cl : secondClasses) {
        m_secondClassCombo->addItem(QString::fromStdString(cl.name + " (" + cl.code + ")"), cl.id);
    }
    int secondIdx = m_secondClassCombo->findData(curSecondId);
    if (secondIdx < 0) secondIdx = 0;
    m_secondClassCombo->setCurrentIndex(secondIdx);
    m_secondClassCombo->blockSignals(false);
    int newSecondId = m_secondClassCombo->itemData(secondIdx).toInt();

    // 2. Update 3rd Class ComboBox
    m_thirdClassCombo->blockSignals(true);
    m_thirdClassCombo->clear();
    m_thirdClassCombo->addItem("なし", 0);
    auto thirdClasses = MasterData::getValidDerivedClasses(ClassStage::Third, newSecondId);
    for (const auto& cl : thirdClasses) {
        m_thirdClassCombo->addItem(QString::fromStdString(cl.name + " (" + cl.code + ")"), cl.id);
    }
    int thirdIdx = m_thirdClassCombo->findData(curThirdId);
    if (thirdIdx < 0) thirdIdx = 0;
    m_thirdClassCombo->setCurrentIndex(thirdIdx);
    m_thirdClassCombo->blockSignals(false);
    int newThirdId = m_thirdClassCombo->itemData(thirdIdx).toInt();

    // 3. Update 4th Class ComboBox
    m_fourthClassCombo->blockSignals(true);
    m_fourthClassCombo->clear();
    m_fourthClassCombo->addItem("なし", 0);
    auto fourthClasses = MasterData::getValidDerivedClasses(ClassStage::Fourth, newThirdId);
    for (const auto& cl : fourthClasses) {
        m_fourthClassCombo->addItem(QString::fromStdString(cl.name + " (" + cl.code + ")"), cl.id);
    }
    int fourthIdx = m_fourthClassCombo->findData(curFourthId);
    if (fourthIdx < 0) fourthIdx = 0;
    m_fourthClassCombo->setCurrentIndex(fourthIdx);
    m_fourthClassCombo->blockSignals(false);
}

void CharacterWidget::updateLevelLimits() {
    Edition edition = static_cast<Edition>(m_editionCombo->currentIndex());
    int maxLvl = MasterData::getMaxLevelForEdition(edition);

    auto updateSpin = [maxLvl](QSpinBox* spin) {
        spin->setMaximum(maxLvl);
        if (spin->value() > maxLvl) {
            spin->setValue(maxLvl);
        }
    };

    updateSpin(m_firstClassLevelSpin);
    updateSpin(m_secondClassLevelSpin);
    updateSpin(m_thirdClassLevelSpin);
    updateSpin(m_fourthClassLevelSpin);
    updateSpin(m_exClassLevelSpin);
}

} // namespace rolc

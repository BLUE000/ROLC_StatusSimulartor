# ROLC ステータスシミュレーター 単体テスト仕様書 (Unit Test Specification)

- **文書バージョン**: 1.7.0
- **最終更新日**: 2026-08-06
- **対応設計書**: [03_DetailedDesign.md](file:///d:/prog/C++/ROLC_StatusSimulator/docs/03_DetailedDesign.md)
- **対象システム**: ROLC ステータスシミュレーター (Qt6 / C++20)

---

## 1. テスト概要
詳細設計書（`03_DetailedDesign.md`）で定義された各関数・クラス単位の動作およびアルゴリズム計算精度を検証する単体テストケース群。

---

## 2. 単体テストケース一覧 (Unit Test Cases)

| テストID | テスト対象クラス/関数 | テスト入力条件 | 期待値・判定基準 |
| :--- | :--- | :--- | :--- |
| **UT-MASTER-001** | `MasterData::getValidDerivedClasses` | `ClassStage::Second, SOL` | 返却リストが `BRV`, `CEN` の2件のみであること |
| **UT-MASTER-002** | `MasterData::getValidDerivedClasses` | `ClassStage::Third, BRV` | 返却リストが `LOR`, `CNQ` の2件のみであること |
| **UT-MASTER-005** | `MasterData::getMaxLevelForShareCategory` | `ShareCategory::Kingdom` (王国編) | 返却値が `50` であること |
| **UT-MASTER-007** | `MasterData::getTitleBonusById` | 称号「剛力」 (ID: 4) | STR 補正 +60% が正しく返却されること |
| **UT-CALC-001** | `CalculatorEngine::calculateMakingPointCost` | `pt = 0, 5, 10, 15, 20` | コスト `0, 5, 15, 30, 50` を正確に返却すること |
| **UT-CALC-004** | `CalculatorEngine::calculate` | 侍(Lv100), 履歴クラスLv50設定 | 実機ステータス（STR 3174, +122%）と理論最大値（STR 3432, +140%）が同時に正しく算出されること |

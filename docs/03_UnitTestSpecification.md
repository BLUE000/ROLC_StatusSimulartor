# ROLC ステータスシミュレーター 単体テスト仕様書 (Unit Test Specification)

- **文書バージョン**: 1.8.0
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
| **UT-MASTER-005** | `MasterData::getMaxLevelForEdition` | `Edition::Kingdom` (王国編) | 返却値が `50` であること |
| **UT-MASTER-006** | `MasterData::getMaxLevelForEdition` | `Edition::Earth` (大地編) | 返却値が `100` であること |
| **UT-MASTER-008** | `MasterData::getSharesForEdition` | `Edition::Kingdom` (王国編) | 「フリー」「王国編固有シェア」が含まれ、「メイキング」「魔城」等が含まれないこと |
| **UT-MASTER-009** | `MasterData::getSharesForEdition` | `Edition::Earth` (大地編) | 「フリー」「メイキング」「呪われし血族」「巨塔」等が含まれること |

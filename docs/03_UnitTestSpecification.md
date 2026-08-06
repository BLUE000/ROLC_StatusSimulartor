# ROLC ステータスシミュレーター 単体テスト仕様書 (Unit Test Specification)

- **文書バージョン**: 2.2.0
- **最終更新日**: 2026-08-07
- **対応設計書**: [03_DetailedDesign.md](file:///d:/prog/C++/ROLC_StatusSimulator/docs/03_DetailedDesign.md)
- **対象システム**: ROLC ステータスシミュレーター (Qt6 / C++20)

---

## 1. 単体テストケース一覧 (Unit Test Cases)

| テストID | テスト対象クラス/関数 | テスト入力条件 | 期待値・判定基準 |
| :--- | :--- | :--- | :--- |
| **UT-CALC-005** | `CalculatorEngine::calculate` | `level = 100`, `restrictedLevel = 5` | ステータス・計算レベルが Lv5 として評価・算出されること |
| **UT-CALC-006** | `CalculatorEngine::calculate` | 物理 Min 250, Max 324, 魔法 Min 143, Max 189 | 物魔 Min が 225, 物魔 Max が 294 と正しく算出されること (`×0.575`) |
| **UT-CALC-007** | `CalculatorEngine::calculate` | DEX 133, 武器ベース 12f | チャージf が 12f (下限5fクランプ) と正しく算出されること |

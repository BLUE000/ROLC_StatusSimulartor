# ROLC ステータスシミュレーター 単体テスト仕様書 (Unit Test Specification)

- **文書バージョン**: 1.9.0
- **最終更新日**: 2026-08-06
- **対応設計書**: [03_DetailedDesign.md](file:///d:/prog/C++/ROLC_StatusSimulator/docs/03_DetailedDesign.md)
- **対象システム**: ROLC ステータスシミュレーター (Qt6 / C++20)

---

## 1. 単体テストケース一覧 (Unit Test Cases)

| テストID | テスト対象クラス/関数 | テスト入力条件 | 期待値・判定基準 |
| :--- | :--- | :--- | :--- |
| **UT-MASTER-007** | `MasterData::getTitleBonusById` | 称号「剛力」 (ID: 4) | STR 補正 +60% が正しく返却されること |
| **UT-MASTER-010** | `MasterData::getTitleBonusById` | 称号「蛮勇」 | STR/VIT 補正各 +20% が正しく返却されること |
| **UT-MASTER-011** | `MasterData::getTitleBonusById` | 称号「闘気」 | STR/DEX/VIT 補正各 +25% が正しく返却されること |
| **UT-MASTER-012** | `MasterData::getTitleBonusById` | 称号「万能」 | 全ステータス 補正各 +15% が正しく返却されること |

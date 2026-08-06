# ROLC ステータスシミュレーター 結合テスト仕様書 (Integration Test Specification)

- **文書バージョン**: 2.0.0
- **最終更新日**: 2026-08-07
- **対応設計書**: [02_BasicDesign.md](file:///d:/prog/C++/ROLC_StatusSimulator/docs/02_BasicDesign.md)
- **対象システム**: ROLC ステータスシミュレーター (Qt6 / C++20)

---

## 1. 結合テストケース一覧 (Integration Test Cases)

| テストID | 検証コンポーネント | テスト内容 | 期待動作・連携結果 |
| :--- | :--- | :--- | :--- |
| **IT-PRES-004** | `SimulatorPresenter` <-> `CalculatorEngine` | `onRestrictedLevelChanged(int lvl)` | Presenter 経由で制限レベル 5 が設定された場合、計算エンジンが Lv5 として全体再計算を行い ViewModel を更新すること |
| **IT-PRES-005** | `SimulatorPresenter` <-> `CalculatorEngine` | 物魔攻撃力計算連携 | 物理・魔法算定値から `(物理+魔法)*0.575` で算出された物魔数値が View に通知されること |

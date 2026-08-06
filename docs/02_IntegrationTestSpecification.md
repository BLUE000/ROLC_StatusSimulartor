# ROLC ステータスシミュレーター 結合テスト仕様書 (Integration Test Specification)

- **文書バージョン**: 1.9.0
- **最終更新日**: 2026-08-06
- **対応設計書**: [02_BasicDesign.md](file:///d:/prog/C++/ROLC_StatusSimulator/docs/02_BasicDesign.md)
- **対象システム**: ROLC ステータスシミュレーター (Qt6 / C++20)

---

## 1. 結合テストケース一覧 (Integration Test Cases)

| テストID | 検証コンポーネント | テスト内容 | 期待動作・連携結果 |
| :--- | :--- | :--- | :--- |
| **IT-PRES-003** | `SimulatorPresenter` <-> `MasterData` | `onRightHandTitleChanged(TitleId)` | 修正された新称号（蛮勇、胆気、万能+15%等）の補正が正しく ViewModel に反映されること |
| **IT-SEC-001** | `TrustChain` <-> `MainWindow` | `applyWatermark` 呼び出し | タイトルバー・ステータスバーにコピーライトが表示されること |

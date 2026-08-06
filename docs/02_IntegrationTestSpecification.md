# ROLC ステータスシミュレーター 結合テスト仕様書 (Integration Test Specification)

- **文書バージョン**: 1.7.0
- **最終更新日**: 2026-08-06
- **対応設計書**: [02_BasicDesign.md](file:///d:/prog/C++/ROLC_StatusSimulator/docs/02_BasicDesign.md)
- **対象システム**: ROLC ステータスシミュレーター (Qt6 / C++20)

---

## 1. テスト概要
基本設計書（`02_BasicDesign.md`）で定義されたコンポーネント間連携（TrustChain <---> MainWindow <---> Presenter <---> Model）を検証するテストケース群。

---

## 2. 結合テストケース一覧 (Integration Test Cases)

| テストID | 検証コンポーネント | テスト内容 (シグナル / イベントトリガー) | 期待動作・連携結果 |
| :--- | :--- | :--- | :--- |
| **IT-PRES-001** | `SimulatorPresenter` <-> `UserBuildState` | `onCharacterSelected(charId)` 呼び出し時の状態伝搬 | `UserBuildState` のキャラIDが更新され、連動して基本ステータス再計算シグナルが発行されること |
| **IT-PRES-003** | `SimulatorPresenter` <-> `MasterData` | `onRightHandTitleChanged(TitleId)` | 剛力(STR+60%)等の公式ステータス称号補正が正しく ViewModel に反映されること |
| **IT-PRES-006** | `SimulatorPresenter` <-> `MasterData` | `onClassSelected(ClassStage::First, SOL_ID)` 呼び出し | 2次クラスの選択可能項目が `BRV`, `CEN` のみに制限更新されること |
| **IT-PRES-008** | `SimulatorPresenter` <-> `MasterData` | `onShareCategorySelected(KingdomIndex)` 呼び出し | View のレベルSpinBox上限が 50 に設定変更され、超過値がクランプされること |
| **IT-PRES-009** | `SimulatorPresenter` <-> `CharacterWidget` | `onShareCategorySelected` 呼び出し | レベル上限提示UIラベルテキストが `[レベル上限: Lv50 (王国編)]` へ即時更新されること |
| **IT-SEC-001** | `TrustChain` <-> `MainWindow` | `applyWatermark(w, Watermarked)` 呼び出し | ウィンドウタイトルバーに `(Custom Build: ...)` が付加され、ステータスバーに著作権表示が自動描画されること |

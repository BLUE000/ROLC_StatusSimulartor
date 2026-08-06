# ROLC ステータスシミュレーター 基本設計書

- **文書バージョン**: 2.1.0
- **最終更新日**: 2026-08-07
- **対象システム**: ROLC ステータスシミュレーター (Qt6 / C++20)

---

## 1. システムアーキテクチャ概要 (MVP パターン & Restricted Level 統合)

UI層（Qt6 Widgets）とビジネスロジック（C++20 Domain Engine）を分離するため、**Model-View-Presenter (MVP)** パターンを採用する。

---

## 2. コンポーネントおよびモジュール設計

### (1) 画面ビュー層 (`App/View`) - Qt6 Widgets
- `CharacterWidget`:
  - 編区分（王国/大地）、シェアComboBox、キャラクター選択ComboBox、各クラスレベルSpinBoxに加え、**「制限レベル SpinBox / Auto CheckBox」** を提供する。
- `OutputWidget`:
  - 物理、魔法、物魔（Min - Max, 会心%, 期待値）、HP, MP, 右手/左手チャージ(f) を視覚的に整形出力する。

### (2) ドメインコア (`Lib/ROLC_Core`) - C++20 (Qt非依存)
- `CalculatorEngine`:
  - `UserBuildState` に `restrictedLevel` を追加。制限レベル指定時、`effectiveLevel = min(currentClassLevel, restrictedLevel)` を基準に全ステータス、レベル成長値、クラス履歴比率（`histLvl / effectiveLevel`）、派生攻撃力・物魔・チャージ時間を算出する。

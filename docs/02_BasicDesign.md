# ROLC ステータスシミュレーター 基本設計書

- **文書バージョン**: 1.0.1
- **最終更新日**: 2026-08-15
- **対象システム**: ROLC ステータスシミュレーター (Qt6 / C++20)

---

## 1. システムアーキテクチャ概要 (MVP パターン & Restricted Level 統合)

UI層（Qt6 Widgets）とビジネスロジック（C++20 Domain Engine）を分離するため、**Model-View-Presenter (MVP)** パターンを採用する。

---

## 2. コンポーネントおよびモジュール設計

### (1) 画面ビュー層 (`App/View`) - Qt6 Widgets
- `CharacterWidget`:
  - 編区分（王国/大地）、シェアComboBox、キャラクター選択ComboBox、各クラスレベルSpinBoxに加え、**「制限レベル SpinBox」** を提供する。
- `StatusWidget`:
  - 「基本ステータス振り分け 計算結果」グループ上部に **「ノンシェア」CheckBox** を配置。
  - Webシミュレーター仕様に完全準拠し、クラス選択状態に関わらず通常時は最大 150pt、ノンシェアチェック時は最大 140pt に連動切り替えし、残りポイントおよび各ステータス（STR〜MEN）の割り振りを制御する。
- `EquipmentWidget`:
  - **「装備称号」グループ**: 右手、左手、胴、手、足の部位別称号選択 ComboBox を提供。
  - **「スキルバフ・特殊設定」グループ**: 
    - CRDクラススキル「アンプリファイ」バフ選択 ComboBox（27種）を配置。
    - デスペレイトスキルバフ（デスペレイトアタック、デスペレイトマジック、デスペレイトアサルト）のチェックボックス群を配置。
    - オブジェクトサイズを変更せず、アンプ選択とチェックボックス群をグリッド/複数行（2行構成等）に配置することで、UIの横幅をコンパクトに抑える。
- `OutputWidget`:
  - 物理、魔法、物魔（Min - Max, 会心%, 期待値）、HP, MP, 右手/左手チャージ(f) を視覚的に整形出力する。

### (2) ドメインコア (`Lib/ROLC_Core`) - C++20 (Qt非依存)
- `CalculatorEngine`:
  - `UserBuildState` に `restrictedLevel` を追加。制限レベル指定時、`effectiveLevel = min(currentClassLevel, restrictedLevel)` を基準に全ステータス、レベル成長値、クラス履歴比率（`histLvl / effectiveLevel`）、派生攻撃力・物魔・チャージ時間を算出する。

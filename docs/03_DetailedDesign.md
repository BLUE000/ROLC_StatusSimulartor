# ROLC ステータスシミュレーター 詳細設計書

- **文書バージョン**: 2.0.0
- **最終更新日**: 2026-08-07
- **対象システム**: ROLC ステータスシミュレーター (Qt6 / C++20)

---

## 1. 制限レベル & 公式戦闘派生ステータス計算構造体

```cpp
struct UserBuildState {
    int level = 100;                 // 現在のクラス到達レベル
    int restrictedLevel = 0;         // 制限レベル (0の場合は制限なし/自動)
    // ... 他のフィールド
};

// 計算エンジン内部ロジック
int effectiveLvl = (state.restrictedLevel > 0) ? std::min(state.level, state.restrictedLevel) : state.level;

// 1. 物魔攻撃力
res.minAtkMatk = static_cast<int>(std::floor((res.minAtk + res.minMatk) * 0.575));
res.maxAtkMatk = static_cast<int>(std::floor((res.maxAtk + res.maxMatk) * 0.575));

// 2. 物理/魔法攻撃力
res.minAtk = state.equipAtk / 2 + str + dex / 2;
res.maxAtk = state.equipAtk + static_cast<int>(std::floor(str * 1.8));

res.minMatk = state.equipMatk / 2 + conStat;
res.maxMatk = state.equipMatk + static_cast<int>(std::floor(intStat * 1.8 + conStat * 0.8));

// 3. チャージフレーム
res.rightChargeFrames = std::max(5, rightWeaponBaseFrames - dex / 10);
res.leftChargeFrames = std::max(5, leftWeaponBaseFrames - dex / 10);
```

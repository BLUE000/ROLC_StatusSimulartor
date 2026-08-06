# ROLC ステータスシミュレーター 詳細設計書

- **文書バージョン**: 2.2.0
- **最終更新日**: 2026-08-07
- **対象システム**: ROLC ステータスシミュレーター (Qt6 / C++20)

---

## 1. 動的ステータス算出 & 公式戦闘派生計算構造体

```cpp
struct UserBuildState {
    int level = 100;                 // 現在のクラス到達レベル
    int restrictedLevel = 0;         // 制限レベル (0の場合は制限なし)
    // ... 他のフィールド
};

// 1. 実効レベル (effectiveLvl) の決定
int effectiveLvl = (state.restrictedLevel > 0) ? std::min(state.level, state.restrictedLevel) : state.level;

// 2. レベル依存動的基本ステータス算術式 (character_status_val)
// b = effectiveLvl, c = 各キャラクターのステータス基本係数
int calculateCharacterStatusVal(int b, int c) {
    double bPlus20 = b + 20.0;
    double bDiv10 = b / 10.0;
    double val1 = (bPlus20 * ((bPlus20 * (c * 3.0 + bDiv10 + 40.0) + 1000.0) / 1000.0));
    double val2 = (bDiv10 + 2.0) * (bDiv10 + 2.0);
    return static_cast<int>(std::floor(val1 - val2));
}

// 3. HP 動的算術式 (calc_character_HP)
// a = effectiveLvl, b = キャラクターの HP 基本係数
int calculateCharacterHp(int a, int b) {
    double aPlus20 = a + 20.0;
    double inner = (b * 60.0 + a / 2.0 + 600.0);
    double hpVal = (aPlus20 * ((aPlus20 * inner + 5000.0) / 5000.0));
    return static_cast<int>(std::floor(hpVal)) * 10;
}

// 4. 物理攻撃力 (store_atk_matk_val)
res.maxAtk = str / 2 + state.equipAtk;
int baseMinAtk = dex + state.equipAtk / 2;
int capMinAtk = str / 2 + state.equipAtk;
res.minAtk = std::min(baseMinAtk, capMinAtk);

// 5. 魔法攻撃力 (store_atk_matk_val)
res.maxMatk = intStat / 2 + state.equipMatk;
int baseMinMatk = conStat + state.equipMatk / 2;
int capMinMatk = intStat / 2 + state.equipMatk;
res.minMatk = std::min(baseMinMatk, capMinMatk);

// 6. 物魔攻撃力 (store_atk_matk_val)
double hybridFactor = state.desperateAssault ? 1.20 : 1.15;
res.maxAtkMatk = static_cast<int>(std::floor((res.maxAtk + res.maxMatk) / 2.0 * hybridFactor));
res.minAtkMatk = static_cast<int>(std::floor((res.minAtk + res.minMatk) / 2.0 * hybridFactor));

// 7. 会心率 (calc_critical_rate)
// max > min の場合は 0% (比率加算なし)、min > max (または同等) の場合は ((min/max - 1) * 50)
double critRate = (maxAtk > minAtk) ? 0.0 : (((static_cast<double>(minAtk) / maxAtk) - 1.0) * 50.0);
res.atkCriticalRate = std::min(100.0, std::max(0.0, critRate + state.equipCrit));

// 8. 期待値 (calc_expectation)
double avgAtk = (res.minAtk + res.maxAtk) / 2.0;
res.atkExpectation = std::round((avgAtk * (1.0 + (res.atkCriticalRate / 100.0) * 0.8)) * 10000.0) / 10000.0;

// 9. チャージフレーム
res.rightChargeFrames = std::max(5, 12 - dex / 10);
res.leftChargeFrames = std::max(5, 16 - dex / 10);
```

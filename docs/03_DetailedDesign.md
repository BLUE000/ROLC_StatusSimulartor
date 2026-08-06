# ROLC ステータスシミュレーター 詳細設計書

- **文書バージョン**: 1.8.0
- **最終更新日**: 2026-08-06
- **対象システム**: ROLC ステータスシミュレーター (Qt6 / C++20)

---

## 1. 編区分（エピソード） & シェア構造定義 (`MasterData.hpp`, `Types.hpp`)

```cpp
enum class Edition {
    Kingdom = 0, // 王国編 (レベル上限 50)
    Earth = 1    // 大地編 (レベル上限 100)
};
```

### 編区分別シェアグループマッピング
- **王国編 (`Edition::Kingdom`)**:
  - `フリー` (ダニル, ハビル, ？？？？ 等)
- **大地編 (`Edition::Earth`)**:
  - `フリー` (フェルテス, ガンツェ, ティンガ, ジウバ, リセル, ゼンデ, ミヒャル, オルラン, ルビ, テオラーリ, ディミート, キリサス, イレス, クァラ)
  - `メイキング`
  - `呪われし血族`
  - `砂漠に眠る王墓`
  - `雪渓に佇む魔城`
  - `炎塵に霞む遺産`
  - `雷雲を貫く巨塔`
  - `絶海に浮ぶ孤城`
  - `深奥に潜む魔宮`
  - `崖下に伏す古跡`
  - `悠久を翔る廃都`
  - `瘴気を纏う城塞`
  - `異空を望む封域`

---

## 2. C++20 データ構造体 & 状態同期仕様 (`Types.hpp`)

```cpp
namespace rolc {

class MasterData {
public:
    static std::vector<std::string> getEditions();
    static std::vector<std::string> getSharesForEdition(Edition edition);
    static int getMaxLevelForEdition(Edition edition);
};

} // namespace rolc
```

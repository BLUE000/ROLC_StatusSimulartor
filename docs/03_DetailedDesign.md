# ROLC ステータスシミュレーター 詳細設計書

- **文書バージョン**: 1.7.0
- **最終更新日**: 2026-08-06
- **対象システム**: ROLC ステータスシミュレーター (Qt6 / C++20)

---

## 1. TrustChain 組み込み & 認証フロー仕様 (`main.cpp`)

```cpp
#include "TrustChainCore.hpp"
#include "TrustChainQt.hpp"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    
    // 出自検証 & コピーライトウォーターマーク保護
    TrustChain::Core guard;
    TrustChain::AuthStatus status = guard.verifyToken();

    rolc::MainWindow mainWindow;
    TrustChain::QtHelper::applyWatermark(&mainWindow, status);

    mainWindow.show();
    return app.exec();
}
```

---

## 2. 公式ステータス称号マスターデータ構造 (`MasterData.cpp`)

```cpp
struct TitleBonus {
    int id;
    std::string name;
    bool isLimited;
    std::array<int, 6> percentBonuses; // STR, DEX, VIT, INT, CON, MEN (%)
};
```

---

## 3. マスターデータ動的派生グラフ構造 (`MasterData.hpp`)

```cpp
namespace rolc {

struct ClassDerivationRule {
    int parentClassId;
    ClassStage parentStage;
    int childClassId;
    ClassStage childStage;
};

class MasterData {
public:
    static std::vector<ClassData> getValidDerivedClasses(ClassStage targetStage, int parentClassId);
    static int getMaxLevelForShareCategory(int shareCategoryId);
};

} // namespace rolc
```

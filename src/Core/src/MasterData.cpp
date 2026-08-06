#include "MasterData.hpp"
#include <algorithm>

namespace rolc {

static const std::vector<std::string> g_shareCategories = {
    "▼シェアを選択▼",
    "フリー",
    "王国編",
    "メイキング",
    "呪われし血族",
    "砂漠に眠る王墓",
    "雪渓に佇む魔城",
    "炎塵に霞む遺産",
    "雷雲を貫く巨塔",
    "絶海に浮ぶ孤城",
    "深奥に潜む魔宮",
    "崖下に伏す古跡",
    "悠久を翔る廃都",
    "瘴気を纏う城塞",
    "異空を望む封域"
};

static const std::vector<CharacterData> g_characters = {
    {1, "フェルテス", ShareCategory::Free, 1, 8, {48, 48, 44, 47, 46, 43}, {12, 11, 8, 12, 10, 7}, {}},
    {2, "ガンツェ", ShareCategory::Free, 2, 7, {43, 54, 43, 44, 48, 42}, {11, 16, 7, 11, 12, 7}, {}},
    {3, "ティンガ", ShareCategory::Free, 2, 8, {55, 52, 44, 36, 36, 38}, {19, 16, 10, 2, 2, 5}, {}},
    {4, "ジウバ", ShareCategory::Free, 3, 10, {55, 48, 50, 38, 35, 42}, {18, 13, 15, 3, 1, 7}, {}},
    {5, "リセル", ShareCategory::Free, 3, 5, {47, 52, 40, 43, 42, 40}, {13, 15, 5, 11, 8, 5}, {}},
    {6, "ゼンデ", ShareCategory::Free, 4, 10, {44, 54, 50, 38, 36, 39}, {11, 16, 15, 3, 2, 7}, {}},
    {7, "ミヒャル", ShareCategory::Free, 4, 6, {54, 51, 46, 38, 35, 46}, {17, 16, 10, 3, 1, 10}, {}},
    {8, "オルラン", ShareCategory::Free, 5, 7, {38, 36, 43, 48, 52, 50}, {4, 2, 8, 13, 16, 15}, {}},
    {9, "ルビ", ShareCategory::Free, 5, 6, {44, 47, 40, 44, 47, 47}, {10, 12, 6, 10, 13, 10}, {}},
    {10, "テオラーリ", ShareCategory::Free, 6, 6, {39, 35, 40, 58, 44, 47}, {4, 1, 6, 20, 10, 13}, {}},
    {11, "ディミート", ShareCategory::Free, 7, 6, {42, 42, 40, 50, 50, 42}, {7, 11, 4, 15, 16, 6}, {}},
    {12, "キリサス", ShareCategory::Free, 7, 5, {46, 45, 43, 45, 46, 43}, {12, 13, 9, 12, 10, 7}, {}},
    {13, "イレス", ShareCategory::Free, 1, 7, {46, 46, 40, 48, 48, 42}, {10, 10, 6, 13, 12, 9}, {}},
    {14, "クァラ", ShareCategory::Free, 1, 7, {48, 48, 42, 46, 46, 40}, {13, 12, 9, 10, 10, 6}, {}},
    {15, "アレン", ShareCategory::CursedBlood, 6, 5, {36, 38, 44, 56, 51, 44}, {2, 3, 9, 17, 15, 11}, {}},
    {16, "マティア", ShareCategory::TombOfKing, 2, 4, {43, 58, 40, 42, 44, 42}, {8, 19, 5, 7, 11, 6}, {}},
    {17, "ベルフ", ShareCategory::TombOfKing, 3, 17, {60, 43, 43, 35, 38, 35}, {20, 10, 11, 1, 3, 1}, {}},
    {18, "リヴァン", ShareCategory::TombOfKing, 4, 10, {48, 47, 56, 35, 35, 48}, {12, 11, 18, 1, 1, 12}, {}},
    {19, "エミン", ShareCategory::TombOfKing, 5, 2, {35, 36, 38, 51, 48, 55}, {1, 2, 5, 14, 12, 17}, {}},
    {20, "アンジーニ", ShareCategory::TombOfKing, 6, 9, {43, 48, 42, 47, 55, 42}, {10, 12, 6, 12, 16, 6}, {}},
    {21, "レナ", ShareCategory::TombOfKing, 7, 8, {35, 39, 36, 50, 58, 40}, {1, 4, 2, 12, 19, 7}, {}},
    {22, "ルナン", ShareCategory::SnowCastle, 1, 10, {56, 48, 46, 40, 38, 39}, {18, 14, 10, 5, 3, 4}, {}},
    {23, "エルオ", ShareCategory::SnowCastle, 1, 4, {43, 44, 39, 48, 52, 42}, {8, 10, 4, 12, 15, 6}, {}},
    {24, "マクス", ShareCategory::SnowCastle, 2, 4, {43, 52, 39, 40, 51, 39}, {7, 16, 4, 6, 15, 4}, {}},
    {25, "ダレン", ShareCategory::SnowCastle, 3, 10, {55, 54, 40, 36, 40, 39}, {19, 15, 5, 2, 5, 4}, {}},
    {26, "メル", ShareCategory::SnowCastle, 4, 4, {47, 50, 43, 46, 48, 43}, {11, 12, 10, 9, 11, 10}, {}},
    {27, "テフィオ", ShareCategory::SnowCastle, 5, 7, {51, 48, 40, 44, 48, 40}, {15, 12, 5, 8, 13, 5}, {}},
    {28, "リスティ", ShareCategory::SnowCastle, 6, 3, {35, 39, 36, 52, 55, 42}, {1, 4, 2, 16, 18, 6}, {}},
    {29, "ルナルディ", ShareCategory::SnowCastle, 7, 10, {52, 51, 42, 43, 46, 42}, {15, 14, 6, 9, 11, 6}, {}},
    {30, "ヴェルン", ShareCategory::Heritage, 1, 5, {47, 47, 43, 47, 47, 43}, {11, 11, 7, 11, 11, 7}, {}},
    {31, "サーシャ", ShareCategory::Heritage, 1, 8, {36, 36, 40, 58, 51, 46}, {2, 2, 5, 19, 15, 11}, {}},
    {32, "デヴィス", ShareCategory::Heritage, 1, 9, {58, 43, 50, 35, 36, 50}, {17, 9, 14, 1, 2, 14}, {}},
    {33, "リンジー", ShareCategory::Heritage, 1, 8, {51, 55, 42, 38, 42, 40}, {14, 17, 6, 3, 10, 5}, {}},
    {34, "ザスター", ShareCategory::Heritage, 1, 7, {60, 54, 44, 38, 38, 40}, {20, 17, 11, 3, 3, 5}, {}},
    {35, "マルル", ShareCategory::Heritage, 1, 4, {42, 42, 38, 52, 51, 44}, {7, 8, 3, 15, 14, 8}, {}},
    {36, "ロアール", ShareCategory::ThunderTower, 2, 7, {48, 48, 40, 50, 50, 40}, {12, 12, 5, 13, 14, 5}, {}},
    {37, "デルファ", ShareCategory::ThunderTower, 3, 5, {48, 50, 42, 48, 50, 42}, {11, 13, 7, 11, 13, 7}, {}},
    {38, "アヴェンス", ShareCategory::ThunderTower, 4, 4, {46, 47, 36, 51, 50, 35}, {10, 12, 3, 14, 13, 1}, {}},
    {39, "シェスティ", ShareCategory::ThunderTower, 5, 4, {36, 36, 39, 59, 47, 46}, {2, 2, 4, 20, 13, 10}, {}},
    {40, "ホルス", ShareCategory::ThunderTower, 6, 5, {36, 36, 47, 55, 46, 54}, {2, 2, 10, 17, 11, 16}, {}},
    {41, "フィリン", ShareCategory::ThunderTower, 7, 5, {35, 39, 42, 56, 51, 50}, {1, 4, 7, 18, 15, 13}, {}},
    {42, "ジッダ", ShareCategory::IsolatedCastle, 1, 9, {54, 48, 42, 51, 44, 39}, {16, 12, 6, 15, 9, 4}, {}},
    {43, "エクトル", ShareCategory::IsolatedCastle, 1, 5, {50, 56, 40, 36, 39, 42}, {13, 18, 8, 2, 4, 7}, {}},
    {44, "レアン", ShareCategory::IsolatedCastle, 1, 7, {44, 46, 39, 46, 48, 40}, {9, 11, 4, 11, 12, 5}, {}},
    {45, "カヴェラ", ShareCategory::IsolatedCastle, 1, 7, {42, 35, 38, 56, 58, 44}, {6, 1, 3, 18, 19, 11}, {}},
    {46, "セニカ", ShareCategory::IsolatedCastle, 1, 5, {36, 42, 39, 55, 50, 44}, {2, 6, 4, 17, 15, 9}, {}},
    {47, "グリン", ShareCategory::DeepPalace, 1, 5, {51, 48, 43, 46, 44, 42}, {14, 12, 8, 11, 9, 6}, {}},
    {48, "パァプ", ShareCategory::DeepPalace, 1, 5, {46, 44, 42, 51, 48, 43}, {11, 9, 6, 14, 12, 8}, {}},
    {49, "クラニィ", ShareCategory::OldRuins, 1, 5, {44, 47, 39, 47, 48, 42}, {9, 11, 4, 11, 12, 7}, {}},
    {50, "ダニル", ShareCategory::Kingdom, 2, 7, {47, 52, 48, 35, 38, 48}, {11, 15, 13, 1, 3, 12}, {}},
    {51, "ハビル", ShareCategory::Kingdom, 3, 17, {60, 40, 58, 35, 34, 38}, {20, 5, 18, 1, 0, 4}, {}},
    {52, "？？？？", ShareCategory::Kingdom, 8, 5, {45, 51, 53, 53, 50, 53}, {10, 13, 3, 10, 13, 3}, {}},
    {53, "メイキングモード", ShareCategory::Making, 8, 0, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {}},
    {54, "STR特化", ShareCategory::Making, 8, 0, {20, 0, 20, 0, 0, 20}, {20, 0, 20, 0, 0, 20}, {}},
    {55, "テスト1", ShareCategory::Making, 8, 0, {17, 15, 20, 0, 0, 15}, {17, 15, 20, 0, 0, 15}, {}},
    {56, "シュナイン", ShareCategory::CursedBlood, 4, 8, {55, 47, 52, 39, 38, 47}, {16, 10, 15, 4, 3, 10}, {}},
};

static const std::vector<ClassData> g_firstClasses = {
    {1, "ソルジャー", "SOL", ClassStage::First, {10, 0, -5, 10, 0, -5}, {}},
    {2, "シーフ", "THF", ClassStage::First, {0, 20, 0, 0, 0, -15}, {}},
    {3, "ウォリアー", "WAR", ClassStage::First, {20, 0, 0, -15, 0, 0}, {}},
    {4, "ナイト", "KNT", ClassStage::First, {0, 0, 20, 0, -15, 0}, {}},
    {5, "クレリック", "CLC", ClassStage::First, {0, -15, 0, 0, 0, 20}, {}},
    {6, "メイジ", "MAG", ClassStage::First, {-15, 0, 0, 20, 0, 0}, {}},
    {7, "エンチャンター", "ENC", ClassStage::First, {0, 0, -15, 0, 20, 0}, {}},
    {8, "メイキング", "", ClassStage::First, {0, 0, 0, 0, 0, 0}, {}}
};

static const std::vector<ClassData> g_secondClasses = {
    {1, "ブレイブハート", "BRV", ClassStage::Second, {10, 10, -15, 10, 10, -15}, {}},
    {2, "センチュリオン", "CEN", ClassStage::Second, {5, 15, -15, 5, 15, -15}, {}},
    {3, "レンジャー", "RNG", ClassStage::Second, {0, 20, -15, 0, 20, -15}, {}},
    {4, "フェンサー", "FNC", ClassStage::Second, {20, 20, 0, -15, -15, 0}, {}},
    {5, "グラディエーター", "GLD", ClassStage::Second, {20, 0, 20, -15, -15, 0}, {}},
    {6, "クルセイダー", "CRD", ClassStage::Second, {0, -15, 20, 0, -15, 20}, {}},
    {7, "ウォーロック", "WLK", ClassStage::Second, {-15, -15, 0, 20, 0, 20}, {}},
    {8, "ソーサラー", "SOR", ClassStage::Second, {-15, 0, -15, 20, 20, 0}, {}}
};

static const std::vector<ClassData> g_thirdClasses = {
    {1, "ロード", "LOR", ClassStage::Third, {20, 0, -15, 20, 0, -15}, {}},
    {2, "コンカラー", "CNQ", ClassStage::Third, {20, -15, 0, 20, -15, 0}, {}},
    {3, "アサシン", "ASN", ClassStage::Third, {0, 30, -5, -5, 0, -10}, {}},
    {4, "ベルセルク", "BSK", ClassStage::Third, {30, 0, 0, -10, -5, -5}, {}},
    {5, "ジェネラル", "GRL", ClassStage::Third, {0, -5, 30, -5, -10, 0}, {}},
    {6, "セイント", "SNT", ClassStage::Third, {-5, -10, 0, 0, -5, 30}, {}},
    {7, "ウィザード", "WIZ", ClassStage::Third, {-10, -5, -5, 30, 0, 0}, {}},
    {8, "エレメンタリスト", "ELM", ClassStage::Third, {-5, 0, -10, 0, 30, -5}, {}}
};

static const std::vector<ClassData> g_fourthClasses = {
    {1, "ヒーロー", "HRO", ClassStage::Fourth, {15, 15, -20, 15, 15, -20}, {}},
    {2, "コンカラー", "CNQ", ClassStage::Fourth, {20, 0, -15, 20, 0, -15}, {}},
    {3, "忍者", "NIN", ClassStage::Fourth, {-10, 30, -10, -10, 30, -10}, {}},
    {4, "ソードマスター", "SDM", ClassStage::Fourth, {30, 30, -10, -10, -10, -10}, {}},
    {5, "侍", "SAM", ClassStage::Fourth, {30, -10, 30, -10, -10, -10}, {}},
    {6, "パラディン", "PLD", ClassStage::Fourth, {-10, -10, 30, -10, -10, 30}, {}},
    {7, "セージ", "SAG", ClassStage::Fourth, {-10, -10, -10, 30, -10, 30}, {}},
    {8, "スペルマスター", "SPM", ClassStage::Fourth, {-10, -10, -10, 30, 30, -10}, {}}
};

static const std::vector<ClassData> g_exClasses = {
    {1, "マーシナリー", "MRC", ClassStage::Ex, {5, 5, 0, 5, 5, 0}, {}},
    {2, "ドラグーン", "DRG", ClassStage::Ex, {10, 5, 5, 0, 0, 0}, {}},
    {3, "ベネラー", "VEN", ClassStage::Ex, {0, 10, 0, 0, 10, 0}, {}},
    {4, "クロノマスター", "CRN", ClassStage::Ex, {0, 0, 0, 10, 5, 5}, {}},
    {5, "モンク", "MNK", ClassStage::Ex, {10, 0, 0, 10, 0, 0}, {}}
};

static const std::vector<TitleBonus> g_titleBonuses = {
    {0, "なし", false, {0, 0, 0, 0, 0, 0}},

    // --- ステータス1種上昇称号 ---
    // STR
    {1, "怪力 (STR+30%)", false, {30, 0, 0, 0, 0, 0}},
    {2, "強力 (STR+40%)", false, {40, 0, 0, 0, 0, 0}},
    {3, "大力 (STR+50%)", false, {50, 0, 0, 0, 0, 0}},
    {4, "剛力 (STR+60%)", true,  {60, 0, 0, 0, 0, 0}},

    // DEX
    {5, "巧技 (DEX+30%)", false, {0, 30, 0, 0, 0, 0}},
    {6, "妙技 (DEX+40%)", false, {0, 40, 0, 0, 0, 0}},
    {7, "絶技 (DEX+50%)", false, {0, 50, 0, 0, 0, 0}},
    {8, "神技 (DEX+60%)", true,  {0, 60, 0, 0, 0, 0}},

    // VIT
    {9,  "堅強 (VIT+30%)", false, {0, 0, 30, 0, 0, 0}},
    {10, "精強 (VIT+40%)", false, {0, 0, 40, 0, 0, 0}},
    {11, "屈強 (VIT+50%)", false, {0, 0, 50, 0, 0, 0}},
    {12, "雄強 (VIT+60%)", true,  {0, 0, 60, 0, 0, 0}},

    // INT
    {13, "才知 (INT+30%)", false, {0, 0, 0, 30, 0, 0}},
    {14, "英知 (INT+40%)", false, {0, 0, 0, 40, 0, 0}},
    {15, "全知 (INT+50%)", false, {0, 0, 0, 50, 0, 0}},
    {16, "至知 (INT+60%)", true,  {0, 0, 0, 60, 0, 0}},

    // CON
    {17, "鋭意 (CON+30%)", false, {0, 0, 0, 0, 30, 0}},
    {18, "専意 (CON+40%)", false, {0, 0, 0, 0, 40, 0}},
    {19, "我意 (CON+50%)", false, {0, 0, 0, 0, 50, 0}},
    {20, "如意 (CON+60%)", true,  {0, 0, 0, 0, 60, 0}},

    // MEN
    {21, "加護 (MEN+30%)", false, {0, 0, 0, 0, 0, 30}},
    {22, "庇護 (MEN+40%)", false, {0, 0, 0, 0, 0, 40}},
    {23, "冥護 (MEN+50%)", false, {0, 0, 0, 0, 0, 50}},
    {24, "応護 (MEN+60%)", true,  {0, 0, 0, 0, 0, 60}},

    // --- ステータス2種上昇称号 ---
    // STR/DEX
    {25, "強猛 (STR/DEX+20%)", false, {20, 20, 0, 0, 0, 0}},
    {26, "獰猛 (STR/DEX+30%)", false, {30, 30, 0, 0, 0, 0}},
    {27, "驍猛 (STR/DEX+40%)", true,  {40, 40, 0, 0, 0, 0}},

    // STR/VIT
    {28, "蛮勇 (STR/VIT+20%)", false, {20, 0, 20, 0, 0, 0}},
    {29, "猛勇 (STR/VIT+30%)", false, {30, 0, 30, 0, 0, 0}},
    {30, "暴勇 (STR/VIT+40%)", true,  {40, 0, 40, 0, 0, 0}},

    // INT/CON
    {31, "俊才 (INT/CON+20%)", false, {0, 0, 0, 20, 20, 0}},
    {32, "偉才 (INT/CON+30%)", false, {0, 0, 0, 30, 30, 0}},
    {33, "鬼才 (INT/CON+40%)", true,  {0, 0, 0, 40, 40, 0}},

    // INT/MEN
    {34, "純心 (INT/MEN+20%)", false, {0, 0, 0, 20, 0, 20}},
    {35, "清心 (INT/MEN+30%)", false, {0, 0, 0, 30, 0, 30}},
    {36, "至心 (INT/MEN+40%)", true,  {0, 0, 0, 40, 0, 40}},

    // STR/INT
    {37, "制裁 (STR/INT+20%)", false, {20, 0, 0, 20, 0, 0}},
    {38, "聖裁 (STR/INT+30%)", false, {30, 0, 0, 30, 0, 0}},
    {39, "神裁 (STR/INT+40%)", true,  {40, 0, 0, 40, 0, 0}},

    // DEX/CON
    {40, "無心 (DEX/CON+20%)", false, {0, 20, 0, 0, 20, 0}},
    {41, "無我 (DEX/CON+30%)", false, {0, 30, 0, 0, 30, 0}},
    {42, "無想 (DEX/CON+40%)", true,  {0, 40, 0, 0, 40, 0}},

    // VIT/MEN
    {43, "不撓 (VIT/MEN+20%)", false, {0, 0, 20, 0, 0, 20}},
    {44, "不屈 (VIT/MEN+30%)", false, {0, 0, 30, 0, 0, 30}},
    {45, "不倒 (VIT/MEN+40%)", true,  {0, 0, 40, 0, 0, 40}},

    // --- ステータス3種上昇称号 ---
    // STR/DEX/VIT
    {46, "胆気 (STR/DEX/VIT+10%)", false, {10, 10, 10, 0, 0, 0}},
    {47, "闘気 (STR/DEX/VIT+20%)", false, {20, 20, 20, 0, 0, 0}},
    {48, "覇気 (STR/DEX/VIT+30%)", true,  {30, 30, 30, 0, 0, 0}},

    // INT/CON/MEN
    {49, "恩恵 (INT/CON/MEN+10%)", false, {0, 0, 0, 10, 10, 10}},
    {50, "慈恵 (INT/CON/MEN+20%)", false, {0, 0, 0, 20, 20, 20}},
    {51, "天恵 (INT/CON/MEN+30%)", true,  {0, 0, 0, 30, 30, 30}},

    // --- ステータス6種上昇称号 ---
    {52, "多能 (全+10%)", false, {10, 10, 10, 10, 10, 10}},
    {53, "万能 (全+15%)", false, {15, 15, 15, 15, 15, 15}},
    {54, "全能 (全+20%)", true,  {20, 20, 20, 20, 20, 20}}
};

static const std::vector<AmplifierBonus> g_amplifierBonuses = {
    {0, "なし", false, {0, 0, 0, 0, 0, 0}},
    {1, "STRI", false, {10, 0, 0, 0, 0, 0}},
    {2, "STRII", false, {20, 0, 0, 0, 0, 0}},
    {3, "STRIII", true,  {30, 0, 0, 0, 0, 0}},
    {4, "DEXI", false, {0, 10, 0, 0, 0, 0}},
    {5, "DEXII", false, {0, 20, 0, 0, 0, 0}},
    {6, "DEXIII", true,  {0, 30, 0, 0, 0, 0}},
    {7, "VITI", false, {0, 0, 10, 0, 0, 0}},
    {8, "VITII", false, {0, 0, 20, 0, 0, 0}},
    {9, "VITIII", true,  {0, 0, 30, 0, 0, 0}},
    {10, "INTI", false, {0, 0, 0, 10, 0, 0}},
    {11, "INTII", false, {0, 0, 0, 20, 0, 0}},
    {12, "INTIII", true,  {0, 0, 0, 30, 0, 0}},
    {13, "CONI", false, {0, 0, 0, 0, 10, 0}},
    {14, "CONII", false, {0, 0, 0, 0, 20, 0}},
    {15, "CONIII", true,  {0, 0, 0, 0, 30, 0}},
    {16, "MENI", false, {0, 0, 0, 0, 0, 10}},
    {17, "MENII", false, {0, 0, 0, 0, 0, 20}},
    {18, "MENIII", true,  {0, 0, 0, 0, 0, 30}}
};

const std::vector<CharacterData>& MasterData::getCharacters() {
    return g_characters;
}

const CharacterData* MasterData::getCharacterById(int id) {
    for (const auto& c : g_characters) {
        if (c.id == id) return &c;
    }
    return nullptr;
}

const std::vector<ClassData>& MasterData::getClassesByStage(ClassStage stage) {
    switch (stage) {
        case ClassStage::First: return g_firstClasses;
        case ClassStage::Second: return g_secondClasses;
        case ClassStage::Third: return g_thirdClasses;
        case ClassStage::Fourth: return g_fourthClasses;
        case ClassStage::Ex: return g_exClasses;
    }
    return g_firstClasses;
}

const ClassData* MasterData::getClassById(ClassStage stage, int id) {
    const auto& classes = getClassesByStage(stage);
    for (const auto& cl : classes) {
        if (cl.id == id) return &cl;
    }
    return nullptr;
}

std::vector<ClassData> MasterData::getValidDerivedClasses(ClassStage targetStage, int parentClassId) {
    if (parentClassId <= 0) {
        return getClassesByStage(targetStage);
    }

    std::vector<int> validIds;

    if (targetStage == ClassStage::Second) {
        switch (parentClassId) {
            case 1: validIds = {1, 2}; break; // SOL -> BRV, CEN
            case 2: validIds = {3, 4}; break; // THF -> RNG, FNC
            case 3: validIds = {4, 5}; break; // WAR -> FNC, GLD
            case 4: validIds = {5, 6}; break; // KNT -> GLD, CRD
            case 5: validIds = {6, 7}; break; // CLC -> CRD, WLK
            case 6: validIds = {7, 8}; break; // MAG -> WLK, SOR
            case 7: validIds = {8, 3}; break; // ENC -> SOR, RNG
        }
    } else if (targetStage == ClassStage::Third) {
        switch (parentClassId) {
            case 1:
            case 2: validIds = {1, 2}; break; // BRV, CEN -> LOR, CNQ
            case 3: validIds = {8, 3}; break; // RNG -> ELM, ASN
            case 4: validIds = {3, 4}; break; // FNC -> ASN, BSK
            case 5: validIds = {4, 5}; break; // GLD -> BSK, GRL
            case 6: validIds = {5, 6}; break; // CRD -> GRL, SNT
            case 7: validIds = {6, 7}; break; // WLK -> SNT, WIZ
            case 8: validIds = {7, 8}; break; // SOR -> WIZ, ELM
        }
    } else if (targetStage == ClassStage::Fourth) {
        switch (parentClassId) {
            case 1:
            case 2: validIds = {1}; break;    // LOR, CNQ -> HRO
            case 3: validIds = {2, 3}; break; // ASN -> NIN, SDM
            case 4: validIds = {3, 4}; break; // BSK -> SDM, SAM
            case 5: validIds = {4, 5}; break; // GRL -> SAM, PLD
            case 6: validIds = {5, 6}; break; // SNT -> PLD, SAG
            case 7: validIds = {6, 7}; break; // WIZ -> SAG, SPM
            case 8: validIds = {7, 2}; break; // ELM -> SPM, NIN
        }
    } else if (targetStage == ClassStage::Ex) {
        return getClassesByStage(ClassStage::Ex);
    }

    if (validIds.empty()) {
        return getClassesByStage(targetStage);
    }

    std::vector<ClassData> result;
    const auto& allClasses = getClassesByStage(targetStage);
    for (int vid : validIds) {
        for (const auto& cl : allClasses) {
            if (cl.id == vid) {
                result.push_back(cl);
                break;
            }
        }
    }
    return result;
}

const std::vector<TitleBonus>& MasterData::getTitleBonuses() {
    return g_titleBonuses;
}

const TitleBonus* MasterData::getTitleBonusById(int id) {
    for (const auto& t : g_titleBonuses) {
        if (t.id == id) return &t;
    }
    return nullptr;
}

const std::vector<AmplifierBonus>& MasterData::getAmplifierBonuses() {
    return g_amplifierBonuses;
}

const AmplifierBonus* MasterData::getAmplifierBonusById(int id) {
    for (const auto& a : g_amplifierBonuses) {
        if (a.id == id) return &a;
    }
    return nullptr;
}

static const std::vector<std::string> g_editions = {
    "王国編",
    "大地編"
};

const std::vector<std::string>& MasterData::getEditions() {
    return g_editions;
}

int MasterData::getMaxLevelForEdition(Edition edition) {
    if (edition == Edition::Kingdom) {
        return 50;
    }
    return 100;
}

const std::vector<std::string>& MasterData::getShareCategories() {
    return g_shareCategories;
}

int MasterData::getMaxLevelForShareCategory(int shareCategoryId) {
    if (shareCategoryId == 2) { // 2 = 王国編
        return 50;
    }
    return 100;
}

} // namespace rolc

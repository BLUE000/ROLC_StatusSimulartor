#ifndef ROLC_MASTERDATA_HPP
#define ROLC_MASTERDATA_HPP

#include "Types.hpp"
#include <vector>
#include <string>

namespace rolc {

class MasterData {
public:
    static const std::vector<CharacterData>& getCharacters();
    static const CharacterData* getCharacterById(int id);

    static const std::vector<ClassData>& getClassesByStage(ClassStage stage);
    static const ClassData* getClassById(ClassStage stage, int id);
    static std::vector<ClassData> getValidDerivedClasses(ClassStage targetStage, int parentClassId);

    static const std::vector<TitleBonus>& getTitleBonuses();
    static const TitleBonus* getTitleBonusById(int id);

    static const std::vector<AmplifierBonus>& getAmplifierBonuses();
    static const AmplifierBonus* getAmplifierBonusById(int id);

    static const std::vector<std::string>& getEditions();
    static int getMaxLevelForEdition(Edition edition);
    static const std::vector<std::string>& getShareCategories();
    static int getMaxLevelForShareCategory(int shareCategoryId);
};

} // namespace rolc

#endif // ROLC_MASTERDATA_HPP

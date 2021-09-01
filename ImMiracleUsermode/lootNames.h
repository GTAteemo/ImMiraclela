#pragma once
#include <string>

using namespace std;

class lootNames
{
public:

	static lootNames* lootNames::Instance();

	const char* getMatchLootName(const char* lootName);

	const char* getMatchTaskLootName(const char* lootName);

	const char* getMatchEquipmentAndPlayersName(const char* lootName);

	const char* getMatchFoodsName(const char* lootName);

	const char* getMatchMedsName(const char* lootName);

	const char* getMatchModsName(const char* lootName);

private:

};



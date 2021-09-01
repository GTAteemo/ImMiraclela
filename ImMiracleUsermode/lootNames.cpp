
#include "lootNames.h"
#include <iostream>


char* loots[] = {
"item_chain_gold",
"item_barter_building_pressuregauge",
"item_barter_building_screw",
"item_barter_electr_cablemilitary",
"item_barter_electr_flashstorage",
"item_barter_electr_gphonex",
"gyroscope",
"item_barter_electr_gyrotachometer",
"item_barter_electr_militaryboard",
"item_barter_electr_phaseantenna",
"item_barter_electr_powerfilter",
"item_barter_electr_rfidreader",
"item_barter_electr_tetriz",
"item_barter_electr_thermalmodule",
"toughbook",
"item_barter_electr_virtexprocessor",
"item_barter_electr_wirelesstranmitter",
"item_barter_energy_batterymilitary",
"item_barter_energy_cyclon",
"item_barter_energy_greenbat",
"item_barter_energy_powerbank",
"item_barter_flammable_klean",
"item_barter_flammable_thermite",
"item_barter_flam_propane",
"defibrillator",
"item_barter_medical_ibuprofen",
"item_barter_medical_nacl",
"item_barter_medical_transilluminator",
"item_barter_other_lupo",
"item_barter_other_paracord",
"item_barter_other_waterfilter",
"item_barter_valuable_1gphone",
"item_barter_valuable_beardoil",
"item_barter_valuable_bitcoin",
"item_barter_valuable_chicken",
"item_barter_valuable_elibadge",
"item_barter_valuable_gp",
"item_barter_valuable_lion",
"item_barter_valuable_prokill",
"item_barter_valuable_raven",
"item_barter_valuable_rolex",
"item_barter_valuable_skullring",
"item_barter_valuable_woodclock",
"item_barter_valuable_wyccaxe",
"item_barter_walletwz",
"item_other_powder_red",
"item_barter_electr_sas",
"item_screw_bolts",
"item_screw_nuts",
"item_barter_electr_ssd",
"item_video_card",
"item_wallet",
"item_food_jack",
"item_food_moonshine",
"item_food_purewater",
"item_thick_diary",
"item_thin_diary",
"docbag",
"intelligence",
"tech_key",
"item_keycard_interchange_store_1",
"item_keycard_interchange_store_2",
"item_keycard_lab_black",
"item_keycard_lab_blue",
"item_keycard_lab_green",
"item_keycard_lab_orange",
"item_keycard_lab_priority_med",
"item_keycard_lab_priority_serv",
"item_keycard_lab_red",
"item_keycard_lab_violet",
"item_keycard_lab_white",
"item_keycard_lab_white_sanitar",
"item_keycard_lab_yellow",
"item_key_1",
"item_key_10",
"item_key_11",
"item_key_12",
"item_key_13",
"item_key_14",
"item_key_2",
"item_key_3",
"item_key_3_red",
"item_key_4",
"item_key_5",
"item_key_6",
"item_key_7",
"item_key_7_sanitar",
"item_key_8",
"item_key_9",
"item_key_assets",
"item_vaselin_loot",
"item_sugar_loot",
"item_ibuprofen_loot",
"item_golden_star_balm_loot",
"whiskey",
"vodka",
"moonshine",
"key_card",
"keycard",
"virtexprocessor",
"gasoline",
"ironkey",
"item_alyonka_loot",
"stimulator",
"container_ammo",
"container_food",
"container_grenadebox",
"container_items_thicc",
"container_junkbox",
"container_lopouch",
"container_magbox",
"container_meds",
"container_money",
"container_smallitems",
"container_trackcase",
"container_weapon_thicc",
"container_weaponcase",
"item_barter_other_30mmround"
};


char* taskLoots[]
{
	"propane",
	"6l31",
	"request",
	"aramid",
	"valuable",
	"cooler_fan",
	"lupo",
	"capacitors",
	"car_battery",
	"batterymilitary",
	"beardoil",
	"fuelconditioner",
	"video_card",
	"kek",
	"cowboy",
	"transilluminator",
	"fuelconditioner",
	"lab",
	"stimulator",
	"syringe",
	"paracord",
	"defibrillator",
	"sparkplug",
	"cigarettes",
	"tplug",
	"beef",
	"rfidreader",
	"flashstorage",
	"quest"
};


char* equipmentAndPlayers[] =
{
	"equipment",
	"slick"
	"backpack",
	"helmet",
	"Player",
	"player",
	"weapon"
};


char* foodes[] =
{
	"food",
	"drink",
	"snacks",
	"mre",
	"juice",
	"filterbottle",
	"alcohol"
	"hotrod"
};


char* meds[] =
{
	"golden_star",
	"buprofen",
	"ifak",
	"meds",
	"vaselin",
	"container"
};


char* mods[] =
{
	"barrels",
	"bipods",
	"charges",
	"flashlights",
	"foregrips",
	"gasblock",
	"handguards",
	"launchers",
	"magazines",
	"mechanics",
	"mounts",
	"muzzle",
	"pistolgrips",
	"recievers",
	"scopes",
	"sights front",
	"sights rear",
	"silencers",
	"stocks",
	"tactical"
};

lootNames* lootNames::Instance()
{
	static lootNames instance;
	return &instance;
}

const char* lootNames::getMatchLootName(const char* lootName)
{

	for (char* loot : loots)
	{
		
		// You can filter out items here, for example:
		// if (loot_item.name.find("bitc") != std::string::npos)
		//	   loot_item.name = "bitcoin";
		// else
		//	   continue;
		
		
		if ( strstr(lootName,loot))
		{
			
			return lootName;
		}
		
	
	}


	return "miracle";
}

const char* lootNames::getMatchTaskLootName(const char* lootName)
{

	for (char* loot : taskLoots)
	{

		// You can filter out items here, for example:
		// if (loot_item.name.find("bitc") != std::string::npos)
		//	   loot_item.name = "bitcoin";
		// else
		//	   continue;


		if (strstr(lootName, loot))
		{

			return lootName;
		}


	}


	return "miracle";
}

const char* lootNames::getMatchEquipmentAndPlayersName(const char* lootName)
{
	for (char* loot : equipmentAndPlayers)
	{

		// You can filter out items here, for example:
		// if (loot_item.name.find("bitc") != std::string::npos)
		//	   loot_item.name = "bitcoin";
		// else
		//	   continue;


		if (strstr(lootName, loot))
		{

			return lootName;
		}


	}


	return "miracle";
}

const char* lootNames::getMatchFoodsName(const char* lootName)
{
	for (char* loot : foodes)
	{

		// You can filter out items here, for example:
		// if (loot_item.name.find("bitc") != std::string::npos)
		//	   loot_item.name = "bitcoin";
		// else
		//	   continue;


		if (strstr(lootName, loot))
		{

			return lootName;
		}


	}


	return "miracle";
}

const char* lootNames::getMatchMedsName(const char* lootName)
{
	for (char* loot : meds)
	{

		// You can filter out items here, for example:
		// if (loot_item.name.find("bitc") != std::string::npos)
		//	   loot_item.name = "bitcoin";
		// else
		//	   continue;


		if (strstr(lootName, loot))
		{

			return lootName;
		}


	}


	return "miracle";
}

const char* lootNames::getMatchModsName(const char* lootName)
{
	for (char* loot : mods)
	{

		// You can filter out items here, for example:
		// if (loot_item.name.find("bitc") != std::string::npos)
		//	   loot_item.name = "bitcoin";
		// else
		//	   continue;


		if (strstr(lootName, loot))
		{

			return lootName;
		}


	}


	return "miracle";
}

#pragma once
#pragma warning(disable : 4996)
#include "driver_control.hpp"
#include "math.hpp"
#include <vector>
#include <list>

#include <d3dx9math.h>
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "d3d9.lib")

class vec2
{
public:
	float x, y;
};
struct Matrix34
{
	Vector3 vec0;
	Vector3 vec1;
	Vector3 vec2;
	Vector3 vec3;

};
struct TransformAccessReadOnly
{
	ULONG64	pTransformData;
	int			index;
};
struct TransformData
{
	ULONG64 pTransformArray;
	ULONG64 pTransformIndices;
};

struct EFTOffsets
{
	ULONG64 offs_gameObjectManager = 0x156C698;

	ULONG64 gameObjectManager = 0x0;
	ULONG64 gameWorld = 0x0;
	ULONG64 localGameWorld = 0x0;
	ULONG64 fpsCamera = 0x0;
	ULONG64 OpticCamera = 0x0;


	struct
	{
		static constexpr uint64_t itemList = 0x60;
		static constexpr uint64_t registeredPlayers = 0x80;
		static constexpr uint64_t m_LocalPlayerID = 0x38;
	} localGameWorld_offsets;

	struct
	{
		static constexpr uint64_t m_pPlayerProfile = 0x0440;
		//static constexpr uint64_t movementContext = 0x38;
		static constexpr uint64_t proceduralWeaponAnimation = 0x190; //updayed 2021 8 26
		static constexpr uint64_t playerBody = 0xA8; //updated 2021 8 26
		static constexpr uint64_t m_pHealthController = 0x490; //updated 2021 8 26
		static constexpr uint64_t profile = 0x458; //updayed 2021 8 26

	} Player;
	struct
	{
		static constexpr uint64_t information = 0x28; //updated
		static constexpr uint64_t id = 0x10; //updated
	} profile;
};

struct EFTPlayer
{
	uintptr_t	 instance;
	FVector		 location;

	FVector	headPos;
	FVector Neck;

	FVector	Spine1;
	FVector Spine2;
	FVector Spine3;

	FVector Pelvis;

	FVector LThight1;
	FVector LThight2;
	FVector LCalf;
	FVector LFoot;
	FVector LToe;

	FVector RThight1;
	FVector RThight2;
	FVector RCalf;
	FVector RFoot;
	FVector RToe;

	FVector LCollarbome;
	FVector LUpperarm;
	FVector LForearm1;
	FVector LForearm2;
	FVector LForearm3;
	FVector LPalm;

	FVector RCollarbome;
	FVector RUpperarm;
	FVector RForearm1;
	FVector RForearm2;
	FVector RForearm3;
	FVector RPalm;

};

struct EFTLoot
{
	uintptr_t	instance;
	std::string name;
	FVector		origin;
};

enum BodyParts : uint64_t
{
	Head = 0x20,
	Thorax = 0x28,
	Stomach = 0x30,
	LeftArm = 0x38,
	RightArm = 0x40,
	LeftLeg = 0x48,
	RightLeg = 0x50,
	value_max
};



namespace EFTStructs
{
	struct BaseObject
	{
		ULONG64 previousObjectLink; //0x0000
		ULONG64 nextObjectLink; //0x0008
		ULONG64 object; //0x0010
	};

	struct GameObjectManager
	{
		ULONG64 lastTaggedObject; //0x0000
		ULONG64 taggedObjects; //0x0008
		ULONG64 lastActiveObject; //0x0010
		ULONG64 activeObjects; //0x0018
	}; //Size: 0x0010

	class ListInternal
	{
	public:
		char pad_0x0000[0x20]; //0x0000
		uintptr_t* firstEntry; //0x0020 
	}; //Size=0x0028

	class List
	{
	public:
		char pad_0x0000[0x10]; //0x0000
		ListInternal* listBase; //0x0010 
		__int32 itemCount; //0x0018 
	}; //Size=0x001C

	
}

class EFTData
{
public:

	ULONG process_id;
	ULONG64 unity_base;

	static EFTData* Instance();
	boolean load();
	boolean init();
	boolean read();

	EFTPlayer localPlayer;
	std::vector<EFTPlayer> players;
	std::vector<EFTLoot> loots;
	

	int playercount;

	EFTOffsets offsets;

	D3DXMATRIX viewMatrix;

	uint64_t matrix_list_base = 0;
	uint64_t dependency_index_table_base = 0;

	bool world_to_screen(FVector& point3D, vec2& point2D, D3DXMATRIX matrix);

	uint64_t GetObjectFromList(uint64_t listPtr, uint64_t lastObjectPtr, const char* objectName);

	uint64_t getbone_matrix(uint64_t instance);

	FVector EFTData::GetPosition(uint64_t transform);

	bool IsAiming(uint64_t	 instance);

	bool IsPlayer(uint64_t instance);

	float get_player_health(uint64_t entity);


	uint64_t get_mpcamera(uint64_t instance);
	D3DXMATRIX getoptic_matrix(uint64_t instance);


};

enum Bones : int
{
	HumanBase = 0,   //»ù×ø±ê

	HumanPelvis = 14,  //¹ÇÅè

	HumanLThigh1 = 15,   //×ó´óÍÈ
	HumanLThigh2 = 16,
	HumanLCalf = 17,	//×óÐ¡ÍÈ
	HumanLFoot = 18,	//×ó×ã
	HumanLToe = 19,		//×ó½ÅÖº

	HumanRThigh1 = 20,  //ÓÒ´óÍÈ
	HumanRThigh2 = 21,	
	HumanRCalf = 22,	//ÓÒÐ¡ÍÈ
	HumanRFoot = 23,	//ÓÒ×ã
	HumanRToe = 24,		//ÓÒ½ÅÖº

	HumanSpine1 = 29,	//¼¹Öù
	HumanSpine2 = 36,
	HumanSpine3 = 37,

	HumanLCollarbone = 89,  //×óËø¹Ç
	HumanLUpperarm = 90,    //×ó±Û
	HumanLForearm1 = 91,	//×óÇ°±Û
	HumanLForearm2 = 92,
	HumanLForearm3 = 93,
	HumanLPalm = 94,		//×óÊÖÕÆ

	HumanRCollarbone = 110, //ÓÒËø¹Ç
	HumanRUpperarm = 111,	//ÓÒ±Û
	HumanRForearm1 = 112,	//ÓÒÇ°±Û
	HumanRForearm2 = 113,
	HumanRForearm3 = 114,
	HumanRPalm = 115,		//ÓÒÊÖÕÆ


	HumanNeck = 132,		//¾±²¿
	HumanHead = 133			//Í·²¿
};


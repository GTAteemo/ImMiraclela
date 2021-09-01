#include "cheat.hpp"
#include <array>
#include <xmmintrin.h>
#include <emmintrin.h>

#include "visuals.hpp"
#include "overlay.h"

#include <string> 
#include <locale> 
#include <codecvt> 
#include <fstream> 

using namespace std;


EFTData* EFTData::Instance()
{
	static EFTData instance;
	return &instance;
}

boolean EFTData::load()
{
	EFTData::process_id = memory::get_process_id_by_name("EscapeFromTarkov.exe");
	EFTData::unity_base = driver_control::get_module_base_address("UnityPlayer.dll", process_id);

	if (!process_id || !unity_base)
	{
		return false;
	}
	return true;
}


bool EFTData::world_to_screen(FVector& point3D, vec2& point2D, D3DXMATRIX matrix)
{
	D3DXVECTOR3 _point3D = D3DXVECTOR3(point3D.x, point3D.z, point3D.y);

	if (EFTData::IsAiming(EFTData::localPlayer.instance) && EFTData::get_mpcamera(EFTData::localPlayer.instance))
	{
		matrix = EFTData::getoptic_matrix(EFTData::localPlayer.instance);
	}


	D3DXVECTOR3 translationVector = D3DXVECTOR3(matrix._41, matrix._42, matrix._43);
	D3DXVECTOR3 up = D3DXVECTOR3(matrix._21, matrix._22, matrix._23);
	D3DXVECTOR3 right = D3DXVECTOR3(matrix._11, matrix._12, matrix._13);

	float w = D3DXVec3Dot(&translationVector, &_point3D) + matrix._44;

	if (w < 0.098f)
		return false;

	float y = D3DXVec3Dot(&up, &_point3D) + matrix._24;
	float x = D3DXVec3Dot(&right, &_point3D) + matrix._14;

	if (EFTData::IsAiming(EFTData::localPlayer.instance) && EFTData::get_mpcamera(EFTData::localPlayer.instance))
	{
		uint64_t chain = driver_control::ReadChain(offsets.fpsCamera, { 0x30, 0x18 },process_id);

		x /= driver_control::Read<float>(chain + 0x12C,process_id);

		if (x < 2.f)
			x /= driver_control::Read<float>(chain + 0xAC,process_id);

		y /= driver_control::Read<float>(chain + 0x118,process_id);

		if (y < 2.f)
			y /= driver_control::Read<float>(chain + 0x98,process_id);
	}

	point2D.x = (1920 / 2) * (1.f + x / w);
	point2D.y = (1080 / 2) * (1.f - y / w);

	return true;
}

boolean EFTData::init()
{

	if (!process_id || !unity_base)
	{
		return false;
	}
	this->offsets.gameObjectManager = driver_control::Read<ULONG64>(unity_base + this->offsets.offs_gameObjectManager, process_id);


	auto active_objects = driver_control::Read<array<ULONG64, 2>>(this->offsets.gameObjectManager + offsetof(EFTStructs::GameObjectManager, lastActiveObject), process_id);
	if (!active_objects[0] || !active_objects[1])
	{
		return false;
	}

	if (!(this->offsets.gameWorld = EFTData::GetObjectFromList(active_objects[1], active_objects[0], "GameWorld")))
	{
		return false;
	}
	this->offsets.localGameWorld = driver_control::ReadChain(this->offsets.gameWorld, { 0x30, 0x18, 0x28 }, process_id);


	auto tagged_objects = driver_control::Read<array<ULONG64, 2>>(this->offsets.gameObjectManager + offsetof(EFTStructs::GameObjectManager, lastTaggedObject), process_id);
	if (!tagged_objects[0] || !tagged_objects[1])
	{
		return false;
	}
	if (!(this->offsets.fpsCamera = EFTData::GetObjectFromList(tagged_objects[1], tagged_objects[0], "FPS Camera")))
	{
		return false;;
	}

	return true;
}

	
boolean EFTData::read()
{

	if (!process_id || !unity_base)
	{
		return false;
	}

	

	this->players.clear();
	uint64_t onlineusers = driver_control::Read<uint64_t>(this->offsets.localGameWorld + this->offsets.localGameWorld_offsets.registeredPlayers, process_id);

	if (!onlineusers)
	{
		return false;
	}

	uint64_t list_base = driver_control::Read<uint64_t>(onlineusers + offsetof(EFTStructs::List, listBase), process_id);
	int player_count = driver_control::Read<int>(onlineusers + offsetof(EFTStructs::List, itemCount), process_id);


	if ( (player_count <= 0) || (!list_base))
	{
		return false;
	}



	constexpr auto BUFFER_SIZE = 128;
	uint64_t player_buffer[BUFFER_SIZE];

	auto player_buffer_chace = driver_control::Read<array<uint64_t, BUFFER_SIZE>>(list_base + offsetof(EFTStructs::ListInternal, firstEntry), process_id);

	for (int i = 0; i < BUFFER_SIZE; i++)
	{
		player_buffer[i] = player_buffer_chace[i];
	}




	//cout << hex << temp_matrix << endl;

	EFTPlayer player;

	//player render
	for (int i = 0; i < player_count; i++)
	{
		player.instance = player_buffer[i];

		this->playercount = player_count;





		uint64_t bone_matrix = this->getbone_matrix(player.instance);



		if (bone_matrix)
		{

			uint64_t bone = driver_control::ReadChain(bone_matrix, { 0x20, 0x10, 0x38 }, process_id);

			player.location = driver_control::Read<FVector>(bone + 0xB0, process_id);


			if (driver_control::Read<int>(player.instance + 0x18, process_id))
			{
				localPlayer = player;
				localPlayer.instance = player.instance;
				localPlayer.location = player.location;
			}

			player.headPos = GetPosition(driver_control::Read<uint64_t>(bone_matrix + (0x20 + ((ULONG64)Bones::HumanHead * 0x8)), process_id));
			player.Neck = GetPosition(driver_control::Read<uint64_t>(bone_matrix + (0x20 + ((ULONG64)Bones::HumanNeck * 0x8)), process_id));

			player.Spine1 = GetPosition(driver_control::Read<uint64_t>(bone_matrix + (0x20 + ((ULONG64)Bones::HumanSpine1 * 0x8)), process_id));
			player.Spine2 = GetPosition(driver_control::Read<uint64_t>(bone_matrix + (0x20 + ((ULONG64)Bones::HumanSpine2 * 0x8)), process_id));
			player.Spine3 = GetPosition(driver_control::Read<uint64_t>(bone_matrix + (0x20 + ((ULONG64)Bones::HumanSpine3 * 0x8)), process_id));

			player.Pelvis = GetPosition(driver_control::Read<uint64_t>(bone_matrix + (0x20 + ((ULONG64)Bones::HumanPelvis * 0x8)), process_id));

			player.LThight1 = GetPosition(driver_control::Read<uint64_t>(bone_matrix + (0x20 + ((ULONG64)Bones::HumanLThigh1 * 0x8)), process_id));
			player.LThight2 = GetPosition(driver_control::Read<uint64_t>(bone_matrix + (0x20 + ((ULONG64)Bones::HumanLThigh2 * 0x8)), process_id));
			player.LCalf = GetPosition(driver_control::Read<uint64_t>(bone_matrix + (0x20 + ((ULONG64)Bones::HumanLCalf * 0x8)), process_id));
			player.LFoot = GetPosition(driver_control::Read<uint64_t>(bone_matrix + (0x20 + ((ULONG64)Bones::HumanLFoot * 0x8)), process_id));
			player.LToe = GetPosition(driver_control::Read<uint64_t>(bone_matrix + (0x20 + ((ULONG64)Bones::HumanLToe * 0x8)), process_id));

			player.RThight1 = GetPosition(driver_control::Read<uint64_t>(bone_matrix + (0x20 + ((ULONG64)Bones::HumanRThigh1 * 0x8)), process_id));
			player.RThight2 = GetPosition(driver_control::Read<uint64_t>(bone_matrix + (0x20 + ((ULONG64)Bones::HumanRThigh2 * 0x8)), process_id));
			player.RCalf = GetPosition(driver_control::Read<uint64_t>(bone_matrix + (0x20 + ((ULONG64)Bones::HumanRCalf * 0x8)), process_id));
			player.RFoot = GetPosition(driver_control::Read<uint64_t>(bone_matrix + (0x20 + ((ULONG64)Bones::HumanRFoot * 0x8)), process_id));
			player.RToe = GetPosition(driver_control::Read<uint64_t>(bone_matrix + (0x20 + ((ULONG64)Bones::HumanRToe * 0x8)), process_id));

			player.LCollarbome = GetPosition(driver_control::Read<uint64_t>(bone_matrix + (0x20 + ((ULONG64)Bones::HumanLCollarbone * 0x8)), process_id));
			player.LUpperarm = GetPosition(driver_control::Read<uint64_t>(bone_matrix + (0x20 + ((ULONG64)Bones::HumanLUpperarm * 0x8)), process_id));
			player.LForearm1 = GetPosition(driver_control::Read<uint64_t>(bone_matrix + (0x20 + ((ULONG64)Bones::HumanLForearm1 * 0x8)), process_id));
			player.LForearm2 = GetPosition(driver_control::Read<uint64_t>(bone_matrix + (0x20 + ((ULONG64)Bones::HumanLForearm2 * 0x8)), process_id));
			player.LForearm3 = GetPosition(driver_control::Read<uint64_t>(bone_matrix + (0x20 + ((ULONG64)Bones::HumanLForearm3 * 0x8)), process_id));
			player.LPalm = GetPosition(driver_control::Read<uint64_t>(bone_matrix + (0x20 + ((ULONG64)Bones::HumanLPalm * 0x8)), process_id));

			player.RCollarbome = GetPosition(driver_control::Read<uint64_t>(bone_matrix + (0x20 + ((ULONG64)Bones::HumanRCollarbone * 0x8)), process_id));
			player.RUpperarm = GetPosition(driver_control::Read<uint64_t>(bone_matrix + (0x20 + ((ULONG64)Bones::HumanRUpperarm * 0x8)), process_id));
			player.RForearm1 = GetPosition(driver_control::Read<uint64_t>(bone_matrix + (0x20 + ((ULONG64)Bones::HumanRForearm1 * 0x8)), process_id));
			player.RForearm2 = GetPosition(driver_control::Read<uint64_t>(bone_matrix + (0x20 + ((ULONG64)Bones::HumanRForearm2 * 0x8)), process_id));
			player.RForearm3 = GetPosition(driver_control::Read<uint64_t>(bone_matrix + (0x20 + ((ULONG64)Bones::HumanRForearm3 * 0x8)), process_id));
			player.RPalm = GetPosition(driver_control::Read<uint64_t>(bone_matrix + (0x20 + ((ULONG64)Bones::HumanRPalm * 0x8)), process_id));



		}

		this->players.emplace_back(player);
	}


	this->loots.clear();
	//loot List
	uint64_t lootList = driver_control::Read<uint64_t>(this->offsets.localGameWorld + this->offsets.localGameWorld_offsets.itemList, process_id);
	if (!lootList)
	{
		return false;
	}

	uint64_t list_base_2 = driver_control::Read<uint64_t>(lootList + offsetof(EFTStructs::List, listBase), process_id);
	uint32_t loot_count = driver_control::Read<uint32_t>(lootList + offsetof(EFTStructs::List, itemCount), process_id);
	if (loot_count <= 0 || !list_base_2)
	{
		return false;
	}

	constexpr auto BUFFER_SIZE_2 = 3000;
	if (loot_count > BUFFER_SIZE_2)
	{
		std::runtime_error("Too much loot, extend buffer.");
		return false;
	}


	uint64_t loot_instance_buffer[BUFFER_SIZE_2];


	auto loot_instance_buffer_cache = driver_control::Read<array<uint64_t, BUFFER_SIZE_2>>(list_base_2 + offsetof(EFTStructs::ListInternal, firstEntry), process_id);

	for (int i = 0; i < loot_count; i++)
	{
		loot_instance_buffer[i] = loot_instance_buffer_cache[i];
	}


	EFTLoot loot_item;
	std::array<char, 64> name_buffer;
	for (uint32_t i = 0; i < loot_count; i++)
	{
		loot_item.instance = loot_instance_buffer[i];

		static std::vector<uintptr_t> offs_name{ 0x10, 0x28,0x10,0x30, 0x60 };
		if (auto name_ptr = driver_control::ReadChain(loot_item.instance, offs_name, process_id))
		{
			name_buffer = driver_control::Read<array<char, 64>>(name_ptr, process_id);
		}

		name_buffer[63] = '\0';

		loot_item.name = name_buffer.data();

		//printf("%s", loot_item.name);
		// You can filter out items here, for example:
		// if (loot_item.name.find("bitc") != std::string::npos)
		//	   loot_item.name = "bitcoin";
		// else
		//	   continue;


		static std::vector<uintptr_t> offs_origin{ 0x10, 0x30, 0x30, 0x8, 0x38 };
		if (auto addr = driver_control::ReadChain(loot_item.instance, offs_origin, process_id))
			loot_item.origin = driver_control::Read<FVector>(addr + 0xB0, process_id);

		this->loots.emplace_back(loot_item);
	}


	//view matrix
	D3DXMATRIX matrix;
	uint64_t temp = driver_control::ReadChain(this->offsets.fpsCamera, { 0x30, 0x18 }, process_id);
	D3DXMATRIX temp_matrix = driver_control::Read<D3DXMATRIX>(temp + 0xD8, process_id);

	if (!temp || !temp_matrix)
	{
		return false;
	}
	D3DXMatrixTranspose(&matrix, &temp_matrix);
	EFTData::viewMatrix = matrix;

	return true;
}



uint64_t EFTData::GetObjectFromList(uint64_t listPtr, uint64_t lastObjectPtr, const char* objectName)
{
	using EFTStructs::BaseObject;
	char name[256];
	uint64_t classNamePtr = 0x0;

	BaseObject activeObject = driver_control::Read<BaseObject>(listPtr,process_id);
	BaseObject lastObject = driver_control::Read<BaseObject>(lastObjectPtr,process_id);



	if (activeObject.object != 0x0)
	{
		while (activeObject.object != 0 && activeObject.object != lastObject.object)
		{
			classNamePtr = driver_control::Read<uint64_t>(activeObject.object + 0x60,process_id);
			auto cache = driver_control::Read<array<char,256>>(classNamePtr + 0x0, process_id);
			for (int i = 0; i < cache.size(); i++)
			{
				name[i] = cache[i];
			}
			
			if (strcmp(name, objectName) == 0)
			{
				//cout << name << endl;
				return activeObject.object;
			}
			activeObject = driver_control::Read<BaseObject>(activeObject.nextObjectLink, process_id);
		}
	}
	if (lastObject.object != 0x0)
	{
		classNamePtr = driver_control::Read<uint64_t>(lastObject.object + 0x60, process_id);
		auto cache = driver_control::Read<array<char, 256>>(classNamePtr + 0x0,process_id);
		for (int i = 0; i < cache.size(); i++)
		{
			name[i] = cache[i];
		}

		if (strcmp(name, objectName) == 0)
		{
			return lastObject.object;
		}
	}

	return uint64_t();
}





uint64_t EFTData::getbone_matrix(uint64_t instance)
{
	static std::vector<uint64_t> temp{ this->offsets.Player.playerBody, 0x28, 0x28, 0x10 };

	return driver_control::ReadChain(instance, temp,process_id);
}

bool EFTData::IsAiming(uint64_t	 instance)
{
	uint64_t m_pbreath = driver_control::ReadChain(instance, { this->offsets.Player.proceduralWeaponAnimation, 0x28 },process_id);
	return driver_control::Read<bool>(m_pbreath + 0xA0, process_id);

}


uint64_t EFTData::get_mpcamera(uint64_t instance)
{
	static std::vector<uint64_t> temp{ this->offsets.Player.proceduralWeaponAnimation, 0x88, 0x20 };

	return driver_control::ReadChain(instance, temp,process_id);
}

D3DXMATRIX EFTData::getoptic_matrix(uint64_t instance)
{
	D3DXMATRIX temp_matrix;
	D3DXMATRIX outmatrix;

	static std::vector<uint64_t> tempchain{ this->offsets.Player.proceduralWeaponAnimation,0x88, 0x20, 0x28, 0x30 };

	uint64_t temp = driver_control::ReadChain(instance, tempchain,process_id);

	//printf(("temp : 0x%X\n"), temp);
	temp_matrix = driver_control::Read<D3DMATRIX>(temp + 0x00D8, process_id);

	D3DXMatrixTranspose(&outmatrix, &temp_matrix);

	return outmatrix;
}

bool EFTData::IsPlayer(uint64_t instance)
{
	static std::vector<uint64_t> tempchain{ this->offsets.Player.profile, this->offsets.profile.information };

	uint64_t information = driver_control::ReadChain(instance, tempchain,process_id);


	if (driver_control::Read<int32_t>(information + 0x5C, process_id) != 0)
	{		
		return true;
	}
		

	return false;
}

float EFTData::get_player_health(uint64_t entity)
{
	uint64_t m_pHealthController = driver_control::Read<uint64_t>(entity + this->offsets.Player.m_pHealthController, process_id);

	uint64_t m_pHealthBody = driver_control::Read<uint64_t>(m_pHealthController + 0x20, process_id);
	uint64_t m_pBodyController = driver_control::Read<uint64_t>(m_pHealthBody + 0x18, process_id);

	float hp = 0.f;
	for (int i = 0; i <= 7; i++) {

		auto body_part = driver_control::Read<uintptr_t>(m_pBodyController + 0x30 + (i * 0x18), process_id);
		auto health_container = driver_control::Read<uintptr_t>(body_part + 0x10, process_id);
		auto health = driver_control::Read<float>(health_container + 0x10, process_id);
		auto health_max = driver_control::Read<float>(health_container + 0x14, process_id);

		health = driver_control::Read<float>(health_container + 0x10, process_id);
		hp += health;
		health = hp / health_max * 100;
	}
	return hp;
}

FVector EFTData::GetPosition(uint64_t transform)
{
	auto transform_internal = driver_control::Read<uint64_t>(transform + 0x10 , process_id);

	auto matrices = driver_control::Read<uint64_t>(transform_internal + 0x38,process_id);
	auto index = driver_control::Read<int>(transform_internal + 0x40,process_id);

	matrix_list_base = driver_control::Read<uintptr_t>((uintptr_t)(matrices + 0x18), process_id);

	dependency_index_table_base = driver_control::Read<uintptr_t>((uintptr_t)(matrices + 0x20), process_id);


	static auto get_dependency_index = [this](uint64_t base, int32_t index)
	{
		index = driver_control::Read<int32_t>((uintptr_t)(base + index * 4), process_id);
		return index;
	};

	static auto get_matrix_blob = [this](uint64_t base, uint64_t offs, float* blob, uint32_t size) {

		blob = driver_control::Read<float*>((uintptr_t)(base + offs),process_id);
		
		return blob;
	};

	int32_t index_relation = driver_control::Read<int32_t>((uintptr_t)(dependency_index_table_base + index * 4), process_id);
	//int32_t index_relation = get_dependency_index(dependency_index_table_base, index);

	FVector ret_value;
	{
		float* base_matrix3x4 = (float*)malloc(64), 
			* matrix3x4_buffer0 = (float*)((uint64_t)base_matrix3x4 + 16),
			* matrix3x4_buffer1 = (float*)((uint64_t)base_matrix3x4 + 32),
			* matrix3x4_buffer2 = (float*)((uint64_t)base_matrix3x4 + 48);

		auto base_matrix3x4_cache = driver_control::Read<array<float, 4>>(matrix_list_base + index * 48, process_id);
		for (int i = 0; i < 4; i++)
		{
			base_matrix3x4[i] = base_matrix3x4_cache[i];
		}
		// get_matrix_blob(matrix_list_base, index * 48, base_matrix3x4, 16);

		__m128 xmmword_1410D1340 = { -2.f, 2.f, -2.f, 0.f };
		__m128 xmmword_1410D1350 = { 2.f, -2.f, -2.f, 0.f };
		__m128 xmmword_1410D1360 = { -2.f, -2.f, 2.f, 0.f };

		while (index_relation >= 0)
		{
			uint32_t matrix_relation_index = 6 * index_relation;

			// paziuret kur tik 3 nureadina, ten translationas, kur 4 = quatas ir yra rotationas.

			auto matrix3x4_buffer2_cache = driver_control::Read<array<float, 4>>(matrix_list_base + 8 * matrix_relation_index, process_id);
			for (int i = 0; i < 4; i++)
			{
				matrix3x4_buffer2[i] = matrix3x4_buffer2_cache[i];
			}
			//get_matrix_blob(matrix_list_base, 8 * matrix_relation_index, matrix3x4_buffer2, 16);
			__m128 v_0 = *(__m128*)matrix3x4_buffer2;


			auto matrix3x4_buffer0_cache = driver_control::Read<array<float, 4>>(matrix_list_base + 8 * matrix_relation_index + 32, process_id);
			for (int i = 0; i < 4; i++)
			{
				matrix3x4_buffer0[i] = matrix3x4_buffer0_cache[i];
			}
			//get_matrix_blob(matrix_list_base, 8 * matrix_relation_index + 32, matrix3x4_buffer0, 16);
			__m128 v_1 = *(__m128*)matrix3x4_buffer0;


			auto matrix3x4_buffer1_cache = driver_control::Read<array<float, 4>>(matrix_list_base + 8 * matrix_relation_index + 16, process_id);
			for (int i = 0; i < 4; i++)
			{
				matrix3x4_buffer1[i] = matrix3x4_buffer1_cache[i];
			}
			//get_matrix_blob(matrix_list_base, 8 * matrix_relation_index + 16, matrix3x4_buffer1, 16);
			__m128i v9 = *(__m128i*)matrix3x4_buffer1;

			__m128* v3 = (__m128*)base_matrix3x4; // r10@1
			__m128 v10; // xmm9@2
			__m128 v11; // xmm3@2
			__m128 v12; // xmm8@2
			__m128 v13; // xmm4@2
			__m128 v14; // xmm2@2
			__m128 v15; // xmm5@2
			__m128 v16; // xmm6@2
			__m128 v17; // xmm7@2

			v10 = _mm_mul_ps(v_1, *v3);
			v11 = _mm_castsi128_ps(_mm_shuffle_epi32(v9, 0));
			v12 = _mm_castsi128_ps(_mm_shuffle_epi32(v9, 85));
			v13 = _mm_castsi128_ps(_mm_shuffle_epi32(v9, -114));
			v14 = _mm_castsi128_ps(_mm_shuffle_epi32(v9, -37));
			v15 = _mm_castsi128_ps(_mm_shuffle_epi32(v9, -86));
			v16 = _mm_castsi128_ps(_mm_shuffle_epi32(v9, 113));

			v17 = _mm_add_ps(
				_mm_add_ps(
					_mm_add_ps(
						_mm_mul_ps(
							_mm_sub_ps(
								_mm_mul_ps(_mm_mul_ps(v11, (__m128)xmmword_1410D1350), v13),
								_mm_mul_ps(_mm_mul_ps(v12, (__m128)xmmword_1410D1360), v14)),
							_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(v10), -86))),
						_mm_mul_ps(
							_mm_sub_ps(
								_mm_mul_ps(_mm_mul_ps(v15, (__m128)xmmword_1410D1360), v14),
								_mm_mul_ps(_mm_mul_ps(v11, (__m128)xmmword_1410D1340), v16)),
							_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(v10), 85)))),
					_mm_add_ps(
						_mm_mul_ps(
							_mm_sub_ps(
								_mm_mul_ps(_mm_mul_ps(v12, (__m128)xmmword_1410D1340), v16),
								_mm_mul_ps(_mm_mul_ps(v15, (__m128)xmmword_1410D1350), v13)),
							_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(v10), 0))),
						v10)),
				v_0);

			*v3 = v17;

			index_relation = driver_control::Read<int32_t>((uintptr_t)(dependency_index_table_base + index_relation * 4), process_id);
			//index_relation = get_dependency_index(dependency_index_table_base, index_relation);
		}

		ret_value = *(FVector*)base_matrix3x4;
		//free(base_matrix3x4);
		//delete[] base_matrix3x4;
		/*free(base_matrix3x4);
		free(matrix3x4_buffer0);
		free(matrix3x4_buffer1);
		free(matrix3x4_buffer2);
		delete[] base_matrix3x4;*/
		delete[] base_matrix3x4;
		
	}

	return ret_value;
}



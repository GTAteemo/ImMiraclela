#include "visuals.hpp"
#include "overlay.h"

#include "memory.hpp"
#include <string>
#include "math.hpp"
#include "lootNames.h"
#include <array>

using namespace std;





PRENDER* PRENDER::Instance()
{
	static PRENDER instance;
	return &instance;
}





//wstring=>string
std::string WString2String(const std::wstring& ws)
{
	std::string strLocale = setlocale(LC_ALL, "");
	const wchar_t* wchSrc = ws.c_str();
	size_t nDestSize = wcstombs(NULL, wchSrc, 0) + 1;
	char* chDest = new char[nDestSize];
	memset(chDest, 0, nDestSize);
	wcstombs(chDest, wchSrc, nDestSize);
	std::string strResult = chDest;
	delete[]chDest;
	setlocale(LC_ALL, strLocale.c_str());
	return strResult;
}

void drawHealth(uint64_t instance, vec2 headPos,vec2 location)
{
	float health = EFTData::Instance()->get_player_health(instance);

	int box_width = (abs(headPos.y - location.y) / 2) * 1.2;
	int box_height = headPos.y - location.y;

	D3DCOLOR color = D3DCOLOR_ARGB(80,255,0,255);

	int box_x = headPos.x - box_width / 2;
	int box_y = headPos.y - box_height;


	int healthHeight;

	GradientBox(box_x, box_y, -10, box_height, color, color, true);
	BorderedBox(box_x, box_y, box_width, box_height, 5, D3DCOLOR_ARGB(80, 0, 0, 0));

}

void PRENDER::Render()
{


	
	

	auto& local_player = EFTData::Instance()->localPlayer;


	float distance;
	float distance2;
	float MaxDrawDistance = 300.f;

	D3DCOLOR color = 0;


	if (playerALL)
	{
		

		for (auto& player : EFTData::Instance()->players)
		{
			if (!player.instance)
			{
				continue;
			}
			if (player.instance == local_player.instance)
			{
				continue;
			}


		

			//EFTData::Instance()->DrawSkeleton(player.instance);

			distance = local_player.location.Distance(player.location);

			if (distance > MaxDrawDistance)
				continue;

			if (distance <= 150.f)
				color = D3DCOLOR_ARGB(77, 0, 255, 230); //color red, if less than 150m
			else if (distance > 150.f && distance <= 380.f)
				color = D3DCOLOR_ARGB(33, 130, 236, 33); //color yellow, if less than 300m and greater than 150m

			vec2 screen_pos;

			//DrawDistance
			if (EFTData::Instance()->world_to_screen(player.location, screen_pos, EFTData::Instance()->viewMatrix))
			{

				static std::vector<uint64_t> tempchain{ EFTData::Instance()->offsets.Player.profile,EFTData::Instance()->offsets.profile.information };

				uint64_t information = driver_control::ReadChain(player.instance, tempchain, EFTData::Instance()->process_id);


				String((int)screen_pos.x, (int)screen_pos.y, color, pFont, true, "%0.2fm", distance);

				if (EFTData::Instance()->IsPlayer(player.instance))
				{
					String((int)screen_pos.x, (int)screen_pos.y + 13, color, pFont, true, "Íæ¼Ò");
				}
					
			}

			vec2 sPos;
			if (EFTData::Instance()->world_to_screen(player.headPos, sPos, EFTData::Instance()->viewMatrix))
			{

				FVector headSize = player.headPos;

				headSize.z = headSize.z + 0.1;

				vec2 headS;
				EFTData::Instance()->world_to_screen(headSize, headS, EFTData::Instance()->viewMatrix);



				double circleSize = sqrt((headS.x - sPos.x) * (headS.x - sPos.x) + (headS.y - sPos.y) * (headS.y - sPos.y));




				Circle((int)sPos.x, (int)sPos.y, circleSize, 0, 1, true, 32, D3DCOLOR_ARGB(50, 255, 255, 255));




				//draw sketelon
				D2D1_POINT_2F headPos, neckPos,
					spine1Pos, spine2Pos, spine3Pos,
					pelviesPos,
					LThight1Pos, LThight2Pos, LCalfPos, LFootPos, LToePos,
					RThight1Pos, RThight2Pos, RCalfPos, RFootPos, RToePos,
					LCoolarbomePos, LUpperarmPos, LForearm1Pos, LForearm2Pos, LForearm3Pos, LPalmPos,
					RCoolarbomePos, RUpperarmPos, RForearm1Pos, RForearm2Pos, RForearm3Pos, RPalmPos;

				WorldToScreenv2(player.headPos, headPos);
				WorldToScreenv2(player.Neck, neckPos);

				WorldToScreenv2(player.Spine1, spine1Pos);
				WorldToScreenv2(player.Spine2, spine2Pos);
				WorldToScreenv2(player.Spine3, spine3Pos);

				WorldToScreenv2(player.Pelvis, pelviesPos);

				WorldToScreenv2(player.LThight1, LThight1Pos);
				WorldToScreenv2(player.LThight2, LThight2Pos);
				WorldToScreenv2(player.LCalf, LCalfPos);
				WorldToScreenv2(player.LFoot, LFootPos);
				WorldToScreenv2(player.LToe, LToePos);

				WorldToScreenv2(player.RThight1, RThight1Pos);
				WorldToScreenv2(player.RThight2, RThight2Pos);
				WorldToScreenv2(player.RCalf, RCalfPos);
				WorldToScreenv2(player.RFoot, RFootPos);
				WorldToScreenv2(player.RToe, RToePos);

				WorldToScreenv2(player.LCollarbome, LCoolarbomePos);
				WorldToScreenv2(player.LUpperarm, LUpperarmPos);
				WorldToScreenv2(player.LForearm1, LForearm1Pos);
				WorldToScreenv2(player.LForearm2, LForearm2Pos);
				WorldToScreenv2(player.LForearm3, LForearm3Pos);
				WorldToScreenv2(player.LPalm, LPalmPos);

				WorldToScreenv2(player.RCollarbome, RCoolarbomePos);
				WorldToScreenv2(player.RUpperarm, RUpperarmPos);
				WorldToScreenv2(player.RForearm1, RForearm1Pos);
				WorldToScreenv2(player.RForearm2, RForearm2Pos);
				WorldToScreenv2(player.RForearm3, RForearm3Pos);
				WorldToScreenv2(player.RPalm, RPalmPos);


				D3DCOLOR color = D3DCOLOR_ARGB(70, 217, 0, 255);

				DrawLine2(headPos.x, headPos.y, neckPos.x, neckPos.y, 4, true, color);
				DrawLine2(neckPos.x, neckPos.y, spine1Pos.x, spine1Pos.y, 4, true, color);
				DrawLine2(spine1Pos.x, spine1Pos.y, spine2Pos.x, spine2Pos.y, 4, true, color);
				DrawLine2(spine2Pos.x, spine2Pos.y, spine3Pos.x, spine3Pos.y, 4, true, color);
				DrawLine2(spine3Pos.x, spine3Pos.y, pelviesPos.x, pelviesPos.y, 4, true, color);

				DrawLine2(pelviesPos.x, pelviesPos.y, LThight1Pos.x, LThight1Pos.y, 4, true, color);
				DrawLine2(LThight1Pos.x, LThight1Pos.y, LThight2Pos.x, LThight2Pos.y, 4, true, color);
				DrawLine2(LThight2Pos.x, LThight2Pos.y, LCalfPos.x, LCalfPos.y, 4, true, color);
				DrawLine2(LCalfPos.x, LCalfPos.y, LFootPos.x, LFootPos.y, 4, true, color);
				DrawLine2(LFootPos.x, LFootPos.y, LToePos.x, LToePos.y, 4, true, color);

				DrawLine2(pelviesPos.x, pelviesPos.y, RThight1Pos.x, RThight1Pos.y, 4, true, color);
				DrawLine2(RThight1Pos.x, RThight1Pos.y, RThight2Pos.x, RThight2Pos.y, 4, true, color);
				DrawLine2(RThight2Pos.x, RThight2Pos.y, RCalfPos.x, RCalfPos.y, 4, true, color);
				DrawLine2(RCalfPos.x, RCalfPos.y, RFootPos.x, RFootPos.y, 4, true, color);
				DrawLine2(RFootPos.x, RFootPos.y, RToePos.x, RToePos.y, 4, true, color);

				DrawLine2(spine3Pos.x, spine3Pos.y, LCoolarbomePos.x, LCoolarbomePos.y, 4, true, color);
				DrawLine2(LCoolarbomePos.x, LCoolarbomePos.y, LUpperarmPos.x, LUpperarmPos.y, 4, true, color);
				DrawLine2(LUpperarmPos.x, LUpperarmPos.y, LForearm1Pos.x, LForearm1Pos.y, 4, true, color);
				DrawLine2(LForearm1Pos.x, LForearm1Pos.y, LForearm2Pos.x, LForearm2Pos.y, 4, true, color);
				DrawLine2(LForearm2Pos.x, LForearm2Pos.y, LForearm3Pos.x, LForearm3Pos.y, 4, true, color);
				DrawLine2(LForearm3Pos.x, LForearm3Pos.y, LPalmPos.x, LPalmPos.y, 4, true, color);

				DrawLine2(spine3Pos.x, spine3Pos.y, RCoolarbomePos.x, RCoolarbomePos.y, 4, true, color);
				DrawLine2(RCoolarbomePos.x, RCoolarbomePos.y, RUpperarmPos.x, RUpperarmPos.y, 4, true, color);
				DrawLine2(RUpperarmPos.x, RUpperarmPos.y, RForearm1Pos.x, RForearm1Pos.y, 4, true, color);
				DrawLine2(RForearm1Pos.x, RForearm1Pos.y, RForearm2Pos.x, RForearm2Pos.y, 4, true, color);
				DrawLine2(RForearm2Pos.x, RForearm2Pos.y, RForearm3Pos.x, RForearm3Pos.y, 4, true, color);
				DrawLine2(RForearm3Pos.x, RForearm3Pos.y, RPalmPos.x, RPalmPos.y, 4, true, color);
			}


		}
	}

	if (playerRect)
	{


		for (auto& player : EFTData::Instance()->players)
		{
			if (!player.instance)
			{
				continue;
			}
			if (player.instance == local_player.instance)
			{
				continue;
			}


			//EFTData::Instance()->DrawSkeleton(player.instance);

			distance = local_player.location.Distance(player.location);

			if (distance > MaxDrawDistance)
				continue;

			if(distance <= 150.f)
				color = D3DCOLOR_ARGB(77, 255, 238, 0); //color red, if less than 150m
			else if (distance > 150.f && distance <= 1000.f)
				color = D3DCOLOR_ARGB(33, 130, 236, 33); //color yellow, if less than 300m and greater than 150m

			vec2 screen_pos;

			vec2 sPos;
			if (EFTData::Instance()->world_to_screen(player.location, sPos, EFTData::Instance()->viewMatrix))
			{

				FVector headSize = player.headPos;

				headSize.z = headSize.z + 0.1;

				//draw sketelon
				D2D1_POINT_2F headPos;

				WorldToScreenv2(headSize, headPos);

				int box_width = (abs(headPos.y - sPos.y) / 2) * 1.2 ;
				int box_height = headPos.y - sPos.y;


				int box_x = headPos.x - box_width/2;
				int box_y = headPos.y - box_height;


				draw_box(box_x, box_y, box_width, box_height,color);

				//BorderedBox(box_x, box_y, box_width, box_height, 5, color);
			}


		}
	}

	if (aimCurse)
	{
;

		color = D3DCOLOR_ARGB(50, 0, 0, 0);
		Circle(1920 / 2, 1080 / 2, 100, 0, 1, true, 32, D3DCOLOR_ARGB(50, 255, 255, 33));
	}
	
	if (lootAll)
	{
		for (auto& loot : EFTData::Instance()->loots)
		{
			if (!loot.instance)
			{
				continue;
			}



			distance = local_player.location.Distance(loot.origin);

			if (distance > MaxDrawDistance)
				continue;

			//if (distance <= 300.f)
			if(true)
			{
				color = D3DCOLOR_ARGB(100, 50, 255, 33); //color red, if less than 150m

				vec2 screen_pos;

				const char* pName = loot.name.c_str();


				if (taskLoot)
				{
					pName = lootNames::Instance()->getMatchTaskLootName(pName);
					if (pName == "miracle")
					{
						continue;
					}
		
				}else if (lootAll2)
				{
					pName = lootNames::Instance()->getMatchLootName(pName);
					if (pName == "miracle")
					{
						continue;
					}

				}
				else if (equipAndPlayer)
				{
					pName = lootNames::Instance()->getMatchEquipmentAndPlayersName(pName);
					if (pName == "miracle")
					{
						continue;
					}

				}
				else if (foods)
				{
					pName = lootNames::Instance()->getMatchFoodsName(pName);
					if (pName == "miracle")
					{
						continue;
					}

				}
				else if (meds)
				{
					pName = lootNames::Instance()->getMatchMedsName(pName);
					if (pName == "miracle")
					{
						continue;
					}
				}
				else if (mods)
				{
					pName = lootNames::Instance()->getMatchModsName(pName);
					if (pName == "miracle")
					{
						continue;
					}
				}

				/*cout << pName << endl;*/
				//Sleep(123);

				//DrawDistance
				if (EFTData::Instance()->world_to_screen(loot.origin, screen_pos, EFTData::Instance()->viewMatrix))
				{

					String((int)screen_pos.x, (int)screen_pos.y, color, pFont, true, "%s %0.2fm", pName, distance);

				}
			}
			//else if (distance > 150.f && distance <= 300.f)
			//	color = D3DCOLOR_ARGB(50, 205, 50, 33); //color yellow, if less than 300m and greater than 150m

			
		}
	}


}

	
bool PRENDER::WorldToScreenv2(const FVector& point3D, D2D1_POINT_2F& point2D)
{
	D3DXVECTOR3 _point3D = D3DXVECTOR3(point3D.x, point3D.z, point3D.y);

	auto& matrix = EFTData::Instance()->viewMatrix;

	if (EFTData::Instance()->IsAiming(EFTData::Instance()->localPlayer.instance) && EFTData::Instance()->get_mpcamera(EFTData::Instance()->localPlayer.instance))
	{
		matrix = EFTData::Instance()->getoptic_matrix(EFTData::Instance()->localPlayer.instance);
	}

	D3DXVECTOR3 translationVector = D3DXVECTOR3(matrix._41, matrix._42, matrix._43);
	D3DXVECTOR3 up = D3DXVECTOR3(matrix._21, matrix._22, matrix._23);
	D3DXVECTOR3 right = D3DXVECTOR3(matrix._11, matrix._12, matrix._13);

	float w = D3DXVec3Dot(&translationVector, &_point3D) + matrix._44;

	if (w < 0.098f)
		return false;

	float y = D3DXVec3Dot(&up, &_point3D) + matrix._24;
	float x = D3DXVec3Dot(&right, &_point3D) + matrix._14;


	if (EFTData::Instance()->IsAiming(EFTData::Instance()->localPlayer.instance) && EFTData::Instance()->get_mpcamera(EFTData::Instance()->localPlayer.instance))
	{
		uint64_t chain = driver_control::ReadChain(EFTData::Instance()->offsets.fpsCamera, { 0x30, 0x18 },EFTData::Instance()->process_id);

		x /= driver_control::Read<float>(chain + 0x12C, EFTData::Instance()->process_id);

		if (x < 2.f)
			x /= driver_control::Read<float>(chain + 0xAC, EFTData::Instance()->process_id);

		y /= driver_control::Read<float>(chain + 0x118, EFTData::Instance()->process_id);

		if (y < 2.f)
			y /= driver_control::Read<float>(chain + 0x98, EFTData::Instance()->process_id);
	}

	point2D.x = (1920 / 2) * (1.f + x / w);
	point2D.y = (1080 / 2) * (1.f - y / w);


	return true;
}

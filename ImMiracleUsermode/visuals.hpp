#pragma once
#include <Windows.h>
#include <d2d1.h>
#include "math.hpp"

#include "cheat.hpp"
#include <list>



class PRENDER
{
public:
	static PRENDER* Instance();


	boolean lootAll = false;
	boolean lootAll2 = true;
	boolean playerALL = true;
	boolean drawHealth = true;
	boolean playerRect = false;
	boolean aimCurse = false;

	boolean taskLoot = false;
	boolean equipAndPlayer = false;
	boolean foods = false;
	boolean meds = false;
	boolean mods = false;

	void Render();
	bool WorldToScreenv2(const FVector& point3D, D2D1_POINT_2F& point2D);



private:

};


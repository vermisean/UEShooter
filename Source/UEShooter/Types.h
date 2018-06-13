// Copyright Sean Duffy 2018

#pragma once

#include "CoreMinimal.h"

/**
 * Stores differentt enumerations
 */

UENUM()
enum class EMonsterBehaviorType
{
	// Stays in place until player spotted (Eating corpse, etc)
	Passive,
	// Guards a region until player spotted (Travelers)
	Patrolling
};

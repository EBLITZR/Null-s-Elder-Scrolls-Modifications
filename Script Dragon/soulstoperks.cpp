// Script Dragon includes.
#include "common\skyscript.h"
#include "common\obscript.h"
#include "common\types.h"
#include "common\enums.h"
#include "common\plugin.h"


// Definitions.
#define CONFIG_FILE "soulstoperks.ini"


// Costs.
int PPC_Constant = 10;
int PPC_Level = 0;
int debugMode = 0;


// GetPerkPointCost - Calculates the cost of buying a perk point.
int
GetPerkPointCost(
	)
{
	// The formula: ( PlayerLevel * PPC_Level ) + PPC_Constant = DragonSoulsRequired
	unsigned int playerLevel = Actor::GetLevel( Game::GetPlayer() );
	int cost = ( playerLevel * PPC_Level ) + PPC_Constant;

	// Ensure no negative cost, and finish up.
	if ( cost < 0 ) cost = 10;
	return cost;
}


// GetPerkCount - Borrowed from the 'Perk Reset' plugin from hitachihex.
unsigned char
GetPerkCount(
	) 
{
	return *(unsigned char*)( (DWORD)( Game::GetPlayer() ) + 0x6D1 );
}


// SetPerkCount - Borrowed from the 'Perk Reset' plugin from hitachihex.
void
SetPerkCount(
	unsigned char i_Count
	)
{
	*(unsigned char*)( (DWORD)( Game::GetPlayer() ) + 0x6D1 ) = i_Count;
}


// GetDragonSoulCount - Wrapper for getting the player's dragon soul count.
int
GetDragonSoulCount(
	) 
{
	return Actor::GetActorValue( Game::GetPlayer(), "dragonsouls" );
}


// SetDragonSoulCount - Wrapper for setting the player's dragon soul count.
void
SetDragonSoulCount(
	int i_Count
	)
{
	Actor::ModActorValue( Game::GetPlayer(), "dragonsouls", -GetPerkPointCost() );
}


// Plugin 'entry' point.
void
main(
	)
{
	// Initialization.
	BYTE keyBuyPerkPoint = IniReadInt( CONFIG_FILE, "general", "buyKey", 0xDE );
	debugMode = IniReadInt( CONFIG_FILE, "general", "debug", 0 );
	PPC_Constant = IniReadInt( CONFIG_FILE, "cost", "constant", PPC_Constant );
	PPC_Level = IniReadInt( CONFIG_FILE, "cost", "levelMult", PPC_Level );

	// Main plugin loop.
	while ( true ) {
		// Calculate them perk points.
		if ( GetKeyPressed( keyBuyPerkPoint ) ) {
			int perkPointCost = GetPerkPointCost();
			int perkCount = GetPerkCount();
			int dragonSouls = GetDragonSoulCount();

			// Can we get more perks?
			if ( perkCount > 200 ) {
				PrintNote( "Too many perks. Get rid of some and try again." );
				Wait( 1000 );
			}

			// Do we have enough souls?
			else if ( dragonSouls < perkPointCost ) {
				PrintNote( "%d dragon souls are required for more power.", perkPointCost );
				Wait( 1000 );
			}

			// All's good? Give me my perk!
			else {
				SetDragonSoulCount( dragonSouls - perkPointCost );
				SetPerkCount( perkCount + 1 );
				PrintNote( "The souls of %d dragons have granted you a new power.", perkPointCost );
				Wait( 1000 );
			}
		}

		// Must be called at the end of execution cycle.
		Wait( 0 );
	}
}
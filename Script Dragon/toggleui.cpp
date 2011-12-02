// Script Dragon includes.
#include "common\skyscript.h"
#include "common\obscript.h"
#include "common\types.h"
#include "common\enums.h"
#include "common\plugin.h"


// Definitions.
#define CONFIG_FILE "toggleui.ini"
#define ADDR_DRAW_MENUS 0x0156D500
#define ADDR_HUD_OPACITY 0x13062C7C


// SafeGetKeyPressed - Checks if the key is pressed, but won't work if in menu mode.
bool
SafeGetKeyPressed(
	BYTE i_Key
	)
{
	// Don't enable in menu mode.
	if ( Utility::IsInMenuMode() ) return false;

	// Don't enable if the key is set to 0x00.
	if ( i_Key == 0x00 ) return false;

	// Otherwise, check as normal.
	return GetKeyPressed( i_Key );
}


// SetHUDOpacity - Sets the hud opacity.
void
SetHUDOpacity(
	float i_Value
	)
{
	Utility::SetINIFloat( "fHUDOpacity", i_Value );
	*(float*)( ADDR_HUD_OPACITY ) = i_Value;
}


// SetShowUI - Enables/disables entire UI drawing.
void
SetShowUI(
	unsigned int i_Value
	)
{
	*(unsigned char*)( ADDR_DRAW_MENUS ) = i_Value;
}


// SetShowCompass - Enable/disables the rendering of the compass.
void
SetShowCompass(
	bool i_Value
	)
{
	Utility::SetINIBool( "bShowCompass:interface", i_Value );
}


// SetShowQuestMarkers - Enable/disables the rendering of compass(/map?) quest markers.
void
SetShowQuestMarkers(
	bool i_Value
	)
{
	Utility::SetINIBool( "bShowQuestMarkers:gameplay", i_Value );
}


// SetShowFloatingQuestMarkers - Enable/disables the rendering of floating quest markers.
void
SetShowFloatingQuestMarkers(
	bool i_Value
	)
{
	Utility::SetINIBool( "bShowFloatingQuestMarkers:gameplay", i_Value );
}


// Plugin entry point.
void
main(
	)
{
	//  Read initialization file.
	bool showUI = ( IniReadInt( CONFIG_FILE, "defaults", "show_ui", 1 ) != 0 );
	bool showCompass = ( IniReadInt( CONFIG_FILE, "defaults", "show_compass", 1 ) != 0 );
	bool showQuestMarkers = ( IniReadInt( CONFIG_FILE, "defaults", "show_quest_markers", 1 ) != 0 );
	bool showFloatingQuestMarkers = ( IniReadInt( CONFIG_FILE, "defaults", "show_float_quest_markers", 1 ) != 0 );
	BYTE keyToggleUI = IniReadInt( CONFIG_FILE, "keys", "toggle_ui", 0x00 );
	BYTE keyToggleCompass = IniReadInt( CONFIG_FILE, "keys", "toggle_compass", 0x00 );
	BYTE keyToggleQuestMarkers = ( IniReadInt( CONFIG_FILE, "defaults", "show_quest_markers", 1 ) != 0 );
	BYTE keyToggleFloatingQuestMarkers = IniReadInt( CONFIG_FILE, "keys", "toggle_float_quest_markers", 0x00 );

	// Keep track of the menu state.
	SetShowCompass( showCompass );
	SetShowQuestMarkers( showQuestMarkers );
	SetShowFloatingQuestMarkers( showFloatingQuestMarkers );
	SetHUDOpacity( showUI ? 1.0 : 0.0 );

	// Handle multiple keys at once.
	bool keyPressWait = false;
	
	// Main plugin loop.
	while ( true ) {
		// Wait if we just got a key event.
		if ( keyPressWait ) {
			Wait( 250 );
			keyPressWait = false;
		}

		// Enable/disable UI.
		if ( SafeGetKeyPressed( keyToggleUI ) ) {
			// Toggle menus/UI, then wait half a second.
			showUI = !showUI;
			SetHUDOpacity( showUI ? 1.0 : 0.0 );
			keyPressWait = true;
		}

		// Enable/disable compass.
		if ( SafeGetKeyPressed( keyToggleCompass ) ) {
			showCompass = !showCompass;
			SetShowCompass( showCompass );
			keyPressWait = true;
		}

		// Enable/disable quest markers.
		if ( SafeGetKeyPressed( keyToggleQuestMarkers ) ) {
			showQuestMarkers = !showQuestMarkers;
			SetShowFloatingQuestMarkers( showQuestMarkers );
			keyPressWait = true;
		}

		// Enable/disable floating quest markers.
		if ( SafeGetKeyPressed( keyToggleFloatingQuestMarkers ) ) {
			showFloatingQuestMarkers = !showFloatingQuestMarkers;
			SetShowFloatingQuestMarkers( showFloatingQuestMarkers );
			keyPressWait = true;
		}
		
		// Must be called at the end of execution cycle.
		Wait(0);
	}
}


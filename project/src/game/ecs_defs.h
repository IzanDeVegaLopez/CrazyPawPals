// This file is part of the course TPV2@UCM - Samir Genaim


#pragma once

// Components list - must have at least one element
//
#define _CMPS_LIST_ \
	TRANSFORM, \
	IMAGE, \
	KEYBOARDPLAYERCTRL, \
	MOVEMENTCONTROLLER, \
    SIMPLEMOVE, \
    HEALTH,\
	LIFETIMETIMER, \
    BULLETDATA, \
    WEAPON, \
	MANA, \
	DECK, \
	CAMERA, \
	CAMERA_FOLLOW, \
	CAMERA_CLAMP, \
	WAVEMANAGER, \
	ENEMYSTATEMACHINE, \
	RENDER_ORDERING, \
    BUTTON,\
    STOPONBORDER,\
	RECT, \
	DYN_IMAGE, \
	OFFSET_DYN_IMAGE,\
	STATEMACHINE, \
	HUD, \
	TRANSFORMLESS_DYN_IMAGE



// Groups list - must have at least one element
//
#define _GRPS_LIST_ \
	BULLET,\
	PLAYER,\
	UI,\
    WEAPONBUTTON, \
    DECKBUTTON,\
	ENEMY,\
	PLAYERBULLETS,\
    ENEMYBULLETS


// Scene list
//
#define _SCENE_LIST_ \
	GAMESCENE,\
	MAINMENUSCENE,\
	CONTROLSSCENE,\
	SELECTIONMENUSCENE

// Handlers list - must have at least one element
//
#define _HDLRS_LIST_ \
    PLAYER, \
    WAVE, \
	GAMEINFO, \
	CAMERA


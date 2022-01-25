#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include "MiniEngine.h"
#include "ExEngine.h"
#include <xaudio2.h>
#include <x3daudio.h>
#include <xaudio2fx.h>
#include "ResourceBankManager.h"

#include "FontRender.h"

#include "SkinModelRender.h"
#include "SpriteRender.h"
#include "LightManager.h"
#include "PostEffectManager.h"
#include "DirectionLight.h"
#include "PointLight.h"
#include "SpotLight.h"

#include "SoundEngine.h"
#include "SoundSource.h"
#include "EnemySEManager.h"
#include "EnemyFindManager.h"

#include "NameKey.h"
#include "InGameSoundConst.h"

#include "effect/Effect.h"
#include "effect/EffectEngine.h"

#include <list>
#include <array>
#include <vector>

const Vector3 LIGHTCAMERA_POSITION = { 300.0f, 500.0f,0.0f };
const Vector3 LIGHTCAMERA_TARGET = { 0.f,0.f,0.f };
const int CAMERA_MAX_NUM = 4;
const int WIPE_MAX_NUM = 3;

const float SOUND_SE_EXPLOSION_VOLUME = 1.0f;
const int MAX_FIND_ENEMY_NUM = 2;

const bool IS_BGM = true;
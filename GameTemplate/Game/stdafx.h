#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include "MiniEngine.h"
#include "ExEngine.h"
#include "ResourceBankManager.h"

#include "FontRender.h"

#include "SkinModelRender.h"
#include "SpriteRender.h"
#include "LightManager.h"
#include "PostEffectManager.h"
#include "DirectionLight.h"
#include "PointLight.h"
#include "SpotLight.h"

const Vector3 LIGHTCAMERA_POSITION = { 100.0f, 500.0f,100.0f };
const Vector3 LIGHTCAMERA_TARGET = { 0.f,1.f,0.f };
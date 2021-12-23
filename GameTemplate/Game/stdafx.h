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

const Vector3 LIGHTCAMERA_POSITION = { 300.0f, 500.0f,0.0f };
const Vector3 LIGHTCAMERA_TARGET = { 0.f,0.f,0.f };
const int CAMERA_MAX_NUM = 5;

#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include "MiniEngine.h"
#include "ExEngine.h"

#include "SkinModelRender.h"
#include "LightManager.h"
#include "PostEffectManager.h"
#include "DirectionLight.h"
#include "PointLight.h"
#include "SpotLight.h"

const Vector3 LIGHTCAMERA_POSITION = { 100.0f, 400.0f,300.0f };
const Vector3 LIGHTCAMERA_TARGET = { 0.f,1.f,0.f };
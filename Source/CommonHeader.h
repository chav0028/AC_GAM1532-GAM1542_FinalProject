#ifndef __CommonHeader_H__
#define __CommonHeader_H__

#include <windows.h>
#include <Shlwapi.h>//INCLUDED FOR ITS "PathFileExists" FUNCTION
#pragma comment(lib, "Shlwapi.lib")//Include the library
#include <stdio.h>
#include <map>
#include <string.h>
#define assert myassert

#include <gl/GL.h>
#include <gl/GLU.h>

#define SAFE_DELETE(x) { if(x!=nullptr){delete x; x=nullptr;} }
#define SAFE_DELETE_ARRAY(x) {if(x!=nullptr){ delete[] x; x=nullptr; }}

extern bool g_KeyStates[256];
extern bool g_OldKeyStates[256];

#include "Core/InputEvent.h"
#include "Core/GL/GLExtensions.h"

#include "Core/Utility/Helpers.h"
#include "Core/Utility/Utility.h"
#include "Core/Utility/QuickWatch.h"

#include "Core/Math/Vector.h"
#include "Core/Math/MyMatrix.h"

typedef MyMatrix mat4;
typedef Vector2 vec2;
typedef Vector3 vec3;

#include "Game/Mesh/VertexFormat.h"

#include "Libraries/lodepng/lodepng.h"
#include "Libraries/Box2D/Box2D/Box2D/Box2D.h"
#include "Libraries/cJSON/cJSON.h"

#include "Game/Box2D/Box2DWorld.h"
#include "Game/Box2D/Box2DDebugDraw.h"
#include "Game/Box2D/Box2DContactListener.h"
#include "Game/Box2D/Box2DRaycast.h"
#include "Game/Box2D/Box2DJointInfo.h"

#include "Libraries/SDL2_mixer-2.0.1/include/SDL_mixer.h"
#include "Libraries/SDL2-2.0.4/include/SDL.h"

#include "Libraries/fmod/include/fmod.hpp"
#include "Libraries/fmod/include/fmod_errors.h"
#include "Core/Sound/SoundSystem.h"
#include "Core/Sound/Sound.h"
#include "Core/Sound/AudioCue.h"

#include "Core/Utility/ShaderProgram.h"
#include "Core/Utility/TweenFuncs.h"
#include "Core/Utility/Tween.h"
#include "Core/Utility/FBODefinition.h"

#include "Game/Mesh/Mesh.h"
#include"Game/Mesh/NumberFontSystem.h"

#include "Game/Base/SceneManager.h"
#include "Game/Base/GameObject.h"
#include "Game/Base/Scene.h"
#include "Game/Base/Game.h"
#include "Game/Base/Pool.h"
#include "Game/Base/LightObject.h"
#include "Game/Base/CameraObject.h"
#include "Game/GameObjects/LivingObject.h"

#include "Game/Base/Menu/Button.h"

#include "Game/Base/Particles/Particle.h"
#include "Game/Base/Particles/ParticleEmitter.h"
#include "Game/Base/Particles/ParticleRenderer.h"
#include "Game/Base/Particles/ParticleRenderer3D.h"
#include "Game/Base/Particles/ParticleSystem.h"

#include "Game/Scenes/ThrustersGame.h"
#include "Game/Scenes/WaterScene.h"
#include "Game/Scenes/CannonScene.h"
#include "Game/Scenes/LightScene.h"
#include "Game/Scenes/LoadingSampleScene.h"

//FG-SCENES
#include "Game/Scenes/FinalProject/SplashScreen.h"
#include "Game/Scenes/FinalProject/MainMenu.h"
#include "Game/Scenes/FinalProject/ControlsScene.h"
#include "Game/Scenes/FinalProject/PauseScene.h"
#include "Game/Scenes/FinalProject/FinalGameScene.h"
#include "Game/Scenes/FinalProject/EndGame.h"


#include "Game/GameObjects/PlayerObject.h"
#include "Game/GameObjects/EnemyObject.h"
#include "Game/GameObjects/ElevatorObject.h"
#include "Game/GameObjects/PlaneRipplesObject.h"
#include"Game/GameObjects/Projectile.h"
#include "Game/GameObjects/HealthPowerUp.h"
#include "Game/GameObjects/RevolutingEnemy.h"
#include "Game/GameObjects/PrismaticEnemy.h"
#include "Game/GameObjects/EndLevelSensor.h"

#include "Game/GameObjects/FloatingMineObject.h"
#include "Game/GameObjects/FloatingPlayerObject.h"
#include "Game/GameObjects/FloatingObject.h"

#include "Game/GameObjects/Cannon.h"
#include "Game/GameObjects/CannonProjectile.h"
#include "Game/GameObjects/MovingWall.h"
#include "Game/GameObjects/ScoreSensor.h"

//FG-Menu Objects
#include "Game/GameObjects/FinalProject/Menu/BackButton.h"
#include "Game/GameObjects/FinalProject/Menu/PlayGameButton.h"
#include "Game/GameObjects/FinalProject/Menu/ControlsButton.h"
#include "Game/GameObjects/FinalProject/Menu/MainMenuButton.h"
#include "Game/GameObjects/FinalProject/Menu/PlayGameButton.h"
#include "Game/GameObjects/FinalProject/Menu/ResetButton.h"
#include "Game/GameObjects/FinalProject/Menu/QuitButton.h"

//FG-Player Objects
#include "Game/GameObjects/FinalProject/FGPlayer.h"
#include "Game/GameObjects/FinalProject/FGPlayerWheel.h"

//FG-Enviroment Objects
#include "Game/GameObjects/FinalProject/FGEndLevelSensor.h"
#include "Game/GameObjects/FinalProject/FGSlidingDoor.h"
#include "Game/GameObjects/FinalProject/FGSpikes.h"

//FG-Enemies Objects
#include "Game/GameObjects/FinalProject/FGPrismaticEnemy.h"
#include "Game/GameObjects/FinalProject/FGRevolutingEnemy.h"
#include "Game/GameObjects/FinalProject/FGInvisibleEnemy.h"

//FG-Pick Up Objects
#include "Game/GameObjects/FinalProject/FGHealthPickUp.h"
#include "Game/GameObjects/FinalProject/FGInvincibilityPickUp.h"
#include "Game/GameObjects/FinalProject/FGSpeedPickUp.h"


#endif //__CommonHeader_H__

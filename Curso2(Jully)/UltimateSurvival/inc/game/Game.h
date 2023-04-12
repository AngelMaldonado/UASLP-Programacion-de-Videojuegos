#pragma once
/**
 * @brief 
 * 
 * @author AngelMaldonado 
 * @file glapi.h
 * @link Github: https://github.com/AngelMaldonado
 * @date Tue Apr 11 2023
 */

#include <glapi/GLApi.h>
#include <game/Scene.h>

class Game {
public:
	Scene scene;
	GLApi* glApi;

	Game();
	~Game();
	void Run();
};


#include "Engine.h"

int main()
{
	Engine::Instance()->RunEngine();
	Engine::Instance()->DeleteEngineInstance();

	return 0;
}
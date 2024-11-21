#pragma once
#include "../DirectX/Render/Render.h"
//#include "../Patchables/Structs.h"
#include "../Globals.h"
#include <future>
#include <list>

// Scripts
//#include "../Scripts/Scripts.h"

using namespace std;

namespace Main {
	void Init();
	void MainWindow();
	void OverlayWindow();

	void Deinit();
}
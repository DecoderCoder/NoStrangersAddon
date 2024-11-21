#include "Main.h"
#include <stdarg.h>
#include "../Utils/Utils.h"
#include "../Utils/EzHook/EzHook.h"

void* oPlayerController_FixedUpdate = nullptr;
void* __fastcall Hooked_PlayerController_FixedUpdate(PlayerController* c) {
	CachedPlayer* player = nullptr;
	if (Global::CachedPlayers.count(c))
		player = Global::CachedPlayers[c];

	if (!player)
	{
		player = new CachedPlayer;
		Global::CachedPlayers[c] = player;
	}

	bool isDead = c->IsDead();
	if (!isDead)
	{
		player->IsDead = isDead;

		auto head = c->GetHeadTransform();
		if (head)
		{
			player->Type = c->GetPlayerType();
			auto headPos = head->GetPosition();
			player->HeadPos = headPos;
			player->HeadPosW2S = Global::WorldToScreen(headPos);
		}
	}

	return ((void* (__fastcall*)(void*))oPlayerController_FixedUpdate)(c);
}

void Main::Init()
{
	auto currentModule = (uintptr_t)GetModuleHandleA("GameAssembly.dll");
	oPlayerController_FixedUpdate = (void*)EzHook::Add((uintptr_t)(currentModule + Offset::PlayerController::Function::FixedUpdate), (uintptr_t)Hooked_PlayerController_FixedUpdate, baseAddr);

	Function::MainCamera = (void*)(currentModule + Offset::MainCamera);
	Function::WorldToScreenPoint = (void*)(currentModule + Offset::WorldToScreenPoint);
	Function::TransformGetPosition = (void*)(currentModule + Offset::TransformGetPosition);
	Function::SetWeaponTypeClientRpc = (void*)(currentModule + Offset::PlayerController::Function::SetWeaponTypeClientRpc);
	Function::TeleportPlayerToSpawnPoint = (void*)(currentModule + Offset::PlayerController::Function::TeleportPlayerToSpawnPoint);

	Global::Inited = true;
}

void Main::Deinit()
{
	if (oPlayerController_FixedUpdate)
		EzHook::Remove(oPlayerController_FixedUpdate);
}

void Main::MainWindow()
{
	ImGui::Begin("NO STRANGERS", NULL, ImGuiWindowFlags_NoCollapse);
	auto manager = Global::GetGameManager();
	if (manager && manager->LocalPlayer) {
		ImGui::Text("Press HOME to TP to spawn");

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		ImGui::Text("Walk speed");
		ImGui::SliderFloat("##walk", &Global::GetGameManager()->LocalPlayer->MoveSpeed, 2.5f, 50);
		ImGui::Text("Run speed");
		ImGui::SliderFloat("##run", &Global::GetGameManager()->LocalPlayer->RunSpeed, 6.f, 50);
		ImGui::Text("Jump height");
		ImGui::SliderFloat("##jump", &Global::GetGameManager()->LocalPlayer->JumpHeight, 0.6f, 50);

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		//if (ImGui::Button("Get Knife")) {
		//	manager->LocalPlayer->SetWeaponTypeClientRpc(WeaponTypeEnum::Knife);
		//}
		//if (ImGui::Button("Get Pistol")) {
		//	manager->LocalPlayer->SetWeaponTypeClientRpc(WeaponTypeEnum::Pistol);
		//}
		//if (ImGui::Button("Get Dynamite")) {
		//	manager->LocalPlayer->SetWeaponTypeClientRpc(WeaponTypeEnum::Dynamite);
		//}
	}


	ImGui::End();
}

void Main::OverlayWindow()
{
	Render::BeginOverlay();
	Render::Draw_Text(ImVec2(10, 0), "NO STRANGERS Addon ( https://t.me/DecoderCoder )", Color::Rainbow(), 15);

	if (Global::Inited) {
		auto manager = Global::GetGameManager();

		auto lp = manager->LocalPlayer;
		if (lp) {
			//if (GetAsyncKeyState(VK_HOME)) {
			//	lp->TeleportPlayerToSpawnPoint();
			//	Sleep(1000);
			//}

			for (const auto& obj : Global::GetGameManager()->GetPlayers()) {
				if (obj == lp)
					continue;
				auto cached = Global::CachedPlayers[obj];
				if (!cached)
					continue;
				if (obj->IsDead())
					continue;
				if (cached->HeadPosW2S.IsValid())
					Render::Draw_Circle(cached->HeadPosW2S, 5, Global::PlayerTypeColors[(int)cached->Type], 5);

			}
		}
	}

	Render::EndOverlay();
}
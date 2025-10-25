
#include "ActionGameplayTags.h"

namespace ActionGameplayTags
{
	/*input tags*/
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Move, "InputTag.Move", "Move input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Look, "InputTag.Look", "Look input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_ToggleWalk, "InputTag.ToggleWalk", "Change gait.");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Equip, "InputTag.Equip");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_UnEquip, "InputTag.UnEquip");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_LightAttack_Combo1, "InputTag.LightAttack.Combo1");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_LightAttack_Combo2, "InputTag.LightAttack.Combo2");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_LightAttack_Combo3, "InputTag.LightAttack.Combo3");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_LightAttack_Combo4, "InputTag.LightAttack.Combo4");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_LightAttack_Combo5, "InputTag.LightAttack.Combo5");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_HeavyAttack, "InputTag.HeavyAttack");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Dodge, "InputTag.Dodge");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_SwitchTarget, "InputTag.SwitchTarget");

	UE_DEFINE_GAMEPLAY_TAG(InputTag_MustBeHeld, "InputTag.MustBeHeld");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_MustBeHeld_Block, "InputTag.MustBeHeld.Block");

	UE_DEFINE_GAMEPLAY_TAG(InputTag_Toggleable, "InputTag.Toggleable");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Toggleable_TargetLock, "InputTag.Toggleable.TargetLock");
	/*Player tags*/
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Equip_Sword, "Player.Ability.Equip.Sword");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Unequip_Sword, "Player.Ability.Unequip.Sword");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_LightAttack_Sword_Combo1, "Player.Ability.LightAttack.Sword.Combo1");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_LightAttack_Sword_Combo2, "Player.Ability.LightAttack.Sword.Combo2");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_LightAttack_Sword_Combo3, "Player.Ability.LightAttack.Sword.Combo3");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_LightAttack_Sword_Combo4, "Player.Ability.LightAttack.Sword.Combo4");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_LightAttack_Sword_Combo5, "Player.Ability.LightAttack.Sword.Combo5");	
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_HeavyAttack_Sword, "Player.Ability.HeavyAttack.Sword");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_HitReact, "Player.Ability.HitReact");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Dodge, "Player.Ability.Dodge");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Block, "Player.Ability.Block");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_TargetLock, "Player.Ability.TargetLock");
	
	UE_DEFINE_GAMEPLAY_TAG(Player_Weapon_Sword, "Player.Weapon.Sword");

	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Equip, "Player.Event.Equip");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_UnEquip, "Player.Event.UnEquip");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_EnableSkillSwitch, "Player.Event.EnableSkillSwitch");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_DisableSkillSwitch, "Player.Event.DisableSkillSwitch");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_InputRelease, "Player.Event.InputRelease");

	UE_DEFINE_GAMEPLAY_TAG(Player_State_Attacking, "Player.State.Attacking");
	UE_DEFINE_GAMEPLAY_TAG(Player_State_Dodging, "Player.State.Dodging");

	UE_DEFINE_GAMEPLAY_TAG(Player_Status_ExecutingSkill, "Player.Status.ExecutingSkill");
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_EnableSkillSwitch, "Player.Status.EnableSkillSwitch");

	UE_DEFINE_GAMEPLAY_TAG(Player_Status_TargetLock, "Player.Status.TargetLock");
	
	
	/*Enemy tags*/
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Weapon_Sword, "Enemy.Weapon.Sword");

	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Equip, "Enemy.Ability.Equip");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Melee, "Enemy.Ability.Melee");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Ranged, "Enemy.Ability.Ranged");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_HitReact, "Enemy.Ability.HitReact");

	UE_DEFINE_GAMEPLAY_TAG(Enemy_Event_Equip, "Enemy.Event.Equip");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Event_UnEquip, "Enemy.Event.UnEquip");
	
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Event_FinishTaskAhead, "Enemy.Event.FinishTaskAhead");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_State_Attacking, "Enemy.State.Attacking");
	
	/*Shared tags*/
	UE_DEFINE_GAMEPLAY_TAG(Shared_Ability_Death, "Shared.Ability.Death");
	
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_MeleeHit, "Shared.Event.MeleeHit");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_HitReact, "Shared.Event.HitReact");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_HitPause, "Shared.Event.HitPause");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_BlockSuccessfully, "Shared.Event.BlockSuccessfully");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_BlockBreak, "Shared.Event.BlockBreak");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_PerfectBlockSuccessfully, "Shared.Event.PerfectBlockSuccessfully");
	

	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_Death, "Shared.Status.Death");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_DamageImmunity, "Shared.Status.DamageImmunity");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_Blocking, "Shared.Status.Blocking");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_PerfectBlocking, "Shared.Status.PerfectBlocking");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_ImmuneDamage, "Shared.Status.ImmuneDamage");
	
	UE_DEFINE_GAMEPLAY_TAG(Shared_SetByCaller_SkillDamagePercentage, "Shared.SetByCaller.SkillDamagePercentage");
	UE_DEFINE_GAMEPLAY_TAG(Shared_SetByCaller_BlockSpiritCost, "Shared.SetByCaller.BlockSpiritCost");
	UE_DEFINE_GAMEPLAY_TAG(Shared_SetByCaller_PerfectBlockSpiritGain, "Shared.SetByCaller.PerfectBlockSpiritGain");

	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_MeleeHit_Sword, "GameplayCue.MeleeHit.Sword");
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_BlockSuccessfully, "GameplayCue.BlockSuccessfully");
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_PerfectBlockSuccessfully, "GameplayCue.PerfectBlockSuccessfully");

	/*Game Data tags*/
	UE_DEFINE_GAMEPLAY_TAG(GameData_Level_ActionGameModeMap, "GameData.Level.ActionGameModeMap");
	UE_DEFINE_GAMEPLAY_TAG(GameData_Level_MainMenuMap, "GameData.Level.MainMenuMap");
}

#include "Library/StructLibrary.h"
#include "AbilitySystem/Abilities/ActionGameplayAbility.h"


bool FActionHeroAbilitySet::IsValid() const
{
	return InputTag.IsValid() && AbilityToGrant;
}

bool FActionAIWeightedSkillParameter::IsValid() const
{
	return DefaultWeight>=0.f&&CooldownTime>=0.f;
}

bool FActionAIWeightedSkillConfig::IsValid() const
{
	return (MinDistance < MaxDistance && MinDistance >= 0.f && !SkillData.IsEmpty());
}



TArray<TSubclassOf<UActionGameplayAbility>> FActionHitReactionAbility::GetAllValidAbilities() const
{
	TArray<TSubclassOf<UActionGameplayAbility>>Abilities;
	if (Default) Abilities.AddUnique(Default);
	if (LeftToRight) Abilities.AddUnique(LeftToRight);
	if (RightToLeft) Abilities.AddUnique(RightToLeft);
	if (TopToBottom) Abilities.AddUnique(TopToBottom);
	if (BottomToTop) Abilities.AddUnique(BottomToTop);
	if (FrontToBack) Abilities.AddUnique(FrontToBack);
	if (BackToFront) Abilities.AddUnique(BackToFront);
	return Abilities;
}

void FActionHitReactionAbilityContainer::GetAllValidAbilities(TArray<TSubclassOf<UActionGameplayAbility>>& Abilities)const  
{
	
	Abilities.Append(Front.GetAllValidAbilities());
	Abilities.Append(Back.GetAllValidAbilities());
	Abilities.Append(Left.GetAllValidAbilities());
	Abilities.Append(Right.GetAllValidAbilities());
	Abilities.Append(Block.GetAllValidAbilities());
	Abilities.Append(BlockBreak.GetAllValidAbilities());
}

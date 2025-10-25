#pragma once

namespace Debug
{

	static void Print(const FString& Msg,const FColor& Color=FColor::Red,int32 Inkey=-1)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, Color, Msg);
			UE_LOG(LogTemp,Warning,TEXT("%s"),*Msg);
		}
	}
}

#pragma once

#define Debug(text, time, color) if(GEngine){ GEngine->AddOnScreenDebugMessage(-1, time, color, text); }
#define Print(text) if(GEngine){ GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, text); }
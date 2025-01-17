// LeaveMeAlone Game by Netologiya. All RightsReserved.


#include "Animations/LMAReloadFinishedAnimNotify.h"
DEFINE_LOG_CATEGORY_STATIC(AnimNotify, All, All);


void ULMAReloadFinishedAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) {
	UE_LOG(AnimNotify, Warning, TEXT("AnimNotify start..."));
	OnNotifyReloadFinished.Broadcast(MeshComp);
	Super::Notify(MeshComp, Animation);
	UE_LOG(AnimNotify, Warning, TEXT("AnimNotify = true"));
}
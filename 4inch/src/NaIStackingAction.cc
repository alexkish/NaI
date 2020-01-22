#include <G4ios.hh>
#include <G4ParticleDefinition.hh>
#include <G4ParticleTypes.hh>
#include <G4Track.hh>
#include <G4Event.hh>
#include <G4VProcess.hh>
#include <G4StackManager.hh>

#include "NaIAnalysisManager.hh"

#include "NaIStackingAction.hh"

NaIStackingAction::NaIStackingAction(NaIAnalysisManager *pAnalysisManager)
{
	m_pAnalysisManager = pAnalysisManager;
}

NaIStackingAction::~NaIStackingAction()
{
}

G4ClassificationOfNewTrack
NaIStackingAction::ClassifyNewTrack(const G4Track *pTrack)
{
	G4ClassificationOfNewTrack hTrackClassification = fUrgent;
	G4double TotalEnergy 	= pTrack->GetTotalEnergy();
	G4String ParticleType	= pTrack->GetDefinition()->GetParticleType();
    G4int ParentId			= pTrack->GetParentID();

	G4String CreatorProcess;

	if(pTrack->GetCreatorProcess()!=0)
	CreatorProcess = pTrack->GetCreatorProcess()->GetProcessName();
	
	//G4cout <<"kinenergy = "<< kinenergy << G4endl;
	if(pTrack->GetDefinition()->GetParticleType() == "nucleus" && !pTrack->GetDefinition()->GetPDGStable())
	{
		if(ParentId > 0 && CreatorProcess == "RadioactiveDecay")
		//if(pTrack->GetDefinition()->GetParticleType() == "nucleus" && pTrack->GetParentID()==0)
		//if(pTrack->GetParentID() > 0 && pTrack->GetCreatorProcess()->GetProcessName() == "RadioactiveDecay")
			hTrackClassification = fPostpone;

		//if(pTrack->GetParentID() > 0 && pTrack->GetCreatorProcess()->GetProcessName() == "RadioactiveDecay")
		//if((pTrack->GetParentID() > 0 && pTrack->GetCreatorProcess()->GetProcessName() == "RadioactiveDecay") || (pTrack->GetDefinition()->GetParticleType() == "nucleus" && kinenergy==0))
		//{	hTrackClassification = fPostpone;}

	}

	return hTrackClassification;
}

void
NaIStackingAction::NewStage()
{
}

void
NaIStackingAction::PrepareNewEvent()
{ 
}









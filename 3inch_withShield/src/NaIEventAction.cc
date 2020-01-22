#include <G4Event.hh>

#include "NaIEventAction.hh"

NaIEventAction::NaIEventAction(NaIAnalysisManager *pAnalysisManager)
{
	m_pAnalysisManager = pAnalysisManager;
}

NaIEventAction::~NaIEventAction()
{
}

void
NaIEventAction::BeginOfEventAction(const G4Event *pEvent)
{
	if(pEvent->GetEventID() % 1000 == 0)
	{
		G4cout << G4endl;
		G4cout << "------ Begin event " << pEvent->GetEventID()
			<< "------" << G4endl;
	}
	
	if(m_pAnalysisManager)
		m_pAnalysisManager->BeginOfEvent(pEvent);
}

void NaIEventAction::EndOfEventAction(const G4Event *pEvent)
{
	if(m_pAnalysisManager)
		m_pAnalysisManager->EndOfEvent(pEvent);
}



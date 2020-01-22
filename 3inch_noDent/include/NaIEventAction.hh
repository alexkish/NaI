#ifndef __NaIEventAction_H__
#define __NaIEventAction_H__

#include <G4UserEventAction.hh>

#include "NaIAnalysisManager.hh"

class G4Event;

class NaIEventAction : public G4UserEventAction
{
public:
	NaIEventAction(NaIAnalysisManager *pAnalysisManager = 0);
	~NaIEventAction();

public:
	void BeginOfEventAction(const G4Event *pEvent);
	void EndOfEventAction(const G4Event *pEvent);

private:
	NaIAnalysisManager *m_pAnalysisManager;
};

#endif // __XENON10PEVENTACTION_H__


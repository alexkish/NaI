#ifndef __NaIStackingAction_H__
#define __NaIStackingAction_H__

#include <globals.hh>
#include <G4UserStackingAction.hh>

class NaIAnalysisManager;

class NaIStackingAction: public G4UserStackingAction
{
public:
	NaIStackingAction(NaIAnalysisManager *pAnalysisManager=0);
	~NaIStackingAction();
  
	virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack);
	virtual void NewStage();
	virtual void PrepareNewEvent();

private:
	NaIAnalysisManager *m_pAnalysisManager;
};

#endif // __XENON10PSTACKINGACTION_H__


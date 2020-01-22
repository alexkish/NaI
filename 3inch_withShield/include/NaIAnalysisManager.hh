#ifndef __NaIAnalysisManager_H__
#define __NaIAnalysisManager_H__

#include <globals.hh>

#include <TParameter.h>

class G4Run;
class G4Event;
class G4Step;

class TFile;
class TTree;

class NaIEventData;
class NaIPrimaryGeneratorAction;

class NaIAnalysisManager
{
public:
	NaIAnalysisManager(NaIPrimaryGeneratorAction *pPrimaryGeneratorAction);
	virtual ~NaIAnalysisManager();

public:
	virtual void BeginOfRun(const G4Run *pRun); 
	virtual void EndOfRun(const G4Run *pRun); 
	virtual void BeginOfEvent(const G4Event *pEvent); 
	virtual void EndOfEvent(const G4Event *pEvent); 
	virtual void Step(const G4Step *pStep);	

	void SetDataFilename(const G4String &hFilename) { m_hDataFilename = hFilename; }
	void SetNbEventsToSimulate(G4int iNbEventsToSimulate) { m_iNbEventsToSimulate = iNbEventsToSimulate; }

private:
	G4bool FilterEvent(NaIEventData *pEventData);

private:
	G4int m_iNaIHitsCollectionID;

	G4String m_hDataFilename;
	G4int m_iNbEventsToSimulate;

	TFile *m_pTreeFile;
	TTree *m_pTree;
	TParameter<int> *m_pNbEventsToSimulateParameter;

	NaIPrimaryGeneratorAction *m_pPrimaryGeneratorAction;

	NaIEventData *m_pEventData;
};

#endif // __XENON10PANALYSISMANAGER_H__


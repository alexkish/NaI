#ifndef __NaIRunAction_H__
#define __NaIRunAction_H__

#include <G4UserRunAction.hh>

class G4Run;

class NaIAnalysisManager;

class NaIRunAction: public G4UserRunAction
{
public:
	NaIRunAction(NaIAnalysisManager *pAnalysisManager=0);
	~NaIRunAction();

public:
	void BeginOfRunAction(const G4Run *pRun);
	void EndOfRunAction(const G4Run *pRun);

private:
	NaIAnalysisManager *m_pAnalysisManager;
};

#endif // __XENON10PRUNACTION_H__


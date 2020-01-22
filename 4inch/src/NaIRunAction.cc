#include <Randomize.hh>

#include <sys/time.h>

#include "NaIAnalysisManager.hh"

#include "NaIRunAction.hh"

NaIRunAction::NaIRunAction(NaIAnalysisManager *pAnalysisManager)
{
	m_pAnalysisManager = pAnalysisManager;
}

NaIRunAction::~NaIRunAction()
{

}

void
NaIRunAction::BeginOfRunAction(const G4Run *pRun)
{
	if(m_pAnalysisManager)
		m_pAnalysisManager->BeginOfRun(pRun);

	struct timeval hTimeValue;
	gettimeofday(&hTimeValue, NULL);

	CLHEP::HepRandom::setTheEngine(new CLHEP::DRand48Engine);
	CLHEP::HepRandom::setTheSeed(hTimeValue.tv_usec);
}

void
NaIRunAction::EndOfRunAction(const G4Run *pRun)
{
	if(m_pAnalysisManager)
		m_pAnalysisManager->EndOfRun(pRun);
}


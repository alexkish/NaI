#include <G4SDManager.hh>
#include <G4Run.hh>
#include <G4Event.hh>
#include <G4HCofThisEvent.hh>
#include <G4Step.hh>

#include <numeric>

#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TParameter.h>

#include "NaIDetectorConstruction.hh"
#include "NaIPrimaryGeneratorAction.hh"
#include "NaIEventData.hh"

#include "NaIHit.hh"


#include "NaIAnalysisManager.hh"

NaIAnalysisManager::NaIAnalysisManager(NaIPrimaryGeneratorAction *pPrimaryGeneratorAction)
{
	m_iNaIHitsCollectionID 		= -1;

	m_hDataFilename = "events.root";

	m_pPrimaryGeneratorAction = pPrimaryGeneratorAction;

	m_pEventData = new NaIEventData();
}

NaIAnalysisManager::~NaIAnalysisManager()
{
}

void
NaIAnalysisManager::BeginOfRun(const G4Run *pRun)
{
	m_pTreeFile = new TFile(m_hDataFilename.c_str(), "RECREATE", "File containing event data for NaI");
	m_pTree = new TTree("t1", "Tree containing event data for NaI");

	gROOT->ProcessLine("#include <vector>");

	m_pTree->Branch("eventid", &m_pEventData->m_iEventId, "eventid/I");
	m_pTree->Branch("nsteps", &m_pEventData->m_iNbSteps, "nsteps/I");
	m_pTree->Branch("etot", &m_pEventData->m_fTotalEnergyDeposited, "etot/F");
	m_pTree->Branch("trackid", "vector<int>", &m_pEventData->m_pTrackId);
	m_pTree->Branch("type", "vector<string>", &m_pEventData->m_pParticleType);
	m_pTree->Branch("parentid", "vector<int>", &m_pEventData->m_pParentId);
	m_pTree->Branch("parenttype", "vector<string>", &m_pEventData->m_pParentType);
	m_pTree->Branch("creaproc", "vector<string>", &m_pEventData->m_pCreatorProcess);
	m_pTree->Branch("edproc", "vector<string>", &m_pEventData->m_pDepositingProcess);
	m_pTree->Branch("xp", "vector<float>", &m_pEventData->m_pX);
	m_pTree->Branch("yp", "vector<float>", &m_pEventData->m_pY);
	m_pTree->Branch("zp", "vector<float>", &m_pEventData->m_pZ);
	m_pTree->Branch("ed", "vector<float>", &m_pEventData->m_pEnergyDeposited);
	m_pTree->Branch("time", "vector<float>", &m_pEventData->m_pTime);
	m_pTree->Branch("type_pri", "vector<string>", &m_pEventData->m_pPrimaryParticleType);
	m_pTree->Branch("xp_pri", &m_pEventData->m_fPrimaryX, "xp_pri/F");
	m_pTree->Branch("yp_pri", &m_pEventData->m_fPrimaryY, "yp_pri/F");
	m_pTree->Branch("zp_pri", &m_pEventData->m_fPrimaryZ, "zp_pri/F");

	m_pTree->SetMaxTreeSize(10e9);
	m_pTree->AutoSave();

	m_pNbEventsToSimulateParameter = new TParameter<int>("nbevents", m_iNbEventsToSimulate);
	m_pNbEventsToSimulateParameter->Write();
}

void
NaIAnalysisManager::EndOfRun(const G4Run *pRun)
{
	m_pTreeFile->Write();
	m_pTreeFile->Close();
}

void
NaIAnalysisManager::BeginOfEvent(const G4Event *pEvent)
{
	if(m_iNaIHitsCollectionID == -1)
	{
		G4SDManager *pSDManager = G4SDManager::GetSDMpointer();
		m_iNaIHitsCollectionID = pSDManager->GetCollectionID("NaIHitsCollection");
	} 
}

void
NaIAnalysisManager::EndOfEvent(const G4Event *pEvent)
{
	G4HCofThisEvent* pHCofThisEvent = pEvent->GetHCofThisEvent();
	NaIHitsCollection	* pNaIHitsCollection 	= 0;

	G4int iNbNaIHits 	= 0.;
	
	if(pHCofThisEvent)
	{
		if(m_iNaIHitsCollectionID != -1)
		{
			pNaIHitsCollection = (NaIHitsCollection *)(pHCofThisEvent->GetHC(m_iNaIHitsCollectionID));
			iNbNaIHits = (pNaIHitsCollection)?(pNaIHitsCollection->entries()):(0);
		}
	}

	if(iNbNaIHits)
	{
		m_pEventData->m_iEventId = pEvent->GetEventID();

		m_pEventData->m_pPrimaryParticleType->push_back(m_pPrimaryGeneratorAction->GetParticleTypeOfPrimary());

		m_pEventData->m_fPrimaryX = m_pPrimaryGeneratorAction->GetPositionOfPrimary().x();
		m_pEventData->m_fPrimaryY = m_pPrimaryGeneratorAction->GetPositionOfPrimary().y();
		m_pEventData->m_fPrimaryZ = m_pPrimaryGeneratorAction->GetPositionOfPrimary().z();

		G4int iNbSteps 					= 0;

		G4float fTotalEnergyDeposited 	= 0.;

		// LXe hits
		for(G4int i=0; i<iNbNaIHits; i++)
		{
			NaIHit *pHit = (*pNaIHitsCollection)[i];

			if(pHit->GetParticleType() != "opticalphoton")
			{
				m_pEventData->m_pTrackId->push_back(pHit->GetTrackId());
				m_pEventData->m_pParentId->push_back(pHit->GetParentId());

				m_pEventData->m_pParticleType->push_back(pHit->GetParticleType());
				m_pEventData->m_pParentType->push_back(pHit->GetParentType());
				m_pEventData->m_pCreatorProcess->push_back(pHit->GetCreatorProcess());
				m_pEventData->m_pDepositingProcess->push_back(pHit->GetDepositingProcess());

				m_pEventData->m_pX->push_back(pHit->GetPosition().x()/mm);
				m_pEventData->m_pY->push_back(pHit->GetPosition().y()/mm);
				m_pEventData->m_pZ->push_back(pHit->GetPosition().z()/mm);

				fTotalEnergyDeposited += pHit->GetEnergyDeposited()/keV;
				m_pEventData->m_pEnergyDeposited->push_back(pHit->GetEnergyDeposited()/keV);

				m_pEventData->m_pKineticEnergy->push_back(pHit->GetKineticEnergy()/keV);
				m_pEventData->m_pTime->push_back(pHit->GetTime()/second);

				iNbSteps++;
			}
		};

		m_pEventData->m_iNbSteps 			= iNbSteps;
		m_pEventData->m_fTotalEnergyDeposited = fTotalEnergyDeposited;


		//m_pEventData->m_pPmtHits->resize(iNbTopPmts+iNbBottomPmts+iNbTopVetoPmts+iNbBottomVetoPmts, 0);

/*		// Pmt hits
		for(G4int i=0; i<iNbPmtHits; i++)
			(*(m_pEventData->m_pPmtHits))[(*pPmtHitsCollection)[i]->GetPmtNb()]++;

		m_pEventData->m_iNbTopPmtHits =
			accumulate(m_pEventData->m_pPmtHits->begin(), m_pEventData->m_pPmtHits->begin()+iNbTopPmts, 0);
		m_pEventData->m_iNbBottomPmtHits =
			accumulate(m_pEventData->m_pPmtHits->begin()+iNbTopPmts, m_pEventData->m_pPmtHits->end(), 0);
*/
		if(fTotalEnergyDeposited>0.)
		m_pTree->Fill();


		m_pEventData->Clear();
	}
}

void
NaIAnalysisManager::Step(const G4Step *pStep)
{
/*	G4StepPoint* point1 = pStep->GetPreStepPoint();
    G4StepPoint* point2 = pStep->GetPostStepPoint();

	G4TouchableHandle touch1 	= point1->GetTouchableHandle();
	G4VPhysicalVolume *volume1 	= touch1->GetVolume();
	G4String name1 = volume1->GetName();
	
	G4cout << name1 << G4endl; 
*/
}

/*
G4bool
NaIAnalysisManager::FilterEvent(NaIEventData *pEventData)
{
	G4double dEnergyDepositedSensitiveRegion = 0.;

	vector<float> *pX = pEventData->m_pX;
	vector<float> *pY = pEventData->m_pY;
	vector<float> *pZ = pEventData->m_pZ;
	vector<float> *pEnergyDeposited = pEventData->m_pEnergyDeposited;

	const G4double dDriftLength = NaIDetectorConstruction::GetGeometryParameter("DriftLength");
	const G4double dRadius = NaIDetectorConstruction::GetGeometryParameter("TeflonCylinderInnerRadius");

	for(G4int i=0; i<pEnergyDeposited->size(); i++)
	{
		if((*pZ)[i] < 0. && (*pZ)[i] > -dDriftLength && std::sqrt((*pX)[i]*(*pX)[i] + (*pY)[i]*(*pY)[i]) < dRadius)
			dEnergyDepositedSensitiveRegion += (*pEnergyDeposited)[i];
	}

	if(dEnergyDepositedSensitiveRegion > 0.)
		return false;
	else
		return true;
}
*/
	

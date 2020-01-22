#ifndef __NaISensitiveDetector_H__
#define __NaISensitiveDetector_H__

#include <G4VSensitiveDetector.hh>

#include "NaIHit.hh"

class G4Step;
class G4HCofThisEvent;

class NaISensitiveDetector: public G4VSensitiveDetector
{
public:
	NaISensitiveDetector(G4String hName);
	~NaISensitiveDetector();

	void Initialize(G4HCofThisEvent *pHitsCollectionOfThisEvent);
	G4bool ProcessHits(G4Step *pStep, G4TouchableHistory *pHistory);
	void EndOfEvent(G4HCofThisEvent *pHitsCollectionOfThisEvent);

private:
	NaIHitsCollection* m_pNaIHitsCollection;

	map<int,G4String> m_hParticleTypes;
};

#endif // __XENON10PLXESENSITIVEDETECTOR_H__


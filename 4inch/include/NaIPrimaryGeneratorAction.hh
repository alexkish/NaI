#ifndef __NaIPrimaryGeneratorAction_H__
#define __NaIPrimaryGeneratorAction_H__

#include <G4VUserPrimaryGeneratorAction.hh>
#include <G4ThreeVector.hh>
#include <globals.hh>

class NaIParticleSource;

class G4Event;

class NaIPrimaryGeneratorAction: public G4VUserPrimaryGeneratorAction
{
public:
	NaIPrimaryGeneratorAction();
	~NaIPrimaryGeneratorAction();

public:
	const long *GetEventSeeds() { return m_lSeeds; }
	const G4String &GetParticleTypeOfPrimary() { return m_hParticleTypeOfPrimary; }
	G4double GetEnergyOfPrimary() { return m_dEnergyOfPrimary; }
	G4ThreeVector GetPositionOfPrimary() { return m_hPositionOfPrimary; }

	void GeneratePrimaries(G4Event *pEvent);

  private:
	long m_lSeeds[2];
	G4String m_hParticleTypeOfPrimary;
	G4double m_dEnergyOfPrimary;
	G4ThreeVector m_hPositionOfPrimary;

	NaIParticleSource *m_pParticleSource;
};

#endif // __XENON10PPRIMARYGENERATORACTION_H__


#ifndef NaIPhysicsList_H
#define NaIPhysicsList_H 1

#include <G4VUserPhysicsList.hh>
#include <globals.hh>

class NaIPhysicsList: public G4VUserPhysicsList
{
public:
	 NaIPhysicsList();
	~NaIPhysicsList();

public:
	void SetCuts();

protected:
	void ConstructParticle();
	void ConstructProcess();

	void ConstructGeneral();
	void ConstructEM();
	void ConstructHad();
	void ConstructOp();

	void AddTransportation();

private:
	G4double cutForGamma;
	G4double cutForElectron;
	G4double cutForPositron;
	G4double cutForProton;
	G4double cutForAlpha;
	G4double cutForGenericIon;

	void ConstructMyBosons();
	void ConstructMyLeptons();
	void ConstructMyHadrons();
	void ConstructMyShortLiveds();

private:
	G4int VerboseLevel;
	G4int OpVerbLevel;
};

#endif // __XENON10PPHYSICSLIST_H__


#include <G4ProcessManager.hh>
#include <G4ProcessVector.hh>
#include <G4ParticleDefinition.hh>
#include <G4ParticleWithCuts.hh>
#include <G4ParticleTypes.hh>
#include <G4ParticleTable.hh>
#include <G4UserLimits.hh>
#include <G4ios.hh>
#include <globals.hh>

#include <iomanip>

#include "NaIPhysicsList.hh"

NaIPhysicsList::NaIPhysicsList():G4VUserPhysicsList()
{

	defaultCutValue = 0.01 * mm;	//
	cutForGamma = defaultCutValue;
	cutForElectron = defaultCutValue;
	cutForPositron = defaultCutValue;

	VerboseLevel = 0;
	OpVerbLevel = 0;

	SetVerboseLevel(VerboseLevel);
}

NaIPhysicsList::~NaIPhysicsList()
{
}

void
NaIPhysicsList::ConstructParticle()
{
	ConstructMyBosons();
	ConstructMyLeptons();
	ConstructMyHadrons();
	ConstructMyShortLiveds();
}

void
NaIPhysicsList::ConstructMyBosons()
{
	G4Geantino::GeantinoDefinition();
	G4ChargedGeantino::ChargedGeantinoDefinition();

	G4Gamma::GammaDefinition();

	G4OpticalPhoton::OpticalPhotonDefinition();

}

void
NaIPhysicsList::ConstructMyLeptons()
{
	G4Electron::ElectronDefinition();
	G4Positron::PositronDefinition();
	G4MuonPlus::MuonPlusDefinition();
	G4MuonMinus::MuonMinusDefinition();

	G4NeutrinoE::NeutrinoEDefinition();
	G4AntiNeutrinoE::AntiNeutrinoEDefinition();
	G4NeutrinoMu::NeutrinoMuDefinition();
	G4AntiNeutrinoMu::AntiNeutrinoMuDefinition();
}

#include "G4MesonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"

void
NaIPhysicsList::ConstructMyHadrons()
{
	G4MesonConstructor mConstructor;
	mConstructor.ConstructParticle();

	G4BaryonConstructor bConstructor;
	bConstructor.ConstructParticle();

	G4IonConstructor iConstructor;
	iConstructor.ConstructParticle();

}

void
NaIPhysicsList::ConstructMyShortLiveds()
{
}

void
NaIPhysicsList::ConstructProcess()
{
	AddTransportation();
	ConstructEM();
	ConstructOp();
	ConstructHad();
	ConstructGeneral();
}

//#include "XeMaxTimeCuts.hh"
//#include "XeMinEkineCuts.hh"

void
NaIPhysicsList::AddTransportation()
{
	G4VUserPhysicsList::AddTransportation();

//    theParticleIterator->reset();
//    while((*theParticleIterator) ())
//    {
//        G4ParticleDefinition *particle = theParticleIterator->value();
//        G4ProcessManager *pmanager = particle->GetProcessManager();
//        G4String particleName = particle->GetParticleName();

//        if(particleName == "neutron")
//            pmanager->AddDiscreteProcess(new XeMaxTimeCuts());
//        pmanager->AddDiscreteProcess(new XeMinEkineCuts());
//    }
}

// Electromagnetic Processes ////////////////////////////////////////////////
// all charged particles
#include "G4MultipleScattering.hh"

// gamma
#include "G4LowEnergyRayleigh.hh"
#include "G4LowEnergyPhotoElectric.hh"
#include "G4LowEnergyCompton.hh"
#include "G4LowEnergyGammaConversion.hh"

// e-
#include "G4LowEnergyIonisation.hh"
#include "G4LowEnergyBremsstrahlung.hh"

// e+
#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"

// alpha and GenericIon and deuterons, triton, He3:
#include "G4hLowEnergyIonisation.hh"
#include "G4EnergyLossTables.hh"
// hLowEnergyIonisation uses Ziegler 1988 as the default

//muon:
#include "G4MuIonisation.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"
#include "G4MuonMinusCaptureAtRest.hh"

//OTHERS:
#include "G4hIonisation.hh" // standard hadron ionisation

//em process options to allow msc step-limitation to be switched off
#include "G4EmProcessOptions.hh"

void
NaIPhysicsList::ConstructEM()
{
	G4LowEnergyPhotoElectric *lowePhot = new G4LowEnergyPhotoElectric();
	G4LowEnergyIonisation *loweIon = new G4LowEnergyIonisation();
	G4LowEnergyBremsstrahlung *loweBrem = new G4LowEnergyBremsstrahlung();

	//G4double fluorcut = 250 * eV;
	G4double fluorcut = 0.25 * eV;

	lowePhot->SetCutForLowEnSecPhotons(fluorcut);
	loweIon->SetCutForLowEnSecPhotons(fluorcut);
	loweBrem->SetCutForLowEnSecPhotons(fluorcut);

	theParticleIterator->reset();
	while((*theParticleIterator) ())
	{
		G4ParticleDefinition *particle = theParticleIterator->value();
		G4ProcessManager *pmanager = particle->GetProcessManager();
		G4String particleName = particle->GetParticleName();
		G4String particleType = particle->GetParticleType();
		G4double charge = particle->GetPDGCharge();

		if(particleName == "gamma")
		{
			//gamma
			pmanager->AddDiscreteProcess(new G4LowEnergyRayleigh());
			pmanager->AddDiscreteProcess(lowePhot);
			pmanager->AddDiscreteProcess(new G4LowEnergyCompton());
			pmanager->AddDiscreteProcess(new G4LowEnergyGammaConversion());
		}
		else if(particleName == "e-")
		{
			//electron
			// process ordering: AddProcess(name, at rest, along step, post step)
			// -1 = not implemented, then ordering
			G4MultipleScattering *aMultipleScattering =
				new G4MultipleScattering();
			pmanager->AddProcess(aMultipleScattering, -1, 1, 1);
			pmanager->AddProcess(loweIon, -1, 2, 2);
			pmanager->AddProcess(loweBrem, -1, -1, 3);
		}
		else if(particleName == "e+")
		{
			//positron
			G4MultipleScattering *aMultipleScattering =
				new G4MultipleScattering();
			pmanager->AddProcess(aMultipleScattering, -1, 1, 1);
			pmanager->AddProcess(new G4eIonisation(), -1, 2, 2);
			pmanager->AddProcess(new G4eBremsstrahlung(), -1, -1, 3);
			pmanager->AddProcess(new G4eplusAnnihilation(), 0, -1, 4);
		}
		else if(particleName == "mu+" || particleName == "mu-")
		{
			//muon  
			G4MultipleScattering *aMultipleScattering =
				new G4MultipleScattering();
			pmanager->AddProcess(aMultipleScattering, -1, 1, 1);
			pmanager->AddProcess(new G4MuIonisation(), -1, 2, 2);
			pmanager->AddProcess(new G4MuBremsstrahlung(), -1, -1, 3);
			pmanager->AddProcess(new G4MuPairProduction(), -1, -1, 4);
			if(particleName == "mu-")
				pmanager->AddProcess(new G4MuonMinusCaptureAtRest(), 0, -1,
					-1);
		}
		else if(particleName == "proton" ||
			particleName == "alpha" ||
			particleName == "deuteron" ||
			particleName == "triton" ||
			particleName == "He3" ||
			particleName == "GenericIon" ||
			(particleType == "nucleus" && charge != 0))
		{
			// OBJECT may be dynamically created as either a GenericIon or nucleus
			// G4Nucleus exists and therefore has particle type nucleus
			// genericIon:
			G4MultipleScattering *aMultipleScattering =
				new G4MultipleScattering();
			G4hLowEnergyIonisation *ahadronLowEIon =
				new G4hLowEnergyIonisation();
			pmanager->AddProcess(aMultipleScattering, -1, 1, 1);
			pmanager->AddProcess(ahadronLowEIon, -1, 2, 2);

			//fluorescence switch off for hadrons (for now) PIXE:
			ahadronLowEIon->SetFluorescence(false);
		}
		else if((!particle->IsShortLived()) &&
			(charge != 0.0) &&
			(particle->GetParticleName() != "chargedgeantino"))
		{
			//all others charged particles except geantino
			G4MultipleScattering *aMultipleScattering =
				new G4MultipleScattering();
			G4hLowEnergyIonisation *ahadronLowEIon =
				new G4hLowEnergyIonisation();
			pmanager->AddProcess(aMultipleScattering, -1, 1, 1);
			pmanager->AddProcess(ahadronLowEIon, -1, 2, 2);
			pmanager->AddProcess(new G4hIonisation(), -1,2,2);      
		}
	}
}

// Optical Processes ////////////////////////////////////////////////////////
#include "G4Scintillation.hh"
#include "G4OpAbsorption.hh"
#include "G4OpRayleigh.hh"
#include "G4OpBoundaryProcess.hh"

void
NaIPhysicsList::ConstructOp()
{
	// default scintillation process
	G4Scintillation *theScintProcessDef =
		new G4Scintillation("Scintillation");
	// theScintProcessDef->DumpPhysicsTable();
	theScintProcessDef->SetTrackSecondariesFirst(true);
	theScintProcessDef->SetScintillationYieldFactor(1.0);	//
	theScintProcessDef->SetScintillationExcitationRatio(0.0);	//
	theScintProcessDef->SetVerboseLevel(OpVerbLevel);

	// scintillation process for alpha:
	G4Scintillation *theScintProcessAlpha =
		new G4Scintillation("Scintillation");
	// theScintProcessNuc->DumpPhysicsTable();
	theScintProcessAlpha->SetTrackSecondariesFirst(true);
	theScintProcessAlpha->SetScintillationYieldFactor(1.1);
	theScintProcessAlpha->SetScintillationExcitationRatio(1.0);
	theScintProcessAlpha->SetVerboseLevel(OpVerbLevel);

	// scintillation process for heavy nuclei
	G4Scintillation *theScintProcessNuc =
		new G4Scintillation("Scintillation");
	// theScintProcessNuc->DumpPhysicsTable();
	theScintProcessNuc->SetTrackSecondariesFirst(true);
	theScintProcessNuc->SetScintillationYieldFactor(0.2);
	theScintProcessNuc->SetScintillationExcitationRatio(1.0);
	theScintProcessNuc->SetVerboseLevel(OpVerbLevel);

	// optical processes
	G4OpAbsorption *theAbsorptionProcess = new G4OpAbsorption();

	G4OpRayleigh* theRayleighScatteringProcess = new G4OpRayleigh();
	G4OpBoundaryProcess *theBoundaryProcess = new G4OpBoundaryProcess();

	//  theAbsorptionProcess->DumpPhysicsTable();
	//  theRayleighScatteringProcess->DumpPhysicsTable();
	theAbsorptionProcess->SetVerboseLevel(OpVerbLevel);
	theRayleighScatteringProcess->SetVerboseLevel(OpVerbLevel);
	theBoundaryProcess->SetVerboseLevel(OpVerbLevel);
	G4OpticalSurfaceModel themodel = unified;

	theBoundaryProcess->SetModel(themodel);

	theParticleIterator->reset();
	while((*theParticleIterator) ())
	{
		G4ParticleDefinition *particle = theParticleIterator->value();
		G4ProcessManager *pmanager = particle->GetProcessManager();
		G4String particleName = particle->GetParticleName();

		if(theScintProcessDef->IsApplicable(*particle))
		{
			//      if(particle->GetPDGMass() > 5.0*GeV) 
			if(particle->GetParticleName() == "GenericIon")
			{
				pmanager->AddProcess(theScintProcessNuc);	// AtRestDiscrete
				pmanager->SetProcessOrderingToLast(theScintProcessNuc,
					idxAtRest);
				pmanager->SetProcessOrderingToLast(theScintProcessNuc,
					idxPostStep);
			}
			else if(particle->GetParticleName() == "alpha")
			{
				pmanager->AddProcess(theScintProcessAlpha);
				pmanager->SetProcessOrderingToLast(theScintProcessAlpha,
					idxAtRest);
				pmanager->SetProcessOrderingToLast(theScintProcessAlpha,
					idxPostStep);
			}
			else
			{
				pmanager->AddProcess(theScintProcessDef);
				pmanager->SetProcessOrderingToLast(theScintProcessDef,
					idxAtRest);
				pmanager->SetProcessOrderingToLast(theScintProcessDef,
					idxPostStep);
			}
		}

		if(particleName == "opticalphoton")
		{
			pmanager->AddDiscreteProcess(theAbsorptionProcess);
			pmanager->AddDiscreteProcess(theRayleighScatteringProcess);
			pmanager->AddDiscreteProcess(theBoundaryProcess);
		}
	}
}

// Hadronic processes ////////////////////////////////////////////////////////

// Elastic processes:
#include "G4HadronElasticProcess.hh"
#include "G4HadronCaptureProcess.hh"

// Inelastic processes:
#include "G4PionPlusInelasticProcess.hh"
#include "G4PionMinusInelasticProcess.hh"
#include "G4KaonPlusInelasticProcess.hh"
#include "G4KaonZeroSInelasticProcess.hh"
#include "G4KaonZeroLInelasticProcess.hh"
#include "G4KaonMinusInelasticProcess.hh"
#include "G4ProtonInelasticProcess.hh"
#include "G4AntiProtonInelasticProcess.hh"
#include "G4NeutronInelasticProcess.hh"
#include "G4AntiNeutronInelasticProcess.hh"
#include "G4DeuteronInelasticProcess.hh"
#include "G4TritonInelasticProcess.hh"
#include "G4AlphaInelasticProcess.hh"

// Low-energy Models: < 20GeV
#include "G4LElastic.hh"
#include "G4LEPionPlusInelastic.hh"
#include "G4LEPionMinusInelastic.hh"
#include "G4LEKaonPlusInelastic.hh"
#include "G4LEKaonZeroSInelastic.hh"
#include "G4LEKaonZeroLInelastic.hh"
#include "G4LEKaonMinusInelastic.hh"
#include "G4LEProtonInelastic.hh"
#include "G4LEAntiProtonInelastic.hh"
#include "G4LENeutronInelastic.hh"
#include "G4LEAntiNeutronInelastic.hh"
#include "G4LEDeuteronInelastic.hh"
#include "G4LETritonInelastic.hh"
#include "G4LEAlphaInelastic.hh"

// High-energy Models: >20 GeV
#include "G4HEPionPlusInelastic.hh"
#include "G4HEPionMinusInelastic.hh"
#include "G4HEKaonPlusInelastic.hh"
#include "G4HEKaonZeroInelastic.hh"
#include "G4HEKaonZeroInelastic.hh"
#include "G4HEKaonMinusInelastic.hh"
#include "G4HEProtonInelastic.hh"
#include "G4HEAntiProtonInelastic.hh"
#include "G4HENeutronInelastic.hh"
#include "G4HEAntiNeutronInelastic.hh"

// Neutron high-precision models: <20 MeV
#include "G4NeutronHPElastic.hh"
#include "G4NeutronHPElasticData.hh"
#include "G4NeutronHPCapture.hh"
#include "G4NeutronHPCaptureData.hh"
#include "G4NeutronHPInelastic.hh"
#include "G4NeutronHPInelasticData.hh"
#include "G4LCapture.hh"

// Stopping processes
#include "G4PiMinusAbsorptionAtRest.hh"
#include "G4KaonMinusAbsorptionAtRest.hh"
#include "G4AntiProtonAnnihilationAtRest.hh"
#include "G4AntiNeutronAnnihilationAtRest.hh"

// ConstructHad()
// Makes discrete physics processes for the hadrons, at present limited
// to those particles with GHEISHA interactions (INTRC > 0).
// The processes are: Elastic scattering and Inelastic scattering.
// F.W.Jones  09-JUL-1998
void
NaIPhysicsList::ConstructHad()
{
	G4HadronElasticProcess *theElasticProcess = new G4HadronElasticProcess;
	G4LElastic *theElasticModel = new G4LElastic;

	theElasticProcess->RegisterMe(theElasticModel);

	theParticleIterator->reset();
	while((*theParticleIterator) ())
	{
		G4ParticleDefinition *particle = theParticleIterator->value();
		G4ProcessManager *pmanager = particle->GetProcessManager();
		G4String particleName = particle->GetParticleName();

		if(particleName == "pi+")
		{
			pmanager->AddDiscreteProcess(theElasticProcess);
			G4PionPlusInelasticProcess *theInelasticProcess =
				new G4PionPlusInelasticProcess("inelastic");
			G4LEPionPlusInelastic *theLEInelasticModel =
				new G4LEPionPlusInelastic;
			theInelasticProcess->RegisterMe(theLEInelasticModel);
			G4HEPionPlusInelastic *theHEInelasticModel =
				new G4HEPionPlusInelastic;
			theInelasticProcess->RegisterMe(theHEInelasticModel);
			pmanager->AddDiscreteProcess(theInelasticProcess);
		}

		else if(particleName == "pi-")
		{
			pmanager->AddDiscreteProcess(theElasticProcess);
			G4PionMinusInelasticProcess *theInelasticProcess =
				new G4PionMinusInelasticProcess("inelastic");
			G4LEPionMinusInelastic *theLEInelasticModel =
				new G4LEPionMinusInelastic;
			theInelasticProcess->RegisterMe(theLEInelasticModel);
			G4HEPionMinusInelastic *theHEInelasticModel =
				new G4HEPionMinusInelastic;
			theInelasticProcess->RegisterMe(theHEInelasticModel);
			pmanager->AddDiscreteProcess(theInelasticProcess);
			G4String prcNam;

			pmanager->AddRestProcess(new G4PiMinusAbsorptionAtRest,
				ordDefault);
		}

		else if(particleName == "kaon+")
		{
			pmanager->AddDiscreteProcess(theElasticProcess);
			G4KaonPlusInelasticProcess *theInelasticProcess =
				new G4KaonPlusInelasticProcess("inelastic");
			G4LEKaonPlusInelastic *theLEInelasticModel =
				new G4LEKaonPlusInelastic;
			theInelasticProcess->RegisterMe(theLEInelasticModel);
			G4HEKaonPlusInelastic *theHEInelasticModel =
				new G4HEKaonPlusInelastic;
			theInelasticProcess->RegisterMe(theHEInelasticModel);
			pmanager->AddDiscreteProcess(theInelasticProcess);
		}

		else if(particleName == "kaon0S")
		{
			pmanager->AddDiscreteProcess(theElasticProcess);
			G4KaonZeroSInelasticProcess *theInelasticProcess =
				new G4KaonZeroSInelasticProcess("inelastic");
			G4LEKaonZeroSInelastic *theLEInelasticModel =
				new G4LEKaonZeroSInelastic;
			theInelasticProcess->RegisterMe(theLEInelasticModel);
			G4HEKaonZeroInelastic *theHEInelasticModel =
				new G4HEKaonZeroInelastic;
			theInelasticProcess->RegisterMe(theHEInelasticModel);
			pmanager->AddDiscreteProcess(theInelasticProcess);
		}

		else if(particleName == "kaon0L")
		{
			pmanager->AddDiscreteProcess(theElasticProcess);
			G4KaonZeroLInelasticProcess *theInelasticProcess =
				new G4KaonZeroLInelasticProcess("inelastic");
			G4LEKaonZeroLInelastic *theLEInelasticModel =
				new G4LEKaonZeroLInelastic;
			theInelasticProcess->RegisterMe(theLEInelasticModel);
			G4HEKaonZeroInelastic *theHEInelasticModel =
				new G4HEKaonZeroInelastic;
			theInelasticProcess->RegisterMe(theHEInelasticModel);
			pmanager->AddDiscreteProcess(theInelasticProcess);
		}

		else if(particleName == "kaon-")
		{
			pmanager->AddDiscreteProcess(theElasticProcess);
			G4KaonMinusInelasticProcess *theInelasticProcess =
				new G4KaonMinusInelasticProcess("inelastic");
			G4LEKaonMinusInelastic *theLEInelasticModel =
				new G4LEKaonMinusInelastic;
			theInelasticProcess->RegisterMe(theLEInelasticModel);
			G4HEKaonMinusInelastic *theHEInelasticModel =
				new G4HEKaonMinusInelastic;
			theInelasticProcess->RegisterMe(theHEInelasticModel);
			pmanager->AddDiscreteProcess(theInelasticProcess);
			pmanager->AddRestProcess(new G4KaonMinusAbsorptionAtRest,
				ordDefault);
		}

		else if(particleName == "proton")
		{
			pmanager->AddDiscreteProcess(theElasticProcess);
			G4ProtonInelasticProcess *theInelasticProcess =
				new G4ProtonInelasticProcess("inelastic");
			G4LEProtonInelastic *theLEInelasticModel =
				new G4LEProtonInelastic;
			theInelasticProcess->RegisterMe(theLEInelasticModel);
			G4HEProtonInelastic *theHEInelasticModel =
				new G4HEProtonInelastic;
			theInelasticProcess->RegisterMe(theHEInelasticModel);
			pmanager->AddDiscreteProcess(theInelasticProcess);
		}

		else if(particleName == "anti_proton")
		{
			pmanager->AddDiscreteProcess(theElasticProcess);
			G4AntiProtonInelasticProcess *theInelasticProcess =
				new G4AntiProtonInelasticProcess("inelastic");
			G4LEAntiProtonInelastic *theLEInelasticModel =
				new G4LEAntiProtonInelastic;
			theInelasticProcess->RegisterMe(theLEInelasticModel);
			G4HEAntiProtonInelastic *theHEInelasticModel =
				new G4HEAntiProtonInelastic;
			theInelasticProcess->RegisterMe(theHEInelasticModel);
			pmanager->AddDiscreteProcess(theInelasticProcess);
		}

		else if(particleName == "neutron")
		{
			// elastic scattering
			G4HadronElasticProcess *theNeutronElasticProcess =
				new G4HadronElasticProcess;
			G4LElastic *theElasticModel1 = new G4LElastic;
			G4NeutronHPElastic *theElasticNeutron = new G4NeutronHPElastic;

			theNeutronElasticProcess->RegisterMe(theElasticModel1);
			theElasticModel1->SetMinEnergy(0.00001 * eV);
			theNeutronElasticProcess->RegisterMe(theElasticNeutron);
			G4NeutronHPElasticData *theNeutronData =
				new G4NeutronHPElasticData;
			theNeutronElasticProcess->AddDataSet(theNeutronData);
			pmanager->AddDiscreteProcess(theNeutronElasticProcess);
			// inelastic scattering
			G4NeutronInelasticProcess *theInelasticProcess =
				new G4NeutronInelasticProcess("inelastic");
			G4LENeutronInelastic *theInelasticModel =
				new G4LENeutronInelastic;
			theInelasticModel->SetMinEnergy(0.00001 * eV);
			theInelasticProcess->RegisterMe(theInelasticModel);
			G4NeutronHPInelastic *theLENeutronInelasticModel =
				new G4NeutronHPInelastic;
			theInelasticProcess->RegisterMe(theLENeutronInelasticModel);
			G4NeutronHPInelasticData *theNeutronData1 =
				new G4NeutronHPInelasticData;
			theInelasticProcess->AddDataSet(theNeutronData1);
			pmanager->AddDiscreteProcess(theInelasticProcess);
			// capture
			G4HadronCaptureProcess *theCaptureProcess =
				new G4HadronCaptureProcess;
			G4LCapture *theCaptureModel = new G4LCapture;

			theCaptureModel->SetMinEnergy(0.00001 * eV);
			theCaptureProcess->RegisterMe(theCaptureModel);
			G4NeutronHPCapture *theLENeutronCaptureModel =
				new G4NeutronHPCapture;
			theCaptureProcess->RegisterMe(theLENeutronCaptureModel);
			G4NeutronHPCaptureData *theNeutronData3 =
				new G4NeutronHPCaptureData;
			theCaptureProcess->AddDataSet(theNeutronData3);
			pmanager->AddDiscreteProcess(theCaptureProcess);
			//  G4ProcessManager* pmanager = G4Neutron::Neutron->GetProcessManager();
			//  pmanager->AddProcess(new G4UserSpecialCuts(),-1,-1,1);
		}
		else if(particleName == "anti_neutron")
		{
			pmanager->AddDiscreteProcess(theElasticProcess);
			G4AntiNeutronInelasticProcess *theInelasticProcess =
				new G4AntiNeutronInelasticProcess("inelastic");
			G4LEAntiNeutronInelastic *theLEInelasticModel =
				new G4LEAntiNeutronInelastic;
			theInelasticProcess->RegisterMe(theLEInelasticModel);
			G4HEAntiNeutronInelastic *theHEInelasticModel =
				new G4HEAntiNeutronInelastic;
			theInelasticProcess->RegisterMe(theHEInelasticModel);
			pmanager->AddDiscreteProcess(theInelasticProcess);
		}

		else if(particleName == "deuteron")
		{
			pmanager->AddDiscreteProcess(theElasticProcess);
			G4DeuteronInelasticProcess *theInelasticProcess =
				new G4DeuteronInelasticProcess("inelastic");
			G4LEDeuteronInelastic *theLEInelasticModel =
				new G4LEDeuteronInelastic;
			theInelasticProcess->RegisterMe(theLEInelasticModel);
			pmanager->AddDiscreteProcess(theInelasticProcess);
		}

		else if(particleName == "triton")
		{
			pmanager->AddDiscreteProcess(theElasticProcess);
			G4TritonInelasticProcess *theInelasticProcess =
				new G4TritonInelasticProcess("inelastic");
			G4LETritonInelastic *theLEInelasticModel =
				new G4LETritonInelastic;
			theInelasticProcess->RegisterMe(theLEInelasticModel);
			pmanager->AddDiscreteProcess(theInelasticProcess);
		}

		else if(particleName == "alpha")
		{
			pmanager->AddDiscreteProcess(theElasticProcess);
			G4AlphaInelasticProcess *theInelasticProcess =
				new G4AlphaInelasticProcess("inelastic");
			G4LEAlphaInelastic *theLEInelasticModel = new G4LEAlphaInelastic;

			theInelasticProcess->RegisterMe(theLEInelasticModel);
			pmanager->AddDiscreteProcess(theInelasticProcess);
		}

	}
}

// Decays ///////////////////////////////////////////////////////////////////
#include "G4Decay.hh"
#include "G4RadioactiveDecay.hh"
#include "G4IonTable.hh"
#include "G4Ions.hh"

void
NaIPhysicsList::ConstructGeneral()
{

	// Add Decay Process
	G4Decay *theDecayProcess = new G4Decay();

	theParticleIterator->reset();
	while((*theParticleIterator) ())
	{
		G4ParticleDefinition *particle = theParticleIterator->value();
		G4ProcessManager *pmanager = particle->GetProcessManager();

		if(theDecayProcess->IsApplicable(*particle)
			&& !particle->IsShortLived())
		{
			pmanager->AddProcess(theDecayProcess);
			// set ordering for PostStepDoIt and AtRestDoIt
			pmanager->SetProcessOrdering(theDecayProcess, idxPostStep);
			pmanager->SetProcessOrdering(theDecayProcess, idxAtRest);
		}
	}

	// Declare radioactive decay to the GenericIon in the IonTable.
	const G4IonTable *theIonTable =
		G4ParticleTable::GetParticleTable()->GetIonTable();
	G4RadioactiveDecay *theRadioactiveDecay = new G4RadioactiveDecay();

	for(G4int i = 0; i < theIonTable->Entries(); i++)
	{
		G4String particleName =
			theIonTable->GetParticle(i)->GetParticleName();
		G4String particleType =
			theIonTable->GetParticle(i)->GetParticleType();

		if(particleName == "GenericIon")
		{
			G4ProcessManager *pmanager =
				theIonTable->GetParticle(i)->GetProcessManager();
			pmanager->SetVerboseLevel(VerboseLevel);
			pmanager->AddProcess(theRadioactiveDecay);
			pmanager->SetProcessOrdering(theRadioactiveDecay, idxPostStep);
			pmanager->SetProcessOrdering(theRadioactiveDecay, idxAtRest);
		}
	}
}

// Cuts /////////////////////////////////////////////////////////////////////
void
NaIPhysicsList::SetCuts()
{

	if(verboseLevel > 1)
		G4cout << "Xenon100PhysicsList::SetCuts:";

	if(verboseLevel > 0)
	{
		G4cout << "Xenon100PhysicsList::SetCuts:";
		G4cout << "CutLength : "
			<< G4BestUnit(defaultCutValue, "Length") << G4endl;
	}

	//special for low energy physics
	//G4double lowlimit = 250 * eV;
	G4double lowlimit = 0.25 * eV;

	G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(lowlimit,
		100. * GeV);

	// set cut values for gamma at first and for e- second and next for e+,
	// because some processes for e+/e- need cut values for gamma 
	SetCutValue(cutForGamma, "gamma");
	SetCutValue(cutForElectron, "e-");
	SetCutValue(cutForPositron, "e+");

	if(verboseLevel > 0)
		DumpCutValuesTable();
}


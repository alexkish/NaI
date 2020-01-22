#ifndef NaIDetectorConstruction_H
#define NaIDetectorConstruction_H 1

#include "G4VUserDetectorConstruction.hh"
#include "NaIDetectorConstruction.hh"


class G4LogicalVolume;
class G4VPhysicalVolume;
class G4VisAttributes;
class G4Material;
class G4MaterialPropertiesTable;
class G4SubtractionSolid;
class G4UnionSolid;




class NaIDetectorConstruction : public G4VUserDetectorConstruction
{

  public:
    NaIDetectorConstruction();
   ~NaIDetectorConstruction();

  public:
    G4VPhysicalVolume *Construct();
	   
  private:

	G4Material *	Air;
	G4Material *	Vacuum;
	G4Material *	SSteel;
	G4Material *	MuMetal;
	G4Material *	metalAl;
	G4Material *	NaI;
	
//----------	Logical Volumes		----------------
	G4LogicalVolume * Laboratory_log;	
	G4LogicalVolume	* NaI_Crystal_log;
	G4LogicalVolume	* NaI_CrystalDent_log;
	G4LogicalVolume	* NaI_CrystalHousingTop_log;
	G4LogicalVolume	* NaI_CrystalHousingBot_log;
	G4LogicalVolume	* NaI_LightShield_log;
	G4LogicalVolume	* NaI_LightShieldVac_log;

	G4LogicalVolume	* SourceCapsule_log;
	G4LogicalVolume	* SourceCapsuleThread_log;
	G4LogicalVolume	* SourceActiveRegion_log;
	
//----------	Physical Volumes	----------------
	G4VPhysicalVolume *	Laboratory_phys;
	G4VPhysicalVolume * NaI_Crystal_phys;
	G4VPhysicalVolume * NaI_CrystalDent_phys;
	G4VPhysicalVolume * NaI_CrystalHousingTop_phys;
	G4VPhysicalVolume * NaI_CrystalHousingBot_phys;
	G4VPhysicalVolume * NaI_LightShield_phys;
	G4VPhysicalVolume * NaI_LightShieldVac_phys;

	G4VPhysicalVolume * SourceCapsule_phys;
	G4VPhysicalVolume * SourceCapsuleThread_phys;
	G4VPhysicalVolume * SourceActiveRegion_phys;

//----------	Visual Attributes	----------------
	G4VisAttributes *	NaI_Crystal_vis;
	G4VisAttributes *	NaI_CrystalDent_vis;
	G4VisAttributes *	NaI_CrystalHousing_vis;
	G4VisAttributes *	NaI_LightShield_vis;
	G4VisAttributes *	SourceCapsule_vis;
	G4VisAttributes *	SourceActiveRegion_vis;


	

};

#endif


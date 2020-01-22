#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh"
#include "G4NistManager.hh"
#include "G4Material.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4Orb.hh"
#include "G4Polyhedra.hh"
#include "G4Trd.hh"
#include "G4Cons.hh"

#include "G4UnionSolid.hh"
#include "G4IntersectionSolid.hh"
#include "G4SubtractionSolid.hh"

#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"

#include "G4VisAttributes.hh"

#include "G4OpBoundaryProcess.hh"
#include "G4SDManager.hh"

#include "G4Colour.hh"
#include "globals.hh"

#include "G4ios.hh"

#include "vector"
#include "numeric"
#include "sstream"
#include "algorithm"
#include "cmath"
#include "cassert"

using std::vector;
using std::stringstream;
using std::max;

#include "NaIDetectorConstruction.hh"
#include "NaISensitiveDetector.hh"


NaIDetectorConstruction::NaIDetectorConstruction()
{
}

NaIDetectorConstruction::~NaIDetectorConstruction()
{
}

G4VPhysicalVolume *NaIDetectorConstruction::Construct()
{
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
G4cout 																			<< G4endl;
G4cout << 	" ============================================================ "	<< G4endl;
G4cout <<	"|   NaI detector                                             |"	<< G4endl;
G4cout <<	"|  -------------                                             |"	<< G4endl;
G4cout <<	"|                                  Alexander Kish, UZH 2010  |"	<< G4endl;
G4cout <<	" ============================================================ "	<< G4endl;
G4cout <<	"| "																<< G4endl;
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////



	G4double	density,	// density
    			a,			// atomic weight (g/mole)
    			z;			// atomic number
	G4String	name,		// name
 				symbol;		// symbol
	G4int		ncomponents,
 				natoms;          
	G4double	temperature,
				pressure;    


	//----------	Define elements		--------------------------------------------------------
	G4Element *H  = new G4Element(name= "Hydrogen",   symbol= "H",  z= 1.,  a= 1.008   *g/mole);
	G4Element *B  = new G4Element(name= "Boron",	  symbol= "B",  z= 5.,  a= 10.811  *g/mole);
	G4Element *C  = new G4Element(name= "Carbon",     symbol= "C",  z= 6.,  a= 12.011  *g/mole);
	G4Element *N  = new G4Element(name= "Nitrogen",   symbol= "N",  z= 7.,  a= 14.01   *g/mole);
	G4Element *O  = new G4Element(name= "Oxygen",     symbol= "O",  z= 8.,  a= 16.00   *g/mole);
	G4Element *F  = new G4Element(name= "Fluorine",   symbol= "F",  z= 9.,  a= 18.998  *g/mole);
	G4Element *Na = new G4Element(name= "Sodium",     symbol= "Na", z= 11., a= 22.9897 *g/mole);
	G4Element *Al = new G4Element(name= "Aluminium",  symbol= "Al", z= 13., a= 26.98   *g/mole);
	G4Element *Si = new G4Element(name= "Silicon",    symbol= "Si", z= 14., a= 28.0855 *g/mole);
	G4Element *P  = new G4Element(name= "Phosphorus", symbol= "P",  z= 15., a= 30.9738 *g/mole);
	G4Element *S  = new G4Element(name= "Sulphur",    symbol= "S",  z= 16., a= 32.065  *g/mole);
	G4Element *Ar = new G4Element(name= "Argon",	  symbol= "Ar", z= 18., a= 39.948  *g/mole);
	G4Element *Ti = new G4Element(name= "Titanium",	  symbol= "Ti", z= 22., a= 47.867  *g/mole);
	G4Element *Cr = new G4Element(name= "Chromium",   symbol= "Cr", z= 24., a= 51.9962 *g/mole);
	G4Element *Mn = new G4Element(name= "Manganese",  symbol= "Mn", z= 25., a= 54.9381 *g/mole);
	G4Element *Fe = new G4Element(name= "Iron",       symbol= "Fe", z= 26., a= 55.845  *g/mole);
	G4Element *Co = new G4Element(name= "Cobalt",     symbol= "Co", z= 27., a= 58.9332 *g/mole);
	G4Element *Ni = new G4Element(name= "Nickel",     symbol= "Ni", z= 28., a= 58.6934 *g/mole);
	G4Element *Cu = new G4Element(name= "Copper",     symbol= "Cu", z= 29., a= 63.546  *g/mole);
 	G4Element* Ge = new G4Element(name= "Germanium",  symbol= "Ge", z= 32., a= 72.64   *g/mole);
	G4Element *Mo = new G4Element(name= "Molybdenum", symbol= "Mo", z= 42., a= 95.94   *g/mole);
	G4Element *I  = new G4Element(name= "Iodine",     symbol= "I",  z= 53., a= 126.90447 *g/mole);
	G4Element *Xe = new G4Element(name= "Xenon",      symbol= "Xe", z= 54., a= 131.29  *g/mole);
	G4Element *Cs = new G4Element(name= "Caesium",    symbol= "Cs", z= 55., a= 132.905451 *g/mole);
	G4Element *Pb = new G4Element(name= "Lead",       symbol= "Pb", z= 82., a= 207.2   *g/mole);
	G4Element *Bi = new G4Element(name= "Bismuth",    symbol= "Bi", z= 83., a= 208.98038 *g/mole);
	
	//================================== materials ================================== 
	G4NistManager* pNistManager = G4NistManager::Instance();

   	// Air
	G4Material *Air = G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");

    // Vacuum
	G4Material *Vacuum 	= new G4Material(name = "Vacuum", z = 1., a = 1. *g/mole, density = 1.e-20 *g/cm3, kStateGas, temperature = 0.1 * kelvin, pressure = 1.e-20 * bar);
 
  	// stainless steel
	G4Material *SSteel  = new G4Material(name = "SSteel", density = 7.7 *g/cm3, ncomponents = 3);
	SSteel->AddElement(C,  0.04);
	SSteel->AddElement(Fe, 0.88);
	SSteel->AddElement(Co, 0.08);

    // Aluminium
    G4Material *metalAl	= new G4Material(name = "MetalAluminium", density = 2.700 *g/cm3, ncomponents = 1);
	metalAl->AddElement(Al, 1);

  	// mu-metal for NaI light shield
	// source: http://www.drmsmetals.com/data/electronic/almu.html
 	// alternative source: http://www.aircraftmaterialsuk.com/data/electronic/almu.html
	// official source: http://mumetal.co.uk/
	G4Material *MuMetal = new G4Material(name = "MuMetal", density = 8.7 *g/cm3, ncomponents = 5);
	MuMetal->AddElement(Ni,  0.80);
	MuMetal->AddElement(Mo,  0.05);
	MuMetal->AddElement(Si,  0.005);
	MuMetal->AddElement(Cu,  0.0002);
	MuMetal->AddElement(Fe,  0.1448);

    // NaI
    // reference: http://www.optical-components.com/NaI-CsI-crystal.html
	G4Material *NaI 	= new G4Material(name = "NaI", density = 3.67 *g/cm3, ncomponents =2);
	NaI->AddElement(Na, 1);
	NaI->AddElement(I,  1);

    // CsI
    // reference: http://www.optical-components.com/NaI-CsI-crystal.html
	G4Material *CsI 	= new G4Material(name = "CsI", density = 4.51 *g/cm3, ncomponents =2);
	CsI->AddElement(Cs, 1);
	CsI->AddElement(I,  1);
	
	// BGO
	// reference: http://www.omegapiezo.com/crystal_scintillators.html
	G4Material *BGO 	= new G4Material(name = "BGO", density = 7.13 *g/cm3, ncomponents =3);
	BGO->AddElement(Bi, 4);
	BGO->AddElement(Ge, 3);
	BGO->AddElement(O, 12);

	// ceramics
	G4Material *Ceramics = new G4Material("Ceramics", 1.00*g/cm3, 4, kStateSolid);
	Ceramics->AddElement(Na, 1);
	Ceramics->AddElement(Al, 1);
	Ceramics->AddElement(Si, 1);
	Ceramics->AddElement(O, 2);

	//------------------------------- stainless steel -------------------------------
	G4Material *StainlessSteel = new G4Material("StainlessSteel", 8.00*g/cm3, 5, kStateSolid);
	StainlessSteel->AddElement(Fe, 0.65);
	StainlessSteel->AddElement(Cr, 0.20);
	StainlessSteel->AddElement(Ni, 0.12);
	StainlessSteel->AddElement(Mn, 0.02);
	StainlessSteel->AddElement(Si, 0.01);


	////////////////////////////////////////////////////////////////
//--------------	colours		--------------------------------
	G4Colour white  (1.0,	1.0,	1.0);
	G4Colour grey   (0.5,	0.5,	0.5);
	G4Colour lgrey  (.85,	.85,	.85);
	G4Colour red    (1.0,	0.0,	0.0);
	G4Colour lred   (0.75,	0.0,	0.0);
	G4Colour xlred  (0.5,	0.0,	0.0);
	G4Colour cyan   (0.0,	1.0,	1.0);
	G4Colour blue   (0.0,	0.0,	1.0);
	G4Colour lblue  (.5,	0.5,	1.,		1.);
	G4Colour xlblue (.5,	0.5,	1.,		0.2);
	G4Colour magenta(1.0,	0.0,	1.0);
	G4Colour yellow (1.0,	1.0,	0.0);
	G4Colour green  (0.,	.1,		0.);
	G4Colour lgreen (0.0,	.75,	0.0);
	G4Colour xlgreen(0.0,	0.5,	0.0);
	G4Colour brown  (0.7,	0.4,	0.1);
	G4Colour orange (1.0,	0.5,	0.0);

	// rotations
	G4RotationMatrix ZeroRot;
	ZeroRot.rotateX(0. *deg);
	ZeroRot.rotateY(0. *deg);
	ZeroRot.rotateZ(0. *deg);

	G4RotationMatrix RotationXPlus90;
	RotationXPlus90.rotateX(90.*deg);
	RotationXPlus90.rotateY(0.*deg);
	RotationXPlus90.rotateZ(0.*deg);

	G4double opendeg	= 0.0 *deg;
	G4double closedeg	= 360.0 *deg;

	// Laboratory
	G4double Lab_HalfX = 2.0 *m;
	G4double Lab_HalfY = 2.0 *m;
	G4double Lab_HalfZ = 2.0 *m;

	G4Box *Laboratory = new G4Box("Laboratory", Lab_HalfX, Lab_HalfY, Lab_HalfZ);

	//////////c//////////////////////		
	// NaI detector
	G4double NaI_height 					= 222.25 *mm;
	
	G4double NaI_Crystal_iD 				= 0.0 *mm;
	//G4double NaI_Crystal_oD 				= 76.2 *mm; // 3inch
	G4double NaI_Crystal_oD 				= 101.6 *mm; // 4inch
	//G4double NaI_Crystal_oD 				= 152.4 *mm; // 6inch
	G4double NaI_Crystal_height 			= NaI_Crystal_oD;
	
	G4double NaI_CrystalHousing_thick		= 0.508 *mm;
	//G4double NaI_CrystalHousingTop_oD		= 80.72 *mm;
	G4double NaI_CrystalHousingTop_oD		= NaI_Crystal_oD + 4.52*mm;
	G4double NaI_CrystalHousingTop_iD		= NaI_CrystalHousingTop_oD - 2*NaI_CrystalHousing_thick;
	G4double NaI_CrystalHousing_height		= NaI_Crystal_height + NaI_CrystalHousing_thick;
	G4double NaI_CrystalHousingTop_height	= NaI_Crystal_height;
	G4double NaI_CrystalHousingBot_height	= NaI_CrystalHousing_thick;
	G4double NaI_CrystalHousingBot_iD		= 0.0 *mm;
	G4double NaI_CrystalHousingBot_oD		= NaI_CrystalHousingTop_oD;

	G4double NaI_CrystalDent_iD 			= 0.0*mm;
	G4double NaI_CrystalDent_oD 			= 17.0*mm;
	//G4double NaI_CrystalDent_depth 		= 52.0*mm; // 3inch
	G4double NaI_CrystalDent_depth 		= 69.2*mm; // 4inch
	//G4double NaI_CrystalDent_depth 			= 104.0*mm; // 6inch

	G4double NaI_LightShield_thick			= 0.635 *mm;
	G4double NaI_LightShield_height			= NaI_height - NaI_CrystalHousing_height;
	G4double NaI_LightShieldBot_oD			= NaI_Crystal_oD + 6.35*mm;
	G4double NaI_LightShieldBot_iD			= NaI_LightShieldBot_oD - 2*NaI_LightShield_thick;
	G4double NaI_LightShieldBot_height		= 30.00 *mm;

	G4double NaI_LightShieldTop_oD			= 58.72 *mm;
	G4double NaI_LightShieldTop_iD			= NaI_LightShieldTop_oD - 2*NaI_LightShield_thick;

	G4double NaI_LightShieldMid_oRtop		= NaI_LightShieldTop_oD/2;
	G4double NaI_LightShieldMid_iRtop		= NaI_LightShieldMid_oRtop - NaI_LightShield_thick;
	G4double NaI_LightShieldMid_oRbot		= NaI_LightShieldBot_oD/2;
	G4double NaI_LightShieldMid_iRbot		= NaI_LightShieldMid_oRbot - NaI_LightShield_thick;
	G4double NaI_LightShieldMid_height		= 15.0 *mm;

	G4double NaI_LightShieldTop_height		= NaI_LightShield_height - NaI_LightShieldBot_height - NaI_LightShieldMid_height;
	// vacuum inside the light shield
	G4double NaI_LightShieldVac_height		= NaI_height - NaI_CrystalHousing_height;
	G4double NaI_LightShieldVacBot_oD		= NaI_LightShieldBot_iD;
	G4double NaI_LightShieldVacBot_iD		= 0.0 *mm;
	G4double NaI_LightShieldVacBot_height	= 30.00 *mm;

	G4double NaI_LightShieldVacTop_oD		= NaI_LightShieldTop_iD;
	G4double NaI_LightShieldVacTop_iD		= 0.0 *mm;

	G4double NaI_LightShieldVacMid_oRtop	= NaI_LightShieldMid_iRtop;
	G4double NaI_LightShieldVacMid_iRtop	= 0.0 *mm;
	G4double NaI_LightShieldVacMid_oRbot	= NaI_LightShieldMid_iRbot;
	G4double NaI_LightShieldVacMid_iRbot	= 0.0 *mm;
	G4double NaI_LightShieldVacMid_height	= 15.0 *mm;

	G4double NaI_LightShieldVacTop_height	= NaI_LightShield_height - NaI_LightShieldBot_height - NaI_LightShieldMid_height;
	
	G4Tubs *NaI_Crystal				= new G4Tubs("NaI_Crystal",	NaI_Crystal_iD/2, NaI_Crystal_oD/2, NaI_Crystal_height/2, opendeg, closedeg);
	G4Tubs *NaI_CrystalHousingTop	= new G4Tubs("NaI_CrystalHousingTop",	NaI_CrystalHousingTop_iD/2, NaI_CrystalHousingTop_oD/2, NaI_CrystalHousingTop_height/2, opendeg, closedeg);
	G4Tubs *NaI_CrystalHousingBot	= new G4Tubs("NaI_CrystalHousingBot",	NaI_CrystalHousingBot_iD/2, NaI_CrystalHousingBot_oD/2, NaI_CrystalHousingBot_height/2, opendeg, closedeg);
	G4Tubs *NaI_CrystalDent			= new G4Tubs("NaI_CrystalDent",	NaI_CrystalDent_iD/2, NaI_CrystalDent_oD/2, NaI_CrystalDent_depth/2, opendeg, closedeg);

	//G4ThreeVector NaI_CrystalDent_V	(0.0, 0.0, -NaI_Crystal_height/2+NaI_CrystalDent_depth/2);
	//G4Transform3D NaI_CrystalDent_T	(ZeroRot, NaI_CrystalDent_V);
	//G4SubtractionSolid *NaI_Crystal = new G4SubtractionSolid( "NaI_Crystal", NaI_CrystalCylinder, NaI_CrystalDent, NaI_CrystalDent_T);

	G4Tubs *NaI_LightShieldBot		= new G4Tubs("NaI_LightShieldBot",	NaI_LightShieldBot_iD/2, NaI_LightShieldBot_oD/2, NaI_LightShieldBot_height/2, opendeg, closedeg);
	G4Tubs *NaI_LightShieldTop		= new G4Tubs("NaI_LightShieldTop",	NaI_LightShieldTop_iD/2, NaI_LightShieldTop_oD/2, NaI_LightShieldTop_height/2, opendeg, closedeg);
	G4Cons *NaI_LightShieldMid		= new G4Cons("NaI_LightShieldMid",	NaI_LightShieldMid_iRbot, NaI_LightShieldMid_oRbot, NaI_LightShieldMid_iRtop, NaI_LightShieldMid_oRtop, NaI_LightShieldMid_height/2, opendeg, closedeg);
	G4ThreeVector NaI_LightShieldMid_V	(0.0, 0.0, NaI_LightShieldBot_height/2+NaI_LightShieldMid_height/2);
	G4Transform3D NaI_LightShieldMid_T	(ZeroRot, NaI_LightShieldMid_V);
	G4UnionSolid *NaI_LightShield1	= new G4UnionSolid( "NaI_LightShield1",	NaI_LightShieldBot, NaI_LightShieldMid,	NaI_LightShieldMid_T);
	G4ThreeVector NaI_LightShieldTop_V	(0.0, 0.0, (NaI_LightShieldBot_height+NaI_LightShieldMid_height)/2+NaI_LightShieldTop_height/2);
	G4Transform3D NaI_LightShieldTop_T	(ZeroRot, NaI_LightShieldTop_V);
	G4UnionSolid *NaI_LightShield	= new G4UnionSolid( "NaI_LightShield",	NaI_LightShield1,	NaI_LightShieldTop,	NaI_LightShieldTop_T);

	G4Tubs *NaI_LightShieldVacBot	= new G4Tubs("NaI_LightShieldVacBot",	NaI_LightShieldVacBot_iD/2, NaI_LightShieldVacBot_oD/2, NaI_LightShieldVacBot_height/2, opendeg, closedeg);
	G4Tubs *NaI_LightShieldVacTop	= new G4Tubs("NaI_LightShieldVacTop",	NaI_LightShieldVacTop_iD/2, NaI_LightShieldVacTop_oD/2, NaI_LightShieldVacTop_height/2, opendeg, closedeg);
	G4Cons *NaI_LightShieldVacMid	= new G4Cons("NaI_LightShieldVacMid",	NaI_LightShieldVacMid_iRbot, NaI_LightShieldVacMid_oRbot, NaI_LightShieldVacMid_iRtop, NaI_LightShieldVacMid_oRtop, NaI_LightShieldVacMid_height/2, opendeg, closedeg);
	G4ThreeVector NaI_LightShieldVacMid_V	(0.0, 0.0, NaI_LightShieldVacBot_height/2+NaI_LightShieldVacMid_height/2);
	G4Transform3D NaI_LightShieldVacMid_T	(ZeroRot, NaI_LightShieldVacMid_V);
	G4UnionSolid *NaI_LightShieldVac1	= new G4UnionSolid( "NaI_LightShieldVac1", NaI_LightShieldVacBot, NaI_LightShieldVacMid, NaI_LightShieldVacMid_T);
	G4ThreeVector NaI_LightShieldVacTop_V	(0.0, 0.0, (NaI_LightShieldVacBot_height+NaI_LightShieldVacMid_height)/2+NaI_LightShieldVacTop_height/2);
	G4Transform3D NaI_LightShieldVacTop_T	(ZeroRot, NaI_LightShieldVacTop_V);
	G4UnionSolid *NaI_LightShieldVac	= new G4UnionSolid( "NaI_LightShieldVac", NaI_LightShieldVac1, NaI_LightShieldVacTop, NaI_LightShieldVacTop_T);



	G4double SourceCapsule_iD 	= 0.0 *mm;
	G4double SourceCapsule_oD 	= 6.4 *mm;
	G4double SourceCapsule_height = 10.6 *mm;
	G4Tubs *SourceCapsule = new G4Tubs("SourceCapsule",	SourceCapsule_iD/2, SourceCapsule_oD/2, SourceCapsule_height/2, opendeg, closedeg);

	G4double SourceCapsuleThread_iD 	= 0.0 *mm;
	G4double SourceCapsuleThread_oD 	= 3.3 *mm;
	G4double SourceCapsuleThread_height = 7.0 *mm;
	G4Tubs *SourceCapsuleThread = new G4Tubs("SourceCapsuleThread",	SourceCapsuleThread_iD/2, SourceCapsuleThread_oD/2, SourceCapsuleThread_height/2, opendeg, closedeg);

	G4double SourceActiveRegion_iD 	= 0.0 *mm;
	G4double SourceActiveRegion_oD 	= 4.0 *mm;
	G4double SourceActiveRegion_height = 4.0 *mm;
	G4Tubs *SourceActiveRegion = new G4Tubs("SourceActiveRegion",	SourceActiveRegion_iD/2, SourceActiveRegion_oD/2, SourceActiveRegion_height/2, opendeg, closedeg);


	//==============================================================================================
	//==============	Logical Volumes (declared in 'XuerichDetectorGeometry.hh')	================
	//==============================================================================================
	G4LogicalVolume *Laboratory_log					= new G4LogicalVolume( Laboratory, 				Air, 		"Laboratory_log");

	// NaI detector, Saint-Gibain mod. 3M3/3
	//G4LogicalVolume	*NaI_Crystal_log				= new G4LogicalVolume( NaI_Crystal,				NaI,		"NaI_Crystal_log");
	G4LogicalVolume	*NaI_Crystal_log				= new G4LogicalVolume( NaI_Crystal,				CsI,		"NaI_Crystal_log");
	//G4LogicalVolume	*NaI_Crystal_log				= new G4LogicalVolume( NaI_Crystal,				BGO,		"NaI_Crystal_log");
	G4LogicalVolume	*NaI_CrystalDent_log			= new G4LogicalVolume( NaI_CrystalDent,			Air,		"NaI_CrystalDent_log");
	G4LogicalVolume	*NaI_CrystalHousingTop_log		= new G4LogicalVolume( NaI_CrystalHousingTop,	metalAl,	"NaI_CrystalHousingTop_log");
	G4LogicalVolume	*NaI_CrystalHousingBot_log		= new G4LogicalVolume( NaI_CrystalHousingBot,	metalAl,	"NaI_CrystalHousingBot_log");
	G4LogicalVolume	*NaI_LightShield_log			= new G4LogicalVolume( NaI_LightShield,			MuMetal,	"NaI_LightShield_log");
	G4LogicalVolume	*NaI_LightShieldVac_log			= new G4LogicalVolume( NaI_LightShieldVac,		Vacuum,		"NaI_LightShieldVac_log");

	G4LogicalVolume	*SourceCapsule_log				= new G4LogicalVolume( SourceCapsule,			StainlessSteel,		"SourceCapsule_log");
	G4LogicalVolume	*SourceCapsuleThread_log		= new G4LogicalVolume( SourceCapsuleThread,		StainlessSteel,		"SourceCapsule_log");
	G4LogicalVolume	*SourceActiveRegion_log			= new G4LogicalVolume( SourceActiveRegion,		Ceramics,			"SourceActiveRegion_log");



	////////////////////////////////////////////
	// NaI detector, Saint-Gibain mod. 3M3/3
	// built from bottom to top (everything related to the center posiiton of the crystal)
	G4double NaI_x	= 0.0 *mm;
	G4double NaI_y	= 0.0 *mm;
	G4double NaI_z	= 0.0 *mm;

	// vertical arrangement, facing bottom
	G4double NaI_Crystal_offsetX = NaI_x;
	G4double NaI_Crystal_offsetY = NaI_y;
	G4double NaI_Crystal_offsetZ = NaI_z;

	G4double NaI_CrystalDent_offsetX = 0.0*mm;
	G4double NaI_CrystalDent_offsetY = 0.0*mm;
	G4double NaI_CrystalDent_offsetZ = -NaI_Crystal_height/2 + NaI_CrystalDent_depth/2;

	G4double NaI_CrystalHousingTop_offsetX 		= NaI_Crystal_offsetX;
	G4double NaI_CrystalHousingTop_offsetY 		= NaI_Crystal_offsetY;
	G4double NaI_CrystalHousingTop_offsetZ 		= NaI_Crystal_offsetZ;

	G4double NaI_CrystalHousingBot_offsetX 		= NaI_Crystal_offsetX;
	G4double NaI_CrystalHousingBot_offsetY 		= NaI_Crystal_offsetY;
	G4double NaI_CrystalHousingBot_offsetZ 		= NaI_CrystalHousingTop_offsetZ - NaI_CrystalHousingTop_height/2 - NaI_CrystalHousingBot_height/2;

	G4double NaI_LightShield_offsetX 			= NaI_Crystal_offsetX;
	G4double NaI_LightShield_offsetY 			= NaI_Crystal_offsetY;
	G4double NaI_LightShield_offsetZ 			= NaI_CrystalHousingTop_offsetZ + NaI_CrystalHousingTop_height/2 + NaI_LightShieldBot_height/2;

	G4double NaI_LightShieldVac_offsetX 		= NaI_Crystal_offsetX;
	G4double NaI_LightShieldVac_offsetY 		= NaI_Crystal_offsetY;
	G4double NaI_LightShieldVac_offsetZ 		= NaI_CrystalHousingTop_offsetZ + NaI_CrystalHousingTop_height/2 + NaI_LightShieldBot_height/2;

	// NaI
	G4PVPlacement *Laboratory_phys				= new G4PVPlacement(0, G4ThreeVector(0, 0, 0), Laboratory_log, "Laboratory_phys", 0, false, 0);
	G4PVPlacement *NaI_Crystal_phys				= new G4PVPlacement(0, G4ThreeVector(NaI_Crystal_offsetX, NaI_Crystal_offsetY, NaI_Crystal_offsetZ), NaI_Crystal_log, "NaI_Crystal_phys", Laboratory_log, false, 0);
	G4PVPlacement *NaI_CrystalDent_phys			= new G4PVPlacement(0, G4ThreeVector(NaI_CrystalDent_offsetX, NaI_CrystalDent_offsetY, NaI_CrystalDent_offsetZ), NaI_CrystalDent_log, "NaI_CrystalDent_phys", NaI_Crystal_log, false, 0);
	G4PVPlacement *NaI_CrystalHousingTop_phys	= new G4PVPlacement(0, G4ThreeVector(NaI_CrystalHousingTop_offsetX, NaI_CrystalHousingTop_offsetY, NaI_CrystalHousingTop_offsetZ), NaI_CrystalHousingTop_log, "NaI_CrystalHousingTop_phys", Laboratory_log, false, 0);
	G4PVPlacement *NaI_CrystalHousingBot_phys	= new G4PVPlacement(0, G4ThreeVector(NaI_CrystalHousingBot_offsetX, NaI_CrystalHousingBot_offsetY, NaI_CrystalHousingBot_offsetZ), NaI_CrystalHousingBot_log, "NaI_CrystalHousingBot_phys", Laboratory_log, false, 0);
	G4PVPlacement *NaI_LightShield_phys			= new G4PVPlacement(0, G4ThreeVector(NaI_LightShield_offsetX, NaI_LightShield_offsetY, NaI_LightShield_offsetZ), NaI_LightShield_log, "NaI_LightShield_phys", Laboratory_log, false, 0);
	G4PVPlacement *NaI_LightShieldVac_phys		= new G4PVPlacement(0, G4ThreeVector(NaI_LightShieldVac_offsetX, NaI_LightShieldVac_offsetY, NaI_LightShieldVac_offsetZ), NaI_LightShieldVac_log, "NaI_LightShieldVac_phys", Laboratory_log, false, 0);

	G4double SourceCapsule_offsetX = 0.0*mm;
	G4double SourceCapsule_offsetY = 0.0*mm;
	G4double SourceCapsule_offsetZ = NaI_CrystalDent_depth/2 - SourceCapsule_height/2 - 1.0*mm;

	G4double SourceCapsuleThread_offsetX = 0.0*mm;
	G4double SourceCapsuleThread_offsetY = 0.0*mm;
	G4double SourceCapsuleThread_offsetZ = SourceCapsule_offsetZ - SourceCapsule_height/2 - SourceCapsuleThread_height/2;

	G4double SourceActiveRegion_offsetX = 0.0*mm;
	G4double SourceActiveRegion_offsetY = 0.0*mm;
	G4double SourceActiveRegion_offsetZ = SourceCapsule_height/2 - SourceActiveRegion_height/2 -  1.2*mm;

	G4PVPlacement *SourceCapsule_phys			= new G4PVPlacement(0, G4ThreeVector(SourceCapsule_offsetX, SourceCapsule_offsetY, SourceCapsule_offsetZ), SourceCapsule_log, "SourceCapsule_phys", NaI_CrystalDent_log, false, 0);
	G4PVPlacement *SourceCapsuleThread_phys		= new G4PVPlacement(0, G4ThreeVector(SourceCapsuleThread_offsetX, SourceCapsuleThread_offsetY, SourceCapsuleThread_offsetZ), SourceCapsuleThread_log, "SourceCapsuleThread_phys", NaI_CrystalDent_log, false, 0);
	G4PVPlacement *SourceActiveRegion_phys		= new G4PVPlacement(0, G4ThreeVector(SourceActiveRegion_offsetX, SourceActiveRegion_offsetY, SourceActiveRegion_offsetZ), SourceActiveRegion_log, "SourceActiveRegion_phys", SourceCapsule_log, false, 0);


	////////////////////////////////////////////////////////////////////////
	// VISUAL ATTRIBUTES
	////////////////////////////////////////////////////////////////////////
	Laboratory_log			->SetVisAttributes(G4VisAttributes::Invisible);
	NaI_LightShieldVac_log	->SetVisAttributes(G4VisAttributes::Invisible);

	G4VisAttributes	*NaI_Crystal_vis = new G4VisAttributes(xlred);
	NaI_Crystal_vis	->SetVisibility(true);
	NaI_Crystal_vis	->SetForceSolid(false);
		NaI_Crystal_log	->SetVisAttributes(NaI_Crystal_vis);

	G4VisAttributes	*NaI_CrystalHousing_vis = new G4VisAttributes(lblue);
	NaI_CrystalHousing_vis	->SetVisibility(true);
	NaI_CrystalHousing_vis	->SetForceSolid(false);
		NaI_CrystalHousingTop_log	->SetVisAttributes(NaI_CrystalHousing_vis);
		NaI_CrystalHousingBot_log	->SetVisAttributes(NaI_CrystalHousing_vis);
		NaI_CrystalDent_log			->SetVisAttributes(NaI_CrystalHousing_vis);

	G4VisAttributes	*NaI_CrystalDent_vis = new G4VisAttributes(white);
	NaI_CrystalDent_vis	->SetVisibility(true);
	NaI_CrystalDent_vis	->SetForceSolid(false);
		NaI_CrystalDent_log			->SetVisAttributes(NaI_CrystalHousing_vis);

	G4VisAttributes	*NaI_LightShield_vis = new G4VisAttributes(blue);
	NaI_LightShield_vis	->SetVisibility(true);
	NaI_LightShield_vis	->SetForceSolid(false);
		NaI_LightShield_log	->SetVisAttributes(NaI_LightShield_vis);

	G4VisAttributes	*SourceCapsule_vis = new G4VisAttributes(lgreen);
	SourceCapsule_vis	->SetVisibility(true);
	SourceCapsule_vis	->SetForceSolid(false);
		SourceCapsule_log		->SetVisAttributes(SourceCapsule_vis);
		SourceCapsuleThread_log	->SetVisAttributes(SourceCapsule_vis);

	G4VisAttributes	*SourceActiveRegion_vis = new G4VisAttributes(magenta);
	SourceActiveRegion_vis	->SetVisibility(true);
	SourceActiveRegion_vis	->SetForceSolid(false);
		SourceActiveRegion_log		->SetVisAttributes(SourceActiveRegion_vis);


	// SENSITIVE DETECTOR
	G4SDManager *pSDManager = G4SDManager::GetSDMpointer();
	// NaI
	NaISensitiveDetector *pNaISD = new NaISensitiveDetector("NaISD");
	pSDManager->AddNewDetector(pNaISD);
	NaI_Crystal_log	->SetSensitiveDetector(pNaISD);




//--------------	END PROGRAM	------------------------
return Laboratory_phys;
//------------------------------------------------------

}


#include <G4UnitsTable.hh>
#include <G4VVisManager.hh>
#include <G4Circle.hh>
#include <G4Colour.hh>
#include <G4VisAttributes.hh>

#include "NaIHit.hh"

G4Allocator<NaIHit> NaIHitAllocator;

NaIHit::NaIHit() {}

NaIHit::~NaIHit()
{
	if(m_pParticleType) delete m_pParticleType;
	if(m_pParentType) delete m_pParentType;
	if(m_pCreatorProcess) delete m_pCreatorProcess;
	if(m_pDepositingProcess) delete m_pDepositingProcess;
}

NaIHit::NaIHit(const NaIHit &hNaIHit):G4VHit()
{
	m_iTrackId = hNaIHit.m_iTrackId;
	m_iParentId = hNaIHit.m_iParentId;
	m_pParticleType = hNaIHit.m_pParticleType;
	m_pParentType = hNaIHit.m_pParentType ;
	m_pCreatorProcess = hNaIHit.m_pCreatorProcess ;
	m_pDepositingProcess = hNaIHit.m_pDepositingProcess ;
	m_hPosition = hNaIHit.m_hPosition;
	m_dEnergyDeposited = hNaIHit.m_dEnergyDeposited;
	m_dKineticEnergy = hNaIHit.m_dKineticEnergy ;
	m_dTime = hNaIHit.m_dTime;
}

const NaIHit &
NaIHit::operator=(const NaIHit &hNaIHit)
{
	m_iTrackId = hNaIHit.m_iTrackId;
	m_iParentId = hNaIHit.m_iParentId;
	m_pParticleType = hNaIHit.m_pParticleType;
	m_pParentType = hNaIHit.m_pParentType ;
	m_pCreatorProcess = hNaIHit.m_pCreatorProcess ;
	m_pDepositingProcess = hNaIHit.m_pDepositingProcess ;
	m_hPosition = hNaIHit.m_hPosition;
	m_dEnergyDeposited = hNaIHit.m_dEnergyDeposited;
	m_dKineticEnergy = hNaIHit.m_dKineticEnergy ;
	m_dTime = hNaIHit.m_dTime;
	
	return *this;
}

G4int
NaIHit::operator==(const NaIHit &hNaIHit) const
{
	return ((this == &hNaIHit) ? (1) : (0));
}

void NaIHit::Draw()
{
	G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
	
	if(pVVisManager)
	{
		G4Circle hCircle(m_hPosition);
		G4Colour hColour(1.000, 0.973, 0.184);
		G4VisAttributes hVisAttributes(hColour);
		
		hCircle.SetScreenSize(0.1);
		hCircle.SetFillStyle(G4Circle::filled);
		hCircle.SetVisAttributes(hVisAttributes);
		pVVisManager->Draw(hCircle);
	}
}

void NaIHit::Print()
{
	G4cout << "-------------------- NaI hit --------------------" 
		<< "Id: " << m_iTrackId
		<< " Particle: " << *m_pParticleType
		<< " ParentId: " << m_iParentId
		<< " ParentType: " << *m_pParentType << G4endl
		<< "CreatorProcess: " << *m_pCreatorProcess
		<< " DepositingProcess: " << *m_pDepositingProcess << G4endl
		<< "Position: " << m_hPosition.x()/mm
		<< " " << m_hPosition.y()/mm
		<< " " << m_hPosition.z()/mm
		<< " mm" << G4endl
		<< "EnergyDeposited: " << m_dEnergyDeposited/keV << " keV"
		<< " KineticEnergyLeft: " << m_dKineticEnergy/keV << " keV"
		<< " Time: " << m_dTime/s << " s" << G4endl;
}


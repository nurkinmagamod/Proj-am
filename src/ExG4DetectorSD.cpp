//#include<G4Step.hh>
#include<fstream>
#include<iostream>

#include "ExG4DetectorSD.hh"
#include "G4SystemOfUnits.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4VProcess.hh"

// Используем пространство имен std, что бы не писать много где std::
//using namespace std;

int nofgma = 0;
int d1 = 0;
int d2 = 0;
int d3 = 0;
int d4 = 0;
int d5 = 0;
int d6 = 0;
int d7 = 0;
int d8 = 0;

double enel = 0;
	double enpos = 0;
	double energyt = 0;

int histogram1[5000];
//int histogram2[1000];
//int histogram3[1000];
//int histogram4[1000];
//int histogram5[1000];
//int histogram6[1000];
//int histogram7[1000];
//int histogram8[1000];
int histogram0[5000];
    
ExG4DetectorSD::ExG4DetectorSD(
                            const G4String& name, 
                            const G4String& hitsCollectionName,
                            G4int nofCells)
 : G4VSensitiveDetector(name),
   fHitsCollection(nullptr),
   fNofCells(nofCells),
	HIST_MAX(20.*MeV),
	HIST_MIN(0.*MeV)
{
  collectionName.insert(hitsCollectionName);
	   std::ofstream file1("testdet.dat", std::ios::trunc);
    	file1.close();
		std::ofstream filed1("det1.dat", std::ios::trunc);
    	filed1.close();
		std::ofstream filed2("det2.dat", std::ios::trunc);
    	filed2.close();
		std::ofstream filed3("det3.dat", std::ios::trunc);
    	filed3.close();
		std::ofstream filed4("det4.dat", std::ios::trunc);
    	filed4.close();
		std::ofstream filed5("det5.dat", std::ios::trunc);
    	filed5.close();
		std::ofstream filed6("det6.dat", std::ios::trunc);
    	filed6.close();
		std::ofstream filed7("det7.dat", std::ios::trunc);
    	filed7.close();
		std::ofstream filed8("det8.dat", std::ios::trunc);
    	filed8.close();
		enel = 0;
	enpos = 0;
	energyt = 0;
}

//ExG4DetectorSD::~ExG4DetectorSD() 
//{ 
//}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExG4DetectorSD::Initialize(G4HCofThisEvent* hce)
{
  // Create hits collection
  fHitsCollection 
    = new ExG4DetectorHitsCollection(SensitiveDetectorName, collectionName[0]); 

  // Add this collection in hce
  auto hcID 
    = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection( hcID, fHitsCollection ); 

  // Create hits
  // fNofCells for cells + one more for total sums 
  for (G4int i=0; i<fNofCells+1; i++ ) {
    fHitsCollection->insert(new ExG4DetectorHit());
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool ExG4DetectorSD::ProcessHits(G4Step* step, 
                                     G4TouchableHistory*)
{  
	auto touchable = (step->GetPreStepPoint()->GetTouchable());
	auto touchableVol = touchable->GetVolume()->GetName();
	auto partname = step->GetTrack()->GetDefinition()->GetParticleName();
	double bin_width = (HIST_MAX - HIST_MIN) / 5000;
	double energy = step->GetPreStepPoint()->GetKineticEnergy();
	double energy1 = step->GetPreStepPoint()->GetKineticEnergy();
	double energy2 = step->GetPostStepPoint()->GetKineticEnergy();
	
	auto atrack = step -> GetTrack();
	auto trackid = atrack -> GetTrackID();
	auto parid = atrack -> GetParentID();
	auto stepno = atrack -> GetCurrentStepNumber();
	
	//auto creat = atrack -> GetCreatorProcess();
	//auto procname = creat->GetProcessName();
	
	
	//gamma spectrum
	if (partname == "gamma" && energy1 != energy2) {
	int index1 = int(floor((energy-HIST_MIN)/bin_width/MeV));
   		  		 	if(index1 >= 0 && index1 < 5000){
    		 			histogram1[index1]++;
					}
	}
	
	//spectrum
	if (trackid != 1 && parid != 0) {
		if (parid == 1) {
			if (trackid == 2 && stepno == 1) {
				enel = energy;
			}
			if (trackid == 3 && stepno == 1) {
				enpos = energy;
			}
			energyt = enel + enpos;
		}
		else {
			energyt = energy;
		}
		int index0 = int(floor((energyt-HIST_MIN)/bin_width/MeV));
   		  		 	if(index0 >= 0 && index0 < 5000){
    		 			histogram0[index0]++;
					}
	}
	else {
			enel = 0;
			enpos = 0;
		}
	//detectors
				if (touchableVol == "Detector1") {
					std::ofstream filed1("det1.dat", std::ios::app);
					if (trackid != 1 && parid != 0) {
						auto creat = atrack -> GetCreatorProcess();
						auto procname = creat->GetProcessName();
        				filed1 << energy/MeV << " " << partname << " " << procname << " " << trackid << " " << parid << " " << stepno << std::endl;
					}
					else {
						filed1 << energy/MeV << " " << partname << " " << "primary" << std::endl;
					}
    				filed1.close();
				}
				if (touchableVol == "Detector2") {
					std::ofstream filed2("det2.dat", std::ios::app);
        			if (trackid != 1 && parid != 0) {
						auto creat = atrack -> GetCreatorProcess();
						auto procname = creat->GetProcessName();
        				filed2 << energy/MeV << " " << partname << " " << procname << std::endl;
					}
					else {
						filed2 << energy/MeV << " " << partname << " " << "primary" << std::endl;
					}
    				filed2.close();
				}
				if (touchableVol == "Detector3") {
					std::ofstream filed3("det3.dat", std::ios::app);
        			if (trackid != 1 && parid != 0) {
						auto creat = atrack -> GetCreatorProcess();
						auto procname = creat->GetProcessName();
        				filed3 << energy/MeV << " " << partname << " " << procname << std::endl;
					}
					else {
						filed3 << energy/MeV << " " << partname << " " << "primary" << std::endl;
					}
    				filed3.close();
				}
				if (touchableVol == "Detector4") {
					std::ofstream filed4("det4.dat", std::ios::app);
        			if (trackid != 1 && parid != 0) {
						auto creat = atrack -> GetCreatorProcess();
						auto procname = creat->GetProcessName();
        				filed4 << energy/MeV << " " << partname << " " << procname << std::endl;
					}
					else {
						filed4 << energy/MeV << " " << partname << " " << "primary" << std::endl;
					}
    				filed4.close();
				}
				if (touchableVol == "Detector5") {
					std::ofstream filed5("det5.dat", std::ios::app);
        			if (trackid != 1 && parid != 0) {
						auto creat = atrack -> GetCreatorProcess();
						auto procname = creat->GetProcessName();
        				filed5 << energy/MeV << " " << partname << " " << procname << std::endl;
					}
					else {
						filed5 << energy/MeV << " " << partname << " " << "primary" << std::endl;
					}
    				filed5.close();
				}
				if (touchableVol == "Detector6") {
					std::ofstream filed6("det6.dat", std::ios::app);
        			if (trackid != 1 && parid != 0) {
						auto creat = atrack -> GetCreatorProcess();
						auto procname = creat->GetProcessName();
        				filed6 << energy/MeV << " " << partname << " " << procname << std::endl;
					}
					else {
						filed6 << energy/MeV << " " << partname << " " << "primary" << std::endl;
					}
    				filed6.close();
				}
				if (touchableVol == "Detector7") {
					std::ofstream filed7("det7.dat", std::ios::app);
        			if (trackid != 1 && parid != 0) {
						auto creat = atrack -> GetCreatorProcess();
						auto procname = creat->GetProcessName();
        				filed7 << energy/MeV << " " << partname << " " << procname << std::endl;
					}
					else {
						filed7 << energy/MeV << " " << partname << " " << "primary" << std::endl;
					}
    				filed7.close();
				}
				if (touchableVol == "Detector8") {
					std::ofstream filed8("det8.dat", std::ios::app);
        			if (trackid != 1 && parid != 0) {
						auto creat = atrack -> GetCreatorProcess();
						auto procname = creat->GetProcessName();
        				filed8 << energy/MeV << " " << partname << " " << procname << std::endl;
					}
					else {
						filed8 << energy/MeV << " " << partname << " " << "primary" << std::endl;
					}
    				filed8.close();
				}
	
	//efficiency
	if(step->GetTrack()->GetDefinition()->GetParticleName() == "gamma" ){
		if(energy >= 19.6*MeV){
			if(energy1 != energy2) {
				std::ofstream file1("testdet.dat", std::ios::app);
        		file1 << std::setw(15) << touchableVol << " " << std::endl;
    			file1.close();
		 		nofgma = nofgma + 1;
				if (touchableVol == "Detector1") {
					d1++;
				}
				if (touchableVol == "Detector2") {
					d2++;
				}
				if (touchableVol == "Detector3") {
					d3++;
				}
				if (touchableVol == "Detector4") {
					d4++;
				}
				if (touchableVol == "Detector5") {
					d5++;
				}
				if (touchableVol == "Detector6") {
					d6++;
				}
				if (touchableVol == "Detector7") {
					d7++;
				}
				if (touchableVol == "Detector8") {
					d8++;
				}
			}
		}
	}
	
	//particleKiller
	if (energy1 != energy2) {
		if (trackid != 1 && parid != 0) {
						auto creat = atrack -> GetCreatorProcess();
						auto procname = creat->GetProcessName();
			if (procname != "annihil") {
				step->GetTrack()->SetTrackStatus(fPostponeToNextEvent);
			}
			else {
				step->GetTrack()->SetTrackStatus(fStopButAlive);
			}
		}
		else {
			step->GetTrack()->SetTrackStatus(fStopAndKill);
		}
	}
  // energy deposit
 // auto edep = step->GetTotalEnergyDeposit();
  
  // step length
  //G4double stepLength = 0.;
  //if ( step->GetTrack()->GetDefinition()->GetPDGCharge() != 0. ) {
   // stepLength = step->GetStepLength();
  //}

  //if ( edep==0. && stepLength == 0. ) return false;      

  
    
  // Get Detector cell id 
  //auto layerNumber = touchable->GetReplicaNumber(1);
  
  // Get hit accounting data for this cell
  //auto hit = (*fHitsCollection)[layerNumber];
  //if ( ! hit ) {
  //  G4ExceptionDescription msg;
   // msg << "Cannot access hit " << layerNumber; 
   // G4Exception("ExG4DetectorSD::ProcessHits()",
    //  "MyCode0004", FatalException, msg);
  //}         

  // Get hit for total accounting
  //auto hitTotal 
  //  = (*fHitsCollection)[fHitsCollection->entries()-1];
  
  // Add values
  //hit->Add(edep, stepLength);
  //hitTotal->Add(edep, stepLength); 
      
  return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExG4DetectorSD::EndOfEvent(G4HCofThisEvent*)
{
  //if ( verboseLevel>1 ) { 
   // auto nofHits = fHitsCollection->entries();
   //  G4cout
    //   << G4endl 
    //   << "-------->Hits Collection: in this event they are " << nofHits 
    //   << " hits in the tracker chambers: " << G4endl;
    // for ( G4int i=0; i<nofHits; i++ ) (*fHitsCollection)[i]->Print();
	
	  //std::ofstream file("test.dat");
        // Выводим в файл
        //file << std::setw(15) << nofHits << " " << std::endl;
    // Закрываем файл
    //file.close();
  //}
}

ExG4DetectorSD::~ExG4DetectorSD() 
{ 
	std::ofstream file("test.dat", std::ios::trunc);
        file << "D1" << " " << d1 << std::endl
			<< "D2" << " " << d2 << std::endl
			<< "D3" << " " << d3 << std::endl
			<< "D4" << " " << d4 << std::endl
			<< "D5" << " " << d5 << std::endl
			<< "D6" << " " << d6 << std::endl
			<< "D7" << " " << d7 << std::endl
			<< "D8" << " " << d8 << std::endl
			<< "total" << " " << nofgma << " " << std::endl;
    file.close();
	
	double bin_width = (HIST_MAX - HIST_MIN) / 5000;
	std::ofstream filed0("spectrum.dat", std::ios::trunc);
    for(int i = 0; i<NOBINS; i++)
    {
        double energy0 = i*bin_width + HIST_MIN;
        filed0 << std::setw(15) << energy0/MeV << " "
             << std::setw(15) << histogram0[i] << std::endl;
   }
	filed0.close();
	
	std::ofstream filed1("spectrum-only-gamma.dat", std::ios::trunc);
    for(int i = 0; i<NOBINS; i++)
    {
        double energy1 = i*bin_width + HIST_MIN;
        filed1 << std::setw(15) << energy1/MeV << " "
             << std::setw(15) << histogram1[i] << std::endl;
   }
	filed1.close();
}

//ExG4DetectorSD::ExG4DetectorSD(const G4String name,
//								const G4String& hitsCollectionName): G4VSensitiveDetector(name),
//								 fHitsCollection(nullptr),
 //                            HIST_MAX(25*MeV),// Инициализация верхней границы
  //                           HIST_MIN(0 *MeV)// Инициализация нижней границы				 
//{
  // Обнуляем гистограммы
 // for(int i = 0; i<NOBINS; i++){
 //   histogram[i] = 0;
 // }
//}
//Вызывается на каждом шаге моделирования частицы, когда она попадает в этот чувствительный объем
//G4bool ExG4DetectorSD::ProcessHits(G4Step* step, G4TouchableHistory* history)
 //  {
    // Получаем кинетическую энергии частицы с предыдущего шага, т.е. начальную
    // кинетическую энегрию перед текущим шагом
  //  double energy = step->GetPreStepPoint()->GetKineticEnergy();
    // Вычисляем ширину бина (интерва) гистограммы
   // double bin_width = (HIST_MAX - HIST_MIN) / NOBINS;
    // Если имя частицы gamma, тогда заполняем гистограммы
  //  if(step->GetTrack()->GetDefinition()->GetParticleName() == "gamma" ){
   //      // Определяем индекс (номер) бина гистограммы энергии
    //     int index = int(floor((energy-HIST_MIN)/bin_width));
         // Добавляем +1 в соответствующий бин
   //      if(index >= 0 && index < NOBINS)
    //       histogram[index]++;
//}
     // Так как мы хотим только измерить параметры частиц после прохождения
     // мишени и не интересуемся дальнейшей их судьбой в детекторе, то их убиваем -
     // устанавливаем статус остановлено и уничтожено (fStopAndKill)
     //step->GetTrack()->SetTrackStatus(fStopAndKill);
     //return true;
  // }
//Вызывается на каждом шаге моделирования частицы, когда она попадает в этот чувствительный объем

//ExG4DetectorSD::~ExG4DetectorSD()
//{
    // В деструкторе выводим гистограммы в файлы
    // Открываем файл (существующий файл полностью перезаписывается)
//	std::ofstream file("test.dat");
        // Выводим в файл
    //    file << std::setw(15) << nofgma << " " << std::endl;
    // Закрываем файл
  //  file.close();
	//
  //  file.open("spectrum.dat");
    // Вычисляем ширину бина
   // double bin_width = (HIST_MAX - HIST_MIN) / NOBINS;
    // Выводим гистограмму
  //  for(int i = 0; i<NOBINS; i++)
  //  {
        // Вычисляем энергию
  //      double energy = i*bin_width + HIST_MIN;
        // Выводим в файл
 //       file << std::setw(15) << energy/MeV << " "
 //            << std::setw(15) << histogram[i] << std::endl;
 //   }
    // Закрываем файл
//    file.close();		
//}

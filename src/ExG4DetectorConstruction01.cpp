#include "ExG4DetectorConstruction01.hh"
#include "ExG4DetectorSD.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4SystemOfUnits.hh"
#include "G4RotationMatrix.hh"


// Конструктор класса объявления материалов и геометрии всей моделируемой системы
ExG4DetectorConstruction01::ExG4DetectorConstruction01()
: G4VUserDetectorConstruction(),
fNofLayers(-1)
{ }

// Деструктор
ExG4DetectorConstruction01::~ExG4DetectorConstruction01()
{ }

// Функция определения материалов и геометрии всей системы,
// должна возвращать физический объем - ссылку на экземпляр класса G4VPhysicalVolume
// Геометрию проектировать будем следующую: пучок протонов попадает на мишень
// вольфрамовый лист толщиной около 1 мм, а за мишень поставим детектор
// таких же размеров, он будет регистрировать что в него попало.
G4VPhysicalVolume* ExG4DetectorConstruction01::Construct()
{
    fNofLayers = 2;
    // Для простоты используем предопределенные в Geant4 материалы
    // Так объявляется менеджер, из которого можно извлечь
    // ранее предопределенные материалы
  G4NistManager* nist = G4NistManager::Instance();

  // Параметры детектора
  G4double det_sizeX = 20*cm, det_sizeY = 20*cm, det_sizeZ = 40*cm;

  // Материал детектора, здесь выбираем натрий
  G4Material* det_mat = nist->FindOrBuildMaterial("G4_SODIUM_IODIDE");

    //Tube material
    G4Material* tube_mat = nist->FindOrBuildMaterial("G4_STAINLESS-STEEL");
    
  // Опция для включения/выключения проверки перекрытия объемов
  G4bool checkOverlaps = true;

  // World
  // Объем мира, самый большой объем, включающий остальные, аналог экспериментального
  // зала
  G4double world_sizeX = 200*cm;
  G4double world_sizeY = 200*cm;  //Размер по x и y здесь будут одинаковы - ширина и высота
  G4double world_sizeZ = 200*cm;//Размер по z - толщина
  // Выбор материала для мира из предопределенных в Geant4, для зала берем воздух
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");
  G4Material* vac_mat = nist->FindOrBuildMaterial("G4_Galactic");
  // Создание объема для мира (экспериментального зала), определяется сама форма объема,
  // берем параллелепипед, это просто геометрическая фигура
  G4Box* solidWorld =
  new G4Box("World",                       //its name, название объема
       0.5*world_sizeX, 0.5*world_sizeY, 0.5*world_sizeZ);     //its size, его размеры
  // указываются половины размеров высоты, ширины и глубины

  // Логический объем, здесь подключается материал, из которого сделан объем
  G4LogicalVolume* logicWorld =
    new G4LogicalVolume(solidWorld,          //its solid, геометрический объем, объявлен выше
                        world_mat,           //its material, материал объема
                        "World");            //its name, название логического объема
  //совпадает с названием объема, но
  //для Geant4 это разные объекты
  //геометрический объем и логический объем

  //Физический объем, а теперь наш логический объем помещаем в "реальный" мир
  G4VPhysicalVolume* physWorld =
    new G4PVPlacement(0,                     //no rotation, нет вращения
                      G4ThreeVector(),       //at (0,0,0), расположение в центре (0,0,0)
                      logicWorld,            //its logical volume, логический объем этого физического
                      "World",               //its name, название физического объема
                      0,                     //its mother  volume, материнский объем, этот самый первый, поэтому 0
                      false,                 //no boolean operation, без логических (булевых) операций
                      0,                     //copy number, номер копии
                      checkOverlaps);        //overlaps checking, флаг проверки перекрытия объемов
    //Tube
    G4Tubs* solidTube =
    new G4Tubs("Tube", 
               12.8*cm, 13.*cm, 40.*cm, 0.*deg, 360.*deg);
  //Логический объем
  G4LogicalVolume* logicTube =
    new G4LogicalVolume(solidTube,            //its solid, объем
                        tube_mat,             //its material, указываем материал мишени
                        "Tube");         //its name, его имя

  //Физический объем мишени
  G4Rep3x3 rottemp(-1,0,0, // matrix representation
    0,1,0, // (inverted)
    0,0,1);
    G4RotationMatrix* rotm  = new G4RotationMatrix(rottemp);
    new G4PVPlacement(rotm,                       //no rotation, так же без вращения
                    G4ThreeVector(0.*cm,0.,30.*cm),//at (0,0,-5 см) положение центра мишени в другую сторону от детектора, смещена на 5 см от центра объема World
                    logicTube,                //its logical volume, подключаем логический объем
                    "Tube",                //its name, имя физического объема
                    logicWorld,              //its mother  volume, родительский логический объем!
                    false,                   //no boolean operation, без булевых операций
                    0,                       //copy number, номер копии
                    checkOverlaps);
    
    
  // Детектор, для него также используем параллелепипед
  G4Box* solidDetVol =
    new G4Box("Detector",                    //its name, имя
        0.5*det_sizeX, 0.5*det_sizeY, 0.5*det_sizeZ); //its size, размеры

  //Логический объем
  G4LogicalVolume* logicDetVol =
    new G4LogicalVolume(solidDetVol,            //its solid, объем
                        det_mat,             //its material, указываем материал детектора
                        "DetectorVol");         //its name, его имя

  //Физический объем детектора
  new G4PVPlacement(0,                       //no rotation, так же без вращения
                    G4ThreeVector(23*cm,0,0*cm), //at (0,0,5 см) положение центра детектора, он смещен на 5 см от центра объема World
                    logicDetVol,                //its logical volume, подключаем логический объем
                    "DetectorVol",              //its name, имя физического объема
                    logicWorld,              //its mother  volume, родительский логический объем, помещаем в world!
                    false,                   //no boolean operation, без булевых операций
                    0,                       //copy number, номер копии
                    checkOverlaps);          //overlaps checking, флаг проверки перекрытия объемов

  //Логический объем
  G4LogicalVolume* logicDetVol2 =
    new G4LogicalVolume(solidDetVol,            //its solid, объем
                        det_mat,             //its material, указываем материал детектора
                        "DetectorVol");         //its name, его имя  
  //Физический объем детектора
 new G4PVPlacement(0,                       //no rotation, так же без вращения
                   G4ThreeVector(-23*cm,0,0*cm), //at (0,0,5 см) положение центра детектора, он смещен на 5 см от центра объема World
                   logicDetVol2,                //its logical volume, подключаем логический объем
                   "Detector2Vol",              //its name, имя физического объема
                   logicWorld,              //its mother  volume, родительский логический объем, помещаем в world!
                   false,                   //no boolean operation, без булевых операций
                    0,                       //copy number, номер копии
                   checkOverlaps);          //overlaps checking, флаг проверки перекрытия объемов
    
    
   G4Box* solidDet =
    new G4Box("Detector1",                    //its name, имя
        0.25*det_sizeX, 0.25*det_sizeY, 0.5*det_sizeZ); //its size, размеры

  //Логический объем
  G4LogicalVolume* logicDet1 =
    new G4LogicalVolume(solidDet,            //its solid, объем
                        det_mat,             //its material, указываем материал детектора
                        "Detector1");         //its name, его имя

  //Физический объем детектора
  new G4PVPlacement(0,                       //no rotation, так же без вращения
                    G4ThreeVector(-0.25*det_sizeX,-0.25*det_sizeY,0), //at (0,0,5 см) положение центра детектора, он смещен на 5 см от центра объема World
                    logicDet1,                //its logical volume, подключаем логический объем
                    "Detector1",              //its name, имя физического объема
                    logicDetVol,              //its mother  volume, родительский логический объем, помещаем в world!
                    false,                   //no boolean operation, без булевых операций
                    0,                       //copy number, номер копии
                    checkOverlaps);          //overlaps checking, флаг проверки перекрытия объемов
    
      //Логический объем
  G4LogicalVolume* logicDet2 =
    new G4LogicalVolume(solidDet,            //its solid, объем
                        det_mat,             //its material, указываем материал детектора
                        "Detector2");         //its name, его имя

  //Физический объем детектора
  new G4PVPlacement(0,                       //no rotation, так же без вращения
                    G4ThreeVector(0.25*det_sizeX,-0.25*det_sizeY,0), //at (0,0,5 см) положение центра детектора, он смещен на 5 см от центра объема World
                    logicDet2,                //its logical volume, подключаем логический объем
                    "Detector2",              //its name, имя физического объема
                    logicDetVol,              //its mother  volume, родительский логический объем, помещаем в world!
                    false,                   //no boolean operation, без булевых операций
                    0,                       //copy number, номер копии
                    checkOverlaps);          //overlaps checking, флаг проверки перекрытия объемов
    
      //Логический объем
  G4LogicalVolume* logicDet3 =
    new G4LogicalVolume(solidDet,            //its solid, объем
                        det_mat,             //its material, указываем материал детектора
                        "Detector3");         //its name, его имя

  //Физический объем детектора
  new G4PVPlacement(0,                       //no rotation, так же без вращения
                    G4ThreeVector(-0.25*det_sizeX,0.25*det_sizeY,0), //at (0,0,5 см) положение центра детектора, он смещен на 5 см от центра объема World
                    logicDet3,                //its logical volume, подключаем логический объем
                    "Detector3",              //its name, имя физического объема
                    logicDetVol,              //its mother  volume, родительский логический объем, помещаем в world!
                    false,                   //no boolean operation, без булевых операций
                    0,                       //copy number, номер копии
                    checkOverlaps);          //overlaps checking, флаг проверки перекрытия объемов
    
      //Логический объем
  G4LogicalVolume* logicDet4 =
    new G4LogicalVolume(solidDet,            //its solid, объем
                        det_mat,             //its material, указываем материал детектора
                        "Detector4");         //its name, его имя

  //Физический объем детектора
  new G4PVPlacement(0,                       //no rotation, так же без вращения
                    G4ThreeVector(0.25*det_sizeX,0.25*det_sizeY,0), //at (0,0,5 см) положение центра детектора, он смещен на 5 см от центра объема World
                    logicDet4,                //its logical volume, подключаем логический объем
                    "Detector4",              //its name, имя физического объема
                    logicDetVol,              //its mother  volume, родительский логический объем, помещаем в world!
                    false,                   //no boolean operation, без булевых операций
                    0,                       //copy number, номер копии
                    checkOverlaps);          //overlaps checking, флаг проверки перекрытия объемов
     //Логический объем
  G4LogicalVolume* logicDet5 =
    new G4LogicalVolume(solidDet,            //its solid, объем
                        det_mat,             //its material, указываем материал детектора
                        "Detector5");         //its name, его имя

  //Физический объем детектора
  new G4PVPlacement(0,                       //no rotation, так же без вращения
                    G4ThreeVector(-0.25*det_sizeX,-0.25*det_sizeY,0), //at (0,0,5 см) положение центра детектора, он смещен на 5 см от центра объема World
                    logicDet5,                //its logical volume, подключаем логический объем
                    "Detector5",              //its name, имя физического объема
                    logicDetVol2,              //its mother  volume, родительский логический объем, помещаем в world!
                    false,                   //no boolean operation, без булевых операций
                    0,                       //copy number, номер копии
                    checkOverlaps);          //overlaps checking, флаг проверки перекрытия объемов
     //Логический объем 
  G4LogicalVolume* logicDet6 =
    new G4LogicalVolume(solidDet,            //its solid, объем
                        det_mat,             //its material, указываем материал детектора
                        "Detector6");         //its name, его имя

  //Физический объем детектора
  new G4PVPlacement(0,                       //no rotation, так же без вращения
                    G4ThreeVector(0.25*det_sizeX,-0.25*det_sizeY,0), //at (0,0,5 см) положение центра детектора, он смещен на 5 см от центра объема World
                    logicDet6,                //its logical volume, подключаем логический объем
                    "Detector6",              //its name, имя физического объема
                    logicDetVol2,              //its mother  volume, родительский логический объем, помещаем в world!
                    false,                   //no boolean operation, без булевых операций
                    0,                       //copy number, номер копии
                    checkOverlaps);          //overlaps checking, флаг проверки перекрытия объемов
     //Логический объем
  G4LogicalVolume* logicDet7 =
    new G4LogicalVolume(solidDet,            //its solid, объем
                        det_mat,             //its material, указываем материал детектора
                        "Detector7");         //its name, его имя

  //Физический объем детектора
  new G4PVPlacement(0,                       //no rotation, так же без вращения
                    G4ThreeVector(-0.25*det_sizeX,0.25*det_sizeY,0), //at (0,0,5 см) положение центра детектора, он смещен на 5 см от центра объема World
                    logicDet7,                //its logical volume, подключаем логический объем
                    "Detector7",              //its name, имя физического объема
                    logicDetVol2,              //its mother  volume, родительский логический объем, помещаем в world!
                    false,                   //no boolean operation, без булевых операций
                    0,                       //copy number, номер копии
                    checkOverlaps);          //overlaps checking, флаг проверки перекрытия объемов
     //Логический объем
  G4LogicalVolume* logicDet8 =
    new G4LogicalVolume(solidDet,            //its solid, объем
                        det_mat,             //its material, указываем материал детектора
                        "Detector8");         //its name, его имя

  //Физический объем детектора
  new G4PVPlacement(0,                       //no rotation, так же без вращения
                    G4ThreeVector(0.25*det_sizeX,0.25*det_sizeY,0), //at (0,0,5 см) положение центра детектора, он смещен на 5 см от центра объема World
                    logicDet8,                //its logical volume, подключаем логический объем
                    "Detector8",              //its name, имя физического объема
                    logicDetVol2,              //its mother  volume, родительский логический объем, помещаем в world!
                    false,                   //no boolean operation, без булевых операций
                    0,                       //copy number, номер копии
                    checkOverlaps);          //overlaps checking, флаг проверки перекрытия объемов
    
  // Для мишени, на которую будет падать пучек, возьмем геометрические размеры как
  // у детектора, параллелепипед - лист вольфрама.
  G4Tubs* solidTar =
    new G4Tubs("Target", 
               0.*cm, 4.9*cm, 0.00006*cm, 0.*deg, 360.*deg);
  //Логический объем
  G4LogicalVolume* logicTar =
    new G4LogicalVolume(solidTar,            //its solid, объем
                        det_mat,             //its material, указываем материал мишени
                        "Target");         //its name, его имя

  // Физический объем 
    new G4PVPlacement(rotm,                       //no rotation, так же без вращения
                    G4ThreeVector(0.*cm,0.,10.*cm),//at (0,0,-5 см) положение центра мишени в другую сторону от детектора, смещена на 5 см от центра объема World
                    logicTar,                //its logical volume, подключаем логический объем
                    "Target",                //its name, имя физического объема
                    logicWorld,              //its mother  volume, родительский логический объем!
                    false,                   //no boolean operation, без булевых операций
                    0,                       //copy number, номер копии
                    checkOverlaps);
  // Объявляем слой подложки -----------------------------------------------------------------
  G4Tubs* solidSub =
    new G4Tubs("substrate", 
               0.*cm, 4.9*cm, 0.0025*cm, 0.*deg, 360.*deg);                 
  //Логический объем
  G4LogicalVolume* logicSub =
    new G4LogicalVolume(solidSub,            //its solid, объем
                        tube_mat,             //its material, указываем материал мишени
                        "substrate");         //its name, его имя
  // Физический объем 
    new G4PVPlacement(rotm,                       //no rotation, так же без вращения
                    G4ThreeVector(0.*cm,0.,9.99744*cm),//at (0,0,-5 см) положение центра мишени в другую сторону от детектора, смещена на 5 см от центра объема World
                    logicSub,                //its logical volume, подключаем логический объем
                    "substrate",                //its name, имя физического объема
                    logicWorld,              //its mother  volume, родительский логический объем!
                    false,                   //no boolean operation, без булевых операций
                    0,                       //copy number, номер копии
                    checkOverlaps);
    // Объявляем слой воздух или вакуум -----------------------------------------------------------------
  G4Tubs* solidAir =
    new G4Tubs("cavity", 
               0.*cm, 4.9*cm, 0.5*cm, 0.*deg, 360.*deg);                 
  //Логический объем
  G4LogicalVolume* logicAir =
    new G4LogicalVolume(solidAir,            //its solid, объем
                        vac_mat,             //its material, указываем материал мишени
                        "cavity");         //its name, его имя
  // Физический объем 
    new G4PVPlacement(rotm,                       //no rotation, так же без вращения
                    G4ThreeVector(0.*cm,0.,9.49494*cm),//at (0,0,-5 см) положение центра мишени в другую сторону от детектора, смещена на 5 см от центра объема World
                    logicAir,                //its logical volume, подключаем логический объем
                    "cavity",                //its name, имя физического объема
                    logicWorld,              //its mother  volume, родительский логический объем!
                    false,                   //no boolean operation, без булевых операций
                    0,                       //copy number, номер копии
                    checkOverlaps);
      // Объявляем слой фольга из нерж -----------------------------------------------------------------
  G4Tubs* solidFoil =
    new G4Tubs("thicFoil", 
               0.*cm, 4.9*cm, 0.0025*cm, 0.*deg, 360.*deg);                 
  //Логический объем
  G4LogicalVolume* logicFoil =
    new G4LogicalVolume(solidFoil,            //its solid, объем
                        tube_mat,             //its material, указываем материал мишени
                        "thicFoil");         //its name, его имя
  // Физический объем 
    new G4PVPlacement(rotm,                       //no rotation, так же без вращения
                    G4ThreeVector(0.*cm,0.,8.99244*cm),//at (0,0,-5 см) положение центра мишени в другую сторону от детектора, смещена на 5 см от центра объема World
                    logicFoil,                //its logical volume, подключаем логический объем
                    "thicFoil",                //its name, имя физического объема
                    logicWorld,              //its mother  volume, родительский логический объем!
                    false,                   //no boolean operation, без булевых операций
                    0,                       //copy number, номер копии
                    checkOverlaps);
  //Всегда возвращает физический объем
  return physWorld;
}

void ExG4DetectorConstruction01::ConstructSDandField()
{
  // Объявление чувствительной области детектора, в которой можно получить подробную
  // информацию о состоянии и движении частицы
  // Назовем чувствительную область DetectorSD
     auto sd1 
    = new ExG4DetectorSD("DetectorSD1", "DetectorSD1HitsCollection", fNofLayers);
  //G4String trackerChamberSDname = "DetectorSD1";
  // Создаем экземпляр чувствительной области
  //ExG4DetectorSD* sd1 = new ExG4DetectorSD(trackerChamberSDname);
  // Передаем указатель менеджеру
  G4SDManager::GetSDMpointer()->AddNewDetector(sd1);
  // Добавляем чувствительный объем ко всем логическим областям с
  // именем Detector
  SetSensitiveDetector("Detector1", sd1, true);
    
    auto sd2 
    = new ExG4DetectorSD("DetectorSD2", "DetectorSD2HitsCollection", fNofLayers);
    // G4String trackerChamberSDname = "DetectorSD2";
  // Создаем экземпляр чувствительной области
  //ExG4DetectorSD* sd2 = new ExG4DetectorSD(trackerChamberSDname);
  // Передаем указатель менеджеру
  G4SDManager::GetSDMpointer()->AddNewDetector(sd2);
  // Добавляем чувствительный объем ко всем логическим областям с
  // именем Detector
  SetSensitiveDetector("Detector2", sd2, true);
    
    
    auto sd3 
    = new ExG4DetectorSD("DetectorSD3", "DetectorSD1HitsCollection",fNofLayers);
    //G4String trackerChamberSDname = "DetectorSD3";
  // Создаем экземпляр чувствительной области
  //ExG4DetectorSD* sd3 = new ExG4DetectorSD(trackerChamberSDname);
  // Передаем указатель менеджеру
  G4SDManager::GetSDMpointer()->AddNewDetector(sd3);
  // Добавляем чувствительный объем ко всем логическим областям с
  // именем Detector
  SetSensitiveDetector("Detector3", sd3, true);
    
    auto sd4 
    = new ExG4DetectorSD("DetectorSD4", "DetectorSD1HitsCollection",fNofLayers);
   // G4String trackerChamberSDname = "DetectorSD4";
  // Создаем экземпляр чувствительной области
 // ExG4DetectorSD* sd4 = new ExG4DetectorSD(trackerChamberSDname);
  // Передаем указатель менеджеру
  G4SDManager::GetSDMpointer()->AddNewDetector(sd4);
  // Добавляем чувствительный объем ко всем логическим областям с
  // именем Detector
  SetSensitiveDetector("Detector4", sd4, true);
    
    auto sd5 
    = new ExG4DetectorSD("DetectorSD5", "DetectorSD5HitsCollection", fNofLayers);
  //G4String trackerChamberSDname = "DetectorSD1";
  // Создаем экземпляр чувствительной области
  //ExG4DetectorSD* sd1 = new ExG4DetectorSD(trackerChamberSDname);
  // Передаем указатель менеджеру
  G4SDManager::GetSDMpointer()->AddNewDetector(sd5);
  // Добавляем чувствительный объем ко всем логическим областям с
  // именем Detector
  SetSensitiveDetector("Detector5", sd5, true);
    
    auto sd6 
    = new ExG4DetectorSD("DetectorSD6", "DetectorSD6HitsCollection", fNofLayers);
  //G4String trackerChamberSDname = "DetectorSD1";
  // Создаем экземпляр чувствительной области
  //ExG4DetectorSD* sd1 = new ExG4DetectorSD(trackerChamberSDname);
  // Передаем указатель менеджеру
  G4SDManager::GetSDMpointer()->AddNewDetector(sd6);
  // Добавляем чувствительный объем ко всем логическим областям с
  // именем Detector
  SetSensitiveDetector("Detector6", sd6, true);
    
    auto sd7 
    = new ExG4DetectorSD("DetectorSD7", "DetectorSD7HitsCollection", fNofLayers);
  //G4String trackerChamberSDname = "DetectorSD1";
  // Создаем экземпляр чувствительной области
  //ExG4DetectorSD* sd1 = new ExG4DetectorSD(trackerChamberSDname);
  // Передаем указатель менеджеру
  G4SDManager::GetSDMpointer()->AddNewDetector(sd7);
  // Добавляем чувствительный объем ко всем логическим областям с
  // именем Detector
  SetSensitiveDetector("Detector7", sd7, true);
    
    auto sd8 
    = new ExG4DetectorSD("DetectorSD8", "DetectorSD8HitsCollection", fNofLayers);
  //G4String trackerChamberSDname = "DetectorSD1";
  // Создаем экземпляр чувствительной области
  //ExG4DetectorSD* sd1 = new ExG4DetectorSD(trackerChamberSDname);
  // Передаем указатель менеджеру
  G4SDManager::GetSDMpointer()->AddNewDetector(sd8);
  // Добавляем чувствительный объем ко всем логическим областям с
  // именем Detector
  SetSensitiveDetector("Detector8", sd8, true);

}










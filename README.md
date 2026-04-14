# Remote Control System for Homes

Raspberry Pi tabanli uzaktan denetim sistemi.
Proje, istemci-sunucu mimarisi ile iki farkli sensor node'u uzerinden ev ici cihaz/sensor kontrolu yapar.

## Genel Mimari

Sistem 4 ana bilesenden olusur:

1. ClientNode
2. ServerNode
3. DigitalIONode
4. GyroSensorNode

Port dagilimi:

- ServerNode: 7001
- DigitalIONode: 7002
- GyroSensorNode: 7003

Veri akisi:

1. Kullanici komutu ClientNode uzerinden gonderir.
2. ServerNode komutu ilgili node'a yonlendirir.
3. Node cevabi ServerNode uzerinden ClientNode'a geri iletilir.

## Klasor Yapisi

```text
.
|-- CMakeLists.txt
|-- ClientNode.cpp
|-- ServerNode.cpp
|-- DigitalIONode.cpp
|-- GyroSensorNode.cpp
|-- include/
|   |-- SocketConLib.h
|   |-- GyroLib.h
|   |-- KeypadLib.h
|   |-- DigSensorLib.h
|   `-- RelayLib.h
`-- src/
    |-- CMakeLists.txt
    |-- SocketConLib.cpp
    |-- GyroLib.cpp
    |-- KeypadLib.cpp
    |-- DigSensorLib.cpp
    `-- RelayLib.cpp
```

## Gereksinimler

Temel:

- CMake 3.10+
- C++11 destekli derleyici (g++, clang++)
- POSIX socket destegi

Raspberry Pi icin ek olarak:

- I2C etkinlestirilmis olmasi
- wiringPi (Gyro I2C kodu icin)

## Derleme

### Linux / Raspberry Pi

```bash
mkdir -p build
cd build
cmake ..
make -j
```

Derleme sonrasi ikili dosyalar:

- build/bin/ClientNode
- build/bin/ServerNode
- build/bin/DigitalIONode
- build/bin/GyroSensorNode

## Calistirma

Ayni makinede test (localhost) icin 4 terminal acin.

Terminal 1:

```bash
cd build
./bin/DigitalIONode
```

Terminal 2:

```bash
cd build
./bin/GyroSensorNode
```

Terminal 3:

```bash
cd build
./bin/ServerNode
```

Terminal 4:

```bash
cd build
./bin/ClientNode 127.0.0.1
```

Ag uzerinden calisma icin son komutta 127.0.0.1 yerine ServerNode calisan cihazin IP adresini girin.

## Iletisim Protokolu

### Client -> Server komutlari

Digital IO komutlari:

- sensorDurum:
- sensorTip:
- relay 0:
- relay 1:
- relayDurum:
- key:

Gyro komutlari:

- temp:
- gyro:
- acc:

Sistem komutu:

- kapat:

### Ornek cevap formatlari

- sensorDurum 0:
- sensorTip ISIK:
- relay ok:
- relay 1:
- key 1234:
- temp 25.30:
- gyro 0.10 -0.05 0.00:
- acc 0.00 0.01 9.78:
- kapat ok:

## Notlar

- Proje C++11 ve sade class yapisi ile hazirlanmistir.
- Gyro tarafi Raspberry Pi olmayan ortamlarda simulasyon verisi uretebilir.
- Digital IO tarafinda donanim baglantisi yapilmadan fonksiyonlar test modunda calisabilir.

## Sorun Giderme

Temiz derleme:

```bash
rm -rf build
mkdir build
cd build
cmake ..
make -j
```

Server'a baglanamama:

- IP/port bilgisini kontrol edin.
- Node'larin ayakta oldugunu kontrol edin.
- Guvenlik duvari/izin ayarlarini kontrol edin.

Port cakismasi:

- 7001, 7002, 7003 portlarinin dolu olmadigini kontrol edin.


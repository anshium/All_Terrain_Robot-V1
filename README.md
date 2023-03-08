# All_Terrain_Robot-V1

* LSB->FWD(0)/BKW(1), LSB + 1-> Motion(1)/No-Motion(0), LSB + 2-> Left Turning(0)/Right Turning(1), LSB + 3-> Turning(1)/No-Turning(0)
* Don't-care states are treated as 0
* Nothing: 0000
* FWD No-Turning: 0010
* BKW No-Turning: 0011
* FWD Turning Left: 1010
* FWD Turning Right: 1110
* BKW Turning Left: 1011
* BKW Turning Right: 1111
* Turning Left No Motion: 1000
* Turning Right No Motion: 1100

/*
  * ShaLib.h - Library for generatiing SHA256 digest.
  * Created by Suresh Kumar, March 30, 2021.
*/
#ifndef ShaLib_h
#define ShaLib_h

#include "Arduino.h"

class ShaLib {
    private:

    public:
        ShaLib();
        String getDigest(String serNum, String actCode, String awsMobMac, String randNum, String splRCCode);
        boolean getRanAwsMod(String digest, int& stp, String& ran, String& aws, String& mob);
};

#endif
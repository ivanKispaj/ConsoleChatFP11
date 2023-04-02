//
// EncodePassword.h
// 2023-03-11
// Ivan Konishchev
//
/*
    description :
    use -> #include "EncodePassword.h"
    std::string password = "kjasnfFKJFf983";
    EncodePassword::encodePassword(password);
    further along the code, the password variable will already be encoded
*/

#pragma once
#include <string>

class EncodePassword
{
public:
    /// @brief Password encoding, using a bit shift to the left with the addition of salt
    /// @param pass std::string password
    static void encodePassword(std::string &pass)
    {
        std::string _salt = "7dB/Y2OnzC9A#EfJNFGN3l@OdHx8?*X*N(DP(VTK6#-pF57jeIFe93-R-*kRZ1zy6!uyQ)3W5q#Ht(@y";

        std::string out;
        for (int i = 0; i < pass.size(); i++)
        {
            char symbol = pass[i] << 4;
            out.push_back(symbol);
            out.push_back(_salt[i]);
        }
        pass = out;
    }
};

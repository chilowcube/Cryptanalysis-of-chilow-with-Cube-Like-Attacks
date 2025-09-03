#ifndef __POLYNOMIAL_H__
#define __POLYNOMIAL_H__

#include<vector>
#include<string>
#include<map>
#include<algorithm>
#include<iostream>
#include"split.h"

using std::string;
using std::vector;
using std::map;
using std::cout;
using std::endl;
using std::ostream;

class Polynomial
{
    public: 
        Polynomial();
        Polynomial( const string & s );
        Polynomial( const Polynomial & s )  { _poly = s.getPoly(); }

        bool has_constant() const;

        Polynomial getCoef( const Polynomial & term ) const;
        Polynomial operator+( const Polynomial& poly2 ) const;
        Polynomial operator*( const Polynomial& poly2 ) const;

        Polynomial pickIndex();
        size_t getDegree() const;

        double calBiasNonLinear( const map< string, double > & B, int R, int index = 0 );

        double calBiasLinear( const map<string, double> & B );

        vector<Polynomial> getMonomials() const;
        vector<Polynomial> getVariables() const;
        double calBiasNonLinearQ( const map< string, double > & B );

        int sigma( const Polynomial & xi );

        Polynomial graded_part( size_t deg ) const;
        Polynomial substitution( const Polynomial & poly, const Polynomial & first  ) const;

        string getPoly() const
        {
            return _poly;
        }

        bool operator==( const Polynomial & poly ) const 
        {
            return _poly == poly.getPoly();
        }

    private:
        string _poly;

};

inline ostream & operator<<( ostream & os, const Polynomial & s )  
{
    if ( s.getPoly() == "" )
        os << 0;
    else
        os << s.getPoly();
    return os;
}


#endif

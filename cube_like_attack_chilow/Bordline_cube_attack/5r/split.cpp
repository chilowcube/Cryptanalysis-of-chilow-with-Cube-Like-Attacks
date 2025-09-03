#include <iostream>  
#include <string>  
#include <cstring>  
#include <vector>
#include <boost/algorithm/string.hpp>

using namespace std;  

/*
vector<string> split ( const string & str, const string & delim )
{
    vector<string> res;
    if ( "" == str ) return res;

    char * strs = new char [ str.length() + 1 ];
    strcpy( strs, str.c_str() );

    char * d = new char [ delim.length() + 1 ];
    //strcpy_s( d, sizeof delim.c_str(), delim.c_str() );
    strcpy( d,  delim.c_str() );

    char * p = strtok ( strs, d );
    while ( p )
    {
        string s = p;
        res.push_back( s );
        p = strtok( NULL, d );
    }

    delete [] strs; 
    delete [] d; 

    return res;
}
*/

/*
template <typename Out>
void split(const std::string &s, char delim, Out result) {
    std::istringstream iss(s);
    std::string item;
    while (std::getline(iss, item, delim)) {
        *result++ = item;
    }
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}
*/

vector<string> split( const string & str, const string & delim )
{
    vector<string> res;
    boost::split( res, str, boost::is_any_of( delim ) );
    return res;
}

string join( const vector<string> & vec, const string& delim )
{
    string s = string( "" );
    for ( size_t i = 0; i < vec.size() - 1; i++ )
    {
        s += vec[i];
        s += delim;
    }

    s += vec[ vec.size() - 1 ];
    return s;
}

string sortString( const string & s )
{
    // + 
    auto M = split( s, "+" );
    vector<string> S;
    for ( auto const & m : M )
    {
        auto T = split( m, "*" );

        sort( T.begin(), T.end() );
        auto ss = join( T, "*" );
        S.push_back( ss );
    }

    sort( S.begin(), S.end() );
    return join( S, "+" );
}


/*
int main()
{
    string s = "x0*x1+x2*x3";

    auto res = split( s, "*" );

    for ( auto it : res )
        cout << it << endl;

    string ss = join( res, "*" );
    cout << ss << endl;
}
*/
  

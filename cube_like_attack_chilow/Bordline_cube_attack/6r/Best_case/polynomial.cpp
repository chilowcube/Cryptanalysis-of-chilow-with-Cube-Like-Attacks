#include"polynomial.h"
#include<vector>
#include<string>
#include<set>
#include<cmath>
#include<fstream>
#include<iomanip>
#include<limits>
#include<cstdlib>
#include<typeinfo>
/*#include"HATF.h"*/

using namespace std;

#define DEBUG 0

struct cmp 
{
    bool operator()( const Polynomial & poly1, const Polynomial & poly2 ) const
    {
        return poly1.getPoly() < poly2.getPoly();
    }
};

Polynomial::Polynomial()
{
     _poly = string( "" );
}

Polynomial::Polynomial( const string & s )
{
    if ( s == "0" )
        _poly = string("");
    else
        _poly = s;
}

Polynomial Polynomial::operator+( const Polynomial& poly2 ) const
{
    //cout << "poly1 " << *this << " poly1 " << poly2 << endl;
    if ( poly2._poly == "" )
        return Polynomial( _poly );
    if ( this -> _poly == "" )
        return Polynomial( poly2._poly );

    Polynomial poly( "" );

    vector<string> monomials1 = split( _poly, "+" );
    vector<string> monomials2 = split( poly2._poly, "+" );

    map<string, int> M; 
    for ( auto const & m : monomials1 )
        M[m] ++;
    for ( auto const & m : monomials2 )
        M[m] ++;

    vector<string> res;

    for ( auto const & it : M )
        if ( it.second % 2 == 1 ) 
            res.push_back( it.first );

    if ( res.size() > 0 )
        poly._poly = join( res, "+" );
    else
        poly._poly = string ( "" );

    return poly;
}

Polynomial Polynomial::operator*( const Polynomial& poly2 )  const
{
    if ( poly2._poly == "" )
        return Polynomial( "" );
    if ( _poly == "" )
        return Polynomial( "" );

    Polynomial poly( "" );

    vector<string> monomials1 = split( _poly, "+" );
    vector<string> monomials2 = split( poly2.getPoly(), "+" );

    vector<string> S;

    for ( auto const & m1 : monomials1 )
        for ( auto const & m2 : monomials2 )
        {
            if ( m1 == string( "1" ) )
                S.push_back( m2 );
            else if ( m2 == string( "1" ) )
                S.push_back( m1 );
            else
            {
                vector<string> M1 = split( m1, "*" );
                vector<string> M2 = split( m2, "*" );

                // sort the variables in a monomial
                vector<string> order;

                size_t p1 = 0, p2 = 0;

                while ( p1 < M1.size() && p2 < M2.size() )
                {
                    if ( M1[p1] < M2[p2] )
                    {
                        order.emplace_back( M1[p1] );
                        p1 += 1;
                    }
                    else
                    {
                        order.emplace_back( M2[p2] );
                        p2 += 1;
                    }
                }

                if ( p1 == M1.size() )
                    for ( size_t i = p2; i < M2.size(); i++ )
                        order.emplace_back( M2[i] );
                else
                    for ( size_t i = p1; i < M1.size(); i++ )
                        order.emplace_back( M1[i] );

                vector<string> mol;
                for ( auto const & it : order )
                {
                    if ( count( mol.begin(), mol.end(), it ) == 1 )
                        ;
                    else
                        mol.emplace_back( it );
                }

                S.emplace_back( join( mol, "*" ) );
            }
        }

    map< string , int > MM;
    for ( auto const & it : S )
        MM[it] ++;

    S.clear();
    for ( auto const & it : MM )
        if ( it.second % 2 == 1 )
            S.emplace_back( it.first );

    if ( S.size() > 0 )
        poly._poly = join( S, "+" );
    else
        poly._poly = string( "" );

    return poly;
}

Polynomial Polynomial::getCoef( const Polynomial & term ) const 
{
    if ( term.getPoly() == "" )
    {
        cerr<< "getCoef::Term must not be empty " << _poly << " " << term << endl;
        cerr << __FUNCTION__ << endl;
        exit( EXIT_FAILURE );
    }
    if ( _poly == "" )
        return Polynomial( "" );

    if ( term._poly == "1" )
    {
        vector<string> mols = split( _poly, "+" );

        vector<string> MOL;

        for ( auto const & it : mols )
        {
            if ( it.find( "x" ) == string::npos )
                MOL.push_back( it );
        }

        Polynomial poly ( "" );
        if ( MOL.size() > 0 )
            poly._poly = join( MOL, "+" );
        else
            poly._poly = string( "" );

        return poly;
    }

    /*
    auto V = getVariables();
    if ( find( V.begin(), V.end(), term ) == V.end() )
    {
        return Polynomial( "" );
    }
    */

    vector<string> vars = split( term.getPoly(), "*" );

    vector<string> mols = split( _poly, "+" );

    vector<string> MOL; 
    for ( auto const & it : mols )
    {
        //cout << it << endl;
        vector<string> vv = split( it, "*" );

        bool flag = true;
        for ( auto const & jt : vars )
            if ( find( vv.begin(), vv.end(), jt ) != vv.end() ) 
                ;
            else
                flag = false;

        if ( flag == true )
        {
            vector<string> MOLtwo;

            for ( auto const & kt : vv )
            {
                if ( find( vars.begin(), vars.end(), kt ) == vars.end()  )
                    MOLtwo.push_back( kt );
            }

            if ( MOLtwo.size() > 0 )
            {
                vector<string> tmp;

                bool flagg = true;
                for ( auto const & kt : MOLtwo )
                {
                    if ( kt.find( "x" ) != string::npos )
                        flagg = false;
                    else
                        tmp.push_back( kt );
                }

                if ( flagg )
                    MOL.push_back( join( tmp, "*" ) );
            }  
            else
            {
                MOL.push_back( "1" );
            }
        }
    }

    /*
    for ( auto const & it : MOL )
    {
        auto vec = split( it, "*" );
        vector<string> vecs;
        for ( auto const & v : vec )
            if ( count( vars.begin(), vars.end(), v )  == 0 )
                vecs.push_back( v );

        if ( vecs.size() > 0 )
        {
            vector<string> vecss;
            for ( auto const & vec : vecs )
            {
                if ( vec.find( "x" ) == string::npos )
                    vecss.push_back( vec );
            }

            if ( vecss.size() > 0 )
            {
                string s = join( vecss, "*" );
                MOLwoT.push_back( s );
            }
            else
            {
                MOLwoT.push_back( "1" );
            }
        }
        else
        {
            MOLwoT.push_back( "1" );
        }
    }
    */

    Polynomial poly ( "0" );
    if ( MOL.size() > 0 )
        poly._poly = join( MOL, "+" );
    else
        poly._poly = string( "" );
    return poly;
}

bool Polynomial::has_constant() const
{
    if ( _poly.find ( "+1" ) != string::npos )
    {
        return true;
    }
    else 
    {
        return false;
    }
}

vector<Polynomial> Polynomial::getMonomials() const
{
    vector<Polynomial> M;

    if ( _poly == "" )
        return M;

    vector<string> S = split( _poly, "+" );
    for ( auto const & it : S )
    {
        M.emplace_back( Polynomial( it ) );
    }

    return M;
}


vector<Polynomial> Polynomial::getVariables() const
{
    set<Polynomial, cmp> M;
    vector<string> S = split( _poly, "+" );

    for ( auto const & it : S )
    {
        if ( it != "1" )
        {
            auto SS = split( it, "*" );

            for ( auto const & jt : SS )
                M.emplace( Polynomial( jt ) );
        }
    }

    vector<Polynomial> MM ( M.begin(), M.end() );
    return MM;
}

double Polynomial::calBiasLinear( const map<string, double> & B )
{
    if ( _poly == "" )
        return 0.5;
    else if ( _poly == "1" )
        return -0.5;

    else if ( getDegree() > 1 )
    {
        cout << "calBinasLinear requires degree = 1: " << _poly << endl;
        exit( EXIT_FAILURE );
    }


    auto V = getVariables();

    // bias : pilling-up lemma

    double bias = pow( 2, V.size() - 1 );

    for ( auto const & v : V )
    {
        //cout << "getVariables " << _poly << "  " <<  v << endl;
        if ( B.find( static_cast<string> ( v.getPoly() ) ) != B.end() )
            bias *= B.at( v.getPoly() ); 
        else
        {
            cerr << typeid( v.getPoly() ).name() << endl;

            cerr << "wrong poly : " <<  _poly << endl;
            cerr << v.getPoly() << endl;
            cerr <<  __PRETTY_FUNCTION__ << std::endl;
            for ( auto it : B )
            //    cout << it.first << " " << it.second << endl;
                if ( it.first == v.getPoly() )
                {
                    cerr << it.first << " " << it.second << endl;
                    cerr << typeid( it.first ).name() << " " << typeid( v.getPoly() ).name() << endl;
                }
            
            exit( EXIT_FAILURE );
        }
    }

    if ( has_constant() == true ) 
       return -bias;
    else
        return bias;
}

/*
double Polynomial::calBiasNonlinear( const map<string, double> & B )
{
    auto V = getVariables();

    // bias : pilling-up lemma

    double bias = pow( 2, len(V) - 1 );

    for ( auto const & v : V )
        bias *= B.at(v.getPoly() ); 

    return bias;
}
*/

double Polynomial::calBiasNonLinearQ( const map< string, double > & B )
{
    auto V = getVariables();
    auto dim = V.size();

    //cout << dim << endl;
    //cout << dim << endl;
    if ( dim > 32 )
    {
        cerr << "Dim is too large " << dim << endl;
        exit( EXIT_FAILURE );
    }

    auto M = getMonomials();

    map<string, int> map;

    double bias = 0;

    for ( int x = 0; x < ( 1 << dim); x++ )
    {
        for ( size_t i = 0; i < dim; i++ )
            map[  V[i].getPoly() ] = x >> (dim - 1 - i) & 0x1;

        int res = 0;
        for ( auto const & it : M )
        {
            if ( it.getPoly() == "1" )
                res ^= 1;
            else 
            {
                int mul = 1;
                for ( auto const & jt : it.getVariables() )
                    mul *= map[ jt.getPoly() ];

                res ^= mul;
            }
        }

        if ( res == 0 )
        {
            double pp = 1;
            for ( size_t i = 0; i < dim; i++ )
            {
                if ( map[ V[i].getPoly() ] == 0 )
                    pp *= ( 0.5 + B.at(  V[i].getPoly() ) );
                else
                    pp *= ( 0.5 - B.at(  V[i].getPoly() ) );
            }

            bias += pp;
        }

    }

    return bias - 0.5;
}

double Polynomial::calBiasNonLinear( const map< string, double > & B, int R, int index )
{
#if DEBUG
    auto start = chrono::steady_clock::now();
#endif
    constexpr auto max_precision {std::numeric_limits<long double>::digits10 + 1};

    auto V = getVariables();
    int dim = V.size();

    string cpp = string ( ".__calbias__" ) + to_string( R ) + string( "_" ) + to_string( index ) + string( ".cpp" );
    //string exe = string ( ".__calbias__" ) + to_string( index );
    string exe = string ( ".__calbias__" ) + to_string( R ) + string( "_" ) + to_string( index );
    string cppresult = string ( ".__calbias__result__" ) +  to_string( R ) + string( "_" ) + to_string( index );

    ofstream os;
    os.open( cpp, ios::out );

    os << "#include<iostream>" << endl;
    os << "#include<map>" << endl;
    os << "#include<iomanip>" << endl;
    os << "#include<string>" << endl;
    os << "#include<limits>" << endl;
    os << "using namespace std;" << endl;
    os << "#define BIT " << dim << endl;

    os << "int main() " << endl;
    os << "{" << endl;
    os << "    constexpr auto max_precision {std::numeric_limits<long double>::digits10 + 1};" << endl;

    os << "    double s = 0;" << endl;
    os << "    int y = 0;" << endl;
    os << "    map<string, double> M;" << endl;

    for ( auto const & it : B )
    os << "    M.insert( pair<string, double> ( \"" << it.first << "\", " << 
     fixed << setprecision(max_precision) << it.second <<  " ) );" << endl;    

    for ( int i = 0; i < dim; i++  )
    os << "    int  " << V[i].getPoly() << ";" << endl;        
    os << "    for ( unsigned long i = 0; i < ( 1L << BIT ); i++ ) " << endl; 
    os << "    { " << endl;

    for ( int j = 0; j < dim; j++ )
    os << "         " << V[j].getPoly() << " = i >> ( BIT - 1 - " << j  << " ) & 0x1; " << endl;

    // repalce + to ^;
    auto temp = split( _poly, "+" );
    string newpoly = join( temp, "^" );

    os << "         y = " << newpoly << ";" << endl;
    os << "         if ( y == 0 )" << endl;
    os << "         {" << endl;
    os << "              double p = 1;" << endl;

    for ( int j = 0; j < dim; j++ )
    {
    os << "             if ( " << V[j].getPoly() << " == 0 ) " << endl;
    os << "                 p *= 0.5 + M[ \"" << V[j].getPoly() << "\" ];" << endl;
    os << "             else" << endl;
    os << "                 p *= 0.5 - M[ \"" << V[j].getPoly() << "\" ];" << endl;
    }

    os << "              s += p;" << endl;
    os << "         }" << endl;
    os << "   }" << endl;

    os << "   cout << fixed << setprecision( max_precision ) << ( s - 0.5 ) << endl; "  << endl;  
    os << "}" << endl;

    os.close();
#if DEBUG
    auto writedone = chrono::steady_clock::now();
#endif

    string compile = string( "g++ ") + cpp + string( " -o " ) + exe + string( " -std=c++11 -O3" );

    if( system( compile.c_str() ) != 0 ) 
    {
        cerr << "__calbias__.cpp file generated failed: " << index << endl;
        exit(0);
    }
#if DEBUG
    auto compiledone = chrono::steady_clock::now();
#endif

    string execute = string( "./" ) + exe + string( " > " ) + cppresult;

    if ( system ( execute.c_str() ) != 0 )
    {
        cerr << "__calbias__result__ file generated failed" << endl;
        exit(0);
    }

#if DEBUG
    auto exedone = chrono::steady_clock::now();
#endif

    //string result ( std::ifstream("__calbias__result__").rdbuf() );
    ifstream is;
    is.open( cppresult, ios::in );
    string result;
    getline( is, result );

    is.close();

#if DEBUG
    auto readdone = chrono::steady_clock::now();
    
    cout << "write " << chrono::duration_cast<chrono::milliseconds> ( writedone - start ).count() << endl; 
    cout << "compile " << chrono::duration_cast<chrono::milliseconds> ( compiledone - writedone ).count() << endl; 
    cout << "execute " << chrono::duration_cast<chrono::milliseconds> ( exedone - compiledone ).count() << endl; 
    cout << "read " << chrono::duration_cast<chrono::milliseconds> ( readdone - exedone ).count() << endl; 
    cout << "total " << chrono::duration_cast<chrono::milliseconds> ( readdone - start ).count() << endl; 
#endif


    return stod( result );
}

size_t Polynomial::getDegree() const
{
    if ( _poly == "" )
        return -1;
    if ( _poly == "1" )
        return 0;

    size_t degree = 0;
    
    vector<string> M = split( _poly, "+" );
    for ( auto const & m : M )
    {
        auto deg = split( m, "*" ).size();

        if ( deg > degree )
            degree = deg;
    }

    return degree;
}

Polynomial Polynomial::graded_part( size_t deg ) const
{
    if ( ( deg == 0 ) && has_constant() )
        return Polynomial( "1" );
    
    if ( getDegree() < deg ) 
        return Polynomial( "" );
    else
    {
        vector<string> res;
        vector<string> M = split( _poly, "+" );
        for ( auto const & m : M )
        {
            if ( ( split( m, "*" ).size() == deg ) && ( m != "1" ) )
                res.emplace_back( m );
        }

        if ( res.size() > 0 )
            return Polynomial ( join( res, "+" ) );
        else
            return Polynomial ( "" );
    }

}

int Polynomial::sigma( const Polynomial & xi )
{
    auto qf = graded_part( 2 );
    //cout << qf << qf.getCoef(xi) << endl;
    //return qf.getCoef(xi ).getMonomials().size(); 
    return qf.getCoef(xi).getMonomials().size(); 
}

Polynomial Polynomial::pickIndex()
{
    auto V = getVariables();
    int maxValue = 0;
    string maxTerm = "";
    //Polynomial maxTerm = Polynomial( "" ); 
    
    map<string, int> M;
    auto fq = graded_part(2);

    string fqpoly = fq.getPoly();

    vector<string> S = split( fqpoly, "+" );
    for ( auto const & s : S )
    {
        auto SS = split( s, "*" );
        for ( auto const & ss : SS )
            M[ss] += 1;
    }

    for ( auto const & m : M )
    {
        if ( m.second > maxValue )
        {
            maxValue = m.second;
            maxTerm = m.first;
        }
    }

    return Polynomial( maxTerm );
}

/*
Polynomial Polynomial::pickIndex()
{
    auto V = getVariables();
    int maxValue = 0;
    Polynomial maxTerm = Polynomial( "" ); 

    for ( auto const & v : V )
    {
        int num = sigma( v );
        if ( num > maxValue )
        {
            maxValue = num;
            maxTerm = v;
        }
    }

    return maxTerm;
}
*/

Polynomial Polynomial::substitution( const Polynomial & fu, const Polynomial & first ) const
{
    return getCoef( first ) * ( fu + first ) + *this;
}






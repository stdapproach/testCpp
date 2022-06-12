#ifndef _DLL_TUTORIAL_H_
#define _DLL_TUTORIAL_H_

#if defined DLL_EXPORT
//#define DECLDIR __declspec(dllexport)
#define DECLDIR
#else
//#define DECLDIR __declspec(dllimport)
#define DECLDIR
#endif

#include <iterator>
#include <vector>

namespace hlpr{
template <typename T>
struct vecWrapper
{//based on https://stackoverflow.com/questions/11140057/exposing-stdvector-over-a-dll-boundary-best-practice-for-compiler-independen
    T*  begin;
    T*  end;
};

template <typename T>
std::ostream& operator<< (std::ostream& out, const std::vector<T>& v) {
  if ( !v.empty() ) {
    out << '[';
    std::copy (v.begin(), v.end(), std::ostream_iterator<T>(out, ", "));
    out << "\b\b]";
  }
  return out;
}
}

extern "C"
{
    enum resultSimu {

        ok=0,
        unknown_error=1,
        stopped_internally = 2,
        stopped_by_external_condition = 3,
        stopped_externally = 4
    };
    typedef double fl;

    /*
     * model's default initialization
     */
    DECLDIR resultSimu initDefault();
    /*
     * model's initialization by phase coord.
     */
    DECLDIR resultSimu init(fl t0, hlpr::vecWrapper<fl> wrapper);
   /*
    * perform one step of simulation
    * intgration method selected by model itself
    */
    resultSimu _calcStep(fl dt);
    DECLDIR resultSimu calcStep(fl dt, fl& t, hlpr::vecWrapper<fl>& wrapper);


    DECLDIR int Add( int a, int b );//stub
}

#endif

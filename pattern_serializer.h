#ifndef UTILS_PATTERNSERIALIZER_H
#define UTILS_PATTERNSERIALIZER_H

#include <cstdio>
#include <vector>
#include "firmware/data.h"



class HumanReadablePatternSerializer {
    public:
        HumanReadablePatternSerializer(const std::string &fname, bool zerosuppress=false) ;
        ~HumanReadablePatternSerializer() ;
        
        void dump_puppi(unsigned int N, const char *label, const l1ct::PuppiObj outpuppi[/*N*/]) ;
        bool startframe();
        void endframe();
    protected:
        const std::string fname_;
        bool zerosuppress_;
        FILE *file_; // may be stdout
        unsigned int ipattern_;

    private:
        template<typename TV> 
        void dump_puppi_(unsigned int N, const char *label, const TV outpuppi) ;
    
};

#endif


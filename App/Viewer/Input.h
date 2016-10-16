#pragma once
#include <string>
#include <kvs/CommandLine>
#include <kvs/Indent>


struct Input
{
private:
    kvs::CommandLine m_commandline;

public:

    int tindex; // time index
    int sindex; // slice index
    std::string dirname; // directory name

    Input( int argc, char** argv );
    bool parse();
    void print( std::ostream& os, const kvs::Indent& indent ) const;
};

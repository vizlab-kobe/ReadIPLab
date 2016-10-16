#pragma once
#include <string>
#include <kvs/CommandLine>
#include <kvs/Indent>


struct Input
{
    int tindex; // time index
    int sindex; // slice index
    std::string dirname; // directory name

    Input( const kvs::CommandLine& commandline );
    void read( const kvs::CommandLine& commandline );
    void print( std::ostream& os, const kvs::Indent& indent ) const;
};
